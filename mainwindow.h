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
#include <QCloseEvent>
#include <QKeyEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QTranslator>

#include "qsptextbox.h"
#include "qsplistbox.h"
#include "qspinputbox.h"
#include "qspimgcanvas.h"
#ifdef _WEBBOX
#include <QWebEngineProfile>
#include "qspwebbox.h"
#include "qspwebengineurlrequestinterceptor.h"
#include "qspwebengineurlschemehandler.h"
#endif
#ifdef _WEBBOX_WEBKIT
#include "qspwebbox_webkit.h"
#endif

#include <qsp_default.h>

#define QSP_LOGO "Qt Quest Soft Player"
#define QSP_APPNAME "Qqsp"
#define QSP_CONFIG "qqsp.ini"
#define QSP_CUSTOM_CONFIG "custom.ini"
#define QSP_QUICKSAVE "quicksave.sav"
#define QSP_GAME "game.qsp"

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
#ifndef _WEBBOX_COMMON
    QspTextBox *GetDesc() const { return _mainDescTextBox; }
    QspTextBox *GetVars() const { return _descTextBox; }
#else
    QspWebBox *GetDesc() const { return _mainDescTextBox; }
    QspWebBox *GetVars() const { return _descTextBox; }
#endif
    QspInputBox *GetInput() const { return _inputTextBox; }
    QspListBox *GetActions() const { return _actionsListBox; }
    QspListBox *GetObjects() const { return _objectsListBox; }

    QDockWidget *GetVarsDock() const { return _descWidget; }
    QDockWidget *GetInputDock() const { return _inputWidget; }
    QDockWidget *GetActionsDock() const { return _actionsWidget; }
    QDockWidget *GetObjectsDock() const { return _objectsWidget; }
    QDockWidget *GetImageDock() const { return _imgViewWidget; }

    QString GetLastPath() { return lastPath; }
    void SetLastPath(const QString &path) { lastPath = path; }

    QspImgCanvas *GetImgView() const { return m_imgView; }
    QMenu *GetGameMenu() const { return _gameMenu; }
    bool IsShowHotkeys() const { return m_isShowHotkeys; }
    bool IsQuit() const { return m_isQuit; }
    bool IsKeyPressedWhileDisabled() const { return m_keyPressedWhileDisabled; }

    QColor GetLinkColor() { return m_linkColor; }
    QColor GetBackgroundColor() { return m_backColor; }
    QColor GetForegroundColor() { return m_fontColor; }
    void SetLinkColor(const QColor& new_color) { m_settingsLinkColor = new_color; if(m_isUseLinkColor) ApplyLinkColor(new_color); }
    void SetBackgroundColor(const QColor& new_color) { m_settingsBackColor = new_color; if(m_isUseBackColor) ApplyBackColor(new_color); }
    void SetForegroundColor(const QColor& new_color) { m_settingsFontColor = new_color; if(m_isUseFontColor) ApplyFontColor(new_color); }

    void SetShowPlainText(bool isPlain);

    bool GetUseFontSize() { return m_isUseFontSize; }
    void SetUseFontSize(bool isUseFontS) { m_isUseFontSize = isUseFontS; }
    int GetFontSize() { return m_fontSize; }
    void SetFontSize(int fontS) { m_fontSize = fontS; }
    bool GetUseFont() { return m_isUseFont; }
    void SetUseFont(bool isUseFont) { m_isUseFont = isUseFont; }
    QFont GetFont() { return m_font; }
    void SetFont(const QFont& new_font) { m_font = new_font; if(m_isUseFont) ApplyFont(new_font, 2, 2); }
    bool GetAutostart() { return autostartLastGame; }
    void SetAutostart(bool isAutostart) { autostartLastGame = isAutostart; }
    bool GetPerGameConfig() { return perGameConfig; }
    void SetPerGameConfig(bool isPerGameConfig) { perGameConfig = isPerGameConfig; }
    bool GetUseBackColor() { return m_isUseBackColor; }
    void SetUseBackColor(bool isUseBackColor) { m_isUseBackColor = isUseBackColor; }
    bool GetUseLinkColor() { return m_isUseLinkColor; }
    void SetUseLinkColor(bool isUseLinkColor) { m_isUseLinkColor = isUseLinkColor; }
    bool GetUseFontColor() { return m_isUseFontColor; }
    void SetUseFontColor(bool isUseFontColor) { m_isUseFontColor = isUseFontColor; }
    QString GetLangID() { return langid; }
    void SetLangID(const QString& new_langid) { langid = new_langid; }
    float GetOverallVolume() { return m_volume; }
    void SetOverallVolume(float new_volume);
    bool GetDisableVideo() { return disableVideo; }
    void SetDisableVideo(bool isDisableVideo);
    bool GetVideoFix() { return m_videoFix; }
    void SetVideoFix(bool isFix);
    void SetAllowHTML5Extras(bool HTML5Extras);
    bool GetAllowHTML5Extras() { return m_isAllowHTML5Extras; }
    void SetUseCaseInsensitiveFilePath(bool CaseInsensitiveFilePath);
    bool GetUseCaseInsensitiveFilePath();

