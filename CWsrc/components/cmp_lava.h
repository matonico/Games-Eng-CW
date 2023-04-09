#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class LavaComponent : public Component {
protected:
	void fire() const;
	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit LavaComponent(Entity* p);
	LavaComponent() = delete;
};
