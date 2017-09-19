#include "callbacks_gui.h"

#include <QCoreApplication>
#include <QThread>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>

#include "comtools.h"
#include "qspmsgdlg.h"
#include "qspinputdlg.h"

QString QSPCallBacks::m_gamePath;
MainWindow *QSPCallBacks::m_frame;
bool QSPCallBacks::m_isHtml;
//FMOD_SYSTEM *QSPCallBacks::m_sys;
QSPSounds QSPCallBacks::m_sounds;
float QSPCallBacks::m_volumeCoeff;

void QSPCallBacks::Init(MainWindow *frame)
{
	m_frame = frame;
	m_volumeCoeff = 1.0;

    //FMOD_System_Create(&m_sys);
    QString soundPath(QSPTools::GetAppPath() + QSP_SOUNDPLUGINS);
    //FMOD_System_SetPluginPath(m_sys, wxConvFile.cWX2MB(soundPath.c_str()));
//	#ifdef __WXMSW__
//		FMOD_System_SetOutput(m_sys, FMOD_OUTPUTTYPE_DSOUND);
//	#elif __WXOSX__
//		FMOD_System_SetOutput(m_sys, FMOD_OUTPUTTYPE_COREAUDIO);
//	#else
//		FMOD_System_SetOutput(m_sys, FMOD_OUTPUTTYPE_ALSA);
//	#endif
//	FMOD_System_Init(m_sys, 32, FMOD_INIT_NORMAL, 0);

	QSPSetCallBack(QSP_CALL_SETTIMER, (QSP_CALLBACK)&SetTimer);
	QSPSetCallBack(QSP_CALL_REFRESHINT, (QSP_CALLBACK)&RefreshInt);
	QSPSetCallBack(QSP_CALL_SETINPUTSTRTEXT, (QSP_CALLBACK)&SetInputStrText);
	QSPSetCallBack(QSP_CALL_ISPLAYINGFILE, (QSP_CALLBACK)&IsPlay);
	QSPSetCallBack(QSP_CALL_PLAYFILE, (QSP_CALLBACK)&PlayFile);
	QSPSetCallBack(QSP_CALL_CLOSEFILE, (QSP_CALLBACK)&CloseFile);
	QSPSetCallBack(QSP_CALL_SHOWMSGSTR, (QSP_CALLBACK)&Msg);
	QSPSetCallBack(QSP_CALL_SLEEP, (QSP_CALLBACK)&Sleep);
	QSPSetCallBack(QSP_CALL_GETMSCOUNT, (QSP_CALLBACK)&GetMSCount);
	QSPSetCallBack(QSP_CALL_SHOWMENU, (QSP_CALLBACK)&ShowMenu);
	QSPSetCallBack(QSP_CALL_INPUTBOX, (QSP_CALLBACK)&Input);
	QSPSetCallBack(QSP_CALL_SHOWIMAGE, (QSP_CALLBACK)&ShowImage);
	QSPSetCallBack(QSP_CALL_SHOWWINDOW, (QSP_CALLBACK)&ShowPane);
	QSPSetCallBack(QSP_CALL_OPENGAME, (QSP_CALLBACK)&OpenGame);
	QSPSetCallBack(QSP_CALL_OPENGAMESTATUS, (QSP_CALLBACK)&OpenGameStatus);
	QSPSetCallBack(QSP_CALL_SAVEGAMESTATUS, (QSP_CALLBACK)&SaveGameStatus);
    //TODO: implement this?
    //QSP_CALL_DEBUG, /* void func(QSPString str) */
}

