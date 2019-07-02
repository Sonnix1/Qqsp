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
#include <QDesktopServices>
#include <QLocale>
#include <QInputDialog>
#include <QMimeData>
#include <QDesktopWidget>

#include "callbacks_gui.h"
#include "comtools.h"

#include "optionsdialog.h"

#ifdef _ANDROIDQT
#include <QStandardPaths>
#include "androidfiledialog.h"
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(600, 450);
    setMinimumSize(240, 180);
    setWindowTitle(QSP_LOGO);
    setWindowIcon(QIcon(":/gfx/logo"));
    setUnifiedTitleAndToolBarOnMac(true);
    setDockNestingEnabled(true);
    setFocusPolicy(Qt::StrongFocus);
    setObjectName(QStringLiteral("MainWindow"));
    setAcceptDrops(true);

#ifdef _WEBBOX
//    qwuri = new QspWebEngineUrlRequestInterceptor();
//    QWebEngineProfile::defaultProfile()->setRequestInterceptor(qwuri);
//    QspWebEngineUrlSchemeHandler *qweush = new QspWebEngineUrlSchemeHandler();
//    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(QByteArray("qsp"),qweush);
#endif

    m_palette = palette();

    mainMenuBar = new QMenuBar(this);
    setMenuBar(mainMenuBar);
    mainMenuBar->setObjectName(QStringLiteral("mainMenuBar"));
    mainMenuBar->setVisible(true);
    mainToolBar = new QToolBar(this);
    mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
    addToolBar(mainToolBar);
    //mainStatusBar = new QStatusBar(this);
    //mainStatusBar->setObjectName(QStringLiteral("mainStatusBar"));
    //setStatusBar(mainStatusBar);

    // Set QMainWindow in the center of desktop
    //QRect rect = geometry();
    //rect.moveCenter(QApplication::desktop()->availableGeometry().center());
    //setGeometry(rect);

    //mainStatusBar->setVisible(false);
    mainToolBar->setVisible(false);
    mainToolBar->setWindowTitle(tr("ToolBar"));

    m_timer = new QTimer(this);
    m_timer->setObjectName(QStringLiteral("m_timer"));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    m_savedGamePath.clear();
    m_isQuit = false;
    m_keyPressedWhileDisabled = false;
    m_isGameOpened = false;
    showPlainText = false;

    //setCentralWidget(_mainDescWidget);

    m_linkColor = palette().color(QPalette::Link);
    m_fontColor = palette().color(QPalette::Text);
    m_backColor = QColor(224, 224, 224);
    m_isUseBackColor = false;
    m_isUseLinkColor = false;
    m_isUseFontColor = false;
    m_defaultBackColor = m_backColor;
    m_defaultLinkColor = m_linkColor;
    m_defaultFontColor = m_fontColor;

    m_font = QFont( "Sans", 12 , QFont::Normal );
    m_font.setStyle(QFont::StyleNormal);
    m_font.setStyleHint( QFont::SansSerif );
    m_defaultFont = m_font;
    m_isUseFontSize = false;
    m_isUseFont = false;
    m_fontSize = 12;

    showCaptions = true;

    m_isShowHotkeys = false;

    m_volume = 1.0f;

    disableVideo = false;
    m_videoFix = false;

    perGameConfig = true;
    autostartLastGame = false;

    m_isAllowHTML5Extras = true;

    langid = QObject::tr("__LANGID__");
    if(langid == QStringLiteral("__LANGID__"))
        langid = QLocale::system().name();

    CreateDockWindows();

    ApplyBackColor(m_backColor);
    ApplyFontColor(m_fontColor);
    ApplyLinkColor(m_linkColor);
    ApplyFont(m_font, 0 , 0);

    QFileInfo settingsFile(QApplication::applicationDirPath() + "/" + QSP_CUSTOM_CONFIG);
    if(settingsFile.exists() && settingsFile.isFile())
    {
        LoadSettings(QApplication::applicationDirPath() + "/" + QSP_CUSTOM_CONFIG);
    }
    else
    {
        LoadSettings();
    }
    CreateMenuBar();

    m_menu = new QMenu(this);
    m_menu->setObjectName(QStringLiteral("m_menu"));
    connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(OnMenu(QAction*)) );

    QSPInit();
    QSPCallBacks::Init(this);
    QSPCallBacks::SetAllowHTML5Extras(m_isAllowHTML5Extras);
    SetOverallVolume(m_volume);

    QFileInfo gameFile(QApplication::applicationDirPath() + "/standalone_content/" + QSP_GAME);
    if(gameFile.exists() && gameFile.isFile())
    {
        OpenGameFile(QApplication::applicationDirPath() + "/standalone_content/" + QSP_GAME);
    }
    else
    {
        if(autostartLastGame)
            OpenGameFile(lastGame);
    }
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
    setPalette(m_palette);
    // --------------
    if(!m_isUseBackColor)
    {
        if(QSPGetVarValues(QSP_FMT("BCOLOR"), 0, &numVal, &strVal))
        {
            if(numVal == 0)
                setBackColor = m_defaultBackColor;
            else
                setBackColor = QSPTools::wxtoQColor(numVal);
        }
        else
            setBackColor = m_defaultBackColor;
    }
    else
    {
        setBackColor = m_settingsBackColor;
    }
    ApplyBackColor(setBackColor);
    // --------------
    if(!m_isUseFontColor)
    {
        if(QSPGetVarValues(QSP_FMT("FCOLOR"), 0, &numVal, &strVal))
        {
            if(numVal == 0)
                setFontColor = m_defaultFontColor;
            else
                setFontColor = QSPTools::wxtoQColor(numVal);
        }
        else
            setFontColor = m_defaultFontColor;
    }
    else
    {
        setFontColor = m_settingsFontColor;
    }
    ApplyFontColor(setFontColor);
    // --------------
    if(!m_isUseLinkColor)
    {
        if(QSPGetVarValues(QSP_FMT("LCOLOR"), 0, &numVal, &strVal))
        {
            if(numVal == 0)
                setLinkColor = m_defaultLinkColor;
            else
                setLinkColor = QSPTools::wxtoQColor(numVal);
        }
        else
            setLinkColor = m_defaultLinkColor;
    }
    else
    {
        setLinkColor = m_settingsLinkColor;
    }
    ApplyLinkColor(setLinkColor);
    // --------------
    QFont new_font = m_defaultFont;
    int fontType = 0;
    int sizeType = 0;
    if(!m_isUseFont)
    {
        if(QSPGetVarValues(QSP_FMT("FNAME"), 0, &numVal, &strVal))
        {
            if(strVal != 0)
            {
                if(!QSPTools::qspStrToQt(strVal).isEmpty())
                {
                    new_font.setFamily(QSPTools::qspStrToQt(strVal));
                    fontType = 1;
                }
            }
        }
        if(!m_isUseFontSize)
        {
            if(QSPGetVarValues(QSP_FMT("FSIZE"), 0, &numVal, &strVal))
            {
                if(numVal != 0)
                {
                    new_font.setPointSize(numVal);
                    sizeType = 1;
                }
            }
        }
        else
        {
            new_font.setPointSize(m_fontSize);
            sizeType = 2;
        }
    }
    else
    {
        new_font = m_font;
        fontType = 2;
        if(m_isUseFontSize)
        {
            new_font.setPointSize(m_fontSize);
            sizeType = 2;
        }
    }
    ApplyFont(new_font, fontType, sizeType);
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
        QFileInfo file(m_path + imgPath);
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
    QString new_path = path;
    if(!new_path.endsWith("/"))
        new_path+="/";
    m_path = new_path;
    _mainDescTextBox->SetGamePath(new_path);
    _descTextBox->SetGamePath(new_path);
    _actionsListBox->SetGamePath(new_path);
    _objectsListBox->SetGamePath(new_path);
    m_imgView->SetGamePath(new_path);
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

