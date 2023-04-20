#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include "SFML/Window/Mouse.hpp"
#include <iostream>

using namespace std;
using namespace sf;


static shared_ptr<TextComponent> backText;
static shared_ptr<Entity> back;

void SettingsScene::Load() {
	
	//Heading
	auto stitle = makeEntity();
	auto settingText = stitle->addComponent<TextComponent>();
	settingText->SetText("Settings HERE");
	stitle->addTag("Settings");
	settingText->getText()->setOrigin(Vector2f(settingText->getText()->getLocalBounds().width / 2.f,
		settingText->getText()->getLocalBounds().height / 2.f));

	settingText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2 - 350.f)));
	settingText->getText()->setCharacterSize(45);
	
	//BackButton for now
	back = makeEntity();
	backText = back->addComponent<TextComponent>();
	backText->SetText("Back");
	back->addTag("Back");
	backText->getText()->setOrigin(Vector2f(backText->getText()->getLocalBounds().width / 2.f,
		backText->getText()->getLocalBounds().height / 2.f));

	backText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2 + 350.f)));
	backText->getText()->setCharacterSize(45);
	
	
	setLoaded(true);
}
void SettingsScene::Update(const double& dt) {
	if (backText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		backText->getText()->setFillColor(sf::Color::Red);

		if (Mouse::isButtonPressed(Mouse::Left())) {
			Engine::ChangeScene(&menu);
		
		}
	}
	else {
		backText->getText()->setFillColor(sf::Color::White);
	}
	
	if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
		Engine::ChangeScene((Scene*)&menu);
		return;
	}
	Scene::Update(dt);
}