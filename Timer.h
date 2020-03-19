#pragma once
class Timer
{
public:
	double currentTime() const;
	double tickTime() const;
	void tick();
private:
	double _time {0};
	double _tick{ 0 };
};

