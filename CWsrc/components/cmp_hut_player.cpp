#include "cmp_hurt_player.h"
#include <engine.h>
#include "cmp_player_hp.h"

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {
	if (auto pl = _player.lock()) {
		if (length(pl->getPosition() - _parent->getPosition()) < _size) {
			// Get tags of parent, can use to determine how much damage to take
			set tags = _parent->getTags();

			if (tags.find("enemyBullet") != tags.end()) // Enemy damage
			{
				PlayerHPComponent::getHit(20);
			}
			else if (tags.find("spike") != tags.end()) // spike damage
			{
				PlayerHPComponent::getHit(50);
				return;
			}
			else if (tags.find("boss") != tags.end())
			{
				PlayerHPComponent::getHit(50);
				return;
			}
			_parent->setForDelete();
		}
	}
}

HurtComponent::HurtComponent(Entity* p, float size)
	: Component(p), _player(_parent->scene->ents.find("player")[0])
{
	_size = size;
}
