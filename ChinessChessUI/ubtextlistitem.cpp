#include "ubtextlistitem.h"
#include "Macro_Const.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetricsF>
#include <iostream>
using std::cout;
using std::endl;

UBTextListItem::UBTextListItem(qreal _width, qreal _height, const QString &_title, const QString &_spliter, QGraphicsItem *parent):
    itemWidth(_width),itemHeight(_height),itemTitle(_title),itemSpliter(_spliter),titleColor(chessWidgetTextColor),
    spliterColor(chessWidgetTextColor),listColor(chessWidgetTextColor),visibleNumber(listVisibleNumber),firstIndex(0),QGraphicsItem(parent)
{
}

void UBTextListItem::setTitle(const QString &_title)
{
    prepareGeometryChange();
    this->itemTitle=_title;
    update();
}

void UBTextListItem::setTitleColor(const QColor &_color)
{
    this->titleColor=_color;
    update();
}

void UBTextListItem::setSpliter(const QString &_spliter)
{
    prepareGeometryChange();
    this->itemSpliter=_spliter;
    update();
}

void UBTextListItem::setSpliterColor(const QColor &_color)
{
    this->spliterColor=_color;
    update();
}

void UBTextListItem::addValue(const QString &_value)
{
    prepareGeometryChange();
    this->itemList.append(_value);
    firstIndex=0;
    update();
}

QString UBTextListItem::removeValue()
{
    prepareGeometryChange();
    if(itemList.count() == 0)
        return QString();
    QString lastStr=itemList.last();
    itemList.removeLast();
    update();
    return lastStr;
}

void UBTextListItem::clearList()
{
    prepareGeometryChange();
    itemList.clear();
    update();
}

void UBTextListItem::setListColor(const QColor &_color)
{
    this->listColor=_color;
    update();
}

QRectF UBTextListItem::boundingRect() const
{
    return QRectF(0,0,itemWidth,itemHeight);
}

QPainterPath UBTextListItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void UBTextListItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush bgBrush(chessWidgetBGColor);
    painter->setBrush(bgBrush);
    painter->drawRect(boundingRect());

    QPen titlePen(titleColor);
    QPen spliterPen(spliterColor);
    QPen listPen(listColor);
    QFont listFont=chessWidgetTextFont;
    qreal listMargin=1;
    qreal eachHeight=((itemHeight-(visibleNumber+1)*listMargin)/visibleNumber);
    listFont.setPointSize(eachHeight/2);
    QFontMetricsF fontMetrics(listFont);
    QPointF titlePt(2*UB_CHESSBOARD_MARGIN,eachHeight/1.2);
    qreal spliterX=((fontMetrics.width(itemTitle) > itemWidth*1/3)?fontMetrics.width(itemTitle):itemWidth*1/3);
    QPointF spliterPt(titlePt.x()+spliterX,eachHeight/1.2);
    QPointF listPt(spliterPt.x()+fontMetrics.width(itemSpliter),eachHeight/1.2);
    painter->setPen(titlePen);
    painter->setFont(listFont);
    painter->drawText(titlePt,itemTitle);
    painter->setPen(spliterPen);
    painter->drawText(spliterPt,itemSpliter);
    painter->setPen(listPen);
    for(int i=0;i<visibleNumber;i++)
    {
        listPt.setY(titlePt.y()+i*(listMargin+eachHeight));
        if(itemList.count() <= (i+firstIndex) )
            break;
        else
        {
            QString tempStr=itemList.at(itemList.count()-(i+firstIndex)-1);
            painter->drawText(listPt,tempStr);
        }
    }
}

void UBTextListItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int off=event->delta()/120;
    if(off==0)
        return;
    //away from user
    //so should back to old list
    firstIndex-=off;
    if(firstIndex<0)
        firstIndex=0;
    if(firstIndex >= (itemList.count()-1))
        firstIndex=itemList.count()-1;
    update();
    //firstIndex=firstIndex
}
