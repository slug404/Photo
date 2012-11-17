#ifndef ANCHORITEM_H
#define ANCHORITEM_H

#include <QGraphicsItem>

struct AnchorItemPrivate;

class AnchorItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    enum Type{UP, DOWN, LEFT, RIGHT, ROTATE};
    explicit AnchorItem(Type type, QGraphicsItem * parent);
    QRectF boundingRect() const;

    //获取属性
    Type anchorType() const;
    int getWidth() const;
    int getHeight() const;

signals:
    void signalDragging(const QPointF & point);
    void signalChangeHorizontal();
    void signalChangeVertical();
    void signalRotate();

public slots:

protected:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

private:
    void initSetting();
    void initData();
    void initGui();

private:
    AnchorItemPrivate *pData_;
    Type type_;
};

#endif // ANCHORITEM_H
