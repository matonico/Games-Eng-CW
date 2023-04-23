#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include "SFML/Window/Mouse.hpp"
#include <iostream>

using namespace std;
using namespace sf;

static shared_ptr<Entity> playButt;
static shared_ptr<Entity> settingsButt;
static shared_ptr<Entity> exitButt;

//Load in buttons
void MenuScene::Load() {
    cout << "Menu Load \n";

    // Main title
    auto title = makeEntity();
    auto menuText = title->addComponent<TextComponent>();
    menuText->SetText("Platformer Game");
    title->addTag("Title");
    menuText->getText()->setOrigin(Vector2f(menuText->getText()->getLocalBounds().width / 2.f,
        menuText->getText()->getLocalBounds().height / 2.f));

    menuText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2 - 350.f)));
    menuText->getText()->setCharacterSize(45);

    // Play button
    playButt = makeEntity();
    auto playText = playButt->addComponent<TextComponent>();
    playText->SetText("Play");
    playButt->addTag("Play");
    playText->getText()->setOrigin(Vector2f(playText->getText()->getLocalBounds().width / 2.f,
        playText->getText()->getLocalBounds().height / 2.f));

    playText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2)));
    playText->getText()->setCharacterSize(35);

    //Settings button
    settingsButt = makeEntity();
    auto settingsText = settingsButt->addComponent<TextComponent>();
    settingsText->SetText("Settings");
    settingsButt->addTag("Settings");
    settingsText->getText()->setOrigin(Vector2f(settingsText->getText()->getLocalBounds().width / 2.f,
        settingsText->getText()->getLocalBounds().height / 2.f));

    settingsText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2) + 100.f));
    settingsText->getText()->setCharacterSize(35);

    //Exit button
    exitButt = makeEntity();
    auto exitText = exitButt->addComponent<TextComponent>();
    exitText->SetText("Exit");
    exitButt->addTag("Exit");
    exitText->getText()->setOrigin(Vector2f(exitText->getText()->getLocalBounds().width / 2.f,
        exitText->getText()->getLocalBounds().height / 2.f));

    exitText->getText()->setPosition(Vector2f((Engine::GetWindow().getSize().x / 2), (Engine::GetWindow().getSize().y / 2) + 200.f));
    exitText->getText()->setCharacterSize(35);

    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    // cout << "Menu Update "<<dt<<"\n";

      //left in for testing purposes
    if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
        Engine::ChangeScene(&menu);
        return;
    }
    //Exit button
    auto exitEnt = ents.find("Exit")[0]->GetCompatibleComponent<TextComponent>();
    shared_ptr<TextComponent>exitButton = exitEnt.front();

    if (exitButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
        exitButton->getText()->setFillColor(sf::Color::Red);
        if (Mouse::isButtonPressed(Mouse::Left())) {
            Engine::GetWindow().close();
            return;
        }
    }
    else {
        exitButton->getText()->setFillColor(sf::Color::White);
    }

    //play button
    auto playEnt = ents.find("Play")[0]->GetCompatibleComponent<TextComponent>();
    shared_ptr<TextComponent>playButton = playEnt.front();

    if (playButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
        playButton->getText()->setFillColor(sf::Color::Red);
        if (Mouse::isButtonPressed(Mouse::Left())) {
            Engine::ChangeScene(&level1);
            return;
        }
    }
    else {
        playButton->getText()->setFillColor(sf::Color::White);
    }

    //settings button
    auto settingsEnt = ents.find("Settings")[0]->GetCompatibleComponent<TextComponent>();
    shared_ptr<TextComponent>settingsButton = settingsEnt.front();

    if (settingsButton->getText()->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(Engine::GetWindow())))) {
        settingsButton->getText()->setFillColor(sf::Color::Red);
        if (Mouse::isButtonPressed(Mouse::Left())) {
            Engine::ChangeScene((Scene*)&settings);
            return;
        }

    }
    else {
        settingsButton->getText()->setFillColor(sf::Color::White);
    }




    Scene::Update(dt);
}
