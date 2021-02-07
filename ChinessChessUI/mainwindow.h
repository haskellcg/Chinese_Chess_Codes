#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ubgraphicsview.h"
#include "ubgraphicsscene.h"
#include "ubchessmanitem.h"
#include "ubtextvalueitem.h"
#include "ubtextcheckitem.h"
#include "ubtextlistitem.h"
#include "CChessStruct.h"
#include "MoveMaker.h"
#include "UBUCCI.h"
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include "ubgraphicseffect.h"
#include <QTransform>
#include <QMainWindow>
#include <QPointF>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setChessStruct(const char *FENStr);
private:
    void buildChessMan();
    void buildOtherWidget();
    void initializeGrid();
    void initializeUIFromStruct();
    void setEffectForItem(UBChessManItem *_item,UBGraphicsScene *_scene);
    void createPropertyAnimation(QObject *parent,UBChessManItem *item,QParallelAnimationGroup *group,QPointF start,QPointF end);
    bool isLegalMove(UBChessManItem *lastItem,UBChessManItem *newSelectItem);
    void initNamedPipe();
    void destoryNamedPipe();
    void initUCCIEngine();
    void quitUCCIEngine();
private slots:
    void newGameSlot();
    void locateItem(UBChessManItem *lastItem,UBChessManItem *newSelectItem);
    void updateItemForOpacityAndTurn();
    void updateItemForOppositeTure();
    void playSound();
    void ucciCMDFunc();
private:
    UBGraphicsView *view;
    UBGraphicsScene *scene;
    UBChessManItem *redKingItem;
    UBChessManItem *redAdvisorOneItem;
    UBChessManItem *redAdvisorTwoItem;
    UBChessManItem *redBishopOneItem;
    UBChessManItem *redBishopTwoItem;
    UBChessManItem *redKnightOneItem;
    UBChessManItem *redKnightTwoItem;
    UBChessManItem *redRookOneItem;
    UBChessManItem *redRookTwoItem;
    UBChessManItem *redCannonOneItem;
    UBChessManItem *redCannonTwoItem;
    UBChessManItem *redPawnOneItem;
    UBChessManItem *redPawnTwoItem;
    UBChessManItem *redPawnThreeItem;
    UBChessManItem *redPawnFourItem;
    UBChessManItem *redPawnFiveItem;
    UBChessManItem *blackKingItem;
    UBChessManItem *blackAdvisorOneItem;
    UBChessManItem *blackAdvisorTwoItem;
    UBChessManItem *blackBishopOneItem;
    UBChessManItem *blackBishopTwoItem;
    UBChessManItem *blackKnightOneItem;
    UBChessManItem *blackKnightTwoItem;
    UBChessManItem *blackRookOneItem;
    UBChessManItem *blackRookTwoItem;
    UBChessManItem *blackCannonOneItem;
    UBChessManItem *blackCannonTwoItem;
    UBChessManItem *blackPawnOneItem;
    UBChessManItem *blackPawnTwoItem;
    UBChessManItem *blackPawnThreeItem;
    UBChessManItem *blackPawnFourItem;
    UBChessManItem *blackPawnFiveItem;
    UBTextValueItem *roundsNumberItem;
    UBTextValueItem *captureStepNumberItem;
    UBTextCheckItem *redCheckedItem;
    UBTextCheckItem *blackCheckedItem;
    UBTextValueItem *modeSelectedItem;
    UBTextListItem  *moveListViewItem;
    UBTextValueItem *infoItem;
    QPointF chessBoardGrid[10][9];
    SmartPtr<CChessStruct> chess;
    MoveMaker moveMaker;
    int fifoHandle;
    POSITION_CMD_STRUCT lastPositionCMD;

    bool captureInLast;
    bool checkedInLast;
    bool winInLast;
    bool loseInLast;
};

#endif // MAINWINDOW_H
