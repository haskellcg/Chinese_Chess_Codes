#include "ubtextvalueitem.h"
#include "Macro_Const.h"
#include <QFontMetricsF>

UBTextValueItem::UBTextValueItem(qreal _width, qreal _height, const QString _text, const QString _spliter, const QString _value, QGraphicsItem *parent):
                                itemWidth(_width),itemHeight(_height),itemText(_text),itemSpliter(_spliter),itemValue(_value),QGraphicsItem(parent),
                                textColor(chessWidgetTextColor),spliterColor(chessWidgetTextColor),valueColor(chessWidgetTextColor),BGColor(chessWidgetBGColor)
{
}

void UBTextValueItem::setText(const QString &_text)
{
    prepareGeometryChange();
    this->itemText=_text;
    update();
}

void UBTextValueItem::setSpliter(const QString &_spliter)
{
    prepareGeometryChange();
    this->itemSpliter=_spliter;
    update();
}

void UBTextValueItem::setValue(const QString &_value)
{
    prepareGeometryChange();
    this->itemValue=_value;
    update();
}

void UBTextValueItem::setTextColor(const QColor &_color)
{
    this->textColor=_color;
    update();
}

void UBTextValueItem::setSpliterColor(const QColor &_color)
{
    this->spliterColor=_color;
    update();
}

void UBTextValueItem::setValueColor(const QColor &_color)
{
    this->valueColor=_color;
    update();
}

void UBTextValueItem::setBGColor(const QColor _color)
{
    this->BGColor=_color;
    update();
}


QRectF UBTextValueItem::boundingRect() const
{
    QRectF boudingR(0,0,itemWidth,itemHeight);
    return boudingR;
}

QPainterPath UBTextValueItem::shape() const
{
    QPainterPath path;
    path.addRect(0,0,itemWidth,itemHeight);
    return path;
}

void UBTextValueItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush bgBrush(BGColor);
    painter->setBrush(bgBrush);
    painter->drawRect(boundingRect());

    QPen textPen(textColor);
    QPen spliterPen(spliterColor);
    QPen valuePen(valueColor);
    QFont textFont=chessWidgetTextFont;
    painter->setFont(textFont);
    QFontMetricsF fontMetricsF(textFont);
    qreal bottomMargin=fontMetricsF.height();
    QPointF textPt(2*UB_CHESSBOARD_MARGIN,bottomMargin);
    qreal spliterX=((fontMetricsF.width(itemText) > itemWidth*1/2)?fontMetricsF.width(itemText):itemWidth*1/2);
    QPointF spliterPt(textPt.x()+spliterX,bottomMargin);
    QPointF valuePt(spliterPt.x()+fontMetricsF.width(itemSpliter)+itemWidth/10,bottomMargin);
    painter->setPen(textPen);
    painter->drawText(textPt,itemText);
    painter->setPen(spliterPen);
    painter->drawText(spliterPt,itemSpliter);
    painter->setPen(valuePen);
    painter->drawText(valuePt,itemValue);
}

