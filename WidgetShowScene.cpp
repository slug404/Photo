#include "WidgetShowScene.h"
#include <QGridLayout>
#include <QDebug>
#include "GraphicsScene.h"
#include "GraphicsView.h"
#include "GraphicsItem.h"

WidgetShowScene::WidgetShowScene(QWidget *parent) :
    QWidget(parent)
{
}

void WidgetShowScene::setGraphicsScene(GraphicsScene *p)
{
    pGraphicsScene_ = p;
    GraphicsView *pView = new GraphicsView(pGraphicsScene_, this);
    pView->setCacheMode(QGraphicsView::CacheBackground);
    pView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    pView->showNormal();
    pView->setFrameShape(QFrame::NoFrame);

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
