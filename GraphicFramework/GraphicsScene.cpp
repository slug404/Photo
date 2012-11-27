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
    , editCount(0)
{
}

GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
  , name_("")
  , index_(0)
  , editCount(0)
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
    emit signalChangeBgBySave(index_, image.scaled(QSize(181, 121)));

    (*pVectorImage_)[index_] = image.toImage();

    //还要保存临时的
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    //图片(QPixmap), 位置(QPointF), Z轴(int), 矩阵(QTransform)
    qDebug() << list_pixmap_.size();
    out << list_pixmap_.size();
    for(int i = 0; i != list_pixmap_.size(); ++i)
    {
        out << list_pixmap_.at(i)->getImage() << list_pixmap_.at(i)->scenePos()
            << list_pixmap_.at(i)->zValue() << list_pixmap_.at(i)->transform()
            << list_pixmap_.at(i)->rotation();
        qDebug() << list_pixmap_.at(i)->scenePos()
                 << list_pixmap_.at(i)->zValue() <<  list_pixmap_.at(i)->transform()
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
            qreal zValue;
            qreal ss;
            qreal ro;
            QPixmap pix;
            QTransform matx;
            in >> pix >> pos >> zValue >> matx >> ro;
            qDebug() << pos << zValue << matx << ro;

            GraphicsItem *p = new GraphicsItem(pix.rect(), pix);
            p->setData(Qt::UserRole, QObject::tr("photo"));
            this->list_pixmap_.append(p);
            p->setPos(pos);
            p->setZValue(zValue);
            p->setTransform(matx);
            p->setRotation(ro);
            this->addItem(p);
            this->update();
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
    if(editCount++)
    {
        emit signalChangeBgByReset(index_);
    }
    else
    {
        emit signalChangeBgByReset(index);
    }

    index_ = index;
    image_ = image;
    qDebug() <<index_;
}

void GraphicsScene::addImage()
{
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
    list_pixmap_.clear();

    //从新加入
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
    this->addImage();
}
