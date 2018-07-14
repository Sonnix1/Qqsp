#include "callbacks_gui.h"

#include <QCoreApplication>
#include <QThread>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QTimer>
#include <QEventLoop>

#include "comtools.h"
#include "qspmsgdlg.h"
#include "qspinputdlg.h"
#ifdef _WEBBOX
#include "qspwebbox.h"
#endif
#ifdef _WEBBOX_WEBKIT
#include "qspwebbox_webkit.h"
#endif

QString QSPCallBacks::m_gamePath;
MainWindow *QSPCallBacks::m_frame;
bool QSPCallBacks::m_isHtml;
QSPSounds QSPCallBacks::m_sounds;
float QSPCallBacks::m_volumeCoeff;
bool QSPCallBacks::m_isAllowHTML5Extras;

void QSPCallBacks::Init(MainWindow *frame)
{
	m_frame = frame;
    m_volumeCoeff = 1.0f;

    m_isAllowHTML5Extras = false;

	QSPSetCallBack(QSP_CALL_SETTIMER, (QSP_CALLBACK)&SetTimer);
	QSPSetCallBack(QSP_CALL_REFRESHINT, (QSP_CALLBACK)&RefreshInt);
	QSPSetCallBack(QSP_CALL_SETINPUTSTRTEXT, (QSP_CALLBACK)&SetInputStrText);
	QSPSetCallBack(QSP_CALL_ISPLAYINGFILE, (QSP_CALLBACK)&IsPlay);
	QSPSetCallBack(QSP_CALL_PLAYFILE, (QSP_CALLBACK)&PlayFile);
	QSPSetCallBack(QSP_CALL_CLOSEFILE, (QSP_CALLBACK)&CloseFile);
	QSPSetCallBack(QSP_CALL_SHOWMSGSTR, (QSP_CALLBACK)&Msg);
	QSPSetCallBack(QSP_CALL_SLEEP, (QSP_CALLBACK)&Sleep);
	QSPSetCallBack(QSP_CALL_GETMSCOUNT, (QSP_CALLBACK)&GetMSCount);
    QSPSetCallBack(QSP_CALL_DELETEMENU, (QSP_CALLBACK)&DeleteMenu);
    QSPSetCallBack(QSP_CALL_ADDMENUITEM, (QSP_CALLBACK)&AddMenuItem);
	QSPSetCallBack(QSP_CALL_SHOWMENU, (QSP_CALLBACK)&ShowMenu);
	QSPSetCallBack(QSP_CALL_INPUTBOX, (QSP_CALLBACK)&Input);
	QSPSetCallBack(QSP_CALL_SHOWIMAGE, (QSP_CALLBACK)&ShowImage);
	QSPSetCallBack(QSP_CALL_SHOWWINDOW, (QSP_CALLBACK)&ShowPane);
    //QSPSetCallBack(QSP_CALL_OPENGAME, (QSP_CALLBACK)&OpenGame); //replace
	QSPSetCallBack(QSP_CALL_OPENGAMESTATUS, (QSP_CALLBACK)&OpenGameStatus);
	QSPSetCallBack(QSP_CALL_SAVEGAMESTATUS, (QSP_CALLBACK)&SaveGameStatus);
    //TODO: implement this?
    //QSP_CALL_DEBUG, /* void func(QSPString str) */
}

void QSPCallBacks::DeInit()
{
    CloseFile(0);
}

void QSPCallBacks::SetTimer(int msecs)
{
	if (m_frame->IsQuit()) return;
	if (msecs)
        m_frame->GetTimer()->start(msecs);
	else
        m_frame->GetTimer()->stop();
}

