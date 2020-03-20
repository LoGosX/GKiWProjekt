#include "GameEngine.h"
#include "RenderSystem.h"
#include "Window.h"
#include "SceneManager.h"
#include "ExampleScene.h"
#include "RaymarchScene.h"
#include <cmath>
#include "Input.h"
#include "Timer.h"


void GameEngine::initialize()
{
	_render_system = new RenderSystem;
	if (!_render_system->initialize()) {
		exit(EXIT_FAILURE);
	}
	_window = _render_system->window();
	_input = new Input(_window);
	_camera = _render_system->camera();
	_scene_manager = new SceneManager;
	int choice;
	fprintf(stdout, "Choose scene to load:\n1. Example Scene\n2. Raymarch scene\nYour choice: ");
	scanf_s("%d", &choice);
	if(choice == 1)
		_scene_manager->setCurrentScene(new ExampleScene(_camera, _window, _input));
	else
		_scene_manager->setCurrentScene(new RaymarchScene(_camera, _window, _input));
	_scene_manager->getCurrentScene()->setup();
}

void GameEngine::run()
{
	Timer timer;
	auto window = _render_system->window();
	window->clearColor(0.5, 0.5, 1, 0);
	while (window->isOpen()) {
		_scene_manager->getCurrentScene()->update(timer.tickTime());

		_window->clear();
		_scene_manager->getCurrentScene()->draw();
		_window->swapBuffers();
		
		_input->update();

		timer.tick();
	}

	_render_system->destroy();
}
