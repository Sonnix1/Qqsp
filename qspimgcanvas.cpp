#include "qspimgcanvas.h"

#include <QFileInfo>

QspImgCanvas::QspImgCanvas(QWidget *parent) : QWidget(parent)
{
    m_posX = m_posY = 0;
    m_isAnim = false;
    setWindowTitle(tr("Image"));
    setContentsMargins(0,0,0,0);
    label_image.setFrameStyle(QFrame::NoFrame);
    label_image.setFrameShadow(QFrame::Plain);
    label_image.setContentsMargins(0,0,0,0);
    layout.setContentsMargins(0,0,0,0);
    sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    label_image.sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    label_image.sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    //label_image.setScaledContents(true);
    layout.addWidget(&label_image);
    setLayout(&layout);
    label_image.setMinimumSize(50, 50);
    setMinimumSize(50, 50);
    //setVisible(false);
    //setModal(false);
    connect(&m_movie, SIGNAL(frameChanged(int)), this, SLOT(OnNewFrame(int)) );
}

QspImgCanvas::~QspImgCanvas()
{

}

bool QspImgCanvas::OpenFile(const QString &fileName)
{
    bool ret;
    QFileInfo file(m_path + fileName);
    QString path(file.absoluteFilePath());
    setWindowTitle(path);
    m_isAnim = false;
    if (!path.isEmpty())
    {
        if (file.exists() && file.isFile())
        {
            if(path.endsWith(".gif", Qt::CaseInsensitive) || path.endsWith(".mng", Qt::CaseInsensitive))
            {
                m_movie.stop();
                m_movie.setFileName(path);
                m_movie.start();
                if(m_movie.isValid())
                    m_isAnim = true;
            }
            if (m_isAnim)
            {
                //label_image.setMovie(&m_movie);
                ret = true;
            }
            else
            {
                ret = m_image.load(path);
                if(ret)
                    label_image.setPixmap(m_image);
            }
            if (ret)
            {
                resizeEvent(0);
                //setVisible(true);
            }
            return ret;
        }
        return false;
    }
    return true;
}

void QspImgCanvas::RefreshUI()
{
//    if (m_isAnim)
//		m_animation->RefreshUI();
//	else
    //		Refresh();
}

void QspImgCanvas::SetGamePath(const QString &path)
{
    m_path = path;
}

bool QspImgCanvas::SetBackgroundColor(const QColor &color)
{
    QString sheet = QString::fromLatin1("QLabel { background-color : %1 }").arg(color.name());
    label_image.setStyleSheet(sheet);
    //wxWindow::SetBackgroundColor(color);
    //m_animation->SetBackgroundColor(color);
    return true;
}

void QspImgCanvas::resizeEvent(QResizeEvent *event)
{
    if (m_isAnim)
    {
        //label_image.setMovie(&m_movie);
    }
    else
    {
        if(!m_image.isNull())
            label_image.setPixmap(m_image.scaled(label_image.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void QspImgCanvas::OnNewFrame(int frameNumber)
{
    if (m_isAnim)
    {
        if(m_movie.isValid())
        {
            label_image.setPixmap(m_movie.currentPixmap().scaled(label_image.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

//void QspImgCanvas::keyPressEvent(QKeyEvent *event)
//{
//    if(event->key() == Qt::Key_Escape)
//    {
//        setVisible(false);
//        event->ignore();
//    }
//}

//void QspImgCanvas::closeEvent(QCloseEvent *event)
//{
//    setVisible(false);
//    event->ignore();
//}
