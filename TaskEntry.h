#pragma once
#include "Entry.h"
#include <ostream>
#include <istream>
#include <sstream>
#include <iostream>

class TaskEntry :
	public Entry
{
	time_t timeToStart;
	time_t period;
	bool repeatable;

public:
	TaskEntry();
	TaskEntry(string name, bool repeat, time_t repeatEvery, time_t currentTime);

	time_t GetTimeToStart() const;

	void ResetTimeToStart();

	bool operator>(TaskEntry task);
	bool operator<(TaskEntry task);
	bool operator==(TaskEntry task);
	bool operator>=(TaskEntry task);
	bool operator<=(TaskEntry task);
	friend ostream& operator<<(ostream &stream, TaskEntry task);
	friend istream& operator>>(istream &stream, TaskEntry* task);

	~TaskEntry();
};
