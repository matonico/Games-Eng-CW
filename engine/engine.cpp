#include "engine.h"
#include "maths.h"
#include "../CWsrc/game.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <stdexcept>

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static  RectangleShape loadingBar;
static RenderWindow* _window;

void Loading_update(float dt, const Scene* const scn) {
	//  cout << "Eng: Loading Screen\n";
	if (scn->isLoaded()) {
		loadingBar.setSize(Vector2f(33, 33));
		loadingBar.setScale(Vector2f(1, 1));
		cout << "Eng: Exiting Loading Screen\n";
		loading = false;

	}
	else {
		loadingspinner += 220.0f * dt;
		loadingTime += dt;
	}
}

void Loading_render() { // Loading screen with loading bar and circle spinning
	// Set bar attributes
	loadingBar.setSize(Vector2f(33, 33));
	loadingBar.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.3f, 0.7f));
	loadingBar.setFillColor(Color(23, 245, 129));

	// Create outline of loading bar
	static RectangleShape loadingBarOutline(Vector2f(600, 33));
	loadingBarOutline.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.3f, 0.7f));
	loadingBarOutline.setFillColor(Color(255, 255, 255));

	// If loading bar isn't at max, scale it up
	if (loadingBar.getGlobalBounds().width <= loadingBarOutline.getGlobalBounds().width)
	{
		loadingBar.setScale(Vector2f(loadingTime * 7, 1));
	}

	// Spinning hexagon
	static CircleShape loadingCircle(30, 6);
	loadingCircle.setOrigin(Vector2f(30., 30.));
	loadingCircle.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.7f, 0.3f));
	loadingCircle.setRotation(degrees(loadingspinner));
	loadingCircle.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));

	// Text that says loading
	static Text t("Loading", *Resources::get<sf::Font>("Poppins-Regular.ttf"));
	t.setFillColor(Color(255, 255, 255));
	t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.3f));

	// Render all these things
	Renderer::queue(&t);
	Renderer::queue(&loadingBarOutline);
	Renderer::queue(&loadingBar);
	Renderer::queue(&loadingCircle);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() {
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	{
		frametimes[++ftc] = dt;
		static string avg = _gameName + " FPS:";
		if (ftc % 60 == 0) {
			double davg = 0;
			for (const auto t : frametimes) {
				davg += t;
			}
			davg = 1.0 / (davg / 255.0);
			_window->setTitle(avg + toStrDecPt(2, davg));
		}
	}

	if (loading) {
		Loading_update(dt, _activeScene);
	}
	else if (_activeScene != nullptr) {
		Physics::update(dt);
		_activeScene->Update(dt);
	}
}

void Engine::Render(RenderWindow& window) {
	if (loading) {
		Loading_render();
	}
	else if (_activeScene != nullptr) {
		_activeScene->Render();
	}

	Renderer::render();
}

void Engine::Start(unsigned int width, unsigned int height,
	const std::string& gameName, Scene* scn) {
	RenderWindow window(VideoMode({ width, height }), gameName);
	_gameName = gameName;
	_window = &window;
	Renderer::initialise(window);
	Physics::initialise();
	ChangeScene(scn);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		/*
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		*/
		window.clear();
		Update();
		Render(window);
		window.display();
	}
	if (_activeScene != nullptr) {
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}
	window.close();
	Physics::shutdown();
	// Render::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::ChangeScene(Scene* s) {
	cout << "Eng: changing scene: " << s << endl;
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr) {
		old->UnLoad(); // todo: Unload Async
	}

	if (!s->isLoaded()) {
		cout << "Eng: Entering Loading Screen\n";
		loadingTime = 0;
		_activeScene->LoadAsync();
		//_activeScene->Load();
		loading = true;
	}
}

void Scene::Update(const double& dt) { ents.update(dt); }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		// Are we already loading asynchronously?
		if (_loaded_future.valid() // yes
			&&                     // Has it finished?
			_loaded_future.wait_for(chrono::seconds(0)) ==
			future_status::ready) {
			// Yes
			_loaded_future.get();
			_loaded = true;
		}
		return _loaded;
	}
}
void Scene::setLoaded(bool b) {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		_loaded = b;
	}
}

void Scene::UnLoad() {
	ents.list.clear();
	setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
	// Return time since Epoc
	long long now() {
		return std::chrono::high_resolution_clock::now()
			.time_since_epoch()
			.count();
	}
	// Return time since last() was last called.
	long long last() {
		auto n = now();
		static auto then = now();
		auto dt = n - then;
		then = n;
		return dt;
	}
} // namespace timing

Scene::~Scene() { UnLoad(); }