#include "myMachine.h"
#include"myArrowLine.h"
#include<xlsxcell.h>

#define FigureNumber  3,13  //图号
#define ProjectName 4,13    //项目名称
#define OrderNumber 5,13   //令号
#define HeaderRow 6         //表头行号


//#pragma execution_character_set("UTF-8")

QTXLSX_USE_NAMESPACE

extern QList<myArrowLine>g_lines;                                                                  //全局变量，所有线条

myMachine::myMachine()
{
    init();
}

myMachine::~myMachine()
{
    init();
}

void myMachine::init()
{
    m_currentLine=HeaderRow+1;
    m_rowHeight=10;
}

//产生默认的一行数据
QStringList myMachine::defaultLine()
{
    QStringList line;
    line<<QString::number(m_currentLine-HeaderRow);       //序号
    line<<QString();                                                                   //代号
    line<<QString();                                                                   //名称
    line<<"A3";                                                                           //幅面
    line<<"1";                                                                              //张数
    line<<QString();                                                                   //备注（数量）

    return line;
}

//获取表头
QStringList myMachine::header() const
{
    QStringList head=getLine(HeaderRow);
    QStringList line;
    if(head.count()>14)
    {
        line.append(head.at(0));
        line.append(head.at(1));
        line.append(head.at(4));
        line.append(head.at(10));
        line.append(head.at(12));
        line.append(head.at(14));
    }
    return line;
}


//另存为
bool myMachine::saveAs(const QString &file)
{
    if(!isValid())
        return false;

    return   m_doucument->saveAs(file);
}


bool myMachine::setFileName(const QString &file)
{
    if(!myDocument::setFileName(file))
        return false;

    //获取模板中的行高和单元格格式
    m_rowHeight=m_doucument->rowHeight(HeaderRow+1);
    m_format=m_doucument->cellAt(HeaderRow+1,1)->format();
    return true;
}

//填充图号、项目名称、令号
void myMachine::setProjectInfo(const QStringList &info)
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
 * @brief myMachine::addLine
 * @param sourceLine
 * 添加一行数据
 * 输入数据是从材料明细表中获取的一行
 * 生产一行默认数据，根据列映射关系，替换部分数据
 */
void myMachine::addLine(const QStringList &sourceLine)
{

    if(sourceLine.isEmpty())
        return;

    QStringList newLine=defaultLine();                                                                                 //生成一行默认数据


    for(int i=0;i<g_lines.count();i++)                                                                                      //根据映射关系替换数据
    {
        myArrowLine line=g_lines.at(i);
        QPoint map;
        if(!line.isPurchaseMapping(map))
        {
            int sourceIndex=map.x();
            int purchaseIndex=map.y();
            newLine.replace(purchaseIndex,sourceLine.at(sourceIndex));
        }
    }
                                                                                                                                             //插入空值，为了匹配模板
    newLine.insert(5,QString());
    newLine.insert(4,QString());
    for(int i=0;i<5;i++)
        newLine.insert(3,QString());
    for(int i=0;i<2;i++)
        newLine.insert(2,QString());

    for(int j=0;j<newLine.count();j++)                                                                                    //写入内容
        m_doucument->write(m_currentLine,j+1,newLine.at(j),m_format);

                                                                                                                                             //调整单元格式
    m_doucument->setRowHeight(m_currentLine,m_rowHeight);
    CellRange range;
    range.setFirstRow(m_currentLine);
    range.setLastRow(m_currentLine);
    range.setFirstColumn(2);
    range.setLastColumn(4);
    m_doucument->mergeCells(range,m_format);
    range.setFirstColumn(5);
    range.setLastColumn(10);
    m_doucument->mergeCells(range,m_format);
    range.setFirstColumn(11);
    range.setLastColumn(12);
    m_doucument->mergeCells(range,m_format);
    range.setFirstColumn(13);
    range.setLastColumn(14);
    m_doucument->mergeCells(range,m_format);
    range.setFirstColumn(15);
    range.setLastColumn(18);
    m_doucument->mergeCells(range,m_format);

                                                                                                                                               //指向下一行
    m_currentLine++;
}




