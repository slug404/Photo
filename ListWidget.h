#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = 0);
    
signals:
    void signalPressListWidgetItem(const QString &str);

public slots:

protected:
    void dropEvent(QDropEvent *event);
private slots:
    void slotItemPressed(QListWidgetItem *p);

private:
    void initSetting();
    void initData();
    void initGui();

private:
    
};

#endif // LISTWIDGET_H
