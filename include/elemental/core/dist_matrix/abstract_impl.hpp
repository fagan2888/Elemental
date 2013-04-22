/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_DISTMATRIX_ABSTRACT_IMPL_HPP
#define CORE_DISTMATRIX_ABSTRACT_IMPL_HPP

#ifdef RELEASE
#define PARENT(x) Parent :: x ## _
#define RPARENT(x) x ## _
#else
#define PARENT(x) Parent :: x
#define RPARENT(x) Parent :: x
#endif

namespace elem {

template <typename T,typename Int>
ScalarTypes AbstractDistMatrix<T,Int>::DataType() const
{ return ScalarType<T>::Enum; }

template<typename T,typename Int> inline
Matrix<T,Int>& AbstractDistMatrix<T,Int>::Matrix()
{ return matrix_; }

template<typename T,typename Int> inline
const Matrix<T,Int>& AbstractDistMatrix<T,Int>::LockedMatrix() const
{ return matrix_; }

template<typename T,typename Int> inline
T* AbstractDistMatrix<T,Int>::Buffer()
{ return matrix_.Buffer(); }

template<typename T,typename Int> inline
T* AbstractDistMatrix<T,Int>::Buffer( Int iLocal, Int jLocal )
{ return matrix_.Buffer( iLocal, jLocal ); }

template<typename T,typename Int> inline
const T* AbstractDistMatrix<T,Int>::LockedBuffer() const
{ return matrix_.LockedBuffer(); }

template<typename T,typename Int> inline
const T* AbstractDistMatrix<T,Int>::LockedBuffer( Int iLocal, Int jLocal ) const
{ return matrix_.LockedBuffer(iLocal,jLocal); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::Align( Int colAlignment, Int rowAlignment )
{ return PARENT(Align)( colAlignment, rowAlignment ); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::AlignCols( Int colAlignment )
{ return PARENT(AlignCols)( colAlignment ); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::AlignRows( Int rowAlignment )
{ return PARENT(AlignRows)( rowAlignment ); }

template<typename T,typename Int> inline
T AbstractDistMatrix<T,Int>::GetLocal( Int iLocal, Int jLocal ) const
{ return matrix_.Get( iLocal, jLocal ); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::SetLocal( Int iLocal, Int jLocal, T alpha )
{ matrix_.Set( iLocal, jLocal, alpha ); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::UpdateLocal( Int iLocal, Int jLocal, T alpha )
{ matrix_.Update( iLocal, jLocal, alpha ); }

template<typename T,typename Int> inline
typename Base<T>::type AbstractDistMatrix<T,Int>::GetLocalRealPart( Int iLocal, Int jLocal ) const
{ return matrix_.GetRealPart( iLocal, jLocal ); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::SetLocalRealPart( Int iLocal, Int jLocal, typename Base<T>::type alpha )
{ matrix_.SetRealPart( iLocal, jLocal, alpha ); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::UpdateLocalRealPart( Int iLocal, Int jLocal, typename Base<T>::type alpha )
{ matrix_.UpdateRealPart( iLocal, jLocal, alpha ); }

template<typename T,typename Int> inline
typename Base<T>::type AbstractDistMatrix<T,Int>::GetLocalImagPart( Int iLocal, Int jLocal ) const
{ return matrix_.GetImagPart( iLocal, jLocal ); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::SetLocalImagPart( Int iLocal, Int jLocal, typename Base<T>::type alpha )
{ matrix_.SetImagPart( iLocal, jLocal, alpha ); }

template<typename T,typename Int> inline
void AbstractDistMatrix<T,Int>::UpdateLocalImagPart( Int iLocal, Int jLocal, typename Base<T>::type alpha )
{ matrix_.UpdateImagPart( iLocal, jLocal, alpha ); }

template <typename T,typename Int>
template <Distribution U,Distribution V>
DistMatrix<T,U,V,Int>& AbstractDistMatrix<T,Int>::Cast_()
{ return *reinterpret_cast<elem::DistMatrix<T,U,V,Int>*>(this); }

template <typename T,typename Int>
template <Distribution U,Distribution V>
const DistMatrix<T,U,V,Int>& AbstractDistMatrix<T,Int>::Cast_() const
{ return *reinterpret_cast<const elem::DistMatrix<T,U,V,Int>*>(this); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::CopyFrom( const Parent& A )
{ Parent::CopyFrom( A ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::CopyFrom( const Self& A )
{ RPARENT(CopyFrom)( A ); }

template <typename T,typename Int>
AbstractDistMatrix<T,Int>& AbstractDistMatrix<T,Int>::operator=( const Parent& A )
{ Parent::CopyFrom( A ); return *this; }

template <typename T,typename Int>
AbstractDistMatrix<T,Int>& AbstractDistMatrix<T,Int>::operator=( const Self& A )
{ RPARENT(CopyFrom)( A ); return *this; }

#undef PARENT

} // namespace elem

#endif // ifndef CORE_DISTMATRIX_ABSTRACT_IMPL_HPP
