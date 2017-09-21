#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDockWidget>
#include <QString>
#include <QColor>
#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QFont>

#include "qsptextbox.h"
#include "qsplistbox.h"
#include "qspinputbox.h"
#include "qspimgcanvas.h"

#include <qsp_default.h>

#define QSP_LOGO "Quest Soft Player 5"
#define QSP_APPNAME "qspgui"
#define QSP_CONFIG "qspgui.cfg"
#define QSP_SOUNDPLUGINS "sound"
#define QSP_MIDIDLS "sound/midi.dls"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Methods
    void EnableControls(bool status, bool isExtended = false);
    void ApplyParams();
    void DeleteMenu();
    void AddMenuItem(const QString &name, const QString &imgPath);
    int ShowMenu();
    void UpdateGamePath(const QString &path);
    void ShowError();

    // Accessors
    QTimer *GetTimer() const { return m_timer; }
    QspTextBox *GetDesc() const { return _mainDescTextBox; }
    QspTextBox *GetVars() const { return _descTextBox; }
    QspInputBox *GetInput() const { return _inputTextBox; }
    QspListBox *GetActions() const { return _actionsListBox; }
    QspListBox *GetObjects() const { return _objectsListBox; }

    QDockWidget *GetVarsDock() const { return _descWidget; }
    QDockWidget *GetInputDock() const { return _inputWidget; }
    QDockWidget *GetActionsDock() const { return _actionsWidget; }
    QDockWidget *GetObjectsDock() const { return _objectsWidget; }

    QString GetLastPath() { return lastPath; }
    void SetLastPath(const QString &path) { lastPath = path; } //TODO: save to config

    QspImgCanvas *GetImgView() const { return m_imgView; }
    QMenu *GetGameMenu() const { return _gameMenu; }
    bool IsShowHotkeys() const { return m_isShowHotkeys; }
    bool IsQuit() const { return m_isQuit; }
    bool IsKeyPressedWhileDisabled() const { return m_keyPressedWhileDisabled; }

    QColor GetLinkColor() { return m_linkColor; }
    QColor GetBackgroundColor() { return m_backColor; }
    QColor GetForegroundColor() { return m_fontColor; }

    void SetShowPlainText(bool isPlain);

private:
    void CreateMenuBar();
    void CreateDockWindows();
    void LoadSettings();
    void SaveSettings();
    void closeEvent(QCloseEvent *event);
    void OpenGameFile(const QString& path);

    // Internal methods
    void UpdateTitle();
    void ReCreateGUI();
    void RefreshUI();
    void ApplyFont(const QFont& new_font);
//    bool ApplyFontSize(int size);
//    bool ApplyFontName(const wxString& name);
    bool ApplyFontColor(const QColor& color);
    bool ApplyBackColor(const QColor& color);
    bool ApplyLinkColor(const QColor& color);
//    void SetOverallVolume(int percents);

    QMenuBar*       mainMenuBar;
    QToolBar*       mainToolBar;
    QStatusBar*     mainStatusBar;
    QMenu*			_fileMenu; // was wxMenu *m_fileMenu;
    QMenu*			_gameMenu; // was wxMenu *m_gameMenu;
    QMenu*			_settingsMenu; // was wxMenu *m_settingsMenu;
    QMenu*			_showHideMenu; //Show / Hide submenu
    QspTextBox*		_mainDescTextBox; //m_desc
    QspListBox*		_objectsListBox; //m_objects
    QspListBox*		_actionsListBox; //m_actions
    QspTextBox*		_descTextBox; //m_vars ID_VARSDESC
    QspInputBox*	_inputTextBox; //m_input
    QDockWidget*	_objectsWidget; //m_objects
    QDockWidget*	_actionsWidget; //m_actions
    QDockWidget*	_descWidget; //m_vars ID_VARSDESC
    QDockWidget*	_inputWidget; //m_input
    QString lastPath; //For QFileDialog

    // Fields
    bool m_isGameOpened;
    QString m_savedGamePath;
    QString m_configPath;
    QString m_configDefPath;
    QTimer *m_timer;
    QspImgCanvas *m_imgView;
    int m_menuItemId;
    QMenu *m_menu; //qsp callback menu
    QColor m_backColor;
    QColor m_linkColor;
    QColor m_fontColor;
    int m_fontSize;
    QString m_fontName;
    QFont m_font;
    bool m_isUseFontSize;
    bool m_isProcessEvents;
    bool m_isQuit;
    bool m_keyPressedWhileDisabled;
    bool m_isShowHotkeys;
    int m_volume;
    int m_menuIndex;
    bool showPlainText;

private slots:
    void OnOpenGame();
    void OnRestartGame();
    void OnOpenSavedGame();
    void OnSaveGame(); //TODO: add quick save/load
//    void OnOptions();
//    void OnAbout();
//    void OnToggleCaptions();
//    void OnToggleHotkeys();
    void OnToggleWinMode();
    void OnToggleShowPlainText(bool checked);
//    void OnChangeSoundVolume();
    void OnNewGame();
    void OnTimer();
    void OnLinkClicked(const QUrl& url);
    void OnObjectListBoxItemClicked (QListWidgetItem * itemClicked);
    void OnActionsListBoxItemClicked (QListWidgetItem * itemClicked);
    void OnObjectChange(int currentRow);
    void OnActionChange(int currentRow);
    void OnMenu(QAction* action);

    // Events
//    void OnInit(); //TODO: add autorun event

//    void OnQuickSave(wxCommandEvent& event);
//    void OnSelectFont(wxCommandEvent& event);
//    void OnUseFontSize(wxCommandEvent& event);
//    void OnSelectFontColor(wxCommandEvent& event);
//    void OnSelectBackColor(wxCommandEvent& event);
//    void OnSelectLinkColor(wxCommandEvent& event);
//    void OnSelectLang(wxCommandEvent& event);
//    void OnToggleCaptions(wxCommandEvent& event);
//    void OnToggleHotkeys(wxCommandEvent& event);
//    void OnVolume(wxCommandEvent& event);
//    void OnAbout(wxCommandEvent& event);
//    void OnInputTextChange(wxCommandEvent& event);
//    void OnInputTextEnter(wxCommandEvent& event);
//    void OnDropFiles(wxDropFilesEvent& event);
};

#endif // MAINWINDOW_H
