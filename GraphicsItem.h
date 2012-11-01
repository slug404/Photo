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

    //返回可用矩形区域
    QRectF boundingRect() const;
    
signals:
    
public slots:

private:
    void initSetting();
    void initData();
    void initGui();

private:
    //外观描述
    QRect bgRect_;
    QPixmap bgPixmap_;
};

#endif // GRAPHICSITEM_H
