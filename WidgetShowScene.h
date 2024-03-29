#ifndef WIDGETSHOWSCENE_H
#define WIDGETSHOWSCENE_H

#include <QWidget>

class GraphicsScene;
class GraphicsView;
class ListWidget;
class WidgetShowScene : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetShowScene(QWidget *parent = 0);
    void setGraphicsScene(GraphicsScene *p);
    void setGraphicsView(GraphicsView *p);

private:
    void initSetting();

private:
    GraphicsScene *pGraphicsScene_;
    GraphicsView *pView;
};

#endif // WIDGETSHOWSCENE_H
