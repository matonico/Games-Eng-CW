#pragma once

#include "cmp_actor_movement.h"

class EnemyAIComponent : public ActorMovementComponent {
protected:
  sf::Vector2f _direction;
  bool _spin;

public:
  void update(double dt) override;

  explicit EnemyAIComponent(Entity* p,bool spin);

  EnemyAIComponent() = delete;
};
