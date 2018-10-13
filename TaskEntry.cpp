#include "TaskEntry.h"
#include "time.h"


TaskEntry::TaskEntry()
{
	name = string("MY_NAME");
	repeatable = false;
	period = 10;
	timeToStart = CurrentTime() + period;
}

TaskEntry::TaskEntry(string name, bool repeat, time_t repeatEvery, time_t currentTime)
{
	name = name;
	period = repeatEvery;
	repeatable = repeat;
	timeToStart = repeatEvery + currentTime;
}

time_t TaskEntry::GetTimeToStart() const
{
	return timeToStart;
}

void TaskEntry::ResetTimeToStart()
{
	timeToStart = CurrentTime() + period;
}

bool TaskEntry::operator>(TaskEntry task)
{
	return this->timeToStart > task.timeToStart;
}

bool TaskEntry::operator<(TaskEntry task)
{
	return this->timeToStart < task.timeToStart;
}

bool TaskEntry::operator==(TaskEntry task)
{
	return this->timeToStart == task.timeToStart;
}

bool TaskEntry::operator>=(TaskEntry task)
{
	return this->timeToStart >= task.timeToStart;
}

bool TaskEntry::operator<=(TaskEntry task)
{
	return this->timeToStart <= task.timeToStart;
}

TaskEntry::~TaskEntry()
{
}

ostream & operator<<(ostream &stream, TaskEntry task)
{
	stream << "Name: " << task.name << ";\t"
		<< "Repeatable: " << task.repeatable << ";\t"
		<< "Period: " << task.period << "s;\t"
		<< "Time to start from now: " << task.timeToStart << "s;" << endl;
	return stream;
}

istream & operator>>(istream &stream, TaskEntry* task)
{
	cout << "\tTASK:" << endl 
		<< "Name(string): ";
	stream >> task->name;
	
	cout << "Repeatable(1/0): ";
	stream >> task->repeatable;
	
	cout << "Repeat itnerval(int): ";
	stream >> task->period;

	task->timeToStart = CurrentTime() + task->period;
	return stream;
}
