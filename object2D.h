#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTrapeze(const std::string& name, glm::vec3 leftBottomCorner, float bottomLength, float sideLength, glm::vec3 color, bool fill);
    Mesh* CreateCircle(const std::string& name, glm::vec3 center, int nrSegments, float radius, float fullCircle, glm::vec3 color, bool fill);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 center, float littleSide, float bigSide, glm::vec3 color, bool fill);
}
