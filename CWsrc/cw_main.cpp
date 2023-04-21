#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
Boss1Scene boss1;

int main() {
	Engine::Start(gameWidth, gameHeight, "Game Name here", &boss1);
}