#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class BossHPComponent : public Component {
protected:
public:
	static void getHit(int dmg);
	void update(double dt) override;
	void render() override {}
	static void resetHP();
	static int getBossHP();
	explicit BossHPComponent(Entity* p);
	BossHPComponent() = delete;
};