#pragma once

#include "cmp_physics.h"

class BossPhysicsComponent : public PhysicsComponent {
protected:
	b2Vec2 _size;
	sf::Vector2f _maxVelocity;
	bool _grounded;
	float _groundspeed;
	int _direction = 1;
	float _directiontime = 2.0f;
	float _jumptime = 2.0f;

	bool isGrounded() const;

public:
	void update(double dt) override;

	explicit BossPhysicsComponent(Entity* p, const sf::Vector2f& size);

	BossPhysicsComponent() = delete;
};
