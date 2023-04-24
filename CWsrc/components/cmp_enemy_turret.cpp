#include "cmp_enemy_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
using namespace std;
using namespace sf;

void EnemyTurretComponent::update(double dt) {
  _firetime -= dt;
  if (_firetime <= 0.f) {
    fire();
    _firetime = 1.f;
  }
  //_parent->setRotation(180.f + sin(angle) * 45.f);
}

void EnemyTurretComponent::fire() const {
  auto bullet = _parent->scene->makeEntity();
  bullet->addTag("enemyBullet");
  bullet->setPosition(_parent->getPosition());
  bullet->addComponent<HurtComponent>(30.0f);
  bullet->addComponent<BulletComponent>();
  auto s = bullet->addComponent<ShapeComponent>();

  s->setShape<sf::CircleShape>(8.f);
  s->getShape().setFillColor(Color::Red);
  s->getShape().setOrigin(Vector2f(8.f, 8.f));
  auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(3.f, 3.f));
  p->setMass(1.f);
  p->setRestitution(.0f);
  p->setFriction(.0f);
  p->dampen(Vector2f(0, 0));
  
  //p->impulse(sf::rotate(Vector2f(0, 15.f), -_parent->getRotation()));
  auto pl = _player.lock();
  auto direction = normalize((_parent->getPosition()) - (pl->getPosition()));
  p->impulse(-direction * 75.f);

}

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f), _player(_parent->scene->ents.find("player")[0]) {}
