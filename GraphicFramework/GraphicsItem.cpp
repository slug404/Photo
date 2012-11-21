#include "GraphicsItem.h"
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <math.h>
#include <qmath.h>
#include "AnchorItem.h"

GraphicsItem::GraphicsItem(const QRect &rect, const QPixmap &bgImage, QObject *parent) :
    bgRect_(rect), bgPixmap_(bgImage), QObject(parent)
{
    initSetting();
    initData();
    initGui();
}

void GraphicsItem::addMouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->mouseMoveEvent(event);
}

QRectF GraphicsItem::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(0 - penWidth/2 - 16, 0-penWidth/2 - 16, bgRect_.width()+penWidth + 32, bgRect_.height()+penWidth+ 32);
}

void GraphicsItem::setOldTransformation()
{
    this->setTransform(oldTransform_);
}

void GraphicsItem::initSetting()
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setAcceptHoverEvents(true);
}
void GraphicsItem::initData()
{
    srcWidth_ = bgRect_.width();
    offset_ = 5;
    size_ = 4/5;
    rotate_ = 10;
    sum_ = 0;
    oldTransform_ = this->transform();

    //锚点
    pAnchor_0_0_ = new AnchorItem(AnchorItem::ROTATE, this);
    pAnchor_0_1_ = new AnchorItem(AnchorItem::UP, this);
    pAnchor_0_2_ = new AnchorItem(AnchorItem::ROTATE, this);
    pAnchor_1_0_ = new AnchorItem(AnchorItem::LEFT, this);
    pAnchor_1_2_ = new AnchorItem(AnchorItem::RIGHT, this);
    pAnchor_2_0_ = new AnchorItem(AnchorItem::ROTATE, this);
    pAnchor_2_1_ = new AnchorItem(AnchorItem::DOWN, this);
    pAnchor_2_2_ = new AnchorItem(AnchorItem::ROTATE, this);

    //rotate的锚点
    connect(pAnchor_0_0_, SIGNAL(signalDragging(QPointF,QPointF)), this, SLOT(slotRotate2(QPointF,QPointF)));
    connect(pAnchor_0_2_, SIGNAL(signalDragging(QPointF,QPointF)), this, SLOT(slotRotate1(QPointF,QPointF)));
    connect(pAnchor_2_0_, SIGNAL(signalDragging(QPointF,QPointF)), this, SLOT(slotRotate3(QPointF,QPointF)));
    connect(pAnchor_2_2_, SIGNAL(signalDragging(QPointF,QPointF)), this, SLOT(slotRotate4(QPointF,QPointF)));
    //resize的锚点
    connect(pAnchor_0_1_, SIGNAL(signalDragging(QPointF,QPointF)), this, SLOT(slotResizeUp(QPointF,QPointF)));
    connect(pAnchor_1_0_, SIGNAL(signalDragging(QPointF,QPointF)), this, SLOT(slotResizeLeft(QPointF,QPointF)));
    connect(pAnchor_1_2_, SIGNAL(signalDragging(QPointF,QPointF)), this, SLOT(slotResizeRight(QPointF,QPointF)));
    connect(pAnchor_2_1_, SIGNAL(signalDragging(QPointF,QPointF)), this, SLOT(slotResizeDown(QPointF,QPointF)));

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

    //计算中心的位置
    originPoint_ = QPointF(bgRect_.width()/2, bgRect_.height()/2);
}

void GraphicsItem::initGui()
{
}

void GraphicsItem::slotResizeUp(const QPointF &currentPos, const QPointF &startPos)
{
    QPoint newPos = mapFromScene(currentPos).toPoint();
    QPoint oldPos = mapFromScene(startPos).toPoint();

    if(newPos == oldPos)
    {
        return;
    }

    qreal height = (originPoint_.y() - newPos.y() + bgRect_.height()/2) / bgRect_.height();
    qDebug() << height;

    QTransform matrixTmp = this->transform();
    this->setTransform(matrixTmp.scale(height, height));
    srcWidth_ = height;
}

void GraphicsItem::slotResizeDown(const QPointF &currentPos, const QPointF &startPos)
{
    QPoint newPos = mapFromScene(currentPos).toPoint();
    QPoint oldPos = mapFromScene(startPos).toPoint();

    if(newPos == oldPos)
    {
        return;
    }

    qreal height = (newPos.y() - originPoint_.y() + bgRect_.height()/2) / bgRect_.height();
    qDebug() << height;
    QTransform matrixTmp = this->transform();
    this->setTransform(matrixTmp.scale(height, height));
    srcWidth_ = height;
}

