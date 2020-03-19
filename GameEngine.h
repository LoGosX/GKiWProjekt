#pragma once

class RenderSystem;
class SceneManager;
class Input;
class Window;
class Camera;

class GameEngine
{
public:
	void initialize();
	void run();

private:
	RenderSystem* _render_system {nullptr};
	SceneManager* _scene_manager {nullptr};
	Input* _input;
	Window* _window;
	Camera* _camera;
};

