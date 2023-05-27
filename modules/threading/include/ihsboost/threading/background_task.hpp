/**
 * @file background_task.hpp
 * @author Eliot Hall
 * @brief Background task class
 * @version 0.1
 * @date 2023-03-15
 *
 * @copyright Copyright (c) 2023
 * @addtogroup threading_id
 * @{
 */

#ifndef IHSBOOST_BACKGROUND_TASK_HPP
#define IHSBOOST_BACKGROUND_TASK_HPP
#include "threadable.hpp"

/**
 * @brief A class that is used for background tasks that have a set
 * number of updates to do every second
 *
 */
class BackgroundTask {
  public:
    /**
     * @brief Construct a new Background Task object
     *
     * @param updates_per_sec how many updates it should do each second
     */
    BackgroundTask(int updates_per_sec);

    /**
     * @brief Destroy the Background Task object, waiting for the thread to
     * finish
     *
     */
    virtual ~BackgroundTask();

    /**
     * @brief Start the Background task
     *
     */
    virtual void start();

    /**
     * @brief Stop the background task, waiting for the thread to finish
     *
     */
    virtual void stop();

    /**
     * @brief Whether or not the thread is running
     *
     * @return true - if it is running
     * @return false - if it isn't running
     */
    virtual bool is_running();

    /**
     * @brief Get how long the task should sleep for
     *
     * @return int
     */
    virtual int get_msleep_time();

    /**
     * @brief Set how many updates the task should do every second
     *
     * @param updates_per_sec
     */
    virtual void set_updates_per_sec(int updates_per_sec);

  protected:
    /**
     * @brief Function that should be called by this task
     *
     */
    virtual void function() = 0;

  private:
    /**
     * @brief The function that runs the task
     *
     */
    void run_function();

    volatile bool running;
    Threadable *t;
    int msleep_time;
};

#endif
/**@}*/