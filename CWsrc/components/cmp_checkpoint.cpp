#include "cmp_checkpoint.h"
#include <engine.h>
#include "SFML/Audio.hpp"
using namespace std;
using namespace sf;

void CheckpointComponent::update(double dt)
{
    // loop over the checkpoints and check collision with player
    for (auto e : _checkpoints)
    {
        if ((length(e->getPosition() - _parent->getPosition()) < 25.0f)&&e->isAlive())
        {
            _checkpoint = e->getPosition();
            e->setForDelete();
            _checkpointSound.play();
        }
    }
}

Vector2f CheckpointComponent::getCheckpoint() { return _checkpoint; }


CheckpointComponent::CheckpointComponent(Entity* p) : Component(p), _checkpoints(_parent->scene->ents.find("checkpoint"))  // fill vector with checkpoints
{
    if (!_checkpointSoundBuffer.loadFromFile("res/audio/checkpoint.wav"))
    {
        cout << "cant load sound fx" << endl;
    }
    _checkpointSound.setBuffer(_checkpointSoundBuffer);
    _checkpointSound.setVolume(75); // TODO set from user preference
}

