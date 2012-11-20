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
    //开启抗锯齿
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    //开启背景缓存, 传说可以提升效率
    this->setCacheMode(QGraphicsView::CacheBackground);
    //this->setMouseTracking(true);
}

