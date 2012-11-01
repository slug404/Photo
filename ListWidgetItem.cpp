#include "ListWidgetItem.h"

ListWidgetItem::ListWidgetItem(ListWidgetItem_Form *p, QObject *parent):
    pWidget_(p), QObject(parent)
{
}
