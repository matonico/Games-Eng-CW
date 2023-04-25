#include "cmp_player_shoot.h"
#include "cmp_player_shoot.h"
#include "cmp_hurt_enemy.h"
#include "cmp_hurt_boss.h"
#include "../CWsrc/game.h" // For user preferences
#include "cmp_activate_btn.h"
#include <engine.h>
#include "cmp_bullet.h"
#include "maths.h"

#include "SFML/Audio.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System/Angle.hpp"

using namespace std;
using namespace sf;

void PlayerShootComponent::update(double dt)
{
	// Using firetime as a way to limit the fire rate of the player
	static float firetime = 0.0f;
	firetime -= dt;
	if (firetime <= 0 && Mouse::isButtonPressed(Mouse::Left()))
	{
		shoot();
		//cout <<to_string( _shootSound.getDuration().asMilliseconds()) << endl;
		_shootSound.setVolume(50);// TODO set as user preference
		_shootSound.play();
		firetime = 0.5f;
	}
}

void PlayerShootComponent::shoot() const
{
	// Create the bullet
	auto bullet = _parent->scene->makeEntity();
	bullet->addTag("bullet");

	//Position
	Vector2f spawnPos = _parent->getPosition();

	Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow()));
	//printf("Mouse position in window = (%f,%f)\n", mousePos.x, mousePos.y);

	Vector2f playerToMouseNormal = normalize(mousePos - _parent->getPosition());
	//printf("player to mouse normal = (%f,%f)\n", playerToMouseNormal.x, playerToMouseNormal.y);
	playerToMouseNormal.y *= -1.0f;

	spawnPos = spawnPos + playerToMouseNormal * Vector2f(20.0f, -30.0f);

	bullet->setPosition(spawnPos);

	if (!_parent->scene->ents.find("enemy").empty()) 
	{
		bullet->addComponent<HurtEnemyComponent>();
	}

	if (!_parent->scene->ents.find("boss").empty()) // Crashes game if there is no boss and player tries to shoot
	{
		bullet->addComponent<HurtBossComponent>();
	}


	bullet->addComponent<ActivateButtonComponent>();

	// Shape
	bullet->addComponent<BulletComponent>();
	auto s = bullet->addComponent<ShapeComponent>();
	s->setShape<sf::RectangleShape>(Vector2f(8, 8));
	s->getShape().setFillColor(Color::Yellow);
	s->getShape().setOrigin(Vector2f(8.f, 8.f));

	// Physics
	auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(3, 3));
	p->setVelocity(playerToMouseNormal * 333.0f);
	p->setMass(0.0f);
	p->setFriction(0.0f);
	p->setRestitution(0.0f);
	p->dampen(Vector2f(3, 3));
	p->setGravityScale(0.0f);
}

PlayerShootComponent::PlayerShootComponent(Entity* p) : Component(p)
{
	if (!_shootSoundBuffer.loadFromFile("res/audio/shoot.wav"))
	{	
		cout << "cant load sound fx" << endl;
	}
	_shootSound.setBuffer(_shootSoundBuffer);
}
