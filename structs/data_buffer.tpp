/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.tpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 18:11:26 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/21 20:00:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    operator<<()

    Serializes object using stream operator (<<) and appends it to buffer

        TType
          |
          | operator <<
          v
    string stream
          |
          v
       insert bytes into buffer
          |
          v
    [ object A ][\0][ object B ][\0][ object C ][\0]
*/
template <typename TType>DataBuffer& DataBuffer::operator<<(const TType& data)
{
    std::ostringstream stream;

    stream << data;

    if (stream.fail())
        throw std::runtime_error("DataBuffer: serialization failed");

    const std::string serialized = stream.str();

    _buffer.insert(_buffer.end(), serialized.begin(), serialized.end());
    _buffer.push_back('\0');

    return *this;
}


/*
    operator>>()

    Reads NEXT serialized object from buffer
        and reconstructs it using stream extraction (>>)

    [ object A ][\0][ object B ][\0][ object C ][\0]
         ^
         |
     _readPosition

         |
         | find next '\0'
         v
      serialized data
         |
         | operator>>
         v
        TType

    *Note*: After deserialization, _readPosition moves to the beginning
    of the next stored object
*/
template <typename TType>DataBuffer& DataBuffer::operator>>(TType& data)
{
    if (_readPosition >= _buffer.size())
        throw std::runtime_error("DataBuffer: no data available");

    std::size_t end = _readPosition;

    while (end < _buffer.size() && _buffer[end] != '\0')
        ++end;

    if (end == _buffer.size())
        throw std::runtime_error("DataBuffer: corrupted data");

    std::string serialized(
        _buffer.begin() + _readPosition,
        _buffer.begin() + end
    );

    std::istringstream stream(serialized);

    stream >> data;

    if (stream.fail())
        throw std::runtime_error("DataBuffer: deserialization failed");

    _readPosition = end + 1;

    return *this;
}