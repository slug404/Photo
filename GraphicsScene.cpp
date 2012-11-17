#include "GraphicsScene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>
#include <QPainter>
#include <QMimeData>
#include <QBuffer>
#include <QFile>
#include <QMessageBox>
#include <QLabel>
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

bool GraphicsScene::saveFile(const QString path)
{
    //保存Scene中的图片
    QPixmap image(rect_.size());
    QPainter painter(&image);
    this->render(&painter);   //关键函数
    qDebug() << path;

//    QLabel *p = new QLabel;
//    p->setPixmap(pix);
//    p->show();
    QPixmap tmp = image.scaled(QSize(image.width()/3, image.height()/3));
    tmp.save(path + ".jpg", "jpg");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "jpg");

    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out << 1 << ba << name_;
    qDebug() << bytes.size();

    QFile file(path + ".zb");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(bytes);
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

void GraphicsScene::addImage()
{
    //this->addPixmap(image_);
//    QLabel *p1 = new QLabel;
//    p1->setPixmap(image_);
//    p1->setWindowTitle("p1");
//    p1->show();
    rect_ = QRect(0, 0, image_.width(), image_.height());

    GraphicsItem *p = new GraphicsItem(image_.rect(), image_);
    p->setFlag(QGraphicsItem::ItemIsMovable, false);
    p->setFlag(QGraphicsItem::ItemIsFocusable, false);
    p->setAcceptHoverEvents(false);

    this->addItem(p);
    this->update();
    emit signalAdjustSize(image_.size());
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
    pItem->setAcceptHoverEvents(false);

    this->addItem(pItem);
    this->update();
    emit signalAdjustSize(pix.size());
}

void GraphicsScene::slotAddImage()
{
    this->addImage();
}
