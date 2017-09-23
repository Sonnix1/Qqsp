#include "mainwindow.h"

#include <QSettings>
#include <QApplication>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QCursor>
#include <QPalette>
#include <QFontDialog>
#include <QIcon>

#include "callbacks_gui.h"
#include "comtools.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(600, 450);
    setMinimumSize(240, 180);
    setWindowTitle(QSP_LOGO);
    setWindowIcon(QIcon(":/gfx/logo"));
    setUnifiedTitleAndToolBarOnMac(true);
    setDockNestingEnabled(true);
    setFocusPolicy(Qt::StrongFocus);

    mainMenuBar = new QMenuBar(this);
    setMenuBar(mainMenuBar);
    mainToolBar = new QToolBar(this);
    addToolBar(mainToolBar);
    mainStatusBar = new QStatusBar(this);
    setStatusBar(mainStatusBar);

    // Set QMainWindow in the center of desktop
    //QRect rect = geometry();
    //rect.moveCenter(QApplication::desktop()->availableGeometry().center());
    //setGeometry(rect);

    mainStatusBar->setVisible(false);
    mainToolBar->setVisible(false);
    mainToolBar->setWindowTitle("ToolBar");

    //DragAcceptFiles(true);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    //SetOverallVolume(100);
    m_savedGamePath.clear();
    m_isQuit = false;
    m_keyPressedWhileDisabled = false;
    m_isGameOpened = false;
    showPlainText = false;

    _mainDescTextBox = new QspTextBox(this);
    connect(_mainDescTextBox, SIGNAL(anchorClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
    setCentralWidget(_mainDescTextBox);

    m_linkColor = palette().color(QPalette::Link);
    m_fontColor = palette().color(QPalette::Text);
    m_backColor = palette().color(QPalette::Window);

    m_font = font();
    m_isUseFontSize = false;
    m_fontSize = m_font.pointSize();

    m_imgView = new QspImgCanvas(this);

    CreateDockWindows();
    //LoadSettings();
    CreateMenuBar();

    showCaptions = true;

    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(OnMenu(QAction*)) );

    QSPInit();
    QSPCallBacks::Init(this);
}

MainWindow::~MainWindow()
{

}

void MainWindow::EnableControls(bool status, bool isExtended)
{
    if (isExtended) _fileMenu->setEnabled(status); //TODO: ???
    _fileMenu->setEnabled(status); //TODO: ???
    _gameMenu->setEnabled(status);
    _settingsMenu->setEnabled(status);
    _objectsListBox->setEnabled(status);
    _actionsListBox->setEnabled(status);
    _inputTextBox->setEnabled(status);
    m_isProcessEvents = status;
    m_keyPressedWhileDisabled = false;
}

void MainWindow::ApplyParams()
{
    int numVal;
    QSP_CHAR *strVal;
    QColor setBackColor, setFontColor, setLinkColor;
    int col;
    // --------------
    if(QSPGetVarValues(QSP_FMT("BCOLOR"), 0, &numVal, &strVal) && numVal)
    {
        setBackColor = QColor::fromRgba(numVal);
        col = setBackColor.red();
        setBackColor.setRed(setBackColor.blue());
        setBackColor.setBlue(col);
        ApplyBackColor(setBackColor);
    }
    // --------------
    if(QSPGetVarValues(QSP_FMT("FCOLOR"), 0, &numVal, &strVal) && numVal)
    {
        setFontColor = QColor::fromRgba(numVal);
        col = setFontColor.red();
        setFontColor.setRed(setFontColor.blue());
        setFontColor.setBlue(col);
        ApplyFontColor(setFontColor);
    }
    // --------------
    if(QSPGetVarValues(QSP_FMT("LCOLOR"), 0, &numVal, &strVal) && numVal)
    {
        setLinkColor = QColor::fromRgba(numVal);
        col = setLinkColor.red();
        setLinkColor.setRed(setLinkColor.blue());
        setLinkColor.setBlue(col);
        ApplyLinkColor(setLinkColor);
    }
    // --------------
    QFont new_font = m_font;
    if(QSPGetVarValues(QSP_FMT("FNAME"), 0, &numVal, &strVal))
        if(strVal != 0)
        {
            new_font.setFamily(QSPTools::qspStrToQt(strVal));
        }
    if(!m_isUseFontSize)
    {
        if(QSPGetVarValues(QSP_FMT("FSIZE"), 0, &numVal, &strVal))
            if(numVal != 0)
            {
                new_font.setPointSize(numVal);
            }
    }
    if(new_font != m_font)
    {
        ApplyFont(new_font);
    }
}

