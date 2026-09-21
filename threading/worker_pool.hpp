/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "thread_safe_queue.hpp"

/*
    WorkerPool: manages worker threads that execute jobs.

    IJobs describes the interface of a job to be executed by the pool.
    addJob() accepts a plain function, which is wrapped in an internal
    IJobs adapter, so users never deal with IJobs directly:

        WorkerPool pool;
        pool.start();
        pool.addJob([]() { ... });
        pool.stop();

    The pool sizes itself with std::thread::hardware_concurrency()
    (fallback: 4 workers when it returns 0). Workers run perpetually
    while started, pulling jobs from an internal ThreadSafeQueue.
    Jobs added before start() are kept and run once started.
    The destructor stops and joins every worker.
*/

class WorkerPool
{
public:
    class IJobs
    {
    public:
        virtual ~IJobs();
        virtual void execute() = 0;
    };

    WorkerPool();
    ~WorkerPool();

    WorkerPool(const WorkerPool&) = delete;
    WorkerPool& operator=(const WorkerPool&) = delete;

    void start();
    void stop();
    void addJob(const std::function<void()>& jobToExecute);
    std::size_t workerCount() const;

private:
    void workerLoop(const std::string& name);

    std::size_t _workerCount;
    std::vector<std::thread> _workers;
    ThreadSafeQueue<std::shared_ptr<IJobs> > _jobs;
    std::atomic<bool> _running;
    bool _started;
};

#endif
