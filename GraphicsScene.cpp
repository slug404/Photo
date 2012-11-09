#include "GraphicsScene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
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

void GraphicsScene::addImage()
{
    this->addPixmap(image_);
    this->update();
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->source() == pListWidget_)
    {
        qDebug() << "drop";
        if(event->mimeData()->hasFormat("myimage/png"))
        {
            qDebug() << "drop success";

            event->setDropAction(Qt::CopyAction);
            event->accept();

            this->addImage();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->ignore();
    }
}

void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "drag enter";
    if(event->mimeData()->hasFormat("myimage/png"))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void GraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "drag enter";
    if(event->mimeData()->hasFormat("myimage/png"))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
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

void GraphicsScene::slotAddImage()
{
    this->addImage();
}


