#include "videolabel.h"

#include <QUrl>
#include <QCoreApplication>
//#include <QThread>

VideoLabel::VideoLabel(QString path, QString filename, QWidget *parent) : QLabel(parent)
{
    m_path = path;
    m_filename = filename;
    setScaledContents(true);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    playlist.setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    playlist.addMedia(QUrl::fromLocalFile(m_path + m_filename));
    mediaPlayer.setPlaylist(&playlist);
    mediaPlayer.setVideoOutput(&vfp);
    mediaPlayer.play();
    while(!vfp.hasFrame && mediaPlayer.error() != QMediaPlayer::InvalidMedia && vfp.error() == QAbstractVideoSurface::NoError)
    {
        QCoreApplication::processEvents();
        //QThread::msleep(4);
    }

    if(mediaPlayer.error() != QMediaPlayer::InvalidMedia && vfp.error() == QAbstractVideoSurface::NoError)
    {
        connect(&vfp, SIGNAL(newFrame(QImage)), this, SLOT(OnNewFrame(QImage)));
        m_videoError = false;
    }
    else
    {
        m_videoError = false;
    }
}

VideoLabel::~VideoLabel()
{

}

void VideoLabel::OnNewFrame(QImage newVideoFrame)
{
    setPixmap(QPixmap::fromImage(newVideoFrame));
}
