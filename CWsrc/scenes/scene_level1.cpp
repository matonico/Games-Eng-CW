#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_shoot.h"
#include "../components/cmp_activate_btn.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_hp.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_lava.h"
#include "../components/cmp_checkpoint.h"
#include "../components/cmp_lifetime.h"
#include "../game.h"
#include "SFML/Audio.hpp"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "user_preferances.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> hpText;
static shared_ptr<Entity> pauseMenu;
static sf::View viewLowres(sf::FloatRect(Vector2f(0,  ( -(lowGameHeight)/2)), Vector2f(1920, 1080)));

void Level1Scene::Load() {
	cout << " Scene 1 Load" << endl;
	

	_pause = false; // Game isn't paused on load

	if (!this->music.openFromFile("res/audio/level1.wav")) { cout << "Music file not found." << endl; }

	// Playing after the load screen
	this->music.setLoop(true);
	this->music.setVolume(35); // TODO set as user preference

	ls::loadLevelFile("res/levels/level_1.txt", 40.0f);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
	ls::setOffset(Vector2f(0, ho));

	hpText = makeEntity();
	auto txtCmp = hpText->addComponent<TextComponent>();
	txtCmp->SetText("HP: 50");
	txtCmp->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.0f, 0.0f));
	hpText->addTag("HP");
	
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto Pixels = Engine::GetWindow().mapCoordsToPixel(txtCmp->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		txtCmp->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(Pixels, viewLowres));
	}

	// Create checkpoints
	{
		for (int i = 0; i < 3; i++)
		{
			auto checkpoint = makeEntity();
			checkpoint->addTag("checkpoint");
			checkpoint->setPosition(ls::getTilePosition(ls::findTiles(ls::CHECKPT)[i]));
			auto shape = checkpoint->addComponent<ShapeComponent>();
			shape->setShape<CircleShape>(16, 4);
			shape->getShape().setOrigin(Vector2f(16, 16));
			shape->getShape().setOutlineThickness(3);
			shape->getShape().setFillColor(sf::Color(128, 196, 193));
			shape->getShape().setOutlineColor(sf::Color(5, 213, 245));


		}
	}

	// Create player
	{
		player = makeEntity();
		player->addTag("player");
		player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		auto s = player->addComponent<ShapeComponent>();
		s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
		s->getShape().setFillColor(Color::Magenta);
		s->getShape().setOrigin(Vector2f(10.f, 15.f));
		auto hp = player->addComponent<PlayerHPComponent>(false);
		hp->resetHP();
		hp->setDeaths(0);

		auto phys = player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));

		phys->setRestitution(-1); // not sure if improves movement



		player->addComponent<PlayerShootComponent>();

		// Checkpoint component
		player->addComponent<CheckpointComponent>();
	}




	// Create Enemy turret
	{
		auto enemy = makeEntity();
		enemy->addTag("enemy");
		enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]) +
			Vector2f(0, 24));
		auto h = enemy->addComponent<HurtComponent>(30.0f);
		auto s = enemy->addComponent<ShapeComponent>();

		s->setShape<sf::CircleShape>(16.0f);
		s->getShape().setFillColor(Color::Green);
		s->getShape().setOrigin(Vector2f(16.0, 16.0f));
		enemy->addComponent<EnemyTurretComponent>();

	}
	// The blade enemy at the bottom of level
	{
		auto enemy = makeEntity();
		enemy->addTag("spike");
		enemy->addTag("blade");
		enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[2]) +
			Vector2f(0, 24));
		auto h = enemy->addComponent<HurtComponent>(30.0f);

		auto s = enemy->addComponent<ShapeComponent>();
		s->setShape<sf::CircleShape>(16.0f, 8);
		s->getShape().setOutlineThickness(6);
		s->getShape().setFillColor(Color(44, 10, 89));
		s->getShape().setOutlineColor(Color(224, 27, 76));
		s->getShape().setOrigin(Vector2f(16.0, 16.0f));

		enemy->addComponent<EnemyAIComponent>(true); // _spin = true
	}

	// The final turret
	{
		auto enemy = makeEntity();
		enemy->addTag("enemy");
		enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[1]) +
			Vector2f(0, 24));

		auto h = enemy->addComponent<HurtComponent>(30.0f);
		auto s = enemy->addComponent<ShapeComponent>();
		s->setShape<sf::CircleShape>(16.0f);
		s->getShape().setFillColor(Color::Green);
		s->getShape().setOrigin(Vector2f(16.0, 16.0f));
		enemy->addComponent<EnemyTurretComponent>();

	}
	// Create button for player to shoot
	{
		auto button = makeEntity();
		button->addTag("button");
		button->setPosition(ls::getTilePosition(ls::findTiles(ls::LVL1BTN)[0]) + Vector2f(16, 24));
		auto s = button->addComponent<ShapeComponent>();
		s->setShape<sf::CircleShape>(8.0f);
		s->getShape().setFillColor(Color::Magenta);
		s->getShape().setOrigin(Vector2f(8.0, 8.0f));

	}

	// Add physics colliders to level tiles.
	{
		auto walls = ls::findTiles(ls::WALL);
		for (auto w : walls) {
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(20.f, 20.f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
			e->addTag("wall");
		}
	}

	// also add collisions to platform tiles
	{
		auto plats = ls::findTiles(ls::LVL1PLAT);
		for (auto p : plats) {
			auto pos = ls::getTilePosition(p);
			pos += Vector2f(20.f, 20.f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
			e->addTag("wall");
		}
	}

	// lava hurts
	{
		auto lavaTiles = ls::findTiles(ls::LVL1LAVA);
		for (auto lava : lavaTiles) {
			auto pos = ls::getTilePosition(lava);
			pos += Vector2f(20.f, 20.f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<LavaComponent>();
		}
	}
	// Pause menu
	pauseMenu = makeEntity();

	//Resolution scaling
	/*if (Engine::user_preferences.video_resolution.y != 1080) {
		pauseMenu->setPosition(Vector2f(Engine::GetWindow().getView().getCenter()));
	}
	else {
		pauseMenu->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.1f, 0.1f));
	}*/

	pauseMenu->setPosition(Vcast<float>(Engine::getWindowSize())* Vector2f(0.1f, 0.1f));
	pauseMenu->setVisible(false);
	auto shape = pauseMenu->addComponent<ShapeComponent>();

	shape->setShape<RectangleShape>(Vcast<float>(Engine::getWindowSize())* Vector2f(0.8f, 0.8f));
	shape->getShape().setFillColor(Color{ 112,128,144 });


	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto pausemenuPixels = Engine::GetWindow().mapCoordsToPixel(pauseMenu->getPosition(), Engine::GetWindow().getDefaultView());
		pauseMenu->setPosition(Vcast<float>(Engine::GetWindow().mapPixelToCoords(pausemenuPixels, viewLowres)) + Vector2f(300.f, 0.f));


		/*
		shape->setShape<RectangleShape>(Vcast<float>(Engine::getWindowSize())* Vector2f(0.9f, 0.9f));
		shape->getShape().setFillColor(Color{ 112,128,144 });
		shape->getShape().setOrigin(Vector2f(shape->getShape().getLocalBounds().width / 2.0f,
											 shape->getShape().getLocalBounds().height / 2.0f));
		pauseMenu->setPosition(Vector2f(viewLowres.getCenter()));
		*/
	}
	//Resolution scaling shit
	/*if (Engine::user_preferences.video_resolution.y != 1080) {
		shape->getShape().setOrigin(Vector2f(shape->getShape().getLocalBounds().width/2.0f,
											 shape->getShape().getLocalBounds().height/2.0f));
	}*/

	auto text = pauseMenu->addComponent<TextComponent>("Paused");

	text->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.2f));
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto pausemenuPixels = Engine::GetWindow().mapCoordsToPixel(text->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		text->getText()->setPosition(Vcast<float>(Engine::GetWindow().mapPixelToCoords(pausemenuPixels, viewLowres)));
	}

	text->getText()->setCharacterSize(45);
	auto exitToMenu = makeEntity();
	exitToMenu->setVisible(false);
	exitToMenu->addTag("ExitToMenu");
	auto exitToMenuText = exitToMenu->addComponent<TextComponent>("Exit to Main Menu");
	exitToMenuText->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.4f));

	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto pausemenuPixels = Engine::GetWindow().mapCoordsToPixel(exitToMenuText->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		exitToMenuText->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(pausemenuPixels, viewLowres));
	}
	exitToMenuText->getText()->setCharacterSize(35);

	auto exitGame = makeEntity();
	exitGame->setVisible(false);
	exitGame->addTag("ExitGame");
	auto exitGameText = exitGame->addComponent<TextComponent>("Exit to Desktop");
	exitGameText->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.5f));

	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto pausemenuPixels = Engine::GetWindow().mapCoordsToPixel(exitGameText->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		exitGameText->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(pausemenuPixels, viewLowres));
	}
	exitGameText->getText()->setCharacterSize(35);


	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << " Scene 1 Load Done" << endl;
	this->music.play();
	setLoaded(true);
}