void GraphicsItem::slotResizeLeft(const QPointF &currentPos, const QPointF &startPos)
{
    QPoint newPos = mapFromScene(currentPos).toPoint();
    QPoint oldPos = mapFromScene(startPos).toPoint();

    if(newPos == oldPos)
    {
        return;
    }

    qreal width = (originPoint_.x()-newPos.x() + bgRect_.width() / 2) / bgRect_.width();
    qDebug() << width;
    QTransform matrixTmp = this->transform();
    this->setTransform(matrixTmp.scale(width, width));
    srcWidth_ = width;
}

void GraphicsItem::slotResizeRight(const QPointF &currentPos, const QPointF &startPos)
{
    QPoint newPos = mapFromScene(currentPos).toPoint();
    QPoint oldPos = mapFromScene(startPos).toPoint();

    if(newPos == oldPos)
    {
        return;
    }

    qreal width = (newPos.x() - originPoint_.x() + bgRect_.width() / 2) / bgRect_.width();
    qDebug() << width;
    QTransform matrixTmp = this->transform();
    this->setTransform(matrixTmp.scale(width, width));
    srcWidth_ = width;
}

void GraphicsItem::slotRotate1(const QPointF &currentPos, const QPointF &startPos)
{
    QPoint newPos = mapFromScene(currentPos).toPoint();
    QPoint oldPos = mapFromScene(startPos).toPoint();

    if(newPos == oldPos)
    {
        return;
    }
    qreal refAngle = atan2(originPoint_.y(), originPoint_.x());
    qreal newAngle = atan2(originPoint_.y() - newPos.y(), newPos.x() - originPoint_.x());

    this->setTransformOriginPoint(originPoint_ );
    this->setRotation(this->rotation() + 57.29577951308232 * (refAngle-newAngle));
}

void GraphicsItem::slotRotate2(const QPointF &currentPos, const QPointF &startPos)
{
    QPoint newPos = mapFromScene(currentPos).toPoint();
    QPoint oldPos = mapFromScene(startPos).toPoint();

    if(newPos == oldPos)
    {
        return;
    }
    qreal refAngle = atan2(originPoint_.y(), originPoint_.x());
    qreal newAngle = atan2(originPoint_.y() - newPos.y(), originPoint_.x() - newPos.x());

    this->setTransformOriginPoint(originPoint_ );

    this->setRotation(this->rotation() + 57.29577951308232 * (newAngle - refAngle));
}

void GraphicsItem::slotRotate3(const QPointF &currentPos, const QPointF &startPos)
{
    QPoint newPos = mapFromScene(currentPos).toPoint();
    QPoint oldPos = mapFromScene(startPos).toPoint();

    if(newPos == oldPos)
    {
        return;
    }
    qreal refAngle = atan2(originPoint_.y(), originPoint_.x());
    qreal newAngle = atan2(newPos.y()-originPoint_.y(), originPoint_.x() - newPos.x());

    this->setTransformOriginPoint(originPoint_ );
    this->setRotation(this->rotation() + 57.29577951308232 * (refAngle - newAngle));
}

void GraphicsItem::slotRotate4(const QPointF &currentPos, const QPointF &startPos)
{
    QPoint newPos = mapFromScene(currentPos).toPoint();
    QPoint oldPos = mapFromScene(startPos).toPoint();

    if(newPos == oldPos)
    {
        return;
    }
    qreal refAngle = atan2(originPoint_.y(), originPoint_.x());
    qreal newAngle = atan2(newPos.y() - originPoint_.y(), newPos.x() - originPoint_.x());

    this->setTransformOriginPoint(originPoint_ );
    this->setRotation(this->rotation() + 57.29577951308232 * (newAngle - refAngle));
//    qDebug() << refAngle;
//    qDebug() << newAngle;
//    qDebug() << 57.29577951308232 * (newAngle - refAngle);
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
    qDebug() << "我按下了啊";
    if(this->data(Qt::UserRole).toString() == tr("bg"))
    {
        event->ignore();
    }

    oldPos_ = this->pos();

    QGraphicsItem::mousePressEvent(event);
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->data(Qt::UserRole).toString() == tr("bg"))
    {
        event->ignore();
    }

    emit signalNewPoint(this, oldPos_);
    QGraphicsItem::mouseReleaseEvent(event);
}

void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //开始显示锚点
    foreach (QGraphicsItem *p, list_anchorItems_)
    {
        p->show();
    }
    event->accept();
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