void QSPCallBacks::DeInit()
{
	CloseFile(qspStringFromPair(0, 0));
    //FMOD_System_Close(m_sys);
    //FMOD_System_Release(m_sys);
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
	QSPString strVal;
	QSPListItem items[MAX_LIST_ITEMS];
	if (m_frame->IsQuit()) return;
	// -------------------------------
	isScroll = !(QSPGetVarValues(QSP_STATIC_STR(QSP_FMT("DISABLESCROLL")), 0, &numVal, &strVal) && numVal);
	isCanSave = !(QSPGetVarValues(QSP_STATIC_STR(QSP_FMT("NOSAVE")), 0, &numVal, &strVal) && numVal);
	m_isHtml = QSPGetVarValues(QSP_STATIC_STR(QSP_FMT("USEHTML")), 0, &numVal, &strVal) && numVal;
	// -------------------------------
	m_frame->GetVars()->SetIsHtml(m_isHtml);
	if (QSPIsVarsDescChanged())
	{
		QSPString varsDesc = QSPGetVarsDesc();
        m_frame->GetVars()->SetText(QSPTools::qspStrToQt(varsDesc), isScroll);
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
		QSPString mainDesc = QSPGetMainDesc();
        m_frame->GetDesc()->SetText(QSPTools::qspStrToQt(mainDesc), isScroll);
	}
	// -------------------------------
	m_frame->GetActions()->SetIsHtml(m_isHtml);
	m_frame->GetActions()->SetIsShowNums(m_frame->IsShowHotkeys());
	if (QSPIsActionsChanged())
	{
		int actionsCount = QSPGetActions(items, MAX_LIST_ITEMS);
		m_frame->GetActions()->BeginItems();
		for (i = 0; i < actionsCount; ++i)
            m_frame->GetActions()->AddItem(QSPTools::qspStrToQt(items[i].Image), QSPTools::qspStrToQt(items[i].Name));
		m_frame->GetActions()->EndItems();
	}
	m_frame->GetActions()->SetSelection(QSPGetSelActionIndex());
	m_frame->GetObjects()->SetIsHtml(m_isHtml);
	if (QSPIsObjectsChanged())
	{
		int objectsCount = QSPGetObjects(items, MAX_LIST_ITEMS);
		m_frame->GetObjects()->BeginItems();
		for (i = 0; i < objectsCount; ++i)
            m_frame->GetObjects()->AddItem(QSPTools::qspStrToQt(items[i].Image), QSPTools::qspStrToQt(items[i].Name));
		m_frame->GetObjects()->EndItems();
	}
	m_frame->GetObjects()->SetSelection(QSPGetSelObjectIndex());
	// -------------------------------
	if (QSPGetVarValues(QSP_STATIC_STR(QSP_FMT("BACKIMAGE")), 0, &numVal, &strVal) && strVal.Str && strVal.Str != strVal.End)
        m_frame->GetDesc()->LoadBackImage(m_gamePath + QSPTools::qspStrToQt(strVal));
	else
        m_frame->GetDesc()->LoadBackImage(QString(""));
	// -------------------------------
	if (isRedraw)
	{
		m_frame->ApplyParams(); //Moved to inside avoid constant update?
		m_frame->EnableControls(false, true);
        //m_frame->Update();
        QCoreApplication::processEvents();
		if (m_frame->IsQuit()) return;
		m_frame->EnableControls(true, true);
	}
    m_frame->GetGameMenu()->setEnabled(isCanSave);
}

void QSPCallBacks::SetInputStrText(QSPString text)
{
	if (m_frame->IsQuit()) return;
    m_frame->GetInput()->SetText(QSPTools::qspStrToQt(text));
}

QSP_BOOL QSPCallBacks::IsPlay(QSPString file)
{
    //FMOD_BOOL playing = FALSE;
//	QSPSounds::iterator elem = m_sounds.find(wxFileName(wxString(file.Str, file.End), wxPATH_DOS).GetFullPath().Upper());
//	if (elem != m_sounds.end())
//		FMOD_Channel_IsPlaying(((QSPSound *)(&elem->second))->Channel, &playing);
//	return (playing == TRUE);
    return QSP_FALSE;
}

