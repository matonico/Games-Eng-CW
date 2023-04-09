#include <engine.h>
#include "cmp_player_physics.h"
#include "cmp_player_hp.h"
#include "LevelSystem.h"

using namespace std;
using namespace sf;

static short int MAXHP = 50;
int currentHP = MAXHP;

void PlayerHPComponent::update(double dt) {
	if (currentHP <= 0) {
		auto phys = _parent->GetCompatibleComponent<PlayerPhysicsComponent>();
		std::shared_ptr<PlayerPhysicsComponent> physCmp = phys.front();
		physCmp->teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		resetHP();
	}
}

void PlayerHPComponent::getHit(int dmg) {
	currentHP = currentHP - dmg;
}
void PlayerHPComponent::resetHP() {
	currentHP = MAXHP;
}

int PlayerHPComponent::getHP() {
	return currentHP;
}

PlayerHPComponent::PlayerHPComponent(Entity* p)
	: Component(p) {}