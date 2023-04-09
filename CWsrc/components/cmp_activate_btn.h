#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class ActivateButtonComponent : public Component {
protected:
	void respawnBridge();
	std::vector<std::shared_ptr<Entity>> _buttons; // Vector of entities with button tag

public:
	void update(double dt) override;
	void render() override {}
	explicit ActivateButtonComponent(Entity* p);
	ActivateButtonComponent() = delete;
};
