#pragma once
#include "cmp_physics.h"
#include <ecm.h>
#include "SFML/Audio.hpp"

class BossHPComponent : public Component {
protected:
	sf::SoundBuffer _bossDieSoundBuffer;
	sf::Sound _bossDieSound;
public:
	static void getHit(int dmg);
	void update(double dt) override;
	void render() override {}
	static void resetHP();
	static int getBossHP();
	explicit BossHPComponent(Entity* p);
	BossHPComponent() = delete;
};