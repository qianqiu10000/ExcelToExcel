/*
 * 《外购件清单》解析类
*/
#ifndef MYPURCHASE_H
#define MYPURCHASE_H


#include"myDocument.h"

class myPurchase:public myDocument
{
public:
    myPurchase();
    ~myPurchase();


    bool saveAs(const QString&file);                                    //保存为xlsx文件
    QStringList header() const override;                              //获取表头
    void addLine(const QStringList&sourceLine);              //添加一行，输入是明细表数据行
    bool setFileName(const QString&file)override;            //设置采购表模板来源
    void setProjectInfo(const QStringList&info);               //设置项目信息，图号、项目号、令号


private:
    int m_rowHeight;                                                               //行高
    int m_currentLine;                                                             //要写入行索引
    QXlsx::Format m_format;                                                 //单元格格式

    void init() override;                                                          //初始化
    QStringList defaultLine();                                              //创建一行默认数据

};

#endif // MYPURCHASE_H
