#include "ubgraphicseffect.h"

UBGraphicsDropShadowEffect::UBGraphicsDropShadowEffect(UBGraphicsScene *_scene):scene(_scene)
{
}


void UBGraphicsDropShadowEffect::draw(QPainter *painter)
{
        QGraphicsDropShadowEffect::draw(painter);
        this->scene->update();
}
