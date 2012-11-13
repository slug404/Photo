#include "widgetshowscene.h"
#include <QGridLayout>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsView>
#include "GraphicsScene.h"
#include "GraphicsView.h"
#include "GraphicsItem.h"
#include <QLabel>

WidgetShowScene::WidgetShowScene(QWidget *parent) :
    QWidget(parent)
{
    this->initSetting();
}

void WidgetShowScene::initSetting()
{
    //接受拖放
    this->setAcceptDrops(true);
}

void WidgetShowScene::setGraphicsScene(GraphicsScene *p)
{
    pGraphicsScene_ = p;
}

void WidgetShowScene::setGraphicsView(GraphicsView *p)
{
    pView = p;
    pView->setScene(pGraphicsScene_);
    pView->setFrameShape(QFrame::NoFrame);
    pView->setAcceptDrops(true);
    //pView->installEventFilter(this);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(pView);
    layout->setSpacing(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    //这一句看来是有用的
    layout->setMargin(0);
    //也不是这个
    this->setLayout(layout);
}
