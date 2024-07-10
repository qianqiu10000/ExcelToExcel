#include "myPurchase.h"
#include"myArrowLine.h"

#include<xlsxcell.h>
#include<QDate>

#define FigureNumber  3,5  //图号
#define ProjectName 4,5    //项目名称
#define OrderNumber 5,5   //令号
#define HeaderRow 6         //表头行号


//#pragma execution_character_set("UTF-8")

QTXLSX_USE_NAMESPACE

extern QList<myArrowLine>g_lines;                                                                  //全局变量，所有线条

myPurchase::myPurchase()
{
    init();

}

myPurchase::~myPurchase()
{
    init();
}

void myPurchase::init()
{
    m_currentLine=HeaderRow+1;
    m_rowHeight=10;
}

//产生默认的一行数据
QStringList myPurchase::defaultLine()
{
    QStringList line;
    line<<QString::number(m_currentLine-HeaderRow);
    line<<QString();
    line<<"名称";
    line<<"型号";                                                                          //这里少了一列，为了从材料明细表替换数据
    line<<"件";
    line<<QString();
    line<<"304";
    line<<QString();
    line<<"B";

    QDate date=QDate::currentDate().addDays(7);                 //默认需求时间，当前日期+7天
    line<<date.toString("yyyy/MM/dd");
    line<<QString();


    return line;
}

//获取表头
QStringList myPurchase::header() const
{
    QStringList head=getLine(HeaderRow);
    if(head.count()>5)
        head.removeAt(4);                                       //这项是空白的，删除
    return head;
}


//另存为
bool myPurchase::saveAs(const QString &file)
{
    if(!isValid())
        return false;

    return   m_doucument->saveAs(file);
}


bool myPurchase::setFileName(const QString &file)
{
    if(!myDocument::setFileName(file))
        return false;

    //获取模板中的行高和单元格格式
    m_rowHeight=m_doucument->rowHeight(HeaderRow+1);
    m_format=m_doucument->cellAt(HeaderRow+1,1)->format();
    return true;
}

//填充图号、项目名称、令号
void myPurchase::setProjectInfo(const QStringList &info)
{
    if(!isValid())
        return;

    int count=info.count();
    switch (count) {
    case 3:
        m_doucument->write(OrderNumber,info.at(2));//令号
    case 2:
        m_doucument->write(ProjectName,info.at(1));//项目名称
    case 1:
        m_doucument->write(FigureNumber,info.at(0));//图号
    }
}


/**
 * @brief myPurchase::addLine
 * @param sourceLine
 * 添加一行数据
 * 输入数据是从材料明细表中获取的一行
 * 生产一行默认数据，根据列映射关系，替换部分数据
 */
void myPurchase::addLine(const QStringList &sourceLine)
{

    if(sourceLine.isEmpty())
        return;

    QStringList newLine=defaultLine();                                                                              //生成一行默认数据

    for(int i=0;i<g_lines.count();i++)                                                                                   //根据映射关系替换数据
    {
        myArrowLine line=g_lines.at(i);
        QPoint map;
        if(line.isPurchaseMapping(map))
        {
            int sourceIndex=map.x();
            int purchaseIndex=map.y();
            newLine.replace(purchaseIndex,sourceLine.at(sourceIndex));
        }
    }

    newLine.insert(4,QString());                                                                                           //插入一个空值，为了匹配模板

    for(int j=0;j<newLine.count();j++)                                                                                    //写入内容
        m_doucument->write(m_currentLine,j+1,newLine.at(j),m_format);

                                                                                                                                             //调整单元格式
    m_doucument->setRowHeight(m_currentLine,m_rowHeight);
    CellRange range;
    range.setFirstRow(m_currentLine);
    range.setLastRow(m_currentLine);
    range.setFirstColumn(4);
    range.setLastColumn(5);
    m_doucument->mergeCells(range,m_format);

                                                                                                                                               //指向下一行
    m_currentLine++;
}




