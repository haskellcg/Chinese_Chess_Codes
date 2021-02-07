#ifndef UBGRAPHICSEFFECT_H
#define UBGRAPHICSEFFECT_H
#include <QGraphicsDropShadowEffect>
#include "ubgraphicsscene.h"

class UBGraphicsDropShadowEffect:public QGraphicsDropShadowEffect
{
public:
    UBGraphicsDropShadowEffect(UBGraphicsScene *_scene);
protected:
    void draw(QPainter *painter);
private:
    UBGraphicsScene *scene;
};

#endif // UBGRAPHICSEFFECT_H
