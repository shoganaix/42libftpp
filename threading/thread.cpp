/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread.hpp"

#include <stdexcept>
#include "../iostream/thread_safe_iostream.hpp"

Thread::Thread(const std::string& name, std::function<void()> functToExecute)
    : _name(name),
      _funct(functToExecute),
      _started(false)
{
}

Thread::~Thread()
{
    stop();
}

/*
    Launches the thread. Throws when already started.
*/
void Thread::start()
{
    if (_started)
        throw std::runtime_error("Thread: already started");
    _started = true;
    _thread = std::thread(&Thread::run, this);
}

/*
    Joins the thread. Safe to call when never started or already stopped.
*/
void Thread::stop()
{
    if (_thread.joinable())
        _thread.join();
    _started = false;
}

const std::string& Thread::name() const
{
    return _name;
}

/*
    Entry point: tags this thread's iostream with the thread name,
    then runs the user function.
*/
void Thread::run()
{
    threadSafeCout.setPrefix("[" + _name + "] ");
    _funct();
}
