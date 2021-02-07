#include "mainwindow.h"
#include "Macro_Const.h"
#include <QList>
#include <QGraphicsTextItem>
#include <QSound>
#include <iostream>
using std::cout;
using std::endl;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),chess(NULL)
{
    this->view=new UBGraphicsView(this);
    connect(this->view,SIGNAL(closeView()),this,SLOT(close()));
    connect(this->view,SIGNAL(newGameSig()),this,SLOT(newGameSlot()));
    this->scene=new UBGraphicsScene(0,0,UB_WINDOW_WIDTH,UB_WINDOW_HEIGHT);
    connect(this->scene,SIGNAL(itemIJChange(UBChessManItem*,UBChessManItem*)),this,SLOT(locateItem(UBChessManItem*,UBChessManItem*)));
    connect(this->scene,SIGNAL(lastItemHasSelect()),this,SLOT(updateItemForOppositeTure()));
    connect(this->scene,SIGNAL(lastItemSetNULL()),this,SLOT(updateItemForOpacityAndTurn()));
    CChessStruct *_chess=new CChessStruct;
    this->chess=makeSmartPtr(_chess);
    this->moveMaker.bindStruct(this->chess);
    initializeGrid();
    buildChessMan();
    buildOtherWidget();
    initializeUIFromStruct();
    initNamedPipe();
    initUCCIEngine();
    this->fifoHandle=0;

    view->setScene(this->scene);
    view->setRenderHints(QPainter::Antialiasing);
    this->setCentralWidget(view);
    this->setWindowTitle(tr("ChinessChess"));
    this->setWindowOpacity(1.0);
    this->setWindowIcon(QIcon(":/8.png"));
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

MainWindow::~MainWindow()
{
    quitUCCIEngine();
    destoryNamedPipe();
}

void MainWindow::setChessStruct(const char *FENStr)
{
    this->chess->initializeFromFEN(FENStr);
    this->moveMaker.bindStruct(this->chess);
    this->initializeUIFromStruct();

    char buffer[BUFFERSIZE];
    memset(buffer,0,BUFFERSIZE);
    lastPositionCMD.fenCMDName="fen";
    lastPositionCMD.fenString=UBString(this->chess->toFENStr().getPtr());
    lastPositionCMD.movesCMDName="";
    lastPositionCMD.movesForFenLen=0;
    UBString ucciStr=lastPositionCMD.toString();
    fifoHandle=::open(FIFONAME,O_WRONLY);
    ::write(fifoHandle,ucciStr.getData(),ucciStr.length());
    ::close(fifoHandle);
    cout<<"sending "<<ucciStr<<endl;
    fifoHandle=::open(FIFONAME,O_RDONLY);
    ::read(fifoHandle,buffer,BUFFERSIZE);
    ::close(fifoHandle);
    cout<<"RCV:"<<buffer<<endl;
}

void MainWindow::buildChessMan()
{
    UBGraphicsScene *_scene=this->scene;
    qreal marginWidth=UB_CHESSBOARD_MARGIN;
    qreal minSide=(_scene->width()>_scene->height())?_scene->height():_scene->width();
    qreal cellWidth=(minSide-marginWidth*14)/9;
    qreal chessManWidth=cellWidth-1.6*marginWidth;
    this->redKingItem=new UBChessManItem(1,chessManWidth,chessManWidth,"帥");
    this->setEffectForItem(redKingItem,_scene);
    this->redAdvisorOneItem=new UBChessManItem(1,chessManWidth,chessManWidth,"仕");
    this->setEffectForItem(redAdvisorOneItem,_scene);
    this->redAdvisorTwoItem=new UBChessManItem(1,chessManWidth,chessManWidth,"仕");
    this->setEffectForItem(redAdvisorTwoItem,_scene);
    this->redBishopOneItem=new UBChessManItem(1,chessManWidth,chessManWidth,"相");
    this->setEffectForItem(redBishopOneItem,_scene);
    this->redBishopTwoItem=new UBChessManItem(1,chessManWidth,chessManWidth,"相");
    this->setEffectForItem(redBishopTwoItem,_scene);
    this->redKnightOneItem=new UBChessManItem(1,chessManWidth,chessManWidth,"傌");
    this->setEffectForItem(redKnightOneItem,_scene);
    this->redKnightTwoItem=new UBChessManItem(1,chessManWidth,chessManWidth,"傌");
    this->setEffectForItem(redKnightTwoItem,_scene);
    this->redRookOneItem=new UBChessManItem(1,chessManWidth,chessManWidth,"俥");
    this->setEffectForItem(redRookOneItem,_scene);
    this->redRookTwoItem=new UBChessManItem(1,chessManWidth,chessManWidth,"俥");
    this->setEffectForItem(redRookTwoItem,_scene);
    this->redCannonOneItem=new UBChessManItem(1,chessManWidth,chessManWidth,"炮");
    this->setEffectForItem(redCannonOneItem,_scene);
    this->redCannonTwoItem=new UBChessManItem(1,chessManWidth,chessManWidth,"炮");
    this->setEffectForItem(redCannonTwoItem,_scene);
    this->redPawnOneItem=new UBChessManItem(1,chessManWidth,chessManWidth,"兵");
    this->setEffectForItem(redPawnOneItem,_scene);
    this->redPawnTwoItem=new UBChessManItem(1,chessManWidth,chessManWidth,"兵");
    this->setEffectForItem(redPawnTwoItem,_scene);
    this->redPawnThreeItem=new UBChessManItem(1,chessManWidth,chessManWidth,"兵");
    this->setEffectForItem(redPawnThreeItem,_scene);
    this->redPawnFourItem=new UBChessManItem(1,chessManWidth,chessManWidth,"兵");
    this->setEffectForItem(redPawnFourItem,_scene);
    this->redPawnFiveItem=new UBChessManItem(1,chessManWidth,chessManWidth,"兵");
    this->setEffectForItem(redPawnFiveItem,_scene);
    this->blackKingItem=new UBChessManItem(0,chessManWidth,chessManWidth,"將");
    this->setEffectForItem(blackKingItem,_scene);
    this->blackAdvisorOneItem=new UBChessManItem(0,chessManWidth,chessManWidth,"士");
    this->setEffectForItem(blackAdvisorOneItem,_scene);
    this->blackAdvisorTwoItem=new UBChessManItem(0,chessManWidth,chessManWidth,"士");
    this->setEffectForItem(blackAdvisorTwoItem,_scene);
    this->blackBishopOneItem=new UBChessManItem(0,chessManWidth,chessManWidth,"象");
    this->setEffectForItem(blackBishopOneItem,_scene);
    this->blackBishopTwoItem=new UBChessManItem(0,chessManWidth,chessManWidth,"象");
    this->setEffectForItem(blackBishopTwoItem,_scene);
    this->blackKnightOneItem=new UBChessManItem(0,chessManWidth,chessManWidth,"馬");
    this->setEffectForItem(blackKnightOneItem,_scene);
    this->blackKnightTwoItem=new UBChessManItem(0,chessManWidth,chessManWidth,"馬");
    this->setEffectForItem(blackKnightTwoItem,_scene);
    this->blackRookOneItem=new UBChessManItem(0,chessManWidth,chessManWidth,"車");
    this->setEffectForItem(blackRookOneItem,_scene);
    this->blackRookTwoItem=new UBChessManItem(0,chessManWidth,chessManWidth,"車");
    this->setEffectForItem(blackRookTwoItem,_scene);
    this->blackCannonOneItem=new UBChessManItem(0,chessManWidth,chessManWidth,"砲");
    this->setEffectForItem(blackCannonOneItem,_scene);
    this->blackCannonTwoItem=new UBChessManItem(0,chessManWidth,chessManWidth,"砲");
    this->setEffectForItem(blackCannonTwoItem,_scene);
    this->blackPawnOneItem=new UBChessManItem(0,chessManWidth,chessManWidth,"卒");
    this->setEffectForItem(blackPawnOneItem,_scene);
    this->blackPawnTwoItem=new UBChessManItem(0,chessManWidth,chessManWidth,"卒");
    this->setEffectForItem(blackPawnTwoItem,_scene);
    this->blackPawnThreeItem=new UBChessManItem(0,chessManWidth,chessManWidth,"卒");
    this->setEffectForItem(blackPawnThreeItem,_scene);
    this->blackPawnFourItem=new UBChessManItem(0,chessManWidth,chessManWidth,"卒");
    this->setEffectForItem(blackPawnFourItem,_scene);
    this->blackPawnFiveItem=new UBChessManItem(0,chessManWidth,chessManWidth,"卒");
    this->setEffectForItem(blackPawnFiveItem,_scene);
}

void MainWindow::buildOtherWidget()
{
    UBGraphicsScene *_scene=this->scene;
    qreal marginWidth=UB_CHESSBOARD_MARGIN;
    qreal minSide=(_scene->width()>_scene->height())?_scene->height():_scene->width();
    qreal cellWidth=(minSide-marginWidth*14)/9;

    QPointF startPt=chessBoardGrid[0][8];
    startPt.setX(startPt.x()+8*UB_CHESSBOARD_MARGIN);
    QPointF endPt=chessBoardGrid[9][8];
    endPt.setX(UB_WINDOW_WIDTH-8*UB_CHESSBOARD_MARGIN);
    this->roundsNumberItem=new UBTextValueItem(endPt.x()-startPt.x(),cellWidth,"回合数",":","");
    _scene->addItem(roundsNumberItem);
    this->roundsNumberItem->setPos(startPt);

    QPointF capturePoint(startPt.x(),startPt.y()+1.5*cellWidth);
    this->captureStepNumberItem=new UBTextValueItem(endPt.x()-startPt.x(),cellWidth,"未吃子步数",":","");
    _scene->addItem(captureStepNumberItem);
    this->captureStepNumberItem->setPos(capturePoint);

    QPointF redCheckedPoint(startPt.x(),startPt.y()+3*cellWidth);
    this->redCheckedItem=new UBTextCheckItem((endPt.x()-startPt.x())/2.5,cellWidth,"红",":",false);
    this->redCheckedItem->setOuterColor(Qt::darkGray);
    this->redCheckedItem->setCheckedColor(Qt::red);
    _scene->addItem(redCheckedItem);
    this->redCheckedItem->setPos(redCheckedPoint);

    QPointF blackCheckedPoint(startPt.x()+((endPt.x()-startPt.x())*3/5),startPt.y()+3*cellWidth);
    this->blackCheckedItem=new UBTextCheckItem((endPt.x()-startPt.x())/2.5,cellWidth,"黑",":",false);
    this->blackCheckedItem->setOuterColor(Qt::darkGray);
    this->blackCheckedItem->setCheckedColor(Qt::black);
    _scene->addItem(blackCheckedItem);
    this->blackCheckedItem->setPos(blackCheckedPoint);

    QPointF modeSelectedPt(startPt.x(),startPt.y()+4.5*cellWidth);
    this->modeSelectedItem=new UBTextValueItem(endPt.x()-startPt.x(),cellWidth,"对战模式",":","");
    this->modeSelectedItem->setValue("人机对战");
    _scene->addItem(modeSelectedItem);
    this->modeSelectedItem->setPos(modeSelectedPt);

    QPointF moveListViewPt(startPt.x(),startPt.y()+6*cellWidth);
    this->moveListViewItem=new UBTextListItem(endPt.x()-startPt.x(),3*cellWidth,"象棋打谱",":");
    _scene->addItem(moveListViewItem);
    this->moveListViewItem->setPos(moveListViewPt);
}

void MainWindow::initializeGrid()
{
    qreal marginWidth=UB_CHESSBOARD_MARGIN;
    qreal minSide=(scene->width()>scene->height())?scene->height():scene->width();
    qreal cellWidth=(minSide-marginWidth*14)/9;
    qreal startX=scene->sceneRect().x()+marginWidth*6.2;
    qreal startY=scene->sceneRect().y()+marginWidth*6.2;
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<9;j++)
        {
            (this->chessBoardGrid[i][j]).setX(startX+j*cellWidth);
            (this->chessBoardGrid[i][j]).setY(startY+i*cellWidth);
        }
    }
}

