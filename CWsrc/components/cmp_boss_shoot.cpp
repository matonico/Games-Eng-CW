#include "cmp_boss_shoot.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
using namespace std;
using namespace sf;

void BossShootComponent::update(double dt) {
    _firetime -= dt;
    if (_firetime <= 0.f&&_parent->isAlive()) {
        fire();
        _bossShootSound.play();
        _bossShootSound.setVolume(50);// TODO set as user preference
        _firetime = 2.f;
    }
}

void BossShootComponent::fire() const {
    auto bullet = _parent->scene->makeEntity();
    bullet->addTag("enemyBullet");
    
    bullet->addComponent<HurtComponent>(30.0f);
    bullet->addComponent<BulletComponent>();
    auto s = bullet->addComponent<ShapeComponent>();

    s->setShape<sf::CircleShape>(8.f);
    //Position
    Vector2f spawnPos = _parent->getPosition();

    Vector2f playerPos = _player.lock()->getPosition();
    //printf("Mouse position in window = (%f,%f)\n", mousePos.x, mousePos.y);

    Vector2f playerToBossNormal = normalize(playerPos - _parent->getPosition());
    //printf("player to mouse normal = (%f,%f)\n", playerToMouseNormal.x, playerToMouseNormal.y);
    playerToBossNormal.y *= -1.0f;

    spawnPos = spawnPos + playerToBossNormal * Vector2f(75.0f, -75.0f);

    bullet->setPosition(spawnPos);


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
    p->impulse(-direction * 37.f);

}

BossShootComponent::BossShootComponent(Entity* p)
    : Component(p), _firetime(2.f), _player(_parent->scene->ents.find("player")[0]) 
{
    if (!_bossShootSoundBuffer.loadFromFile("res/audio/boss_shoot.wav"))
    {
        cout << "cant load sound fx" << endl;
    }
    _bossShootSound.setBuffer(_bossShootSoundBuffer);
}
