#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = 0);
    
signals:

public slots:

protected:
    void dropEvent(QDropEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:

private:
    void initSetting();

private:
    
};

#endif // LISTWIDGET_H
