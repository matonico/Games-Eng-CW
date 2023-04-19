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

  Scene::Update(dt);
}
