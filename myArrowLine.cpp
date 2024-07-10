#include "myArrowLine.h"
#include<QTransform>
#include<QDataStream>
#include<QDebug>
#include<QtMath>


//从数据中恢复线
myArrowLine::myArrowLine(const QByteArray &byte)
    :QLineF()
{
    QPointF p1,p2;
    QDataStream in(byte);
    in>>m_radius;
    in>>m_arrow;
    in>>p1;
    in>>p2;
    in>>m_map;
    setP1(p1);
    setP2(p2);
}

//起点终点构造线
myArrowLine::myArrowLine(const QPointF &pt1, const QPointF &pt2,const QPoint&column)
    :QLineF(pt1,pt2)
{
    init();
    m_map=column;
}

//初始化
void myArrowLine::init()
{
    m_radius=5;
    m_arrow=10;
}

//线数据
QByteArray myArrowLine::data()const
{
    QByteArray byte;
    QDataStream out(&byte,QIODevice::WriteOnly);
    out<<m_radius;
    out<<m_arrow;
    out<<p1();
    out<<p2();
    out<<m_map;

    return byte;
}



//设置原点半径
void myArrowLine::setArrowSize(qreal s)
{
    if(s<2)
        return;
    m_arrow=s;
}

//获取绘制路径
QPainterPath myArrowLine::painterPath()
{
    //长度太小不用绘制
    if(length()<10)
        return QPainterPath();

    //从原点绘制路径
    QPainterPath path;
    path.addEllipse(QPointF(0,0),m_radius,m_radius);
    path.moveTo(QPointF(m_radius,0));
    path.lineTo(length(),0);
    path.lineTo(length()-m_arrow,0.268*m_arrow);
    path.lineTo(length()-m_arrow,-0.268*m_arrow);
    path.lineTo(length(),0);

    //变换路径
    QTransform transform;
    transform.translate(p1().x(),p1().y());
    transform.rotate(-angle());

    path= transform.map(path);


    return path;
}

void myArrowLine::setPointRadius(qreal r)
{
    if(r<2)
        return;
    m_radius=r;
}


//是否属于外购件
bool myArrowLine::isPurchaseMapping(QPoint&map) const
{
    bool b=p2().y()<p1().y();
    map=m_map;
    return b;
}

qreal myArrowLine::distenceToPoint(const QPointF &p) const
{
    qreal x0=p.x();
    qreal y0=p.y();
    qreal x1=p1().x();
    qreal y1=p1().y();
    qreal x2=p2().x();
    qreal y2=p2().y();

    qreal l=qSqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    qreal l1=qSqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
    qreal l2=qSqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));


    qreal d=(l1+l2-l)/l;

    return d;
}
