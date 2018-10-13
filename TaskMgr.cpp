#include "TaskMgr.h"
#include <time.h>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace std::chrono;

deque<TaskEntry> TaskMgr::taskPool;

//void TaskMgr::CreateTask(string name, Repeatable repeat, int repeatEvery)
void TaskMgr::CreateTask(TaskEntry newEntry)
{
	taskPool.push_back(newEntry);
	sort(taskPool.begin(), taskPool.end());
	cout << "Task created" << endl;
}

int TaskMgr::FreeTaskPool()
{
	int poolSize = taskPool.size();
	taskPool.clear();
	return poolSize;
}

int TaskMgr::CheckQCondition()
{
	if (taskPool.size() == 0) 
	{
		//cout << "Task pool is FREE\tTIME: " << time(&curTime) << endl;
		return -1;
	}

	for (TaskEntry &task : taskPool)
	{
		if (task.GetTimeToStart() <= CurrentTime())
		{
			//There should be command dechiping and command execution
			cout << "EXECUTION:\t" << task;
			task.ResetTimeToStart();
		}
	}
	// It is not nessesary to sort in anywhere
	sort(taskPool.begin(), taskPool.end());
	return TaskMgr::taskPool.size();
}

int TaskMgr::ShowTaskList()
{
	int taskPoolSize = taskPool.size();
	if (taskPoolSize == 0)
	{
		cout << endl << "TASK POOL IS FREE" << endl;
	}
	else 
	{
		cout << endl << "\tTASK POOL START" << endl;
		for (auto task : taskPool)
		{
			cout << task;
		}
		cout << "\tTASK POOL END" << endl;
	}
	return taskPoolSize;
}
