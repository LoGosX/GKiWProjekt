#pragma once

class Input;
class Window;
class Camera;

class Scene
{
public:
	virtual void setup() = 0; //prepare scene
	virtual void update() = 0; //physic and logic
	virtual void draw() = 0; //just drawing
	Scene(Camera*, Window*, Input*);
	void setTime(float time);
	void setDeltaTime(float dt);
protected:
	Window* window();
	Input* input();
	Camera* camera();
	float getTime();
	float deltaTime();
private:
	float _t;
	float _dt;
	Window* _window;
	Input* _input;
	Camera* _camera;
};

