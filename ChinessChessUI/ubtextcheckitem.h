#ifndef UBTEXTCHECKITEM_H
#define UBTEXTCHECKITEM_H
#include <QGraphicsItem>
#include <QString>
#include <QRectF>
#include <QPainterPath>
#include <QColor>
class UBTextCheckItem : public QGraphicsItem
{
public:
    UBTextCheckItem(qreal _width,qreal _height,const QString& _text,const QString& _spliter,bool _isChecked,QGraphicsItem *parent=0);
    void setText(const QString& _text);
    void setSpliter(const QString& _spliter);
    void setChecked(bool _isChecked);
    void setTextColor(const QColor& _color);
    void setSpliterColor(const QColor& _color);
    void setCheckedColor(const QColor& _color);
    void setOuterColor(const QColor& _color);
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
    bool isChecked;
    QColor checkedColor;
    QColor outerColor;

};

#endif // UBTEXTCHECKITEM_H
