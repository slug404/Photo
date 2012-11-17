#include "AnchorItem.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

struct AnchorItemPrivate
{
    AnchorItemPrivate()
    {
        startPos_ = QPoint();
        width_ = 16;
        height_ = 16;
    }
    QPointF startPos_;
    int width_;
    int height_;
};

AnchorItem::AnchorItem(Type type, QGraphicsItem * parent)
    : QGraphicsItem(parent)
    , type_(type)
    , pData_(new AnchorItemPrivate())
{
    this->initSetting();
    this->initData();
    this->initGui();
}

QRectF AnchorItem::boundingRect() const
{
    return QRectF(0, 0, 16, 16);
}

AnchorItem::Type AnchorItem::anchorType() const
{
    return type_;
}

int AnchorItem::getWidth() const
{
    return pData_->width_;
}

int AnchorItem::getHeight() const
{
    return pData_->height_;
}

void AnchorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->drawRect(this->boundingRect());
}

void AnchorItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pData_->startPos_ = event->scenePos();
    qDebug() << "锚点响应了";
    event->accept();
}

void AnchorItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "锚点的 mouse move";
    //nice互不干扰 相当的好啊
    //在这里应该对类型进行一个判断, 以便于通知GraphicsItem进行矩阵变换
    if(pData_->startPos_.isNull())
    {
        return;
    }

    emit signalDragging(event->scenePos(), this->scenePos());
    event->accept();
    update();
}


void AnchorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pData_->startPos_ = QPointF();
    update();
}

void AnchorItem::initSetting()
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void AnchorItem::initData()
{
}

void AnchorItem::initGui()
{
}
