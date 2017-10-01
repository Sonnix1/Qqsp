#include "videoframeprocessor.h"

#include <QVideoSurfaceFormat>

//#include "private/qvideoframe_p.h"

VideoFrameProcessor::VideoFrameProcessor(QObject *parent) : QAbstractVideoSurface(parent)
{
    hasFrame = false;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(OnFrameTimeout()));
    timer.start(5000);
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
            //curFrame = qt_imageFromVideoFrame(videoFrame);
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
            << QVideoFrame::Format_RGB555
            << QVideoFrame::Format_ARGB8565_Premultiplied
            << QVideoFrame::Format_RGB24;
//            << QVideoFrame::Format_BGRA32
//            << QVideoFrame::Format_BGRA32_Premultiplied
//            << QVideoFrame::Format_BGR32
//            << QVideoFrame::Format_BGR24
//            << QVideoFrame::Format_BGR565
//            << QVideoFrame::Format_BGR555
//            << QVideoFrame::Format_BGRA5658_Premultiplied
//            << QVideoFrame::Format_AYUV444
//            << QVideoFrame::Format_AYUV444_Premultiplied
//            << QVideoFrame::Format_YUV444
//            << QVideoFrame::Format_YUV420P
//            << QVideoFrame::Format_YV12
//            << QVideoFrame::Format_UYVY
//            << QVideoFrame::Format_YUYV
//            << QVideoFrame::Format_NV12
//            << QVideoFrame::Format_NV21
//            << QVideoFrame::Format_IMC1
//            << QVideoFrame::Format_IMC2
//            << QVideoFrame::Format_IMC3
//            << QVideoFrame::Format_IMC4
//            << QVideoFrame::Format_Y8
//            << QVideoFrame::Format_Y16
//            << QVideoFrame::Format_Jpeg
//            << QVideoFrame::Format_CameraRaw
//            << QVideoFrame::Format_AdobeDng;
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
    curFrame = QImage(mediaResolution, QImage::Format_ARGB32);
    curFrame.fill(qRgba(0,0,0,0));
    hasFrame = true;
}
