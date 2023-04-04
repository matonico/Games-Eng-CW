#include "cmp_hurt_enemy.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtEnemyComponent::update(double dt) {
    for (auto e : _enemies)
    {
        if (length(e->getPosition() - _parent->getPosition()) < 5.0f)
        {
            e->setForDelete();
            _parent->setForDelete();
        }
    }

    /*
    if (auto pl = _player.lock()) {
        if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {
            pl->setForDelete();
            _parent->setForDelete();
        }
    }
    */
}

HurtEnemyComponent::HurtEnemyComponent(Entity* p)
    : Component(p), _enemies(_parent->scene->ents.find("enemy")) {}
