#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HurtBossComponent : public Component {
protected:
	std::shared_ptr<Entity> _boss; // entity of boss

public:
	void update(double dt) override;
	void render() override {}
	explicit HurtBossComponent(Entity* p);
	HurtBossComponent() = delete;
};
