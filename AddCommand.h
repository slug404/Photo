#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <QPointF>
#include <QPixmap>

class GraphicsItem;
class GraphicsScene;
class AddCommand : public QUndoCommand
{
public:
    explicit AddCommand(GraphicsItem *pItem, GraphicsScene *pScene, const QPixmap &pix, QUndoCommand *parent = 0);
    ~AddCommand();

    void undo();
    void redo();

private:
    QString createCommandString(GraphicsItem *item, const QPointF &point);

private:
    GraphicsScene *pGraphicsScene_;
    GraphicsItem *pGraphicsItem_;
    QPointF initPos_;
};

#endif // ADDCOMMAND_H
