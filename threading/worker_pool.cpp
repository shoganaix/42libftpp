/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "worker_pool.hpp"

#include <chrono>
#include <sstream>
#include <stdexcept>
#include "../iostream/thread_safe_iostream.hpp"

WorkerPool::IJobs::~IJobs()
{
}

/*
    Internal adapter: turns a std::function into an IJobs.
*/
class FunctionJob : public WorkerPool::IJobs
{
public:
    FunctionJob(const std::function<void()>& job)
        : _job(job)
    {
    }

    void execute()
    {
        _job();
    }

private:
    std::function<void()> _job;
};

WorkerPool::WorkerPool()
    : _workerCount(std::thread::hardware_concurrency()),
      _running(false),
      _started(false)
{
    if (_workerCount == 0)
        _workerCount = 4;
}

WorkerPool::~WorkerPool()
{
    stop();
}

/*
    Launches the workers. Throws when already started.
    Workers run perpetually until stop() is called.
*/
void WorkerPool::start()
{
    if (_started)
        throw std::runtime_error("WorkerPool: already started");
    _started = true;
    _running = true;

    for (std::size_t i = 0; i < _workerCount; ++i)
    {
        std::ostringstream name;
        name << "WorkerPool-" << i;
        _workers.push_back(std::thread(&WorkerPool::workerLoop, this, name.str()));
    }
}

/*
    Signals workers to finish and joins them. Safe to call when
    never started or already stopped. Pending jobs are kept.
*/
void WorkerPool::stop()
{
    _running = false;
    for (std::vector<std::thread>::iterator it = _workers.begin();
        it != _workers.end(); ++it)
    {
        if (it->joinable())
            it->join();
    }
    _workers.clear();
    _started = false;
}

/*
    Inserts a new job to be executed by the pool. Works both
    before and after start(): queued jobs wait for running workers.
*/
void WorkerPool::addJob(const std::function<void()>& jobToExecute)
{
    std::shared_ptr<IJobs> job(new FunctionJob(jobToExecute));
    _jobs.push_back(job);
}

std::size_t WorkerPool::workerCount() const
{
    return _workerCount;
}

/*
    Perpetual loop: pulls jobs from the queue and executes them.
    The ThreadSafeQueue has no blocking pop, so an empty queue
    sleeps briefly instead of busy-spinning.
*/
void WorkerPool::workerLoop(const std::string& name)
{
    threadSafeCout.setPrefix("[" + name + "] ");

    while (_running)
    {
        try
        {
            std::shared_ptr<IJobs> job = _jobs.pop_front();
            job->execute();
        }
        catch (const std::exception&)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}
