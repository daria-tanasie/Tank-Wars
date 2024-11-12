#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/Projectile.h"
#include <vector>

namespace m1
{
	class Tank : public gfxc::SimpleScene
	{
	public:

		void GetTankPos(std::vector<glm::vec2> points);

		glm::mat3 tankMatrix;
		glm::mat3 trajectoryMatrix;
		glm::mat3 healthMatrix;

		float posTank;
		float angularStepPipe;
		float angularStep;
		float x;
		float y;
		float hp;
		int damageTaken;

		glm::vec2 projectilePos;
		glm::vec2 trajectoryPos;
		glm::vec2 vProj;

		std::vector<Projectile> projectiles;

		Tank(float pos, float tankAngle, float pipeAngle, float hpTank)
			: posTank(pos), angularStep(tankAngle), angularStepPipe(pipeAngle), hp(hpTank) {}
	};
}
