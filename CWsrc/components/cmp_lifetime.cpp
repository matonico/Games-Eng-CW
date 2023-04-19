#include "cmp_lifetime.h"
#include <engine.h>
using namespace std;
using namespace sf;

void LifetimeComponent::update(double dt)
{
    // If lifetime is <=0, delete the parent entity that lifetimeComponent is attached to
    if (_lifetime <= 0.0f)
    {
        _parent->setForDelete();
    }
    _lifetime -= dt;
}

LifetimeComponent::LifetimeComponent(Entity* p, float l) : Component(p) {
    _lifetime = l; // Constructor allowing you to pass in a lifetime value
}
