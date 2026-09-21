/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>

template <typename TType>
ThreadSafeQueue<TType>::ThreadSafeQueue()
{
}

template <typename TType>
ThreadSafeQueue<TType>::~ThreadSafeQueue()
{
}

template <typename TType>
void ThreadSafeQueue<TType>::push_back(const TType& newElement)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_back(newElement);
}

template <typename TType>
void ThreadSafeQueue<TType>::push_front(const TType& newElement)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_front(newElement);
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_back()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_queue.empty())
        throw std::runtime_error("ThreadSafeQueue: pop from empty queue");
    TType value = _queue.back();
    _queue.pop_back();
    return value;
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_front()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_queue.empty())
        throw std::runtime_error("ThreadSafeQueue: pop from empty queue");
    TType value = _queue.front();
    _queue.pop_front();
    return value;
}

template <typename TType>
std::size_t ThreadSafeQueue<TType>::size() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.size();
}

template <typename TType>
bool ThreadSafeQueue<TType>::empty() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.empty();
}
