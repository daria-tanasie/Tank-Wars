#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/Tank.h"
#include "lab_m1/Tema1/Projectile.h"

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
    float healtBarLen = 50;
    float healtBarSide = 9;
    fire = 0;

    Mesh* square1 = object2D::CreateSquare("square1", origin, squareSide, glm::vec3(0.74, 0.73, 0.22), true);
    AddMeshToList(square1);

    Mesh* upperTrapeze = object2D::CreateTrapeze("upperTrapeze", middleTrapeze, trapezeBottom, trapezeSide, glm::vec3(0.65, 0.64, 0.48), true);
    AddMeshToList(upperTrapeze);
    Mesh* upperTrapeze2 = object2D::CreateTrapeze("upperTrapeze2", middleTrapeze, trapezeBottom, trapezeSide, glm::vec3(0.25, 0.52, 0.04), true);
    AddMeshToList(upperTrapeze2);

    trapezeBottom = 60; // changes for the bottom trapeze
    trapezeSide = 16;
    //middleTrapeze = glm::vec3(0, 0, 0);

    Mesh* bottomTrapeze = object2D::CreateTrapeze("bottomTrapeze", middleTrapeze, trapezeBottom, trapezeSide, glm::vec3(0.49, 0.48, 0.36), true);
    AddMeshToList(bottomTrapeze);

    Mesh* circleTank1 = object2D::CreateCircle("circleTank1", origin, 60, 30, 1.0f, glm::vec3(0.65, 0.64, 0.48), true);
    AddMeshToList(circleTank1);

    Mesh* circleTank2 = object2D::CreateCircle("circleTank2", origin, 60, 30, 1.0f, glm::vec3(0.25, 0.52, 0.04), true);
    AddMeshToList(circleTank2);

    Mesh* pipeTank1 = object2D::CreateRectangle("pipeTank1", origin, pipeLittleSide, pipeBigSide, glm::vec3(0.27, 0.27, 0.04), true);
    AddMeshToList(pipeTank1);

    Mesh* projectileTank1 = object2D::CreateCircle("projectileTank1", origin, 40, 5, 2.0f, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectileTank1);

    Mesh* trajectory = object2D::CreateCircle("trajectory", origin, 40, 2, 2.0f, glm::vec3(1, 1, 1), true);
    AddMeshToList(trajectory);

    Mesh* healthBar1 = object2D::CreateRectangle("healthBar1", origin, healtBarSide, healtBarLen, glm::vec3(1, 1, 1), false);
    AddMeshToList(healthBar1);
    Mesh* healthBarInner1 = object2D::CreateRectangle("healthBarInner1", origin, healtBarSide, healtBarLen, glm::vec3(1, 1, 1), true);
    AddMeshToList(healthBarInner1);

    firstTank->hp = healtBarLen;
    secondTank->hp = healtBarLen;
    firstTank->damageTaken = 100;
    secondTank->damageTaken = 100;
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
    float deltaTime = 0.1f;
    float gravity = 9.8f;

    BuildTerrain();


    if (firstTank->damageTaken >= 0) {
        firstTank->GetTankPos(points);
        MoveTank(firstTank, 1);
        RenderTrajectory(firstTank, deltaTime, gravity, 100.0f);
    }

    if (secondTank->damageTaken >= 0) {
        secondTank->GetTankPos(points);
        MoveTank(secondTank, 2);
        RenderTrajectory(secondTank, deltaTime, gravity, 100.0f);
    }

    for (int i = 0; i < firstTank->projectiles.size(); i++) {
        Projectile& currentProj = firstTank->projectiles[i];
        currentProj.position += currentProj.v * deltaTime;
        currentProj.v.y -= gravity * deltaTime;
        projectileMatrix = glm::mat3(1);
        projectileMatrix *= transform2D::Translate(currentProj.position.x, currentProj.position.y);

        if (currentProj.position.y > 10 + secondTank->y
                && (currentProj.position.x > secondTank->x - 30 && currentProj.position.x < secondTank->x + 30)) {
            secondTank->damageTaken -= 2;
        }


        RenderMesh2D(meshes["projectileTank1"], shaders["VertexColor"], projectileMatrix);   
    }

    for (int i = 0; i < secondTank->projectiles.size(); i++) {
        Projectile& currentProj = secondTank->projectiles[i];
        currentProj.position += currentProj.v * deltaTime;
        currentProj.v.y -= gravity * deltaTime;
        projectileMatrix = glm::mat3(1);
        projectileMatrix *= transform2D::Translate(currentProj.position.x, currentProj.position.y);

        if (currentProj.position.y > 10 + firstTank->y
            && (currentProj.position.x > firstTank->x - 15 && currentProj.position.x < firstTank->x + 15)) {
            firstTank->damageTaken -= 2;
        }


        RenderMesh2D(meshes["projectileTank1"], shaders["VertexColor"], projectileMatrix);
    }
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_D))
    {
        if (firstTank->posTank + 1 < points.size()) {
            firstTank->posTank += 1;
        }
    }

    if (window->KeyHold(GLFW_KEY_A))
    {   if (firstTank->posTank - 1 > 0)
            firstTank->posTank -= 1;
    }

    if (window->KeyHold(GLFW_KEY_W))
    {
        if (firstTank->angularStepPipe - RADIANS(40) * deltaTime > -RADIANS(90)) {
            firstTank->angularStepPipe -= RADIANS(40) * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        if (firstTank->angularStepPipe + RADIANS(40) * deltaTime <  RADIANS(90)) {
            firstTank->angularStepPipe += RADIANS(40) * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_RIGHT))
    {
        if (secondTank->posTank + 1 < points.size()) {
            secondTank->posTank += 1;
        }
    }

    if (window->KeyHold(GLFW_KEY_LEFT))
    {
        if (secondTank->posTank - 1 > 0)
            secondTank->posTank -= 1;
    }

    if (window->KeyHold(GLFW_KEY_UP))
    {
        if (secondTank->angularStepPipe - RADIANS(40) * deltaTime > -RADIANS(90)) {
            secondTank->angularStepPipe -= RADIANS(40) * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_DOWN))
    {
        if (secondTank->angularStepPipe + RADIANS(40) * deltaTime < RADIANS(90)) {
            secondTank->angularStepPipe += RADIANS(40) * deltaTime;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (window->KeyHold(GLFW_KEY_SPACE)) {
        glm::vec2 projectilePos = glm::vec2(firstTank->tankMatrix[2][0], firstTank->tankMatrix[2][1]);
        glm::vec2 vProj = glm::vec2(cos(firstTank->angularStepPipe + firstTank->angularStep + RADIANS(90)),
            sin(firstTank->angularStepPipe + firstTank->angularStep + RADIANS(90))) * 100.0f;

        Projectile proj = Projectile(projectilePos, vProj);
        firstTank->projectiles.push_back(proj);
    }

    if (window->KeyHold(GLFW_KEY_ENTER)) {
        glm::vec2 projectilePos = glm::vec2(secondTank->tankMatrix[2][0], secondTank->tankMatrix[2][1]);
        glm::vec2 vProj = glm::vec2(cos(secondTank->angularStepPipe + secondTank->angularStep + RADIANS(90)),
            sin(secondTank->angularStepPipe + secondTank->angularStep + RADIANS(90))) * 100.0f;

        Projectile proj = Projectile(projectilePos, vProj);
        secondTank->projectiles.push_back(proj);
    }
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

        float factor = (heightMap[i + 1] - heightMap[i]) / scaling;

        points[i].x = i * 2.5;
        points[i].y = heightMap[i];

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(i * 2.5, heightMap[i] * 0.001); // partea de jos
        modelMatrix *= transform2D::Shear(factor);
        modelMatrix *= transform2D::Scale(scaling, maxY);
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderTrajectory(Tank* tank, float deltaTime, float gravity, float magnitude)
{
    tank->trajectoryPos = glm::vec2(tank->tankMatrix[2][0], tank->tankMatrix[2][1]);
    glm::vec2 v = glm::vec2(cos(tank->angularStepPipe + tank->angularStep + RADIANS(90)),
                    sin(tank->angularStepPipe + tank->angularStep + RADIANS(90))) * magnitude;

    glm::vec2 trajPointsPos = tank->trajectoryPos;

    for (int i = 0; i < 300; i++) {
        float time = i * deltaTime;

        trajPointsPos.x = tank->trajectoryPos.x + v.x * time;
        trajPointsPos.y = tank->trajectoryPos.y + v.y * time - 0.5f * gravity * time * time;
        tank->trajectoryMatrix = glm::mat3(1);
        tank->trajectoryMatrix *= transform2D::Translate(trajPointsPos.x, trajPointsPos.y);
        RenderMesh2D(meshes["trajectory"], shaders["VertexColor"], tank->trajectoryMatrix);
    }
}

void Tema1::MoveTank(Tank* tank, int tankNr)
{
    tank->tankMatrix = glm::mat3(1);
    tank->tankMatrix *= transform2D::Translate(tank->x, tank->y);
    tank->tankMatrix *= transform2D::Translate(0, 8);
    tank->tankMatrix *= transform2D::Rotate(tank->angularStep);
    tank->tankMatrix *= transform2D::Translate(0, -8);

    RenderMesh2D(meshes["bottomTrapeze"], shaders["VertexColor"], tank->tankMatrix);

    tank->tankMatrix *= transform2D::Translate(0, 16);
    tank->tankMatrix *= transform2D::Translate(0, 12);
    tank->tankMatrix *= transform2D::Rotate(RADIANS(180));
    tank->tankMatrix *= transform2D::Translate(0, -12);

    if (tankNr == 1) {
        RenderMesh2D(meshes["upperTrapeze"], shaders["VertexColor"], tank->tankMatrix);
    }
    else {
        RenderMesh2D(meshes["upperTrapeze2"], shaders["VertexColor"], tank->tankMatrix);
    }

    tank->tankMatrix = glm::mat3(1);
    tank->tankMatrix *= transform2D::Translate(tank->x, tank->y);
    tank->tankMatrix *= transform2D::Translate(-20, 100);
    tank->tankMatrix *= transform2D::Translate(0, 4.5);
    tank->tankMatrix *= transform2D::Rotate(RADIANS(270));
    tank->tankMatrix *= transform2D::Translate(0, -4.5);

    RenderMesh2D(meshes["healthBar1"], shaders["VertexColor"], tank->tankMatrix);

    tank->healthMatrix = tank->tankMatrix;
    tank->healthMatrix *= transform2D::Scale(1, 0.01 * tank->damageTaken);
    RenderMesh2D(meshes["healthBarInner1"], shaders["VertexColor"], tank->healthMatrix);

    tank->tankMatrix = glm::mat3(1);
    tank->tankMatrix *= transform2D::Translate(tank->x, tank->y);
    tank->tankMatrix *= transform2D::Translate(0, 8);
    tank->tankMatrix *= transform2D::Rotate(tank->angularStep);
    tank->tankMatrix *= transform2D::Translate(0, -8);
    tank->tankMatrix *= transform2D::Translate(0, 30);

    if (tankNr == 1) {
        RenderMesh2D(meshes["circleTank1"], shaders["VertexColor"], tank->tankMatrix);
    }
    else {
        RenderMesh2D(meshes["circleTank2"], shaders["VertexColor"], tank->tankMatrix);
    }

    tank->tankMatrix *= transform2D::Translate(0, 20);
    tank->tankMatrix *= transform2D::Rotate(tank->angularStepPipe);

    RenderMesh2D(meshes["pipeTank1"], shaders["VertexColor"], tank->tankMatrix);
}