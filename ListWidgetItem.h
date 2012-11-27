#ifndef LISTWIDGETITEM_H
#define LISTWIDGETITEM_H

#include <QListWidgetItem>
class ListWidgetItem_Form;
class ListWidgetItem :public QObject, public QListWidgetItem
{
    Q_OBJECT
public:
    explicit ListWidgetItem(ListWidgetItem_Form *p, QObject *parent = 0);

    ListWidgetItem_Form *pWidget_;

signals:
    
public slots:
    
};

#endif // LISTWIDGETITEM_H
