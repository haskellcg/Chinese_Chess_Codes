#ifndef UBTEXTLISTITEM_H
#define UBTEXTLISTITEM_H
#include <QGraphicsItem>
#include <QRectF>
#include <QPainterPath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QString>
#include <QList>
#include <QColor>
#include <QGraphicsSceneWheelEvent>

class UBTextListItem : public QGraphicsItem
{
public:
    UBTextListItem(qreal _width,qreal _height,const QString& _title,const QString& _spliter,QGraphicsItem *parent=0);
    void setTitle(const QString& _title);
    void setTitleColor(const QColor& _color);
    void setSpliter(const QString& _spliter);
    void setSpliterColor(const QColor& _color);
    void addValue(const QString& _value);
    QString removeValue();
    void clearList();
    void setListColor(const QColor& _color);
protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
private:
    qreal itemWidth;
    qreal itemHeight;
    QString itemTitle;
    QColor titleColor;
    QString itemSpliter;
    QColor spliterColor;
    QList<QString> itemList;
    QColor listColor;
    int visibleNumber;
    int firstIndex;
};

#endif // UBTEXTLISTITEM_H
