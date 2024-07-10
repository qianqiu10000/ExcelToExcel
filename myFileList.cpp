#include "myFileList.h"
#include<QEventLoop>

//#pragma execution_character_set("UTF-8")

myFileList::myFileList(const QStringList&list)
{
    QListWidgetItem*item=new QListWidgetItem("发现多个文件，选择一个然后关闭改窗口");
    item->setFlags(Qt::NoItemFlags);

    addItem(item);
    addItems(list);
    setSelectionMode(QAbstractItemView::SingleSelection);
    m_loop=new QEventLoop;

    setWindowTitle("发现多个文件");
    resize(800,600);
    show();
}

int myFileList::exec()
{
    return  m_loop->exec();
}

/**
 * @brief myFileList::closeEvent
 * @param event
 * 窗口关闭后退出事件循环
 */
void myFileList::closeEvent(QCloseEvent *event)
{
    m_loop->quit();
    m_loop->deleteLater();

    QListWidget::closeEvent(event);
}

