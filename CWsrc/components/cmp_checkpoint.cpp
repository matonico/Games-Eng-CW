#include "cmp_checkpoint.h"
#include <engine.h>
using namespace std;
using namespace sf;

void CheckpointComponent::update(double dt)
{
    // loop over the checkpoints and check collision with player
    for (auto e : _checkpoints)
    {
        if (length(e->getPosition() - _parent->getPosition()) < 25.0f)
        {
            _checkpoint = e->getPosition();
            e->setForDelete();
        }
    }
}

Vector2f CheckpointComponent::getCheckpoint() { return _checkpoint; }


CheckpointComponent::CheckpointComponent(Entity* p) : Component(p), _checkpoints(_parent->scene->ents.find("checkpoint")) {} // fill vector with checkpoints

