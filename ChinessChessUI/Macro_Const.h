#ifndef MACRO_CONST_H
#define MACRO_CONST_H
#include <QColor>
#include <QFont>
#include <QEasingCurve>

#define UB_WINDOW_WIDTH 800
#define UB_WINDOW_HEIGHT 500
#define UB_WINDOW_WIDTH_OFF 10
#define UB_WINDOW_HEIGHT_OFF 10
#define UB_CHESSBOARD_MARGIN 5
#define UB_CHESSMAN_ADJUST 2
const QColor sceneBGColor=Qt::darkMagenta;
const QColor chessBoardBackColor=Qt::black;
const QColor chessBoardOutRectColor=Qt::white;
const QColor chessBoardInnerRectColor=Qt::gray;
const QColor chessBoardGridColor=Qt::gray;
const QColor chessBoardPawnBaseColor=Qt::green;
const QColor chessBoardRiverTextColor=Qt::gray;
const QFont   chessBoardRiverTextFont("Monospace",20,20);
const QFont   chessManTextFont("Monospace",20,20);
const QFont   chessWidgetTextFont("Monospace",18,18);
const QColor chessWidgetBGColor=Qt::black;
const QColor chessWidgetTextColor=Qt::green;
const int listVisibleNumber=4;
const QColor infoDialogBGColor=Qt::darkGray;
const QColor infoDialogFGColor=Qt::red;
const int infoDialogTime=500;
const QEasingCurve infoDialogEasingCurve=QEasingCurve::OutInSine;
const QColor redChessManBGColor=Qt::gray;
const QColor blackChessManBGColor=Qt::darkGreen;
const QColor redChessManFGColor=Qt::red;
const QColor blackChessManFGColor=Qt::white;
const QColor opacityChessManFGColor=Qt::darkYellow;
const int initialChessManTime=2000;
const int   movingChessManTime=600;
const int capturingChessManTime=600;
const QEasingCurve movingChessMan=QEasingCurve::InExpo;
const QEasingCurve initialingChessMan=QEasingCurve::InOutCirc;
const QEasingCurve capturingChessMan=QEasingCurve::TCBSpline;
enum ChessManType
{
    RED_KING=16,RED_ADVISOR,RED_BISHOP,RED_KNIGHT,RED_ROOK,RED_CANNON,RED_PAWN,
    BLACK_KING,BLACK_ADVISOR,BLACK_BISHOP,BLACK_KNIGHT,BLACK_ROOK,BLACK_CANNON,BLACK_PAWN,
    NONE_TYPE
};
#define FIFONAME "/tmp/UCCIEngineFIFO"
#define BUFFERSIZE 200
#endif // MACRO_CONST_H
