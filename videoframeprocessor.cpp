#include "videoframeprocessor.h"

#include <QVideoSurfaceFormat>

VideoFrameProcessor::VideoFrameProcessor(QObject *parent) : QAbstractVideoSurface(parent)
{
    hasFrame = false;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(OnFrameTimeout()));
    timer.start(100);
}

VideoFrameProcessor::~VideoFrameProcessor()
{

}

bool VideoFrameProcessor::present(const QVideoFrame &frame)
{
    if(frame.isValid())
    {
        QVideoFrame videoFrame(frame);
        if(videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        {
            mediaResolution = videoFrame.size();
            curFrame = QImage(
                        videoFrame.bits(),
                        videoFrame.width(),
                        videoFrame.height(),
                        videoFrame.bytesPerLine(),
                        QVideoFrame::imageFormatFromPixelFormat(videoFrame.pixelFormat()));
            if(!hasFrame)
            {
                disconnect(&timer, SIGNAL(timeout()), this, SLOT(OnFrameTimeout()));
                timer.stop();
                mediaResolution = videoFrame.size();
                hasFrame = true;
            }
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

void VideoFrameProcessor::OnFrameTimeout()
{
    mediaResolution.setWidth(640);
    mediaResolution.setHeight(480);
    hasFrame = true;
}
