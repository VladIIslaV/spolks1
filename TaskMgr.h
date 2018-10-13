#pragma once
#include "TaskEntry.h"
#include <list>
#include <queue>
#include <deque>
#include <string>
#include <chrono>

#include "time.h"

using namespace std;

typedef int TaskId_t;

static time_t curTime;

class TaskMgr
{
	static deque<TaskEntry> taskPool;
	//static list<TaskEntry> taskPoolReadyForImplementation;
	//static time_point<system_clock> sysClock;	//embedded clock
public:

	TaskMgr() = default;
	~TaskMgr() = default;

	static void CreateTask(TaskEntry newEntry);	//TODO: should return task ID

	static void DeleteTask(TaskId_t taskId);

	static int FreeTaskPool();	//return number of deleted tasks

	static int	CheckQCondition();	//TODO: should return number of the active tasks in the Q

	static int ShowTaskList();
		//TODO: implement inerface for activation and deactivation tasks
};

