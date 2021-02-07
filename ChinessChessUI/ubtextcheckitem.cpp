#include "ubtextcheckitem.h"
#include "Macro_Const.h"
#include <QFontMetricsF>
#include <QBrush>
#include <QPen>
#include <QPainter>

UBTextCheckItem::UBTextCheckItem(qreal _width, qreal _height, const QString &_text, const QString &_spliter, bool _isChecked, QGraphicsItem *parent):
                                itemWidth(_width),itemHeight(_height),itemText(_text),itemSpliter(_spliter),isChecked(_isChecked),QGraphicsItem(parent),
                                textColor(chessWidgetTextColor),spliterColor(chessWidgetTextColor),checkedColor(chessWidgetTextColor),
                                outerColor(chessWidgetTextColor)
{

}

void UBTextCheckItem::setText(const QString &_text)
{
    prepareGeometryChange();
    this->itemText=_text;
    update();
}

void UBTextCheckItem::setSpliter(const QString &_spliter)
{
    prepareGeometryChange();
    this->itemSpliter=_spliter;
    update();
}

void UBTextCheckItem::setChecked(bool _isChecked)
{
    this->isChecked=_isChecked;
    update();
}

void UBTextCheckItem::setTextColor(const QColor &_color)
{
    this->textColor=_color;
    update();
}

void UBTextCheckItem::setSpliterColor(const QColor &_color)
{
    this->spliterColor=_color;
    update();
}

void UBTextCheckItem::setCheckedColor(const QColor &_color)
{
    this->checkedColor=_color;
    update();
}

void UBTextCheckItem::setOuterColor(const QColor &_color)
{
    this->outerColor=_color;
    update();
}

QRectF UBTextCheckItem::boundingRect() const
{
    return QRectF(0,0,itemWidth,itemHeight);
}

QPainterPath UBTextCheckItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void UBTextCheckItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush bgBrush(chessWidgetBGColor);
    painter->setBrush(bgBrush);
    painter->drawRect(boundingRect());

    QPen textPen(textColor);
    QPen spliterPen(spliterColor);
    QBrush checkedBrush(checkedColor);
    QFont textFont=chessWidgetTextFont;
    painter->setFont(textFont);
    QFontMetricsF fontMetricsF(textFont);
    qreal bottomMargin=fontMetricsF.height();
    QPointF textPt(2*UB_CHESSBOARD_MARGIN,bottomMargin);
    qreal spliterX=fontMetricsF.width(itemText);
    QPointF spliterPt(textPt.x()+spliterX,bottomMargin);
    QPointF checkedPt(spliterPt.x()+fontMetricsF.width(itemSpliter),bottomMargin);
    QRectF checkedBox(checkedPt.x()+itemWidth*2/15,checkedPt.y()-itemHeight/2.2,itemWidth-checkedPt.x()-itemWidth*4/15,itemHeight/2);

    painter->setPen(textPen);
    painter->drawText(textPt,itemText);
    painter->setPen(spliterPen);
    painter->drawText(spliterPt,itemSpliter);
    setOpacity(0.5);
    painter->setBrush(checkedBrush);
    painter->drawRect(checkedBox);

    if(isChecked)
    {
        setOpacity(1);
        checkedBox.adjust(-2,-2,2,2);
        QPen outerPen(outerColor);
        outerPen.setWidth(2);
        painter->setPen(outerPen);
        painter->drawLine(checkedBox.x(),checkedBox.y(),checkedBox.x(),checkedBox.y()+checkedBox.height());
        painter->drawLine(checkedBox.x(),checkedBox.y(),checkedBox.x()+checkedBox.width(),checkedBox.y());
        painter->drawLine(checkedBox.x()+checkedBox.width(),checkedBox.y(),checkedBox.x()+checkedBox.width(),checkedBox.y()+checkedBox.height());
        painter->drawLine(checkedBox.x(),checkedBox.y()+checkedBox.height(),checkedBox.x()+checkedBox.width(),checkedBox.y()+checkedBox.height());
    }
}

