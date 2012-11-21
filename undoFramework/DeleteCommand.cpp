#include "DeleteCommand.h"
#include <QGraphicsScene>
#include "GraphicFramework/GraphicsItem.h"
#include "GraphicFramework/GraphicsScene.h"

DeleteCommand::DeleteCommand(GraphicsItem *pItem, GraphicsScene *pScene, QUndoCommand *parent)
    : QUndoCommand(parent)
    , pGraphicsItem_(pItem)
    , pGraphicsScene_(pScene)
{
    setText(QObject::tr("删除 %1")
            .arg(createCommandString(pGraphicsItem_, pGraphicsItem_->pos())));
}

void DeleteCommand::undo()
{
    pGraphicsScene_->addItem(pGraphicsItem_);
    pGraphicsScene_->update();
}

void DeleteCommand::redo()
{
    pGraphicsScene_->removeItem(pGraphicsItem_);
}

QString DeleteCommand::createCommandString(GraphicsItem *item, const QPointF &point)
{
    return QObject::tr("%1 在 (%2, %3)")
            .arg(QString("照片"))
        .arg(point.x()).arg(point.y());
}