void QSPCallBacks::RefreshInt(QSP_BOOL isRedraw)
{
	static int oldFullRefreshCount = 0;
	int i, numVal;
	bool isScroll, isCanSave;
    QSP_CHAR *strVal, *imgPath;
	if (m_frame->IsQuit()) return;
    // -------------------------------
    UpdateGamePath();
    // -------------------------------
    const QSP_CHAR *mainDesc = QSPGetMainDesc();
    const QSP_CHAR *varsDesc = QSPGetVarsDesc();
    // -------------------------------
    isScroll = !(QSPGetVarValues(QSP_FMT("DISABLESCROLL"), 0, &numVal, &strVal) && numVal);
    isCanSave = !(QSPGetVarValues(QSP_FMT("NOSAVE"), 0, &numVal, &strVal) && numVal);
    m_isHtml = QSPGetVarValues(QSP_FMT("USEHTML"), 0, &numVal, &strVal) && numVal;
    // -------------------------------
	m_frame->GetVars()->SetIsHtml(m_isHtml);
	if (QSPIsVarsDescChanged())
	{
        m_frame->EnableControls(false, true);
        if(m_isAllowHTML5Extras)
        {
            if (QSPGetVarValues(QSP_FMT("SETSTATHEAD"), 0, &numVal, &strVal) && strVal)
                m_frame->GetVars()->SetHead(QSPTools::qspStrToQt(strVal));
            else
                 m_frame->GetVars()->SetHead(QString(""));
        }
        m_frame->GetVars()->SetText(QSPTools::qspStrToQt(varsDesc), isScroll);
        m_frame->EnableControls(true, true);
	}
	// -------------------------------
	int fullRefreshCount = QSPGetFullRefreshCount();
	if (oldFullRefreshCount != fullRefreshCount)
	{
		isScroll = false;
		oldFullRefreshCount = fullRefreshCount;
	}
	m_frame->GetDesc()->SetIsHtml(m_isHtml);
    if (QSPIsMainDescChanged())
    {
        m_frame->EnableControls(false, true);
        if(m_isAllowHTML5Extras)
        {
            if (QSPGetVarValues(QSP_FMT("SETMAINDESCHEAD"), 0, &numVal, &strVal) && strVal)
                m_frame->GetDesc()->SetHead(QSPTools::qspStrToQt(strVal));
            else
                 m_frame->GetDesc()->SetHead(QString(""));
        }
        m_frame->GetDesc()->SetText(QSPTools::qspStrToQt(mainDesc), isScroll);
        m_frame->EnableControls(true, true);
	}
	// -------------------------------
	m_frame->GetActions()->SetIsHtml(m_isHtml);
	m_frame->GetActions()->SetIsShowNums(m_frame->IsShowHotkeys());
    if (QSPIsActionsChanged())
    {
        int actionsCount = QSPGetActionsCount();
        m_frame->GetActions()->BeginItems();
        for (i = 0; i < actionsCount; ++i)
        {
            QSPGetActionData(i, &imgPath, &strVal);
            m_frame->GetActions()->AddItem(QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(imgPath)), QSPTools::qspStrToQt(strVal));
        }
        m_frame->GetActions()->EndItems();
    }
    m_frame->GetActions()->SetSelection(QSPGetSelActionIndex());
	m_frame->GetObjects()->SetIsHtml(m_isHtml);
    if (QSPIsObjectsChanged())
    {
        int objectsCount = QSPGetObjectsCount();
        m_frame->GetObjects()->BeginItems();
        for (i = 0; i < objectsCount; ++i)
        {
            QSPGetObjectData(i, &imgPath, &strVal);
            m_frame->GetObjects()->AddItem(QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(imgPath)), QSPTools::qspStrToQt(strVal));
        }
        m_frame->GetObjects()->EndItems();
    }
	m_frame->GetObjects()->SetSelection(QSPGetSelObjectIndex());
	// -------------------------------
    if (QSPGetVarValues(QSP_FMT("BACKIMAGE"), 0, &numVal, &strVal) && strVal)
        m_frame->GetDesc()->LoadBackImage(QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(strVal)));
	else
        m_frame->GetDesc()->LoadBackImage(QString(""));
    // -------------------------------
    m_frame->ApplyParams();
	if (isRedraw)
	{
		m_frame->EnableControls(false, true);
        //m_frame->Update();
        //QCoreApplication::processEvents();
		if (m_frame->IsQuit()) return;
		m_frame->EnableControls(true, true);
	}
    m_frame->GetGameMenu()->setEnabled(isCanSave);
}

