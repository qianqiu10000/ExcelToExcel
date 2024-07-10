#include "myDocument.h"
#include<QFileInfo>
#include<QDebug>

QTXLSX_USE_NAMESPACE

myDocument::myDocument()
{
    m_doucument=nullptr;
    init();
}

myDocument::~myDocument()
{
    init();
}

/**
 * @brief myDocument::init
 * 初始化
 * 将xlsx文件指针释放，清空
 */
void myDocument::init()
{
    delete m_doucument;
    m_doucument=nullptr;


    m_lastCol=0;
    m_firstCol=0;
    m_lastRow=0;
    m_firstRow=0;

}


/**
 * @brief myDocument::isValid
 * @return     文件是否有效
 */
bool myDocument::isValid()
{
    return m_doucument;
}

//获取行数
int myDocument::rowCount()
{
    return m_lastRow-m_firstRow+1;
}

//获取列数
int myDocument::columnCount()
{
    return m_lastCol-m_firstCol+1;
}

//获取某一行数据，row是绝对行数
QStringList myDocument::getLine(int row)const
{
    QStringList line;

    //如果行数范围无效
    if(row>m_lastRow||row<m_firstRow)
    {
        qWarning()<<QString("有效数据起始行号范围%1-%2，输入的行号%3不在有效范围！")
                    .arg(m_firstRow)
                    .arg(m_lastRow)
                    .arg(row);
        return line;
    }



    for(int j=m_firstCol;j<=m_lastCol;j++)
        line.append(m_doucument->read(row,j).toString());

    return line;
}


/**
 * @brief myDocument::setFileName
 * @param file xlsx文件路径
 * @return  执行结果
 */
bool myDocument::setFileName(const QString &file)
{

    //如果文件不存在，返回结果失败
    if(!QFileInfo::exists(file))
    {
        qWarning()<<QString("xlsx文件%1不存在！").arg(file);
        return false;
    }

    //如果已经设置过了就销毁重建
    if(m_doucument)
        init();

    //构建document
    m_doucument=new Document(file);

    //获取sheet列表
    QStringList sheetNames=m_doucument->sheetNames();
    if(sheetNames.isEmpty())
    {
        init();
        qWarning()<<QString("无法从文件%1中获得表格！").arg(file);
        return false;
    }

    //选中第一个活动sheet
    m_doucument->selectSheet(sheetNames.first());

    //获取有效内容行列数
    CellRange dimension= m_doucument->dimension();
    m_firstRow=dimension.firstRow();
    m_lastRow=dimension.lastRow();
    m_firstCol=dimension.firstColumn();
    m_lastCol=dimension.lastColumn();
    int rows=dimension.rowCount();
    int cols=dimension.columnCount();



    //读到的行列数太少认为文件错误
    if(rows<2||cols<2)
    {
        init();
        qWarning()<<QString("文件%1中包含%2行%3列！数据太少！")
                    .arg(file)
                    .arg(rows)
                    .arg(cols);
        return false;
    }

    return true;
}