private:
    void CreateMenuBar();
    void CreateDockWindows();
    void LoadSettings(QString filePath = QString());
    void SaveSettings(QString filePath = QString());
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void ActionsListBoxDoAction(int action);
    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);

    // Internal methods
    void UpdateTitle();
    void ReCreateGUI();
    void RefreshUI();
    void ApplyFont(const QFont& new_font, int fontType, int sizeType);
    bool ApplyFontColor(const QColor& color);
    bool ApplyBackColor(const QColor& color);
    bool ApplyLinkColor(const QColor& color);

    QMenuBar*       mainMenuBar;
    QToolBar*       mainToolBar;
    //QStatusBar*     mainStatusBar;
    QMenu*			_fileMenu; // was wxMenu *m_fileMenu;
    QMenu*			_gameMenu; // was wxMenu *m_gameMenu;
    QMenu*			_settingsMenu; // was wxMenu *m_settingsMenu;
    QMenu*			_showHideMenu; //Show / Hide submenu
#ifndef _WEBBOX_COMMON
    QspTextBox*		_mainDescTextBox; //m_desc
    QspTextBox*		_descTextBox; //m_vars ID_VARSDESC
#else
    QspWebBox*		_mainDescTextBox; //m_desc
    QspWebBox*		_descTextBox; //m_vars ID_VARSDESC
#endif
    QspListBox*		_objectsListBox; //m_objects
    QspListBox*		_actionsListBox; //m_actions

    QspInputBox*	_inputTextBox; //m_input
    QDockWidget*	_objectsWidget; //m_objects
    QDockWidget*	_actionsWidget; //m_actions
    QDockWidget*	_descWidget; //m_vars ID_VARSDESC
    QDockWidget*	_inputWidget; //m_input
    QDockWidget*    _mainDescWidget;
    QString lastPath; //For QFileDialog
    QString lastGame;

    // Fields
    bool m_isGameOpened;
    QString m_savedGamePath;
    QString m_configPath;
    bool perGameConfig;
    QString m_path;
    QTimer *m_timer;
    QspImgCanvas *m_imgView;
    QDockWidget*    _imgViewWidget;
    int m_menuItemId;
    QMenu *m_menu; //qsp callback menu
    QColor m_backColor;
    QColor m_linkColor;
    QColor m_fontColor;
    QColor m_defaultBackColor;
    QColor m_defaultLinkColor;
    QColor m_defaultFontColor;
    QColor m_settingsBackColor;
    QColor m_settingsLinkColor;
    QColor m_settingsFontColor;
    bool m_isUseBackColor;
    bool m_isUseLinkColor;
    bool m_isUseFontColor;
    QPalette m_palette;
    int m_fontSize;
    QFont m_font;
    QFont m_defaultFont;
    bool m_isUseFontSize;
    bool m_isUseFont;
    bool m_isProcessEvents;
    bool m_isQuit;
    bool m_keyPressedWhileDisabled;
    bool m_isShowHotkeys;
    float m_volume;
    int m_menuIndex;
    bool showPlainText;
    bool showCaptions;
    bool autostartLastGame;
    QString langid;
    bool disableVideo;
    bool m_videoFix;
    bool m_isAllowHTML5Extras;

public slots:
    void OpenGameFile(const QString& path);

private slots:
    void OnOpenGame();
    void OnRestartGame();
    void OnOpenSavedGame();
    void OnSaveGame();
    void OnOpenQuickSavedGame();
    void OnQuickSaveGame();
    void OnOptions();
    void OnAbout();
    void OnToggleCaptions(bool checked);
    void OnToggleMenuBar(bool checked);
    void OnToggleHotkeys(bool checked);
    void OnToggleWinMode();
    void OnToggleShowPlainText(bool checked);
    void OnNewGame();
    void OnTimer();
    void OnLinkClicked(const QUrl& url);
    void OnObjectListBoxItemClicked (QListWidgetItem * itemClicked);
    void OnActionsListBoxItemClicked (QListWidgetItem * itemClicked);
    void OnObjectChange(int currentRow);
    void OnActionChange(int currentRow);
    void OnMenu(QAction* action);
    void OnInputTextChange();
    void OnInputTextEnter();
};

#endif // MAINWINDOW_H