void QSPCallBacks::SetInputStrText(const QSP_CHAR *text)
{
	if (m_frame->IsQuit()) return;
    m_frame->GetInput()->SetText(QSPTools::qspStrToQt(text));
}

QSP_BOOL QSPCallBacks::IsPlay(const QSP_CHAR *file)
{
    QSP_BOOL playing = QSP_FALSE;
    QSPSounds::iterator elem = m_sounds.find(QFileInfo(m_gamePath + QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(file))).absoluteFilePath());
    if (elem != m_sounds.end())
        if(elem.value()->state() == QMediaPlayer::PlayingState)
            playing = QSP_TRUE;
    return playing;
}

void QSPCallBacks::CloseFile(const QSP_CHAR *file)
{
    if (file)
	{
        QSPSounds::iterator elem = m_sounds.find(QFileInfo(m_gamePath + QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(file))).absoluteFilePath());
		if (elem != m_sounds.end())
		{
            delete elem.value();
			m_sounds.erase(elem);
		}
	}
	else
	{
        for (QSPSounds::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
            delete i.value();
		m_sounds.clear();
	}
}

void QSPCallBacks::PlayFile(const QSP_CHAR *file, int volume)
{
    if (SetVolume(file, volume)) return;
    CloseFile(file);
    QString strFile(QFileInfo(m_gamePath + QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(file))).absoluteFilePath());
    QMediaPlayer *snd = new QMediaPlayer();
    snd->setMedia(QUrl::fromLocalFile(strFile));
    snd->setVolume(volume*m_volumeCoeff);
    snd->play();
    m_sounds.insert(strFile, snd);
    UpdateSounds();
}

void QSPCallBacks::ShowPane(int type, QSP_BOOL isShow)
{
	if (m_frame->IsQuit()) return;
	switch (type)
	{
	case QSP_WIN_ACTS:
        m_frame->GetActionsDock()->setVisible(isShow != QSP_FALSE);
		break;
	case QSP_WIN_OBJS:
        m_frame->GetObjectsDock()->setVisible(isShow != QSP_FALSE);
		break;
	case QSP_WIN_VARS:
        m_frame->GetVarsDock()->setVisible(isShow != QSP_FALSE);
		break;
	case QSP_WIN_INPUT:
        m_frame->GetInputDock()->setVisible(isShow != QSP_FALSE);
		break;
	}
}

void QSPCallBacks::Sleep(int msecs)
{
    QTimer wtimer;
    wtimer.setSingleShot(true);
    QEventLoop loop;
    QObject::connect(&wtimer, SIGNAL(timeout()), &loop, SLOT(quit()));
    wtimer.start(50);
    loop.exec();
    //RefreshInt(QSP_TRUE);
	if (m_frame->IsQuit()) return;
    bool isSave = m_frame->GetGameMenu()->isEnabled();
	bool isBreak = false;
    m_frame->EnableControls(false, true);
	int i, count = msecs / 50;
	for (i = 0; i < count; ++i)
    {
        //QThread::msleep(50);
        wtimer.start(50);
        loop.exec();
        //qDebug() << QSPTools::qspStrToQt(QSPGetMainDesc());
        //m_frame->Update();
        //QCoreApplication::processEvents();
		if (m_frame->IsQuit() ||
            m_frame->IsKeyPressedWhileDisabled()) //TODO: implement
		{
			isBreak = true;
			break;
		}
	}
    if (!isBreak) //NOTE: no check in old code
    {
        //QThread::msleep(msecs % 50);
        wtimer.start(msecs % 50);
        loop.exec();
        //m_frame->Update();
        //QCoreApplication::processEvents();
    }
	m_frame->EnableControls(true, true);
    m_frame->GetGameMenu()->setEnabled(isSave);
}

