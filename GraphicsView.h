#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QGraphicsScene *scene = 0, QWidget *parent = 0);
    
signals:
    
public slots:

protected:
    
};

#endif // GRAPHICSVIEW_H
