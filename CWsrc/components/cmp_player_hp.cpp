#include <engine.h>
#include "cmp_player_physics.h"
#include "cmp_player_hp.h"
#include "cmp_checkpoint.h"
#include "cmp_boss_hp.h"
#include "LevelSystem.h"
#include "SFML/Audio.hpp"

using namespace std;
using namespace sf;

static short int MAXHP = 50;
int currentHP = MAXHP;
int deaths = 0;
sf::SoundBuffer _takeDmgSoundBuffer;
sf::Sound _takeDmgSound;

void PlayerHPComponent::update(double dt) {
	if (currentHP <= 0) {
		if (!_bossLvl) {
			_playerDieSound.play();
			auto phys = _parent->GetCompatibleComponent<PlayerPhysicsComponent>();
			std::shared_ptr<PlayerPhysicsComponent> physCmp = phys.front();
			auto chk = _parent->GetCompatibleComponent<CheckpointComponent>();
			std::shared_ptr<CheckpointComponent> checkCmp = chk.front();
			physCmp->setVelocity(Vector2f(0.0f, 0.0f));
			physCmp->teleport(checkCmp->getCheckpoint());
			resetHP();
		}
	}
}

void PlayerHPComponent::getHit(int dmg)  {
	currentHP = currentHP - dmg;
	_takeDmgSound.play();
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

PlayerHPComponent::PlayerHPComponent(Entity* p, bool bossLvl)
	: Component(p) 
{
	_bossLvl = bossLvl;
	// get hit sound
	if (!_takeDmgSoundBuffer.loadFromFile("res/audio/hurt.wav"))
	{
		cout << "cant load sound fx" << endl;
	}
	_takeDmgSound.setBuffer(_takeDmgSoundBuffer);
	_takeDmgSound.setVolume(50); // TODO set from user preference

	// death sound
	if (!_playerDieSoundBuffer.loadFromFile("res/audio/player_die.wav"))
	{
		cout << "cant load sound fx" << endl;
	}
	_playerDieSound.setBuffer(_playerDieSoundBuffer);
	_playerDieSound.setVolume(50); // TODO set from user preference
}