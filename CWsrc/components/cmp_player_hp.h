#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PlayerHPComponent : public Component {
public:
	static void getHit(int dmg);
	void update(double dt) override;
	void render() override {}
	static void resetHP();
	int getHP();
	explicit PlayerHPComponent(Entity* p);
	PlayerHPComponent() = delete;
};