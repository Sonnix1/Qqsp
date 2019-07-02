#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QFontDialog>
#include <QColorDialog>
#include <QDirIterator>
#include <QFileInfo>
#include <QCoreApplication>
#include <QTranslator>

#include "mainwindow.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    if(this->parent() == 0)
        return;
    MainWindow *mw = qobject_cast<MainWindow*>(this->parent());
    ui->checkBox_fontSize->setChecked(mw->GetUseFontSize());
    ui->spinBox_fontSize->setValue(mw->GetFontSize());
    ui->checkBox_font->setChecked(mw->GetUseFont());
    m_font = mw->GetFont();
    ui->label_font->setText(m_font.family());
    ui->label_font->setFont(m_font);
    m_backColor = mw->GetBackgroundColor();
    m_linkColor = mw->GetLinkColor();
    m_fontColor = mw->GetForegroundColor();
    ui->label_backColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(QString::number(m_backColor.red()),QString::number(m_backColor.green()), QString::number(m_backColor.blue())));
    ui->label_linkColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(QString::number(m_linkColor.red()),QString::number(m_linkColor.green()), QString::number(m_linkColor.blue())));
    ui->label_fontColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(QString::number(m_fontColor.red()),QString::number(m_fontColor.green()), QString::number(m_fontColor.blue())));
    ui->checkBox_backColor->setChecked(mw->GetUseBackColor());
    ui->checkBox_linkColor->setChecked(mw->GetUseLinkColor());
    ui->checkBox_fontColor->setChecked(mw->GetUseFontColor());
    ui->checkBox_disableVideo->setChecked(mw->GetDisableVideo());
    ui->checkBox_perGameConfig->setChecked(mw->GetPerGameConfig());
    ui->checkBox_autostart->setChecked(mw->GetAutostart());
    ui->horizontalSlider_volume->setValue((int)(mw->GetOverallVolume() * 100));
    ui->checkBox_videoFix->setChecked(mw->GetVideoFix());
    ui->checkBox_HTML5Extras->setChecked(mw->GetAllowHTML5Extras());
    ui->checkBox_CaseInsensitiveFilePath->setChecked(mw->GetUseCaseInsensitiveFilePath());
#ifdef _WIN32
    ui->checkBox_CaseInsensitiveFilePath->setVisible(false);
#endif

    QDirIterator qmIt(QCoreApplication::applicationDirPath(), QStringList() << "*.qm", QDir::Files);
    while(qmIt.hasNext())
    {
        qmIt.next();
        QFileInfo finfo = qmIt.fileInfo();
        QTranslator translator;
        if(translator.load(finfo.completeBaseName(), finfo.absolutePath()))
            ui->comboBox_language->addItem(translator.translate("__LANG__", "__LANGNAME__"), QVariant(translator.translate("__LANG__", "__LANGID__")));
    }
    QDirIterator qmItRC(":/translations/", QStringList() << "*.qm", QDir::Files);
    while(qmItRC.hasNext())
    {
        qmItRC.next();
        QFileInfo finfo = qmItRC.fileInfo();
        QTranslator translator;
        if(translator.load(finfo.completeBaseName(), finfo.absolutePath()))
            ui->comboBox_language->addItem(translator.translate("__LANG__", "__LANGNAME__"), QVariant(translator.translate("__LANG__", "__LANGID__")));
    }
    langid = mw->GetLangID();
    int index = ui->comboBox_language->findData(langid);
    if ( index != -1 )
       ui->comboBox_language->setCurrentIndex(index);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_pushButton_font_clicked()
{
    bool ok;
    QFont new_font = QFontDialog::getFont(&ok, m_font, this);
    if (ok)
    {
        m_font = new_font;
        ui->label_font->setText(m_font.family());
        ui->label_font->setFont(m_font);
    }
}

void OptionsDialog::on_pushButton_backColor_clicked()
{
    QColor new_color = QColorDialog::getColor(m_backColor, this);
    if(new_color.isValid())
    {
        m_backColor = new_color;
        ui->label_backColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(QString::number(m_backColor.red()),QString::number(m_backColor.green()), QString::number(m_backColor.blue())));
    }
}

void OptionsDialog::on_pushButton_fontColor_clicked()
{
    QColor new_color = QColorDialog::getColor(m_fontColor, this);
    if(new_color.isValid())
    {
        m_fontColor = new_color;
        ui->label_fontColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(QString::number(m_fontColor.red()),QString::number(m_fontColor.green()), QString::number(m_fontColor.blue())));
    }
}

void OptionsDialog::on_pushButton_linkColor_clicked()
{
    QColor new_color = QColorDialog::getColor(m_linkColor, this);
    if(new_color.isValid())
    {
        m_linkColor = new_color;
        ui->label_linkColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(QString::number(m_linkColor.red()),QString::number(m_linkColor.green()), QString::number(m_linkColor.blue())));
    }
}

void OptionsDialog::on_pushButton_cancel_clicked()
{
    close();
}

void OptionsDialog::on_pushButton_ok_clicked()
{
    if(this->parent() == 0)
    {
        close();
        return;
    }
    MainWindow *mw = qobject_cast<MainWindow*>(this->parent());

    mw->SetUseFontSize(ui->checkBox_fontSize->isChecked());
    if(ui->checkBox_fontSize->isChecked())
        mw->SetFontSize(ui->spinBox_fontSize->value());
    mw->SetUseFont(ui->checkBox_font->isChecked());
    if(ui->checkBox_font->isChecked())
        mw->SetFont(m_font);
    mw->SetUseBackColor(ui->checkBox_backColor->isChecked());
    mw->SetUseLinkColor(ui->checkBox_linkColor->isChecked());
    mw->SetUseFontColor(ui->checkBox_fontColor->isChecked());
    if(ui->checkBox_backColor->isChecked())
        mw->SetBackgroundColor(m_backColor);
    if(ui->checkBox_linkColor->isChecked())
        mw->SetLinkColor(m_linkColor);
    if(ui->checkBox_fontColor->isChecked())
        mw->SetForegroundColor(m_fontColor);
    mw->SetDisableVideo(ui->checkBox_disableVideo->isChecked());
    mw->SetPerGameConfig(ui->checkBox_perGameConfig->isChecked());
    mw->SetAutostart(ui->checkBox_autostart->isChecked());
    mw->SetVideoFix(ui->checkBox_videoFix->isChecked());
    mw->SetAllowHTML5Extras(ui->checkBox_HTML5Extras->isChecked());
    mw->SetUseCaseInsensitiveFilePath(ui->checkBox_CaseInsensitiveFilePath->isChecked());
    if(ui->comboBox_language->count() > 0)
        mw->SetLangID(ui->comboBox_language->itemData(ui->comboBox_language->currentIndex()).toString());
    if(mw->GetOverallVolume() != ui->horizontalSlider_volume->value() / 100.0f)
        mw->SetOverallVolume(ui->horizontalSlider_volume->value() / 100.0f);

    close();
}
