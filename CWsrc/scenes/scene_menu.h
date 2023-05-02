#pragma once
#include <SFML\Audio\Music.hpp>
#include "engine.h"

class MenuScene : public Scene {
public:
	sf::Music music;

  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;
  void Update(const double& dt) override;
};
