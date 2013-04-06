/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_MEMORY_DECL_HPP
#define CORE_MEMORY_DECL_HPP

namespace elem {

template <typename Int>
class AutoMemory
{
	std::size_t chunk_, size_;
	char* buffer_;
public:
	AutoMemory( std::size_t chunk );
	~AutoMemory();
	char* Buffer() const;
	char* Buffer( Int i ) const;
	size_t Size() const;
	size_t Chunk() const;
    char* Require( std::size_t size );
    void Release();
    void Empty();
};

template <typename T,typename Int>
class Memory : public AutoMemory<Int>
{
public:
	Memory();
    T* Buffer() const;
    T* Buffer( Int i ) const;
    T* Require( std::size_t size );
};

} // namespace elem

#endif // ifndef CORE_MEMORY_DECL_HPP
