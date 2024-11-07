#pragma once

#include "components/simple_scene.h"
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
        void GetTankPos(int pos);
        void MoveTank();

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        glm::mat3 projectileMatrix;
        glm::mat3 tankMatrix;
        glm::mat3 trajectoryMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        float factor;
        float posTank1;
        float angularStepPipe;
        float fire;

        std::vector<double> heightMap;
        std::vector<glm::vec2> points;
        glm::vec2 tank1;
        glm::vec2 segmentPointsDist;
        glm::vec2 projectilePos;
        glm::vec2 trajectoryPos;
        glm::vec2 vProj = glm::vec2(1, 1);
    };
}   // namespace m1