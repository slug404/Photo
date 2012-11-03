#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
class GraphicsScene;

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
    void on_action_Clear_triggered();

    void slotPressListWidgetItem(const QString &str);
    //插入新的图层信息
    void slotCreateItem(const QString &path);
    void slotItemSizeValueChanged(int value);

private:
    void initSetting();
    void initData();
    void initGui();
    QStringList getComponentsName(const QString &filePath);
    void setListWidgetPointer(ListWidget *p);
    void addPhotoItem(const QPixmap &pix);
    void saveFile(const QString path);

private:
    QString filePath_;
    QStringList templateFilesName_;
    GraphicsScene *pGraphicsScene_;
};

#endif // MAINWINDOW_H
