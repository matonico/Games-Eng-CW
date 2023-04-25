#pragma once
#include "cmp_physics.h"
#include <ecm.h>
#include "SFML/Audio.hpp"

class PlayerShootComponent : public Component {
protected:
	sf::SoundBuffer _shootSoundBuffer;
	sf::Sound _shootSound;
	void shoot() const;

public:
	void update(double dt) override;
	void render() override {}
	explicit PlayerShootComponent(Entity* p);
	PlayerShootComponent() = delete;
};
