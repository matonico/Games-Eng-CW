#include "cmp_enemy_ai.h"

using namespace std;
using namespace sf;

void EnemyAIComponent::update(double dt) {
  auto mov = _direction * (float)(dt * _speed);
  mov.x += _direction.x * 16.f;
  if (!validMove(_parent->getPosition() + mov)) {
    _direction *= -1.f;
  }
  if (_spin) // if spin is set to true, it will spin the entity 
  {
      static float angle = 0.f;
      angle += 1.f * dt;

      _parent->setRotation(180.f + angle * 90.f*_direction.x);
    
  }

  move(_direction * (float)(dt * _speed));
  ActorMovementComponent::update(dt);
}

EnemyAIComponent::EnemyAIComponent(Entity* p, bool spin) : ActorMovementComponent(p) {
  _spin = spin;
  _direction = Vector2f(1.0f, 0);
  _speed = 100.0f;
}
