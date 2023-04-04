#pragma once
#include "cmp_physics.h"
#include <ecm.h>
#include "cmp_player_hp.h"

class HurtComponent : public Component {
protected:
  void fire() const;
  std::weak_ptr<Entity> _player;

public:
  void update(double dt) override;
  void render() override {}
  explicit HurtComponent(Entity* p);
  HurtComponent() = delete;
};
