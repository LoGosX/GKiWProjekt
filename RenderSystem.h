#pragma once

class Window;
class Camera;

class RenderSystem
{
public:
	bool initialize();
	void destroy();
	Window* window() const;
	Camera* camera();
private:
	Window* _window{ nullptr };
	Camera* _camera;
};

