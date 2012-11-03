#ifndef LISTWIDGETLAYOUT_H
#define LISTWIDGETLAYOUT_H

#include <QListWidget>

class ListWidgetLayout : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidgetLayout(QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:
    void dropEvent(QDropEvent *event);

private:
    void initSetting();
    void initData();
    void initGui();

private:
};

#endif // LISTWIDGETLAYOUT_H
