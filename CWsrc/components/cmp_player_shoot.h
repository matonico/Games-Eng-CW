#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PlayerShootComponent : public Component {
protected:

	void shoot() const;

public:
	void update(double dt) override;
	void render() override {}
	explicit PlayerShootComponent(Entity* p);
	PlayerShootComponent() = delete;
};
