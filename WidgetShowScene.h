#ifndef WIDGETSHOWSCENE_H
#define WIDGETSHOWSCENE_H

#include <QWidget>

class GraphicsScene;

class WidgetShowScene : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetShowScene(QWidget *parent = 0);
    
signals:
    
public slots:

private:
    void initSetting();
    void initData();
    void initGui();

private:
    GraphicsScene *pGraphicsScene_;
};

#endif // WIDGETSHOWSCENE_H
