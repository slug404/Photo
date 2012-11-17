#include "GraphicsItem.h"
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "AnchorItem.h"

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

void GraphicsItem::setOldTransformation()
{
    this->setTransform(oldTransform_);
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
    size_ = 4/5;
    rotate_ = 10;
    sum_ = 0;
    oldTransform_ = this->transform();
    originPoint_ = this->scenePos() + QPointF(this->bgRect_.width()/2, bgRect_.height()/2);

    //锚点
    pAnchor_0_0_ = new AnchorItem(AnchorItem::ROTATE, this);
    pAnchor_0_1_ = new AnchorItem(AnchorItem::UP_DOWN, this);
    pAnchor_0_2_ = new AnchorItem(AnchorItem::ROTATE, this);
    pAnchor_1_0_ = new AnchorItem(AnchorItem::LEFT_RIGHT, this);
    pAnchor_1_2_ = new AnchorItem(AnchorItem::LEFT_RIGHT, this);
    pAnchor_2_0_ = new AnchorItem(AnchorItem::ROTATE, this);
    pAnchor_2_1_ = new AnchorItem(AnchorItem::UP_DOWN, this);
    pAnchor_2_2_ = new AnchorItem(AnchorItem::ROTATE, this);

    //设置好位置
    pAnchor_0_0_->setPos(0-16, 0-16);
    pAnchor_0_1_->setPos((bgRect_.width()-16) / 2, 0-16);
    pAnchor_0_2_->setPos(bgRect_.width(), 0-16);
    pAnchor_1_0_->setPos(0-16, (bgRect_.height()-16)/2);
    pAnchor_1_2_->setPos(bgRect_.width(), (bgRect_.height()-16)/2);
    pAnchor_2_0_->setPos(0-16, bgRect_.height());
    pAnchor_2_1_->setPos((bgRect_.width()-16)/2, bgRect_.height());
    pAnchor_2_2_->setPos(bgRect_.width(), bgRect_.height());

    list_anchorItems_ << pAnchor_0_0_ << pAnchor_0_1_ << pAnchor_0_2_ << pAnchor_1_0_
                         << pAnchor_1_2_ << pAnchor_2_0_ << pAnchor_2_1_ << pAnchor_2_2_;
}

void GraphicsItem::initGui()
{
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->drawPixmap(bgRect_, bgPixmap_);
}

void GraphicsItem::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "键盘事件";
    if(Qt::Key_W == event->key())
    {
        qDebug() << "向上";
        moveBy(0, -offset_);
    }
    else if(Qt::Key_S == event->key())
    {
        qDebug() << "向下";
        moveBy(0, offset_);
    }
    else if(Qt::Key_A == event->key())
    {
        qDebug() << "向左";
        moveBy(-offset_, 0);
    }
    else if(Qt::Key_D == event->key())
    {
        qDebug() << "向右";
        moveBy(offset_, 0);
    }
    //else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Plus)
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Up)
    {
        qDebug() << "放大";
//        this->setScale(size_);
//        this->scene()->update();
//        QTransform *p = new QTransform();
//        p->translate(5, 5);
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Down)
    {
        //加号减号 -> 缩小
        qDebug() << "缩小";
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Left)
    {
        qDebug() << "左转";
        sum_ -= rotate_;
        int i  = (int)sum_ % 360;
        this->setTransformOriginPoint(originPoint_ );
        this->setRotation(i);
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Right)
    {
        qDebug() << "右转";
        sum_ += rotate_;
        int i = (int)sum_% 360;
        this->setTransformOriginPoint(originPoint_ );
        this->setRotation(i);
    }
}

void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mousePressEvent QGraphicsItem";
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    originPoint_ = this->scenePos() + QPointF(this->bgRect_.width()/2, bgRect_.height()/2);
    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "hover enter!!";
    //开始显示锚点
    foreach (QGraphicsItem *p, list_anchorItems_)
    {
        p->show();
    }
}

void GraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "hover leave";
    //隐藏锚点
    foreach (QGraphicsItem *p, list_anchorItems_)
    {
        p->hide();
    }
}

void GraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "hover move";
}
