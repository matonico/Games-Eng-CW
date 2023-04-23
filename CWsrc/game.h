#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_boss1.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_settings.h"


const int gameWidth = 1920;
const int gameHeight = 1080;

//const int gameWidth = 1080;
//const int gameHeight = 720;

extern MenuScene menu;
extern SettingsScene settings;
extern Level1Scene level1;
extern Level2Scene level2;
extern Level3Scene level3;
extern Boss1Scene boss1;
