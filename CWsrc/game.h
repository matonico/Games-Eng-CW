#pragma once
#include "scenes/scene_hub.h"
#include "scenes/scene_level1.h"
#include "scenes/scene_boss1.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_settings.h"



const int highGameWidth = 1920;
const int highGameHeight = 1080;

const int lowGameWidth = 1280;
const int lowGameHeight = 720;

const int gameWidth = 1080;
const int gameHeight = 720;

//const int fullScreen = 8; // full screen
const int fullScreen = 7; // window mode

extern MenuScene menu;
extern SettingsScene settings;
extern Level1Scene level1;
extern HubScene hub;
extern Boss1Scene boss1;


