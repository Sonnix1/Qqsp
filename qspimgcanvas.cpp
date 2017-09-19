#include "qspimgcanvas.h"

#include <QFileInfo>

QspImgCanvas::QspImgCanvas(QWidget *parent) : QDialog(parent)
{
    m_posX = m_posY = 0;
    m_isAnim = false;
    setWindowTitle(tr("Image"));
    sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    label_image.sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    label_image.sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    layout.addWidget(&label_image);
    setLayout(&layout);
    setVisible(false);
    setModal(false);
}

QspImgCanvas::~QspImgCanvas()
{

}

bool QspImgCanvas::OpenFile(const QString &fileName)
{
    bool ret;
    QFileInfo file(fileName);
    QString path(file.absoluteFilePath());
    setWindowTitle(path);
    if (m_path != path || path.isEmpty())
    {
        if (file.exists() && file.isFile())
        {
            if ( false /* m_isAnim = m_animation->LoadFile(path) */)
            {
                //m_animation->Show();
                ret = true;
            }
            else
            {
                //m_animation->Hide();
                ret = m_image.load(path);
                if(ret)
                    label_image.setPixmap(m_image);
            }
            if (ret)
            {
                if (m_isAnim)
                {
                    //m_animation->Play();
                }
                else
                {
                    //Refresh();
                }
                m_path = path;
                setVisible(true);
            }
            return ret;
        }
        return false;
    }
    if(!fileName.isEmpty())
    {
        setVisible(true);
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

bool QspImgCanvas::SetBackgroundColour(const QColor &color)
{
    //wxWindow::SetBackgroundColour(color);
    //m_animation->SetBackgroundColour(color);
    return true;
}

void QspImgCanvas::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        setVisible(false);
        event->ignore();
    }
}

void QspImgCanvas::closeEvent(QCloseEvent *event)
{
    setVisible(false);
    event->ignore();
}
