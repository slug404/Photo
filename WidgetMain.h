#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include "ui_WidgetMain.h"

class WidgetMain : public QWidget, private Ui::WidgetMain
{
    Q_OBJECT
    
public:
    explicit WidgetMain(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);
private slots:
    void on_pushButtonOpenTemplate_clicked();
    void on_pushButtonAddPhoto_clicked();
    void on_pushButtonForward_clicked();
    void on_pushButtonUndo_clicked();
    void on_pushButtonBackward_clicked();
    void on_pushButtonRedu_clicked();
    void on_pushButtonDelete_clicked();
    void on_pushButtonSendFile_clicked();
    void on_pushButtonBuild_clicked();
    void on_pushButtonSaveAll_clicked();
};

#endif // WIDGETMAIN_H
