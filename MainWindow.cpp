#include "MainWindow.h"
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    this->initSetting();
    this->initData();
    this->initGui();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void MainWindow::initSetting()
{

}

void MainWindow::initData()
{
    pGraphicsScene_ = new GraphicsScene(this);
    pGraphicsView_ = new GraphicsView;
    widgetShowScene->setGraphicsScene(pGraphicsScene_);
    widgetShowScene->setGraphicsView(pGraphicsView_);
    connect(listWidgetTemplate, SIGNAL(signalSetPixmap(QString,int,QPixmap)), this, SLOT(slotSetPixmap(QString,int,QPixmap)));
    connect(listWidgetTemplate, SIGNAL(signalSceneAddImage()), pGraphicsScene_, SLOT(slotAddImage()));
    connect(pGraphicsScene_, SIGNAL(signalAdjustSize(QSize)), this, SLOT(slotAdjustSize(QSize)));
    connect(pGraphicsScene_, SIGNAL(signalRemoveItem()), this, SLOT(slotRemoveItem()));

    //signal的参数个数和slot的不匹配是有意而为,因为不需要pos参数
    connect(pGraphicsScene_, SIGNAL(signalCreateItem(QString,QPointF)), this, SLOT(slotCreateItem(QString)));
    //listWidgetLayer->setIconSize(QSize(64, 64));

    //滑动条大小初始化
    //图层的
    horizontalSlider->setMinimum(24);        //最小值24*24 像素
    horizontalSlider->setMaximum(121);     //最大值121*121 像素
    connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotItemSizeValueChanged(int)));
    //编辑区缩放的
    horizontalSliderMap->setMinimum(1);         //最小值1/100
    horizontalSliderMap->setMaximum(100);   //最大值原始大小 100/100
    horizontalSliderMap->setValue(100);
    connect(horizontalSliderMap, SIGNAL(valueChanged(int)), this, SLOT(slotViewSizeValueChange(int)));

    //连接重新设置Z轴循序的
    connect(listWidgetLayer, SIGNAL(signalResetZvalue(QString,int)), this, SLOT(slotResetZvalue(QString,int)));

    //设置焦点
    connect(listWidgetLayer, SIGNAL(signalSetFocus(QString)), this, SLOT(slotSetFcous(QString)));

//    QString file = "./Template/";
//    templateFilesName_ = getComponentsName(file);
//    qSort(templateFilesName_.begin(), templateFilesName_.end());

//    int index = 0;
//    foreach (QString name, templateFilesName_)
//    {
//        QPixmap pix(181, 161);
//        pix.load(file+name);

//        ListWidgetItem_Form *pWidget = new ListWidgetItem_Form(name, pix, this);
//        ListWidgetItem *pItem = new ListWidgetItem(pWidget, this);
//        pItem->setData(Qt::UserRole, index++);
//        pItem->setSizeHint(QSize(181, 161));
//        listWidgetTemplate->addItem(pItem);
//        listWidgetTemplate->setItemWidget(pItem, pWidget);
//    }

    this->setListWidgetPointer(listWidgetTemplate);

    //init undo framework
    pUndoStack_ = new QUndoStack(this);
    this->createUndoView();

    pGraphicsScene_->setVectorImage(&vector_image_);
}

void MainWindow::initGui()
{
}

QStringList MainWindow::getComponentsName(const QString &filePath)
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

void MainWindow::setListWidgetPointer(ListWidget *p)
{
    pGraphicsScene_->setListWidget(p);
}

void MainWindow::createUndoView()
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
    layout->setMargin(0);
    //也不是这个
    widgetHistory->setLayout(layout);
}

void MainWindow::slotCreateItem(const QString &path)
{
    QString fileName = path.right(path.size() - path.lastIndexOf("/") - 1);
    qDebug() << "path:" << path << "flieName:" << fileName;
    QListWidgetItem *pItem = new QListWidgetItem(fileName);

    pItem->setIcon(QPixmap(path));
    listWidgetLayer->insertItem(0, pItem);
}

void MainWindow::slotItemSizeValueChanged(int value)
{
    qDebug() << value;
    listWidgetLayer->setIconSize(QSize(value, value));
}

void MainWindow::slotResetZvalue(const QString &name, int value)
{
    if(hash_Name_GraphicsItem_.contains(name))
    {
        QGraphicsItem *p = hash_Name_GraphicsItem_.value(name);
        p->setZValue(value);
    }
}

void MainWindow::slotSetFcous(const QString &name)
{
    qDebug() << name << "设置焦点";
    if(hash_Name_GraphicsItem_.contains(name))
    {
        hash_Name_GraphicsItem_[name]->setFocus(Qt::MouseFocusReason);
    }
}

void MainWindow::slotViewSizeValueChange(int value)
{
    qDebug() << "修改视图的大小了" << value;
    //setTransform(QTransform().scale(totalScaleFactor_ * currentScaleFactor, totalScaleFactor_ * currentScaleFactor));
//    qDebug() << "view " << "width:"<< pGraphicsView_->width() << "height:" << pGraphicsView_->height();
    qDebug() << "scene "<< "width:"<< pGraphicsScene_->width() << "height:" << pGraphicsScene_->height();
    qreal i = value / 100.0;

    pGraphicsView_->setTransform(QTransform().scale(i,  i));
}

