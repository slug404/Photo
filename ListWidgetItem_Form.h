#ifndef LISTWIDGETITEM_FORM_H
#define LISTWIDGETITEM_FORM_H

#include "ui_ListWidgetItem_Form.h"

class ListWidgetItem_Form : public QWidget, private Ui::ListWidgetItem_Form
{
    Q_OBJECT
    
public:
    explicit ListWidgetItem_Form(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);
};

#endif // LISTWIDGETITEM_FORM_H