void QSPCallBacks::CloseFile(QSPString file)
{
	if (file.Str)
	{
        QSPSounds::iterator elem = m_sounds.find(QFileInfo(QSPTools::qspStrToQt(file)).absoluteFilePath().toUpper());
		if (elem != m_sounds.end())
		{
            elem.value().Free();
            //((QSPSound *)(&elem->second))->Free();
			m_sounds.erase(elem);
		}
	}
	else
	{
		for (QSPSounds::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
            i.value().Free();
            //((QSPSound *)(&i->second))->Free();
		m_sounds.clear();
	}
}

void QSPCallBacks::PlayFile(QSPString file, int volume)
{
//	FMOD_SOUND *newSound;
//	FMOD_CHANNEL *newChannel;
//	QSPSound snd;
//	if (SetVolume(file, volume)) return;
//	CloseFile(file);
//	wxString strFile(wxFileName(wxString(file.Str, file.End), wxPATH_DOS).GetFullPath());
//	#if defined(__WXMSW__) || defined(__WXOSX__)
//	if (!FMOD_System_CreateSound(m_sys, wxConvFile.cWX2MB(strFile.c_str()), FMOD_SOFTWARE | FMOD_CREATESTREAM, 0, &newSound))
//	#else
//	FMOD_CREATESOUNDEXINFO exInfo;
//	memset(&exInfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
//	exInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
//	wxString dlsPath(QSPTools::GetAppPath() + QSP_MIDIDLS);
//	wxCharBuffer dlsCharPath(wxConvFile.cWX2MB(dlsPath.c_str()));
//	exInfo.dlsname = dlsCharPath;
//	if (!FMOD_System_CreateSound(m_sys, wxConvFile.cWX2MB(strFile.c_str()), FMOD_SOFTWARE | FMOD_CREATESTREAM, &exInfo, &newSound))
//	#endif
//	{
//		UpdateSounds();
//		FMOD_System_PlaySound(m_sys, FMOD_CHANNEL_FREE, newSound, FALSE, &newChannel);
//		snd.Channel = newChannel;
//		snd.Sound = newSound;
//		snd.Volume = volume;
//		m_sounds.insert(QSPSounds::value_type(strFile.Upper(), snd));
//		SetVolume(file, volume);
//	}
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
	if (m_frame->IsQuit()) return;
    bool isSave = m_frame->GetGameMenu()->isEnabled();
	bool isBreak = false;
	m_frame->EnableControls(false, true);
	int i, count = msecs / 50;
	for (i = 0; i < count; ++i)
	{
        QThread::msleep(50);
        //m_frame->Update();
        QCoreApplication::processEvents();
		if (m_frame->IsQuit() ||
			m_frame->IsKeyPressedWhileDisabled())
		{
			isBreak = true;
			break;
		}
	}
	if (!isBreak)
	{
        QThread::msleep(msecs % 50);
        //m_frame->Update();
        QCoreApplication::processEvents();
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

void QSPCallBacks::Msg(QSPString str)
{
	if (m_frame->IsQuit()) return;
	RefreshInt(QSP_FALSE);
    QspMsgDlg dialog(m_frame->GetDesc()->GetBackgroundColour(),
		m_frame->GetDesc()->GetForegroundColour(),
		m_frame->GetDesc()->GetTextFont(),
        "Info", //caption
        QSPTools::qspStrToQt(str),
		m_isHtml,
        m_gamePath,
        m_frame
	);
	m_frame->EnableControls(false);
    dialog.exec();
	m_frame->EnableControls(true);
}

int QSPCallBacks::ShowMenu(QSPListItem *items, int count)
{
	if (m_frame->IsQuit()) return -1;
	m_frame->EnableControls(false);
	m_frame->DeleteMenu();
	for (int i = 0; i < count; ++i)
        m_frame->AddMenuItem(QSPTools::qspStrToQt(items[i].Name), QSPTools::qspStrToQt(items[i].Image));
	int index = m_frame->ShowMenu();
	m_frame->EnableControls(true);
	return index;
}

void QSPCallBacks::Input(QSPString text, QSP_CHAR *buffer, int maxLen)
{
	if (m_frame->IsQuit()) return;
	RefreshInt(QSP_FALSE);
//	QSPInputDlg dialog(m_frame,
//		wxID_ANY,
//		m_frame->GetDesc()->GetBackgroundColour(),
//		m_frame->GetDesc()->GetForegroundColour(),
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
    QString inputText = QInputDialog::getMultiLineText(m_frame, "Input data", QSPTools::qspStrToQt(text));
    c16sncpy(buffer, (QSP_CHAR *)(inputText.utf16()), maxLen);
}

void QSPCallBacks::ShowImage(QSPString file)
{
	if (m_frame->IsQuit()) return;
    m_frame->GetImgView()->OpenFile(QSPTools::qspStrToQt(file)); //NOTE: will not display image if file is not found
    //m_frame->GetImgView()->setVisible(true);
}

void QSPCallBacks::OpenGame(QSPString file, QSP_BOOL isNewGame)
{
	if (m_frame->IsQuit()) return;
	if (QSPLoadGameWorld(file, isNewGame) && isNewGame)
	{
        QFileInfo fileName(QSPTools::qspStrToQt(file));
        m_gamePath = fileName.canonicalPath();
        if(!m_gamePath.endsWith('/')) m_gamePath+="/";
		m_frame->UpdateGamePath(m_gamePath);
	}
}

void QSPCallBacks::OpenGameStatus(QSPString file)
{
	if (m_frame->IsQuit()) return;
	if (file.Str)
	{
        QFileInfo fileInfo(QSPTools::qspStrToQt(file));
        if ( fileInfo.exists() && fileInfo.isFile() ) QSPOpenSavedGame(file, QSP_FALSE);
	}
	else
	{
        m_frame->EnableControls(false);
        QString path = QFileDialog::getOpenFileName(m_frame, "Select saved game file", m_frame->GetLastPath(), "Saved game files (*.sav)");
        m_frame->EnableControls(true);
        if (!path.isEmpty())
		{
            m_frame->SetLastPath(QFileInfo(path).canonicalPath());
            QSPOpenSavedGame(qspStringFromQString(path), QSP_FALSE);
		}
	}
}

void QSPCallBacks::SaveGameStatus(QSPString file)
{
	if (m_frame->IsQuit()) return;
	if (file.Str)
		QSPSaveGame(file, QSP_FALSE);
	else
	{
        m_frame->EnableControls(false);
        QString path = QFileDialog::getSaveFileName(m_frame, "Select file to save", m_frame->GetLastPath(), "Saved game files (*.sav)");
		m_frame->EnableControls(true);
        if (!path.isEmpty())
		{
            m_frame->SetLastPath(QFileInfo(path).canonicalPath());
            QSPSaveGame(qspStringFromQString(path), QSP_FALSE);
		}
	}
}

bool QSPCallBacks::SetVolume(QSPString file, int volume)
{
//	if (!IsPlay(file)) return false;
//	QSPSounds::iterator elem = m_sounds.find(wxFileName(wxString(file.Str, file.End), wxPATH_DOS).GetFullPath().Upper());
//	QSPSound *snd = (QSPSound *)&elem->second;
//	snd->Volume = volume;
//	FMOD_Channel_SetVolume(snd->Channel, (float)(m_volumeCoeff * volume) / 100);
	return true;
}

void QSPCallBacks::SetOverallVolume(float coeff)
{
//	QSPSound *snd;
//	FMOD_BOOL playing = FALSE;
//	if (coeff < 0.0)
//		coeff = 0.0;
//	else if (coeff > 1.0)
//		coeff = 1.0;
//	m_volumeCoeff = coeff;
//	for (QSPSounds::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
//	{
//		snd = (QSPSound *)&i->second;
//		FMOD_Channel_IsPlaying(snd->Channel, &playing);
//		if (playing)
//			FMOD_Channel_SetVolume(snd->Channel, (float)(m_volumeCoeff * snd->Volume) / 100);
//	}
}

void QSPCallBacks::UpdateSounds()
{
//	QSPSound *snd;
//	FMOD_BOOL playing = FALSE;
//	QSPSounds::iterator i = m_sounds.begin();
//	while (i != m_sounds.end())
//	{
//		snd = (QSPSound *)&i->second;
//		FMOD_Channel_IsPlaying(snd->Channel, &playing);
//		if (playing)
//			++i;
//		else
//		{
//			snd->Free();
//			m_sounds.erase(i++);
//		}
//	}
}
