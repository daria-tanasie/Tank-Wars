#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, -length, 0), color),
        VertexFormat(corner + glm::vec3(0, -length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 center,
    float littleSide,
    float bigSide,
    glm::vec3 color,
    bool fill)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center - glm::vec3(littleSide / 2, 0, 0), color),
        VertexFormat(center + glm::vec3(littleSide / 2, 0, 0), color),
        VertexFormat(center + glm::vec3(littleSide / 2, bigSide, 0), color),
        VertexFormat(center + glm::vec3(-littleSide / 2, bigSide, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTrapeze(
    const std::string& name,
    glm::vec3 bottomMiddle,
    float bottomLength,
    float sideLength,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 middle = bottomMiddle;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(middle - glm::vec3(bottomLength / 2, 0, 0), color),
        VertexFormat(middle + glm::vec3(bottomLength / 2, 0, 0), color),
        VertexFormat(middle + glm::vec3(bottomLength / 2 + 20, sideLength, 0), color),
        VertexFormat(middle + glm::vec3(-(bottomLength / 2 + 20), sideLength, 0), color)
    };

    Mesh* trapeze = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        trapeze->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    trapeze->InitFromData(vertices, indices);
    return trapeze;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    int nrSegments,
    float radius,
    float fullCircle,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(center, color));

    for (int i = 1; i <= nrSegments; i++) {
        float theta = fullCircle * 3.14f * float(i) / float(nrSegments);
        glm::vec3 newPoint = center + glm::vec3(cos(theta) * radius, sin(theta) * radius, 0);
        vertices.push_back(VertexFormat(newPoint, color));
        indices.push_back(i);
    }

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}