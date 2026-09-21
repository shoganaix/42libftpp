/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_threading.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_threading.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include "../threading/threading.hpp"
#include "../iostream/thread_safe_iostream.hpp"

/*
    Waits until a predicate turns true or a timeout expires.
    Avoids fixed sleeps that make tests flaky on slow machines.
*/
template <typename TPred>
static bool waitFor(TPred predicate, int timeoutMs)
{
    int waited = 0;

    while (!predicate() && waited < timeoutMs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        waited += 5;
    }
    return predicate();
}

void testThreading()
{
    threadSafeCout.setPrefix("[Main] ");
    threadSafeCout << "\n--- THREADING TEST ---\n";

    /*
        TEST 1:
        ThreadSafeQueue: both ends, order and empty-pop exception.
    */
    threadSafeCout << "Testing ThreadSafeQueue..." << std::endl;

    ThreadSafeQueue<int> queue;

    queue.push_back(1);
    queue.push_back(2);
    queue.push_front(0);

    threadSafeCout << "pop_front (expected 0): " << queue.pop_front() << std::endl;
    threadSafeCout << "pop_front (expected 1): " << queue.pop_front() << std::endl;
    threadSafeCout << "pop_back (expected 2): " << queue.pop_back() << std::endl;

    try
    {
        queue.pop_front();
        threadSafeCout << "Unexpected: no exception" << std::endl;
    }
    catch (const std::exception& e)
    {
        threadSafeCout << "Expected exception: " << e.what() << std::endl;
    }

    /*
        TEST 2:
        ThreadSafeQueue shared between producer and consumer threads.
    */
    threadSafeCout << "\nTesting queue producer/consumer..." << std::endl;

    ThreadSafeQueue<int> shared;
    std::atomic<int> consumed(0);

    std::thread producer([&shared]() {
        for (int i = 0; i < 100; ++i)
            shared.push_back(i);
    });
    std::thread consumer([&shared, &consumed]() {
        while (consumed < 100)
        {
            try
            {
                shared.pop_front();
                ++consumed;
            }
            catch (const std::exception&)
            {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();

    threadSafeCout << "Consumed (expected 100): " << consumed.load() << std::endl;

    /*
        TEST 3:
        Thread: deferred start, prefixed output, double start throws.
    */
    threadSafeCout << "\nTesting Thread..." << std::endl;

    std::atomic<bool> ran(false);

    Thread worker("Loader", [&ran]() {
        threadSafeCout << "loading done" << std::endl;
        ran = true;
    });

    worker.start();

    try
    {
        worker.start();
        threadSafeCout << "Unexpected: no exception" << std::endl;
    }
    catch (const std::exception& e)
    {
        threadSafeCout << "Expected exception: " << e.what() << std::endl;
    }

    worker.stop();

    threadSafeCout << "Thread ran (expected yes): " << (ran ? "yes" : "no") << std::endl;

    /*
        TEST 4:
        WorkerPool: jobs run on workers, including jobs queued pre-start.
    */
    threadSafeCout << "\nTesting WorkerPool..." << std::endl;

    WorkerPool pool;

    threadSafeCout << "Workers: " << pool.workerCount() << std::endl;

    std::atomic<int> jobsDone(0);

    for (int i = 0; i < 20; ++i)
    {
        pool.addJob([&jobsDone]() {
            ++jobsDone;
        });
    }

    pool.start();

    bool poolOk = waitFor([&jobsDone]() { return jobsDone.load() == 20; }, 5000);

    threadSafeCout << "Jobs done (expected 20): " << jobsDone.load()
        << (poolOk ? "" : " TIMEOUT") << std::endl;

    pool.stop();

    /*
        TEST 5:
        PersistentWorker: repeating tasks, then removal stops them.
    */
    threadSafeCout << "\nTesting PersistentWorker..." << std::endl;

    PersistentWorker persistent;
    std::atomic<int> ticks(0);
    std::atomic<int> tocks(0);

    persistent.addTask("tick", [&ticks]() { ++ticks; });
    persistent.addTask("tock", [&tocks]() { ++tocks; });
    persistent.start();

    waitFor([&ticks]() { return ticks.load() > 0; }, 2000);
    waitFor([&tocks]() { return tocks.load() > 0; }, 2000);

    threadSafeCout << "Ticks (>0 expected): " << ticks.load() << std::endl;
    threadSafeCout << "Tocks (>0 expected): " << tocks.load() << std::endl;

    persistent.removeTask("tick");

    int ticksFrozen = ticks.load();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    threadSafeCout << "Tick stopped (expected yes): "
        << (ticks.load() == ticksFrozen ? "yes" : "no") << std::endl;
    threadSafeCout << "Tock keeps running (expected yes): "
        << (tocks.load() > 0 ? "yes" : "no") << std::endl;

    persistent.removeTask("unknown-name");
    persistent.stop();

    threadSafeCout.setPrefix("[Main] ");

    threadSafeCout << "\n--- END THREADING TEST ---\n" << std::endl;
}
