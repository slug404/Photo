#include "WidgetMain.h"

WidgetMain::WidgetMain(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void WidgetMain::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void WidgetMain::on_pushButtonOpenTemplate_clicked()
{

}

void WidgetMain::on_pushButtonAddPhoto_clicked()
{

}

void WidgetMain::on_pushButtonForward_clicked()
{

}

void WidgetMain::on_pushButtonUndo_clicked()
{

}

void WidgetMain::on_pushButtonBackward_clicked()
{

}

void WidgetMain::on_pushButtonRedu_clicked()
{

}

void WidgetMain::on_pushButtonDelete_clicked()
{

}

void WidgetMain::on_pushButtonSendFile_clicked()
{

}

void WidgetMain::on_pushButtonBuild_clicked()
{

}

void WidgetMain::on_pushButtonSaveAll_clicked()
{

}
