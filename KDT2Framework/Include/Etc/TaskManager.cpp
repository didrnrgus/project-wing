#include "TaskManager.h"
#include "Share/Log.h"

DEFINITION_SINGLE(CTaskManager);

CTaskManager::CTaskManager()
{

}

CTaskManager::~CTaskManager()
{

}

void CTaskManager::AddTask(std::thread&& task)
{
    taskNumber++;
    int taskID = taskNumber;
    CLog::PrintLog("CTaskManager::AddTask taskStart: " + std::to_string(taskID));

    mThreadTasks.insert(std::make_pair(taskID, std::move(task)));

    if (mThreadTasks[taskID].joinable())
        mThreadTasks[taskID].join();

    mThreadTasks.erase(taskID);

    CLog::PrintLog("CTaskManager::AddTask taskEnd: " + std::to_string(taskID));
}

