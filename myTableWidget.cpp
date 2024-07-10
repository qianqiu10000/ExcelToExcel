#include "myTableWidget.h"
#include<myArrowLine.h>

#include<QPainter>
#include<QPen>
#include<QMouseEvent>
#include<QDebug>
#include<QSettings>
#include<QHeaderView>


#define RowCount 11                                                                                //行数量
#define ColumnCount 11                                                                           //列数量
#define pTitleCell 0,0                                                                                 //采购清单标题单元
#define pDetailRow 1                                                                                //采购清单明细行
#define sTitleCell 3,0                                                                                 //材料明细表标题单元
#define sDetailRow 4                                                                                //材料明细表明细行
#define sValueRow 5                                                                                //外购件特征值行
#define mTitleCell 6,0                                                                               //加工件标题单元
#define mDetailRow 7                                                                              //加工件明细行
#define iTitleCell 9,0                                                                                 //项目信息标题单元
#define iFigureTitleCell  10,0                                                                   //图号标题单元
#define iFigureNumberCell  10,1                                                             //图号数据单元
#define iProjectTitleCell 10,3                                                                   //项目名称标题单元
#define iProjectNameCell 10,4                                                                //项目名称数据单元
#define iOrderTitleCell 10,8                                                                     //令号标题单元
#define iOrderNumber 10,9                                                                    //令号数据单元

//#pragma execution_character_set("utf-8")


QList<myArrowLine>g_lines;                                                                  //全局变量，所有线条

myTableWidget::myTableWidget(QWidget *parent)
    :QTableWidget(parent)
{

    init();
    initUI();
    createConnect();
}

myTableWidget::~myTableWidget()
{
    saveSettings();
}

//清理所有连线
void myTableWidget::clearLines()
{
    g_lines.clear();
    setState(NoneState);
    viewportUpdate();
}


//内部方便函数
void myTableWidget::init()
{
    //变量初始化
    m_state=NoneState;
    m_startItem=nullptr;
}

void myTableWidget::initUI()
{
    //表格初始化
    setRowCount(RowCount);
    setColumnCount(ColumnCount);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);

    for(int i=0;i<rowCount();i++)
        for(int j=0;j<columnCount();j++)
        {
            QTableWidgetItem*item=new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEnabled);
            setItem(i,j,item);
        }


    item(pTitleCell)->setText("采购清单");
    item(sTitleCell)->setText("材料明细表");
    item(mTitleCell)->setText("图样目录");
    item(iTitleCell)->setText("通用信息");
    item(iFigureTitleCell)->setText("图号");
    item(iProjectTitleCell)->setText("项目名称");
    item(iOrderTitleCell)->setText("令号");

    //这些单元格是绿色，标识可以连线，红色不能连线
    item(pDetailRow,0)->setBackground(Qt::red);
    item(pDetailRow,1)->setBackground(Qt::red);
    item(pDetailRow,2)->setBackground(Qt::green);
    item(pDetailRow,3)->setBackground(Qt::green);
    item(pDetailRow,4)->setBackground(Qt::red);
    item(pDetailRow,5)->setBackground(Qt::green);
    item(pDetailRow,6)->setBackground(Qt::green);
    item(pDetailRow,7)->setBackground(Qt::red);
    item(pDetailRow,8)->setBackground(Qt::red);
    item(pDetailRow,9)->setBackground(Qt::red);
    item(pDetailRow,10)->setBackground(Qt::green);

    item(mDetailRow,0)->setBackground(Qt::red);
    item(mDetailRow,1)->setBackground(Qt::green);
    item(mDetailRow,2)->setBackground(Qt::green);
    item(mDetailRow,3)->setBackground(Qt::red);
    item(mDetailRow,4)->setBackground(Qt::red);
    item(mDetailRow,5)->setBackground(Qt::green);

    item(sDetailRow,0)->setBackground(Qt::red);

    Qt::ItemFlags flags=
            Qt::ItemIsEditable|
            Qt::ItemIsSelectable|
            Qt::ItemIsEnabled;
    item(iFigureNumberCell)->setFlags(flags);
    item(iProjectNameCell)->setFlags(flags);
    item(iOrderNumber)->setFlags(flags);

    for(int j=0;j<columnCount();j++)
        item(5,j)->setFlags(flags);

    setSpan(pTitleCell,1,11);
    setSpan(2,0,1,11);
    setSpan(sTitleCell,1,11);
    setSpan(mTitleCell,1,11);
    setSpan(8,0,1,11);
    setSpan(iTitleCell,1,11);
    setSpan(iFigureNumberCell,1,2);
    setSpan(iProjectNameCell,1,4);
    setSpan(iOrderNumber,1,2);
}