void MainWindow::DeleteMenu()
{
    m_menu->clear();
    m_menuItemId = 0;
}

void MainWindow::AddMenuItem(const QString &name, const QString &imgPath)
{
    if (name == QString("-"))
        m_menu->addSeparator();
    else
    {
        bool pixmap_ok = false;
        QPixmap itemPixmap;
        QFileInfo file(imgPath);
        QString itemPath(file.absoluteFilePath());
        if (file.exists() && file.isFile())
        {
            if(itemPixmap.load(itemPath))
                pixmap_ok = true;
        }
        QAction *action;
        if(pixmap_ok)
        {
            action = m_menu->addAction(QIcon(itemPixmap), name);
            //m_menu->addAction(QIcon(itemPixmap), name, this, SLOT(OnMenu(bool)));

        }
        else
        {
            action = m_menu->addAction(name);
            //m_menu->addAction(name, this, SLOT(OnMenu(bool)));
        }
        action->setData(m_menuItemId);
    }
    m_menuItemId++;
}

int MainWindow::ShowMenu()
{
    m_menuIndex = -1;
    m_menu->exec(QCursor::pos());
    return m_menuIndex;
}

void MainWindow::UpdateGamePath(const QString &path)
{
    _mainDescTextBox->SetGamePath(path);
    _descTextBox->SetGamePath(path);
    _actionsListBox->SetGamePath(path);
    _objectsListBox->SetGamePath(path);
}

void MainWindow::ShowError()
{
    bool oldIsProcessEvents;
    QString errorMessage;
    QSP_CHAR *loc;
    int code, actIndex, line;
    if (m_isQuit) return;
    QSPGetLastErrorData(&code, &loc, &actIndex, &line);
    QString desc = QSPTools::qspStrToQt(QSPGetErrorDesc(code));
    if (loc)
        errorMessage = QString("Location: %1\nArea: %2\nLine: %3\nCode: %4\nDesc: %5")
                .arg(QSPTools::qspStrToQt(loc))
                .arg(actIndex < 0 ? QString("on visit") : QString("on action"))
                .arg(line)
                .arg(code)
                .arg(desc);
    else
        errorMessage = QString("Code: %1\nDesc: %2")
                .arg(code)
                .arg(desc);
    QMessageBox dialog(QMessageBox::Critical, tr("Error"), errorMessage, QMessageBox::Ok, this);
    oldIsProcessEvents = m_isProcessEvents;
    m_isProcessEvents = false;
    dialog.exec();
    m_isProcessEvents = oldIsProcessEvents;
    if (m_isGameOpened) QSPCallBacks::RefreshInt(QSP_FALSE);
}

void MainWindow::SetShowPlainText(bool isPlain)
{
    showPlainText = isPlain;
    _mainDescTextBox->SetShowPlainText(showPlainText);
    _descTextBox->SetShowPlainText(showPlainText);
    _actionsListBox->SetShowPlainText(showPlainText);
    _objectsListBox->SetShowPlainText(showPlainText);
}

void MainWindow::RefreshUI()
{
    _mainDescTextBox->RefreshUI();
    _objectsListBox->RefreshUI();
    _actionsListBox->RefreshUI();
    _descTextBox->RefreshUI();
    //m_input->Refresh();
    m_imgView->RefreshUI();
}

void MainWindow::ApplyFont(const QFont &new_font)
{
    m_font = new_font;
    _mainDescTextBox->SetTextFont(new_font);
    _descTextBox->SetTextFont(new_font);
    _objectsListBox->SetTextFont(new_font);
    _actionsListBox->SetTextFont(new_font);
}

