#ifndef UBCHESSMANITEM_H
#define UBCHESSMANITEM_H
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QRectF>
#include <QPainterPath>
#include <QString>
#include "Macro_Const.h"
class UBChessManItem:public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    UBChessManItem(int _isRed,qreal _width,qreal _height,const QString& _chessManText,QGraphicsItem *parent=0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    qreal getWidth() const;
    qreal getHeight() const;
    int isRedChessMan() const;
    QString getText() const;
    ChessManType getChessType() const;
    void setLocateIJ(int I,int J);
    int  getLocateI() const;
    int  getLocateJ() const;
private:
    qreal chessManWidth;
    qreal chessManHeight;
    int isRed;                              //1-red 0-black -1-opacity
    QString chessManText;
    ChessManType chessManType;
    bool isHaveBeenLocated;
    int  locatedI;
    int  locatedJ;
};

#endif // UBCHESSMANITEM_H
