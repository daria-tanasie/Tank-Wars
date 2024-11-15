#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/Tank.h"
#include "lab_m1/Tema1/Projectile.h"
#include "lab_m1/Tema1/objects.h"
#include <vector>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void BuildTerrain();
        void BuildMap();
        void MoveTank(Tank *tank, int tankNr);
        void RenderTrajectory(Tank* tank, Tank* enemy, float gravity, float magnitude);
        void Shoot(Tank* tank1, Tank* tank2, float deltaTimeSeconds);
        void UpdateTerrain(float deltaTimeSeconds);

    protected:
        glm::mat3 modelMatrix;
        glm::mat3 projectileMatrix;
        glm::mat3 projStart;

        float magnitude = 700.0f;
        float gravity = 700.0f;

        std::vector<double> heightMap;
        std::vector<glm::vec2> points;

        Tank* firstTank = new Tank(100, 0, 0, 100);
        Tank* secondTank = new Tank(1000, 0, 0, 100);
    };
}   // namespace m1