void MainWindow::ApplyFont(const QFont &new_font, int fontType, int sizeType)
{
    m_font = new_font;
    _mainDescTextBox->SetTextFont(new_font);
    _mainDescTextBox->SetFontType(fontType);
    _mainDescTextBox->SetSizeType(sizeType);
    _descTextBox->SetTextFont(new_font);
    _descTextBox->SetFontType(fontType);
    _descTextBox->SetSizeType(sizeType);
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
    m_imgView->SetBackgroundColor(color);
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

void MainWindow::SetOverallVolume(float new_volume)
{
    QSPCallBacks::SetOverallVolume(new_volume);
    m_volume = new_volume;
}

void MainWindow::SetDisableVideo(bool isDisableVideo)
{
    disableVideo = isDisableVideo;
#ifndef _WEBBOX_COMMON
    _mainDescTextBox->SetDisableVideo(disableVideo);
    _descTextBox->SetDisableVideo(disableVideo);
#endif
}

void MainWindow::SetVideoFix(bool isFix)
{
    m_videoFix = isFix;
#ifdef _WEBBOX_COMMON
    _mainDescTextBox->SetVideoFix(m_videoFix);
    _descTextBox->SetVideoFix(m_videoFix);
#endif
}

void MainWindow::SetAllowHTML5Extras(bool HTML5Extras)
{
    m_isAllowHTML5Extras = HTML5Extras;
    QSPCallBacks::SetAllowHTML5Extras(m_isAllowHTML5Extras);
}

void MainWindow::SetUseCaseInsensitiveFilePath(bool CaseInsensitiveFilePath)
{
    QSPTools::useCaseInsensitiveFilePath = CaseInsensitiveFilePath;
}

bool MainWindow::GetUseCaseInsensitiveFilePath()
{
    return QSPTools::useCaseInsensitiveFilePath;
}

void MainWindow::LoadSettings(QString filePath)
{
    QSettings *settings;
    if(filePath.isEmpty())
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName(), this);
    else
        settings = new QSettings(filePath, QSettings::IniFormat);

    restoreGeometry(settings->value("mainWindow/geometry").toByteArray());
    if ( isMaximized() )
        setGeometry( QApplication::desktop()->availableGeometry( this ) );
    restoreState(settings->value("mainWindow/windowState").toByteArray());

    if (settings->value("mainWindow/isMaximized", isMaximized()).toBool())
        showMaximized();
    if (settings->value("mainWindow/isFullScreen", isFullScreen()).toBool())
        showFullScreen();

    OnToggleCaptions(settings->value("mainWindow/showCaptions", showCaptions).toBool());

    SetShowPlainText(settings->value("application/isShowPlainText", showPlainText).toBool());

    SetLastPath(settings->value("application/lastPath", GetLastPath()).toString());
    perGameConfig = settings->value("application/perGameConfig", perGameConfig).toBool();

    m_isUseFontSize = settings->value("application/isUseFontSize", m_isUseFontSize).toBool();
    m_fontSize = settings->value("application/fontSize", m_fontSize).toInt();
    m_isUseFont = settings->value("application/isUseFont", m_isUseFont).toBool();
    if(m_isUseFont)
        ApplyFont(qvariant_cast<QFont>(settings->value("application/font", m_font)), 2, 2);

    m_isUseBackColor = settings->value("application/isUseBackColor", m_isUseBackColor).toBool();
    m_isUseLinkColor = settings->value("application/isUseLinkColor", m_isUseLinkColor).toBool();
    m_isUseFontColor = settings->value("application/isUseFontColor", m_isUseFontColor).toBool();
    if(m_isUseBackColor)
        ApplyBackColor(qvariant_cast<QColor>(settings->value("application/backColor", m_backColor)));
    if(m_isUseLinkColor)
        ApplyLinkColor(qvariant_cast<QColor>(settings->value("application/linkColor", m_linkColor)));
    if(m_isUseFontColor)
        ApplyFontColor(qvariant_cast<QColor>(settings->value("application/fontColor", m_fontColor)));
    m_settingsBackColor = qvariant_cast<QColor>(settings->value("application/backColor", m_backColor));
    m_settingsLinkColor = qvariant_cast<QColor>(settings->value("application/linkColor", m_linkColor));
    m_settingsFontColor = qvariant_cast<QColor>(settings->value("application/fontColor", m_fontColor));

    disableVideo = settings->value("application/disableVideo", disableVideo).toBool();
    SetDisableVideo(disableVideo);
    m_videoFix = settings->value("application/videoFix", m_videoFix).toBool();
    SetVideoFix(m_videoFix);

    lastGame = settings->value("application/lastGame", lastGame).toString();
    autostartLastGame = settings->value("application/autostartLastGame", autostartLastGame).toBool();

    m_volume = settings->value("application/volume", m_volume).toFloat();
    SetOverallVolume(m_volume);

    m_isShowHotkeys = settings->value("application/isShowHotkeys", m_isShowHotkeys).toBool();

    m_isAllowHTML5Extras = settings->value("application/isAllowHTML5Extras", m_isAllowHTML5Extras).toBool();

    QSPTools::useCaseInsensitiveFilePath = settings->value("application/useCaseInsensitiveFilePath", QSPTools::useCaseInsensitiveFilePath).toBool();

    langid = settings->value("application/language", langid).toString();

    RefreshUI();
    delete settings;
}

