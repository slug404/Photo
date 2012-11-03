#include "GraphicsItem.h"
#include <QDebug>
#include <QPainter>

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
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

void GraphicsItem::initData()
{
}

void GraphicsItem::initGui()
{
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(bgRect_, bgPixmap_);
}
