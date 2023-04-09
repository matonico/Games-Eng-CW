#include "cmp_bullet.h"
#include <engine.h>

using namespace std;
using namespace sf;

void BulletComponent::update(double dt) {
  _lifetime -= dt;
  if (_lifetime <= 0.f) {
    _parent->setForDelete();
  }
  else {
      for (auto w : _walls)
      {
          if (length(w->getPosition() - _parent->getPosition()) < 30.0f)
          {
              _parent->setForDelete();
          }
      }
  }
}

BulletComponent::BulletComponent(Entity* p, float lifetime)
    : Component(p), _lifetime(lifetime),_walls(_parent->scene->ents.find("wall")) {}
