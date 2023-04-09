#include "cmp_activate_btn.h"
#include "LevelSystem.h"
#include "../components/cmp_sprite.h"
#include <engine.h>
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

void ActivateButtonComponent::update(double dt) {
    // loop over the enemies and check collision with bullet
    for (auto e : _buttons)
    {
        if (length(e->getPosition() - _parent->getPosition()) < 25.0f)
        {
            _parent->setForDelete();
            respawnBridge();
        }
    }
}

ActivateButtonComponent::ActivateButtonComponent(Entity* p)
    : Component(p), _buttons(_parent->scene->ents.find("button")) {} // fill vector with buttons

void ActivateButtonComponent::respawnBridge()
{
    // Delete bridge if it already exists
    std::vector<shared_ptr<Entity>> bridges = _parent->scene->ents.find("bridge");
    if (bridges.size()>0)
    {
        for (auto b : bridges)
        {
            b.get()->setForDelete();
        }
        bridges.clear();
    }
    // Create bridge to help player
    auto bridge = _parent->scene->makeEntity();
    bridge->addTag("bridge");
    bridge->setPosition(ls::getTilePosition(ls::findTiles(ls::LVL1BTN)[0]) + Vector2f(120, 10));

    auto s = bridge->addComponent<ShapeComponent>();

    s->setShape<sf::RectangleShape>(Vector2f( 200,10 ));
    s->getShape().setFillColor(Color::Blue);
    s->getShape().setOrigin(Vector2f(100.0, 5.0f));
    auto p = bridge->addComponent<PhysicsComponent>(true,Vector2f(200.f, 10.f));
    p->setMass(5000);
}