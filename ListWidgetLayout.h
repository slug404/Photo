#ifndef LISTWIDGETLAYOUT_H
#define LISTWIDGETLAYOUT_H

#include <QListWidget>

class ListWidgetLayout : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidgetLayout(QWidget *parent = 0);
    
signals:
    void signalResetZvalue(const QString &name, int index);
    void signalSetFocus(const QString &name);

public slots:
    
protected:
    void dropEvent(QDropEvent *event);

private slots:
    void slotClicked(QListWidgetItem *p);

private:
    void initSetting();
    void initData();
    void initGui();

private:
};

#endif // LISTWIDGETLAYOUT_H
