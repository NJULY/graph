#include "graphUnit/graphUnit.h"

graphUnit::graphUnit()
{
    beChosen = false;
    shape = unKnown;
    id = -1;
}

Shape graphUnit::getShape()
{
    return shape;
}
