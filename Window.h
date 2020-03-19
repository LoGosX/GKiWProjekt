#pragma once
#include "Event.h"
#include <queue>

struct GLFWwindow;
class Input;

class Window
{

public:
	bool create(int width, int height, const char* name);
	GLFWwindow* windowHandle();
	void makeContextCurrent();
	bool isOpen() const;
	bool pollEvents(Event& ev);
	void close();
	void destroy();
	void clearColor(float, float, float, float);
	void clear();
	void swapBuffers();
	int width() const;
	int height() const;
	void centerCursor();
private:
	int _width{ 0 };
	int _height{ 0 };
	GLFWwindow* _window{ nullptr };
	std::queue<Event> _window_events;
};

