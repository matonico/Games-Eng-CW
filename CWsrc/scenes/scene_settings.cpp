#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_player_physics.h"
#include <SFML/Window/Keyboard.hpp>
#include "SFML/Window/Mouse.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;
using namespace sf;


//Back button
static shared_ptr<Entity> back;
//Handle changin
static shared_ptr<Entity> jump;
static shared_ptr<Entity> jumpPrompt;
static shared_ptr<Entity> jumpConfirm;
static bool jumpPressed = false;

static shared_ptr<Entity> leftkey;
static shared_ptr<Entity> leftPrompt;
static shared_ptr<Entity> leftConfirm;
static bool leftPressed = false;

static shared_ptr<Entity> rightkey;
static shared_ptr<Entity> rightPrompt;
static shared_ptr<Entity> rightConfirm;
static bool rightPressed = false;

static shared_ptr<Entity> highRes;
static shared_ptr<Entity> lowRes;
static shared_ptr<Entity> fullScreenMode;
static shared_ptr<Entity> windowMode;

Event event;

void SettingsScene::Load() {

	//Heading
	{
		auto stitle = makeEntity();
		auto settingText = stitle->addComponent<TextComponent>();
		settingText->SetText("Settings");
		stitle->addTag("Settings");
		settingText->getText()->setOrigin(Vector2f(settingText->getText()->getLocalBounds().width / 2.f,
			settingText->getText()->getLocalBounds().height / 2.f));

		settingText->getText()->setPosition(Vector2f(((Engine::GetWindow().getView().getSize().x / 2) -40.f), (Engine::GetWindow().getView().getSize().y / 2 - 350.f)));
		settingText->getText()->setCharacterSize(45);
	}
	//Jump
	{
		jump = makeEntity();
		auto jumpText = jump->addComponent<TextComponent>();
		jumpText->SetText("Jump");
		jump->addTag("Jumpkey");
		jumpText->getText()->setOrigin(Vector2f(jumpText->getText()->getLocalBounds().width / 2.f,
			jumpText->getText()->getLocalBounds().height / 2.f));

		jumpText->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2), (Engine::GetWindow().getView().getSize().y / 2 - 200.f)));
		jumpText->getText()->setCharacterSize(25);

		jumpPrompt = makeEntity();
		auto jumpTextPrompt = jumpPrompt->addComponent<TextComponent>();
		jumpTextPrompt->SetText("Press key");
		jumpPrompt->addTag("setjump");
		jumpTextPrompt->getText()->setOrigin(Vector2f(jumpTextPrompt->getText()->getLocalBounds().width / 2.f,
			jumpTextPrompt->getText()->getLocalBounds().height / 2.f));

		jumpTextPrompt->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2), (Engine::GetWindow().getView().getSize().y / 2 - 200.f)));
		jumpTextPrompt->getText()->setCharacterSize(25);
		jumpPrompt->setVisible(false);

		jumpConfirm = makeEntity();
		auto jumpTextConfirm = jumpConfirm->addComponent<TextComponent>();
		jumpTextConfirm->SetText("Jump key set");
		jumpConfirm->addTag("JumpConfirm");
		jumpTextConfirm->getText()->setOrigin(Vector2f(jumpTextConfirm->getText()->getLocalBounds().width / 2.f,
			jumpTextConfirm->getText()->getLocalBounds().height / 2.f));
		jumpTextConfirm->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2) + 200.f, (Engine::GetWindow().getView().getSize().y / 2 - 200.f)));
		jumpTextConfirm->getText()->setCharacterSize(25);
		jumpConfirm->setVisible(false);
	}
	//Left
	{
		leftkey = makeEntity();
		auto leftText = leftkey->addComponent<TextComponent>();
		leftText->SetText("Left");
		leftkey->addTag("Leftkey");
		leftText->getText()->setOrigin(Vector2f(leftText->getText()->getLocalBounds().width / 2.f,
			leftText->getText()->getLocalBounds().height / 2.f));

		leftText->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2), (Engine::GetWindow().getView().getSize().y / 2 - 150.f)));
		leftText->getText()->setCharacterSize(25);

		leftPrompt = makeEntity();
		auto leftTextPrompt = leftPrompt->addComponent<TextComponent>();
		leftTextPrompt->SetText("Press key");
		leftPrompt->addTag("setLeft");
		leftTextPrompt->getText()->setOrigin(Vector2f(leftTextPrompt->getText()->getLocalBounds().width / 2.f,
			leftTextPrompt->getText()->getLocalBounds().height / 2.f));

		leftTextPrompt->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2), (Engine::GetWindow().getView().getSize().y / 2 - 150.f)));
		leftTextPrompt->getText()->setCharacterSize(25);
		leftPrompt->setVisible(false);

		leftConfirm = makeEntity();
		auto leftTextConfirm = leftConfirm->addComponent<TextComponent>();
		leftTextConfirm->SetText("Left key set");
		leftConfirm->addTag("LeftConfirm");
		leftTextConfirm->getText()->setOrigin(Vector2f(leftTextConfirm->getText()->getLocalBounds().width / 2.f,
			leftTextConfirm->getText()->getLocalBounds().height / 2.f));
		leftTextConfirm->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2) + 200.f, (Engine::GetWindow().getView().getSize().y / 2 - 150.f)));
		leftTextConfirm->getText()->setCharacterSize(25);
		leftConfirm->setVisible(false);

	}
	//Right
	{
		rightkey = makeEntity();
		auto rightText = rightkey->addComponent<TextComponent>();
		rightText->SetText("Right");
		rightkey->addTag("Rightkey");
		rightText->getText()->setOrigin(Vector2f(rightText->getText()->getLocalBounds().width / 2.f,
			rightText->getText()->getLocalBounds().height / 2.f));

		rightText->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2), (Engine::GetWindow().getView().getSize().y / 2 - 100.f)));
		rightText->getText()->setCharacterSize(25);

		rightPrompt = makeEntity();
		auto rightTextPrompt = rightPrompt->addComponent<TextComponent>();
		rightTextPrompt->SetText("Press key");
		rightPrompt->addTag("setRight");
		rightTextPrompt->getText()->setOrigin(Vector2f(rightTextPrompt->getText()->getLocalBounds().width / 2.f,
			rightTextPrompt->getText()->getLocalBounds().height / 2.f));

		rightTextPrompt->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2), (Engine::GetWindow().getView().getSize().y / 2 - 100.f)));
		rightTextPrompt->getText()->setCharacterSize(25);
		rightPrompt->setVisible(false);

		rightConfirm = makeEntity();
		auto rightTextConfirm = rightConfirm->addComponent<TextComponent>();
		rightTextConfirm->SetText("Right key set");
		rightConfirm->addTag("RightConfirm");
		rightTextConfirm->getText()->setOrigin(Vector2f(rightTextConfirm->getText()->getLocalBounds().width / 2.f,
			rightTextConfirm->getText()->getLocalBounds().height / 2.f));
		rightTextConfirm->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2) + 200.f, (Engine::GetWindow().getView().getSize().y / 2 - 100.f)));
		rightTextConfirm->getText()->setCharacterSize(25);
		rightConfirm->setVisible(false);

	}
	
	//BackButton
	{
		
		back = makeEntity();
		auto backText = back->addComponent<TextComponent>();
		backText->SetText("Back");
		back->addTag("Back");
		backText->getText()->setOrigin(Vector2f(backText->getText()->getLocalBounds().width / 2.f,
			backText->getText()->getLocalBounds().height / 2.f));

		backText->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2), (Engine::GetWindow().getView().getSize().y / 2 + 300.f)));
		backText->getText()->setCharacterSize(25);
	}

	//HighResButton
	{
		highRes = makeEntity();
		auto highResText = highRes->addComponent<TextComponent>();
		highResText->SetText("1920 x 1080");
		highRes->addTag("highRes");
		highResText->getText()->setOrigin(Vector2f(highResText->getText()->getLocalBounds().width / 2.f,
			highResText->getText()->getLocalBounds().height / 2.f));

		highResText->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2 - 350.f), (Engine::GetWindow().getView().getSize().y / 2 + 100.f)));
		highResText->getText()->setCharacterSize(35);

	}
	//LowResButton
	{
		lowRes = makeEntity();
		auto lowResText = lowRes->addComponent<TextComponent>();
		lowResText->SetText("1280 x 720");
		lowRes->addTag("lowRes");
		lowResText->getText()->setOrigin(Vector2f(lowResText->getText()->getLocalBounds().width / 2.f,
			lowResText->getText()->getLocalBounds().height / 2.f));

		lowResText->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2 - 350.f), (Engine::GetWindow().getView().getSize().y / 2 )));
		lowResText->getText()->setCharacterSize(35);
	}
	//Fullscreem
	{
		fullScreenMode = makeEntity();
		auto fullScreenModeText = fullScreenMode->addComponent<TextComponent>();
		fullScreenModeText->SetText("Full Screen");
		fullScreenMode->addTag("fullScreenMode");
		fullScreenModeText->getText()->setOrigin(Vector2f(fullScreenModeText->getText()->getLocalBounds().width / 2.f,
			fullScreenModeText->getText()->getLocalBounds().height / 2.f));

		fullScreenModeText->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2 + 350.f), (Engine::GetWindow().getView().getSize().y / 2)));
		fullScreenModeText->getText()->setCharacterSize(35);

	}
	//Windowed
	{  
		windowMode = makeEntity();
		auto windowModeText = windowMode->addComponent<TextComponent>();
		windowModeText->SetText("Window Mode");
		windowMode->addTag("windowMode");
		windowModeText->getText()->setOrigin(Vector2f(windowModeText->getText()->getLocalBounds().width / 2.f,
			windowModeText->getText()->getLocalBounds().height / 2.f));

		windowModeText->getText()->setPosition(Vector2f((Engine::GetWindow().getView().getSize().x / 2 + 350.f), (Engine::GetWindow().getView().getSize().y / 2 + 100.f)));
		windowModeText->getText()->setCharacterSize(35);

	}

	setLoaded(true);
}

