#pragma once

#include "GameInfo.h"

class CTaskManager
{
private:
	std::map<int, std::thread> mThreadTasks;
	int taskNumber = 0;
public:
	void AddTask(std::thread&& task);

private:
	DECLARE_SINGLE(CTaskManager);
};

