#include "background_task.hpp"
#include <kipr/wombat.h>

BackgroundTask::BackgroundTask(int updates_per_sec) : running(false), t(nullptr), msleep_time(1000 / updates_per_sec){};
BackgroundTask::~BackgroundTask() { stop(); }

void BackgroundTask::start()
{
    if (t == nullptr)
    {
        running = true;
        t = new Threadable<void(BackgroundTask *), BackgroundTask *>(run_function, this);
    }
}

void BackgroundTask::stop()
{
    running = false;
    if (t != nullptr)
    {
        // wait till finished
        while (!(*t)())
            ;

        // cleanup
        delete t;
        t = nullptr;
    }
}

bool BackgroundTask::is_running() { return running; }
int BackgroundTask::get_msleep_time() { return msleep_time; }
void BackgroundTask::set_updates_per_sec(int updates_per_sec) { msleep_time = 1000 / updates_per_sec; }

void BackgroundTask::run_function(BackgroundTask *b)
{
    while (b->running)
    {
        b->function();
        msleep(b->msleep_time);
    }
}