void MainWindow::on_action_O_triggered()
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

        for(int i = 0; i != size; ++i)
        {
            //在这里循环插入item
            QPixmap pix;
            QByteArray imageData;
            QString fileName;
            in >> imageData >> fileName;
            pix.loadFromData(imageData, "png");

            vector_image_[i] = pix.toImage();

            ListWidgetItem_Form *pWidget = new ListWidgetItem_Form(fileName, pix, this);
            ListWidgetItem *pItem = new ListWidgetItem(pWidget, this);
            pItem->setSizeHint(QSize(176, 151));
            listWidgetTemplate->addItem(pItem);
            listWidgetTemplate->setItemWidget(pItem, pWidget);
        }
    }
    else
    {
        //提示模板读取失败
        QMessageBox::warning(this, tr("警告"), tr("操作失败!"));
    }
}

void MainWindow::on_action_S_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("保存文件"), ".");

    if(pGraphicsScene_->saveFile(filePath))
    {
        QMessageBox::information(this, trUtf8("操作成功"), trUtf8("保存成功!"));
    }
    else
    {
        QMessageBox::warning(this, trUtf8("操作失败"), trUtf8("保存失败!"));
    }
}

void MainWindow::on_action_I_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "File(*.jpg *.jpeg)");
    //在这里插入图片到QGraphicsItem
    QPixmap pix(filePath);
    QFileInfo fileInfor(filePath);
    qDebug() << fileInfor.size();
    if(pix.width() <= 200 || pix.height() <= 600)
    {
        QMessageBox::warning(this, tr("标题"), tr("插入的照片不能小于1200*1600像素"));
        return;
    }

    if(fileInfor.size() <= 1024 * 1)
    {
        QMessageBox::warning(this, tr("标题"), tr("插入的照片不能小于1MB"));
        return;
    }
    GraphicsItem *p = new GraphicsItem(pix.rect(), pix);
    //pGraphicsScene_->setSceneRect(pix.rect());
    p->setData(Qt::UserRole, QObject::tr("photo"));
    connect(p, SIGNAL(signalNewPoint(GraphicsItem*,QPointF)), SLOT(slotMoveItem(GraphicsItem*,QPointF)));
    p->setAcceptHoverEvents(true);

    AddCommand *pAddCommand = new AddCommand(p, pGraphicsScene_, pix);
    pUndoStack_->push(pAddCommand);

    //插入图层信息  这里也要放进undo framework里面去
    QString name = filePath.right(filePath.size() - filePath.lastIndexOf("/") - 1);
    hash_Name_GraphicsItem_[name] = p;
    this->slotCreateItem(filePath);
}

void MainWindow::slotSetPixmap(const QString &name, int index, const QPixmap &image)
{
    qDebug() << "!!!!!!";
    //pGraphicsScene_->addPixmap(image);
    pGraphicsScene_->setImage(name, index, image);
}

void MainWindow::on_action_SendPhoto_triggered()
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

void MainWindow::on_action_forward_triggered()
{
    qDebug() << "前移";

    QList<QGraphicsItem *> list_graphics = pGraphicsScene_->selectedItems();
    foreach (QGraphicsItem *p, list_graphics )
    {
        p->setZValue(p->zValue() + 10);
    }
}

void MainWindow::on_action_backward_triggered()
{
    qDebug() << "后移";
    QList<QGraphicsItem *> list_graphics = pGraphicsScene_->selectedItems();
    foreach (QGraphicsItem *p, list_graphics )
    {
        p->setZValue(p->zValue() - 10);
    }
}

void MainWindow::on_pushButtonAddPhoto_clicked()
{
    this->on_action_I_triggered();
}

void MainWindow::on_pushButtonOpenTemplate_clicked()
{
    this->on_action_O_triggered();
}

void MainWindow::on_pushButtonForward_clicked()
{
    this->on_action_forward_triggered();
}

void MainWindow::on_pushButtonBackward_clicked()
{
    this->on_action_backward_triggered();
}

void MainWindow::on_pushButtonBuild_clicked()
{
    this->on_action_S_triggered();
}

void MainWindow::on_pushButtonUndo_clicked()
{
    this->on_action_Undo_triggered();
}

void MainWindow::slotAdjustSize(const QSize &size)
{
    qDebug() << size;
    double tmpW = size.width();
    double tmpH = size.height();
    qDebug() << widgetShowScene->width() / tmpW;
    qDebug() << widgetShowScene->height() / tmpH;
    qDebug() << widgetShowScene->width() << widgetShowScene->height();
    qDebug() <<  size.width() << size.height();
    //qDebug() << w << h;
    //pGraphicsView_->setTransform(QTransform().scale(widgetShowScene->width() / tmpW - 0.02,  widgetShowScene->height() / tmpH - 0.02));
    double tmp = qMin(widgetShowScene->width() / tmpW, widgetShowScene->height() / tmpH);
    qDebug() << tmp <<"~~~~~~~";
    horizontalSliderMap->setValue(tmp*100);
}

void MainWindow::slotRemoveItem()
{
    listWidgetLayer->clear();
    pUndoStack_->clear();       //果然是这个
}

void MainWindow::slotMoveItem(GraphicsItem *p, const QPointF &oldPos)
{
    MoveCommand *pMoveCommand = new MoveCommand(p, oldPos);
    pUndoStack_->push(pMoveCommand);
}

void MainWindow::on_pushButtonRedu_clicked()
{
    this->on_action__triggered();
}

void MainWindow::on_pushButtonDelete_clicked()
{
    qDebug() << "删除所选图片";
    if(pGraphicsScene_->items().isEmpty())
    {
        return;
    }

    DeleteCommand *pDeleteCommand = new DeleteCommand(pGraphicsScene_);
    pUndoStack_->push(pDeleteCommand);
    //pGraphicsScene_->deleteSelectItem();
}

void MainWindow::on_action_Undo_triggered()
{
    pUndoStack_->undo();
}

void MainWindow::on_action__triggered()
{
    pUndoStack_->redo();
}

void MainWindow::on_pushButtonSaveAll_clicked()
{

}