void myTableWidget::clearDraw()
{
    m_startItem=nullptr;
    setMouseTracking(false);
    viewportUpdate();
}

void myTableWidget::saveSettings()
{
    //没有传入文件，不需要保存配置，否则都是空的
    QString valid=item(sDetailRow,0)->text();
    if(valid.isEmpty())
        return;

    QSettings setting("JN","JNExcel");
    setting.clear();

    setting.beginGroup("表格数据");
    int count=g_lines.count();
    setting.setValue("映射数量",count);
    for(int i=0;i<count;i++)
    {
        QByteArray data=g_lines.at(i).data();
        setting.setValue(QString("箭头线%1").arg(i),data);
    }
    QString value;
    int index=getPurchaseValue(value);
    setting.setValue("外购件列",index);
    setting.setValue("外购件值",value);

    setting.setValue("图号",item(iFigureNumberCell)->text());
    setting.setValue("项目名称",item(iProjectNameCell)->text());
    setting.setValue("令号",item(iOrderNumber)->text());

    setting.endGroup();


}

void myTableWidget::loadSettings()
{
    QSettings setting("JN","JNExcel");
    setting.beginGroup("表格数据");
    int count=setting.value("映射数量",0).toInt();
    for(int i=0;i<count;i++)
    {
        QByteArray data=setting.value(QString("箭头线%1").arg(i)).toByteArray();
        myArrowLine line(data);
        g_lines.append(line);
    }

    int index=setting.value("外购件列",6).toInt();
    QString value=setting.value("外购件值","外购件").toString();

    if(value.isEmpty())
        value="外购件";
    if(index>=0)
        item(sValueRow,index)->setText(value);

    item(iFigureNumberCell)->setText(setting.value("图号").toString());
    item(iProjectNameCell)->setText(setting.value("项目名称").toString());
    item(iOrderNumber)->setText(setting.value("令号").toString());

    setting.endGroup();

    viewportUpdate();
}

void myTableWidget::createConnect()
{
    connect(this,&QTableWidget::itemClicked,this,&myTableWidget::slotItemClicked);
}

void myTableWidget::viewportUpdate()
{
    this->viewport()->update();
}

void  myTableWidget::eraseLine(const QPoint &p)
{
    //右键擦除最近的线
    int index=-1;
    qreal l=0.1;
    qreal d=0;

    for(int i=0;i<g_lines.count();i++)
    {
        myArrowLine line=g_lines.at(i);
        d=line.distenceToPoint(p);

        if(d<l)
        {
            l=d;
            index=i;
        }
    }


    if(index>=0)
    {
        g_lines.removeAt(index);
        viewportUpdate();
    }

}

void myTableWidget::beginDraw(QTableWidgetItem *item)
{
    m_startItem=item;
    setMouseTracking(true);
}

//外部接口函数
void myTableWidget::setState(myTableWidget::State state)
{
    m_state=state;
    clearDraw();

    switch (m_state)
    {
    case DrawState:
        setCursor(QCursor(QPixmap(":/icons/editIcon.png").scaled(20,20)));
        break;
    case ErasState:
        setCursor(QCursor(QPixmap(":/icons/eraseIcon.png").scaled(20,20)));
        break;
    default:
        setCursor(QCursor(Qt::ArrowCursor));
        break;
    }
}

//填充外购件清单表头
void myTableWidget::setPurchaseListHeader(const QStringList &header)
{
    int col=qMin(header.count(),columnCount());
    for(int j=0;j<col;j++)
        item(pDetailRow,j)->setText(header.at(j));
}

//填充材料明细表表头
void myTableWidget::setSourceListHeader(const QStringList &header)
{
    int col=qMin(header.count(),columnCount());
    for(int j=0;j<col;j++)
        item(sDetailRow,j)->setText(header.at(j));

    loadSettings();
}

