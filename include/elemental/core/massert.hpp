/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_MASSERT_HPP
#define CORE_MASSERT_HPP

namespace elem {

void AssertUnlocked( bool );

template <typename Int>
void AssertUnlocked( const AutoMatrix<Int>& A );
	
template <typename Int>
void AssertUnlocked( const AutoDistMatrix<Int>& A );
	
template <typename Int> 
void AssertNonnegative( Int i, const char* s );
	
template <typename Int>
void AssertIndices( Int i, Int j );
	
template <typename Int>
void AssertValidEntry( Int i, Int j, Int height, Int width );
	
template <typename Int>
void AssertLDim( Int ldim );
	
template <typename Int>
void AssertDimensions( Int height, Int width, Int ldim, bool zero_ok = false );
	
template <typename Int>
void AssertValidSubmatrix( Int i, Int j, Int height, Int width, Int oheight, Int owidth );

template <typename Int>
void AssertSameSize( Int h1, Int w1, Int h2, Int w2 );

template <typename Int>
void AssertSameSize( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B );

template <typename Int>
void AssertSameSize( const AutoDistMatrix<Int>& A, const AutoDistMatrix<Int>& B );

void AssertDataTypes( ScalarTypes A, ScalarTypes B );
void AssertDistributions( Distribution AR, Distribution AC, Distribution BR, Distribution BC );

template<typename Int>
void AssertDataTypes
	( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B );
	
template<typename Int>
void AssertDataTypes
	( const AutoDistMatrix<Int>& A, const AutoDistMatrix<Int>& B );
	
void AssertCRDataTypes( ScalarTypes A, ScalarTypes B, bool imag_bad = false );

template<typename Int>
void AssertCRDataTypes
	( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B );
	
template<typename Int>
void AssertCRDataTypes
	( const AutoDistMatrix<Int>& A, const AutoDistMatrix<Int>& B );
	
template<typename Int>
void AssertDataTypes
	( Int N, const AutoMatrix<Int>* A, ... );
	
template<typename Int>
void AssertDataTypes
	( Int N, const AutoDistMatrix<Int>* A, ... );
	
template <typename Int> 
void AssertContiguous2x1( 
	const AutoMatrix<Int>& A0, 
	const AutoMatrix<Int>& A1 );
	
template <typename Int>
void AssertContiguous1x2( 
	const AutoMatrix<Int>& A0, const AutoMatrix<Int>& A1 );
	
template <typename Int> 
void AssertContiguous3x1( 
	const AutoMatrix<Int>& A0, 
	const AutoMatrix<Int>& A1,
	const AutoMatrix<Int>& A2 );
	
template <typename Int> 
void AssertContiguous1x3( 
	const AutoMatrix<Int>& A0, const AutoMatrix<Int>& A1, const AutoMatrix<Int>& A2 );
	
template <typename Int> 
void AssertContiguous2x2( 
	const AutoMatrix<Int>& A00, const AutoMatrix<Int>& A01,
	const AutoMatrix<Int>& A10, const AutoMatrix<Int>& A11 );
	
template <typename Int> 
void AssertContiguous3x3( 
	const AutoMatrix<Int>& A00, const AutoMatrix<Int>& A01, const AutoMatrix<Int>& A02,
	const AutoMatrix<Int>& A10, const AutoMatrix<Int>& A11, const AutoMatrix<Int>& A12,
	const AutoMatrix<Int>& A20, const AutoMatrix<Int>& A21, const AutoMatrix<Int>& A22 );

template <typename Int> 
void AssertContiguous2x1( 
	const AutoDistMatrix<Int>& A0, 
	const AutoDistMatrix<Int>& A1 );
	
template <typename Int> 
void AssertContiguous1x2( 
	const AutoDistMatrix<Int>& A0, const AutoDistMatrix<Int>& A1 );
	
template <typename Int> 
void AssertContiguous3x1( 
	const AutoDistMatrix<Int>& A0, 
	const AutoDistMatrix<Int>& A1,
	const AutoDistMatrix<Int>& A2 );
	
template <typename Int> 
void AssertContiguous1x3( 
	const AutoDistMatrix<Int>& A0, const AutoDistMatrix<Int>& A1, const AutoDistMatrix<Int>& A2 );
	
template <typename Int> 
void AssertContiguous2x2( 
	const AutoDistMatrix<Int>& A00, const AutoDistMatrix<Int>& A01,
	const AutoDistMatrix<Int>& A10, const AutoDistMatrix<Int>& A11 );
	
template <typename Int> 
void AssertContiguous3x3( 
	const AutoDistMatrix<Int>& A00, const AutoDistMatrix<Int>& A01, const AutoDistMatrix<Int>& A02,
	const AutoDistMatrix<Int>& A10, const AutoDistMatrix<Int>& A11, const AutoDistMatrix<Int>& A12,
	const AutoDistMatrix<Int>& A20, const AutoDistMatrix<Int>& A21, const AutoDistMatrix<Int>& A22 );
	
enum NotImplementedTypes {
	NIDone,
	NIBool,
	NIInt,
	NIScalar,
	NIMatrix,
	NIDistData,
	NIDistMatrix
};

void NotImplemented(
	bool is_method,
	const char* func_name,
	...
);
	
} // namespace elem

#endif // CORE_MASSERT_HPP