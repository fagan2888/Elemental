/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_SCALAR_DECL_HPP
#define CORE_SCALAR_DECL_HPP

namespace elem {

void AssertTypeKnown( ScalarTypes ntype );
void AssertTypeMatch( ScalarTypes A, ScalarTypes B );
void TypeCastError( ScalarTypes A, ScalarTypes B );

template <typename Int> class Scalar;
template <typename T,typename Int> const T& Get_( const Scalar<Int>& );
template <typename T,typename Int> T& Set_( const Scalar<Int>& );
template <typename T,typename Int> T Cast_( const Scalar<Int>& );

template <typename Int>
class Scalar
{
	public:
		Scalar();
		template <class T> 
		Scalar( const T& );
		Scalar( const Scalar<Int>& v );
		template <class T>
		Scalar<Int>& operator=( const T& v );
		Scalar<Int>& operator=( const Scalar<Int>& v );
			
		ScalarTypes Type() const;
	    void Print( std::ostream& os ) const;
		
		//
		// The difference between Get() and Cast() is that Get() requires the internal
		// type to be exactly what is requested, whereas Cast() will allow numeric
		// casting to the target type if no precision is lost.
		//
		
		template <typename T> T& Set();
		template <typename T> const T& Get() const;
		template <typename T> T Cast() const;
		
		//
		// No type checking. Dangerous. But template friends are such a pain that this
		// is how I'm implementing it. Don't use this.
		//
		
		template <typename T> const T& Get_() const;
	private:
		ScalarTypes type_;
		byte data_[sizeof(dcomplex)];
};

template <typename Int>
class Buffer;

template <typename Int>
class ConstBuffer
{
	public:
		ConstBuffer();
		ConstBuffer( const ConstBuffer<Int>& v );
		ConstBuffer( const Buffer<Int>& v );
		template <class T> ConstBuffer( const T* v );
		ConstBuffer<Int>& operator=( const ConstBuffer<Int>& v );
		ConstBuffer<Int>& operator=( const Buffer<Int>& v );
		template <class T> ConstBuffer<Int>& operator=( const T* v );
		
		ScalarTypes Type() const;
		
		template <class T> const T* Get() const;
		operator bool() const;
		
	private:
		ScalarTypes type_;
		const void* data_;
};

template <typename Int>
class Buffer
{
	public:
		Buffer();
		Buffer( const Buffer<Int>& v );
		template <class T> Buffer( T* v );
		Buffer<Int>& operator=( const Buffer<Int>& v );
		template <class T> Buffer<Int>& operator=( T* v );
		
		ScalarTypes Type() const;
		
		template <class T> T* Get() const;
		operator bool() const;
		
	private:
		friend class ConstBuffer<Int>;
		ScalarTypes type_;
		void* data_;
};

} // namespace elem

#endif // ifndef CORE_SCALAR_DECL_HPP
