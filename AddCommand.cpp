#include "AddCommand.h"
#include <QGraphicsScene>
#include "GraphicsItem.h"
#include "GraphicsScene.h"
AddCommand::AddCommand(GraphicsScene *p,  const QPixmap &pix, QUndoCommand *parent)
    :QUndoCommand(parent)
    , pGraphicsScene_(p)
{
    pGraphicsItem_ = new GraphicsItem(pix.rect(), pix);
    pGraphicsItem_->setData(Qt::UserRole, QObject::tr("photo"));
    pGraphicsItem_->setAcceptHoverEvents(true);
    pGraphicsScene_->addItem(pGraphicsItem_);
    initPos_ = QPointF(0, 0);
    pGraphicsScene_->update();

    setText(QObject::tr("添加图片 %1")
        .arg(createCommandString(pGraphicsItem_, initPos_)));
}

AddCommand::~AddCommand()
{
    if(!pGraphicsItem_->scene())
    {
        pGraphicsItem_->deleteLater();
    }
}

void AddCommand::undo()
{
    pGraphicsScene_->removeItem(pGraphicsItem_);
    pGraphicsScene_->update();
}

void AddCommand::redo()
{
    pGraphicsScene_->addItem(pGraphicsItem_);
    pGraphicsItem_->setPos(initPos_);
    pGraphicsScene_->clearSelection();
    pGraphicsScene_->update();
}

QString AddCommand::createCommandString(GraphicsItem *item, const QPointF &point)
{
    return QObject::tr("%1 在 (%2, %3)")
            .arg(QString("照片"))
        .arg(point.x()).arg(point.y());
}
