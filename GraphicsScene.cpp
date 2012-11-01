#include "GraphicsScene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

GraphicsScene::GraphicsScene(const QRectF &sceneRect, QObject *parent) :
    QGraphicsScene(sceneRect, parent)
{

}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{

}

void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "enter!";
    event->accept();
}
