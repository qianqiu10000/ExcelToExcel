#include "MainWindow.h"
#include"myTableWidget.h"
#include"mySourceFile.h"
#include"myPurchase.h"
#include"myFileList.h"
#include"myArrowLine.h"
#include"myMachine.h"



#include<QDir>
#include<QTimer>
#include<QAction>
#include<QDebug>
#include<QToolBar>
#include<QMimeData>
#include<QToolButton>
#include<QMessageBox>
#include<QDragEnterEvent>


//#pragma execution_character_set("utf-8")

extern QList<myArrowLine>g_lines;                                                                  //全局变量，所有线条

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    init();

    createUI();

    createConnection();
}

MainWindow::~MainWindow()
{
    delete m_source;
    delete m_purchase;
    delete m_machine;
}



void MainWindow::init()
{
    m_source=nullptr;
    m_purchase=nullptr;
    m_machine=nullptr;

}

//创建UI
void MainWindow::createUI()
{
    //工具栏
    m_toolBar=new QToolBar;
    addToolBar(Qt::TopToolBarArea,m_toolBar);

    m_toolBar->setMovable(false);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_createAction=m_toolBar->addAction("生成文件");
    m_toolBar->addSeparator();
    m_drawAction= m_toolBar->addAction("绘制连线");
    m_eraseAction=m_toolBar->addAction("擦除连线");
    m_clearAction=m_toolBar->addAction("清空连线");

    m_drawAction->setCheckable(true);
    m_eraseAction->setCheckable(true);

    m_createAction->setIcon(QIcon(":/icons/fileIcon.png"));
    m_drawAction->setIcon(QIcon(":/icons/editIcon.png"));
    m_eraseAction->setIcon(QIcon(":/icons/eraseIcon.png"));
    m_clearAction->setIcon(QIcon(":/icons/clearIcon.png"));

    //表格窗口
    m_table=new myTableWidget;
    setCentralWidget(m_table);


    setAcceptDrops(true);
    resize(1400,500);
}

//生成文件
void MainWindow::generateFile()
{
    //检查映射关系
    int pMap=0;
    int mMap=0;
    for(int i=0;i<g_lines.count();i++)
    {
        QPoint map;
        myArrowLine line=g_lines.at(i);
        if(line.isPurchaseMapping(map))
            pMap++;
        else
            mMap++;
    }

    if(pMap==0)
    {
        QMessageBox message;
        message.setText("无法确定材料名表->采购件清单映射关系！");
        message.exec();
        return;
    }
    if(mMap==0)
    {
        QMessageBox message;
        message.setText("无法确定材料名表->加工件清单映射关系！");
        message.exec();
        return;
    }



    QString value;
    int index=m_table->getPurchaseValue(value);
    if(index<0||value.isEmpty())
    {
        QMessageBox message;
        message.setText("未发现外购件特征值！");
        message.exec();
        return;
    }

    m_source->setPurchaseFilter(index,value);

    //获取索引列队
    QVector<int> purchaseIndex=m_source->getIndexOfPurchase();
    QVector<int> machineIndex=m_source->getIndexOfMachine();

    //填充外购件数据
    for(int i=0;i<purchaseIndex.count();i++)
        m_purchase->addLine(m_source->getLine(purchaseIndex.at(i)));

    //填充加工件数据
    for(int i=0;i<machineIndex.count();i++)
        m_machine->addLine(m_source->getLine(machineIndex.at(i)));

    //填充通用数据
    m_purchase->setProjectInfo(m_table->getProjectInfo());
    m_machine->setProjectInfo(m_table->getProjectInfo());

    bool pOk= m_purchase->saveAs(m_source->getSourcePath()+"/外购件清单.xlsx");
    bool mOk= m_machine->saveAs(m_source->getSourcePath()+"/加工件清单.xlsx");
    QString text=pOk?"外购件清单保存成功！":"外购件清单保存失败！";
    text.append("\n");
    text.append(mOk?"加工件清单保存成功！":"加工件清单保存失败！");
    QMessageBox message;
    message.setText(text);
    message.exec();
}

void MainWindow::createMachine()
{
    if(!m_machine)
        m_machine=new myMachine;

    m_machine->setFileName(":/template/machine.xlsx");
    m_table->setMachineListHeader(m_machine->header());
}

void MainWindow::createPurchase()
{
    if(!m_purchase)
        m_purchase=new myPurchase;

    m_purchase->setFileName(":/template/purchase.xlsx");
    m_table->setPurchaseListHeader(m_purchase->header());
}

void MainWindow::setSourceFile(const QString&file)
{
    QFileInfo info(file);

    if(info.suffix()!="xlsx")
    {
        QMessageBox message;
        message.setText("不是xlsx文件！");
        message.exec();

        close();
        return;
    }


    createSource(file);
    createPurchase();
    createMachine();
}

//创建信号槽
void MainWindow::createConnection()
{
    connect(m_toolBar,&QToolBar::actionTriggered,this,&MainWindow::actionTriggered);
}

void MainWindow::createSource(const QString&file)
{
    if(!m_source)
        m_source=new mySourceFile;

    m_source->setFileName(file);
    m_table->setSourceListHeader(m_source->header());

}

void MainWindow::actionTriggered(QAction *action)
{
    if(action==m_drawAction)
    {
        m_eraseAction->setChecked(false);
        m_table->setState(action->isChecked()?myTableWidget::DrawState:myTableWidget::NoneState);
    }
    else if(action==m_eraseAction)
    {
        m_drawAction->setChecked(false);
        m_table->setState(action->isChecked()?myTableWidget::ErasState:myTableWidget::NoneState);
    }
    else if(action==m_createAction)
    {
        m_drawAction->setChecked(false);
        m_eraseAction->setChecked(false);
        m_table->setState(myTableWidget::NoneState);
        generateFile();
    }
    else if(action==m_clearAction)
    {
        m_drawAction->setChecked(false);
        m_eraseAction->setChecked(false);

        m_table->clearLines();
    }
}


//拖拽事件
void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData*data= event->mimeData();

    if(!data->hasUrls())
    {
        event->ignore();
        return;
    }

    event->accept();
    QString path=data->urls().first().toLocalFile();
    setSourceFile(path);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData*data= event->mimeData();

    if(!data->hasUrls())
    {
        event->ignore();
        return;
    }


    event->accept();
}


