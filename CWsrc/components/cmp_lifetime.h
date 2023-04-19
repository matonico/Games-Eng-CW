#pragma once
#include "ecm.h"

using namespace std;
using namespace sf;


class LifetimeComponent : public Component {
protected:
	float _lifetime; // Lifetime of the object
public:
	void update(double dt) override;
	void render() override {}
	Vector2f getCheckpoint();
	explicit LifetimeComponent(Entity* p,float l);
	LifetimeComponent() = delete;
};
