/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hpp"

std::mutex ThreadSafeIOStream::_mutex;

thread_local ThreadSafeIOStream threadSafeCout;

ThreadSafeIOStream::ThreadSafeIOStream()
{
}

ThreadSafeIOStream::~ThreadSafeIOStream()
{
    flushPending();
}

/*
    Sets the prefix printed before each line.
    Called by the user (or later by Thread with the thread name).
*/
void ThreadSafeIOStream::setPrefix(const std::string& prefix)
{
    _prefix = prefix;
}

/*
    Handles stream manipulators (std::endl, std::flush, ...).

    std::endl completes the pending line: it is printed with its prefix
    and std::cout is flushed. Any other manipulator is applied to
    std::cout directly under lock after flushing the pending content.
*/
ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&))
{
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
    {
        flushPending();
        std::lock_guard<std::mutex> lock(_mutex);
        std::cout.flush();
    }
    else
    {
        flushPending();
        std::lock_guard<std::mutex> lock(_mutex);
        std::cout << manip;
    }
    return *this;
}

/*
    Atomically writes one complete line (prefix + content + newline).
*/
void ThreadSafeIOStream::flushLine(const std::string& line)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << _prefix << line << std::endl;
}

/*
    Flushes the accumulated content that has no trailing newline yet.
*/
void ThreadSafeIOStream::flushPending()
{
    if (_buffer.empty())
        return;
    flushLine(_buffer);
    _buffer.clear();
}
