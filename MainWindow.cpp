#include "MainWindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include "ListWidget.h"
#include "ListWidgetItem.h"
#include "ListWidgetItem_Form.h"
#include "GraphicsScene.h"
#include "WidgetShowScene.h"

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
    horizontalSlider->setMinimum(24);
    horizontalSlider->setMaximum(121);
    connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotItemSizeValueChanged(int)));

    QString file = "./Template/";
    templateFilesName_ = getComponentsName(file);
    qSort(templateFilesName_.begin(), templateFilesName_.end());
    foreach (QString name, templateFilesName_)
    {
        QPixmap pix(251, 171);
        pix.load(file+name);

        ListWidgetItem_Form *pWidget = new ListWidgetItem_Form(name, pix.scaled(251, 171), this);
        ListWidgetItem *pItem = new ListWidgetItem(pWidget, this);
        pItem->setSizeHint(QSize(251, 201));
        listWidgetTemplate->addItem(pItem);
        listWidgetTemplate->setItemWidget(pItem, pWidget);
    }

    this->setListWidgetPointer(listWidgetTemplate);
}

void MainWindow::initGui()
{
    connect(listWidgetTemplate, SIGNAL(signalPressListWidgetItem(QString)), this, SLOT(slotPressListWidgetItem(QString)));
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

void MainWindow::addPhotoItem(const QPixmap &pix)
{
    pGraphicsScene_->addPhotoItem(pix);
}

void MainWindow::saveFile(const QString path)
{
    pGraphicsScene_->saveFile(path);
}

void MainWindow::slotPressListWidgetItem(const QString &str)
{
    pGraphicsScene_->setItemName(str);
}

void MainWindow::slotCreateItem(const QString &path)
{
    QString fileName = path.right(path.size() - path.lastIndexOf("/") - 1);
    qDebug() << "path:" << path << "flieName:" << fileName;
    QListWidgetItem *pItem = new QListWidgetItem(fileName);

    pItem->setIcon(QPixmap(path));
    listWidgetLayer->addItem(pItem);
}

void MainWindow::slotItemSizeValueChanged(int value)
{
    qDebug() << value;
    listWidgetLayer->setIconSize(QSize(value, value));
}

void MainWindow::on_action_O_triggered()
{
    QStringList names = QFileDialog::getOpenFileNames(this, tr("打开文件"), ".", "File(*.png *.p)");
    foreach (QString name, names)
    {
        qDebug() << "在这里加载模板图片做处理";
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
    this->addPhotoItem(pix);
    //插入图层信息
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
