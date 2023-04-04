#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HurtEnemyComponent : public Component {
protected:
	void fire() const;
	std::vector<std::shared_ptr<Entity>> _enemies; // Vector of entities with enemy tag

public:
	void update(double dt) override;
	void render() override {}
	explicit HurtEnemyComponent(Entity* p);
	HurtEnemyComponent() = delete;
};
