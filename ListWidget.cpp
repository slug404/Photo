#include "ListWidget.h"
#include "ListWidgetItem.h"
#include "ListWidgetItem_Form.h"
#include <QDropEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMimeData>
#include <QPainter>

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->initSetting();
    connect(this, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(slotItemPress(QListWidgetItem*)));
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotSceneAddImage(QListWidgetItem*)));
}

void ListWidget::initSetting()
{
    //this->setMouseTracking(false);
    //设置无焦点
    this->setFocusPolicy(Qt::NoFocus);
    //设置选择模式为单选
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    //启动拖放
    //this->setDragEnabled(true);
    //接受拖放
    //this->setAcceptDrops(true);
    //设置显示将要被放置的位置
    this->setDropIndicatorShown(true);
    //设置拖放模式为移动项目, 如果不设置,默认为复制项目
    //如果设置了这个属性的话,相当的蛋疼,比如dragLeaveEvent,dragMoveEvent都有问题了
    //拖放的时候drag和drop是一起的
    //this->setDragDropMode(QAbstractItemView::DragOnly);
}

void ListWidget::dropEvent(QDropEvent *event)
{
    //为了防止自身拖拽
    if(event->source() == this)
    {
        return;
    }
}

void ListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);

    //哈哈曲线救国
    ListWidgetItem *pItem = static_cast<ListWidgetItem *>(this->currentItem());
    qDebug() << pItem->pWidget_->getName();
    QByteArray dragData;
    QDataStream out(&dragData, QIODevice::WriteOnly);
    QPixmap pix = pItem->pWidget_->getBgImage();
    //    out << pix;

    //将数据放入QMimeData中
    QMimeData *pMimeData = new QMimeData;
    pMimeData->setData("myimage/png", dragData);

    //将QMimeData放入QDrag中
    QDrag *pDrag = new QDrag(this);
    pDrag->setMimeData(pMimeData);
    pDrag->setPixmap(pix);  //移动中显示的图片
    pDrag->setHotSpot(event->pos());    //拖动鼠标指针的位置不变

    QPixmap tmp = pix;
    QPainter painter;           //用来绘制tmp
    painter.begin(&tmp);
    painter.fillRect(pix.rect(), QColor(127, 127, 127, 187));
    painter.end();
    pItem->pWidget_->setPixmap(tmp);

    //执行拖放
    if(pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction))
    {
        //pItem->pWidget_->setImage(pix);
    }
    else
    {
        //pItem->pWidget_->setImage(pix);
    }
}

void ListWidget::slotItemPress(QListWidgetItem *p)
{
    qDebug() << "asdadlakdj";
    ListWidgetItem *pItem = static_cast<ListWidgetItem *>(p);
    emit signalSetPixmap(pItem->pWidget_->getName(), pItem->data(Qt::UserRole).toInt(), pItem->pWidget_->getImage());
}

void ListWidget::slotSceneAddImage(QListWidgetItem *p)
{
    ListWidgetItem *pItem = static_cast<ListWidgetItem *>(p);
    emit signalSetPixmap(pItem->pWidget_->getName(), pItem->data(Qt::UserRole).toInt(), pItem->pWidget_->getImage());
    emit signalSceneAddImage();
}

void ListWidget::slotChangeBgBySave(int index, const QPixmap &pix)
{
    qDebug() <<"保存的时候更改背景" << index;
    ListWidgetItem *p = static_cast<ListWidgetItem *>(this->item(index));
    p->pWidget_->setImage(pix);
}

void ListWidget::slotChangeBgByReset(int index)
{
    qDebug() <<"从新拖拽的时候更改背景" << index;
    ListWidgetItem *p = static_cast<ListWidgetItem *>(this->item(index));
    p->pWidget_->setImage(p->pWidget_->getBgImage());
}