void MainWindow::initializeUIFromStruct()
{
    QList<QGraphicsItem*> itemList=this->scene->items();
    QListIterator<QGraphicsItem*> iterator(itemList);
    while(iterator.hasNext())
    {
        QGraphicsItem *baseItem=iterator.next();
        UBChessManItem *tempItem=dynamic_cast<UBChessManItem *>(baseItem);
        if(!tempItem)
            continue;
        if(tempItem->isRedChessMan() != 0 && tempItem->isRedChessMan() != 1)
        {
            this->scene->removeItem(tempItem);
            delete tempItem;
        }
        else
        {
            this->scene->removeItem(tempItem);
        }
    }
    QParallelAnimationGroup *group=new QParallelAnimationGroup(this);
    for(int i=3;i<=12;i++)
    {
        for(int j=3;j<=11;j++)
        {
            int chessManValue=this->chess->cChessArray[i*16+j];
            switch(chessManValue)
            {
                case redKing:
                            this->scene->addItem(this->redKingItem);
                            this->createPropertyAnimation(this,redKingItem,group,redKingItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redKingItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redKingItem->setLocateIJ(i-3,j-3);
                            break;
                case redAdvisorOne:
                            this->scene->addItem(this->redAdvisorOneItem);
                            this->createPropertyAnimation(this,redAdvisorOneItem,group,redAdvisorOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redAdvisorOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redAdvisorOneItem->setLocateIJ(i-3,j-3);
                            break;
                case redAdvisorTwo:
                            this->scene->addItem(this->redAdvisorTwoItem);
                            this->createPropertyAnimation(this,redAdvisorTwoItem,group,redAdvisorTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redAdvisorTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redAdvisorTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case redBishopOne:
                            this->scene->addItem(this->redBishopOneItem);
                            this->createPropertyAnimation(this,redBishopOneItem,group,redBishopOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redBishopOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redBishopOneItem->setLocateIJ(i-3,j-3);
                            break;
                case redBishopTwo:
                            this->scene->addItem(this->redBishopTwoItem);
                            this->createPropertyAnimation(this,redBishopTwoItem,group,redBishopTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redBishopTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redBishopTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case redKnightOne:
                            this->scene->addItem(this->redKnightOneItem);
                            this->createPropertyAnimation(this,redKnightOneItem,group,redKnightOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redKnightOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redKnightOneItem->setLocateIJ(i-3,j-3);
                            break;
                case redKnightTwo:
                            this->scene->addItem(this->redKnightTwoItem);
                            this->createPropertyAnimation(this,redKnightTwoItem,group,redKnightTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redKnightTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redKnightTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case redRookOne:
                            this->scene->addItem(this->redRookOneItem);
                            this->createPropertyAnimation(this,redRookOneItem,group,redRookOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redRookOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redRookOneItem->setLocateIJ(i-3,j-3);
                            break;
                case redRookTwo:
                            this->scene->addItem(this->redRookTwoItem);
                            this->createPropertyAnimation(this,redRookTwoItem,group,redRookTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redRookTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redRookTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case redCannonOne:
                            this->scene->addItem(this->redCannonOneItem);
                            this->createPropertyAnimation(this,redCannonOneItem,group,redCannonOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redCannonOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redCannonOneItem->setLocateIJ(i-3,j-3);
                            break;
                case redCannonTwo:
                            this->scene->addItem(this->redCannonTwoItem);
                            this->createPropertyAnimation(this,redCannonTwoItem,group,redCannonTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redCannonTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redCannonTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case redPawnOne:
                            this->scene->addItem(this->redPawnOneItem);
                            this->createPropertyAnimation(this,redPawnOneItem,group,redPawnOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redPawnOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redPawnOneItem->setLocateIJ(i-3,j-3);
                            break;
                case redPawnTwo:
                            this->scene->addItem(this->redPawnTwoItem);
                            this->createPropertyAnimation(this,redPawnTwoItem,group,redPawnTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redPawnTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redPawnTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case redPawnThree:
                            this->scene->addItem(this->redPawnThreeItem);
                            this->createPropertyAnimation(this,redPawnThreeItem,group,redPawnThreeItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redPawnThreeItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redPawnThreeItem->setLocateIJ(i-3,j-3);
                            break;
                case redPawnFour:
                            this->scene->addItem(this->redPawnFourItem);
                            this->createPropertyAnimation(this,redPawnFourItem,group,redPawnFourItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redPawnFourItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redPawnFourItem->setLocateIJ(i-3,j-3);
                            break;
                case redPawnFive:
                            this->scene->addItem(this->redPawnFiveItem);
                            this->createPropertyAnimation(this,redPawnFiveItem,group,redPawnFiveItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->redPawnFiveItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->redPawnFiveItem->setLocateIJ(i-3,j-3);
                            break;
                case blackKing:
                            this->scene->addItem(this->blackKingItem);
                            this->createPropertyAnimation(this,blackKingItem,group,blackKingItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackKingItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackKingItem->setLocateIJ(i-3,j-3);
                            break;
                case blackAdvisorOne:
                            this->scene->addItem(this->blackAdvisorOneItem);
                            this->createPropertyAnimation(this,blackAdvisorOneItem,group,blackAdvisorOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackAdvisorOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackAdvisorOneItem->setLocateIJ(i-3,j-3);
                            break;
                case blackAdvisorTwo:
                            this->scene->addItem(this->blackAdvisorTwoItem);
                            this->createPropertyAnimation(this,blackAdvisorTwoItem,group,blackAdvisorTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackAdvisorTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackAdvisorTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case blackBishopOne:
                            this->scene->addItem(this->blackBishopOneItem);
                            this->createPropertyAnimation(this,blackBishopOneItem,group,blackBishopOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackBishopOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackBishopOneItem->setLocateIJ(i-3,j-3);
                            break;
                case blackBishopTwo:
                            this->scene->addItem(this->blackBishopTwoItem);
                            this->createPropertyAnimation(this,blackBishopTwoItem,group,blackBishopTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackBishopTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackBishopTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case blackKnightOne:
                            this->scene->addItem(this->blackKnightOneItem);
                            this->createPropertyAnimation(this,blackKnightOneItem,group,blackKnightOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackKnightOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackKnightOneItem->setLocateIJ(i-3,j-3);
                            break;
                case blackKnightTwo:
                            this->scene->addItem(this->blackKnightTwoItem);
                            this->createPropertyAnimation(this,blackKnightTwoItem,group,blackKnightTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackKnightTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackKnightTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case blackRookOne:
                            this->scene->addItem(this->blackRookOneItem);
                            this->createPropertyAnimation(this,blackRookOneItem,group,blackRookOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackRookOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackRookOneItem->setLocateIJ(i-3,j-3);
                            break;
                case blackRookTwo:
                            this->scene->addItem(this->blackRookTwoItem);
                            this->createPropertyAnimation(this,blackRookTwoItem,group,blackRookTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackRookTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackRookTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case blackCannonOne:
                            this->scene->addItem(this->blackCannonOneItem);
                            this->createPropertyAnimation(this,blackCannonOneItem,group,blackCannonOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackCannonOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackCannonOneItem->setLocateIJ(i-3,j-3);
                            break;
                case blackCannonTwo:
                            this->scene->addItem(this->blackCannonTwoItem);
                            this->createPropertyAnimation(this,blackCannonTwoItem,group,blackCannonTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackCannonTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackCannonTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case blackPawnOne:
                            this->scene->addItem(this->blackPawnOneItem);
                            this->createPropertyAnimation(this,blackPawnOneItem,group,blackPawnOneItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackPawnOneItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackPawnOneItem->setLocateIJ(i-3,j-3);
                            break;
                case blackPawnTwo:
                            this->scene->addItem(this->blackPawnTwoItem);
                            this->createPropertyAnimation(this,blackPawnTwoItem,group,blackPawnTwoItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackPawnTwoItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackPawnTwoItem->setLocateIJ(i-3,j-3);
                            break;
                case blackPawnThree:
                            this->scene->addItem(this->blackPawnThreeItem);
                            this->createPropertyAnimation(this,blackPawnThreeItem,group,blackPawnThreeItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackPawnThreeItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackPawnThreeItem->setLocateIJ(i-3,j-3);
                            break;
                case blackPawnFour:
                            this->scene->addItem(this->blackPawnFourItem);
                            this->createPropertyAnimation(this,blackPawnFourItem,group,blackPawnFourItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackPawnFourItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackPawnFourItem->setLocateIJ(i-3,j-3);
                            break;
                case blackPawnFive:
                            this->scene->addItem(this->blackPawnFiveItem);
                            this->createPropertyAnimation(this,blackPawnFiveItem,group,blackPawnFiveItem->pos(),this->chessBoardGrid[i-3][j-3]);
                            //this->blackPawnFiveItem->setPos(this->chessBoardGrid[i-3][j-3]);
                            this->blackPawnFiveItem->setLocateIJ(i-3,j-3);
                            break;
                default:
                            UBChessManItem *temp=new UBChessManItem(-1,this->redKingItem->getWidth(),this->redKingItem->getHeight(),"");
                            this->scene->addItem(temp);
                            temp->setPos(this->chessBoardGrid[i-3][j-3]);
                            temp->setLocateIJ(i-3,j-3);
                            break;
            }
        }
    }
    group->start();
    this->roundsNumberItem->setValue(QString().setNum(this->chess->roundsNumber));
    this->captureStepNumberItem->setValue(QString().setNum(this->chess->captureStepNumber));
    if(this->chess->isRed)
    {
        this->redCheckedItem->setChecked(true);
        this->blackCheckedItem->setChecked(false);
    }
    else
    {
        this->redCheckedItem->setChecked(false);
        this->blackCheckedItem->setChecked(true);
    }
    this->moveListViewItem->clearList();
    this->scene->clearSelection();
    this->updateItemForOpacityAndTurn();
    this->captureInLast=false;
    this->checkedInLast=false;
    this->winInLast=false;
    this->loseInLast=false;
    QSound::play(":/Sounds/NEWGAME.WAV");
}

void MainWindow::setEffectForItem(UBChessManItem *_item,UBGraphicsScene *_scene)
{
    UBGraphicsDropShadowEffect *effect=new UBGraphicsDropShadowEffect(_scene);
    effect->setOffset(3);
    _item->setGraphicsEffect(effect);
}

void MainWindow::createPropertyAnimation(QObject *parent, UBChessManItem *item, QParallelAnimationGroup *group,QPointF start,QPointF end)
{
    QPropertyAnimation *animationPos=new QPropertyAnimation(item,"pos",parent);
    animationPos->setDuration(initialChessManTime);
    animationPos->setEasingCurve(initialingChessMan);
    animationPos->setStartValue(start);
    animationPos->setEndValue(end);
    group->addAnimation(animationPos);
}
//no illegal out of chess board
//no illegal FEN string
bool MainWindow::isLegalMove(UBChessManItem *lastItem, UBChessManItem *newSelectItem)
{
    int newI=newSelectItem->getLocateI();
    int newJ=newSelectItem->getLocateJ();
    int oldI=lastItem->getLocateI();
    int oldJ=lastItem->getLocateJ();
    bool isRedOnBigEnd=(this->chess->cChessManPosition[0] > 8*16);
    int redKingMaxI,redKingMinI,redKingMaxJ,redKingMinJ;
    int blackKingMaxI,blackKingMinI,blackKingMaxJ,blackKingMinJ;
    int redBishopMaxI,redBishopMinI,redBishopMaxJ,redBishopMinJ;
    int blackBishopMaxI,blackBishopMinI,blackBishopMaxJ,blackBishopMinJ;
    if(isRedOnBigEnd)
    {
        redKingMaxI=9;redKingMinI=7;redKingMaxJ=5;redKingMinJ=3;
        blackKingMaxI=2;blackKingMinI=0;blackKingMaxJ=5;blackKingMinJ=3;
        redBishopMaxI=9;redBishopMinI=5;redBishopMaxJ=8;redBishopMinJ=0;
        blackBishopMaxI=4;blackBishopMinI=0;blackBishopMaxJ=8;blackBishopMinJ=0;
    }
    else
    {
        redKingMaxI=2;redKingMinI=0;redKingMaxJ=5;redKingMinJ=3;
        blackKingMaxI=9;blackKingMinI=7;blackKingMaxJ=5;blackKingMinJ=3;
        redBishopMaxI=4;redBishopMinI=0;redBishopMaxJ=8;redBishopMinJ=0;
        blackBishopMaxI=9;blackBishopMinI=5;blackBishopMaxJ=8;blackBishopMinJ=0;
    }
    switch(lastItem->getChessType())
    {
            case RED_KING:
                          if(newI>=redKingMinI && newI<=redKingMaxI && newJ>=redKingMinJ && newJ<=redKingMaxJ)
                          {
                            if(((oldI==newI) && (abs(oldJ-newJ)==1)) || ((abs(oldI-newI)==1) && (oldJ==newJ)))
                            {
                                if(newSelectItem->isRedChessMan() != 1)
                                    return true;
                            }
                          }
                          break;
            case RED_ADVISOR:
                          if(newI>=redKingMinI && newI<=redKingMaxI && newJ>=redKingMinJ && newJ<=redKingMaxJ)
                          {
                            if((abs(oldI-newI)==1) && (abs(oldJ-newJ)==1))
                            {
                                if(newSelectItem->isRedChessMan() != 1)
                                    return true;
                            }
                          }
                          break;
            case RED_BISHOP:
                          if(newI>=redBishopMinI && newI<=redBishopMaxI && newJ>=redBishopMinJ && newJ<=redBishopMaxJ)
                          {
                              if((abs(oldI-newI)==2) && (abs(oldJ-newJ)==2))
                              {
                                    if(newSelectItem->isRedChessMan() == 1)
                                    {
                                            return false;
                                    }
                                    QPointF bishopEyeItemPos=this->chessBoardGrid[((oldI+newI)/2)][((oldJ+newJ)/2)];
                                    QGraphicsItem *tempItem=this->scene->itemAt(bishopEyeItemPos,QTransform());
                                    UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                    if((ubTempItem->isRedChessMan() != 0) && (ubTempItem->isRedChessMan() != 1))
                                            return true;
                              }
                          }
                          break;
            case RED_KNIGHT:
                          if(newI>=0 && newI<=9 && newJ>=0 && newJ<=8)
                          {
                              if((abs(oldI-newI)==1) && (abs(oldJ-newJ)==2))
                              {
                                    if(newSelectItem->isRedChessMan() == 1)
                                        return false;
                                    QPointF knightLegItemPos=this->chessBoardGrid[oldI][((oldJ+newJ)/2)];
                                    QGraphicsItem *tempItem=this->scene->itemAt(knightLegItemPos,QTransform());
                                    UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                    if((ubTempItem->isRedChessMan() != 0) && (ubTempItem->isRedChessMan() != 1))
                                            return true;

                              }
                              if((abs(oldI-newI)==2) && (abs(oldJ-newJ)==1))
                              {
                                  if(newSelectItem->isRedChessMan() == 1)
                                      return false;
                                  QPointF knightLegItemPos=this->chessBoardGrid[((oldI+newI)/2)][oldJ];
                                  QGraphicsItem *tempItem=this->scene->itemAt(knightLegItemPos,QTransform());
                                  UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                  if((ubTempItem->isRedChessMan() != 0) && (ubTempItem->isRedChessMan() != 1))
                                          return true;
                              }
                          }
                          break;
            case RED_ROOK:
                          if(newI>=0 && newJ<=9 && newJ>=0 && newJ<=8)
                          {
                              if(newSelectItem->isRedChessMan()==1)
                                  return false;
                              if(newI==oldI && oldJ!=newJ)
                              {
                                    int offInr=((newJ>oldJ)?1:-1);
                                    for(int i=oldJ+offInr;i!=newJ;i+=offInr)
                                    {
                                        QPointF tempPos=this->chessBoardGrid[oldI][i];
                                        QGraphicsItem *tempItem=this->scene->itemAt(tempPos,QTransform());
                                        UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                        if((ubTempItem->isRedChessMan()==1) || (ubTempItem->isRedChessMan() ==0))
                                            return false;
                                    }
                                    return true;
                              }
                              if(newJ==oldJ && oldI!=newI)
                              {
                                  int offInr=((newI>oldI)?1:-1);
                                  for(int i=oldI+offInr;i!=newI;i+=offInr)
                                  {
                                      QPointF tempPos=this->chessBoardGrid[i][oldJ];
                                      QGraphicsItem *tempItem=this->scene->itemAt(tempPos,QTransform());
                                      UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                      if((ubTempItem->isRedChessMan()==1) || (ubTempItem->isRedChessMan() ==0))
                                          return false;
                                  }
                                  return true;
                              }
                          }
                          break;
            case RED_CANNON:
                          if(newI>=0 && newI<=9 && newJ>=0 && newJ<=8)
                          {
                            if(newSelectItem->isRedChessMan() == 1)
                                return false;
                            int midNumber=0;
                            if(newI==oldI && newJ!=oldJ)
                            {
                                int offInr=((newJ>oldJ)?1:-1);
                                for(int i=oldJ+offInr;i!=newJ;i+=offInr)
                                {
                                    QPointF tempPos=this->chessBoardGrid[oldI][i];
                                    QGraphicsItem *tempItem=this->scene->itemAt(tempPos,QTransform());
                                    UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                    if((ubTempItem->isRedChessMan()==1) || (ubTempItem->isRedChessMan() ==0))
                                        ++midNumber;
                                    if(midNumber >= 2)
                                        return false;
                                }
                                if((midNumber==0) && (newSelectItem->isRedChessMan() != 0))
                                    return true;
                                if((midNumber==1) && (newSelectItem->isRedChessMan() == 0))
                                    return true;
                            }
                            if(newJ==oldJ && newI!=oldI)
                            {
                                int offInr=((newI>oldI)?1:-1);
                                for(int i=oldI+offInr;i!=newI;i+=offInr)
                                {
                                    QPointF tempPos=this->chessBoardGrid[i][oldJ];
                                    QGraphicsItem *tempItem=this->scene->itemAt(tempPos,QTransform());
                                    UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                    if((ubTempItem->isRedChessMan()==1) || (ubTempItem->isRedChessMan() ==0))
                                    {
                                        ++midNumber;
                                    }
                                    if(midNumber>=2)
                                        return false;
                                }
                                if((midNumber==0) && (newSelectItem->isRedChessMan() != 0))
                                    return true;
                                if((midNumber==1) && (newSelectItem->isRedChessMan() == 0))
                                    return true;
                            }
                          }
                          break;
            case RED_PAWN:
                          if(newI>=0 && newI<=9 && newJ>=0 && newJ<=8)
                          {
                              if(newSelectItem->isRedChessMan() == 1)
                                  return false;
                              bool isCrossRiver=false;
                              if((isRedOnBigEnd && newI<=4) || ((!isRedOnBigEnd) && newI>=5))
                                  isCrossRiver=true;
                              int factor=(isRedOnBigEnd?1:-1);
                              if((!isCrossRiver) && (oldJ==newJ) && ((newI-oldI)==-1*factor))
                                  return true;
                              if(isCrossRiver)
                              {
                                  if((oldI==newI) && (abs(newJ-oldJ)==1))
                                      return true;
                                  if((oldJ==newJ) && ((newI-oldI)==-1*factor))
                                      return true;
                              }
                          }
                          break;
            case BLACK_KING:
                          if(newI<=blackKingMaxI && newI>=blackKingMinI && newJ>=blackKingMinJ && newJ<=blackKingMaxJ)
                          {
                              if(((oldI==newI) && (abs(oldJ-newJ)==1)) || ((abs(oldI-newI)==1) && (oldJ==newJ)))
                              {
                                  if(newSelectItem->isRedChessMan() != 0)
                                      return true;
                              }
                          }
                          break;
            case BLACK_ADVISOR:
                          if(newI<=blackKingMaxI && newI>=blackKingMinI && newJ>=blackKingMinJ && newJ<=blackKingMaxJ)
                          {
                              if((abs(oldI-newI)==1) && (abs(oldJ-newJ)==1))
                              {
                                  if(newSelectItem->isRedChessMan() != 0)
                                      return true;
                              }
                          }
                          break;
            case BLACK_BISHOP:
                          if(newI<=blackBishopMaxI && newI>=blackBishopMinI && newJ<=blackBishopMaxJ && newJ>=blackBishopMinJ)
                          {
                              if((abs(oldI-newI)==2) && (abs(oldJ-newJ)==2))
                              {
                                    if(newSelectItem->isRedChessMan() == 0)
                                    {
                                            return false;
                                    }
                                    QPointF bishopEyeItemPos=this->chessBoardGrid[((oldI+newI)/2)][((oldJ+newJ)/2)];
                                    QGraphicsItem *tempItem=this->scene->itemAt(bishopEyeItemPos,QTransform());
                                    UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                    if((ubTempItem->isRedChessMan() != 0) && (ubTempItem->isRedChessMan() != 1))
                                            return true;
                              }
                          }
                          break;
            case BLACK_KNIGHT:
                        if(newI<=9 && newI>=0 && newJ>=0 && newJ<=8)
                        {
                            if((abs(oldI-newI)==1) && (abs(oldJ-newJ)==2))
                            {
                                  if(newSelectItem->isRedChessMan() == 0)
                                      return false;
                                  QPointF knightLegItemPos=this->chessBoardGrid[oldI][((oldJ+newJ)/2)];
                                  QGraphicsItem *tempItem=this->scene->itemAt(knightLegItemPos,QTransform());
                                  UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                  if((ubTempItem->isRedChessMan() != 0) && (ubTempItem->isRedChessMan() != 1))
                                          return true;

                            }
                            if((abs(oldI-newI)==2) && (abs(oldJ-newJ)==1))
                            {
                                if(newSelectItem->isRedChessMan() == 0)
                                    return false;
                                QPointF knightLegItemPos=this->chessBoardGrid[((oldI+newI)/2)][oldJ];
                                QGraphicsItem *tempItem=this->scene->itemAt(knightLegItemPos,QTransform());
                                UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                if((ubTempItem->isRedChessMan() != 0) && (ubTempItem->isRedChessMan() != 1))
                                        return true;
                            }
                        }
                        break;
            case BLACK_ROOK:
                        if(newI<=9 && newI>=0 && newJ>=0 && newJ<=8)
                        {
                            if(newSelectItem->isRedChessMan()==0)
                                return false;
                            if(newI==oldI && oldJ!=newJ)
                            {
                                  int offInr=((newJ>oldJ)?1:-1);
                                  for(int i=oldJ+offInr;i!=newJ;i+=offInr)
                                  {
                                      QPointF tempPos=this->chessBoardGrid[oldI][i];
                                      QGraphicsItem *tempItem=this->scene->itemAt(tempPos,QTransform());
                                      UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                      if((ubTempItem->isRedChessMan()==1) || (ubTempItem->isRedChessMan() ==0))
                                          return false;
                                  }
                                  return true;
                            }
                            if(newJ==oldJ && oldI!=newI)
                            {
                                int offInr=((newI>oldI)?1:-1);
                                for(int i=oldI+offInr;i!=newI;i+=offInr)
                                {
                                    QPointF tempPos=this->chessBoardGrid[i][oldJ];
                                    QGraphicsItem *tempItem=this->scene->itemAt(tempPos,QTransform());
                                    UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                    if((ubTempItem->isRedChessMan()==1) || (ubTempItem->isRedChessMan() ==0))
                                        return false;
                                }
                                return true;
                            }
                        }
                        break;
            case BLACK_CANNON:
                        if(newI<=9 && newI>=0 && newJ>=0 && newJ<=8)
                        {
                            if(newSelectItem->isRedChessMan() == 0)
                                return false;
                            int midNumber=0;
                            if(newI==oldI && newJ!=oldJ)
                            {
                                int offInr=((newJ>oldJ)?1:-1);
                                for(int i=oldJ+offInr;i!=newJ;i+=offInr)
                                {
                                    QPointF tempPos=this->chessBoardGrid[oldI][i];
                                    QGraphicsItem *tempItem=this->scene->itemAt(tempPos,QTransform());
                                    UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                    if((ubTempItem->isRedChessMan()==1) || (ubTempItem->isRedChessMan() ==0))
                                        ++midNumber;
                                    if(midNumber >= 2)
                                        return false;
                                }
                                if((midNumber==0) && (newSelectItem->isRedChessMan() != 1))
                                    return true;
                                if((midNumber==1) && (newSelectItem->isRedChessMan() == 1))
                                    return true;
                            }
                            if(newJ==oldJ && newI!=oldI)
                            {
                                int offInr=((newI>oldI)?1:-1);
                                for(int i=oldI+offInr;i!=newI;i+=offInr)
                                {
                                    QPointF tempPos=this->chessBoardGrid[i][oldJ];
                                    QGraphicsItem *tempItem=this->scene->itemAt(tempPos,QTransform());
                                    UBChessManItem *ubTempItem=dynamic_cast<UBChessManItem *>(tempItem);
                                    if((ubTempItem->isRedChessMan()==1) || (ubTempItem->isRedChessMan() ==0))
                                    {
                                        ++midNumber;
                                    }
                                    if(midNumber>=2)
                                        return false;
                                }
                                if((midNumber==0) && (newSelectItem->isRedChessMan() != 1))
                                    return true;
                                if((midNumber==1) && (newSelectItem->isRedChessMan() == 1))
                                    return true;
                            }
                        }
                        break;
            case BLACK_PAWN:
                        if(newI<=9 && newI>=0 && newJ>=0 && newJ<=8)
                        {
                            if(newSelectItem->isRedChessMan() == 0)
                                return false;
                            bool isCrossRiver=false;
                            if((isRedOnBigEnd && newI>=5) || ((!isRedOnBigEnd) && newI<=4))
                                isCrossRiver=true;
                            int factor=(isRedOnBigEnd?1:-1);
                            if((!isCrossRiver) && (oldJ==newJ) && ((newI-oldI)==1*factor))
                                return true;
                            if(isCrossRiver)
                            {
                                if((oldI==newI) && (abs(newJ-oldJ)==1))
                                    return true;
                                if((oldJ==newJ) && ((newI-oldI)==1*factor))
                                    return true;
                            }
                        }
                        break;
            default:
                        break;
    }
    return false;
}

void MainWindow::initNamedPipe()
{
    if(::access(FIFONAME,F_OK) == -1)
    {
        ::mkfifo(FIFONAME,0777);
    }
}

void MainWindow::destoryNamedPipe()
{
    ::unlink(FIFONAME);
}

void MainWindow::initUCCIEngine()
{
    //1---start UCCIEngine process
    char buffer[BUFFERSIZE];
    memset(buffer,0,BUFFERSIZE);
    fifoHandle=::open(FIFONAME,O_WRONLY);
    UCCI_CMD_STRUCT ucci_cmd;
    UBString ucciStr=ucci_cmd.toString();
    ::write(fifoHandle,ucciStr.getData(),ucciStr.length());
    ::close(fifoHandle);
    cout<<"sending "<<ucciStr<<endl;
    fifoHandle=::open(FIFONAME,O_RDONLY);
    ::read(fifoHandle,buffer,BUFFERSIZE);
    ::close(fifoHandle);
    cout<<"RCV:"<<buffer<<endl;

    lastPositionCMD.fenCMDName="startpos";
    lastPositionCMD.fenString="";
    lastPositionCMD.movesCMDName="";
    lastPositionCMD.movesForFenLen=0;
    ucciStr=lastPositionCMD.toString();
    fifoHandle=::open(FIFONAME,O_WRONLY);
    ::write(fifoHandle,ucciStr.getData(),ucciStr.length());
    ::close(fifoHandle);
    cout<<"sending "<<ucciStr<<endl;
    memset(buffer,0,BUFFERSIZE);
    fifoHandle=::open(FIFONAME,O_RDONLY);
    ::read(fifoHandle,buffer,BUFFERSIZE);
    ::close(fifoHandle);
    cout<<"RCV:"<<buffer<<endl;
}

void MainWindow::quitUCCIEngine()
{
    char buffer[BUFFERSIZE];
    memset(buffer,0,BUFFERSIZE);
    fifoHandle=::open(FIFONAME,O_WRONLY);
    QUIT_CMD_STRUCT quit_cmd;
    UBString ucciStr=quit_cmd.toString();
    ::write(fifoHandle,ucciStr.getData(),ucciStr.length());
    ::close(fifoHandle);
    cout<<"sending "<<ucciStr<<endl;
    fifoHandle=::open(FIFONAME,O_RDONLY);
    ::read(fifoHandle,buffer,BUFFERSIZE);
    ::close(fifoHandle);
    cout<<"RCV:"<<buffer<<endl;
}

void MainWindow::newGameSlot()
{
    CChessStruct *temp=new CChessStruct;
    this->chess=makeSmartPtr(temp);
    this->moveMaker.bindStruct(this->chess);
    this->initializeUIFromStruct();

    char buffer[BUFFERSIZE];
    memset(buffer,0,BUFFERSIZE);
    lastPositionCMD.fenCMDName="startpos";
    lastPositionCMD.fenString="";
    lastPositionCMD.movesCMDName="";
    lastPositionCMD.movesForFenLen=0;
    UBString ucciStr=lastPositionCMD.toString();
    fifoHandle=::open(FIFONAME,O_WRONLY);
    ::write(fifoHandle,ucciStr.getData(),ucciStr.length());
    ::close(fifoHandle);
    cout<<"sending "<<ucciStr<<endl;
    fifoHandle=::open(FIFONAME,O_RDONLY);
    ::read(fifoHandle,buffer,BUFFERSIZE);
    ::close(fifoHandle);
    cout<<"RCV:"<<buffer<<endl;
}

void MainWindow::locateItem(UBChessManItem *lastItem,UBChessManItem *newSelectItem)
{
    int newI=newSelectItem->getLocateI();
    int newJ=newSelectItem->getLocateJ();
    int oldI=lastItem->getLocateI();
    int oldJ=lastItem->getLocateJ();
    if(!isLegalMove(lastItem,newSelectItem))
    {
        updateItemForOpacityAndTurn();
        QSound::play(":/Sounds/ILLEGAL.WAV");
        return;
    }
    newSelectItem->setSelected(true);
    QParallelAnimationGroup *group=new QParallelAnimationGroup(this);
    QPropertyAnimation *animation=new QPropertyAnimation(lastItem,"pos",this);
    animation->setDuration(movingChessManTime);
    animation->setEasingCurve(movingChessMan);
    animation->setStartValue(lastItem->pos());
    animation->setEndValue(this->chessBoardGrid[newI][newJ]);
    group->addAnimation(animation);
    QPropertyAnimation *animation2=new QPropertyAnimation(newSelectItem,"opacity",this);
    animation2->setDuration(capturingChessManTime);
    animation2->setEasingCurve(capturingChessMan);
    animation2->setStartValue(1);
    animation2->setEndValue(0);
    group->addAnimation(animation2);
    group->start();
    lastItem->setLocateIJ(newI,newJ);
    Move _move;_move.to=(newI+3)*16+(newJ+3);
               _move.from=(oldI+3)*16+(oldJ+3);
               _move.capture=this->chess->cChessArray[_move.to];
    cout<<_move.from<<" "<<_move.to<<" "<<_move.capture<<endl;
    ChinessMove _chinessMove=moveMaker.translateToChinessZX(this->chess,_move);
    moveMaker.makeMove(_move);
    UBString moveStr=(_chinessMove.chessMan+_chinessMove.from+_chinessMove.act+_chinessMove.to);
    int spaceNumber=UBString::toString(this->chess->roundsNumber).length();
    UBString spaceStr;
    for(int i=0;i<3-spaceNumber;i++)
        spaceStr+="  ";
    moveStr=(UBString::toString(this->chess->roundsNumber) +spaceStr+ "."+ moveStr);
    this->moveListViewItem->addValue(QString(moveStr.getData()));
    this->roundsNumberItem->setValue(QString().setNum(this->chess->roundsNumber));
    this->captureStepNumberItem->setValue(QString().setNum(this->chess->captureStepNumber));
    if(this->chess->isRed)
    {
        this->redCheckedItem->setChecked(true);
        this->blackCheckedItem->setChecked(false);
    }
    else
    {
        this->redCheckedItem->setChecked(false);
        this->blackCheckedItem->setChecked(true);
    }
    if(_move.capture!=0)
    {
        this->winInLast=false;
        this->loseInLast=false;
        this->checkedInLast=false;
        this->captureInLast=true;
    }
    else
    {
        this->winInLast=false;
        this->loseInLast=false;
        this->checkedInLast=false;
        this->captureInLast=false;
    }
    connect(group,SIGNAL(finished()),this,SLOT(updateItemForOpacityAndTurn()));
    connect(group,SIGNAL(finished()),this,SLOT(playSound()));
    //这个slot的执行一定是该函数尸体内容执行万之后，所以go一定在position之后
    connect(group,SIGNAL(finished()),this,SLOT(ucciCMDFunc()));

    UBChessManItem *tempItem=new UBChessManItem(-1,this->redKingItem->getWidth(),this->redKingItem->getHeight(),"");
    this->scene->addItem(tempItem);
    tempItem->setPos(this->chessBoardGrid[oldI][oldJ]);
    tempItem->setLocateIJ(oldI,oldJ);

    if(chess->isRed)
    {
        if(_move.capture!=0)
        {
            lastPositionCMD.fenCMDName="fen";
            lastPositionCMD.fenString=UBString(this->chess->toFENStr().getPtr());
            lastPositionCMD.movesCMDName="";
            lastPositionCMD.movesForFenLen=0;
        }
    }
    else
    {
        //红方走后，需要向引擎传递人的走法，构造position命令
        ICCSMove _redICCSMove=MoveMaker::translateToICCS(_move);
        lastPositionCMD.movesCMDName="moves";
        lastPositionCMD.movesForFen[lastPositionCMD.movesForFenLen]="";
        lastPositionCMD.movesForFen[lastPositionCMD.movesForFenLen]+=_redICCSMove.fromX;
        lastPositionCMD.movesForFen[lastPositionCMD.movesForFenLen]+=_redICCSMove.fromY;
        lastPositionCMD.movesForFen[lastPositionCMD.movesForFenLen]+=_redICCSMove.toX;
        lastPositionCMD.movesForFen[lastPositionCMD.movesForFenLen]+=_redICCSMove.toY;
        ++lastPositionCMD.movesForFenLen;
        UBString ucciStr=lastPositionCMD.toString();
        fifoHandle=::open(FIFONAME,O_WRONLY);
        ::write(fifoHandle,ucciStr.getData(),ucciStr.length());
        ::close(fifoHandle);
        cout<<"sending "<<ucciStr<<endl;
        char buffer[BUFFERSIZE];
        memset(buffer,0,BUFFERSIZE);
        fifoHandle=::open(FIFONAME,O_RDONLY);
        ::read(fifoHandle,buffer,BUFFERSIZE);
        ::close(fifoHandle);
        cout<<"RCV:"<<buffer<<endl;

        //考虑吃子的情况
        if(_move.capture!=0)
        {
            lastPositionCMD.fenCMDName="fen";
            lastPositionCMD.fenString=UBString(this->chess->toFENStr().getPtr());
            lastPositionCMD.movesCMDName="";
            lastPositionCMD.movesForFenLen=0;
        }

    }
}

void MainWindow::updateItemForOpacityAndTurn()
{
    QList<QGraphicsItem *> itemList=this->scene->items();
    QListIterator<QGraphicsItem *> iterator(itemList);
    while(iterator.hasNext())
    {
        UBChessManItem *tempItem=dynamic_cast<UBChessManItem *>(iterator.next());
        if(tempItem && (tempItem->opacity()==0))
        {
            if((tempItem->isRedChessMan() ==0) || (tempItem->isRedChessMan()==1))
            {
                tempItem->setOpacity(0.95);
                this->scene->removeItem(tempItem);
            }
        }
        if(tempItem)
        {
            if(this->chess->isRed)
            {
                if(tempItem->isRedChessMan()==1)
                    tempItem->setFlag(QGraphicsItem::ItemIsSelectable,true);
                else if(tempItem->isRedChessMan()==0)
                    tempItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
            }else
            {
                if(tempItem->isRedChessMan()==1)
                    tempItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
                else if(tempItem->isRedChessMan()==0)
                    tempItem->setFlag(QGraphicsItem::ItemIsSelectable,true);
            }
        }
    }
}

void MainWindow::updateItemForOppositeTure()
{
    QList<QGraphicsItem *> itemList=this->scene->items();
    QListIterator<QGraphicsItem *> iterator(itemList);
    while(iterator.hasNext())
    {
        UBChessManItem *tempItem=dynamic_cast<UBChessManItem *>(iterator.next());
        if(tempItem)
            tempItem->setFlag(QGraphicsItem::ItemIsSelectable,true);
    }
}

void MainWindow::playSound()
{
    if(winInLast)
        QSound::play(":/Sounds/WIN.WAV");
    else if(loseInLast)
        QSound::play(":/Sounds/LOSE.WAV");
    else if(captureInLast)
        QSound::play(":/Sounds/CAPTURE.WAV");
    else if(checkedInLast)
        QSound::play(":/Sounds/CHECK.WAV");
    else
        QSound::play(":/Sounds/MOVE.WAV");
}

void MainWindow::ucciCMDFunc()
{
    //传递之后需要引擎思考，然后接受引擎走法，同时修改position命令
    if(chess->isRed)
        return;
    UBString ucciStr="go";
    fifoHandle=::open(FIFONAME,O_WRONLY);
    ::write(fifoHandle,ucciStr.getData(),ucciStr.length());
    ::close(fifoHandle);
    cout<<"sending "<<ucciStr<<endl;
    char buffer[BUFFERSIZE];
    memset(buffer,0,BUFFERSIZE);
    fifoHandle=::open(FIFONAME,O_RDONLY);
    ::read(fifoHandle,buffer,BUFFERSIZE);
    ::close(fifoHandle);
    cout<<"RCV:"<<buffer<<endl;
    BESTMOVE_REPLY_STRUCT bestmove_reply;
    bestmove_reply.setMember(UBString(buffer));
    ICCSMove _bestICCS;_bestICCS.fromX=bestmove_reply.bestMove.at(0);
                        _bestICCS.fromY=bestmove_reply.bestMove.at(1);
                        _bestICCS.toX=bestmove_reply.bestMove.at(2);
                        _bestICCS.toY=bestmove_reply.bestMove.at(3);
    //set lastPosition cmd
    lastPositionCMD.movesForFen[lastPositionCMD.movesForFenLen]=bestmove_reply.bestMove;
    ++lastPositionCMD.movesForFenLen;
    Move _bestMove=MoveMaker::translateToMove(this->chess,_bestICCS);

    int lastItem_I=(9-(_bestICCS.fromY-'0'));
    int lastItem_J=(_bestICCS.fromX-'a');
    int newItem_I=(9-(_bestICCS.toY-'0'));
    int newItem_J=(_bestICCS.toX-'a');
    UBChessManItem *lastItemPtr=dynamic_cast<UBChessManItem *>(this->scene->itemAt(chessBoardGrid[lastItem_I][lastItem_J],QTransform()));
    UBChessManItem *NewItemPtr=dynamic_cast<UBChessManItem *>(this->scene->itemAt(chessBoardGrid[newItem_I][newItem_J],QTransform()));
    locateItem(lastItemPtr,NewItemPtr);
}
