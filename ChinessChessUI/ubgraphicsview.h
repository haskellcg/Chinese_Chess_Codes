#ifndef UBGRAPHICSVIEW_H
#define UBGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QPoint>
#include <QObject>

class UBGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    UBGraphicsView(QWidget *parent=0);
signals:
    void closeView();
    void newGameSig();
private slots:

private:
    void createAction();
    void contextMenuEvent(QContextMenuEvent *event);
private:
    QMenu *mainMenu;
    QAction *newGame;
    QAction *exitAction;
};

#endif // UBGRAPHICSVIEW_H