int QSPCallBacks::GetMSCount()
{
    static QElapsedTimer stopWatch;
    if(stopWatch.isValid() == false)
        stopWatch.start();
    int ret = stopWatch.restart();
	return ret;
}

void QSPCallBacks::Msg(const QSP_CHAR *str)
{
	if (m_frame->IsQuit()) return;
	RefreshInt(QSP_FALSE);
    QspMsgDlg dialog(m_frame->GetDesc()->GetBackgroundColor(),
        m_frame->GetDesc()->GetForegroundColor(),
		m_frame->GetDesc()->GetTextFont(),
        MainWindow::tr("Info"), //caption
        QSPTools::qspStrToQt(str),
		m_isHtml,
        m_gamePath,
        m_frame
	);
	m_frame->EnableControls(false);
    dialog.exec();
	m_frame->EnableControls(true);
}

void QSPCallBacks::DeleteMenu()
{
    if (m_frame->IsQuit()) return;
    m_frame->DeleteMenu();
}

void QSPCallBacks::AddMenuItem(const QSP_CHAR *name, const QSP_CHAR *imgPath)
{
    if (m_frame->IsQuit()) return;
    m_frame->AddMenuItem(QSPTools::qspStrToQt(name), QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(imgPath)));
}

int QSPCallBacks::ShowMenu()
{
    if (m_frame->IsQuit()) return -1;
    m_frame->EnableControls(false);
    int index = m_frame->ShowMenu();
    m_frame->EnableControls(true);
    return index;
}

void QSPCallBacks::Input(const QSP_CHAR *text, QSP_CHAR *buffer, int maxLen)
{
	if (m_frame->IsQuit()) return;
    RefreshInt(QSP_FALSE);
//	QSPInputDlg dialog(m_frame,
//		wxID_ANY,
//		m_frame->GetDesc()->GetBackgroundColor(),
//		m_frame->GetDesc()->GetForegroundColor(),
//		m_frame->GetDesc()->GetTextFont(),
//		_("Input data"),
//		wxString(text.Str, text.End),
//		m_isHtml,
//		m_gamePath
//	);
//	m_frame->EnableControls(false);
//	dialog.ShowModal();
//	m_frame->EnableControls(true);
//	#ifdef _UNICODE
//		wcsncpy(buffer, dialog.GetText().c_str(), maxLen);
//	#else
//		strncpy(buffer, dialog.GetText().c_str(), maxLen);
//	#endif
    //QString inputText = QInputDialog::getMultiLineText(m_frame, MainWindow::tr("Input data"), QSPTools::qspStrToQt(text));
    QString inputText = QInputDialog::getText(m_frame, MainWindow::tr("Input data"), QSPTools::qspStrToQt(text), QLineEdit::Normal);
    c16sncpy(buffer, (QSP_CHAR *)(inputText.utf16()), maxLen);
}

void QSPCallBacks::ShowImage(const QSP_CHAR *file)
{
	if (m_frame->IsQuit()) return;
    m_frame->GetImgView()->OpenFile(QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(file))); //NOTE: will not display image if file is not found
    if(QSPTools::qspStrToQt(file) == "")
    {
        m_frame->GetImageDock()->setVisible(false);
    }
    else
    {
        m_frame->GetImageDock()->setVisible(true);
    }

    //m_frame->GetImgView()->setVisible(true);
}

//void QSPCallBacks::OpenGame(const QSP_CHAR *file, QSP_BOOL isNewGame)
//{
//	if (m_frame->IsQuit()) return;
//	if (QSPLoadGameWorld(file, isNewGame) && isNewGame)
//	{
//        QFileInfo fileName(QSPTools::qspStrToQt(file));
//        m_gamePath = fileName.canonicalPath();
//        if(!m_gamePath.endsWith('/')) m_gamePath+="/";
//		m_frame->UpdateGamePath(m_gamePath);
//	}
//}

