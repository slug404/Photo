#include "ListWidgetItem_Form.h"

ListWidgetItem_Form::ListWidgetItem_Form(const QString &name, const QPixmap &bgImage, QWidget *parent) :
    name_(name),image_(bgImage),QWidget(parent)
{
    setupUi(this);
    this->initGui();
}

void ListWidgetItem_Form::setName(const QString &name)
{
    name_ = name;
    labelName->setText(name_);
}

void ListWidgetItem_Form::setImage(const QPixmap &image)
{
    image_ = image;
    labelImage->setPixmap(image_);
}

void ListWidgetItem_Form::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void ListWidgetItem_Form::initGui()
{
    labelImage->setPixmap(image_);
    labelName->setText(name_);
}
