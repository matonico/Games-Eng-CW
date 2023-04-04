#include "cmp_hurt_enemy.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtEnemyComponent::update(double dt) {
    // loop over the enemies and check collision with bullet
    for (auto e : _enemies)
    {
        if (length(e->getPosition() - _parent->getPosition()) < 25.0f)
        {
            e->setForDelete();
            _parent->setForDelete();
        }
    }
}

HurtEnemyComponent::HurtEnemyComponent(Entity* p)
    : Component(p), _enemies(_parent->scene->ents.find("enemy")) {} // fill vector with enemies
