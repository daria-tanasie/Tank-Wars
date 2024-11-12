#include "lab_m1/Tema1/Tank.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/Tema1.h"

using namespace std;
using namespace m1;

void Tank::GetTankPos(std::vector<glm::vec2> points)
{
    glm::vec2 segmentPointsDist;
    x = points[posTank].x;
    y = points[posTank].y + ((x - points[posTank].x) / (points[posTank + 1].x - points[posTank].x))
                * ((points[posTank + 1].y - points[posTank].y));

    segmentPointsDist.x = points[posTank + 1].x - points[posTank].x;
    segmentPointsDist.y = points[posTank + 1].y - points[posTank].y;

    angularStep = atan2(segmentPointsDist.y, segmentPointsDist.x);
}