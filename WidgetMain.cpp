#include "WidgetMain.h"
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QProcess>
#include <QTransform>
#include <QFileInfo>
#include <QUndoCommand>
#include <QUndoStack>
#include <QUndoView>
#include <QBuffer>
#include "ListWidget.h"
#include "ListWidgetItem.h"
#include "ListWidgetItem_Form.h"
#include "GraphicFramework/GraphicsScene.h"
#include "WidgetShowScene.h"
#include "GraphicFramework/GraphicsItem.h"
#include "GraphicFramework/GraphicsView.h"
#include "undoFramework/MoveCommand.h"
#include "undoFramework/DeleteCommand.h"
#include "undoFramework/AddCommand.h"

WidgetMain::WidgetMain(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    initSetting();
    initData();
    initGui();
}

void WidgetMain::setLogo(const QPixmap &pix)
{
    labelLogo->setPixmap(pix.scaled(labelLogo->size()));
}

void WidgetMain::changeEvent(QEvent *e)
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

bool WidgetMain::eventFilter(QObject *target, QEvent *event)
{
    if(target == widgetTitle)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        switch(event->type())
        {
            case QEvent::MouseButtonPress:
            {
                if(Qt::LeftButton == mouseEvent->button())
                {
                    //获取屏幕左上角坐标与软件左上角坐标的偏移
                    offset_ = mouseEvent->globalPos() - frameGeometry().topLeft();
                    return true;
                }
                else
                {
                    return false;
                }
                break;
            }
            case QEvent::MouseMove:
            {
                if (Qt::LeftButton == mouseEvent->buttons() )
                {
                    move(mouseEvent->globalPos() - offset_);
                    return true;
                }
                else
                {
                    return false;
                }
                break;
            }
            default:
            {
                //...
                break;
            }
        }
    }
}

void WidgetMain::on_pushButtonOpenTemplate_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "File(*.zb)");

    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QByteArray bytes;

        bytes = file.readAll();

        QDataStream in(&bytes, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_8);

        int size;
        in >> size;
        qDebug() << size;

        int num = listWidgetTemplate->count();
        for(int i = 0; i != num; ++i)
        {
            qDebug() << i;
            ListWidgetItem *p = static_cast<ListWidgetItem *>(listWidgetTemplate->takeItem(i));
            if(p)
                delete p;
        }
        listWidgetTemplate->clear();

        //listWidgetTemplate->setLayoutMode();
        for(int i = 0; i != size; ++i)
        {
            //在这里循环插入item
            QPixmap pix;
            QByteArray imageData;
            QString fileName;
            in >> imageData >> fileName;
            pix.loadFromData(imageData, "png");

            vector_image_.append(pix.toImage());

            ListWidgetItem_Form *pWidget = new ListWidgetItem_Form(fileName, pix, this);
            ListWidgetItem *pItem = new ListWidgetItem(pWidget, this);
            pItem->setData(Qt::UserRole, i);
            pItem->setSizeHint(QSize(181, 131));
            listWidgetTemplate->addItem(pItem);
            listWidgetTemplate->setItemWidget(pItem, pWidget);
        }
    }
    else
    {
        //提示模板读取失败
        QMessageBox::warning(this, tr("警告"), tr("操作失败!"));
    }
    file.close();
}

void WidgetMain::initSetting()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    widgetTitle->installEventFilter(this);
}

void WidgetMain::initData()
{
    pGraphicsScene_ = new GraphicsScene(this);
    pGraphicsView_ = new GraphicsView;
    widgetShowScene->setGraphicsScene(pGraphicsScene_);
    widgetShowScene->setGraphicsView(pGraphicsView_);
    connect(listWidgetTemplate, SIGNAL(signalSetPixmap(QString,int,QPixmap)), this, SLOT(slotSetPixmap(QString,int,QPixmap)));
    connect(listWidgetTemplate, SIGNAL(signalSceneAddImage()), pGraphicsScene_, SLOT(slotAddImage()));
    connect(pGraphicsScene_, SIGNAL(signalAdjustSize(QSize)), this, SLOT(slotAdjustSize(QSize)));
    connect(pGraphicsScene_, SIGNAL(signalRemoveItem()), this, SLOT(slotRemoveItem()));
    connect(pGraphicsScene_, SIGNAL(signalChangeBgBySave(int,QPixmap)), listWidgetTemplate, SLOT(slotChangeBgBySave(int,QPixmap)));
    connect(pGraphicsScene_, SIGNAL(signalChangeBgByReset(int)), listWidgetTemplate, SLOT(slotChangeBgByReset(int)));
    //signal的参数个数和slot的不匹配是有意而为,因为不需要pos参数
    connect(pGraphicsScene_, SIGNAL(signalCreateItem(QString,QPointF)), this, SLOT(slotCreateItem(QString)));
    //listWidgetLayer->setIconSize(QSize(64, 64));

    //滑动条大小初始化
    //图层的
    //horizontalSlider->setMinimum(24);        //最小值24*24 像素
    //horizontalSlider->setMaximum(121);     //最大值121*121 像素
    //connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotItemSizeValueChanged(int)));
    //编辑区缩放的
    horizontalSliderMap->setMinimum(1);         //最小值1/100
    horizontalSliderMap->setMaximum(100);   //最大值原始大小 100/100
    horizontalSliderMap->setValue(100);
    connect(horizontalSliderMap, SIGNAL(valueChanged(int)), this, SLOT(slotViewSizeValueChange(int)));

    //连接重新设置Z轴循序的
    //connect(listWidgetLayer, SIGNAL(signalResetZvalue(QString,int)), this, SLOT(slotResetZvalue(QString,int)));

    //设置焦点
    //connect(listWidgetLayer, SIGNAL(signalSetFocus(QString)), this, SLOT(slotSetFcous(QString)));

    this->setListWidgetPointer(listWidgetTemplate);

    //init undo framework
    pUndoStack_ = new QUndoStack(this);
    this->createUndoView();

    pGraphicsScene_->setVectorImage(&vector_image_);
}

