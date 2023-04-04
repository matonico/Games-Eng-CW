#include "cmp_hurt_player.h"
#include <engine.h>
#include "cmp_player_hp.h"

using namespace std;
using namespace sf;

static int HP = 50;

void HurtComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 30.0) {
      //pl->setForDelete();
       PlayerHPComponent::getHit(30);
      _parent->setForDelete();
    }
  }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}
