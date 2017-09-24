#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QFont>
#include <QColor>
#include <QString>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

private slots:
    void on_pushButton_font_clicked();

    void on_pushButton_backColor_clicked();

    void on_pushButton_fontColor_clicked();

    void on_pushButton_linkColor_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::OptionsDialog *ui;
    QFont m_font;
    QColor m_backColor;
    QColor m_linkColor;
    QColor m_fontColor;
    QString langid;
};

#endif // OPTIONSDIALOG_H