void WidgetMain::initGui()
{
}

QStringList WidgetMain::getComponentsName(const QString &filePath)
{
    QDir dataDir(filePath);
    dataDir.setFilter(QDir::Files);	//只需要文件,去掉.和..
    //qDebug() << "看看有没有.和.." <<dataDir.entryInfoList().;
    dataDir.setSorting(QDir::Name);	//排序,可有可无

    QStringList strFileNameList;
    QFileInfoList fileNameList = dataDir.entryInfoList();	//这里
    QFileInfoList::const_iterator iter;
    iter = fileNameList.begin();

    QString tmp;
    while(iter != fileNameList.end())
    {
        if((*iter).fileName() !="." && (*iter).fileName() !="..")
        {
            strFileNameList << (*iter).fileName();
        }
        ++iter;
    }
    qDebug() << strFileNameList;
    return strFileNameList;
}

void WidgetMain::setListWidgetPointer(ListWidget *p)
{
    pGraphicsScene_->setListWidget(p);
}

void WidgetMain::createUndoView()
{
    //以后右上角做成一个地图浏览器, 然后历史记录做成一个可停靠的悬浮窗口.
    pUndoView_ = new QUndoView(pUndoStack_);
    pUndoView_->setWindowTitle(tr("Command List"));
    //widgetHistory->
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(pUndoView_);
    layout->setSpacing(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    //这一句看来是有用的
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    //也不是这个
    widgetHistory->setLayout(layout);
}

void WidgetMain::on_pushButtonAddPhoto_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "File(*.jpg *.jpeg *png)");
    //在这里插入图片到QGraphicsItem
    QPixmap pix(filePath);
    QFileInfo fileInfor(filePath);
    qDebug() << fileInfor.size();

    GraphicsItem *p = new GraphicsItem(pix.rect(), pix);
    p->setData(Qt::UserRole, QObject::tr("photo"));
    pGraphicsScene_->list_pixmap_.append(p);
    connect(p, SIGNAL(signalNewPoint(GraphicsItem*,QPointF)), this, SLOT(slotMoveItem(GraphicsItem*,QPointF)));
    //p->setAcceptHoverEvents(true);

    AddCommand *pAddCommand = new AddCommand(p, pGraphicsScene_, pix);
    pUndoStack_->push(pAddCommand);

    //插入图层信息  这里也要放进undo framework里面去
    QString name = filePath.right(filePath.size() - filePath.lastIndexOf("/") - 1);
    hash_Name_GraphicsItem_[name] = p;
    this->slotCreateItem(filePath);
}

void WidgetMain::on_pushButtonForward_clicked()
{
    qDebug() << "前移";

    QList<QGraphicsItem *> list_graphics = pGraphicsScene_->selectedItems();
    foreach (QGraphicsItem *p, list_graphics )
    {
        p->setZValue(p->zValue() + 10);
    }
}

void WidgetMain::on_pushButtonUndo_clicked()
{
    pUndoStack_->undo();
}

void WidgetMain::on_pushButtonBackward_clicked()
{
    qDebug() << "后移";
    QList<QGraphicsItem *> list_graphics = pGraphicsScene_->selectedItems();
    foreach (QGraphicsItem *p, list_graphics )
    {
        p->setZValue(p->zValue() - 10);
    }
}

void WidgetMain::on_pushButtonRedu_clicked()
{
    pUndoStack_->redo();
}

void WidgetMain::on_pushButtonDelete_clicked()
{
    qDebug() << "删除所选图片";
    if(pGraphicsScene_->items().isEmpty())
    {
        return;
    }
    QList<QGraphicsItem *> list = pGraphicsScene_->selectedItems();
    if(list.isEmpty())
    {
        qDebug() << "没有选中的图片不能删除";
        return;
    }
    list.first()->setSelected(false);
    GraphicsItem *p= static_cast<GraphicsItem *>(list.first());
    DeleteCommand *pDeleteCommand = new DeleteCommand(p, pGraphicsScene_);
    pUndoStack_->push(pDeleteCommand);
    //pGraphicsScene_->deleteSelectItem();
}

