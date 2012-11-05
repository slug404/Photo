#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
class GraphicsScene;
class QGraphicsItem;

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
    //根据滑块调整ListWidget里面的Item的icon的size
    void slotItemSizeValueChanged(int value);
    //根据ListWidget循序更改Z轴循序
    void slotResetZvalue(const QString &name, int value);
    //设置焦点, 为了接受键盘事件
    void slotSetFcous(const QString &name);

private:
    void initSetting();
    void initData();
    void initGui();
    QStringList getComponentsName(const QString &filePath);
    void setListWidgetPointer(ListWidget *p);
    void saveFile(const QString path);

private:
    QString filePath_;
    QStringList templateFilesName_;
    GraphicsScene *pGraphicsScene_;
    QHash<QString, QGraphicsItem *> hash_Name_GraphicsItem_;
};

#endif // MAINWINDOW_H
