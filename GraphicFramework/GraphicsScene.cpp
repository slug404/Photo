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
}

GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{
}

bool GraphicsScene::saveFile(const QString path)
{
    //保存Scene中的图片
    QPixmap image(rect_.size());
    QPainter painter(&image);
    this->render(&painter);   //关键函数
    qDebug() << path;

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
    rect_ = QRect(0, 0, image_.width(), image_.height());

    GraphicsItem *p = new GraphicsItem(image_.rect(), image_);
    this->setSceneRect(image_.rect());
    //这到底是怎么了?
    p->setAcceptHoverEvents(false);
    p->setFlag(QGraphicsItem::ItemIsMovable, false);
    p->setData(Qt::UserRole, tr("bg"));
    this->addItem(p);
    this->update();
    emit signalAdjustSize(image_.size());
}

void GraphicsScene::deleteSelectItem()
{
    foreach (QGraphicsItem *p, selectedItems())
    {
        GraphicsItem *pItem = static_cast<GraphicsItem *>(p);
        this->removeItem(pItem);
        pItem->deleteLater();
    }
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->source() == pListWidget_)
    {
        qDebug() << "drop";
        if(event->mimeData()->hasFormat("myimage/png"))
        {
            qDebug() << "drop success";

            //清理上一张的内容
            foreach (QGraphicsItem *p, this->items())
            {
                GraphicsItem *pItem = static_cast<GraphicsItem *>(p);
                this->removeItem(pItem);
                pItem->deleteLater();
            }
            //通知图层列表清空图层
            emit signalRemoveItem();    

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

void GraphicsScene::slotAddImage()
{
    this->addImage();
}