void WidgetMain::on_pushButtonSendFile_clicked()
{
    qDebug() << "选择一张图片要发送图片了";
    QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("选择文件"), tr("."), tr("文件(*.zb)"));
    foreach (QString filePath, filePaths)
    {
        qDebug() << filePath;
        QProcess *pCmd = new QProcess(this);
        QString cmd = tr("./email/main.exe -p %1 -a 517549554@qq.com").arg(filePath);
        //cmdList <<tr("-p %1 -a 517549554@qq.com").arg(filePath );
        qDebug() << cmd;

        pCmd->start(cmd);
    }
}

void WidgetMain::on_pushButtonBuild_clicked()
{
    if(pGraphicsScene_->saveFile())
    {
        QMessageBox::information(this, trUtf8("操作成功"), trUtf8("保存成功!"));
    }
    else
    {
        QMessageBox::warning(this, trUtf8("操作失败"), trUtf8("保存失败!"));
    }
}

void WidgetMain::on_pushButtonSaveAll_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("保存"), ".");
    if(path.isEmpty())
    {
        return;
    }
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    qDebug() <<vector_image_.size();;
    out << vector_image_.size();
    for(int i = 0; i != vector_image_.size(); ++i)
    {
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        vector_image_.at(i).save(&buffer, "jpg");
        out << ba << QString::number(i);
    }

    QFile file(path + ".zb");
    if(file.open(QFile::WriteOnly))
    {
        file.write(bytes);
        QMessageBox::information(this, tr("成功!"), tr("操作成功!"));
    }
    else
    {
        QMessageBox::warning(this, tr("失败!"), tr("操作失败!"));
    }
}

void WidgetMain::on_pushButtonMinSize_clicked()
{
    if(windowState() != Qt::WindowMinimized)
    {
        this->setWindowState(Qt::WindowMinimized);
    }
}

void WidgetMain::on_pushButtonClose_clicked()
{
    pUndoStack_->clear();
    pUndoView_->close();
    this->close();
}

void WidgetMain::slotSetPixmap(const QString &name, int index, const QPixmap &image)
{
    qDebug() << "!!!!!!";
    //pGraphicsScene_->addPixmap(image);
    pGraphicsScene_->setImage(name, index, image);
}

void WidgetMain::slotCreateItem(const QString &path)
{
    QString fileName = path.right(path.size() - path.lastIndexOf("/") - 1);
    qDebug() << "path:" << path << "flieName:" << fileName;
    QListWidgetItem *pItem = new QListWidgetItem(fileName);

    pItem->setIcon(QPixmap(path));
    //listWidgetLayer->insertItem(0, pItem);
}

void WidgetMain::slotItemSizeValueChanged(int value)
{
    qDebug() << value;
    //listWidgetLayer->setIconSize(QSize(value, value));
}

void WidgetMain::slotResetZvalue(const QString &name, int value)
{
    if(hash_Name_GraphicsItem_.contains(name))
    {
        QGraphicsItem *p = hash_Name_GraphicsItem_.value(name);
        p->setZValue(value);
    }
}

void WidgetMain::slotSetFcous(const QString &name)
{
    qDebug() << name << "设置焦点";
    if(hash_Name_GraphicsItem_.contains(name))
    {
        hash_Name_GraphicsItem_[name]->setFocus(Qt::MouseFocusReason);
    }
}

void WidgetMain::slotViewSizeValueChange(int value)
{
    qDebug() << "修改视图的大小了" << value;
    //setTransform(QTransform().scale(totalScaleFactor_ * currentScaleFactor, totalScaleFactor_ * currentScaleFactor));
    //    qDebug() << "view " << "width:"<< pGraphicsView_->width() << "height:" << pGraphicsView_->height();
    qDebug() << "scene "<< "width:"<< pGraphicsScene_->width() << "height:" << pGraphicsScene_->height();
    qreal i = value / 100.0;

    pGraphicsView_->setTransform(QTransform().scale(i,  i));
}

void WidgetMain::slotAdjustSize(const QSize &size)
{
    qDebug() << size;
    double tmpW = size.width();
    double tmpH = size.height();
    qDebug() << widgetShowScene->width() / tmpW;
    qDebug() << widgetShowScene->height() / tmpH;
    qDebug() << widgetShowScene->width() << widgetShowScene->height();
    qDebug() <<  size.width() << size.height();
    //qDebug() << w << h;
    pGraphicsView_->setTransform(QTransform().scale(widgetShowScene->width() / tmpW,  widgetShowScene->height() / tmpH));
    //double tmp = qMin(widgetShowScene->width() / tmpW, widgetShowScene->height() / tmpH);
    //qDebug() << tmp <<"~~~~~~~";
    //horizontalSliderMap->setValue(tmp*100);
}

void WidgetMain::slotRemoveItem()
{
    //listWidgetLayer->clear();
    pUndoStack_->clear();       //果然是这个
}

void WidgetMain::slotMoveItem(GraphicsItem *p, const QPointF &oldPos)
{
    MoveCommand *pMoveCommand = new MoveCommand(p, oldPos);
    pUndoStack_->push(pMoveCommand);
}
