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
    //菜单操作
    void on_action_O_triggered();
    void on_action_S_triggered();
    void on_action_I_triggered();
    void on_action_Clear_triggered();
    void on_action_undo_triggered();
    void on_action_SendPhoto_triggered();
    void on_action_forward_triggered();
    void on_action_backward_triggered();

    //pushbutton按键操作
    void on_pushButtonAddPhoto_clicked();
    void on_pushButtonOpenTemplate_clicked();
    void on_pushButtonForward_clicked();
    void on_pushButtonAddText_clicked();
    void on_pushButtonBackward_clicked();
    void on_pushButtonClear_clicked();
    void on_pushButtonBuild_clicked();
    void on_pushButtonUndo_clicked();

    //自定义槽
    void slotSetPixmap(const QString &name, const QPixmap &image);
    //插入新的图层信息
    void slotCreateItem(const QString &path);
    //根据滑块调整ListWidget里面的Item的icon的size
    void slotItemSizeValueChanged(int value);
    //根据ListWidget循序更改Z轴循序
    void slotResetZvalue(const QString &name, int value);
    //设置焦点, 为了接受键盘事件
    void slotSetFcous(const QString &name);
    //更改视图的大小
    void slotViewSizeValueChange(int value);

private:
    void initSetting();
    void initData();
    void initGui();
    QStringList getComponentsName(const QString &filePath);
    void setListWidgetPointer(ListWidget *p);

private:
    QString filePath_;
    QStringList templateFilesName_;
    GraphicsScene *pGraphicsScene_;
    GraphicsView *pGraphicsView_;
    QHash<QString, QGraphicsItem *> hash_Name_GraphicsItem_;
};

#endif // MAINWINDOW_H
