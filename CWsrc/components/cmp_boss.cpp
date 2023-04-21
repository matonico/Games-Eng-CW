#include "cmp_boss.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <random>

using namespace std;
using namespace sf;
using namespace Physics;

bool BossPhysicsComponent::isGrounded() const {
	auto touch = getTouching();
	const auto& pos = _body->GetPosition();
	const float halfPlrHeigt = _size.y * .5f;
	const float halfPlrWidth = _size.x * .52f;
	b2WorldManifold manifold;
	for (const auto& contact : touch) {
		contact->GetWorldManifold(&manifold);
		const int numPoints = contact->GetManifold()->pointCount;
		bool onTop = numPoints > 0;
		// If all contacts are below the player.
		for (int j = 0; j < numPoints; j++) {
			onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
		}
		if (onTop) {
			return true;
		}
	}

	return false;
}

void BossPhysicsComponent::update(double dt) {

	const auto pos = _parent->getPosition();


	if (_direction == 1) {
		if (getVelocity().x < _maxVelocity.x)
			impulse({ (float)(dt * _groundspeed), 0 });
	}
	else {
		if (getVelocity().x > -_maxVelocity.x)
			impulse({ -(float)(dt * _groundspeed), 0 });
	}

	// Handle Jump
	if (_jumptime <= 0.0f) {
		_grounded = isGrounded();
		if (_grounded) {
			setVelocity(Vector2f(getVelocity().x * 1.5, 0.f));
			//teleport(Vector2f(pos.x, pos.y - 2.0f)); // Not sure if this is needed
			impulse(Vector2f(0, -15.f)); // changed gravity and this to make movement a bit snappier
		}
		_jumptime = 2.0f;
	}

	if (_directiontime <= 0.0f)
	{
		_direction *= -1;
		// random number to switch jumps
		std::random_device rd;
		std::mt19937 gen(rd());
		// define the range for init positions, multiplied by 2 later
		
		std::uniform_int_distribution<> posDistr(3,5);
		_directiontime = posDistr(gen);

	}

	//Are we in air?
	if (!_grounded) {
		// Check to see if we have landed yet
		_grounded = isGrounded();
		// disable friction while jumping
		setFriction(0.f);
	}
	else {
		//setFriction(0.1f);
		setFriction(0.f); // idk if better
	}

	// Clamp velocity.
	auto v = getVelocity();
	v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
	v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
	setVelocity(v);

	_jumptime -= dt;
	_directiontime -= dt;
	PhysicsComponent::update(dt);
}

BossPhysicsComponent::BossPhysicsComponent(Entity* p,
	const Vector2f& size)
	: PhysicsComponent(p, true, size) {
	_size = sv2_to_bv2(size, true);
	_maxVelocity = Vector2f(200.f, 700.f); // Changed max y vel so boost pads work
	_groundspeed = 30.f;
	_grounded = false;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	//Bullet items have higher-res collision detection
	_body->SetBullet(true);
}
