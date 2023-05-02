#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_boost.h"
#include "../game.h"
#include "SFML/Audio.hpp"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> pauseMenu;
static shared_ptr<Entity> npc;

static sf::View viewLowres(sf::FloatRect(Vector2f(0, (-(lowGameHeight) / 2)), Vector2f(1920, 1080)));

void HubScene::Load() {
	std::cout << "Hub load" << endl;
	std::cout <<"video height: "<< Engine::user_preferences.video_resolution.y << endl;
	/*if (Engine::user_preferences.video_resolution.y != 1080) {
		sf::View view(sf::FloatRect(Vector2f(0, -Engine::user_preferences.video_resolution.y / 2), Vector2f(1920, 1080)));
		Engine::GetWindow().setView(view);
	}*/

	//stuff
	{
		_pause = false; // Game isn't paused on load

		menu.music.stop(); // Stop the main menu music
		if (!this->music.openFromFile("res/audio/hub.wav")) { std::cout << "Music file not found." << endl; }

		// Playing after the load screen
		this->music.setLoop(true);
		this->music.setVolume(35); // TODO set as user preference

		ls::loadLevelFile("res/levels/hub.txt", 40.0f);

		auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
		ls::setOffset(Vector2f(0, ho));
	}
	// Add colliders to the boost platforms, doing it above player, because player needs a list of boost tiles for boost component
	{
		auto boosts = ls::findTiles(ls::BOOST);
		for (auto w : boosts) {
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(20.f, 20.f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
			e->addTag("boost");
		}
	}

	// Create player
	{
		player = makeEntity();
		player->addTag("player");
		player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		auto s = player->addComponent<ShapeComponent>();
		s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
		s->getShape().setFillColor(Color::Magenta);
		s->getShape().setOrigin(Vector2f(10.f, 15.f));

		auto phys = player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
		phys->setRestitution(-1); // not sure if improves movement
		player->addComponent<BoostComponent>();
	}

	// Npc that speaks
	{
		npc = makeEntity();
		npc->addTag("npc");
		npc->setPosition(ls::getTilePosition(ls::findTiles(ls::NPC)[0]));
		auto spriteCmp = npc->addComponent<SpriteComponent>();
		shared_ptr<Texture> npcTex = make_shared<Texture>();
		if (!npcTex->loadFromFile("res/img/npc.png")) {
			std::cerr << "Failed to load spritesheet!" << std::endl;
		}
		spriteCmp->setTexure(npcTex);
		spriteCmp->getSprite().setTextureRect(IntRect({ 0,0 }, { 32,32 }));

		spriteCmp->getSprite().setScale(Vector2f(1.5, 1.5));

		auto pos = ls::getTilePosition(ls::findTiles(ls::NPC)[0]);
		pos += Vector2f(16.f, 16.f); //offset to center
		auto e = makeEntity();
		e->setPosition(pos);
		e->addComponent<PhysicsComponent>(false, Vector2f(32.f, 32.f));
		e->addTag("wall");
	}
	// Other npc yellow sign
	{
		npc = makeEntity();
		npc->addTag("npc2");
		npc->setPosition(ls::getTilePosition(ls::findTiles(ls::NPC)[1]));
		auto spriteCmp = npc->addComponent<SpriteComponent>();
		shared_ptr<Texture> npcTex = make_shared<Texture>();
		if (!npcTex->loadFromFile("res/img/npc.png")) {
			std::cerr << "Failed to load spritesheet!" << std::endl;
		}
		spriteCmp->setTexure(npcTex);
		spriteCmp->getSprite().setTextureRect(IntRect({ 0,0 }, { 32,32 }));
		spriteCmp->getSprite().setColor(Color::Yellow);

		spriteCmp->getSprite().setScale(Vector2f(1.5, 1.5));

		auto pos = ls::getTilePosition(ls::findTiles(ls::NPC)[1]);
		pos += Vector2f(16.f, 16.f); //offset to center
		auto e = makeEntity();
		e->setPosition(pos);
		e->addComponent<PhysicsComponent>(false, Vector2f(32.f, 32.f));
		e->addTag("wall");
	}
	// Lava sign
	{
		auto lavaSign = makeEntity();
		lavaSign->addTag("lavaSign");
		lavaSign->setPosition(ls::getTilePosition(ls::findTiles(ls::SIGN)[0]));
	}
	// Green sign
	{
		auto greenSign = makeEntity();
		greenSign->addTag("greenSign");
		greenSign->setPosition(ls::getTilePosition(ls::findTiles(ls::SIGN)[1]));
	}
	// Yellow sign
	{
		auto yellowSign = makeEntity();
		yellowSign->addTag("yellowSign");
		yellowSign->setPosition(ls::getTilePosition(ls::findTiles(ls::SIGN)[2]));
	}
	// Blue sign
	{
		auto blueSign = makeEntity();
		blueSign->addTag("blueSign");
		blueSign->setPosition(ls::getTilePosition(ls::findTiles(ls::SIGN)[3]));
	}
	// Add physics colliders to level tiles.
	{
		auto walls = ls::findTiles(ls::WALL);
		for (auto w : walls) {
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(20.f, 20.f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
			e->addTag("wall");
		}
	}

	// also add collisions to platform tiles
	{
		auto plats = ls::findTiles(ls::HUBPLAT);
		for (auto p : plats) {
			auto pos = ls::getTilePosition(p);
			pos += Vector2f(20.f, 20.f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
			e->addTag("wall");
		}
	}

	// Pause menu
	pauseMenu = makeEntity();

	//Resolution scaling
	/*if (Engine::user_preferences.video_resolution.y != 1080) {
		pauseMenu->setPosition(Vector2f(Engine::GetWindow().getView().getCenter()));
	}
	else {
		pauseMenu->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.1f, 0.1f));
	}*/

	pauseMenu->setPosition(Vcast<float>(Engine::getWindowSize())* Vector2f(0.1f, 0.1f));
	pauseMenu->setVisible(false);
	auto shape = pauseMenu->addComponent<ShapeComponent>();

	shape->setShape<RectangleShape>(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.8f, 0.8f));
	shape->getShape().setFillColor(Color{ 112,128,144 });
	

	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto pausemenuPixels = Engine::GetWindow().mapCoordsToPixel(pauseMenu->getPosition(), Engine::GetWindow().getDefaultView());
		pauseMenu->setPosition(Vcast<float>(Engine::GetWindow().mapPixelToCoords(pausemenuPixels, viewLowres))+Vector2f(300.f,0.f));
		
		
		/*
		shape->setShape<RectangleShape>(Vcast<float>(Engine::getWindowSize())* Vector2f(0.9f, 0.9f));
		shape->getShape().setFillColor(Color{ 112,128,144 });
		shape->getShape().setOrigin(Vector2f(shape->getShape().getLocalBounds().width / 2.0f,
											 shape->getShape().getLocalBounds().height / 2.0f));
		pauseMenu->setPosition(Vector2f(viewLowres.getCenter()));
		*/
	}
	//Resolution scaling shit
	/*if (Engine::user_preferences.video_resolution.y != 1080) {
		shape->getShape().setOrigin(Vector2f(shape->getShape().getLocalBounds().width/2.0f,
											 shape->getShape().getLocalBounds().height/2.0f));
	}*/

	auto text = pauseMenu->addComponent<TextComponent>("Paused");
	
	text->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.2f));
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto pausemenuPixels = Engine::GetWindow().mapCoordsToPixel(text->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		text->getText()->setPosition(Vcast<float>(Engine::GetWindow().mapPixelToCoords(pausemenuPixels, viewLowres)));
	}

	text->getText()->setCharacterSize(45);
	auto exitToMenu = makeEntity();
	exitToMenu->setVisible(false);
	exitToMenu->addTag("ExitToMenu");
	auto exitToMenuText = exitToMenu->addComponent<TextComponent>("Exit to Main Menu");
	exitToMenuText->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.4f));
	
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto pausemenuPixels = Engine::GetWindow().mapCoordsToPixel(exitToMenuText->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		exitToMenuText->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(pausemenuPixels, viewLowres));
	}
	exitToMenuText->getText()->setCharacterSize(35);

	auto exitGame = makeEntity();
	exitGame->setVisible(false);
	exitGame->addTag("ExitGame");
	auto exitGameText = exitGame->addComponent<TextComponent>("Exit to Desktop");
	exitGameText->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.5f));
	
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto pausemenuPixels = Engine::GetWindow().mapCoordsToPixel(exitGameText->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		exitGameText->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(pausemenuPixels, viewLowres));
	}
	exitGameText->getText()->setCharacterSize(35);

	//npc1
	auto npcText = makeEntity();
	npcText->setVisible(false);
	npcText->addTag("npcText");
	auto npcTextCmp = npcText->addComponent<TextComponent>("Please help us...\nObsidiaani roams in the Lava World.");
	npcTextCmp->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.05f, 0.1f));
	npcTextCmp->getText()->setCharacterSize(25);
	
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto Pixels = Engine::GetWindow().mapCoordsToPixel(npcTextCmp->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		npcTextCmp->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(Pixels, viewLowres));
	}
	
	//npc2
	auto npc2Text = makeEntity();
	npc2Text->setVisible(false);
	npc2Text->addTag("npc2Text");
	auto npc2TextCmp = npc2Text->addComponent<TextComponent>("You might need to come back later\nto get to the Thunder World");
	npc2TextCmp->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.7f, 0.85f));
	npc2TextCmp->getText()->setCharacterSize(25);
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto Pixels = Engine::GetWindow().mapCoordsToPixel(npc2TextCmp->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		npc2TextCmp->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(Pixels, viewLowres));
	}


	// Lava world sign text
	auto lavaSignText = makeEntity();
	lavaSignText->setVisible(false);
	lavaSignText->addTag("lavaSignText");
	auto lavaSignTextCmp = lavaSignText->addComponent<TextComponent>("Portal to the Lava World");
	lavaSignTextCmp->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.05f));
	lavaSignTextCmp->getText()->setCharacterSize(25);
	
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto Pixels = Engine::GetWindow().mapCoordsToPixel(lavaSignTextCmp->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		lavaSignTextCmp->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(Pixels, viewLowres));
	}

	// Earth world sign text
	auto greenSignText = makeEntity();
	greenSignText->setVisible(false);
	greenSignText->addTag("greenSignText");
	auto greenSignTextCmp = greenSignText->addComponent<TextComponent>("Portal to the Earth World");
	greenSignTextCmp->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.8f, 0.3f));
	greenSignTextCmp->getText()->setCharacterSize(25);

	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto Pixels = Engine::GetWindow().mapCoordsToPixel(greenSignTextCmp->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		greenSignTextCmp->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(Pixels, viewLowres));
	}

	// Aqua world sign text
	auto blueSignText = makeEntity();
	blueSignText->setVisible(false);
	blueSignText->addTag("blueSignText");
	auto blueSignTextCmp = blueSignText->addComponent<TextComponent>("Portal to the Aqua World");
	blueSignTextCmp->getText()->setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.05f, 0.85f));
	blueSignTextCmp->getText()->setCharacterSize(25);
	
	if (Engine::user_preferences.video_resolution.y != 1080) {
		auto Pixels = Engine::GetWindow().mapCoordsToPixel(blueSignTextCmp->getText()->getPosition(), Engine::GetWindow().getDefaultView());
		blueSignTextCmp->getText()->setPosition(Engine::GetWindow().mapPixelToCoords(Pixels, viewLowres));
	}



	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << "Hub Load Done" << endl;
	this->music.play();
	setLoaded(true);
}

