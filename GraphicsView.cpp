#include "GraphicsView.h"
#include <QDebug>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene, parent)
{
    this->initSetting();
}

void GraphicsView::initSetting()
{
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    //开启抗锯齿
    this->setRenderHint(QPainter::Antialiasing, true);
    //开启背景缓存, 传说可以提升效率
    this->setCacheMode(QGraphicsView::CacheBackground);

//    this->setDragMode(QGraphicsView::RubberBandDrag);
//    this->setOptimizationFlags(QGraphicsView::DontSavePainterState);
//    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
//    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
   // setDragMode(ScrollHandDrag);
}

