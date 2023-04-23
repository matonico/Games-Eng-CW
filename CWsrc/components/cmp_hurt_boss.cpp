#include "cmp_hurt_boss.h"
#include "cmp_boss_hp.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtBossComponent::update(double dt) {
	// Check boss collision with bullet

	if (length(_boss->getPosition() - _parent->getPosition()) < 110.0f)
	{
		_boss->get_components<BossHPComponent>()[0]->getHit(10.0f);
		_parent->setForDelete();
	}

}

HurtBossComponent::HurtBossComponent(Entity* p)
	: Component(p), _boss(_parent->scene->ents.find("boss")[0]) {} // fill vector with boss