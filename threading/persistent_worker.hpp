/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERSISTENT_WORKER_HPP
#define PERSISTENT_WORKER_HPP

#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>

/*
    PersistentWorker: a thread that continuously performs a set of
    user-defined tasks in a loop.

    Tasks are stored by name. addTask() inserts (or replaces) a task,
    removeTask() drops it (silently when the name is unknown). The loop
    runs every task on each round; with no tasks it sleeps instead of
    busy-spinning. The task list is copied under lock per round, so
    tasks run without holding the mutex.

    Extra start()/stop() (same style as Thread): start launches the
    loop, stop joins it. The destructor stops a running worker.

    Example:
        PersistentWorker worker;
        worker.addTask("tick", []() { ... });
        worker.start();
        worker.removeTask("tick");
        worker.stop();
*/

class PersistentWorker
{
public:
    PersistentWorker();
    ~PersistentWorker();

    PersistentWorker(const PersistentWorker&) = delete;
    PersistentWorker& operator=(const PersistentWorker&) = delete;

    void start();
    void stop();
    void addTask(const std::string& name, const std::function<void()>& jobToExecute);
    void removeTask(const std::string& name);

private:
    void loop();

    std::map<std::string, std::function<void()> > _tasks;
    std::mutex _mutex;
    std::thread _thread;
    std::atomic<bool> _running;
    bool _started;
};

#endif
