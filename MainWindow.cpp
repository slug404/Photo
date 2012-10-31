#include "MainWindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>

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
    QString file = "./Template/";
    templateFilesName_ = getComponentsName(file);

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
    return strFileNameList;
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
    //在这里生成文件
}

void MainWindow::on_action_I_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "File(*.png)");
    //在这里插入图片到QGraphicsItem
}
