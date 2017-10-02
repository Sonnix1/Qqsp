#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QImage>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSize>

#include "videoframeprocessor.h"

class VideoLabel : public QLabel
{
    Q_OBJECT

public:
    explicit VideoLabel(QString path, QString filename, QWidget *parent = 0);
    ~VideoLabel();
    bool videoError() { return m_videoError; }
    QSize getResolution() { return vfp.mediaResolution; }

private:
    QString m_path;
    QString m_filename;
    VideoFrameProcessor vfp;
    QMediaPlayer mediaPlayer;
    QMediaPlaylist playlist;
    bool m_videoError;

private slots:
    void OnNewFrame(QImage newVideoFrame);
};

#endif // VIDEOLABEL_H
