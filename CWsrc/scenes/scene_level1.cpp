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

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> hpText;

void Level1Scene::Load() {
	cout << " Scene 1 Load" << endl;

	menu.music.stop(); // Stop the main menu music
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
	hpText->addTag("HP");

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

	Scene::Update(dt);
}

void Level1Scene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}
