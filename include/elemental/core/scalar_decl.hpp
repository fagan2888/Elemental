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

template <typename Int>
class Scalar
{
	public:
		// Note: assumes a POD object. In C++11 we can test for this.
		
		Scalar();
		Scalar( const Scalar& v );
		template <class T> Scalar( const T& v );
		Scalar& operator=( const Scalar& v );
		template <class T> Scalar& operator=( const T& val );
		
		ScalarTypes Type() const;
		
		//
		// The difference between Get() and Cast() is that Get() requires the internal
		// type to be exactly what is requested, whereas Cast() will allow numeric
		// casting to the target type if no precision is lost.
		//
		
		template <class T> T& Set();
		template <class T> const T& Get() const;
		template <class T> T Cast() const;
		
	private:
		ScalarTypes type_;
		byte data_[sizeof(dcomplex)];
};

} // namespace elem

#endif // ifndef CORE_SCALAR_DECL_HPP
