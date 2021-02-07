#ifndef UBTEXTVALUEITEM_H
#define UBTEXTVALUEITEM_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QString>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QRectF>
#include <QColor>
#include <QPainterPath>
class UBTextValueItem:public QGraphicsItem
{
public:
    UBTextValueItem(qreal _width,qreal _height,const QString _text,const QString _spliter,const QString _value,QGraphicsItem *parent=0);
    void setText(const QString& _text);
    void setSpliter(const QString& _spliter);
    void setValue(const QString& _value);
    void setTextColor(const QColor& _color);
    void setSpliterColor(const QColor& _color);
    void setValueColor(const QColor& _color);
    void setBGColor(const QColor _color);
protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    qreal itemWidth;
    qreal itemHeight;
    QString itemText;
    QColor textColor;
    QString itemSpliter;
    QColor spliterColor;
    QString itemValue;
    QColor valueColor;
    QColor BGColor;
};

#endif // UBTEXTVALUEITEM_H
