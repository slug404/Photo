#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
class ListWidget;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent);
    explicit GraphicsScene(const QRectF &sceneRect, QObject *parent = 0);

    void setListWidget(ListWidget *p) {pListWidget_ = p;}
    void saveFile(const QString path);
    void setImage(const QPixmap &image) { image_ = image;}
    void addImage();
signals:
    void signalCreateItem(const QString &name, const QPointF &pos);

public slots:
    void slotCreateItem(const QString &path, const QPointF &pos);
    void slotAddImage();

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

private:
    void initSetting();
    void initData();
    void initGui();

private:
    QString name_;
    QRect rect_;
    ListWidget *pListWidget_;
    QGraphicsPixmapItem *pPixmapItem_;
    QPixmap image_;
};

#endif // GRAPHICSSCENE_H
