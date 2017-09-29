#include "videoframeprocessor.h"

#include <QVideoSurfaceFormat>
#include <QTimer>
#include <QEventLoop>
#include <QMediaPlaylist>

VideoFrameProcessor::VideoFrameProcessor(const QString &filepath, const QString &filename, QObject *parent) : QAbstractVideoSurface(parent)
{
    m_path = filepath;
    mediaPlayer = new QMediaPlayer(this);
    connect(mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(OnMediaStatusChanged(QMediaPlayer::MediaStatus)));

    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    playlist->addMedia(QUrl::fromLocalFile(m_path + filename));
    mediaPlayer->setPlaylist(playlist);

    //mediaPlayer->setMedia(QUrl::fromLocalFile(m_path + filename));
    mediaPlayer->setVideoOutput(this);

    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;
    connect(this,  SIGNAL(medialLoaded()), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(500);
    loop.exec();

    mediaPlayer->play();
}

VideoFrameProcessor::~VideoFrameProcessor()
{
    delete mediaPlayer->playlist();
    delete mediaPlayer;
}

bool VideoFrameProcessor::present(const QVideoFrame &frame)
{
    if(frame.isValid())
    {
        QVideoFrame videoFrame(frame);
        if(videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        {

            curFrame = QImage(
                        videoFrame.bits(),
                        videoFrame.width(),
                        videoFrame.height(),
                        videoFrame.bytesPerLine(),
                        QVideoFrame::imageFormatFromPixelFormat(videoFrame.pixelFormat()));
            emit newFrame();
        }
        videoFrame.unmap();
    }

    return true;
}

QList<QVideoFrame::PixelFormat> VideoFrameProcessor::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    return QList<QVideoFrame::PixelFormat>()
            << QVideoFrame::Format_RGB32
            << QVideoFrame::Format_ARGB32
            << QVideoFrame::Format_ARGB32_Premultiplied
            << QVideoFrame::Format_RGB565
            << QVideoFrame::Format_RGB555;
}

bool VideoFrameProcessor::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool VideoFrameProcessor::IsValid()
{
    if(mediaPlayer->error() == QMediaPlayer::NoError)
        return true;
    else
        return false;
}

void VideoFrameProcessor::OnMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::LoadedMedia)
    {
        disconnect(mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(OnMediaStatusChanged(QMediaPlayer::MediaStatus)));
        mediaResolution = mediaPlayer->metaData("Resolution").toSize();
        emit medialLoaded();
    }
    if(status == QMediaPlayer::InvalidMedia)
    {
        //TODO:
    }
}