bool MainWindow::ApplyFontColor(const QColor &color)
{
    m_fontColor = color;
    _mainDescTextBox->SetForegroundColor(color);
    _descTextBox->SetForegroundColor(color);
    _objectsListBox->SetForegroundColor(color);
    _actionsListBox->SetForegroundColor(color);
    return false;
}

bool MainWindow::ApplyBackColor(const QColor &color)
{
    m_backColor = color;
    QPalette p = palette();
    p.setColor(QPalette::Base, color);
    setPalette(p);
    _mainDescTextBox->SetBackgroundColor(color);
    _descTextBox->SetBackgroundColor(color);
    _objectsListBox->SetBackgroundColor(color);
    _actionsListBox->SetBackgroundColor(color);
    return false;
}

bool MainWindow::ApplyLinkColor(const QColor &color)
{
    m_linkColor = color;
    _mainDescTextBox->SetLinkColor(color);
    _descTextBox->SetLinkColor(color);
    _objectsListBox->SetLinkColor(color);
    _actionsListBox->SetLinkColor(color);
    return false;
}

void MainWindow::LoadSettings()
{
    QSettings settings(QApplication::applicationDirPath() + "/config.cfg", QSettings::IniFormat);

    restoreGeometry(settings.value("mainWindow/geometry").toByteArray());
    restoreState(settings.value("mainWindow/windowState").toByteArray());

    if (settings.value("mainWindow/isMaximized", false).toBool())
        showMaximized();
    if (settings.value("mainWindow/isFullScreen", false).toBool())
        showFullScreen();

    SetShowPlainText(settings.value("application/isShowPlainText", false).toBool());
    OnToggleCaptions(settings.value("application/showCaptions", true).toBool());

//    cfg.Read(wxT("Colors/BackColor"), &temp, 0xE0E0E0);
//	m_backColor = wxColour(temp);
//	cfg.Read(wxT("Colors/FontColor"), &temp, 0x000000);
//	m_fontColor = wxColour(temp);
//	cfg.Read(wxT("Colors/LinkColor"), &temp, 0xFF0000);
//	m_linkColor = wxColour(temp);
//	temp = wxNORMAL_FONT->GetPointSize();
//	if (temp < 12) temp = 12;
//	cfg.Read(wxT("Font/FontSize"), &m_fontSize, temp);
//	cfg.Read(wxT("Font/FontName"), &m_fontName, wxNORMAL_FONT->GetFaceName());
//	cfg.Read(wxT("Font/UseFontSize"), &m_isUseFontSize, false);
//	cfg.Read(wxT("General/ShowHotkeys"), &m_isShowHotkeys, false);
//	cfg.Read(wxT("General/Volume"), &m_volume, 100);
//	cfg.Read(wxT("Pos/Left"), &x, 10);
//	cfg.Read(wxT("Pos/Top"), &y, 10);
//	cfg.Read(wxT("Pos/Width"), &w, 850);
//	cfg.Read(wxT("Pos/Height"), &h, 650);
//	cfg.Read(wxT("Pos/Maximize"), &isMaximize, false);
//	wxString panels(wxT("layout2|") \
//		wxT("name=imgview;state=1080035327;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=832;besth=150;minw=50;minh=50;maxw=-1;maxh=-1;floatx=175;floaty=148;floatw=518;floath=372|") \
//		wxT("name=desc;state=768;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=613;besth=341;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|") \
//		wxT("name=objs;state=6293500;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=213;besth=324;minw=50;minh=50;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|") \
//		wxT("name=acts;state=6293500;dir=3;layer=0;row=0;pos=0;prop=117349;bestw=475;besth=185;minw=50;minh=50;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|") \
//		wxT("name=vars;state=6293500;dir=3;layer=0;row=0;pos=1;prop=82651;bestw=351;besth=185;minw=50;minh=50;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|") \
//		wxT("name=input;state=2099196;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=832;besth=22;minw=50;minh=20;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|") \
//		wxT("dock_size(5,0,0)=22|dock_size(2,0,0)=215|dock_size(3,0,0)=204|dock_size(3,1,0)=41|"));
//	cfg.Read(wxT("General/Panels"), &panels);
//	m_transhelper->Load(cfg, wxT("General/Language"));
//	// -------------------------------------------------
//	SetOverallVolume(m_volume);
//	ApplyBackColor(m_backColor);
//	ApplyFontColor(m_fontColor);
//	ApplyLinkColor(m_linkColor);
//	ApplyFontSize(m_fontSize);
//	if (!ApplyFontName(m_fontName))
//	{
//		m_fontName = wxNORMAL_FONT->GetFaceName();
//		ApplyFontName(m_fontName);
//	}
    RefreshUI();
//	m_settingsMenu->Check(ID_USEFONTSIZE, m_isUseFontSize);
//	m_manager->LoadPerspective(panels);
//	m_manager->RestoreMaximizedPane();
//	// Check for correct position
//	wxSize winSize(ClientToWindowSize(wxSize(w, h)));
//	w = winSize.GetWidth();
//	h = winSize.GetHeight();
//	wxRect dispRect(wxGetClientDisplayRect());
//	if (w > dispRect.GetWidth()) w = dispRect.GetWidth();
//	if (h > dispRect.GetHeight()) h = dispRect.GetHeight();
//	if (x < dispRect.GetLeft()) x = dispRect.GetLeft();
//	if (y < dispRect.GetTop()) y = dispRect.GetTop();
//	if (x + w - 1 > dispRect.GetRight()) x = dispRect.GetRight() - w + 1;
//	if (y + h - 1 > dispRect.GetBottom()) y = dispRect.GetBottom() - h + 1;
//	// --------------------------
//	SetSize(x, y, w, h);
//	ShowPane(ID_VIEWPIC, false);
//	ShowPane(ID_ACTIONS, true);
//	ShowPane(ID_OBJECTS, true);
//	ShowPane(ID_VARSDESC, true);
//	ShowPane(ID_INPUT, true);
//	ReCreateGUI();
//	if (isMaximize) Maximize();
//	Show();
//	m_manager->Update();
}