void QSPCallBacks::OpenGameStatus(const QSP_CHAR *file)
{
	if (m_frame->IsQuit()) return;
    if (file)
	{
        QFileInfo fileInfo(QSPTools::qspStrToQt(file));
        if ( fileInfo.exists() && fileInfo.isFile() ) QSPOpenSavedGame(file, QSP_FALSE);
	}
	else
	{
        m_frame->EnableControls(false);
        QString path = QFileDialog::getOpenFileName(m_frame, MainWindow::tr("Select saved game file"), m_frame->GetLastPath(), MainWindow::tr("Saved game files (*.sav)"));
        m_frame->EnableControls(true);
        if (!path.isEmpty())
		{
            m_frame->SetLastPath(QFileInfo(path).canonicalPath());
            QSPOpenSavedGame(qspStringFromQString(path), QSP_FALSE);
		}
	}
}

void QSPCallBacks::SaveGameStatus(const QSP_CHAR *file)
{
	if (m_frame->IsQuit()) return;
    if (file)
		QSPSaveGame(file, QSP_FALSE);
	else
	{
        m_frame->EnableControls(false);
        QString path = QFileDialog::getSaveFileName(m_frame, MainWindow::tr("Select file to save"), m_frame->GetLastPath(), MainWindow::tr("Saved game files (*.sav)"));
		m_frame->EnableControls(true);
        if (!path.isEmpty())
		{
            m_frame->SetLastPath(QFileInfo(path).canonicalPath());
            QSPSaveGame(qspStringFromQString(path), QSP_FALSE);
		}
	}
}

void QSPCallBacks::UpdateGamePath()
{
    QFileInfo fileName(QSPTools::qspStrToQt(QSPGetQstFullPath()));
    m_gamePath = fileName.canonicalPath();
    if(!m_gamePath.endsWith("/")) m_gamePath+="/";
    //m_frame->UpdateGamePath(m_gamePath);
    m_frame->GetDesc()->SetGamePath(m_gamePath);
    m_frame->GetObjects()->SetGamePath(m_gamePath);
    m_frame->GetActions()->SetGamePath(m_gamePath);
    m_frame->GetVars()->SetGamePath(m_gamePath);
    m_frame->GetImgView()->SetGamePath(m_gamePath);
}

bool QSPCallBacks::SetVolume(const QSP_CHAR *file, int volume)
{
    if (!IsPlay(file)) return false;
    QSPSounds::iterator elem = m_sounds.find(QString(QFileInfo(m_gamePath + QSPTools::GetCaseInsensitiveFilePath(m_gamePath, QSPTools::qspStrToQt(file))).absoluteFilePath()));
    QMediaPlayer *snd = elem.value();
    snd->setVolume(volume*m_volumeCoeff);
	return true;
}

void QSPCallBacks::SetOverallVolume(float coeff)
{
    QMediaPlayer *snd;
    if (coeff < 0.0)
        coeff = 0.0;
    else if (coeff > 1.0)
        coeff = 1.0;
    m_volumeCoeff = coeff;
    for (QSPSounds::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
    {
        snd = i.value();
        if (snd->state() == QMediaPlayer::PlayingState)
            snd->setVolume(snd->volume()*m_volumeCoeff);
    }
}

void QSPCallBacks::SetAllowHTML5Extras(bool HTML5Extras)
{
    m_isAllowHTML5Extras = HTML5Extras;
}

void QSPCallBacks::UpdateSounds()
{
    QMediaPlayer *snd;
    QSPSounds::iterator i = m_sounds.begin();
    while (i != m_sounds.end())
    {
        snd = i.value();
        if(snd->state() == QMediaPlayer::PlayingState)
            ++i;
        else
        {
            delete snd;
            i = m_sounds.erase(i);
        }
    }
}
