#pragma once

class Input;
class Window;
class Camera;

class Scene
{
public:
	virtual void setup() = 0; //prepare scene
	virtual void update(double delta_time) = 0; //physic and logic
	virtual void draw() = 0; //just drawing
	Scene(Camera*, Window*, Input*);
protected:
	Window* window();
	Input* input();
	Camera* camera();
private:
	Window* _window;
	Input* _input;
	Camera* _camera;
};