void MainWindow::SaveSettings()
{
    QSettings settings(QApplication::applicationDirPath() + "/config.cfg", QSettings::IniFormat);

    bool maximized = isMaximized();
    if (maximized) showNormal();

    bool fullscreen = isFullScreen();
    if (fullscreen) showNormal();

    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/windowState", saveState());
    settings.setValue("mainWindow/isMaximized", maximized);
    settings.setValue("mainWindow/isFullScreen", fullscreen);
    settings.setValue("mainWindow/showCaptions", showCaptions);

    settings.sync();


//    cfg.Write(wxT("Colors/BackColor"), m_backColor.Blue() << 16 | m_backColor.Green() << 8 | m_backColor.Red());
//	cfg.Write(wxT("Colors/FontColor"), m_fontColor.Blue() << 16 | m_fontColor.Green() << 8 | m_fontColor.Red());
//	cfg.Write(wxT("Colors/LinkColor"), m_linkColor.Blue() << 16 | m_linkColor.Green() << 8 | m_linkColor.Red());
//	cfg.Write(wxT("Font/FontSize"), m_fontSize);
//	cfg.Write(wxT("Font/FontName"), m_fontName);
//	cfg.Write(wxT("Font/UseFontSize"), m_isUseFontSize);
//	cfg.Write(wxT("General/Volume"), m_volume);
//	cfg.Write(wxT("General/ShowHotkeys"), m_isShowHotkeys);
//	cfg.Write(wxT("General/Panels"), m_manager->SavePerspective());
//	m_transhelper->Save(cfg, wxT("General/Language"));
//	GetPosition(&x, &y);
//	GetClientSize(&w, &h);
//	cfg.Write(wxT("Pos/Left"), x);
//	cfg.Write(wxT("Pos/Top"), y);
//	cfg.Write(wxT("Pos/Width"), w);
//	cfg.Write(wxT("Pos/Height"), h);
//	cfg.Write(wxT("Pos/Maximize"), isMaximized);
}

