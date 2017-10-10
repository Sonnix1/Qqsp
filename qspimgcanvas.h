#ifndef QSPIMGCANVAS_H
#define QSPIMGCANVAS_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QColor>
#include <QPixmap>
#include <QDialog>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMovie>

namespace Ui {
class QspImgCanvas;
}

class QspImgCanvas : public QDialog
{
    Q_OBJECT

public:
    explicit QspImgCanvas(QWidget *parent = 0);
    ~QspImgCanvas();

    // Methods
    bool OpenFile(const QString& fileName);
    void RefreshUI();
    void SetGamePath(const QString& path);

    // Overloaded methods
    virtual bool SetBackgroundColor(const QColor& color);

private:
    // Fields
    bool m_isAnim;
    QPixmap m_image;
    QMovie m_movie;
    QLabel label_image;
    QVBoxLayout layout;
    QString m_path;
    int m_posX;
    int m_posY;
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // QSPIMGCANVAS_H
