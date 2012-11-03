#include "ListWidget.h"
#include "ListWidgetItem.h"
#include "ListWidgetItem_Form.h"
#include <QDropEvent>

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->initSetting();
    this->initData();
    this->initGui();
}

void ListWidget::slotItemPressed(QListWidgetItem *p)
{
    ListWidgetItem *pItem = static_cast<ListWidgetItem *>(p);
    emit signalPressListWidgetItem(pItem->pWidget_->getName());
}

void ListWidget::initSetting()
{
    //this->setMouseTracking(false);
    //设置无焦点
    this->setFocusPolicy(Qt::NoFocus);
    //设置选择模式为单选
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    //启动拖放
    this->setDragEnabled(true);
    //接受拖放
    this->setAcceptDrops(true);
    //设置显示将要被放置的位置
    this->setDropIndicatorShown(true);
    //设置拖放模式为移动项目, 如果不设置,默认为复制项目
    //如果设置了这个属性的话,相当的蛋疼,比如dragLeaveEvent,dragMoveEvent都有问题了
    //拖放的时候drag和drop是一起的
    //this->setDragDropMode(QAbstractItemView::DragOnly);
}

void ListWidget::initData()
{
    connect(this, SIGNAL(itemPressed(QListWidgetItem*)), SLOT(slotItemPressed(QListWidgetItem*)));
}

void ListWidget::initGui()
{
}

void ListWidget::dropEvent(QDropEvent *event)
{
    //为了防止自身拖拽
    if(event->source() == this)
    {
        return;
    }
}
