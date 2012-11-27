#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QVector>
#include <QMap>
class ListWidget;
class GraphicsItem;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent);
    explicit GraphicsScene(const QRectF &sceneRect, QObject *parent = 0);

    void setVectorImage(QVector<QImage> *p){ pVectorImage_ = p;}
    void setListWidget(ListWidget *p) {pListWidget_ = p;}
    bool saveFile();
    void setImage(const QString &name, int index, const QPixmap &image);
    void addImage();
    void deleteSelectItem();
    void loadTmpSaveFile(int index);
    QMap<int, int> map_index_isSave_;
    QList<GraphicsItem *> list_pixmap_;

signals:
    void signalCreateItem(const QString &name, const QPointF &pos);
    void signalAdjustSize(const QSize &size);
    void signalRemoveItem();
    void signalChangeBgBySave(int index, const QPixmap &pix);
    void signalChangeBgByReset(int index);

public slots:
    void slotAddImage();

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

private slots:

private:
    QString name_;
    ListWidget *pListWidget_;
    QGraphicsPixmapItem *pPixmapItem_;
    QPixmap image_;
    int index_;
    QVector<QImage> *pVectorImage_;
    QRect rect_;

    int editCount;
};

#endif // GRAPHICSSCENE_H