void Level1Scene::UnLoad() {
	cout << "Scene 1 Unload" << endl;
	player.reset();
	ls::unload();
	this->music.stop();
	Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {
	//Resolution
	if (Engine::user_preferences.video_resolution.y != 1080) {
		Engine::GetWindow().setView(viewLowres);
	}
	
	// Handling Pausing
	static float pauseTime = 0.0f;
	if (_pause) // Execute this code if the game is paused
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape) && pauseTime <= 0.0f)
		{
			_pause = !_pause;
			for (auto e : ents.list)
			{
				e->setVisible(true);
			}
			pauseMenu->setVisible(false);
			ents.find("ExitGame")[0]->setVisible(false);
			ents.find("ExitToMenu")[0]->setVisible(false);

			pauseTime = 0.5f;
		}

		//Exit menu to scale with view
		sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
		//Exit to menu button
		auto exitToMenuEnt = ents.find("ExitToMenu")[0];
		auto exitToMenuTextCmp = exitToMenuEnt->GetCompatibleComponent<TextComponent>()[0];

		//if (exitToMenuTextCmp->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		if (exitToMenuTextCmp->getText()->getGlobalBounds().contains(Engine::GetWindow().mapPixelToCoords(pixelPos))) {
			exitToMenuTextCmp->getText()->setFillColor(sf::Color::Red);
			if (Mouse::isButtonPressed(Mouse::Left())) {
				Engine::ChangeScene(&menu);
				return;
			}
		}
		else {
			exitToMenuTextCmp->getText()->setFillColor(sf::Color::White);
		}

		//Exit game button

		auto exitGameEnt = ents.find("ExitGame")[0];
		auto exitGameTextCmp = exitGameEnt->GetCompatibleComponent<TextComponent>()[0];

		//if (exitGameTextCmp->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow()))) && exitGameEnt->isVisible()) {
		if (exitGameTextCmp->getText()->getGlobalBounds().contains(Engine::GetWindow().mapPixelToCoords(pixelPos)) && exitGameEnt->isVisible()) {
			exitGameTextCmp->getText()->setFillColor(sf::Color::Red);
			if (Mouse::isButtonPressed(Mouse::Left())) {
				Engine::GetWindow().close();
				return;
			}
		}
		else {
			exitGameTextCmp->getText()->setFillColor(sf::Color::White);
		}

		pauseTime -= dt;
		return;
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape) && pauseTime <= 0.0f) // Pause the game
	{
		_pause = !_pause;
		for (auto e : ents.list)
		{
			e->setVisible(false);
		}
		pauseMenu->setVisible(true);
		ents.find("ExitGame")[0]->setVisible(true);
		ents.find("ExitToMenu")[0]->setVisible(true);
		

		pauseTime = 0.5f;
	}

	// If player gets to the portal, change scene to boss1
	if (ls::getTileAt(player->getPosition()) == ls::END) {
		Engine::ChangeScene((Scene*)&boss1);
		return;
	}
	else if (!player->isAlive()) {
		Engine::ChangeScene((Scene*)&level1);
		return;
	}
	static float spikeTime = 0.0f;
	spikeTime -= dt;

	if (spikeTime <= 0.f) {
		spikeTime = 2.f;
		auto spike = makeEntity();
		spike->addTag("spike");

		// This component adds a lifetime to the spike, so that they get deleted and don't waste memory, takes in a float in seconds
		spike->addComponent<LifetimeComponent>(2.0f);

		sf::ConvexShape polygon;
		polygon.setPointCount(3);
		polygon.setPoint(0, sf::Vector2f(0, 0));
		polygon.setPoint(1, sf::Vector2f(10, 0));
		polygon.setPoint(2, sf::Vector2f(5, 30));
		polygon.setOrigin(Vector2f(5, 15));

		spike->setPosition(ls::getTilePosition(ls::findTiles('r')[0]) +
			Vector2f(16, 0));
		auto s = spike->addComponent<ShapeComponent>();
		s->setShape<ConvexShape>(polygon);

		auto p = spike->addComponent<PhysicsComponent>(true, Vector2f(5, 15));
		auto h = spike->addComponent<HurtComponent>(30.0f);

		p->setMass(1000000000.f);

	}
	string hp = to_string(player->get_components<PlayerHPComponent>()[0]->getHP());
	string deaths = to_string(player->get_components<PlayerHPComponent>()[0]->getDeaths());


	hpText->get_components<TextComponent>()[0]->SetText("HP: " + hp + "\t\t" + "Deaths: " + deaths);

	pauseTime -= dt;


	Scene::Update(dt);
}

void Level1Scene::Render() {

	ls::render(Engine::GetWindow(),2);
	Scene::Render();
}
