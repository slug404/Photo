#include "ListWidgetLayout.h"
#include <QDebug>
#include <QDropEvent>

ListWidgetLayout::ListWidgetLayout(QWidget *parent) :
    QListWidget(parent)
{
    this->initSetting();
    this->initData();
    this->initGui();
}

void ListWidgetLayout::dropEvent(QDropEvent *event)
{
    if(event->source() == this)
    {
        QListWidget::dropEvent(event);
        for(int i = 0; i != this->count(); ++i)
        {
            qDebug() << this->item(i)->text();
            emit signalResetZvalue(this->item(i)->text(), -i);
        }
    }
}

void ListWidgetLayout::slotClicked(QListWidgetItem *p)
{
    emit signalSetFocus(p->text());
}

void ListWidgetLayout::initSetting()
{
    this->setFocusPolicy(Qt::NoFocus);

    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->setDragEnabled(true);

    this->setAcceptDrops(true);

    this->setDropIndicatorShown(true);

    this->setDragDropMode(QAbstractItemView::InternalMove);
}

void ListWidgetLayout::initData()
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotClicked(QListWidgetItem*)));
}

void ListWidgetLayout::initGui()
{
}
