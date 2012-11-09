#include "WidgetShowScene.h"
#include <QGridLayout>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "GraphicsScene.h"
#include "GraphicsView.h"
#include "GraphicsItem.h"
#include <QLabel>

WidgetShowScene::WidgetShowScene(QWidget *parent) :
    QWidget(parent)
{
    this->initSetting();
}

void WidgetShowScene::setGraphicsScene(GraphicsScene *p)
{
    pGraphicsScene_ = p;
    pView = new GraphicsView(pGraphicsScene_, this);
    pView->setCacheMode(QGraphicsView::CacheBackground);
    pView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    pView->showNormal();
    pView->setFrameShape(QFrame::NoFrame);
    pView->setAcceptDrops(true);
    pGraphicsScene_->installEventFilter(this);

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

bool WidgetShowScene::eventFilter(QObject *target, QEvent *event)
{
    if(target == pView)
    {
        switch(event->type())
        {
            case QEvent::DragEnter:
            {
                qDebug() << "dragEnter";
                break;
            }
            case QEvent::DragMove:
            {
                qDebug() << "drag move";
                break;
            }
            case QEvent::Drop:
            {
                qDebug() << "drop";
                break;
            }
            default:
            {
                event->ignore();
            }
        }
    }
    else
    {
        event->ignore();
    }
}

void WidgetShowScene::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "drag enter";
    if(event->mimeData()->hasFormat("myimage/png"))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void WidgetShowScene::dragMoveEvent(QDragMoveEvent *event)
{
    //qDebug() << "drag move";
    if(event->mimeData()->hasFormat("myimage/png"))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void WidgetShowScene::dropEvent(QDropEvent *event)
{
    qDebug() << "drop";
    if(event->mimeData()->hasFormat("myimage/png"))
    {
        qDebug() << "drop success";
        QByteArray bytes = event->mimeData()->data("myimage/png");
        QDataStream in(&bytes, QIODevice::ReadOnly);
        QPixmap pix;

        in >> pix;

        pGraphicsScene_->addPixmap(pix);
        pGraphicsScene_->update();

        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void WidgetShowScene::initSetting()
{
    //接受拖放
    this->setAcceptDrops(true);
}
