/*
 * 解析源文件，用于处理《材料明细表》
*/
#ifndef MYSOURCEFILE_H
#define MYSOURCEFILE_H

#include"myDocument.h"

class mySourceFile:public myDocument
{
public:
    mySourceFile();
    ~mySourceFile();


    QString getSourcePath()const;                                               //获取源文件路径
    QStringList header()const override;                                         //获取表头数据
    QStringList getLine(int row)const override;                             //获取某行数据，相对行号
    bool setFileName(const QString&file)override;                      //指定xlsx文件名字
    void setPurchaseFilter(int colIndex,const QString&value);  //指定外购件特征列、特征值
    const QVector<int>&getIndexOfPurchase()const;               //获取外购件行号列队
    const QVector<int>&getIndexOfMachine()const;                 //获取加工件行号列队

private:

    int m_orientation;                                                                       //表格方向0↓ 1↑
    QString m_fileName;                                                                 //保存设置的文件名
    QVector<int>m_indexOfPurchase;                                          //外购件索引
    QVector<int>m_indexOfMachine;                                           //加工件索引

    void init()override;                                                                    //初始化
};

#endif // MYSOURCEFILE_H
