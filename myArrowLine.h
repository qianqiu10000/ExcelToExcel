/*
 * 构造一条直线，并带有绘制路径
 * 起点绘制成小圆点
 * 终点绘制成箭头
*/


#ifndef MYARROWLINE_H
#define MYARROWLINE_H

#include<QLineF>
#include<QPainterPath>

class myArrowLine : public QLineF
{
public:
    myArrowLine(const QByteArray&byte);
    myArrowLine(const QPointF &pt1, const QPointF &pt2,const QPoint&column);


    QPainterPath painterPath();                                                        //绘制路径
    void setPointRadius(qreal r);                                                      //设置点半径
    void setArrowSize(qreal s);                                                         //设置箭头长度

    QByteArray data()const;                                                               //保存数据用
    bool isPurchaseMapping(QPoint&map)const;                          //属于外购件
    qreal distenceToPoint(const QPointF&p)const;                        //直线到点距离

private:
    qreal m_radius;                                                                               //半径
    qreal m_arrow;                                                                               //箭头长度
    QPoint m_map;                                                                              //列映射
    void init();                                                                                      //初始化

};

#endif // MYARROWLINE_H
