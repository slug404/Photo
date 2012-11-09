#include "GraphicsScene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>
#include <QPainter>
#include "ListWidget.h"
#include "GraphicsItem.h"

GraphicsScene::GraphicsScene(const QRectF &sceneRect, QObject *parent) :
    name_(""), QGraphicsScene(sceneRect, parent)
{
    this->initSetting();
    this->initData();
    this->initGui();
}

GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{
    this->initSetting();
    this->initData();
    this->initGui();
}

void GraphicsScene::saveFile(const QString path)
{
        //保存Scene中的图片
        QImage image(rect_.size(), QImage::Format_RGB32);
        QPainter painter(&image);
        this->render(&painter);   //关键函数
        qDebug() << path;
        image.save(path+".jpg", "JPG");
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->source() == pListWidget_)
    {
        event->accept();
        event->setDropAction(Qt::CopyAction);
        emit signalCreateItem(tr("./Template/")+name_, event->scenePos());
    }
    else
    {
        QGraphicsScene::dragMoveEvent(event);
    }
}

void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void GraphicsScene::initSetting()
{
}

void GraphicsScene::initData()
{
    connect(this, SIGNAL(signalCreateItem(QString,QPointF)), this, SLOT(slotCreateItem(QString,QPointF)));
}

void GraphicsScene::initGui()
{
}

void GraphicsScene::slotCreateItem(const QString &path, const QPointF &pos)
{
    //加载item
    QPixmap pix(path);
    int w = pix.width();
    int h = pix.height();
    rect_ = QRect(0, 0, w, h);
    GraphicsItem *pItem = new GraphicsItem(rect_ , pix, this);
    //qDebug() << "是这里么";
    pItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    pItem->setFlag(QGraphicsItem::ItemIsFocusable, false);
    this->addItem(pItem);
}


