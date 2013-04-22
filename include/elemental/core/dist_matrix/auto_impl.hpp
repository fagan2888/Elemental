/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_DISTMATRIX_AUTO_IMPL_HPP
#define CORE_DISTMATRIX_AUTO_IMPL_HPP

namespace elem {

template <typename Int> inline
Int AutoDistMatrix<Int>::Height() const 
{ return height_; }

template <typename Int> inline
Int AutoDistMatrix<Int>::Width() const 
{ return width_; }

template <typename Int> inline
Int AutoDistMatrix<Int>::LocalHeight() const
{ return LockedMatrix().Height(); }

template <typename Int> inline
Int AutoDistMatrix<Int>::LocalWidth() const
{ return LockedMatrix().Width(); }

template <typename Int> inline
Int AutoDistMatrix<Int>::LDim() const
{ return LockedMatrix().LDim(); }

template <typename Int> inline
bool AutoDistMatrix<Int>::IsComplex() const
{ return LockedMatrix().IsComplex(); }

template <typename Int> inline
size_t AutoDistMatrix<Int>::DataSize() const
{ return LockedMatrix().DataSize(); }

template <typename Int> inline
size_t AutoDistMatrix<Int>::AllocatedMemory() const
{ return LockedMatrix().MemorySize(); }

template <typename Int> inline
ScalarTypes AutoDistMatrix<Int>::DataType() const
{ return LockedMatrix().DataType(); }

template<typename Int> inline
const elem::Grid& AutoDistMatrix<Int>::Grid() const
{ return *grid_; }

template<typename Int> inline
void* AutoDistMatrix<Int>::Buffer()
{ return Matrix().Buffer(); }

template<typename Int> inline
void* AutoDistMatrix<Int>::Buffer( Int i, Int j )
{ return Matrix().Buffer( i, j ); }

template<typename Int> inline
const void* AutoDistMatrix<Int>::LockedBuffer() const
{ return LockedMatrix().LockedBuffer(); }

template<typename Int> inline
const void* AutoDistMatrix<Int>::LockedBuffer( Int i, Int j ) const
{ return LockedMatrix().LockedBuffer( i, j ); }

template<typename Int> inline
void AutoDistMatrix<Int>::FreeAlignments() 
{ constrainedColAlignment_ = constrainedRowAlignment_ = false; }

template<typename Int> inline
bool AutoDistMatrix<Int>::ConstrainedColAlignment() const
{ return constrainedColAlignment_; }

template<typename Int> inline
bool AutoDistMatrix<Int>::ConstrainedRowAlignment() const
{ return constrainedRowAlignment_; }

template <typename Int> inline
Int AutoDistMatrix<Int>::ColAlignment() const
{ return colAlignment_; }
    
template <typename Int> inline
Int AutoDistMatrix<Int>::RowAlignment() const
{ return rowAlignment_; }
    
template<typename Int> inline
Int AutoDistMatrix<Int>::ColShift() const
{ return colShift_; }

template<typename Int> inline
Int AutoDistMatrix<Int>::RowShift() const
{ return rowShift_;  }

template<typename Int> inline
bool AutoDistMatrix<Int>::Viewing() const
{ return viewing_; }

template<typename Int> inline
bool AutoDistMatrix<Int>::Locked() const
{ return locked_; }

template <typename Int>
template <typename T>
AbstractDistMatrix<T,Int>& AutoDistMatrix<Int>::Cast_()
{ return *reinterpret_cast<elem::AbstractDistMatrix<T,Int>*>(this); }

template <typename Int>
template <typename T>
const AbstractDistMatrix<T,Int>& AutoDistMatrix<Int>::Cast_() const
{ return *reinterpret_cast<const elem::AbstractDistMatrix<T,Int>*>(this); }

template <typename Int>
template <typename T,Distribution U,Distribution V>
DistMatrix<T,U,V,Int>& AutoDistMatrix<Int>::Cast_()
{ return *reinterpret_cast<elem::DistMatrix<T,U,V,Int>*>(this); }

template <typename Int>
template <typename T,Distribution U,Distribution V>
const DistMatrix<T,U,V,Int>& AutoDistMatrix<Int>::Cast_() const
{ return *reinterpret_cast<const elem::DistMatrix<T,U,V,Int>*>(this); }

template<typename Int> inline
AutoDistMatrix<Int>& AutoDistMatrix<Int>::operator=( const Self& A )
{ CopyFrom( A ); return *this; }

} // namespace elem

#endif // ifndef CORE_DISTMATRIX_AUTO_HPP