void HubScene::UnLoad() {
	std::cout << "Hub Unload" << endl;
	player.reset();
	ls::unload();
	this->music.stop();
	Scene::UnLoad();
}

void HubScene::Update(const double& dt) {
	
	if (Engine::user_preferences.video_resolution.y != 1080) {
		Engine::GetWindow().setView(viewLowres);
		cout << "lowres view";
	}
	// Handling Pausing
	static float pauseTime = 0.0f;
	if (_pause) // Execute this code if the game is paused
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape) && pauseTime <= 0.0f)
		{
			_pause = !_pause;

			for (auto e : ents.list)
			{
				{
					e->setVisible(true);
				}
			}
			pauseMenu->setVisible(false);
			ents.find("ExitGame")[0]->setVisible(false);
			ents.find("ExitToMenu")[0]->setVisible(false);


			pauseTime = 0.5f;
		}
		//Exit menu to scale with view
		sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
		//Exit to menu button
		auto exitToMenuEnt = ents.find("ExitToMenu")[0];
		auto exitToMenuTextCmp = exitToMenuEnt->GetCompatibleComponent<TextComponent>()[0];

		//if (exitToMenuTextCmp->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
		if (exitToMenuTextCmp->getText()->getGlobalBounds().contains(Engine::GetWindow().mapPixelToCoords(pixelPos))) {
			exitToMenuTextCmp->getText()->setFillColor(sf::Color::Red);
			if (Mouse::isButtonPressed(Mouse::Left())) {
				Engine::ChangeScene(&menu);
				return;
			}
		}
		else {
			exitToMenuTextCmp->getText()->setFillColor(sf::Color::White);
		}

		//Exit game button

		auto exitGameEnt = ents.find("ExitGame")[0];
		auto exitGameTextCmp = exitGameEnt->GetCompatibleComponent<TextComponent>()[0];
		
		//if (exitGameTextCmp->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow()))) && exitGameEnt->isVisible()) {
		if (exitGameTextCmp->getText()->getGlobalBounds().contains(Engine::GetWindow().mapPixelToCoords(pixelPos)) && exitGameEnt->isVisible()) {
			exitGameTextCmp->getText()->setFillColor(sf::Color::Red);
			if (Mouse::isButtonPressed(Mouse::Left())) {
				Engine::GetWindow().close();
				return;
			}
		}
		else {
			exitGameTextCmp->getText()->setFillColor(sf::Color::White);
		}

		pauseTime -= dt;
		return;
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape) && pauseTime <= 0.0f) // Pause the game
	{
		_pause = !_pause;
		for (auto e : ents.list)
		{
			e->setVisible(false);
		}
		pauseMenu->setVisible(true);
		ents.find("ExitGame")[0]->setVisible(true);
		ents.find("ExitToMenu")[0]->setVisible(true);
		ents.find("npcText")[0]->setVisible(false);
		ents.find("lavaSignText")[0]->setVisible(false);
		ents.find("blueSignText")[0]->setVisible(false);
		ents.find("greenSignText")[0]->setVisible(false);
		ents.find("npc2Text")[0]->setVisible(false);

		pauseTime = 0.5f;
	}

	// Handle loading other levels
	if (ls::getTileAt(player->getPosition()) == ls::END) {
		Engine::ChangeScene((Scene*)&level1);
		return;
	}
	else if (!player->isAlive()) {
		Engine::ChangeScene((Scene*)&hub);
		return;
	}

	// Handling npc text
	if (length(player->getPosition() - ents.find("npc")[0]->getPosition()) < 70.0f && !ents.find("npcText")[0]->isVisible() && !pauseMenu->isVisible())
	{
		ents.find("npcText")[0]->setVisible(true);
	}
	else if (length(player->getPosition() - ents.find("npc")[0]->getPosition()) > 70.0f && ents.find("npcText")[0]->isVisible()) {
		ents.find("npcText")[0]->setVisible(false);
	}

	// Other npc text
	if (length(player->getPosition() - ents.find("npc2")[0]->getPosition()) < 70.0f && !ents.find("npc2Text")[0]->isVisible() && !pauseMenu->isVisible())
	{
		ents.find("npc2Text")[0]->setVisible(true);
	}
	else if (length(player->getPosition() - ents.find("npc2")[0]->getPosition()) > 70.0f && ents.find("npc2Text")[0]->isVisible()) {
		ents.find("npc2Text")[0]->setVisible(false);
	}

	// Lava Sign text
	if (length(player->getPosition() - ents.find("lavaSign")[0]->getPosition()) < 70.0f && !ents.find("lavaSignText")[0]->isVisible() && !pauseMenu->isVisible())
	{
		ents.find("lavaSignText")[0]->setVisible(true);
	}
	else if (length(player->getPosition() - ents.find("lavaSign")[0]->getPosition()) > 70.0f && ents.find("lavaSignText")[0]->isVisible()) {
		ents.find("lavaSignText")[0]->setVisible(false);
	}

	// Green Sign text
	if (length(player->getPosition() - ents.find("greenSign")[0]->getPosition()) < 70.0f && !ents.find("greenSignText")[0]->isVisible() && !pauseMenu->isVisible())
	{
		ents.find("greenSignText")[0]->setVisible(true);
	}
	else if (length(player->getPosition() - ents.find("greenSign")[0]->getPosition()) > 70.0f && ents.find("greenSignText")[0]->isVisible()) {
		ents.find("greenSignText")[0]->setVisible(false);
	}

	// Blue Sign text
	if (length(player->getPosition() - ents.find("blueSign")[0]->getPosition()) < 70.0f && !ents.find("blueSignText")[0]->isVisible() && !pauseMenu->isVisible())
	{
		ents.find("blueSignText")[0]->setVisible(true);
	}
	else if (length(player->getPosition() - ents.find("blueSign")[0]->getPosition()) > 70.0f && ents.find("blueSignText")[0]->isVisible()) {
		ents.find("blueSignText")[0]->setVisible(false);
	}
	
	
	pauseTime -= dt;

	Scene::Update(dt);
}

void HubScene::Render() {
	ls::render(Engine::GetWindow(), 1);
	Scene::Render();
}
