#include "cmp_boost.h"
#include "cmp_player_physics.h"
#include <engine.h>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;

void BoostComponent::update(double dt)
{
    // loop over the boost tiles and check collision with player
    for (auto e : _boostTiles)
    {
        if (length(e->getPosition() - _parent->getPosition()) < 40.0f)
        {
            Vector2f pos = _parent->getPosition();
            Vector2f vel = _parent->get_components<PlayerPhysicsComponent>()[0]->getVelocity();
            _parent->get_components<PlayerPhysicsComponent>()[0]->setVelocity(Vector2f(vel.x,0));
            _parent->get_components<PlayerPhysicsComponent>()[0]->teleport(Vector2f(pos.x, pos.y - 2.0f)); 
            _parent->get_components<PlayerPhysicsComponent>()[0]->impulse(Vector2f(0, -80.f)); 
            _boostSound.play();
        }
    }
}

BoostComponent::BoostComponent(Entity* p) : Component(p), _boostTiles(_parent->scene->ents.find("boost")) // fill vector with boost tiles
{
    if (!_boostSoundBuffer.loadFromFile("res/audio/boost.wav"))
    {
        cout << "cant load sound fx" << endl;
    }
    _boostSound.setBuffer(_boostSoundBuffer);
    _boostSound.setVolume(50); // TODO set from user preference
} 

