#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include "Macro_Const.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //set MainWindow geometry as central screen
    QDesktopWidget *screenWidget=QApplication::desktop();
    int screenWidth=screenWidget->width();
    int screenHeight=screenWidget->height();
    w.setGeometry(screenWidth/2-UB_WINDOW_WIDTH/2,screenHeight/2-UB_WINDOW_HEIGHT/2,
                  UB_WINDOW_WIDTH+UB_WINDOW_WIDTH_OFF,UB_WINDOW_HEIGHT+UB_WINDOW_HEIGHT_OFF);
    w.show();
    return a.exec();
}
