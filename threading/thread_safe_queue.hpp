/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <cstddef>
#include <deque>
#include <mutex>

/*
    ThreadSafeQueue templated by TType.

    A thread-safe double-ended queue. Every operation locks an internal
    mutex, so producers and consumers can share one instance safely.

    Note: the subject shows pop_back/pop_front with odd signatures
    ("TType pop_back(const & newElement)"). They are implemented as
    plain getters (TType pop_back() / TType pop_front()), which is the
    only sensible reading: popping from an empty queue throws.

    Example:
        ThreadSafeQueue<int> queue;
        queue.push_back(1);
        queue.push_front(0);
        int value = queue.pop_front(); // 0
*/

template <typename TType>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue();
    ~ThreadSafeQueue();

    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void push_back(const TType& newElement);
    void push_front(const TType& newElement);
    TType pop_back();
    TType pop_front();
    std::size_t size() const;
    bool empty() const;

private:
    std::deque<TType> _queue;
    mutable std::mutex _mutex;
};

#include "thread_safe_queue.tpp"

#endif
