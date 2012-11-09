#include "MainWindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QProcess>
#include "ListWidget.h"
#include "ListWidgetItem.h"
#include "ListWidgetItem_Form.h"
#include "GraphicsScene.h"
#include "WidgetShowScene.h"
#include "GraphicsItem.h"

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
    widgetShowScene->setGraphicsScene(pGraphicsScene_);
    //signal的参数个数和slot的不匹配是有意而为,因为不需要pos参数
    connect(pGraphicsScene_, SIGNAL(signalCreateItem(QString,QPointF)), this, SLOT(slotCreateItem(QString)));
//    listWidgetLayer->setIconSize(QSize(64, 64));
    //滑动条大小初始化
    //horizontalSlider->setMinimum(24);
    //horizontalSlider->setMaximum(121);
    //connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotItemSizeValueChanged(int)));
    //连接重新设置Z轴循序的
    //connect(listWidgetLayer, SIGNAL(signalResetZvalue(QString,int)), this, SLOT(slotResetZvalue(QString,int)));

    //设置焦点
    //connect(listWidgetLayer, SIGNAL(signalSetFocus(QString)), this, SLOT(slotSetFcous(QString)));

    QString file = "./Template/";
    templateFilesName_ = getComponentsName(file);
    qSort(templateFilesName_.begin(), templateFilesName_.end());
    foreach (QString name, templateFilesName_)
    {
        QPixmap pix(251, 171);
        pix.load(file+name);

        ListWidgetItem_Form *pWidget = new ListWidgetItem_Form(name, pix, this);
        ListWidgetItem *pItem = new ListWidgetItem(pWidget, this);
        pItem->setSizeHint(QSize(251, 201));
        listWidgetTemplate->addItem(pItem);
        listWidgetTemplate->setItemWidget(pItem, pWidget);
    }

    this->setListWidgetPointer(listWidgetTemplate);
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

void MainWindow::saveFile(const QString path)
{
    pGraphicsScene_->saveFile(path);
}

void MainWindow::slotCreateItem(const QString &path)
{
    QString fileName = path.right(path.size() - path.lastIndexOf("/") - 1);
    qDebug() << "path:" << path << "flieName:" << fileName;
    QListWidgetItem *pItem = new QListWidgetItem(fileName);

    pItem->setIcon(QPixmap(path));
    //listWidgetLayer->insertItem(0, pItem);
}

void MainWindow::slotItemSizeValueChanged(int value)
{
    qDebug() << value;
    //listWidgetLayer->setIconSize(QSize(value, value));
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

void MainWindow::on_action_O_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "File(*.zb)");

    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QByteArray bytes;

        bytes = file.readAll();

        QDataStream in(&bytes, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_7);

        int size;
        in >> size;
        qDebug() << size;

        for(int i = 0; i != size; ++i)
        {
            //在这里循环插入item
            QPixmap pix(251, 171);
            QByteArray imageData;
            in >> imageData;
            pix.loadFromData(imageData, "png");

//            QLabel *p = new QLabel;
//            p->setPixmap(pix);
//            p->show();
            ListWidgetItem_Form *pWidget = new ListWidgetItem_Form(QString::number(i+1), pix.scaled(251, 171), this);
            ListWidgetItem *pItem = new ListWidgetItem(pWidget, this);
            pItem->setSizeHint(QSize(251, 201));
            listWidgetTemplate->addItem(pItem);
            listWidgetTemplate->setItemWidget(pItem, pWidget);
        }
    }
    else
    {
        //提示模板读取失败
    }




}

void MainWindow::on_action_S_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("保存文件"), ".");
    this->saveFile(filePath);
}

void MainWindow::on_action_I_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "File(*.png)");
    //在这里插入图片到QGraphicsItem
    QPixmap pix(filePath);
    GraphicsItem *p = new GraphicsItem(pix.rect(), pix);
    pGraphicsScene_->addItem(p);
    pGraphicsScene_->update();
    //插入图层信息
    p->setFlag(QGraphicsItem::ItemIsMovable);
    p->setFlag(QGraphicsItem::ItemIsSelectable);
    p->setFlag(QGraphicsItem::ItemIsFocusable);

    QString name = filePath.right(filePath.size() - filePath.lastIndexOf("/") - 1);
    hash_Name_GraphicsItem_[name] = p;
    this->slotCreateItem(filePath);
}

void MainWindow::on_action_Clear_triggered()
{
    int ok = QMessageBox::warning(this, tr("警告"),tr("真的要清理画布? 按OK确认清理,按Cancel取消操作"), QMessageBox::Ok, QMessageBox::Cancel);
    if(ok == QMessageBox::Ok)
    {
        pGraphicsScene_->clear();
    }
}

void MainWindow::on_action_undo_triggered()
{
    QHash<QString, QGraphicsItem *>::iterator iter;
    for(iter = hash_Name_GraphicsItem_.begin(); iter != hash_Name_GraphicsItem_.end(); ++iter)
    {
        GraphicsItem *p = (GraphicsItem *)iter.value();
        //p->setOldTransformation();
        p->setRotation(0);
        p->setScale(0.9);
    }
}

void MainWindow::on_action_SendPhoto_triggered()
{
    qDebug() << "选择一张图片要发送图片了";
    QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("选择文件"), tr("."), tr("文件(*.jpg *.jpeg)"));
    foreach (QString filePath, filePaths)
    {
        qDebug() << filePath;
        QProcess *pCmd = new QProcess(this);
        QString cmd = tr("main.exe -p %1 -a 517549554@qq.com").arg(filePath);
        //cmdList <<tr("-p %1 -a 517549554@qq.com").arg(filePath );
        qDebug() << cmd;

        pCmd->start(cmd);
    }
}
