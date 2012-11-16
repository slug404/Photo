#ifndef LISTWIDGETITEM_FORM_H
#define LISTWIDGETITEM_FORM_H

#include "ui_ListWidgetItem_Form.h"

class ListWidgetItem_Form : public QWidget, private Ui::ListWidgetItem_Form
{
    Q_OBJECT
    
public:
    explicit ListWidgetItem_Form(const QString &name, const QPixmap &bgImage, QWidget *parent = 0);
    QString getName() const {return name_;}
    QPixmap getImage() const {return image_;}
    void setName(const QString &name);
    void setImage(const QPixmap &image);

protected:
    void changeEvent(QEvent *e);

    qreal getOpacity() const
    {
        return opacity_;
    }

    void setOpacity()
    {

    }
    qreal opacity_;

private:
    void initGui();

private:
    QString name_;
    QPixmap image_;
    QPixmap imageBg_;
};

#endif // LISTWIDGETITEM_FORM_H
