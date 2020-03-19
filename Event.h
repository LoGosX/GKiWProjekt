#pragma once
struct Event
{
	Event() = default;
	Event(int k, int s, int a, int m) : key(k), scancode(s), action(a), mods(m) {}
	int key;
	int scancode;
	int action;
	int mods;
};

