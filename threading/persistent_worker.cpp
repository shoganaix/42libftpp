/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "persistent_worker.hpp"

#include <chrono>
#include <stdexcept>
#include <utility>
#include <vector>
#include "../iostream/thread_safe_iostream.hpp"

PersistentWorker::PersistentWorker()
    : _running(false),
      _started(false)
{
}

PersistentWorker::~PersistentWorker()
{
    stop();
}

/*
    Launches the loop thread. Throws when already started.
*/
void PersistentWorker::start()
{
    if (_started)
        throw std::runtime_error("PersistentWorker: already started");
    _started = true;
    _running = true;
    _thread = std::thread(&PersistentWorker::loop, this);
}

/*
    Signals the loop to finish and joins it. Safe to call when
    never started or already stopped.
*/
void PersistentWorker::stop()
{
    _running = false;
    if (_thread.joinable())
        _thread.join();
    _started = false;
}

/*
    Inserts a task into the worker's pool, assigning it a name.
    Replaces the task when the name already exists. Thread-safe:
    works while the worker is running.
*/
void PersistentWorker::addTask(const std::string& name,
    const std::function<void()>& jobToExecute)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _tasks[name] = jobToExecute;
}

/*
    Removes a task from the worker's pool. Unknown names are
    silently ignored.
*/
void PersistentWorker::removeTask(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _tasks.erase(name);
}

/*
    Runs every registered task on each round. Sleeps between rounds
    (longer when idle) to avoid busy-spinning.
*/
void PersistentWorker::loop()
{
    threadSafeCout.setPrefix("[PersistentWorker] ");

    while (_running)
    {
        std::vector<std::function<void()> > snapshot;

        {
            std::lock_guard<std::mutex> lock(_mutex);
            for (std::map<std::string, std::function<void()> >::iterator it = _tasks.begin();
                it != _tasks.end(); ++it)
            {
                snapshot.push_back(it->second);
            }
        }

        if (snapshot.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        for (std::vector<std::function<void()> >::iterator it = snapshot.begin();
            it != snapshot.end() && _running; ++it)
        {
            (*it)();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
