#include "ubgraphicsscene.h"
#include <QBrush>
#include <QPen>
#include <QFont>
#include "Macro_Const.h"
#include <QList>
#include <QListIterator>
#include <QMessageBox>
#include <iostream>
#include <QString>
using std::cout;
using std::endl;
UBGraphicsScene::UBGraphicsScene(QObject *_parent):QGraphicsScene(_parent),lastItem(NULL)
{
    createConnection();
}
UBGraphicsScene::UBGraphicsScene(const QRectF& _sceneRect,QObject *_parent):QGraphicsScene(_sceneRect,_parent),lastItem(NULL)
{
    createConnection();
}
UBGraphicsScene::UBGraphicsScene(qreal _x, qreal _y, qreal _width, qreal _height, QObject *_parent):QGraphicsScene(_x,_y,_width,_height,_parent),lastItem(NULL)
{
    createConnection();
}
void UBGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    //set Scene BackGround color
    QBrush sceneBrush(sceneBGColor);
    painter->setBrush(sceneBrush);
    painter->drawRect(rect);
    //painter->fillRect(rect,Qt::Dense5Pattern);
    //some var for draw
    int startX,startY,endX,endY,minSide,marginWidth,cellWidth;
    startX=startY=endX=endY=0;
    minSide=(rect.width()>rect.height())?rect.height():rect.width();
    marginWidth=UB_CHESSBOARD_MARGIN;
    cellWidth=(minSide-marginWidth*14)/9;
    //set scene BackGround Grid
    //outer rect
    QPen gridPen(chessBoardOutRectColor);
    gridPen.setWidth(3);
    painter->setPen(gridPen);
    startX=rect.x()+marginWidth*6;
    startY=rect.y()+marginWidth*6;
    endX=startX+marginWidth*2+cellWidth*8;
    endY=startY+marginWidth*2+cellWidth*9;
    painter->drawLine(startX,startY,endX,startY);
    painter->drawLine(startX,startY,startX,endY);
    painter->drawLine(endX,startY,endX,endY);
    painter->drawLine(startX,endY,endX,endY);
    QBrush backBrush(chessBoardBackColor);
    painter->setBrush(backBrush);
    painter->drawRect(startX,startY,endX-startX,endY-startY);
    //inner rect
    gridPen.setWidth(2);
    gridPen.setColor(chessBoardInnerRectColor);
    painter->setPen(gridPen);
    startX+=marginWidth;startY+=marginWidth;endX-=marginWidth;endY-=marginWidth;
    painter->drawLine(startX,startY,endX,startY);
    painter->drawLine(startX,startY,startX,endY);
    painter->drawLine(endX,startY,endX,endY);
    painter->drawLine(startX,endY,endX,endY);
    //cell rect
    gridPen.setColor(chessBoardGridColor);
    painter->setPen(gridPen);
    for(int i=1;i<9;i++)
    {
        painter->drawLine(startX,startY+i*cellWidth,endX,startY+i*cellWidth);
    }
    for(int i=1;i<8;i++)
    {
        painter->drawLine(startX+i*cellWidth,startY,startX+i*cellWidth,startY+4*cellWidth);
        painter->drawLine(startX+i*cellWidth,startY+5*cellWidth,startX+i*cellWidth,endY);
    }
    //king and advisor line
    painter->drawLine(startX+3*cellWidth,startY,startX+5*cellWidth,startY+2*cellWidth);
    painter->drawLine(startX+3*cellWidth,startY+2*cellWidth,startX+5*cellWidth,startY);
    painter->drawLine(startX+3*cellWidth,endY-2*cellWidth,startX+5*cellWidth,endY);
    painter->drawLine(startX+3*cellWidth,endY,startX+5*cellWidth,endY-2*cellWidth);
    //pawn base icon
    gridPen.setColor(chessBoardPawnBaseColor);
    painter->setPen(gridPen);
    QPoint fourPawnBase[10]={QPoint(startX+cellWidth,startY+cellWidth*2),QPoint(startX+cellWidth*2,startY+cellWidth*3),
                                                    QPoint(startX+cellWidth*4,startY+cellWidth*3),QPoint(startX+cellWidth*6,startY+cellWidth*3),
                                                    QPoint(startX+cellWidth*7,startY+cellWidth*2),QPoint(startX+cellWidth*2,startY+cellWidth*6),
                                                    QPoint(startX+cellWidth*4,startY+cellWidth*6),QPoint(startX+cellWidth*6,startY+cellWidth*6),
                                                    QPoint(startX+cellWidth,startY+cellWidth*7),QPoint(startX+cellWidth*7,startY+cellWidth*7)};
    for(int i=0;i<10;i++)
        drawPawnBaseForFour(painter,marginWidth,marginWidth*2.5,fourPawnBase[i]);
    QPoint leftTwoPawnBase[2]={QPoint(endX,startY+cellWidth*3),QPoint(endX,startY+cellWidth*6)};
    for(int i=0;i<2;i++)
        drawPawnBaseForLeftTwo(painter,marginWidth,marginWidth*2.5,leftTwoPawnBase[i]);
    QPoint rightTwoPawnBase[2]={QPoint(startX,startY+cellWidth*3),QPoint(startX,startY+cellWidth*6)};
    for(int i=0;i<2;i++)
        drawPawnBaseForRightTwo(painter,marginWidth,marginWidth*2.5,rightTwoPawnBase[i]);
    //river text
    gridPen.setColor(chessBoardRiverTextColor);
    painter->setPen(gridPen);
    QFont riverTextFont(chessBoardRiverTextFont);
    painter->setFont(riverTextFont);
    painter->drawText(startX+cellWidth+marginWidth*2,startY+cellWidth*4+marginWidth*7,QString("楚"));
    painter->drawText(startX+cellWidth*2+marginWidth*2,startY+cellWidth*4+marginWidth*7,QString("河"));
    painter->drawText(startX+cellWidth*5+marginWidth*2,startY+cellWidth*4+marginWidth*7,QString("汉"));
    painter->drawText(startX+cellWidth*6+marginWidth*2,startY+cellWidth*4+marginWidth*7,QString("界"));
}

