#include "ListWidgetItem.h"
#include "ListWidgetItem_Form.h"
#include <QDebug>

ListWidgetItem::ListWidgetItem(ListWidgetItem_Form *p, QObject *parent):
    pWidget_(p), QObject(parent)
{
}
