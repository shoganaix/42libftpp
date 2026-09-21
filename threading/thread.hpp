/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_HPP
#define THREAD_HPP

#include <functional>
#include <string>
#include <thread>

/*
    Thread: a named wrapper for std::thread with deferred start.

    The constructor only stores the name and the function. The thread
    is launched with start() and joined with stop(). On start, the
    thread sets its ThreadSafeIOStream prefix to "[name] " so everything
    it prints is tagged (this is the hint from the subject).

    The destructor joins the thread when still running, otherwise
    std::thread would call std::terminate.

    Example:
        Thread worker("Loader", []() {
            threadSafeCout << "loading..." << std::endl;
        });
        worker.start();
        worker.stop();
*/

class Thread
{
public:
    Thread(const std::string& name, std::function<void()> functToExecute);
    ~Thread();

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    void start();
    void stop();
    const std::string& name() const;

private:
    void run();

    std::string _name;
    std::function<void()> _funct;
    std::thread _thread;
    bool _started;
};

#endif
