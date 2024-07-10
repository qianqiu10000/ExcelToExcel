/*
 * 处理xlsx文件的基类
*/


#ifndef MYDOCUMENT_H
#define MYDOCUMENT_H

#include<xlsxdocument.h>
class myDocument
{
public:
    myDocument();
    virtual ~myDocument();

    bool isValid();                                                                             //是否有效
    int rowCount();                                                                           //行数
    int columnCount();                                                                    //列数

    virtual QStringList header()const =0;                                        //获取表头
    virtual QStringList getLine(int row)const;                                 //获取某行数据
    virtual bool setFileName(const QString&file);                         //指定xlsx文件名字

 protected:

    int m_firstRow;                                                                             //有效数据第一行索引
    int m_lastRow;                                                                             //有效数据最后行索引
    int m_firstCol;                                                                              //有效数据第一列索引
    int m_lastCol;                                                                              //有效数据最后列索引

    QXlsx::Document*m_doucument;                                             //指向xlsx文件的指针


    virtual void init();                                                                         //初始化
};

#endif // MYDOCUMENT_H
