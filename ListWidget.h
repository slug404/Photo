#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = 0);
    
signals:
    void signalSetPixmap(const QString &name, int index, const QPixmap &image);
    void signalSceneAddImage();

public slots:
    void slotChangeBgBySave(int index, const QPixmap &pix);
    void slotChangeBgByReset(int index);

protected:
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void slotItemPress(QListWidgetItem *p);
    void slotSceneAddImage(QListWidgetItem *p);

private:
    void initSetting();

private:
    
};

#endif // LISTWIDGET_H
