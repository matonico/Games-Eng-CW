#pragma once
#include "cmp_physics.h"
#include <ecm.h>
#include "SFML/Audio.hpp"

class PlayerHPComponent : public Component {
protected:
	sf::SoundBuffer _playerDieSoundBuffer;
	sf::Sound _playerDieSound;
public:
	bool _bossLvl;
	static void getHit(int dmg);
	void update(double dt) override;
	void render() override {}
	static void resetHP();
	int getHP();
	int getDeaths();
	void setDeaths(int num);
	explicit PlayerHPComponent(Entity* p, bool bossLvl);
	PlayerHPComponent() = delete;
};