#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = 0);
    
signals:
    void signalPressListWidgetItem(const QPixmap &str);

public slots:

protected:
    void dropEvent(QDropEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void slotItemPressed(QListWidgetItem *p);

private:
    void initSetting();
    void initData();
    void initGui();

private:
    
};

#endif // LISTWIDGET_H
