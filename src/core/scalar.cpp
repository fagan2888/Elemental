/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#include "elemental-lite.hpp"

namespace elem {

void AssertTypeMatch( ScalarTypes A, ScalarTypes B )
{
	if ( A != B ) {
		throw std::logic_error( "enum::Scalar: type mismatch" );
	}
}

void TypeCastError( ScalarTypes A, ScalarTypes B )
{
	throw std::logic_error( "enum::Scalar: cannot cast between types" );
}

template <typename T,typename Int> inline
T Cast_( const Scalar<Int>& s )
{
	return TypeCastError( s.type_, ScalarType<T,Int>::Enum );
}

template <>
int Cast_<int,int>( const Scalar<int>& s )
{
	switch ( s.Type() ) {
	default:       TypeCastError( s.Type(), INTEGRAL );
	case INTEGRAL: return s.Get<int>();
	}
}

template <>
float Cast_<float,int>( const Scalar<int>& s )
{
	switch ( s.Type() ) {
	default:        TypeCastError( s.Type(), SINGLE );
	case SINGLE:    return s.Get<float>();
	}
}

template <>
double Cast_<double,int>( const Scalar<int>& s )
{
	switch ( s.Type() ) {
	default:        TypeCastError( s.Type(), DOUBLE );
	case INTEGRAL:  return s.Get<int>();
	case SINGLE:    return s.Get<float>();
	case DOUBLE:    return s.Get<double>();
	}
} 

template <>
scomplex Cast_<scomplex,int>( const Scalar<int>& s )
{
	switch ( s.Type() ) {
	default:       TypeCastError( s.Type(), SCOMPLEX );
	case SINGLE:   return s.Get<float>();
	case SCOMPLEX: return s.Get<scomplex>();
	}
}

template <>
dcomplex Cast_<dcomplex,int>( const Scalar<int>& s )
{
	switch ( s.Type() ) {
	default:        TypeCastError( s.Type(), DCOMPLEX );
	case INTEGRAL:  return s.Get<int>();
	case SINGLE:    return s.Get<float>();
	case DOUBLE:    return s.Get<double>();
	case SCOMPLEX:  return s.Get<scomplex>();
	case DCOMPLEX:  return s.Get<dcomplex>();
	}
}

template <typename Int>
template <typename T> inline
T 
Scalar<Int>::Cast() const
{
	AssertTypeKnown( ScalarType<T,Int>::Enum );
	return Cast_<T,Int>( *this );
}

template class Scalar<int>;
template int& Scalar<int>::Set<int>();
template const int& Scalar<int>::Get<int>() const;
template int Scalar<int>::Cast<int>() const;
template float& Scalar<int>::Set<float>();
template const float& Scalar<int>::Get<float>() const;
template float Scalar<int>::Cast<float>() const;
template double& Scalar<int>::Set<double>();
template const double& Scalar<int>::Get<double>() const;
template double Scalar<int>::Cast<double>() const;
template scomplex& Scalar<int>::Set<scomplex>();
template const scomplex& Scalar<int>::Get<scomplex>() const;
template scomplex Scalar<int>::Cast<scomplex>() const;
template dcomplex& Scalar<int>::Set<dcomplex>();
template const dcomplex& Scalar<int>::Get<dcomplex>() const;
template dcomplex Scalar<int>::Cast<dcomplex>() const;

} // namespace elem
