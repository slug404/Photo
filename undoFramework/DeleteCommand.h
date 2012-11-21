#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>
#include <QPointF>
class GraphicsItem;
class GraphicsScene;

class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(GraphicsItem *pItem, GraphicsScene *pScene, QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    QString createCommandString(GraphicsItem *item, const QPointF &point);

private:
    GraphicsScene *pGraphicsScene_;
    GraphicsItem *pGraphicsItem_;
    
};

#endif // DELETECOMMAND_H
