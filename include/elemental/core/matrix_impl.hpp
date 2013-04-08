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
#define GPARENT(x,i,j) return x ## _(i,j)
#define GRPARENT(x,i,j) return x ## _(i,j)
#define SPARENT(x,i,j,a) x ## _(i,j,a)
#else
#define PARENT(x) Parent :: x
#define RPARENT(x) Parent :: x
#define GPARENT(x,i,j) T a; Parent :: x ( DataType(), i, j, &a ); return a
#define GRPARENT(x,i,j) typename Base<T>::type a; Parent :: x ( DataType(), i, j, &a ); return a
#define SPARENT(x,i,j,a) Parent :: x( DataType(), i, j, &a )
#endif

namespace elem {

template <> inline
MatrixTypes Matrix<int,int>::DataType() const { return Integral; }
#ifndef DISABLE_FLOAT
template <> MatrixTypes inline
Matrix<float,int>::DataType() const { return Float; }
#endif // ifndef DISABLE_FLOAT
template <> MatrixTypes inline
Matrix<double,int>::DataType() const { return Double; }
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
template <> MatrixTypes inline
Matrix<Complex<float>,int>::DataType() const { return FComplex; }
#endif // ifndef DISABLE_FLOAT
template <> MatrixTypes inline
Matrix<Complex<double>,int>::DataType() const { return DComplex; }
#endif // ifndef DISABLE_COMPLEX

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
MatrixTypes AutoMatrix<Int>::DataType() const
{ return Unknown; }

template<typename Int> inline
Int AutoMatrix<Int>::DataSize() const
{ return dsize_; }

template <typename T,typename Int> inline
size_t Matrix<T,Int>::MemorySize() const
{ return memory_.Size(); }

template<typename Int> inline
bool AutoMatrix<Int>::Viewing() const
{ return viewing_; }

template<typename Int> inline
bool AutoMatrix<Int>::Locked() const
{ return locked_; }

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
{ return static_cast<char*>(data_) + ( i + j * ldim_ ) * dsize_; }

template<typename T,typename Int> inline
T* Matrix<T,Int>::Buffer_( Int i, Int j )
{ return static_cast<T*>(Parent::Buffer_()) + ( i + j * Parent::LDim() ); }

template <typename T,typename Int> inline
T* Matrix<T,Int>::Buffer( Int i, Int j ) 
{ return static_cast<T*>(RPARENT(Buffer)(i,j)); }

template <typename Int> inline
const void* AutoMatrix<Int>::LockedBuffer_( Int i, Int j ) const
{ return static_cast<const char*>(data_) + ( i + j * ldim_ ) * dsize_; }

template<typename T,typename Int> inline
const T* Matrix<T,Int>::LockedBuffer_( Int i, Int j ) const
{ return static_cast<const T*>(Parent::LockedBuffer_()) + ( i + j * Parent::LDim() ); }

template <typename T,typename Int> inline
const T* Matrix<T,Int>::LockedBuffer( Int i, Int j ) const
{ return static_cast<const T*>(RPARENT(LockedBuffer)(i,j)); }

template <typename Int> inline
void AutoMatrix<Int>::Attach_( MatrixTypes dtype, Int height, Int width, const void* buffer, Int ldim, bool lock )
{ Attach_( height, width, buffer, ldim, false, 0, 0 ); }

template <typename Int> inline
void AutoMatrix<Int>::Print( std::string msg )
{ return Print( std::cout, msg ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Print( std::string msg )
{ return Print( std::cout, msg ); }

//
// Individual element manipulation
//

template <typename T,typename Int> inline
T Matrix<T,Int>::Get_( Int i, Int j ) const
{ return *LockedBuffer( i, j ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Set_( Int i, Int j, T a )
{ *Buffer( i, j ) = a; }

template <typename T,typename Int> inline
void Matrix<T,Int>::Update_( Int i, Int j, T a )
{ *Buffer( i, j ) += a; }

template <typename T,typename Int> inline
typename Base<T>::type 
Matrix<T,Int>::GetRealPart_( Int i, Int j ) const
{ return elem::GReal_( *LockedBuffer( i, j ) ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetRealPart_( Int i, Int j, typename Base<T>::type a )
{ elem::SReal_( *Buffer( i, j ) ) = a; }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateRealPart_( Int i, Int j, typename Base<T>::type a )
{ elem::SReal_( *Buffer( i, j ) ) += a; }

template <typename T,typename Int> inline
typename Base<T>::type 
Matrix<T,Int>::GetImagPart_( Int i, Int j ) const
{ return elem::GImag_( *LockedBuffer( i, j ) ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetImagPart_( Int i, Int j, typename Base<T>::type a )
{ elem::SImag_( *Buffer( i, j ) ) = a; }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateImagPart_( Int i, Int j, typename Base<T>::type a )
{ elem::SImag_( *Buffer( i, j ) ) += a; }

template <typename T,typename Int> inline
T Matrix<T,Int>::Get( Int i, Int j ) const
{ GPARENT(Get,i,j); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Set( Int i, Int j, T a )
{ SPARENT(Set,i,j,a); }

template <typename T,typename Int> inline
void Matrix<T,Int>::Update( Int i, Int j, T a )
{ SPARENT(Update,i,j,a); }

template <typename T,typename Int> inline
typename Base<T>::type 
Matrix<T,Int>::GetRealPart( Int i, Int j ) const
{ GRPARENT(GetRealPart,i,j); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetRealPart( Int i, Int j, typename Base<T>::type a )
{ SPARENT(SetRealPart,i,j,a); }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateRealPart( Int i, Int j, typename Base<T>::type a )
{ SPARENT(UpdateRealPart,i,j,a); }

// Only valid for complex data

template <typename T,typename Int> inline
typename Base<T>::type Matrix<T,Int>::GetImagPart( Int i, Int j ) const
{ GRPARENT(GetImagPart,i,j); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetImagPart( Int i, Int j, typename Base<T>::type a )
{ SPARENT(SetImagPart,i,j,a); }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateImagPart( Int i, Int j, typename Base<T>::type a )
{ SPARENT(UpdateImagPart,i,j,a); }

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
void Matrix<T,Int>::GetRealPartOfDiagonal( Self& d, Int offset ) const
{ RPARENT(GetRealPartOfDiagonal)( d, offset ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::SetRealPartOfDiagonal( Self& d, Int offset )
{ RPARENT(SetRealPartOfDiagonal)( d, offset ); }

template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateRealPartOfDiagonal( Self& d, Int offset )
{ RPARENT(UpdateRealPartOfDiagonal)( d, offset ); }

// Only valid for complex data
template <typename T,typename Int> inline
void Matrix<T,Int>::GetImagPartOfDiagonal( Self& d, Int offset ) const
{ RPARENT(GetImagPartOfDiagonal)( d, offset ); }

// Only valid for complex data
template <typename T,typename Int> inline
void Matrix<T,Int>::SetImagPartOfDiagonal( Self& d, Int offset )
{ RPARENT(SetImagPartOfDiagonal)( d, offset ); }

// Only valid for complex data
template <typename T,typename Int> inline
void Matrix<T,Int>::UpdateImagPartOfDiagonal( Self& d, Int offset )
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
const Matrix<T,Int>& Matrix<T,Int>::operator=( const Self& A ) 
{ PARENT(CopyFrom)( A ); return *this; }


#undef PARENT
#undef RPARENT
#undef GPARENT
#undef SPARENT

} // namespace elem

#endif // ifndef CORE_MATRIX_HPP
