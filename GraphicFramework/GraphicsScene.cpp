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

GraphicsScene::GraphicsScene(const QRectF &sceneRect, QObject *parent)
    :QGraphicsScene(sceneRect, parent)
    , name_("")
    , index_(0)
{
}

GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{
}

bool GraphicsScene::saveFile()
{
    map_index_isSave_[index_] = 1;
    //保存Scene中的图片
    QPixmap image(rect_.size());
    QPainter painter(&image);
    this->render(&painter);   //关键函数

    QPixmap tmp = image.scaled(QSize(image.width()/3, image.height()/3));
    tmp.save(tr("./") +QString::number(index_) + ".jpg", "jpg");

    (*pVectorImage_)[index_] = image.toImage();

    //还要保存临时的
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    //图片(QPixmap), 位置(QPointF), Z轴(int), 矩阵(QTransform)
    qDebug() << list_pixmap_.size();
    out << list_pixmap_.size();
    for(int i = 0; i != list_pixmap_.size(); ++i)
    {
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        list_pixmap_.at(i)->getImage().save(&buffer, "jpg");
        out << ba << list_pixmap_.at(i)->scenePos()
            << list_pixmap_.at(i)->zValue() << list_pixmap_.at(i)->zoom
            << list_pixmap_.at(i)->rotation();
        qDebug() << list_pixmap_.at(i)->scenePos()
                 << list_pixmap_.at(i)->zValue() <<  list_pixmap_.at(i)->zoom
                 << list_pixmap_.at(i)->rotation();
    }

    QFile file(tr("./")+QString::number(index_)+".tmp");
    if(file.open(QFile::WriteOnly))
    {
        file.write(bytes);
    }
    file.close();

    return true;
}

void GraphicsScene::loadTmpSaveFile(int index)
{
    QFile file(tr("./")+QString::number(index)+".tmp");
    if(file.open(QFile::ReadOnly))
    {
        QByteArray bytes = file.readAll();
        QDataStream in(&bytes, QIODevice::ReadOnly);
        int num;
        in >> num;
        qDebug() << "照片数:" << num;
        for(int i = 0; i != num; ++i)
        {
            QPointF pos;
            int zValue;
            qreal ss;
            qreal ro;
            QByteArray ba;
            in >> ba >> pos >> zValue >> ss >> ro;
            qDebug() << pos << zValue << ss << ro;
            QPixmap pix;
            pix.loadFromData(ba, "jpg");
            QLabel *p = new QLabel;
            p->setPixmap(pix);
            p->show();
        }
    }
    else
    {
        QMessageBox::warning(0, tr("失败"), tr("加载保存的中间文件失败, 该月份将会被从新编辑"));
    }
}

void GraphicsScene::setImage(const QString &name, int index, const QPixmap &image)
{
    name_ = name;
    index_ = index;
    image_ = image;
    qDebug() <<index_;
}

void GraphicsScene::addImage()
{
    list_pixmap_.clear();
    rect_ = QRect(0, 0, image_.width(), image_.height());
    //this->setSceneRect(image_.rect());
    QGraphicsPixmapItem *p = this->addPixmap(image_);
    this->setSceneRect(image_.rect());
    //这到底是怎么了?
    p->setFlag(QGraphicsItem::ItemIsMovable, false);
    p->setData(Qt::UserRole, tr("bg"));
    this->addItem(p);
    this->update();
    emit signalAdjustSize(image_.size());

    //还原
    qDebug() << "还原" << index_;
    if(map_index_isSave_[index_] == 1)
    {
        loadTmpSaveFile(index_);
    }
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
                //GraphicsItem *pItem = static_cast<GraphicsItem *>(p);
                this->removeItem(p);
                //pItem->deleteLater();
            }
            //qDeleteAll(this->items());
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
    //qDebug() << "drag enter";
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
    foreach (QGraphicsItem *p, this->items())
    {
        //GraphicsItem *pItem = static_cast<GraphicsItem *>(p);
        this->removeItem(p);
        //pItem->deleteLater();
    }
    //qDeleteAll(this->items());
    //通知图层列表清空图层
    emit signalRemoveItem();
    this->addImage();
}
