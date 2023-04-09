#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class BulletComponent : public Component {
protected:
  float _lifetime;
  std::vector<std::shared_ptr<Entity>> _walls; // Vector of entities with wall tag

public:
  void update(double dt) override;
  void render() override {}
  explicit BulletComponent(Entity* p, float lifetime = 3.f);
  BulletComponent() = delete;
};
