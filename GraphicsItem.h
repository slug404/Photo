#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>

class GraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit GraphicsItem(const QRect &rect, const QPixmap &bgImage, QObject *parent = 0);
    enum
    {
        Type = UserType + 1
    };
    int type() const
    {
        return Type;
    }
    //void resetMatrix();
    //返回可用矩形区域
    QRectF boundingRect() const;
    void setOldTransformation();
    
signals:
    
public slots:

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    void initSetting();
    void initData();
    void initGui();

private:
    //外观描述
    QRect bgRect_;
    QPixmap bgPixmap_;
    int offset_;
    qreal size_;
    qreal rotate_;
    qreal sum_;
    QTransform oldTransform_;
    QPointF originPoint_;
};

#endif // GRAPHICSITEM_H
