#include "widgetshowscene.h"
#include <QGridLayout>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsView>
#include <QResizeEvent>
#include "GraphicFramework/GraphicsScene.h"
#include "GraphicFramework/GraphicsView.h"
#include "GraphicFramework/GraphicsItem.h"

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
    //pView->setFixedSize(pGraphicsScene_->rect_.size());
    //pView->setVerticalScrollBar();
    pView->setAcceptDrops(true);
    //pView->installEventFilter(this);

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    //这一句看来是有用的
    //layout->setMargin(0);
    this->setLayout(layout);
    layout->addWidget(pView);
    //也不是这个
}
