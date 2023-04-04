#include <engine.h>
#include "cmp_player_hp.h"

using namespace std;
using namespace sf;

static short int MAXHP = 50;
int currentHP = MAXHP;

void PlayerHPComponent::update(double dt) {
	if (currentHP <= 0) {
		_parent->setForDelete();
		resetHP();
	}
}

void PlayerHPComponent::getHit(int dmg) {
	currentHP = currentHP - dmg;
}
void PlayerHPComponent::resetHP() {
	currentHP = MAXHP;
}

PlayerHPComponent::PlayerHPComponent(Entity* p)
	: Component(p) {}