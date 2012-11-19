#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <QUndoCommand>
#include <QPointF>
class GraphicsItem;

class MoveCommand : public QUndoCommand
{
public:
    enum {Id = 2};
    explicit MoveCommand(GraphicsItem *p, const QPointF &oldPos,
                         QUndoCommand *parent = 0);
    
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *other);
    int id() const { return Id; }
private:
    QString createCommandString(GraphicsItem *item, const QPointF &point);

private:
    GraphicsItem *pGraphicsItem_;
    QPointF oldPos_;
    QPointF newPos_;
};

#endif // MOVECOMMAND_H
