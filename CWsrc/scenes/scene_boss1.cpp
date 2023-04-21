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

void Boss1Scene::Load() {
	std::cout << "Boss 1 Load" << endl;

	ls::loadLevelFile("res/levels/boss_1.txt", 40.0f);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
	ls::setOffset(Vector2f(0, ho));

	{
		hpText = makeEntity();
		auto txtCmp = hpText->addComponent<TextComponent>();
		txtCmp->SetText("HP: 50");
		hpText->addTag("HP");
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
		auto hp = player->addComponent<PlayerHPComponent>();

		auto phys = player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));

		phys->setRestitution(-1); // not sure if improves movement

		// Shoot component
		player->addComponent<PlayerShootComponent>();

		// Boost component
		player->addComponent<BoostComponent>();
	}

	// Boss 
	{
		Texture spritesheet;
		boss = makeEntity();
		boss->addTag("boss");
		boss->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));

		auto s = boss->addComponent<ShapeComponent>();
		s->setShape<RectangleShape>(Vector2f(150.f, 150.f));
		s->getShape().setOrigin(Vector2f(75.f, 75.f));
		s->getShape().setFillColor(Color::Red);

		boss->addComponent<BossPhysicsComponent>(Vector2f(150.0f, 150.f));
		boss->addComponent<BossHPComponent>();
	}
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

	portal = makeEntity();
	portal->setVisible(false);

	portal->setPosition(ls::getTilePosition(ls::findTiles(ls::CHECKPT)[0]));
	auto s = portal->addComponent<ShapeComponent>();
	s->setShape<CircleShape>(16, 4);
	s->getShape().setOrigin(Vector2f(16, 16));
	s->getShape().setOutlineThickness(3);
	s->getShape().setFillColor(sf::Color(128, 196, 193));
	s->getShape().setOutlineColor(sf::Color(5, 213, 245));


	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << " Boss 1 Load Done" << endl;

	setLoaded(true);
}

void Boss1Scene::UnLoad() {
	std::cout << "Boss 1 Unload" << endl;
	player.reset();
	boss.reset();
	ls::unload();
	Scene::UnLoad();
}

void Boss1Scene::Update(const double& dt) {

	if (ls::getTileAt(player->getPosition()) == ls::END) {
		Engine::ChangeScene((Scene*)&menu);
		return;
	}
	else if (!player->isAlive()) {
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

	bossHp->setPosition(boss->getPosition() + Vector2f(-95, -120));
	int bossHpValue = boss->get_components<BossHPComponent>()[0]->getBossHP();
	bossHp->GetCompatibleComponent<ShapeComponent>()[0]->setShape<RectangleShape>(Vector2f(bossHpValue, 10));
	bossHp->GetCompatibleComponent<ShapeComponent>()[0]->getShape().setFillColor(Color::Green);


	//printf("Boss HP: %i\n", boss->get_components<BossHPComponent>()[0]->getBossHP());


	if (spikeTime <= 0.0f && boss->isAlive())
	{
		boss->get_components<BossHPComponent>()[0]->getHit(10);
		spikeTime = 0.3f;
	}

	// Set portal to visible if boss is dead and portal isn't currently visible
	if (!boss->isAlive() && !portal->isVisible())
	{
		portal->setVisible(true);
	}
	
	// End conditions
	if (portal->isVisible() && ls::getTileAt(player->getPosition()) == ls::CHECKPT)
	{
		// This will tp to ending cutscene?
		Engine::ChangeScene((Scene*)&menu);
		return;
	}

	Scene::Update(dt);
}

void Boss1Scene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}
