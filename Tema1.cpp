#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    BuildMap();

    glm::vec3 origin = glm::vec3(0, 0, 0);
    glm::vec3 middleTrapeze = glm::vec3(0, 0,0);
    float squareSide = 1;
    float trapezeBottom = 90;
    float trapezeSide = 24;
    float pipeLittleSide = 7;
    float pipeBigSide = 50;
    fire = 0;
    projectilePos = glm::vec2(0, 0);
    trajectoryPos = glm::vec2(0, 0);

    translateX = 0;
    translateY = 0;
    posTank1 = 100;

    //// Initialize sx and sy (the scale factors)
    //scaleX = 1;
    //scaleY = 1;

    angularStep = 0;
    angularStepPipe = 0;

    Mesh* square1 = object2D::CreateSquare("square1", origin, squareSide, glm::vec3(0.74, 0.73, 0.22), true);
    AddMeshToList(square1);

    Mesh* upperTrapeze = object2D::CreateTrapeze("upperTrapeze", middleTrapeze, trapezeBottom, trapezeSide, glm::vec3(0.65, 0.64, 0.48), true);
    AddMeshToList(upperTrapeze);

    trapezeBottom = 60;
    trapezeSide = 16;
    middleTrapeze = glm::vec3(0, 0, 0);

    Mesh* bottomTrapeze = object2D::CreateTrapeze("bottomTrapeze", middleTrapeze, trapezeBottom, trapezeSide, glm::vec3(0.49, 0.48, 0.36), true);
    AddMeshToList(bottomTrapeze);

    Mesh* circleTank1 = object2D::CreateCircle("circleTank1", origin, 60, 30, 1.0f, glm::vec3(0.65, 0.64, 0.48), true);
    AddMeshToList(circleTank1);

    Mesh* pipeTank1 = object2D::CreateRectangle("pipeTank1", origin, pipeLittleSide, pipeBigSide, glm::vec3(0.27, 0.27, 0.04), true);
    AddMeshToList(pipeTank1);

    Mesh* projectileTank1 = object2D::CreateCircle("projectileTank1", origin, 40, 5, 2.0f, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectileTank1);

    Mesh* trajectory = object2D::CreateCircle("trajectory", origin, 40, 2, 2.0f, glm::vec3(1, 1, 1), true);
    AddMeshToList(trajectory);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.2, 0.6, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    BuildTerrain();
    GetTankPos(posTank1);
    MoveTank();
    
    trajectoryPos = glm::vec2(tankMatrix[2][0], tankMatrix[2][1]);
    glm::vec2 v = glm::vec2(cos(angularStepPipe + angularStep + RADIANS(90)), sin(angularStepPipe + angularStep + RADIANS(90))) * 100.0f;
    
    float deltaTime = 0.1f;
    glm::vec2 position = trajectoryPos;
    float gravity = 9.8f;

    for (int i = 0; i < 300; i++) {
        float time = i * deltaTime;

        position.x = trajectoryPos.x + v.x * time;
        position.y = trajectoryPos.y + v.y * time - 0.5f * gravity * time * time;
        trajectoryMatrix = glm::mat3(1);
        trajectoryMatrix *= transform2D::Translate(position.x, position.y);
        RenderMesh2D(meshes["trajectory"], shaders["VertexColor"], trajectoryMatrix);
    }

    if (fire == 1) {
        projectilePos = glm::vec2(tankMatrix[2][0], tankMatrix[2][1]);
        vProj = glm::vec2(cos(angularStepPipe + angularStep + RADIANS(90)), sin(angularStepPipe + angularStep + RADIANS(90))) * 100.0f;
        fire++;
    }

    if (fire > 1) {
        projectilePos += vProj * deltaTime;
        vProj.y -= gravity * deltaTime;
        projectileMatrix = glm::mat3(1);
        projectileMatrix *= transform2D::Translate(projectilePos.x, projectilePos.y);

        RenderMesh2D(meshes["projectileTank1"], shaders["VertexColor"], projectileMatrix);   
    }
    //fire = 0;
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_D))
    {
        if (posTank1 + 1 < points.size()) {
            posTank1 += 1;
        }
    }

    if (window->KeyHold(GLFW_KEY_A))
    {   if (posTank1 - 1 > 0)
            posTank1 -= 1;
    }

    if (window->KeyHold(GLFW_KEY_W))
    {
        if (angularStepPipe - RADIANS(60) * deltaTime > -RADIANS(90)) {
            angularStepPipe -= RADIANS(60) * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        if (angularStepPipe + RADIANS(60) * deltaTime <  RADIANS(90)) {
            angularStepPipe += RADIANS(60) * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_SPACE)) {
        fire++;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

void Tema1::BuildMap()
{
    glm::ivec2 resolution = window->GetResolution();
    heightMap.resize(resolution.x);
    points.resize(resolution.x);
    double cnt = 0.01;

    for (int i = 0; i < resolution.x; i++)
    {
        heightMap[i] = (0.2 * sin(cnt * i) + 1.5 * sin(0.5 * cnt * i) + 0.5 * sin(3 * cnt * i)) * 200;  // 200 pt cat de mari sunt damburile
    }
}

void Tema1::BuildTerrain()
{
    double maxY;
    double scaling = 10; // pt cat de intinse sunt parabolele

    for (double i = 0; i < heightMap.size() - 1; i++) {

        if (heightMap[i] < heightMap[i + 1]) {
            maxY = heightMap[i + 1];
        }
        else {
            maxY = heightMap[i];
        }

        factor = (heightMap[i + 1] - heightMap[i]) / scaling;

        points[i].x = i * 2.5;
        points[i].y = heightMap[i];

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(i * 2.5, heightMap[i] * 0.001); // partea de jos
        modelMatrix *= transform2D::Shear(factor);
        modelMatrix *= transform2D::Scale(scaling, maxY);
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::GetTankPos(int pos)
{
    tank1.x = points[pos].x;
    tank1.y = points[pos].y + ((tank1.x - points[pos].x)
        / (points[pos + 1].x - points[pos].x)) * ((points[pos + 1].y - points[pos].y));

    segmentPointsDist.x = points[pos + 1].x - points[pos].x;
    segmentPointsDist.y = points[pos + 1].y - points[pos].y;

    angularStep = atan2(segmentPointsDist.y, segmentPointsDist.x);
}

void Tema1::MoveTank()
{
    tankMatrix = glm::mat3(1);
    tankMatrix *= transform2D::Translate(tank1.x, tank1.y);
    tankMatrix *= transform2D::Translate(0, 8);
    tankMatrix *= transform2D::Rotate(angularStep);
    tankMatrix *= transform2D::Translate(0, -8);

    RenderMesh2D(meshes["bottomTrapeze"], shaders["VertexColor"], tankMatrix);

    tankMatrix *= transform2D::Translate(0, 16);
    tankMatrix *= transform2D::Translate(0, 12);
    tankMatrix *= transform2D::Rotate(RADIANS(180));
    tankMatrix *= transform2D::Translate(0, -12);

    RenderMesh2D(meshes["upperTrapeze"], shaders["VertexColor"], tankMatrix);

    tankMatrix = glm::mat3(1);
    tankMatrix *= transform2D::Translate(tank1.x, tank1.y);
    tankMatrix *= transform2D::Translate(0, 8);
    tankMatrix *= transform2D::Rotate(angularStep);
    tankMatrix *= transform2D::Translate(0, -8);
    tankMatrix *= transform2D::Translate(0, 30);

    RenderMesh2D(meshes["circleTank1"], shaders["VertexColor"], tankMatrix);

    tankMatrix *= transform2D::Translate(0, 20);
    tankMatrix *= transform2D::Rotate(angularStepPipe);

    RenderMesh2D(meshes["pipeTank1"], shaders["VertexColor"], tankMatrix);
}