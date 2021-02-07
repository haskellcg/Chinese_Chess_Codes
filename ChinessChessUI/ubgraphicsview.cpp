#include "ubgraphicsview.h"

UBGraphicsView::UBGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    createAction();
}

void UBGraphicsView::createAction()
{
    this->mainMenu=new QMenu(this);
    this->newGame=this->mainMenu->addAction(QIcon(":/1.ico"),tr("&New Game"));
    connect(this->newGame,SIGNAL(triggered()),this,SIGNAL(newGameSig()));
    this->mainMenu->addSeparator();
    this->exitAction=this->mainMenu->addAction(QIcon(":/4.ico"),tr("&Exit"));
    connect(this->exitAction,SIGNAL(triggered()),this,SIGNAL(closeView()));
}

void UBGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    this->mainMenu->exec(event->globalPos());
}
