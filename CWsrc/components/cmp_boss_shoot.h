#pragma once
#include "cmp_physics.h"
#include <ecm.h>
#include "SFML/Audio.hpp"

class BossShootComponent : public Component {
protected:
	sf::SoundBuffer _bossShootSoundBuffer;
	sf::Sound _bossShootSound;
	void fire() const;
	float _firetime;
	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit BossShootComponent(Entity* p);
	BossShootComponent() = delete;
};
