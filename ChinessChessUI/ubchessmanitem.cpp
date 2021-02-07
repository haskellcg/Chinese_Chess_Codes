#include "ubchessmanitem.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QFontMetricsF>
#include <iostream>
using std::cout;
using std::endl;
UBChessManItem::UBChessManItem(int _isRed,qreal _width,qreal _height,const QString& _chessManText,QGraphicsItem *parent):QGraphicsItem(parent),
    chessManWidth(_width),chessManHeight(_height),isRed(_isRed),chessManText(_chessManText)
{
    setFlags(QGraphicsItem::ItemIsSelectable);
    this->isHaveBeenLocated=false;
    if(_chessManText == "帥")
        this->chessManType=RED_KING;
    else if(_chessManText=="將")
            this->chessManType=BLACK_KING;
    else if(_chessManText=="仕")
            this->chessManType=RED_ADVISOR;
    else if(_chessManText=="士")
            this->chessManType=BLACK_ADVISOR;
    else if(_chessManText=="相")
            this->chessManType=RED_BISHOP;
    else if(_chessManText=="象")
            this->chessManType=BLACK_BISHOP;
    else if(_chessManText=="俥")
            this->chessManType=RED_ROOK;
    else if(_chessManText=="車")
           this->chessManType=BLACK_ROOK;
    else if(_chessManText=="傌")
            this->chessManType=RED_KNIGHT;
    else if(_chessManText=="馬")
            this->chessManType=BLACK_KNIGHT;
    else if(_chessManText=="炮")
            this->chessManType=RED_CANNON;
    else if(_chessManText=="砲")
            this->chessManType=BLACK_CANNON;
    else if(_chessManText=="兵")
            this->chessManType=RED_PAWN;
    else if(_chessManText=="卒")
            this->chessManType=BLACK_PAWN;
    else
            this->chessManType=NONE_TYPE;
}
QRectF UBChessManItem::boundingRect() const
{
    QRectF rect(-chessManWidth/2,-chessManHeight/2,chessManWidth,chessManHeight);
    rect.adjust(-UB_CHESSMAN_ADJUST,-UB_CHESSMAN_ADJUST,UB_CHESSMAN_ADJUST,UB_CHESSMAN_ADJUST);
    return rect;
}
QPainterPath UBChessManItem::shape() const
{
    QPainterPath shapePath;
    shapePath.addEllipse(QRectF(-chessManWidth/2,-chessManHeight/2,chessManWidth,chessManHeight));
    return shapePath;
}
void UBChessManItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen chessManPen;
    QBrush chessManBrush;
    QFont chessManFont(chessManTextFont);
    if(isRed==1)
    {
        chessManPen=redChessManFGColor;
        chessManBrush=redChessManBGColor;
    }else if(isRed==0)
    {
        chessManPen=blackChessManFGColor;
        chessManBrush=blackChessManBGColor;
    }else
    {
        //opacity chessMan Draw pawn icon when Selected
        chessManPen=opacityChessManFGColor;
        if(option->state & QStyle::State_Selected)
        {
            chessManPen.setWidth(3);
            painter->setPen(chessManPen);
            setOpacity(1);

            qreal startX=boundingRect().x();
            qreal startY=boundingRect().y();
            qreal endX=startX+boundingRect().width();
            qreal endY=startY+boundingRect().height();
            qreal length=boundingRect().width()/10;
            painter->drawLine(startX,startY,startX,startY+length);
            painter->drawLine(startX,startY,startX+length,startY);
            painter->drawLine(startX,endY,startX,endY-length);
            painter->drawLine(startX,endY,startX+length,endY);
            painter->drawLine(endX,startY,endX,startY+length);
            painter->drawLine(endX,startY,endX-length,startY);
            painter->drawLine(endX,endY,endX,endY-length);
            painter->drawLine(endX,endY,endX-length,endY);
        }
        this->scene()->update();
        return;
    }
    if(option->state & QStyle::State_Selected)
    {
       chessManPen.setWidth(3);
       painter->setPen(chessManPen);
       setOpacity(1);

       qreal startX=boundingRect().x();
       qreal startY=boundingRect().y();
       qreal endX=startX+boundingRect().width();
       qreal endY=startY+boundingRect().height();
       qreal length=boundingRect().width()/10;
       painter->drawLine(startX,startY,startX,startY+length);
       painter->drawLine(startX,startY,startX+length,startY);
       painter->drawLine(startX,endY,startX,endY-length);
       painter->drawLine(startX,endY,startX+length,endY);
       painter->drawLine(endX,startY,endX,startY+length);
       painter->drawLine(endX,startY,endX-length,startY);
       painter->drawLine(endX,endY,endX,endY-length);
       painter->drawLine(endX,endY,endX-length,endY);
    }else
    {
        chessManPen.setWidth(2);
        painter->setPen(chessManPen);
        setOpacity(0.95);
    }
    painter->setBrush(chessManBrush);
    painter->setFont(chessManFont);
    painter->drawEllipse(boundingRect());
    painter->drawText(-(2*UB_CHESSBOARD_MARGIN+((qreal)UB_CHESSBOARD_MARGIN)/1.3),2*UB_CHESSBOARD_MARGIN,chessManText);
    this->scene()->update();
}
qreal UBChessManItem::getWidth() const
{return this->chessManWidth;}
qreal UBChessManItem::getHeight() const
{return this->chessManHeight;}
int UBChessManItem::isRedChessMan() const
{return this->isRed;}
QString UBChessManItem::getText() const
{return this->chessManText;}
ChessManType UBChessManItem::getChessType() const
{return this->chessManType;}
void UBChessManItem::setLocateIJ(int I, int J)
{this->locatedI=I;this->locatedJ=J;this->isHaveBeenLocated=true;}
int UBChessManItem::getLocateI() const
{return this->locatedI;}
int UBChessManItem::getLocateJ() const
{return this->locatedJ;}
