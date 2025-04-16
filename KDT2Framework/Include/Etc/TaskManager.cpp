#include "TaskManager.h"

DEFINITION_SINGLE(CTaskManager);

CTaskManager::CTaskManager()
{

}

CTaskManager::~CTaskManager()
{

}

int CTaskManager::AddTask(std::thread&& task)
{
    taskNumber++;
    int taskID = taskNumber;
    CLog::PrintLog("CTaskManager::AddTask taskStart: " + std::to_string(taskID));

    mThreadTasks.insert(std::make_pair(taskID, std::move(task)));

    //if (mThreadTasks[taskID].joinable())
    //    mThreadTasks[taskID].join();
    mThreadTasks[taskID].detach();
    return taskID;
}

void CTaskManager::RemoveTask(int taskID)
{
    CLog::PrintLog("CTaskManager::RemoveTask taskEnd: " + std::to_string(taskID));
    int count = mThreadTasks.count(taskID);
    CLog::PrintLog("CTaskManager::RemoveTask mThreadTasks.count(taskID): " + std::to_string(count));
    
    if (mThreadTasks.count(taskID) > 0)
    {
        mThreadTasks.erase(taskID);
    }
}

