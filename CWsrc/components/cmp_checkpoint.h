#pragma once

#include "cmp_physics.h"
#include "ecm.h"
#include "LevelSystem.h"

using namespace std;
using namespace sf;


class CheckpointComponent : public Component {
protected:
	Vector2f _checkpoint = ls::getTilePosition(ls::findTiles(ls::START)[0]); // set the original checkpoint to the starting tile
	std::vector<std::shared_ptr<Entity>> _checkpoints; // Vector of entities with checkpoint tag
public:
	void update(double dt) override;
	void render() override {}
	Vector2f getCheckpoint();
	explicit CheckpointComponent(Entity* p);
	CheckpointComponent() = delete;
};
