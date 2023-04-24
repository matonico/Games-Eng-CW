#pragma once
#include <SFML\Audio\Music.hpp>
#include "engine.h"

class Boss1Scene : public Scene {
public:
	sf::Music music;
	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;
};
