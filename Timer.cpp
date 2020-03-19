#include "Timer.h"
#include "GLFW/glfw3.h"

double Timer::currentTime() const {
	return glfwGetTime();
}
double Timer::tickTime() const {
	return _tick;
}
void Timer::tick() {
	double t = currentTime();
	_tick = t - _time;
	_time = t;
}