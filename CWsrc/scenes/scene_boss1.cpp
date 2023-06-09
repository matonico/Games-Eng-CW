#include "scene_boss1.h"
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
#include "../components/cmp_boost.h"
#include "../components/cmp_boss.h"
#include "../components/cmp_boss_hp.h"
#include "../components/cmp_boss_shoot.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> hpText;

static shared_ptr<Entity> boss;
static shared_ptr<Entity> bossText;
static shared_ptr<Entity> bossHp;

static shared_ptr<Entity> portal;
static shared_ptr<Entity> pauseMenu;

static sf::View viewLowres(sf::FloatRect(Vector2f(0, -lowGameHeight/2), Vector2f(1920, 1080)));

void Boss1Scene::Load() {
	std::cout << "Boss 1 Load" << endl;
	if (Engine::user_preferences.video_resolution.y != 1080 ) {
		cout << "lowResview"<< endl;
		Engine::GetWindow().setView(viewLowres);
	}

	if (!this->music.openFromFile("res/audio/boss1.wav")) { cout << "Music file not found." << endl; }

	// Playing after the load screen
	this->music.setLoop(true);
	this->music.setVolume(35); // TODO set as user preference


	ls::loadLevelFile("res/levels/boss_1.txt", 40.0f);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
	ls::setOffset(Vector2f(0, ho));

	{
		hpText = makeEntity();
		auto txtCmp = hpText->addComponent<TextComponent>();
		txtCmp->SetText("HP: 50");
		hpText->addTag("HP");
		//dirty resolution
		if (Engine::user_preferences.video_resolution.y != 1080) {
			auto Pixels = Engine::GetWindow().mapCoordsToPixel(txtCmp->getText()->getPosition(), Engine::GetWindow().getDefaultView());
			txtCmp->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(Pixels, viewLowres));
		}
	}
	{
		bossText = makeEntity();
		auto txtCmp = bossText->addComponent<TextComponent>();
		txtCmp->SetText("\tObsidiaani\nThe Molten One");
		txtCmp->getText()->setOutlineColor(Color(25, 25, 25));
		txtCmp->getText()->setFillColor(Color(255, 180, 180));
		txtCmp->getText()->setOutlineThickness(2);
		txtCmp->getText()->setCharacterSize(45);
		txtCmp->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.07f));
		bossText->addTag("BossName");
		//dirty resolution
		if (Engine::user_preferences.video_resolution.y != 1080) {
			auto Pixels = Engine::GetWindow().mapCoordsToPixel(txtCmp->getText()->getPosition(), Engine::GetWindow().getDefaultView());
			txtCmp->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(Pixels, viewLowres));
		}
	}


	// Add colliders to the boost platforms, doing it above player, because player needs a list of boost tiles for boost component
	{
		auto boosts = ls::findTiles(ls::BOOST);
		for (auto w : boosts) {
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(20.f, 20.f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
			e->addTag("boost");
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
		auto hp = player->addComponent<PlayerHPComponent>(true);
		hp->resetHP();
		

		auto phys = player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));

		phys->setRestitution(-1); // not sure if improves movement

		// Shoot component
		player->addComponent<PlayerShootComponent>();

		// Boost component
		player->addComponent<BoostComponent>();

		// Adding checkpoint component so the player can spawn in again after dying
		player->addComponent<CheckpointComponent>();
	}

	// Boss 
	{
		boss = makeEntity();
		boss->addTag("boss");
		boss->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));

		auto s = boss->addComponent<ShapeComponent>();
		s->setShape<RectangleShape>(Vector2f(150.f, 150.f));
		s->getShape().setOrigin(Vector2f(75.f, 75.f));
		s->getShape().setFillColor(Color::Red);

		boss->addComponent<BossPhysicsComponent>(Vector2f(150.0f, 150.f));
		auto bb = boss->addComponent<BossHPComponent>();
		bb->resetHP();
		boss->addComponent<HurtComponent>(110.0f);
		boss->addComponent<BossShootComponent>();
	}
	// Boss hp
	{
		bossHp = makeEntity();
		bossHp->setPosition(boss->getPosition() + Vector2f(0, 50));
		auto shape = bossHp->addComponent<ShapeComponent>();
		shape->setShape<RectangleShape>(Vector2f(200, 10));
		shape->getShape().setFillColor(Color::Green);
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

	// Create a portal but make it invisible
	portal = makeEntity();
	portal->setVisible(false);
	portal->addTag("Portal");

	// Portal styling
	portal->setPosition(ls::getTilePosition(ls::findTiles(ls::CHECKPT)[0]));
	auto s = portal->addComponent<ShapeComponent>();
	s->setShape<CircleShape>(16, 4);
	s->getShape().setOrigin(Vector2f(16, 16));
	s->getShape().setOutlineThickness(3);
	s->getShape().setFillColor(sf::Color(128, 196, 193));
	s->getShape().setOutlineColor(sf::Color(5, 213, 245));

	// Pause menu
	pauseMenu = makeEntity();
	pauseMenu->setPosition(Vcast<float>(Engine::getWindowSize())* Vector2f(0.1f, 0.1f));
	pauseMenu->setVisible(false);
	auto shape = pauseMenu->addComponent<ShapeComponent>();

	shape->setShape<RectangleShape>(Vcast<float>(Engine::getWindowSize())* Vector2f(0.8f, 0.8f));
	shape->getShape().setFillColor(Color{ 112,128,144 });

	auto text = pauseMenu->addComponent<TextComponent>("Paused");
	text->getText()->setPosition(Vcast<float>(Engine::getWindowSize())* Vector2f(0.4f, 0.2f));
	text->getText()->setCharacterSize(45);

	auto exitToMenu = makeEntity();
	exitToMenu->setVisible(false);
	exitToMenu->addTag("ExitToMenu");
	auto exitToMenuText = exitToMenu->addComponent<TextComponent>("Exit to Main Menu");
	exitToMenuText->getText()->setPosition(Vcast<float>(Engine::getWindowSize())* Vector2f(0.4f, 0.4f));
	exitToMenuText->getText()->setCharacterSize(35);

	auto exitGame = makeEntity();
	exitGame->setVisible(false);
	exitGame->addTag("ExitGame");
	auto exitGameText = exitGame->addComponent<TextComponent>("Exit to Desktop");
	exitGameText->getText()->setPosition(Vcast<float>(Engine::getWindowSize())* Vector2f(0.4f, 0.5f));
	exitGameText->getText()->setCharacterSize(35);


	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << "Boss 1 Load Done" << endl;

	this->music.play(); 
	setLoaded(true);
}

