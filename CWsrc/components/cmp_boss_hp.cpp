#include <engine.h>
#include "cmp_boss.h"
#include "cmp_boss_hp.h"
#include "LevelSystem.h"

using namespace std;
using namespace sf;

static short int BOSSMAXHP = 200;
int bossCurrentHP = BOSSMAXHP;


void BossHPComponent::update(double dt) {
	if (bossCurrentHP <= 0) {
		_parent->setForDelete();
	}
}

void BossHPComponent::getHit(int dmg) {
	bossCurrentHP -=dmg;
}
void BossHPComponent::resetHP() {
	bossCurrentHP = BOSSMAXHP;
}

int BossHPComponent::getBossHP() {
	return bossCurrentHP;
}


BossHPComponent::BossHPComponent(Entity* p): Component(p)
{
}