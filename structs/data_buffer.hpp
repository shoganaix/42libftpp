/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 18:10:42 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/21 19:55:49 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <cstddef>      // std::size_t
#include <sstream>      // std::ostringstream, std::istringstream
#include <stdexcept>    // std::runtime_error
#include <string>       // std::string
#include <vector>       // std::vector

/*
    This class implements a simple data buffer that can store and retrieve
    objects of various types using serialization and deserialization

    NOTE: deserialization uses stream extraction (operator>>), so values
    containing whitespace (e.g. std::string with spaces) do not round-trip:
    only the first whitespace-delimited token is recovered.
*/

class DataBuffer
{
public:
    DataBuffer();
    ~DataBuffer();

    DataBuffer(const DataBuffer& other);
    DataBuffer& operator=(const DataBuffer& other);

    // Overload << and >>: (CONST & NON-CONST respectively)
        // buffer << data instead of buffer.push_back(data)
        // buffer >> data, instead, reads from buffer and reconstructs data

    template <typename TType>
    DataBuffer& operator<<(const TType& data);
    
    template <typename TType>
    DataBuffer& operator>>(TType& data);


    void clear();               // empties buffer + resets read position
    std::size_t size() const;   // returns number of bytes stored in buffer
    bool empty() const;         // returns true if buffer empty, false otherwise

private:
    std::vector<char> _buffer;  // internal storage for serialized data
    std::size_t _readPosition;  // position in buffer
};

#include "data_buffer.tpp"      // Include template implementation
// * Note: Compiler needs to know the complete definition of template at the point of instantiation

#endif

