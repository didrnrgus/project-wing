#pragma once

#include "GameInfo.h"

class CTaskManager
{
private:
	std::map<int, std::thread> mThreadTasks;
	int taskNumber = 0;
public:
	int AddTask(std::thread&& task);
	void RemoveTask(int taskID);
private:
	DECLARE_SINGLE(CTaskManager);
};

