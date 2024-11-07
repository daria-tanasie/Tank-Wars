
#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Tanks::Tanks()
{
}

Tanks::~Tanks()
{
}

void Tanks::getTankPos(int pos)
{
    tank1.x = points[pos].x;
    tank1.y = points[pos].y + ((tank1.x - points[pos].x)
        / (points[pos + 1].x - points[pos].x)) * ((points[pos + 1].y - points[pos].y));

    segmentPointsDist.x = points[pos + 1].x - points[pos].x;
    segmentPointsDist.y = points[pos + 1].y - points[pos].y;

    angularStep = atan2(segmentPointsDist.y, segmentPointsDist.x);
}