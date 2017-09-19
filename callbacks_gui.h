#ifndef CALLBACKS_GUI_H
#define CALLBACKS_GUI_H

#include <QString>
#include <qsp_default.h>
#include <QMap>
#include "mainwindow.h"

typedef struct
{
    //FMOD_CHANNEL *Channel;
    //FMOD_SOUND *Sound;
    int Volume;

    void Free() const
    {
        //FMOD_Sound_Release(Sound);
    }
} QSPSound;

typedef QMap<QString, QSPSound> QSPSounds;

static QSPString qspStringFromPair(const QSP_CHAR *start, const QSP_CHAR *end)
{
    QSPString string;
    string.Str = (QSP_CHAR *)start;
    string.End = (QSP_CHAR *)end;
    return string;
}

static QSPString qspStringFromLen(const QSP_CHAR *s, int len)
{
    QSPString string;
    string.Str = (QSP_CHAR *)s;
    string.End = (QSP_CHAR *)s + len;
    return string;
}

static QSPString qspStringFromQString(const QString &s)
{
    QSPString string;
    string.Str = (QSP_CHAR *)s.utf16();
    string.End = (QSP_CHAR *)s.utf16() + s.length();
    return string;
}

/* Helpers */
#define QSP_STATIC_LEN(x) (sizeof(x) / sizeof(QSP_CHAR) - 1)
#define QSP_STATIC_STR(x) (qspStringFromLen(x, QSP_STATIC_LEN(x)))

class QSPCallBacks
{
public:
    // Methods
    static void Init(MainWindow *frame);
    static void DeInit();
    static void SetOverallVolume(float coeff);

    // CallBacks
    static void RefreshInt(QSP_BOOL isRedraw);
    static void SetTimer(int msecs);
    static void SetInputStrText(QSPString text);
    static QSP_BOOL IsPlay(QSPString file);
    static void CloseFile(QSPString file);
    static void PlayFile(QSPString file, int volume);
    static void ShowPane(int type, QSP_BOOL isShow);
    static void Sleep(int msecs);
    static int GetMSCount();
    static void Msg(QSPString str);
    static int ShowMenu(QSPListItem *items, int count);
    static void Input(QSPString text, QSP_CHAR *buffer, int maxLen);
    static void ShowImage(QSPString file);
    static void OpenGame(QSPString file, QSP_BOOL isNewGame);
    static void OpenGameStatus(QSPString file);
    static void SaveGameStatus(QSPString file);
private:
    // Internal methods
    static bool SetVolume(QSPString file, int volume);
    static void UpdateSounds();

    // Fields
    static QString m_gamePath;
    static MainWindow *m_frame;
    static bool m_isHtml;
    //static FMOD_SYSTEM *m_sys;
    static QSPSounds m_sounds;
    static float m_volumeCoeff;

    static const int MAX_LIST_ITEMS = 1000;
};

#endif
