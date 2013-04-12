/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_SCALAR_IMPL_HPP
#define CORE_SCALAR_IMPL_HPP

namespace elem {

//
// This should be a static, compile-time error, actually. If we can figure out a clean
// way to do it, we should. (boost::static_assert is not clean, though perhaps if we 
// duplicated the key aspect of its approach...)
//

inline void AssertTypeKnown( ScalarTypes type )
{
	if ( type == UNKNOWN )
		throw std::logic_error( "enum::Scalar: attempted to assign an supported type" );
}

template <typename Int> inline
Scalar<Int>::Scalar() :
type_(UNKNOWN) {}

template <typename Int>
template <typename T> inline
Scalar<Int>&
Scalar<Int>::operator=( const T& v )
{
	AssertTypeKnown( ScalarType<T,Int>::Enum );
	type_ = ScalarType<T,Int>::Enum;
	*reinterpret_cast<T*>(&data_) = v;
	return *this;
}

template <typename Int> inline
Scalar<Int>& 
Scalar<Int>::operator=( const Scalar<Int>& v )
{
	type_ = v.type_;
	*reinterpret_cast<dcomplex*>(&data_) = *reinterpret_cast<const dcomplex*>(&v.data_);
	return *this;
}

template <typename Int>
template <typename T> inline
Scalar<Int>::Scalar( const T& v )
{ operator=( v ); }

template <typename Int> inline
Scalar<Int>::Scalar( const Scalar& v )
{ operator=( v ); }

template <typename Int> inline
ScalarTypes 
Scalar<Int>::Type() const
{ return type_; }

template <typename Int>
template <typename T> inline
T& 
Scalar<Int>::Set()
{
	AssertTypeKnown( ScalarType<T,Int>::Enum );
	type_ = ScalarType<T,Int>::Enum;
	return *reinterpret_cast<T*>(&data_);
}

template <typename Int>	
template <typename T> inline
const T& 
Scalar<Int>::Get() const
{
	AssertTypeMatch( type_, ScalarType<T,Int>::Enum );
	return *reinterpret_cast<const T*>(&data_);
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

} // namespace elem

#endif // ifndef CORE_SCALAR_IMPL_HPP

