#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class BoostComponent : public Component {
protected:
	std::vector<std::shared_ptr<Entity>> _boostTiles; // Vector of entities with boost

public:
	void update(double dt) override;
	void render() override {}
	explicit BoostComponent(Entity* p);
	BoostComponent() = delete;
};
