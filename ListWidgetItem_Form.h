#ifndef LISTWIDGETITEM_FORM_H
#define LISTWIDGETITEM_FORM_H

#include "ui_ListWidgetItem_Form.h"
#include <QPropertyAnimation>
class ListWidgetItem_Form : public QWidget, private Ui::ListWidgetItem_Form
{
    Q_OBJECT
    
public:
    explicit ListWidgetItem_Form(const QString &name, const QPixmap &bgImage, QWidget *parent = 0);
    void setPixmap(const QPixmap &pix) {labelImage->setPixmap(pix);}        //仅仅用在拖拽的时候

    QString getName() const {return name_;}
    QPixmap getImage() const {return image_;}

    void setName(const QString &name);
    void setImage(const QPixmap &image);

    QPixmap getBgImage() const {return imageBg_;}

protected:
    void changeEvent(QEvent *e);

private:
    void initGui();

private:
    QString name_;
    QPixmap image_;
    QPixmap imageBg_;
};

#endif // LISTWIDGETITEM_FORM_H
