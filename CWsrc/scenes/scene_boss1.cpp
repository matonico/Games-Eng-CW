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
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> hpText;

void Boss1Scene::Load() {
	cout << "Boss 1 Load" << endl;

	ls::loadLevelFile("res/levels/boss_1.txt", 40.0f);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
	ls::setOffset(Vector2f(0, ho));

	hpText = makeEntity();
	auto txtCmp = hpText->addComponent<TextComponent>();
	txtCmp->SetText("HP: 50");
	hpText->addTag("HP");

	// Create player
	{
		player = makeEntity();
		player->addTag("player");
		player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		auto s = player->addComponent<ShapeComponent>();
		s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
		s->getShape().setFillColor(Color::Magenta);
		s->getShape().setOrigin(Vector2f(10.f, 15.f));
		auto hp = player->addComponent<PlayerHPComponent>();

		auto phys = player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));

		phys->setRestitution(-1); // not sure if improves movement

		// Shoot component
		player->addComponent<PlayerShootComponent>();

		// Checkpoint component
		player->addComponent<CheckpointComponent>();
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


	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << " Boss 1 Load Done" << endl;

	setLoaded(true);
}

void Boss1Scene::UnLoad() {
	cout << "Boss 1 Unload" << endl;
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void Boss1Scene::Update(const double& dt) {



	// Player HP stuff
	{
		string hp = to_string(player->get_components<PlayerHPComponent>()[0]->getHP());
		string deaths = to_string(player->get_components<PlayerHPComponent>()[0]->getDeaths());
		hpText->get_components<TextComponent>()[0]->SetText("HP: " + hp + "\t\t" + "Deaths: " + deaths);
	}


	Scene::Update(dt);
}

void Boss1Scene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}
