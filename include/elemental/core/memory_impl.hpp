/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_MEMORY_IMPL_HPP
#define CORE_MEMORY_IMPL_HPP

namespace elem {

template <typename Int>
inline 
AutoMemory<Int>::AutoMemory( std::size_t chunk )
: chunk_(chunk), size_(0), buffer_(0)
{ }

template <typename Int>
inline 
AutoMemory<Int>::~AutoMemory()
{ delete[] buffer_; }

template <typename Int>
inline char* 
AutoMemory<Int>::Buffer() const
{ return buffer_; }

template <typename Int>
inline char*
AutoMemory<Int>::Buffer( Int i ) const
{ return buffer_ + i * chunk_; }

template <typename Int>
inline size_t
AutoMemory<Int>::Chunk() const
{ return chunk_; }

template <typename Int>
inline std::size_t 
AutoMemory<Int>::Size() const
{ return size_; }

template <typename Int>
inline char* 
AutoMemory<Int>::Require( std::size_t size )
{
    if( size > size_ )
    {
        delete[] buffer_;
#ifndef RELEASE
        try {
#endif
        buffer_ = new char[chunk_*size];
#ifndef RELEASE
        } 
        catch( std::bad_alloc& exception )
        {
            std::cerr << "Failed to allocate " << size*chunk_
                      << " bytes" << std::endl;
            throw exception;
        }
#endif
        size_ = size;
    }
    return buffer_;
}

template <typename Int>
inline void 
AutoMemory<Int>::Release()
{
#ifndef POOL_AutoMemory
    this->Empty();
#endif
}

template <typename Int>
inline void 
AutoMemory<Int>::Empty()
{
    delete[] buffer_;
    size_ = 0;
    buffer_ = 0;
}

template <typename T,typename Int>
Memory<T,Int>::Memory()
: AutoMemory<Int>( sizeof(T) )
{
}

template <typename T,typename Int>
T*
Memory<T,Int>::Buffer() const
{
	return static_cast<T*>( AutoMemory<Int>::Buffer() );
}

template <typename T,typename Int>
T*
Memory<T,Int>::Buffer( Int i ) const
{
	return static_cast<T*>( AutoMemory<Int>::Buffer(i) );
}

} // namespace elem

#endif // ifndef CORE_MEMORY_IMPL_HPP
