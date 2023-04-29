#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
SettingsScene settings;
Level1Scene level1;
HubScene hub;
Boss1Scene boss1;

int main() {
	Engine::Start(gameWidth, gameHeight, "The Defence of Jola Prime", &menu);
}