/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_MATRIX_IMPL_HPP
#define CORE_MATRIX_IMPL_HPP

#ifdef RELEASE
#define PARENT(x) Parent :: x ## _
#define RPARENT(x) x ## _
#else
#define PARENT(x) Parent :: x
#define RPARENT(x) Parent :: x
#endif

namespace elem {

template <typename T,typename Int> inline
ScalarTypes Matrix<T,Int>::DataType() const { return ScalarType<T>::Enum; }

template <typename Int>
void AutoMatrix<Int>::AssertDataTypes( const Self& BB, bool unknown_ok ) const
{ AssertDataTypes( BB.DataType(), unknown_ok ); }

template <typename Int>
void AutoMatrix<Int>::AssertCRDataTypes( const Self& BB, bool unknown_ok ) const
{ AssertDataTypes( BB.DataType(), unknown_ok ); }

template<typename Int> inline
Int AutoMatrix<Int>::Height() const
{ return height_;  }

template<typename Int> inline
Int AutoMatrix<Int>::Width() const
{ return width_;  }

template<typename Int> inline
Int AutoMatrix<Int>::DiagonalLength( Int offset ) const
{ return elem::DiagonalLength(height_,width_,offset); }

template<typename Int> inline
Int AutoMatrix<Int>::LDim() const
{ return ldim_;  }

template<typename Int> inline
size_t AutoMatrix<Int>::DataSize() const
{ return dsize_; }

template <typename Int> inline
size_t AutoMatrix<Int>::MemorySize() const
{ return numel_ * dsize_; }

template<typename Int> inline
bool AutoMatrix<Int>::Viewing() const
{ return viewing_; }

template<typename Int> inline
bool AutoMatrix<Int>::Locked() const
{ return locked_; }

template <typename Int>
template <typename T>
Matrix<T,Int>& AutoMatrix<Int>::Cast_()
{ return reinterpret_cast<elem::Matrix<T,Int>&>(*this); }

template <typename Int>
template <typename T>
const Matrix<T,Int>& AutoMatrix<Int>::Cast_() const
{ return reinterpret_cast<const elem::Matrix<T,Int>&>(*this); }

//
// Buffer retrieval
//

template <typename Int> inline
void* AutoMatrix<Int>::Buffer_()
{ return data_; }

template <typename T,typename Int> inline
T* Matrix<T,Int>::Buffer_()
{ return static_cast<T*>(Parent::Buffer_()); }

template <typename T,typename Int> inline
T* Matrix<T,Int>::Buffer()
{ return static_cast<T*>(RPARENT(Buffer)()); }

template<typename Int> inline
const void* AutoMatrix<Int>::LockedBuffer_() const
{ return data_; }

template <typename T,typename Int> inline
const T* Matrix<T,Int>::LockedBuffer_() const
{ return static_cast<const T*>(Parent::LockedBuffer_()); }

template <typename Int> inline
const void* AutoMatrix<Int>::LockedBuffer() const
{ return LockedBuffer_(); }

template <typename T,typename Int> inline
const T* Matrix<T,Int>::LockedBuffer() const
{ return LockedBuffer_(); }

template<typename Int> inline
void* AutoMatrix<Int>::Buffer_( Int i, Int j )
{ return data_ + ( i + j * ldim_ ) * dsize_; }

template<typename T,typename Int> inline
T* Matrix<T,Int>::Buffer_( Int i, Int j )
{ return static_cast<T*>(Parent::Buffer_()) + ( i + j * Parent::LDim() ); }

template <typename T,typename Int> inline
T* Matrix<T,Int>::Buffer( Int i, Int j ) 
{ return static_cast<T*>(RPARENT(Buffer)(i,j)); }

template <typename Int> inline
const void* AutoMatrix<Int>::LockedBuffer_( Int i, Int j ) const
{ return data_ + ( i + j * ldim_ ) * dsize_; }

template<typename T,typename Int> inline
const T* Matrix<T,Int>::LockedBuffer_( Int i, Int j ) const
{ return static_cast<const T*>(Parent::LockedBuffer_()) + ( i + j * Parent::LDim() ); }

template <typename T,typename Int> inline
const T* Matrix<T,Int>::LockedBuffer( Int i, Int j ) const
{ return static_cast<const T*>(RPARENT(LockedBuffer)(i,j)); }

template <typename Int> inline
void AutoMatrix<Int>::Attach_( ScalarTypes dtype, Int height, Int width, const void* buffer, Int ldim, bool lock )
{ Attach_( height, width, buffer, ldim, false ); }

template <typename Int> inline
void AutoMatrix<Int>::Print( std::string msg ) const
{ return Print( std::cout, msg ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Print( std::string msg ) const
{ return Print( std::cout, msg ); }

//
// Individual element manipulation
//

template <typename T,typename Int> inline
T Matrix<T,Int>::Get_( Int i, Int j ) const
{ return *LockedBuffer_( i, j ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Set_( Int i, Int j, T a )
{ *Buffer_( i, j ) = a; }

template <typename T,typename Int> inline
void Matrix<T,Int>::Update_( Int i, Int j, T a )
{ *Buffer_( i, j ) += a; }

template <typename T,typename Int> inline
typename Base<T>::type 
Matrix<T,Int>::GetRealPart_( Int i, Int j ) const
{ return elem::GReal_( *LockedBuffer_( i, j ) ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetRealPart_( Int i, Int j, typename Base<T>::type a )
{ elem::SReal_( *Buffer_( i, j ) ) = a; }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateRealPart_( Int i, Int j, typename Base<T>::type a )
{ elem::SReal_( *Buffer_( i, j ) ) += a; }

template <typename T,typename Int> inline
typename Base<T>::type 
Matrix<T,Int>::GetImagPart_( Int i, Int j ) const
{ return elem::GImag_( *LockedBuffer_( i, j ) ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetImagPart_( Int i, Int j, typename Base<T>::type a )
{ elem::SImag_( *Buffer_( i, j ) ) = a; }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateImagPart_( Int i, Int j, typename Base<T>::type a )
{ elem::SImag_( *Buffer_( i, j ) ) += a; }

template <typename T,typename Int> inline
T Matrix<T,Int>::Get( Int i, Int j ) const
{ return RPARENT(Get)(i,j); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Set( Int i, Int j, T a )
{ RPARENT(Set)(i,j,a); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Update( Int i, Int j, T a )
{ RPARENT(Update)(i,j,a); }

template <typename T,typename Int> inline
typename Base<T>::type 
Matrix<T,Int>::GetRealPart( Int i, Int j ) const
{ return RPARENT(GetRealPart)(i,j); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetRealPart( Int i, Int j, typename Base<T>::type a )
{ RPARENT(SetRealPart)(i,j,a); }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateRealPart( Int i, Int j, typename Base<T>::type a )
{ RPARENT(UpdateRealPart)(i,j,a); }

// Only valid for complex data

template <typename T,typename Int> inline
typename Base<T>::type Matrix<T,Int>::GetImagPart( Int i, Int j ) const
{ return RPARENT(GetImagPart)(i,j); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetImagPart( Int i, Int j, typename Base<T>::type a )
{ RPARENT(SetImagPart)(i,j,a); }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateImagPart( Int i, Int j, typename Base<T>::type a )
{ RPARENT(UpdateImagPart)(i,j,a); }

//
// Diagonal manipulation
//

template <typename T,typename Int> inline
void Matrix<T,Int>::GetDiagonal( Self& d, Int offset ) const
{ RPARENT(GetDiagonal)( d, offset ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetDiagonal( Self& d, Int offset )
{ RPARENT(SetDiagonal)( d, offset ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateDiagonal( Self& d, Int offset )
{ RPARENT(UpdateDiagonal)( d, offset ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::GetRealPartOfDiagonal( RSelf& d, Int offset ) const
{ RPARENT(GetRealPartOfDiagonal)( d, offset ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetRealPartOfDiagonal( RSelf& d, Int offset )
{ RPARENT(SetRealPartOfDiagonal)( d, offset ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateRealPartOfDiagonal( RSelf& d, Int offset )
{ RPARENT(UpdateRealPartOfDiagonal)( d, offset ); }

// Only valid for complex data
template <typename T,typename Int> inline
void Matrix<T,Int>::GetImagPartOfDiagonal( RSelf& d, Int offset ) const
{ RPARENT(GetImagPartOfDiagonal)( d, offset ); }

// Only valid for complex data
template <typename T,typename Int> inline
void Matrix<T,Int>::SetImagPartOfDiagonal( RSelf& d, Int offset )
{ RPARENT(SetImagPartOfDiagonal)( d, offset ); }

// Only valid for complex data
template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateImagPartOfDiagonal( RSelf& d, Int offset )
{ RPARENT(UpdateImagPartOfDiagonal)( d, offset ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Attach( Int height, Int width, T* buffer, Int ldim )
{ PARENT(Attach)( DataType(), height, width, buffer, ldim, false ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::LockedAttach( Int height, Int width, const T* buffer, Int ldim )
{ PARENT(Attach)( DataType(), height, width, buffer, ldim, true ); }

template <typename Int> inline
const AutoMatrix<Int>& AutoMatrix<Int>::operator=( const Self& A ) 
{ CopyFrom( A ); return *this; }

template <typename T,typename Int> inline
void Matrix<T,Int>::CopyFrom( const Self& A )
{ PARENT(CopyFrom)( A ); }

template <typename T,typename Int> inline
const Matrix<T,Int>& Matrix<T,Int>::operator=( const Self& A ) 
{ CopyFrom( A ); return *this; }

template <typename T,typename Int> inline
void Matrix<T,Int>::ResizeTo( Int height, Int width )
{ PARENT(ResizeTo)( height, width ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::ResizeTo( Int height, Int width, Int ldim )
{ PARENT(ResizeTo)( height, width, ldim ); }
    
template <typename Int>
void AutoMatrix<Int>::Attach__( const Self& B, Int i, Int j, Int height, Int width, bool lock )
{ Attach_( height, width, B.data_ + ( i + j * B.ldim_ ) * B.dsize_, B.ldim_, lock ); }

#undef PARENT
#undef RPARENT
#undef GPARENT
#undef SPARENT

} // namespace elem

#endif // ifndef CORE_MATRIX_HPP
