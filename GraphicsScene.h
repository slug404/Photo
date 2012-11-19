#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
class ListWidget;
class GraphicsItem;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent);
    explicit GraphicsScene(const QRectF &sceneRect, QObject *parent = 0);

    void setListWidget(ListWidget *p) {pListWidget_ = p;}
    bool saveFile(const QString path);
    void setImage(const QString &name, const QPixmap &image)
    {
        name_ = name;
        image_ = image;
    }
    void addImage();
    void deleteSelectItem();

signals:
    void signalCreateItem(const QString &name, const QPointF &pos);
    void signalAdjustSize(const QSize &size);
    void signalRemoveItem();

public slots:
    void slotAddImage();

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

private slots:

private:
    QString name_;
    QRect rect_;
    ListWidget *pListWidget_;
    QGraphicsPixmapItem *pPixmapItem_;
    QPixmap image_;
};

#endif // GRAPHICSSCENE_H
