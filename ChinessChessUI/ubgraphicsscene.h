#ifndef UBGRAPHICSSCENE_H
#define UBGRAPHICSSCENE_H
#include "ubchessmanitem.h"
#include <QGraphicsScene>
#include <QRectF>
#include <QPainter>
#include <QObject>
class UBGraphicsScene:public QGraphicsScene
{
    Q_OBJECT

public:
    UBGraphicsScene(QObject * _parent=0);
    UBGraphicsScene(const QRectF& _sceneRect,QObject *_parent=0);
    UBGraphicsScene(qreal _x,qreal _y,qreal _width,qreal _height,QObject *_parent=0);
private:
    void drawBackground(QPainter *painter,const QRectF &rect);
    void createConnection();
signals:
    void itemIJChange(UBChessManItem *lastItem,UBChessManItem *newSelectItem);
    void lastItemHasSelect();
    void lastItemSetNULL();
private slots:
    void handleSelectionChanged();
private:
    void drawPawnBaseForFour(QPainter *painter,int margin,int length,const QPoint &point);
    void drawPawnBaseForLeftTwo(QPainter *painter,int margin,int length,const QPoint &point);
    void drawPawnBaseForRightTwo(QPainter *painter,int margin,int length,const QPoint &point);
private:
    UBChessManItem *lastItem;
};

#endif // UBGRAPHICSSCENE_H