//填充加工件清单表头
void myTableWidget::setMachineListHeader(const QStringList &header)
{
    int col=qMin(header.count(),columnCount());
    for(int j=0;j<col;j++)
        item(mDetailRow,j)->setText(header.at(j));
}

//获取表格中的图号、项目号、令号
QStringList myTableWidget::getProjectInfo()
{
    QStringList list;
    list.append(item(iFigureNumberCell)->text());
    list.append(item(iProjectNameCell)->text());
    list.append(item(iOrderNumber)->text());

    return list;

}

//获取外购件特征列和特征值
int myTableWidget::getPurchaseValue(QString &value)
{
    int colum=-1;
    for(int j=0;j<columnCount();j++)
    {
        QString text=item(sValueRow,j)->text();
        if(!text.isEmpty())
        {
            colum=j;
            value=text;
            return colum;
        }
    }

    return colum;
}

//event事件
void myTableWidget::paintEvent(QPaintEvent *event)
{

    QTableWidget::paintEvent(event);

    QPainter painter(this->viewport());
    QPen pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);

    //如果是绘制状态，并且已经有了第一个点，绘制点到当前鼠标的箭头
    if(m_startItem&&m_state==DrawState)
        painter.drawPath(myArrowLine(
                                     visualItemRect(m_startItem).center(),
                                     m_currentPt,
                                     QPoint())
                         .painterPath());


    //绘制存储的箭头线
    for(int i=0;i<g_lines.count();i++)
    {
        painter.fillPath(g_lines[i].painterPath(),Qt::red);
        painter.drawLine(g_lines[i]);
    }

}

void myTableWidget::mousePressEvent(QMouseEvent *event)
{

    //如果是右键，结束当前绘制的线条
    if(event->button()==Qt::RightButton)
    {
        clearDraw();
        return;
    }

    //如果是擦除状态，擦除最近的线
    if(m_state==ErasState)
    {
        eraseLine(event->pos());
        return;
    }

    QTableWidget::mousePressEvent(event);

}

void myTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    QTableWidget::mouseMoveEvent(event);

    //如果按住鼠标键，不处理
    if(event->buttons())
        return;


    //如果是绘制状态，不断刷新绘制
    if(m_state==DrawState)
    {
        m_currentPt=event->pos();
        viewportUpdate();
    }
}


//槽函数

/**
 * @brief myTableWidget::slotItemClicked
 * @param item
 * 某个单元格被点击
 */
void myTableWidget::slotItemClicked(QTableWidgetItem *item)
{


    if(m_state!=DrawState)                                                                  //如果不是绘制状态，不处理
        return;

    //材料明细表一些单元格不能连线
    int row=item->row();
    int col=item->column();

    if(row==pDetailRow&&
      (col==0||
       col==1||
       col==4||
       col==7||
       col==8||
       col==9))
        return;

    //图样目录有些单元格不能连线
    if(row==mDetailRow&&
      (col==0||
       col==3||
       col==4||
       col>5))
        return;

    //材料明细表第一个单元格“序号”不能连线
    if(row==sDetailRow&&col==0)
        return;

    if(!m_startItem)                                                                               //如果还没有第一个点
    { 
        if(item->row()!=sDetailRow)                                                      //如果点击的不是材料明细表明细行，不处理
            return;
        beginDraw(item);                                                                       //直线起点必须是材料明细表明细行的单元格
        return;
    }


    if(item->row()==m_startItem->row())                                           //如果已经有了第一个点，但是当前点和第一点在同一行，不处理
        return;

    if(item->row()!=pDetailRow&&item->row()!=mDetailRow)          //如果第二点不在外购件或加工件明细行，不处理
        return;

    QRect rect1=visualItemRect(m_startItem);                                 //获取起点单元格矩形
    QRect rect2=visualItemRect(item);                                              //获取终点单元格矩形
    myArrowLine line(
                rect1.center(),
                rect2.center(),
                QPoint(m_startItem->column(),item->column()));           //以单元格中点构建箭头线
    g_lines.append(line);                                                                     //添加箭头线


    clearDraw();                                                                                    //清理箭头线数据，准备下一条
}


