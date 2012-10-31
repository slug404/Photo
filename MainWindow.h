#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_action_O_triggered();

    void on_action_S_triggered();

    void on_action_I_triggered();

private:
    void initSetting();
    void initData();
    void initGui();
    QStringList getComponentsName(const QString &filePath);

private:
    QString filePath_;
    QStringList templateFilesName_;
};

#endif // MAINWINDOW_H
