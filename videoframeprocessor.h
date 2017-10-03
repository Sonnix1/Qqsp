#ifndef VIDEOFRAMEPROCESSOR_H
#define VIDEOFRAMEPROCESSOR_H

#include <QObject>
#include <QAbstractVideoSurface>
#include <QAbstractVideoBuffer>
#include <QVideoFrame>
#include <QList>
#include <QSize>
#include <QImage>
#include <QString>
//#include <QTimer>

class VideoFrameProcessor : public QAbstractVideoSurface
{
    Q_OBJECT

signals:
    void medialLoaded();
    void newFrame(QImage);

public:
    explicit VideoFrameProcessor(QObject *parent = 0);
    ~VideoFrameProcessor();
    bool present(const QVideoFrame& frame);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;
    //QImage curFrame;
    QSize mediaResolution;
    bool hasFrame;
//    QTimer timer;
private:

private slots:
    void OnFrameTimeout();
};

#endif // VIDEOFRAMEPROCESSOR_H
