#ifndef VIDEOFRAMEPROCESSOR_H
#define VIDEOFRAMEPROCESSOR_H

#include <QObject>
#include <QAbstractVideoSurface>
#include <QAbstractVideoBuffer>
#include <QMediaPlayer>
#include <QVideoFrame>
#include <QList>
#include <QSize>
#include <QImage>
#include <QString>

class VideoFrameProcessor : public QAbstractVideoSurface
{
    Q_OBJECT

signals:
    void medialLoaded();
    void newFrame();

public:
    explicit VideoFrameProcessor(const QString &filepath, const QString& filename, QObject *parent = 0);
    ~VideoFrameProcessor();
    bool present(const QVideoFrame& frame);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;
    bool IsValid();
    QImage curFrame;
    QMediaPlayer *mediaPlayer;
    QSize mediaResolution;
    QString m_path;
private:

private slots:
    void OnMediaStatusChanged(QMediaPlayer::MediaStatus status);
};

#endif // VIDEOFRAMEPROCESSOR_H
