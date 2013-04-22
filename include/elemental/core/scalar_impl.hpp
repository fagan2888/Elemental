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

#if __cplusplus < 201103L 
inline void static_assert( bool test, const char* str )
{ if ( !test ) throw std::logic_error( str ); }
#endif

template <typename Int>
Scalar<Int>::Scalar()
{}

template <typename Int> 
template <class T> inline
Scalar<Int>::Scalar( const T& v )
{ 
	Set<T>() = v; 
}

template <typename Int> 
template <class T> inline
Scalar<Int>& Scalar<Int>::operator=( const T& v )
{ 
	Set<T>() = v; 
	return *this; 
}

template <typename Int> inline
ScalarTypes 
Scalar<Int>::Type() const
{ 
	return type_; 
}

template <typename Int>
template <typename T>
const T& Scalar<Int>::Get() const
{ 
	static_assert( ScalarType<T,Int>::Enum != UNKNOWN, 
		"Scalar value retrieval not supported for this type" );
	AssertTypeMatch( type_, ScalarType<T,Int>::Enum );
	return *reinterpret_cast<const T*>(reinterpret_cast<const void*>(&data_[0]));
}

template <typename Int>
template <typename T>
const T& Scalar<Int>::Get_() const
{ 
	return *reinterpret_cast<const T*>(reinterpret_cast<const void*>(&data_[0]));
}

template <typename Int> inline
Scalar<Int>& Scalar<Int>::operator=( const Scalar<Int>& v )
{ 
	type_ = v.type_;
	*reinterpret_cast<dcomplex*>(reinterpret_cast<void*>(&data_[0])) = v.Get_<dcomplex>();
	return *this; 
}

template <typename Int>
template <typename T>
T& Scalar<Int>::Set()
{ 
	static_assert( ScalarType<T,Int>::Enum != UNKNOWN, 
		"Scalar value setting not supported for this type" );
	type_ = ScalarType<T,Int>::Enum;
	return *reinterpret_cast<T*>(reinterpret_cast<void*>(&data_[0]));
}

template <typename Int> inline
Int Cast_( const Scalar<Int>& s, ScalarType<Int,Int> )
{
	switch ( s.Type() ) {
	default: TypeCastError( s.Type(), INTEGRAL );
	case INTEGRAL:   return s.template Get_<Int>();
	}
}

#ifndef DISABLE_FLOAT
template <typename Int> inline
float Cast_( const Scalar<Int>& s, ScalarType<float,Int> )
{
	switch ( s.Type() ) {
	default: TypeCastError( s.Type(), SINGLE );
	case SINGLE:     return s.template Get_<float>();
	}
}
#endif

template <typename Int> inline
double Cast_( const Scalar<Int>& s, ScalarType<double,Int> )
{
	switch ( s.Type() ) {
	default: TypeCastError( s.Type(), DOUBLE );
	case INTEGRAL:   return s.template Get_<Int>();
	case SINGLE:     return s.template Get_<float>();
	case DOUBLE:     return s.template Get_<double>();
	}
}

#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT

template <typename Int> inline
scomplex Cast_( const Scalar<Int>& s, ScalarType<scomplex,Int> q )
{
	switch ( s.Type() ) {
	default: TypeCastError( s.Type(), SCOMPLEX );
	case SINGLE:     return s.template Get_<float>();
	case SCOMPLEX:   return s.template Get_<scomplex>();
	}
}

template <typename Int> inline
std::complex<float> Cast_( const Scalar<Int>& s, ScalarType<std::complex<float>,Int> )
{
	scomplex ans = Cast_( s, ScalarType<scomplex,Int>() );
	return std::complex<float>( ans.real, ans.imag );
}

#endif

template <typename Int> inline
dcomplex Cast_( const Scalar<Int>& s, ScalarType<dcomplex,Int> q )
{
	switch ( s.Type() ) {
	default: TypeCastError( s.Type(), DCOMPLEX );
	case INTEGRAL:   return s.template Get_<Int>();
	case SINGLE:     return s.template Get_<float>();
	case DOUBLE:     return s.template Get_<double>();
	case SCOMPLEX:   return s.template Get_<scomplex>();
	case DCOMPLEX:   return s.template Get_<dcomplex>();
	}
}

template <typename Int> inline
std::complex<double> Cast_( const Scalar<Int>& s, ScalarType<std::complex<double>,Int> )
{
	dcomplex ans = Cast_( s, ScalarType<dcomplex,Int>() );
	return std::complex<double>( ans.real, ans.imag );
}

#endif

template <typename Int>
template <typename T> inline
T Scalar<Int>::Cast() const
{ return elem::Cast_( *this, ScalarType<T,Int>() ); }

template <typename Int> inline
ConstBuffer<Int>::ConstBuffer() {}

template <typename Int> inline
Buffer<Int>::Buffer() {}

template <typename Int>
ConstBuffer<Int>& ConstBuffer<Int>::operator=( const ConstBuffer<Int>& v )
{ type_ = v.type_; data_ = v.data_; return *this; }

template <typename Int>
ConstBuffer<Int>& ConstBuffer<Int>::operator=( const Buffer<Int>& v )
{ type_ = v.type_; data_ = v.data_; return *this; }

template <typename Int>
Buffer<Int>& Buffer<Int>::operator=( const Buffer<Int>& v )
{ type_ = v.type_; data_ = v.data_; return *this; }

template <typename Int>
template <typename T>
ConstBuffer<Int>& ConstBuffer<Int>::operator=( const T* v )
{ 
	static_assert( type_ != UNKNOWN, "Typed buffers not supported for this type" );
	type_ = ScalarType<T>::Enum; 
	data_ = v; 
	return *this; 
}

template <typename Int>
template <typename T>
Buffer<Int>& Buffer<Int>::operator=( T* v )
{ 
	static_assert( type_ != UNKNOWN, "Typed buffers not supported for this type" );
	type_ = ScalarType<T>::Enum; 
	data_ = v; 
	return *this; 
}

template <typename Int> inline
ConstBuffer<Int>::ConstBuffer( const ConstBuffer<Int>& v )
{ operator=( v ); }

template <typename Int> inline
ConstBuffer<Int>::ConstBuffer( const Buffer<Int>& v )
{ operator=( v ); }

template <typename Int> inline
Buffer<Int>::Buffer( const Buffer<Int>& v )
{ operator=( v ); }

template <typename Int>
template <typename T>
ConstBuffer<Int>::ConstBuffer( const T* v )
{ operator=( v ); }

template <typename Int>
template <typename T>
Buffer<Int>::Buffer( T* v )
{ operator=( v ); }

template <typename Int>
ScalarTypes ConstBuffer<Int>::Type() const
{ return type_; }

template <typename Int>
ScalarTypes Buffer<Int>::Type() const
{ return type_; }

template <typename Int>
template <typename T>
const T* ConstBuffer<Int>::Get() const
{
	AssertTypeMatch( type_, ScalarType<T,Int>::Enum );
	return reinterpret_cast<const T*>(data_);
}

template <typename Int>
template <typename T>
T* Buffer<Int>::Get() const
{
	AssertTypeMatch( type_, ScalarType<T,Int>::Enum );
	return reinterpret_cast<T*>(data_);
}

template <typename Int>
ConstBuffer<Int>::operator bool() const
{ return data_ != 0; }

template <typename Int>
Buffer<Int>::operator bool() const
{ return data_ != 0; }

} // namespace elem

#endif // ifndef CORE_SCALAR_IMPL_HPP