void UBGraphicsScene::createConnection()
{
    connect(this,SIGNAL(selectionChanged()),this,SLOT(handleSelectionChanged()));
}

void UBGraphicsScene::handleSelectionChanged()
{
    QList<QGraphicsItem *> itemList=this->selectedItems();
    if(itemList.count()==0)
    {
        lastItem=NULL;
    }else if(itemList.count()==1)
    {
        UBChessManItem *newSelectItem=dynamic_cast<UBChessManItem *>(itemList.at(0));
        //if no type of UBChessManItem
        //          or  last is no select
        //          or  is same item
        if(newSelectItem && lastItem && (newSelectItem != lastItem))
        {
            cout<<"emit signal for move :"<<endl;
            emit this->itemIJChange(lastItem,newSelectItem);
            lastItem=NULL;
        }else if(newSelectItem &&
        ((newSelectItem->isRedChessMan()==0) || (newSelectItem->isRedChessMan()==1)) &&
        (newSelectItem != lastItem))
        {
            cout<<"set lastItem :"<<endl;
            lastItem=newSelectItem;
            emit this->lastItemHasSelect();
        }else if(newSelectItem==NULL || (newSelectItem->isRedChessMan()!=0 && newSelectItem->isRedChessMan()!=1))
        {
            cout<<"set lastItem as NULL"<<endl;
            lastItem=NULL;
            emit this->lastItemSetNULL();
        }
    }
}
void UBGraphicsScene::drawPawnBaseForFour(QPainter *painter, int margin,int length, const QPoint &point)
{
    //some var for paint
    int startX,startY;
    startX=startY=0;
    //left and up
    startX=point.x()-margin;
    startY=point.y()-margin;
    painter->drawLine(startX,startY,startX-length,startY);
    painter->drawLine(startX,startY,startX,startY-length);
    //left and down
    startX=point.x()-margin;
    startY=point.y()+margin;
    painter->drawLine(startX,startY,startX,startY+length);
    painter->drawLine(startX,startY,startX-length,startY);
    //right and up
    startX=point.x()+margin;
    startY=point.y()-margin;
    painter->drawLine(startX,startY,startX+length,startY);
    painter->drawLine(startX,startY,startX,startY-length);
    //right and down
    startX=point.x()+margin;
    startY=point.y()+margin;
    painter->drawLine(startX,startY,startX+length,startY);
    painter->drawLine(startX,startY,startX,startY+length);
}
void UBGraphicsScene::drawPawnBaseForLeftTwo(QPainter *painter, int margin,int length, const QPoint &point)
{
    //some var for paint
    int startX,startY;
    startX=startY=0;
    //left and up
    startX=point.x()-margin;
    startY=point.y()-margin;
    painter->drawLine(startX,startY,startX-length,startY);
    painter->drawLine(startX,startY,startX,startY-length);
    //left and down
    startX=point.x()-margin;
    startY=point.y()+margin;
    painter->drawLine(startX,startY,startX,startY+length);
    painter->drawLine(startX,startY,startX-length,startY);
}
void UBGraphicsScene::drawPawnBaseForRightTwo(QPainter *painter,int margin, int length, const QPoint &point)
{
    //some var for paint
    int startX,startY;
    startX=startY=0;
    //right and up
    startX=point.x()+margin;
    startY=point.y()-margin;
    painter->drawLine(startX,startY,startX+length,startY);
    painter->drawLine(startX,startY,startX,startY-length);
    //right and down
    startX=point.x()+margin;
    startY=point.y()+margin;
    painter->drawLine(startX,startY,startX+length,startY);
    painter->drawLine(startX,startY,startX,startY+length);
}
