#ifndef WIDGETSHOWSCENE_H
#define WIDGETSHOWSCENE_H

#include <QWidget>

class GraphicsScene;
class ListWidget;
class WidgetShowScene : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetShowScene(QWidget *parent = 0);
    void setGraphicsScene(GraphicsScene *p);

signals:

public slots:

private:

private:
    GraphicsScene *pGraphicsScene_;
};

#endif // WIDGETSHOWSCENE_H
