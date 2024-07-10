#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QTableWidget>

class myTableWidget:public QTableWidget
{
    Q_OBJECT
public:
    explicit myTableWidget(QWidget *parent = nullptr);
    ~myTableWidget();
    enum State{NoneState,DrawState,ErasState};                                //普通状态、绘制箭头状态、擦除箭头状态

    void clearLines();                                                                                //清理所有线
    void setState(State state);                                                                  //设置状态
    QStringList getProjectInfo();                                                             //获取项目信息
    int getPurchaseValue(QString &value);                                          //获取外购件所在的列和外购件特征值
    void setPurchaseListHeader(const QStringList&header);           //填充采购明细表头
    void setSourceListHeader(const QStringList&header);               //填充材料明细表头
    void setMachineListHeader(const QStringList&header);             //填充加工明细表头



protected:
    void paintEvent(QPaintEvent *event) override;                               //绘制箭头
    void mousePressEvent(QMouseEvent*event)override;                //确定箭头始终点
    void mouseMoveEvent(QMouseEvent*event)override;                 //箭头动画效果

private slots:
    void slotItemClicked(QTableWidgetItem *item);                              //点击单元格

private:
    State m_state;                                                                                      //当前状态值
    QPoint m_currentPt;                                                                            //当前鼠标位置

    QTableWidgetItem*m_startItem;                                                        //一条新线开始点



    void init();                                                                                           //初始化值
    void initUI();                                                                                       //初始化界面
    void clearDraw();                                                                              //取消当前线条绘制
    void saveSettings();                                                                         //保存重要数据到本地
    void loadSettings();                                                                          //从本地加载数据
    void createConnect();                                                                      //建立信号槽连接
    void viewportUpdate();                                                                    //视口刷新
    void beginDraw(QTableWidgetItem *item);                                     //开始一条新线
    void eraseLine(const QPoint&p);                                                     //擦除最近的线

};

#endif // MYTABLEWIDGET_H
