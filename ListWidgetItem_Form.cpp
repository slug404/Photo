#include "ListWidgetItem_Form.h"

ListWidgetItem_Form::ListWidgetItem_Form(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void ListWidgetItem_Form::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
