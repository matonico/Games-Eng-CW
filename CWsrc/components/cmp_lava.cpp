#include "cmp_lava.h"
#include "cmp_player_physics.h"
#include "cmp_player_hp.h"
#include "LevelSystem.h"
#include "system_physics.h"
#include "cmp_physics.h"
#include <engine.h>

using namespace std;
using namespace sf;


void LavaComponent::update(double dt) {
    if (auto pl = _player.lock()) {
        if (length(pl->getPosition() - _parent->getPosition()) < 30.0) {
            PlayerHPComponent::getHit(50);
        }
    }
}

LavaComponent::LavaComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}