void Boss1Scene::UnLoad() {
	std::cout << "Boss 1 Unload" << endl;
	boss.reset();
	player.reset();
	ls::unload();
	this->music.stop();
	Scene::UnLoad();
}

void Boss1Scene::Update(const double& dt) {
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
			ents.find("Portal")[0]->setVisible(false); // make sure portal isn't made visible when pausing
			pauseMenu->setVisible(false);
			ents.find("ExitGame")[0]->setVisible(false);
			ents.find("ExitToMenu")[0]->setVisible(false);

			pauseTime = 0.5f;
		}

		//Exit to menu button
		auto exitToMenuEnt = ents.find("ExitToMenu")[0];
		auto exitToMenuTextCmp = exitToMenuEnt->GetCompatibleComponent<TextComponent>()[0];

		if (exitToMenuTextCmp->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
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

		if (exitGameTextCmp->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow()))) && exitGameEnt->isVisible()) {
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


		pauseTime = 1.0f;
	}
	// Check to see if we should reset level. Doing it in the main scene update because doing it in the HP component causes scene to load twice somtimes
	if (player->get_components<PlayerHPComponent>()[0]->getHP()<=0) {

		player->get_components<PlayerHPComponent>()[0]->resetHP();
		boss->get_components<BossHPComponent>()[0]->resetHP();

		Engine::ChangeScene((Scene*)&boss1);
		return;
	}

	static float spikeTime = 0.0f;
	spikeTime -= dt;

	// Player HP stuff
	{
		string hp = to_string(player->get_components<PlayerHPComponent>()[0]->getHP());
		string deaths = to_string(player->get_components<PlayerHPComponent>()[0]->getDeaths());
		hpText->get_components<TextComponent>()[0]->SetText("HP: " + hp + "\t\t" + "Deaths: " + deaths);
	}
	// Boss hp stuff

	if (boss->isAlive())
	{
		bossHp->setPosition(boss->getPosition() + Vector2f(-95, -120));
		int bossHpValue = boss->get_components<BossHPComponent>()[0]->getBossHP();
		bossHp->GetCompatibleComponent<ShapeComponent>()[0]->setShape<RectangleShape>(Vector2f(bossHpValue, 10));
		bossHp->GetCompatibleComponent<ShapeComponent>()[0]->getShape().setFillColor(Color::Green);
	}

	// Set portal to visible if boss is dead and portal isn't currently visible
	if (!boss->isAlive() && !portal->isVisible())
	{
		portal->setVisible(true);
	}
	
	// End conditions
	if (portal->isVisible() && (length(player->getPosition() - portal->getPosition()) < 25.0f))
	{
		// This will tp to ending cutscene?
		Engine::ChangeScene((Scene*)&menu);
		return;
	}
	pauseTime -= dt;

	Scene::Update(dt);
}

void Boss1Scene::Render() {
	
	ls::render(Engine::GetWindow(),3);
	Scene::Render();
}
