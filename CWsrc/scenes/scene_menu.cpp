#include "scene_menu.h"
#include "LevelSystem.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include "SFML/Window/Mouse.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

static shared_ptr<Entity> playButt;
static shared_ptr<Entity> settingsButt;
static shared_ptr<Entity> exitButt;


//Load in buttons
void MenuScene::Load() {
	cout << "Menu Load \n";

	Engine::GetWindow().setView(Engine::GetWindow().getDefaultView());
	// Background 1
	{
		Texture bgTex;
		auto bg = makeEntity();
		if (!bgTex.loadFromFile("res/img/menuBg.png")) {
			std::cerr << "Failed to load spritesheet!" << std::endl;
		}
		Sprite bgSprite;
		bgSprite.setTexture(bgTex);
		auto bgCmp = bg->addComponent<SpriteComponent>();

		bgCmp->setTexure(make_shared<Texture>(bgTex));
		sf::Vector2f targetSize = Engine::user_preferences.video_resolution;
		
		float width = bgCmp->getSprite().getLocalBounds().width;
		float height = bgCmp->getSprite().getLocalBounds().height;
		bgCmp->getSprite().setScale(Vector2f(targetSize.x / width, targetSize.y / height));
	}
	// Background 2
	{
		Texture bgTex;
		auto bg = makeEntity();
		if (!bgTex.loadFromFile("res/img/planets.png")) {
			std::cerr << "Failed to load spritesheet!" << std::endl;
		}
		auto bgCmp = bg->addComponent<SpriteComponent>();
		bgCmp->setTexure(make_shared<Texture>(bgTex));

		sf::Vector2f targetSize = Engine::user_preferences.video_resolution;
		float width = bgCmp->getSprite().getLocalBounds().width;
		float height = bgCmp->getSprite().getLocalBounds().height;
		bgCmp->getSprite().setScale(Vector2f(targetSize.x / width, targetSize.y / height));
	}


	// Music stuff
	if (!this->music.openFromFile("res/audio/menu.wav")) { cout << "Music file not found." << endl; }

	this->music.setLoop(true);
	this->music.setVolume(35); // TODO set as user preference
	this->music.play();

	// Main title
	auto title = makeEntity();
	auto menuText = title->addComponent<TextComponent>();
	menuText->SetText("The Defence of Jola Prime");
	title->addTag("Title");
	menuText->getText()->setOrigin(Vector2f(menuText->getText()->getLocalBounds().width / 2.f,
		menuText->getText()->getLocalBounds().height / 2.f));

	menuText->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.2f));
	menuText->getText()->setCharacterSize(65);
	menuText->getText()->setFillColor(Color(180, 26, 20));
	menuText->getText()->setOutlineColor(Color::Black);
	menuText->getText()->setOutlineThickness(1);

	// Play button
	{
		playButt = makeEntity();
		auto playText = playButt->addComponent<TextComponent>();
		playText->SetText("Play");
		playButt->addTag("Play");
		playText->getText()->setOrigin(Vector2f(playText->getText()->getLocalBounds().width / 2.f,
			playText->getText()->getLocalBounds().height / 2.f));

		playText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2)));
		playText->getText()->setCharacterSize(35);
	}
	//Settings button
	{
		settingsButt = makeEntity();
		auto settingsText = settingsButt->addComponent<TextComponent>();
		settingsText->SetText("Settings");
		settingsButt->addTag("Settings");
		settingsText->getText()->setOrigin(Vector2f(settingsText->getText()->getLocalBounds().width / 2.f,
			settingsText->getText()->getLocalBounds().height / 2.f));

		settingsText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2) + 100.f));
		settingsText->getText()->setCharacterSize(35);
	}
	
		//Exit button
	{
		exitButt = makeEntity();
		auto exitText = exitButt->addComponent<TextComponent>();
		exitText->SetText("Exit");
		exitButt->addTag("Exit");
		exitText->getText()->setOrigin(Vector2f(exitText->getText()->getLocalBounds().width / 2.f,
			exitText->getText()->getLocalBounds().height / 2.f));

		exitText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2) + 200.f));
		exitText->getText()->setCharacterSize(35);
	}



	setLoaded(true);
}

void MenuScene::Update(const double& dt) {
	// cout << "Menu Update "<<dt<<"\n";

	//Exit button
	auto exitEnt = ents.find("Exit")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>exitButton = exitEnt.front();

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
			Engine::ChangeScene(&hub);
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

	//handels window resizing
	sf::Event event;
	while (Engine::GetWindow().pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Resized: {
			sf::FloatRect visibleArea(Vector2f(0, 0),Vector2f(event.size.width, event.size.height));
			Engine::GetWindow().setView(sf::View(visibleArea));
			break;
			}
		}
	}
	

	Scene::Update(dt);
}
