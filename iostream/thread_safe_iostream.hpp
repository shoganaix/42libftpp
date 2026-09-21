/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_IOSTREAM_HPP
#define THREAD_SAFE_IOSTREAM_HPP

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

/*
    ThreadSafeIOStream: thread-safe version of iostream with prefixed lines.

    Each thread uses its own instance (see threadSafeCout below), so the
    accumulation buffer needs no locking. Only the moment a complete line
    is written to std::cout is protected by a shared static mutex, which
    guarantees lines from different threads never get interleaved.

    A line is flushed (with its prefix) every time a '\n' is detected in
    the accumulated input or std::endl is used.

    Example:
        threadSafeCout.setPrefix("[Server] ");
        threadSafeCout << "Hello" << std::endl;
        // prints: "[Server] Hello\n"

        int value = 0;
        threadSafeCout.prompt("Enter a number: ", value);
        threadSafeCout << "You entered " << value << std::endl;
*/

class ThreadSafeIOStream
{
public:
    ThreadSafeIOStream();
    ~ThreadSafeIOStream();

    ThreadSafeIOStream(const ThreadSafeIOStream&) = delete;
    ThreadSafeIOStream& operator=(const ThreadSafeIOStream&) = delete;

    void setPrefix(const std::string& prefix);

    template <typename TType>
    ThreadSafeIOStream& operator<<(const TType& value);

    ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

    template <typename TType>
    ThreadSafeIOStream& operator>>(TType& dest);

    template <typename TType>
    void prompt(const std::string& question, TType& dest);

private:
    void flushLine(const std::string& line);
    void flushPending();

    static std::mutex _mutex;
    std::string _prefix;
    std::string _buffer;
};

template <typename TType>
ThreadSafeIOStream& ThreadSafeIOStream::operator<<(const TType& value)
{
    std::ostringstream stream;

    stream << value;
    _buffer += stream.str();

    std::string::size_type pos = _buffer.find('\n');
    while (pos != std::string::npos)
    {
        flushLine(_buffer.substr(0, pos));
        _buffer.erase(0, pos + 1);
        pos = _buffer.find('\n');
    }
    return *this;
}

template <typename TType>
ThreadSafeIOStream& ThreadSafeIOStream::operator>>(TType& dest)
{
    flushPending();

    std::lock_guard<std::mutex> lock(_mutex);
    std::cin >> dest;
    return *this;
}

template <typename TType>
void ThreadSafeIOStream::prompt(const std::string& question, TType& dest)
{
    *this << question;
    flushPending();

    std::lock_guard<std::mutex> lock(_mutex);
    std::cin >> dest;
}

extern thread_local ThreadSafeIOStream threadSafeCout;

#endif
