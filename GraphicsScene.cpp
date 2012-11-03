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

void GraphicsScene::setItemName(const QString &name)
{
    name_ = name;
    qDebug() << name_;
}

void GraphicsScene::addPhotoItem(const QPixmap &pix)
{
    pPixmapItem_ = this->addPixmap(pix);

    pPixmapItem_->setFlag(QGraphicsItem::ItemIsMovable);

    ////实验
    QGraphicsPixmapItem *p1 = addPixmap(QPixmap("./UI/UI_Block_Pin.png"));
    QGraphicsPixmapItem *p2 = addPixmap(QPixmap("./UI/UI_Block_Pin.png"));
    QGraphicsPixmapItem *p3 = addPixmap(QPixmap("./UI/UI_Block_Pin.png"));
    QGraphicsPixmapItem *p4 = addPixmap(QPixmap("./UI/UI_Block_Pin.png"));
    QGraphicsPixmapItem *p5 = addPixmap(QPixmap("./UI/UI_Block_Pin.png"));

//    p1->setFlag(QGraphicsItem::ItemIsMovable);
//    p2->setFlag(QGraphicsItem::ItemIsMovable);
//    p3->setFlag(QGraphicsItem::ItemIsMovable);
//    p4->setFlag(QGraphicsItem::ItemIsMovable);
//    p5->setFlag(QGraphicsItem::ItemIsMovable);
    QList<QGraphicsItem *> items;
    items<< p1 << p2 << p3 << p4 << p5;
    QGraphicsItemGroup *p = this->createItemGroup(items);
    p->setFlag(QGraphicsItem::ItemIsMovable);
    this->update();
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
    list_GraphicsItem_.append(pItem);
    this->addItem(pItem);
}


