#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QImage>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSize>
#include <QMutex>

#include "videoframeprocessor.h"

class VideoLabel : public QLabel
{
    Q_OBJECT

signals:
    void medialLoaded();

public:
    explicit VideoLabel(QString path, QString filename, QWidget *parent = 0);
    ~VideoLabel();
    bool videoError();
    QSize getResolution() { return vfp.mediaResolution; }
    bool hasFrame() { return m_medialLoaded; }
    bool resolution_set;

private:
    QString m_path;
    QString m_filename;
    VideoFrameProcessor vfp;
    QMediaPlayer mediaPlayer;
    QMediaPlaylist playlist;
    bool m_videoError;
    bool m_medialLoaded;
    QMutex mutex;

private slots:
    void OnNewFrame(QImage newVideoFrame);
};

#endif // VIDEOLABEL_H
