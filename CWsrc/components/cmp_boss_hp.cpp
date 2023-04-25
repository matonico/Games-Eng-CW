#include <engine.h>
#include "cmp_boss.h"
#include "cmp_boss_hp.h"
#include "LevelSystem.h"
#include "SFML/Audio.hpp"

using namespace std;
using namespace sf;

static short int BOSSMAXHP = 200;
int bossCurrentHP = BOSSMAXHP;


void BossHPComponent::update(double dt) {
	if (bossCurrentHP <= 0) {
		_parent->setForDelete();
		_bossDieSound.setVolume(50);// TODO set as user preference
		_bossDieSound.play();
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
	if (!_bossDieSoundBuffer.loadFromFile("res/audio/boss_die.wav"))
	{
		cout << "cant load sound fx" << endl;
	}
	_bossDieSound.setBuffer(_bossDieSoundBuffer);
}