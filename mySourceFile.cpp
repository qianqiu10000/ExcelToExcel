#include "mySourceFile.h"
#include<QDebug>
#include<QFileInfo>
#include<xlsxcellrange.h>

//#pragma execution_character_set("UTF-8")

QTXLSX_USE_NAMESPACE
mySourceFile::mySourceFile()
{
    init();
}

mySourceFile::~mySourceFile()
{
    init();
}

QString mySourceFile::getSourcePath() const
{
    QFileInfo info(m_fileName);
    return info.absolutePath();
}

//初始化变量
void mySourceFile::init()
{
    m_orientation=0;
    m_fileName.clear();
    m_indexOfMachine.clear();
    m_indexOfPurchase.clear();
}

//第1行就是表头，相对行号
QStringList mySourceFile::header() const
{
    return getLine(1);
}

/**
 * @brief mySourceFile::getLine
 * @param row   相对行号
 * @return
 * 获取某一行数据
 * 此处行号是相对值，从有效数据范围算起
 * 材料名表有方向，无论↑ 还是↓排序，行号1都是表头
 */
QStringList mySourceFile::getLine(int row)const
{
    int index=m_orientation?m_lastRow-row+1:row+m_firstRow-1;
    return myDocument::getLine(index);
}

/**
 * @brief mySourceFile::setFileName
 * @param file
 * @return
 * 继承自基类，然后加入针对材料明细表的特殊处理
 */
bool mySourceFile::setFileName(const QString &file)
{
    //调用基类函数
    if(!myDocument::setFileName(file))
    {
        init();
        return false;
    }

    m_fileName=file;
    //判断表格方向
    QString first= m_doucument->read(m_firstRow,m_firstCol).toString();
    QString last= m_doucument->read(m_lastRow,m_firstCol).toString();
    if(first=="序号")
        m_orientation=0;
    else if(last=="序号")
        m_orientation=1;
    else
    {
        init();
        myDocument::init();
        qWarning()<<QString("文件%1表头第一项不是“序号”，无法判断表头！").arg(file);
        return false;
    }
    return true;
}

//获取加工件行号列队，相对行号
const QVector<int> &mySourceFile::getIndexOfMachine() const
{
    return m_indexOfMachine;
}

//获取外购件行号列队，相对行号
const QVector<int> &mySourceFile::getIndexOfPurchase() const
{
    return m_indexOfPurchase;
}

/**
 * @brief mySourceFile::setPurchaseFilter
 * @param colIndex    外购件和加工件在哪一列标记
 * @param value          外购件的标记词
 * 只有设置好这两个数据才能用材料明细表中区分出外购件，其余的全部是加工件
 */
void mySourceFile::setPurchaseFilter(int colIndex,const QString&value)
{
    //索引列表清空
    m_indexOfMachine.clear();
    m_indexOfPurchase.clear();

    if(colIndex<0)
    {
        qWarning()<<QString("指定的外购件标记列号%1，不能是负值！").arg(colIndex);
        return;
    }

    if(value.isEmpty())
    {
        qWarning()<<QString("指定的外购件标记词不能为空！");
        return;
    }

    //从2行开始，因为1行是表头
    //此处行号是相对值
    for(int i=2;i<=rowCount();i++)
    {
        QStringList line=getLine(i);
        if(line.at(colIndex)==value)                      //匹配到了外购件
            m_indexOfPurchase.append(i);
        else
            m_indexOfMachine.append(i);
    }
}