void MainWindow::SaveSettings(QString filePath)
{
    QSettings *settings;
    if(filePath.isEmpty())
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName(), this);
    else
        settings = new QSettings(filePath, QSettings::IniFormat);

    bool maximized = isMaximized();

    bool fullscreen = isFullScreen();

    settings->setValue("mainWindow/geometry", saveGeometry());
    settings->setValue("mainWindow/windowState", saveState());
    settings->setValue("mainWindow/isMaximized", maximized);
    settings->setValue("mainWindow/isFullScreen", fullscreen);
    settings->setValue("mainWindow/showCaptions", showCaptions);

    settings->setValue("application/isShowPlainText", showPlainText);

    settings->setValue("application/lastPath", lastPath);
    settings->setValue("application/perGameConfig", perGameConfig);

    settings->setValue("application/isUseFontSize", m_isUseFontSize);
    settings->setValue("application/fontSize", m_fontSize);
    settings->setValue("application/isUseFont", m_isUseFont);
    settings->setValue("application/font", m_font);

    settings->setValue("application/isUseBackColor", m_isUseBackColor);
    settings->setValue("application/isUseLinkColor", m_isUseLinkColor);
    settings->setValue("application/isUseFontColor", m_isUseFontColor);
    settings->setValue("application/backColor", m_settingsBackColor);
    settings->setValue("application/linkColor", m_settingsLinkColor);
    settings->setValue("application/fontColor", m_settingsFontColor);

    settings->setValue("application/disableVideo", disableVideo);
    settings->setValue("application/videoFix", m_videoFix);

    settings->setValue("application/lastGame", lastGame);
    settings->setValue("application/autostartLastGame", autostartLastGame);

    settings->setValue("application/volume", m_volume);

    settings->setValue("application/isShowHotkeys", m_isShowHotkeys);

    settings->setValue("application/isAllowHTML5Extras", m_isAllowHTML5Extras);

    settings->setValue("application/useCaseInsensitiveFilePath", QSPTools::useCaseInsensitiveFilePath);

    settings->setValue("application/language", langid);

    settings->sync();

    delete settings;
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
    // Open quicksave item
    action =  _gameMenu->addAction(tr("Quick Load"),
        this, SLOT(OnOpenQuickSavedGame()), QKeySequence(Qt::Key_F9));
    mainToolBar->addAction(action);
    // Quicksave item
    action =  _gameMenu->addAction(tr("Quick Save"),
        this, SLOT(OnQuickSaveGame()), QKeySequence(Qt::Key_F5));
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

    // Main desc item
    action = _mainDescWidget->toggleViewAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_5));
    _showHideMenu->addAction(action);

    // Image item
    action = _imgViewWidget->toggleViewAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_6));
    _showHideMenu->addAction(action);

    _showHideMenu->addSeparator();

    // Captions item
    action = _showHideMenu->addAction(tr("Captions"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_6));
    action->setCheckable(true);
    if(_objectsWidget->titleBarWidget() == 0)
        action->setChecked(true);
    else
        action->setChecked(false);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(OnToggleCaptions(bool)));

    // ToolBar
    action = mainToolBar->toggleViewAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    _showHideMenu->addAction(action);

    //TODO: MenuBar
    // MenuBar
    //action = _showHideMenu->addAction(tr("MenuBar"));
    //action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    //action->setCheckable(true);
    //if(mainMenuBar->isVisible())
    //    action->setChecked(true);
    //else
    //    action->setChecked(false);
    //connect(action, SIGNAL(toggled(bool)), this, SLOT(OnToggleMenuBar(bool)));

    // Hotkeys for actions item
    action = _showHideMenu->addAction(tr("Hotkeys for actions"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_8));
    action->setCheckable(true);
    action->setChecked(m_isShowHotkeys);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(OnToggleHotkeys(bool)));

    // Window / Fullscreen mode item
    action = _settingsMenu->addAction(QIcon(":/gfx/menu/windowmode"), tr("Window / Fullscreen mode"),
        this, SLOT(OnToggleWinMode()), QKeySequence(Qt::Key_F11));
    mainToolBar->addAction(action);

    _settingsMenu->addSeparator();
    mainToolBar->addSeparator();

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
    // "Main desc" widget
