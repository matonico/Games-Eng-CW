#pragma once
#include <SFML\Audio\Music.hpp>
#include "engine.h"

class HubScene : public Scene {
public:
	sf::Music music;
	bool _pause = false;
	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;
};
