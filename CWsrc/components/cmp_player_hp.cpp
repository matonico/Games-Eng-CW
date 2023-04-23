#include <engine.h>
#include "cmp_player_physics.h"
#include "cmp_player_hp.h"
#include "cmp_checkpoint.h"
#include "LevelSystem.h"

using namespace std;
using namespace sf;

static short int MAXHP = 50;
int currentHP = MAXHP;
int deaths = 0;

void PlayerHPComponent::update(double dt) {
	if (currentHP <= 0) {
		auto phys = _parent->GetCompatibleComponent<PlayerPhysicsComponent>();
		std::shared_ptr<PlayerPhysicsComponent> physCmp = phys.front();
		auto chk = _parent->GetCompatibleComponent<CheckpointComponent>();
		std::shared_ptr<CheckpointComponent> checkCmp = chk.front();
		physCmp->setVelocity(Vector2f(0.0f, 0.0f));
		physCmp->teleport(checkCmp->getCheckpoint());
		resetHP();
	}
}

void PlayerHPComponent::getHit(int dmg) {
	currentHP = currentHP - dmg;
}
void PlayerHPComponent::resetHP() {
	currentHP = MAXHP;
	deaths++;
}

int PlayerHPComponent::getHP() {
	return currentHP;
}

int PlayerHPComponent::getDeaths() {
	return deaths;
}

PlayerHPComponent::PlayerHPComponent(Entity* p)
	: Component(p) {}