#ifndef _WEBBOX_COMMON
    _mainDescTextBox = new QspTextBox(this);
    connect(_mainDescTextBox, SIGNAL(anchorClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
#endif
#ifdef _WEBBOX
    _mainDescTextBox = new QspWebBox(this);
    connect(_mainDescTextBox, SIGNAL(qspLinkClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
#endif
#ifdef _WEBBOX_WEBKIT
    _mainDescTextBox = new QspWebBox(this);
    connect(_mainDescTextBox, SIGNAL(linkClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
    _mainDescTextBox->load(QUrl("qsp:/"));
    {
        QEventLoop loop;
        connect(_mainDescTextBox,SIGNAL(loadFinished(bool)),&loop,SLOT(quit()));
        loop.exec();
    }
#endif
    _mainDescTextBox->setObjectName(QStringLiteral("_mainDescTextBox"));
    _mainDescWidget = new QDockWidget(tr("Main desc"), this);
    _mainDescWidget->setObjectName(QStringLiteral("_mainDescWidget"));
    addDockWidget(Qt::TopDockWidgetArea, _mainDescWidget, Qt::Vertical);
    _mainDescWidget->setWidget(_mainDescTextBox);

    // "Objects" widget
    _objectsWidget = new QDockWidget(tr("Objects"), this);
    _objectsWidget->setObjectName(QStringLiteral("_objectsWidget"));
    addDockWidget(Qt::RightDockWidgetArea, _objectsWidget, Qt::Vertical);
    _objectsListBox = new QspListBox(this);
    _objectsListBox->setObjectName(QStringLiteral("_objectsListBox"));
    connect(_objectsListBox, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(OnObjectListBoxItemClicked(QListWidgetItem *)));
    //connect(_objectsListBox, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(OnObjectListBoxItemClicked(QListWidgetItem *)));
    connect(_objectsListBox, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(OnObjectListBoxItemClicked(QListWidgetItem *)));
    //connect(_objectsListBox, SIGNAL(currentRowChanged(int)), this, SLOT(OnObjectChange(int)));
    _objectsWidget->setWidget(_objectsListBox);

    // "Actions" widget
    _actionsWidget = new QDockWidget(tr("Actions"), this);
    _actionsWidget->setObjectName(QStringLiteral("_actionsWidget"));
    addDockWidget(Qt::BottomDockWidgetArea, _actionsWidget, Qt::Vertical);
    _actionsListBox = new QspListBox(this);
    _actionsListBox->setObjectName(QStringLiteral("_actionsListBox"));
    connect(_actionsListBox, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(OnActionsListBoxItemClicked(QListWidgetItem *)));
    //connect(_actionsListBox, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(OnActionsListBoxItemClicked(QListWidgetItem *)));
    connect(_actionsListBox, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(OnActionsListBoxItemClicked(QListWidgetItem *)));
    connect(_actionsListBox, SIGNAL(SelectionChange(int)), this, SLOT(OnActionChange(int)));
    _actionsListBox->SetMouseTracking(true);
    _actionsWidget->setWidget(_actionsListBox);

    // "Additional desc" widget
    _descWidget = new QDockWidget(tr("Additional desc"), this);
    _descWidget->setObjectName(QStringLiteral("_descWidget"));
    addDockWidget(Qt::BottomDockWidgetArea, _descWidget, Qt::Horizontal);
#ifndef _WEBBOX_COMMON
    _descTextBox = new QspTextBox(this);
    connect(_descTextBox, SIGNAL(anchorClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
#endif
#ifdef _WEBBOX
    _descTextBox = new QspWebBox(this);
    connect(_descTextBox, SIGNAL(qspLinkClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
#endif
#ifdef _WEBBOX_WEBKIT
    _descTextBox = new QspWebBox(this);
    connect(_descTextBox, SIGNAL(linkClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
    _descTextBox->load(QUrl("qsp:/"));
    {
        QEventLoop loop;
        connect(_descTextBox,SIGNAL(loadFinished(bool)),&loop,SLOT(quit()));
        loop.exec();
    }
#endif
    _descTextBox->setObjectName(QStringLiteral("_descTextBox"));
    _descWidget->setWidget(_descTextBox);

    // "Input area" widget
    _inputWidget = new QDockWidget(tr("Input area"), this);
    _inputWidget->setObjectName(QStringLiteral("_inputWidget"));
    addDockWidget(Qt::BottomDockWidgetArea, _inputWidget, Qt::Vertical);
    _inputTextBox = new QspInputBox(this);
    _inputTextBox->setObjectName(QStringLiteral("_inputTextBox"));
    _inputWidget->setWidget(_inputTextBox);
    connect(_inputTextBox, SIGNAL(textChanged()), this, SLOT(OnInputTextChange()));
    connect(_inputTextBox, SIGNAL(InputTextEnter()), this, SLOT(OnInputTextEnter()));

    m_imgView = new QspImgCanvas(this);
    m_imgView->setObjectName(QStringLiteral("m_imgView"));
    _imgViewWidget = new QDockWidget(tr("Image"), this);
    _imgViewWidget->setObjectName(QStringLiteral("_imgViewWidget"));
    _imgViewWidget->setWidget(m_imgView);
    addDockWidget(Qt::BottomDockWidgetArea, _imgViewWidget, Qt::Vertical);

    splitDockWidget(_actionsWidget, _inputWidget, Qt::Vertical);
    splitDockWidget(_mainDescWidget, _objectsWidget, Qt::Horizontal);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef _WEBBOX
    _mainDescTextBox->Quit();
    _descTextBox->Quit();
    //delete _mainDescTextBox;
    //delete _descTextBox;
#endif
    if(!m_configPath.isEmpty())
        SaveSettings(m_configPath);
    QFileInfo settingsFile(QApplication::applicationDirPath() + "/" + QSP_CUSTOM_CONFIG);
    if(settingsFile.exists() && settingsFile.isFile())
    {
        SaveSettings(QApplication::applicationDirPath() + "/" + QSP_CUSTOM_CONFIG);
    }
    else
    {
        SaveSettings();
    }
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

    if(event->key() == Qt::Key_Up)
    {
        if(_actionsListBox->count()!=0)
        {
            int newSel = _actionsListBox->GetSelection() - 1;
            if(newSel < 0)
                _actionsListBox->SetSelection(_actionsListBox->count()-1);
            else
                _actionsListBox->SetSelection(newSel);
        }
        return;
    }
    if(event->key() == Qt::Key_Down)
    {
        if(_actionsListBox->count()!=0)
        {
            int newSel = _actionsListBox->GetSelection() + 1;
            if(newSel <= 0 || newSel >= _actionsListBox->count())
                _actionsListBox->SetSelection(0);
            else
                _actionsListBox->SetSelection(newSel);
        }
        return;
    }
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        if(_actionsListBox->GetSelection() != -1)
        {
            ActionsListBoxDoAction(_actionsListBox->GetSelection());
            return;
        }


    if(event->key() == Qt::Key_Escape)
        if(isFullScreen())
            showNormal();
#ifndef _WEBBOX_COMMON
    _descTextBox->keyPressEvent(event);
    _mainDescTextBox->keyPressEvent(event);
#endif
    QMainWindow::keyPressEvent(event);
}

void MainWindow::OpenGameFile(const QString &path)
{
    if(!path.isEmpty())
    {
        QFileInfo fileName(path);
        QSPCallBacks::m_gamePath = fileName.canonicalPath();
        if(!QSPCallBacks::m_gamePath.endsWith("/")) QSPCallBacks::m_gamePath+="/";
        _mainDescTextBox->SetGamePath(QSPCallBacks::m_gamePath);
        _objectsListBox->SetGamePath(QSPCallBacks::m_gamePath);
        _actionsListBox->SetGamePath(QSPCallBacks::m_gamePath);
        _descTextBox->SetGamePath(QSPCallBacks::m_gamePath);
        if (QSPLoadGameWorld(qspStringFromQString(path)))
        {
            m_isGameOpened = true;
            lastGame = path;
            QFileInfo file(path);
            QString filePath(file.canonicalPath());
            if(!filePath.endsWith("/")) filePath+="/";
            QString configString(filePath + QSP_CONFIG);
            if (configString != m_configPath && perGameConfig)
            {
                if(m_configPath.isEmpty())
                {
                    QFileInfo settingsFile(QApplication::applicationDirPath() + "/" + QSP_CUSTOM_CONFIG);
                    if(settingsFile.exists() && settingsFile.isFile())
                    {
                        SaveSettings(QApplication::applicationDirPath() + "/" + QSP_CUSTOM_CONFIG);
                    }
                    else
                    {
                        SaveSettings();
                    }
                }
                else
                {
                    SaveSettings(m_configPath);
                }
                m_configPath = configString;
                QFileInfo configFile(configString);
                if(configFile.exists() && configFile.isFile())
                    LoadSettings(configString);
            }
            if(!m_isUseBackColor)
                ApplyBackColor(m_defaultBackColor);
            if(!m_isUseLinkColor)
                ApplyLinkColor(m_defaultLinkColor);
            if(!m_isUseFontColor)
                ApplyFontColor(m_defaultFontColor);
            if(!m_isUseFont)
                ApplyFont(m_defaultFont, 0, 0);
            QFileInfo cssFile(filePath + "custom.css");
            if(cssFile.exists() && cssFile.isFile())
            {
                _mainDescTextBox->SetCustomCSS(true);
                _descTextBox->SetCustomCSS(true);
            }
            else
            {
                _mainDescTextBox->SetCustomCSS(false);
                _descTextBox->SetCustomCSS(false);
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
}

void MainWindow::ActionsListBoxDoAction(int action)
{
    if(m_isProcessEvents)
    {
        if(action != -1)
        {
            if (!QSPSetSelActionIndex(action, QSP_TRUE))
                ShowError();
            if (!QSPExecuteSelActionCode(QSP_TRUE))
                ShowError();
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        if(event->mimeData()->urls().count() > 0)
        {
            if(event->mimeData()->urls().at(0).toLocalFile().endsWith(".qsp"))
            {
                OpenGameFile(event->mimeData()->urls().at(0).toLocalFile());
                event->acceptProposedAction();
            }
            if(event->mimeData()->urls().at(0).toLocalFile().endsWith(".sav"))
            {
                if(m_isGameOpened)
                {
                    if (!QSPOpenSavedGame(qspStringFromQString(event->mimeData()->urls().at(0).toLocalFile()), QSP_TRUE))
                        ShowError();
                    else
                        ApplyParams();
                }
                event->acceptProposedAction();
            }
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::OnOpenGame()
{
#ifndef _ANDROIDQT
    #ifndef _NONATIVEDIALOG
    QString path = QFileDialog::getOpenFileName(this, tr("Select game file"), GetLastPath(), tr("QSP games (*.qsp *.gam)"));
    #else
    QString path = QFileDialog::getOpenFileName(this, tr("Select game file"), GetLastPath(), tr("QSP games (*.qsp *.gam)"), nullptr, QFileDialog::DontUseNativeDialog);
    #endif
    if (!path.isEmpty())
    {
        SetLastPath(QFileInfo(path).canonicalPath());
        OpenGameFile(path);
    }
#else
    QString path = QFileDialog::getOpenFileName(this, tr("Select game file"), QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0), tr("QSP games (*.qsp *.gam)"));
    if (!path.isEmpty())
    {
        SetLastPath(QFileInfo(path).canonicalPath());
        OpenGameFile(path);
    }
    return;
    AndroidFileDialog fileDialog;
    connect(&fileDialog, SIGNAL(existingFileNameReady(QString)), this, SLOT(OpenGameFile(QString)));
    bool success = fileDialog.provideExistingFileName();
    if (!success) {
        qDebug() << "Problem with JNI or sth like that...";
        disconnect(fileDialog, SIGNAL(existingFileNameReady(QString)), this, SLOT(OpenGameFile(QString)));
        //or just delete fileDialog instead of disconnect
    }
#endif
}

void MainWindow::OnRestartGame()
{
    if(m_isGameOpened)
    {
        if (!QSPRestartGame(QSP_TRUE))
            ShowError();
        else
            ApplyParams();
    }
}

void MainWindow::OnOpenSavedGame()
{
    if(!m_isGameOpened)
        return;
#ifndef _NONATIVEDIALOG
    QString path = QFileDialog::getOpenFileName(this, tr("Select saved game file"), GetLastPath(), tr("Saved game files (*.sav)"));
#else
    QString path = QFileDialog::getOpenFileName(this, tr("Select saved game file"), GetLastPath(), tr("Saved game files (*.sav)"), nullptr, QFileDialog::DontUseNativeDialog);
#endif
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
#ifndef _NONATIVEDIALOG
    QString path = QFileDialog::getSaveFileName(this, tr("Select file to save"), GetLastPath(), tr("Saved game files (*.sav)"));
#else
    QString path = QFileDialog::getSaveFileName(this, tr("Select file to save"), GetLastPath(), tr("Saved game files (*.sav)"), nullptr, QFileDialog::DontUseNativeDialog);
#endif
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

void MainWindow::OnOpenQuickSavedGame()
{
    if(!m_isGameOpened)
        return;
    QString path = m_path + QSP_QUICKSAVE;
    QFileInfo fileInfo(path);
    if(fileInfo.exists() && fileInfo.isFile())
    {
        if (!QSPOpenSavedGame(qspStringFromQString(path), QSP_TRUE))
            ShowError();
        else
            ApplyParams();
    }
}

void MainWindow::OnQuickSaveGame()
{
    if(!m_isGameOpened)
        return;
    QString path = m_path + QSP_QUICKSAVE;
    if (QSPSaveGame(qspStringFromQString(path), QSP_TRUE))
        m_savedGamePath = path;
    else
        ShowError();
}

void MainWindow::OnOptions()
{
    OptionsDialog optdlg(this);
    optdlg.exec();
    if(!m_configPath.isEmpty())
        SaveSettings(m_configPath);
    QFileInfo settingsFile(QApplication::applicationDirPath() + "/" + QSP_CUSTOM_CONFIG);
    if(settingsFile.exists() && settingsFile.isFile())
    {
        SaveSettings(QApplication::applicationDirPath() + "/" + QSP_CUSTOM_CONFIG);
    }
    else
    {
        SaveSettings();
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
        "<p>Copyright &copy; 2017-2019, Sonnix</p>"));
    text += tr("<p>Application version: %1<br>QSP library version: %2<br>Qt library version: %3<br>Application compilation date: %4<br>Library compilation date: %5</p>").arg(QApplication::applicationVersion(), version, QT_VERSION_STR, guiCompiledDate, libCompiledDate);
    QMessageBox dlg(QMessageBox::NoIcon, tr("About"), text, QMessageBox::Ok);
    dlg.setIconPixmap(icon);
    dlg.exec();
}

void MainWindow::OnToggleCaptions(bool checked)
{
    showCaptions = checked;
    QWidget* mainTitleBarWidget = _mainDescWidget->titleBarWidget();
    QWidget* objectsTitleBarWidget = _objectsWidget->titleBarWidget();
    QWidget* actionsTitleBarWidget = _actionsWidget->titleBarWidget();
    QWidget* descTitleBarWidget = _descWidget->titleBarWidget();
    QWidget* inputTitleBarWidget = _inputWidget->titleBarWidget();
    if(checked == false)
    {
        _mainDescWidget->setTitleBarWidget(new QWidget(_mainDescWidget));
        _mainDescWidget->titleBarWidget()->hide();
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
        _mainDescWidget->setTitleBarWidget(0);
        _objectsWidget->setTitleBarWidget(0);
        _actionsWidget->setTitleBarWidget(0);
        _descWidget->setTitleBarWidget(0);
        _inputWidget->setTitleBarWidget(0);
    }
    if(mainTitleBarWidget)
        delete mainTitleBarWidget;
    if(objectsTitleBarWidget)
        delete objectsTitleBarWidget;
    if(actionsTitleBarWidget)
        delete actionsTitleBarWidget;
    if(descTitleBarWidget)
        delete descTitleBarWidget;
    if(inputTitleBarWidget)
        delete inputTitleBarWidget;
}

void MainWindow::OnToggleMenuBar(bool checked)
{
    mainMenuBar->setVisible(checked);
}

void MainWindow::OnToggleHotkeys(bool checked)
{
    m_isShowHotkeys = checked;
    RefreshUI();
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
    if(!m_isProcessEvents)
        return;
    QString href;
    href = QByteArray::fromPercentEncoding(url.toString().toUtf8());

    if (href.startsWith("#"))
    {
        QObject* obj = sender();
        if (obj == _mainDescTextBox)
#ifndef _WEBBOX_COMMON
            _mainDescTextBox->setSource(url);
#else
            _mainDescTextBox->setUrl(url);
#endif
        else
#ifndef _WEBBOX_COMMON
            _descTextBox->setSource(url);
#else
            _descTextBox->setUrl(url);
#endif
    }
    else if (href.startsWith("EXEC:", Qt::CaseInsensitive))
    {
        QString string = href.mid(5);
        if (m_isProcessEvents && !QSPExecString(qspStringFromQString(string), QSP_TRUE))
            ShowError();
    }
    else
    {
        QDesktopServices::openUrl(url);
    }
}

void MainWindow::OnObjectListBoxItemClicked(QListWidgetItem *itemClicked)
{
    if(!m_isProcessEvents)
        return;
    int object = _objectsListBox->row(itemClicked);
    if (!QSPSetSelObjectIndex(object, QSP_TRUE))
        ShowError();
}

void MainWindow::OnActionsListBoxItemClicked(QListWidgetItem *itemClicked)
{
    if(!m_isProcessEvents)
        return;
    int action = _actionsListBox->row(itemClicked);
    ActionsListBoxDoAction(action);
}

void MainWindow::OnObjectChange(int currentRow)
{
    if(!m_isProcessEvents)
        return;
    //QThread::msleep(20);
    if (!QSPSetSelObjectIndex(currentRow, QSP_TRUE))
        ShowError();
}

void MainWindow::OnActionChange(int currentRow)
{
    if(!m_isProcessEvents)
        return;
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
    if(!m_isProcessEvents)
        return;
    QSPSetInputStrText(qspStringFromQString(_inputTextBox->GetText()));
    if (!QSPExecUserInput(QSP_TRUE))
        ShowError();
}
