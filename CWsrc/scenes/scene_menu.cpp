#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include "SFML/Window/Mouse.hpp"
#include <iostream>

using namespace std;
using namespace sf;

 
 static shared_ptr<TextComponent> menuText;
 static shared_ptr<Entity> title;
 static shared_ptr<TextComponent> playButton;
 static shared_ptr<Entity> playtext;
 static shared_ptr<TextComponent> settingsButton;
 static shared_ptr<Entity> settingstext;
 static shared_ptr<TextComponent> exitButton;
 static shared_ptr<Entity> exittext;

void MenuScene::Load() {
  cout << "Menu Load \n";
  
  // Main title
  auto title = makeEntity();
  auto menuText = title->addComponent<TextComponent>();
  menuText->SetText("Platformer Game");
  title->addTag("Title");
  menuText->getText()->setOrigin(Vector2f(menuText->getText()->getLocalBounds().width/2.f, 
                                          menuText->getText()->getLocalBounds().height/2.f));

  menuText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2 - 350.f)));
  menuText->getText()->setCharacterSize(45);

  // Play button
  playtext = makeEntity();
  playButton = playtext->addComponent<TextComponent>();
  playButton->SetText("Play");
  playtext->addTag("Play");
  playButton->getText()->setOrigin(Vector2f(playButton->getText()->getLocalBounds().width / 2.f,
                                            playButton->getText()->getLocalBounds().height / 2.f));

  playButton->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2)));
  playButton->getText()->setCharacterSize(35);
  
  //Settings button
  settingstext = makeEntity();
  settingsButton = settingstext->addComponent<TextComponent>();
  settingsButton->SetText("Settings");
  settingstext->addTag("Settings");
  settingsButton->getText()->setOrigin(Vector2f(settingsButton->getText()->getLocalBounds().width / 2.f,
                                                settingsButton->getText()->getLocalBounds().height / 2.f));

  settingsButton->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2) + 100.f));
  settingsButton->getText()->setCharacterSize(35);
  
  //Exit button
  exittext = makeEntity();
  exitButton = exittext->addComponent<TextComponent>();
  exitButton->SetText("Exit");
  exittext->addTag("Exit");
  exitButton->getText()->setOrigin(Vector2f(exitButton->getText()->getLocalBounds().width / 2.f,
                                            exitButton->getText()->getLocalBounds().height / 2.f));

  exitButton->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2) + 200.f));
  exitButton->getText()->setCharacterSize(35);

  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

    //left in for testing purposes
    if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
      Engine::ChangeScene(&level1);
      return;
  }
  //play button
  if (playButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
      playButton->getText()->setFillColor(sf::Color::Red);
      if (Mouse::isButtonPressed(Mouse::Left())) {
          Engine::ChangeScene(&menu);
          return;
          
      }
  }
  else {
      playButton->getText()->setFillColor(sf::Color::White);
  }
  
  //settings button
  if (settingsButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
      settingsButton->getText()->setFillColor(sf::Color::Red);
      if (Mouse::isButtonPressed(Mouse::Left())) {
          Engine::ChangeScene(&settings);
          return;
      }

  }
  else {
      settingsButton->getText()->setFillColor(sf::Color::White);
  }
  
  //exit button
  if (exitButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
      exitButton->getText()->setFillColor(sf::Color::Red);
      if (Mouse::isButtonPressed(Mouse::Left())) {
          Engine::GetWindow().close();
      }

  }
  else {
      exitButton->getText()->setFillColor(sf::Color::White);
  }
  
  Scene::Update(dt);
}
