#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void MenuScene::Load() {
  cout << "Menu Load \n";
  
  // Main title
  auto title = makeEntity();
  auto menuText = title->addComponent<TextComponent>();
  menuText->SetText("Game name here");
  title->addTag("Title");
  menuText->getText()->setOrigin(Vector2f(0.0f, 0.0f));
  menuText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2)));
  menuText->getText()->setCharacterSize(45);

  
















  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

  if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
    Engine::ChangeScene(&level1);
  }

    if (exitButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
        exitButton->getText()->setFillColor(sf::Color::Red);
        if (Mouse::isButtonPressed(Mouse::Left())) {
            Engine::GetWindow().close();
            return;
        }
    }
    else {
        exitButton->getText()->setFillColor(sf::Color::White);
    }

  //play button
    auto playEnt = ents.find("Play")[0]->GetCompatibleComponent<TextComponent>();
    shared_ptr<TextComponent>playButton = playEnt.front();
   
  if (playButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
      playButton->getText()->setFillColor(sf::Color::Red);
      if (Mouse::isButtonPressed(Mouse::Left())) {
          Engine::ChangeScene(&level1);
          return;
      }
  }
  else {
      playButton->getText()->setFillColor(sf::Color::White);
  }
  
  //settings button
    auto settingsEnt = ents.find("Settings")[0]->GetCompatibleComponent<TextComponent>();
    shared_ptr<TextComponent>settingsButton = settingsEnt.front();

  if (settingsButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
      settingsButton->getText()->setFillColor(sf::Color::Red);
      if (Mouse::isButtonPressed(Mouse::Left())) {
          Engine::ChangeScene((Scene*)&settings);
          return;
      }

  }
  else {
      settingsButton->getText()->setFillColor(sf::Color::White);
  }
  
   
  
  
  Scene::Update(dt);
}