void MainWindow::CreateMenuBar()
{
    QAction* action;
    //------------------------------------------------------------------
    // File menu
    _fileMenu = menuBar()->addMenu(tr("&Quest"));

    // Open item
    action = _fileMenu->addAction(QIcon(":/gfx/menu/open"), tr("Open game..."),
        this, SLOT(OnOpenGame()), QKeySequence(Qt::ALT + Qt::Key_O));
    mainToolBar->addAction(action);

    // New game item
    action = _fileMenu->addAction(QIcon(":/gfx/menu/new"),tr("Restart game"),
        this, SLOT(OnRestartGame()), QKeySequence(Qt::ALT + Qt::Key_N));
    mainToolBar->addAction(action);

    _fileMenu->addSeparator();
    mainToolBar->addSeparator();

    // Exit item
    action = _fileMenu->addAction(QIcon(":/gfx/menu/exit"), tr("Exit"),
        this, SLOT(close()), QKeySequence(Qt::ALT + Qt::Key_X));
    mainToolBar->addAction(action);
    //------------------------------------------------------------------
    mainToolBar->addSeparator();
    // Game menu
    _gameMenu = menuBar()->addMenu(tr("&Game"));

    // Open saved game item
    action =  _gameMenu->addAction(QIcon(":/gfx/menu/statusopen"), tr("Open saved game..."),
        this, SLOT(OnOpenSavedGame()), QKeySequence(Qt::CTRL + Qt::Key_O));
    mainToolBar->addAction(action);
    // Save game item
    action =  _gameMenu->addAction(QIcon(":/gfx/menu/statussave"), tr("Save game..."),
        this, SLOT(OnSaveGame()), QKeySequence(Qt::CTRL + Qt::Key_S));
    mainToolBar->addAction(action);
    //------------------------------------------------------------------
    mainToolBar->addSeparator();
    // Settings menu
    _settingsMenu = menuBar()->addMenu(tr("&Settings"));

    // Show / Hide submenu
    _showHideMenu = _settingsMenu->addMenu(tr("Show / Hide"));

    // Objects item
    action = _objectsWidget->toggleViewAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    _showHideMenu->addAction(action);

    // Actions item
    action = _actionsWidget->toggleViewAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    _showHideMenu->addAction(action);

    // Additional desc item
    action = _descWidget->toggleViewAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    _showHideMenu->addAction(action);

    // Input area item
    action = _inputWidget->toggleViewAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_4));
    _showHideMenu->addAction(action);

    _showHideMenu->addSeparator();

    // Captions item
    action = _showHideMenu->addAction(tr("Captions"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_5));
    action->setCheckable(true);
    if(_objectsWidget->titleBarWidget() == 0)
        action->setChecked(true);
    else
        action->setChecked(false);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(OnToggleCaptions(bool)));

    // ToolBar
    action = mainToolBar->toggleViewAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_6));
    _showHideMenu->addAction(action);

    // Hotkeys for actions item
    //_showHideMenu->addAction(tr("Hotkeys for actions"), this, SLOT(OnToggleHotkeys()),
    //    QKeySequence(Qt::CTRL + Qt::Key_6));

    // Sound volume item
    _settingsMenu->addAction(tr("Sound volume..."),
        this, SLOT(OnChangeSoundVolume()), QKeySequence(Qt::ALT + Qt::Key_V));

    // Window / Fullscreen mode item
    _settingsMenu->addAction(QIcon(":/gfx/menu/windowmode"), tr("Window / Fullscreen mode"),
        this, SLOT(OnToggleWinMode()), QKeySequence(Qt::Key_F11));

    _settingsMenu->addSeparator();

    // Display HTML code as plain text
    action = _settingsMenu->addAction(tr("Display HTML code as plain text"));
    action->setShortcut(QKeySequence(Qt::ALT + Qt::Key_D));
    action->setCheckable(true);
    action->setChecked(showPlainText);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(OnToggleShowPlainText(bool)));
