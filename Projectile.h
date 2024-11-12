#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace m1
{
	class Projectile
	{
	public:
		glm::vec2 position;
		glm::vec2 v;

		int explosionRadius = 50;

		Projectile(glm::vec2 pos, glm::vec2 vProj)
		{
			position = pos;
			v = vProj;
		}
	};
}