/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_MASSERT_IMPL_HPP
#define CORE_MASSERT_IMPL_HPP

namespace elem {

template <typename Int> inline
void AssertUnlocked( const AutoMatrix<Int>& A )
{ AssertUnlocked( A.Locked() ); }

template <typename Int> inline
void AssertUnlocked( const AutoDistMatrix<Int>& A )
{ AssertUnlocked( A.Locked() ); }

template <typename Int> inline
void AssertDataTypes( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B )
{ AssertDataTypes( A.DataType(), B.DataType() ); }

template <typename Int> inline
void AssertDataTypes( const AutoDistMatrix<Int>& A, const AutoDistMatrix<Int>& B )
{ AssertDataTypes( A.DataType(), B.DataType() ); }

template <typename Int> inline
void AssertCRDataTypes( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B )
{ AssertCRDataTypes( A.DataType(), B.DataType() ); }

template <typename Int> inline
void AssertCRDataTypes( const AutoDistMatrix<Int>& A, const AutoDistMatrix<Int>& B )
{ AssertCRDataTypes( A.DataType(), B.DataType() ); }

template <typename Int> inline
void AssertSameSize( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B )
{ AssertSameSize( A.Height(), A.Width(), B.Height(), B.Width() ); }

template <typename Int> inline
void AssertSameSize( const AutoDistMatrix<Int>& A, const AutoDistMatrix<Int>& B )
{ AssertSameSize( A.Height(), A.Width(), B.Height(), B.Width() ); }

} // namespace elem

#endif // ifndef CORE_MATRIX_HPP
