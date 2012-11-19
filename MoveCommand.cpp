#include "MoveCommand.h"
#include <QGraphicsScene>
#include <QPointF>
#include "GraphicsItem.h"
MoveCommand::MoveCommand(GraphicsItem *p, const QPointF &oldPos, QUndoCommand *parent)
    :QUndoCommand(parent)
    , pGraphicsItem_(p)
    , oldPos_(oldPos)
    , newPos_(p->pos())
{
}

void MoveCommand::undo()
{
    pGraphicsItem_->setPos(oldPos_);
    pGraphicsItem_->scene()->update();
    setText(QObject::tr("移动 %1")
            .arg(createCommandString(pGraphicsItem_, newPos_)));
}

void MoveCommand::redo()
{
    pGraphicsItem_->setPos(newPos_);
    setText(QObject::tr("移动 %1")
            .arg(createCommandString(pGraphicsItem_, newPos_)));
}

bool MoveCommand::mergeWith(const QUndoCommand *other)
{
    const MoveCommand *pMoveCommand = static_cast<const MoveCommand *>(other);
    GraphicsItem *pItem = pMoveCommand->pGraphicsItem_;

    if(pGraphicsItem_ != pItem)
    {
        return false;
    }

    newPos_ = pItem->pos();
    setText(QObject::tr("移动 %1")
            .arg(createCommandString(pGraphicsItem_, newPos_)));
    return false;   //如是true就会合并移动命令
}

QString MoveCommand::createCommandString(GraphicsItem *item, const QPointF &point)
{
    return QObject::tr("%1 在 (%2, %3)")
            .arg(QString("照片"))
        .arg(point.x()).arg(point.y());
}
