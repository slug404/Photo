#include "GraphicsItem.h"
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>

GraphicsItem::GraphicsItem(const QRect &rect, const QPixmap &bgImage, QObject *parent) :
    bgRect_(rect), bgPixmap_(bgImage), QObject(parent)
{
    initSetting();
    initData();
    initGui();
}

QRectF GraphicsItem::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(0 - penWidth/2, 0-penWidth/2, bgRect_.width()+penWidth, bgRect_.height()+penWidth);
}

void GraphicsItem::initSetting()
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->setAcceptHoverEvents(true);
}

void GraphicsItem::initData()
{
    offset_ = 5;
}

void GraphicsItem::initGui()
{
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(bgRect_, bgPixmap_);
}

void GraphicsItem::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "键盘事件";
    switch(event->key())
    {
        case Qt::Key_Up:
        {
            qDebug() << "向上";
            moveBy(0, -offset_);
            break;
        }
        case Qt::Key_Down:
        {
            qDebug() << "向下";
            moveBy(0, offset_);
            break;
        }
        case Qt::Key_Left:
        {
            qDebug() << "向左";
            moveBy(-offset_, 0);
            break;
        }
        case Qt::Key_Right:
        {
            qDebug() << "向右";
            moveBy(offset_, 0);
            break;
        }
        //加号减号 -> 扩大缩小
//        case :
//        {
//            break;
//        }
//        case Qt::Key_Right:
//        {
//            break;
//        }
        default:
        {
            //...
        }
    }
}

