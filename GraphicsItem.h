#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
class AnchorItem;
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

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

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

    //各个锚点
    AnchorItem *pAnchor_0_0_;
    AnchorItem *pAnchor_0_1_;
    AnchorItem *pAnchor_0_2_;
    AnchorItem *pAnchor_1_0_;
    AnchorItem *pAnchor_1_2_;
    AnchorItem *pAnchor_2_0_;
    AnchorItem *pAnchor_2_1_;
    AnchorItem *pAnchor_2_2_;
    QList<AnchorItem *> list_anchorItems_;
};

#endif // GRAPHICSITEM_H