//    _settingsMenu->addAction(tr("Display HTML code as plain text"),
//        this, SLOT(OnToggleShowPlainText()), QKeySequence(Qt::ALT + Qt::Key_D))->setCheckable(true);

    _settingsMenu->addSeparator();

    // Options item
    action =  _settingsMenu->addAction(tr("Options..."),
        this, SLOT(OnOptions()), QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_O));
    //mainToolBar->addAction(action);
    //------------------------------------------------------------------
    //mainToolBar->addSeparator();
    // Help menu
    QMenu* helpMenu(menuBar()->addMenu(tr("&Help")));

    // About item
    action =  helpMenu->addAction(QIcon(":/gfx/menu/about"), tr("About..."),
        this, SLOT(OnAbout()), QKeySequence(Qt::CTRL + Qt::Key_H));
    mainToolBar->addAction(action);
}

void MainWindow::CreateDockWindows()
{
    // "Objects" widget
    _objectsWidget = new QDockWidget(tr("Objects"), this);
    addDockWidget(Qt::RightDockWidgetArea, _objectsWidget, Qt::Vertical);
    _objectsListBox = new QspListBox(this);
    connect(_objectsListBox, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(OnObjectListBoxItemClicked(QListWidgetItem *)));
    connect(_objectsListBox, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(OnObjectListBoxItemClicked(QListWidgetItem *)));
    connect(_objectsListBox, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(OnObjectListBoxItemClicked(QListWidgetItem *)));
    //connect(_objectsListBox, SIGNAL(currentRowChanged(int)), this, SLOT(OnObjectChange(int)));
    _objectsWidget->setWidget(_objectsListBox);

    // "Actions" widget
    _actionsWidget = new QDockWidget(tr("Actions"), this);
    addDockWidget(Qt::BottomDockWidgetArea, _actionsWidget, Qt::Vertical);
    _actionsListBox = new QspListBox(this);
    connect(_actionsListBox, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(OnActionsListBoxItemClicked(QListWidgetItem *)));
    connect(_actionsListBox, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(OnActionsListBoxItemClicked(QListWidgetItem *)));
    connect(_actionsListBox, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(OnActionsListBoxItemClicked(QListWidgetItem *)));
    //connect(_actionsListBox, SIGNAL(currentRowChanged(int)), this, SLOT(OnActionChange(int)));
    _actionsWidget->setWidget(_actionsListBox);

    // "Additional desc" widget
    _descWidget = new QDockWidget(tr("Additional desc"), this);
    addDockWidget(Qt::BottomDockWidgetArea, _descWidget, Qt::Horizontal);
    _descTextBox = new QspTextBox(this);
    connect(_descTextBox, SIGNAL(anchorClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
    _descWidget->setWidget(_descTextBox);

    // "Input area" widget
    _inputWidget = new QDockWidget(tr("Input area"), this);
    addDockWidget(Qt::BottomDockWidgetArea, _inputWidget, Qt::Vertical);
    _inputTextBox = new QspInputBox(this);
    _inputWidget->setWidget(_inputTextBox);
    connect(_inputTextBox, SIGNAL(textChanged()), this, SLOT(OnInputTextChange()));
    connect(_inputTextBox, SIGNAL(InputTextEnter()), this, SLOT(OnInputTextEnter()));

    splitDockWidget(_actionsWidget, _inputWidget, Qt::Vertical);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //SaveSettings();
    EnableControls(false, true);
    setVisible(false);
    m_isQuit = true;

    QSPDeInit();
    QSPCallBacks::DeInit();

    QCoreApplication::processEvents();
    QMainWindow::closeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int action = -1;
    if(event->key() == Qt::Key_1)
        if(_actionsListBox->count() >= 1)
            action = 0;
    if(event->key() == Qt::Key_2)
        if(_actionsListBox->count() >= 2)
            action = 1;
    if(event->key() == Qt::Key_3)
        if(_actionsListBox->count() >= 3)
            action = 2;
    if(event->key() == Qt::Key_4)
        if(_actionsListBox->count() >= 4)
            action = 3;
    if(event->key() == Qt::Key_5)
        if(_actionsListBox->count() >= 5)
            action = 4;
    if(event->key() == Qt::Key_6)
        if(_actionsListBox->count() >= 6)
            action = 5;
    if(event->key() == Qt::Key_7)
        if(_actionsListBox->count() >= 7)
            action = 6;
    if(event->key() == Qt::Key_8)
        if(_actionsListBox->count() >= 8)
            action = 7;
    if(event->key() == Qt::Key_9)
        if(_actionsListBox->count() >= 9)
            action = 8;
    if(event->key() == Qt::Key_0)
        if(_actionsListBox->count() >= 10)
            action = 9;
    if(action != -1)
    {
        if (!QSPSetSelActionIndex(action, QSP_TRUE))
            ShowError();
        if (!QSPExecuteSelActionCode(QSP_TRUE))
            ShowError();
        return;
    }
    if(event->key() == Qt::Key_Escape)
        if(isFullScreen())
            showNormal();
    QMainWindow::keyPressEvent(event);
}

void MainWindow::OpenGameFile(const QString &path)
{
    if (QSPLoadGameWorld(qspStringFromQString(path)))
    {
        m_isGameOpened = true;
        QFileInfo file(path);
        QString filePath(file.canonicalPath());
        if(!filePath.endsWith('/')) filePath+="/";
        QString configString(filePath + QSP_CONFIG);
        QFileInfo configStringFile(configString);
        QString newPath((configStringFile.exists() && configStringFile.isFile()) ? configString : m_configDefPath);
        if (newPath != m_configPath)
        {
            SaveSettings();
            m_configPath = newPath;
            LoadSettings();
        }
        UpdateGamePath(filePath);
        OnNewGame();
        if (m_isQuit) return;
        //UpdateTitle();
        EnableControls(true);
        m_savedGamePath.clear();
        ApplyParams();
    }
    else
        ShowError();
}

void MainWindow::OnOpenGame()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select game file"), GetLastPath(), tr("QSP games (*.qsp *.gam)"));
    if (!path.isEmpty())
    {
        SetLastPath(QFileInfo(path).canonicalPath());
        OpenGameFile(path);
    }
}

void MainWindow::OnRestartGame()
{
    if(m_isGameOpened)
        if (!QSPRestartGame(QSP_TRUE))
            ShowError();
        else
            ApplyParams();
}

void MainWindow::OnOpenSavedGame()
{
    if(!m_isGameOpened)
        return;
    QString path = QFileDialog::getOpenFileName(this, tr("Select saved game file"), GetLastPath(), tr("Saved game files (*.sav)"));
    if (!path.isEmpty())
    {
        SetLastPath(QFileInfo(path).canonicalPath());
        if (!QSPOpenSavedGame(qspStringFromQString(path), QSP_TRUE))
            ShowError();
        else
            ApplyParams();
    }
}

void MainWindow::OnSaveGame()
{
    if(!m_isGameOpened)
        return;
    QString path = QFileDialog::getSaveFileName(this, tr("Select file to save"), GetLastPath(), tr("Saved game files (*.sav)"));
    if (!path.isEmpty())
    {
        if(!path.endsWith(".sav"))
            path.append(".sav");
        QString p = GetLastPath();
        if (QSPSaveGame(qspStringFromQString(path), QSP_TRUE))
        {
            SetLastPath(QFileInfo(path).canonicalPath());
            m_savedGamePath = path;
        }
        else
            ShowError();
    }
}

void MainWindow::OnAbout()
{
    QPixmap icon = QPixmap(":/gfx/logo");
    icon = icon.scaledToHeight(64, Qt::SmoothTransformation);
    QString version(QSPTools::qspStrToQt(QSPGetVersion()));
    QString libCompiledDate(QSPTools::qspStrToQt(QSPGetCompiledDateTime()));
    QString guiCompiledDate(tr(__DATE__) + tr(", ") + tr(__TIME__));
    QString text = (tr("<h2>Qqsp</h2>"
        "<p>Copyright &copy; 2017, S.</p>"));
    text += tr("<p>Application version: %1<br>QSP library version: %2<br>Application compilation date: %3<br>Library compilation date: %4</p>").arg(QApplication::applicationVersion(), version, guiCompiledDate, libCompiledDate);
    QMessageBox dlg(QMessageBox::NoIcon, tr("About"), text, QMessageBox::Ok);
    dlg.setIconPixmap(icon);
    dlg.exec();
}

void MainWindow::OnToggleCaptions(bool checked)
{
    showCaptions = checked;
    QWidget* objectsTitleBarWidget = _objectsWidget->titleBarWidget();
    QWidget* actionsTitleBarWidget = _actionsWidget->titleBarWidget();
    QWidget* descTitleBarWidget = _descWidget->titleBarWidget();
    QWidget* inputTitleBarWidget = _inputWidget->titleBarWidget();
    if(checked == false)
    {
        _objectsWidget->setTitleBarWidget(new QWidget(_objectsWidget));
        _objectsWidget->titleBarWidget()->hide();
        _actionsWidget->setTitleBarWidget(new QWidget(_actionsWidget));
        _actionsWidget->titleBarWidget()->hide();
        _descWidget->setTitleBarWidget(new QWidget(_descWidget));
        _descWidget->titleBarWidget()->hide();
        _inputWidget->setTitleBarWidget(new QWidget(_inputWidget));
        _inputWidget->titleBarWidget()->hide();
    }
    else
    {
        _objectsWidget->setTitleBarWidget(0);
        _actionsWidget->setTitleBarWidget(0);
        _descWidget->setTitleBarWidget(0);
        _inputWidget->setTitleBarWidget(0);
    }
    if(objectsTitleBarWidget)
        delete objectsTitleBarWidget;
    if(actionsTitleBarWidget)
        delete actionsTitleBarWidget;
    if(descTitleBarWidget)
        delete descTitleBarWidget;
    if(inputTitleBarWidget)
        delete inputTitleBarWidget;
}

void MainWindow::OnToggleWinMode()
{
    if(isFullScreen())
    {
        showNormal();
    }
    else
    {
        showFullScreen();
    }
}

void MainWindow::OnToggleShowPlainText(bool checked)
{
    SetShowPlainText(checked);
}

void MainWindow::OnNewGame()
{
    if (!QSPRestartGame(QSP_TRUE))
        ShowError();
}

void MainWindow::OnTimer()
{
    if (m_isProcessEvents && !QSPExecCounter(QSP_TRUE))
        ShowError();
}

void MainWindow::OnLinkClicked(const QUrl &url)
{
    QString href;
    href = QByteArray::fromPercentEncoding(url.toString().toUtf8());

    if (href.startsWith("#"))
    {
        QObject* obj = sender();
        if (obj == _mainDescTextBox)
            _mainDescTextBox->setSource(url);
        else
            _descTextBox->setSource(url);
    }
    else if (href.startsWith("EXEC:", Qt::CaseInsensitive))
    {
        QString string = href.mid(5);
        if (m_isProcessEvents && !QSPExecString(qspStringFromQString(string), QSP_TRUE))
            ShowError();
    }
    else
    {
        //wxLaunchDefaultBrowser(href); //TODO: this
    }
}

void MainWindow::OnObjectListBoxItemClicked(QListWidgetItem *itemClicked)
{
    int object = _objectsListBox->row(itemClicked);
    if (!QSPSetSelObjectIndex(object, QSP_TRUE))
        ShowError();
}

void MainWindow::OnActionsListBoxItemClicked(QListWidgetItem *itemClicked)
{
    int action = _actionsListBox->row(itemClicked);
    if (!QSPSetSelActionIndex(action, QSP_TRUE))
        ShowError();
    if (!QSPExecuteSelActionCode(QSP_TRUE))
        ShowError();
}

void MainWindow::OnObjectChange(int currentRow)
{
    //QThread::msleep(20);
    if (!QSPSetSelObjectIndex(currentRow, QSP_TRUE))
        ShowError();
}

void MainWindow::OnActionChange(int currentRow)
{
    if (!QSPSetSelActionIndex(currentRow, QSP_TRUE))
        ShowError();
}

void MainWindow::OnMenu(QAction* action)
{
    m_menuIndex = action->data().toInt();
}

void MainWindow::OnInputTextChange()
{
    QSPSetInputStrText(qspStringFromQString(_inputTextBox->GetText()));
}

void MainWindow::OnInputTextEnter()
{
    QSPSetInputStrText(qspStringFromQString(_inputTextBox->GetText()));
    if (!QSPExecUserInput(QSP_TRUE))
        ShowError();
}