void SettingsScene::Update(const double& dt) {
	
	//Back button
	auto backEnt = ents.find("Back")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>backText = backEnt.front();
	if (backText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		backText->getText()->setFillColor(sf::Color::Red);

		if (Mouse::isButtonPressed(Mouse::Left())) {
			
			fstream preferencesFile("res/user_preferences.txt");
			if (!preferencesFile.is_open()) {
				cout << "File failed to open";
				return;
			}
			
			preferencesFile << Engine::user_preferences.fullscreen <<"," << Engine::user_preferences.video_resolution.x << "," << Engine::user_preferences.video_resolution.y<<",";
			preferencesFile.close();
			
			unsigned int width = Engine::user_preferences.video_resolution.x;
			unsigned int height = Engine::user_preferences.video_resolution.y;
			Engine::GetWindow().create(VideoMode({ width,height }),"gamename",Engine::user_preferences.fullscreen);
			
			Engine::ChangeScene((Scene*)&menu);
			return;
		}
	}
	else {
		backText->getText()->setFillColor(sf::Color::White);
	}

	//Jump button
	auto jumpEnt = ents.find("Jumpkey")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>jumpText = jumpEnt.front();
	
		if (jumpText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
			jumpText->getText()->setFillColor(sf::Color::Red);
				if (Mouse::isButtonPressed(Mouse::Left())) {
					jumpConfirm->setVisible(false);
					jump->setVisible(false);
					jumpPrompt->setVisible(true);
					jumpPressed = true;
					
				}
				
				
		}
		else {
			jumpText->getText()->setFillColor(sf::Color::White);

		}
	
	//Left button
	auto leftEnt = ents.find("Leftkey")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>leftText = leftEnt.front();
		
	if (leftText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		leftText->getText()->setFillColor(sf::Color::Red);
		if (Mouse::isButtonPressed(Mouse::Left())) {
			leftConfirm->setVisible(false);
			leftkey->setVisible(false);
			leftPrompt->setVisible(true);
			leftPressed = true;
			
		}
		
	}
	else {
		leftText->getText()->setFillColor(sf::Color::White);
	}

	//Right button
	auto rightEnt = ents.find("Rightkey")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>rightText = rightEnt.front();

	if (rightText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		rightText->getText()->setFillColor(sf::Color::Red);
		
		if (Mouse::isButtonPressed(Mouse::Left())) {
			rightConfirm->setVisible(false);
			rightkey->setVisible(false);
			rightPrompt->setVisible(true);
			rightPressed = true;
			
		}
	}
	else {
		rightText->getText()->setFillColor(sf::Color::White);
	}

	//highRes button
	
	
	auto highResEnt = ents.find("highRes")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>highResText = highResEnt.front();
	if (highResText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		highResText->getText()->setFillColor(sf::Color::Red);

		if (Mouse::isButtonPressed(Mouse::Left())) {
			Engine::user_preferences.video_resolution.x = 1920;
			Engine::user_preferences.video_resolution.y = 1080;
			highResText->getText()->setFillColor(sf::Color::Green);
			return;
		}
	}
	else {
		highResText->getText()->setFillColor(sf::Color::White);
	}
	
	//lowRes buttom
	
	auto lowResEnt = ents.find("lowRes")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>lowResText = lowResEnt.front();
	if (lowResText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		lowResText->getText()->setFillColor(sf::Color::Red);

		if (Mouse::isButtonPressed(Mouse::Left())) {
			Engine::user_preferences.video_resolution.x = 1280;
			Engine::user_preferences.video_resolution.y = 720;
			lowResText->getText()->setFillColor(sf::Color::Green);
			return;
		}
	}
	else {
		lowResText->getText()->setFillColor(sf::Color::White);
	}
	
	//Fullscreen
	
	auto fullScreenEnt = ents.find("fullScreenMode")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>fullScreenText = fullScreenEnt.front();
	if (fullScreenText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		fullScreenText->getText()->setFillColor(sf::Color::Red);

		if (Mouse::isButtonPressed(Mouse::Left())) {
			Engine::user_preferences.fullscreen = 8;
			fullScreenText->getText()->setFillColor(sf::Color::Green);
			return;
		}
	}
	else {
		fullScreenText->getText()->setFillColor(sf::Color::White);
	}
	
	//Windowed

	auto windowModeEnt = ents.find("windowMode")[0]->GetCompatibleComponent<TextComponent>();
	shared_ptr<TextComponent>windowModeText = windowModeEnt.front();
	if (windowModeText->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		windowModeText->getText()->setFillColor(sf::Color::Red);

		if (Mouse::isButtonPressed(Mouse::Left())) {
			Engine::user_preferences.fullscreen = 7;
			windowModeText->getText()->setFillColor(sf::Color::Green);
			return;
		}
	}
	else {
		windowModeText->getText()->setFillColor(sf::Color::White);
	}

		//Handle channgeing inputs
		if (jumpPressed) {
			while (Engine::GetWindow().pollEvent(event)) {
				if (event.type == Event::KeyPressed) {
					sf::Keyboard::Key keyPressed = event.key.code;
					Engine::user_preferences.keyboard.UP = keyPressed;
					cout << "done";
					jumpPrompt->setVisible(false);
					jumpConfirm->setVisible(true);
					jumpPressed = false;
					jump->setVisible(true);
				}
			}
		}
		if (leftPressed) {
			while (Engine::GetWindow().pollEvent(event)) {
				if (event.type == Event::KeyPressed) {
					sf::Keyboard::Key keyPressed = event.key.code;
					Engine::user_preferences.keyboard.LEFT = keyPressed;
					cout << "done";
					leftPrompt->setVisible(false);
					leftConfirm->setVisible(true);
					leftPressed = false;
					leftkey->setVisible(true);
				}
			}

		}
		if (rightPressed) {
			while (Engine::GetWindow().pollEvent(event)) {
				if (event.type == Event::KeyPressed) {
					sf::Keyboard::Key keyPressed = event.key.code;
					Engine::user_preferences.keyboard.RIGHT = keyPressed;
					cout << "done";
					rightPrompt->setVisible(false);
					rightConfirm->setVisible(true);
					rightPressed = false;
					rightkey->setVisible(true);
				}
			}
		}
	
	Scene::Update(dt);
}



