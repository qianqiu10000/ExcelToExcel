/*
 * 用一个QListWidget显示找到的文件列表
 * 如果程序目下有多个有效xlsx文件，会弹出该窗口供用户选择一个
 * 该窗口采用模态显示
*/


#ifndef MYFILELIST_H
#define MYFILELIST_H

#include<QListWidget>

class QEventLoop;
class myFileList : public QListWidget
{
public:
    myFileList(const QStringList&list);                            //使用字符串列表构造
    int exec();                                                                     //启用消息循环，模态显示

protected:
    void closeEvent(QCloseEvent *event)override;      //关闭时进行一些操作


private:
    QEventLoop*m_loop;                                                   //事件循环指针
};

#endif // MYFILELIST_H
