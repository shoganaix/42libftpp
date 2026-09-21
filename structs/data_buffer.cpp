/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 18:45:10 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/21 19:47:52 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data_buffer.hpp"

/*
    Initializes an empty DataBuffer with a read position set to 0
*/
DataBuffer::DataBuffer()
    : _readPosition(0)
{
}

DataBuffer::~DataBuffer()
{
}
/*
    Creates new DataBuffer as a copy of an existing one, duplicating its internal buffer and readPosition
*/
DataBuffer::DataBuffer(const DataBuffer& other)
    : _buffer(other._buffer),
      _readPosition(other._readPosition)
{
}
/*
    Assigns the contents of another DataBuffer to this one, duplicating its internal buffer and readPosition
*/
DataBuffer& DataBuffer::operator=(const DataBuffer& other)
{
    if (this != &other)
    {
        _buffer = other._buffer;
        _readPosition = other._readPosition;
    }

    return *this;
}

/*
    Clears buffer and resets readPosition
*/
void DataBuffer::clear()
{
    _buffer.clear();
    _readPosition = 0;
}

std::size_t DataBuffer::size() const
{
    return _buffer.size();
}


bool DataBuffer::empty() const
{
    return _buffer.empty();
}
