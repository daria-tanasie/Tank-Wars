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

    BuildMap(); // generating the function

    glm::vec3 origin = glm::vec3(0, 0, 0);
    glm::vec3 middleTrapeze = glm::vec3(0, 0,0);
    float squareSide = 1;
    float trapezeBottom = 90, trapezeSide = 24;
    float pipeLittleSide = 7, pipeBigSide = 50;
    float healtBarLen = 50, healtBarSide = 9;
    float trajPointsRadius = 2, turretRadius = 30, projRadius = 5;

    Mesh* square1 = object2D::CreateSquare("square1", origin, squareSide, glm::vec3(0.74, 0.73, 0.22), true);
    AddMeshToList(square1);

    Mesh* upperTrapeze = object2D::CreateTrapeze("upperTrapeze", middleTrapeze, trapezeBottom, trapezeSide, glm::vec3(0.65, 0.64, 0.48), true);
    AddMeshToList(upperTrapeze);
    Mesh* upperTrapeze2 = object2D::CreateTrapeze("upperTrapeze2", middleTrapeze, trapezeBottom, trapezeSide, glm::vec3(0.25, 0.52, 0.04), true);
    AddMeshToList(upperTrapeze2);

    trapezeBottom = 60; // changes for the bottom trapeze
    trapezeSide = 16;

    Mesh* bottomTrapeze = object2D::CreateTrapeze("bottomTrapeze", middleTrapeze, trapezeBottom, trapezeSide, glm::vec3(0.49, 0.48, 0.36), true);
    AddMeshToList(bottomTrapeze);

    Mesh* circleTank1 = object2D::CreateCircle("circleTank1", origin, 60, turretRadius, 1.0f, glm::vec3(0.65, 0.64, 0.48), true);
    AddMeshToList(circleTank1);

    Mesh* circleTank2 = object2D::CreateCircle("circleTank2", origin, 60, turretRadius, 1.0f, glm::vec3(0.25, 0.52, 0.04), true);
    AddMeshToList(circleTank2);

    Mesh* pipeTank1 = object2D::CreateRectangle("pipeTank1", origin, pipeLittleSide, pipeBigSide, glm::vec3(0.27, 0.27, 0.04), true);
    AddMeshToList(pipeTank1);

    Mesh* projectileTank1 = object2D::CreateCircle("projectileTank1", origin, 40, projRadius, 2.0f, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectileTank1);

    Mesh* trajectory = object2D::CreateCircle("trajectory", origin, 40, trajPointsRadius, 2.0f, glm::vec3(1, 1, 1), true);
    AddMeshToList(trajectory);

    Mesh* healthBar1 = object2D::CreateRectangle("healthBar1", origin, healtBarSide, healtBarLen, glm::vec3(1, 1, 1), false);
    AddMeshToList(healthBar1);
    Mesh* healthBarInner1 = object2D::CreateRectangle("healthBarInner1", origin, healtBarSide, healtBarLen, glm::vec3(1, 1, 1), true);
    AddMeshToList(healthBarInner1);
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

    glm::ivec2 resolution = window->GetResolution();

    if (firstTank->hp > 0) {
        firstTank->GetTankPos(points);
        MoveTank(firstTank, 1);
        RenderTrajectory(firstTank, secondTank, gravity, magnitude);
        Shoot(firstTank, secondTank, deltaTimeSeconds);
    }

    if (secondTank->hp > 0) {
        secondTank->GetTankPos(points);
        MoveTank(secondTank, 2);
        RenderTrajectory(secondTank, firstTank, gravity, magnitude);
        Shoot(secondTank, firstTank, deltaTimeSeconds);
    }

    UpdateTerrain(deltaTimeSeconds);

    if (firstTank->y + 10 < 0) {
        firstTank->hp = 0;
    }

    if (secondTank->y  + 10 < 0) {
        secondTank->hp = 0;
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
        if (firstTank->angularStepPipe - RADIANS(30) * deltaTime > -RADIANS(90)) {
            firstTank->angularStepPipe -= RADIANS(30) * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        if (firstTank->angularStepPipe + RADIANS(30) * deltaTime <  RADIANS(90)) {
            firstTank->angularStepPipe += RADIANS(30) * deltaTime;
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
        if (secondTank->angularStepPipe - RADIANS(30) * deltaTime > -RADIANS(90)) {
            secondTank->angularStepPipe -= RADIANS(30) * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_DOWN))
    {
        if (secondTank->angularStepPipe + RADIANS(30) * deltaTime < RADIANS(90)) {
            secondTank->angularStepPipe += RADIANS(30) * deltaTime;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (window->KeyHold(GLFW_KEY_SPACE)) {
        glm::vec2 projectilePos = glm::vec2(firstTank->tankMatrix[2][0], firstTank->tankMatrix[2][1]);
        glm::vec2 vProj = glm::vec2(cos(firstTank->angularStepPipe + firstTank->angularStep + RADIANS(90)),
            sin(firstTank->angularStepPipe + firstTank->angularStep + RADIANS(90))) * magnitude;

        Projectile proj = Projectile(projectilePos, vProj);
        firstTank->projectiles.push_back(proj);
    }

    if (window->KeyHold(GLFW_KEY_ENTER)) {
        glm::vec2 projectilePos = glm::vec2(secondTank->tankMatrix[2][0], secondTank->tankMatrix[2][1]);
        glm::vec2 vProj = glm::vec2(cos(secondTank->angularStepPipe + secondTank->angularStep + RADIANS(90)),
            sin(secondTank->angularStepPipe + secondTank->angularStep + RADIANS(90))) * magnitude;

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

    for (int i = 0; i < resolution.x; i++)
    {
        heightMap[i] = 100 * sin(0.0005 * i) + 118 * sin(0.008 * i) + 150 * sin(0.001 * i) + 140 * sin(0.005 * i) + 70;
    }
}

void Tema1::BuildTerrain()
{
    double maxY;

    for (double i = 0; i < heightMap.size() - 1; i++) {

        if (heightMap[i] < heightMap[i + 1]) {
            maxY = heightMap[i + 1];
        }
        else {
            maxY = heightMap[i];
        }

        float factor = (heightMap[i + 1] - heightMap[i]);

        points[i].x = i;
        points[i].y = heightMap[i];

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(i, heightMap[i]);
        modelMatrix *= transform2D::Shear(factor);
        modelMatrix *= transform2D::Scale(1, maxY);

        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderTrajectory(Tank* tank, Tank* enemy, float gravity, float magnitude)
{
    glm::ivec2 resolution = window->GetResolution();

    tank->trajectoryPos = glm::vec2(tank->tankMatrix[2][0], tank->tankMatrix[2][1]);
    glm::vec2 v = glm::vec2(cos(tank->angularStepPipe + tank->angularStep + RADIANS(90)),
                    sin(tank->angularStepPipe + tank->angularStep + RADIANS(90))) * magnitude;

    glm::vec2 trajPointsPos = tank->trajectoryPos;
    float elapsedTime = 0.05f;

    for (int i = 0; i < 200; i++) {
        float timeTaken = i * elapsedTime;

        trajPointsPos.x = tank->trajectoryPos.x + v.x * timeTaken;
        trajPointsPos.y = tank->trajectoryPos.y + v.y * timeTaken - 0.5f * gravity * timeTaken * timeTaken;
        
        tank->trajectoryMatrix = glm::mat3(1);
        tank->trajectoryMatrix *= transform2D::Translate(trajPointsPos.x, trajPointsPos.y);

        if ((int)(trajPointsPos.x) > 0 && (int)(trajPointsPos.x) < resolution.x
                && trajPointsPos.y < heightMap[(int)(trajPointsPos.x)]) {
            break;
        }

        if ((int)(trajPointsPos.x) > enemy->x - 45 && (int)(trajPointsPos.x) < enemy->x + 45
                && (int)(trajPointsPos.y) - 45 < secondTank->y) {
            break;
        }

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
    tank->healthMatrix *= transform2D::Scale(1, 0.01 * tank->hp);
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

void Tema1::Shoot(Tank* tank1, Tank* tank2, float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    for (int i = 0; i < tank1->projectiles.size(); i++) {
        Projectile& currentProj = tank1->projectiles[i];
        currentProj.position += currentProj.v * deltaTimeSeconds;
        currentProj.v.y -= gravity * deltaTimeSeconds;
        projectileMatrix = glm::mat3(1);
        projectileMatrix *= transform2D::Translate(currentProj.position.x, currentProj.position.y);

        if (currentProj.position.x + 5 > resolution.x || currentProj.position.x - 5 < 0
                || currentProj.position.y < 0) {
            tank1->projectiles.erase(tank1->projectiles.begin() + i);
            continue;
        }

        if (currentProj.position.y > tank2->y - 35 && currentProj.position.y < tank2->y + 35
            && currentProj.position.x > tank2->x - 45 && currentProj.position.x < tank2->x + 45 && tank2->hp > 0) {
            tank2->hp -= 20;
            tank1->projectiles.erase(tank1->projectiles.begin() + i);
            continue;
        }
        else {
            RenderMesh2D(meshes["projectileTank1"], shaders["VertexColor"], projectileMatrix);
        }

        if (currentProj.position.y > tank1->y - 35 && currentProj.position.y < tank1->y + 35
            && currentProj.position.x > tank1->x - 30 && currentProj.position.x < tank1->x + 30 && tank2->hp > 0) {
            tank1->hp -= 20;
            tank1->projectiles.erase(tank1->projectiles.begin() + i);
            continue;
        }
        else {
            RenderMesh2D(meshes["projectileTank1"], shaders["VertexColor"], projectileMatrix);
        }

        float t = (currentProj.position.x - points[(int)(currentProj.position.x)].x)
            / (points[(int)(currentProj.position.x) + 1].x - points[(int)(currentProj.position.x)].x);

        float yExp = points[(int)(currentProj.position.x)].y
            + t * (points[(int)(currentProj.position.x) + 1].y - points[(int)(currentProj.position.x)].y);

        if (currentProj.position.x > 0 && currentProj.position.x < resolution.x
            && currentProj.position.y - yExp < 3) {

            int holeEnd, holeStart;

            if (currentProj.position.x + currentProj.explosionRadius > resolution.x) {
                holeEnd = resolution.x - 2;
            }
            else {
                holeEnd = currentProj.position.x + currentProj.explosionRadius;
            }

            if (currentProj.position.x - currentProj.explosionRadius + 1 < 0) {
                holeStart = 0;
            }
            else {
                holeStart = currentProj.position.x - currentProj.explosionRadius + 1;
            }
 
            for (int j = holeStart; j <= holeEnd; j++) {

                float hole = -sqrt(currentProj.explosionRadius * currentProj.explosionRadius
                    - (j - currentProj.position.x) * (j - currentProj.position.x)) + yExp;

                if (heightMap[j] > hole) {  // not to calculate more than the height map values for the circle hole
                    heightMap[j] = hole;
                }
            }

            tank1->projectiles.erase(tank1->projectiles.begin() + i);
            continue;
        }
        else {
            RenderMesh2D(meshes["projectileTank1"], shaders["VertexColor"], projectileMatrix);
        }
    }
}

void Tema1::UpdateTerrain(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    for (int i = 0; i < resolution.x - 1; i++) {

        if (heightMap[i] - 5 > heightMap[i + 1]) {
            heightMap[i] -= deltaTimeSeconds * 100;
            heightMap[i + 1] += deltaTimeSeconds * 100;
        }
        else if (heightMap[i + 1] - 5 > heightMap[i]) {
            heightMap[i] += deltaTimeSeconds * 100;
            heightMap[i + 1] -= deltaTimeSeconds * 100;
        }
    }
}