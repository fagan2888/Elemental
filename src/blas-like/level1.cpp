/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#ifndef BLAS_LIKE_LEVEL1_CPP
#define BLAS_LIKE_LEVEL1_CPP

#include "elemental-lite.hpp"
#include "elemental/blas-like/level1.hpp"
#include "elemental/dispatch.hpp"

namespace elem {

#define M AutoMatrix<int>
#define S Scalar<int>

void Adjoint( const M& A1, M& A2 )
{
	PushCallStack("Adjoint");
	Transpose( A1, A2, true );
	PopCallStack();
}

void Axpy( const S& alpha, const M& A, M& B )
{ DISPATCH_VOID( 111, A, Axpy, alpha, A, B ) }

void AxpyTriangle( UpperOrLower uplo, const S& alpha, const M& A, M& B )
{ DISPATCH_VOID( 0111, A, AxpyTriangle, uplo, alpha, A, B ) }

void Conjugate( M& A )
{ DISPATCH_VOID( 1, A, Conjugate, A ) }

void Conjugate( const M& A, M& B )
{ DISPATCH_VOID( 11, A, Conjugate, A, B ) }

void Copy( const M& A, M& B )
{
	PushCallStack("Matrix::Copy");
	B.CopyFrom( A );
	PopCallStack();
}

void DiagonalScale( LeftOrRight side, Orientation orientation, M& d, M& X )
{
	PushCallStack("Matrix::DiagonalScale");
	switch ( X.DataType() ) {
	case SINGLE:    CAST_0011(DiagonalScale,float,side,orientation,d,X);
	case DOUBLE:    CAST_0011(DiagonalScale,double,side,orientation,d,X);
	case SCOMPLEX:
		if ( d.IsComplex() )
			DiagonalScale( side, orientation, d.Cast<scomplex>(), X.Cast<scomplex>() );
		else
			DiagonalScale( side, orientation, d.Cast<float>(), X.Cast<scomplex>() );
		break;
	case DCOMPLEX:
		if ( d.IsComplex() )
			DiagonalScale( side, orientation, d.Cast<dcomplex>(), X.Cast<dcomplex>() );
		else
			DiagonalScale( side, orientation, d.Cast<double>(), X.Cast<dcomplex>() );
		break;
    default: throw std::logic_error( "Not implemented for this data type" ); break;
	}
	PopCallStack();
}

void DiagonalSolve( LeftOrRight side, Orientation orientation, M& d, M& X )
{
	PushCallStack("Matrix::DiagonalSolve");
	switch ( X.DataType() ) {
	case SINGLE: CAST_0011(DiagonalSolve,float,side,orientation,d,X);
	case DOUBLE: CAST_0011(DiagonalScale,double,side,orientation,d,X);
	case SCOMPLEX:
		if ( d.IsComplex() )
			DiagonalSolve( side, orientation, d.Cast<scomplex>(), X.Cast<scomplex>() );
		else
			DiagonalSolve( side, orientation, d.Cast<float>(), X.Cast<scomplex>() );
		break;
	case DCOMPLEX:
		if ( d.IsComplex() )
			DiagonalSolve( side, orientation, d.Cast<dcomplex>(), X.Cast<dcomplex>() );
		else
			DiagonalSolve( side, orientation, d.Cast<double>(), X.Cast<dcomplex>() );
		break;
    default: throw std::logic_error( "Not implemented for this data type" ); break;
	}
	PopCallStack();
}

S Dot( const M& A, const M& B )
{ DISPATCH_SCALAR( 11, A, Dot, A, B ) }

S Dotu( const M& A, const M& B )
{ DISPATCH_SCALAR( 11, A, Dotu, A, B ) }

void MakeHermitian( UpperOrLower uplo, M& A )
{ DISPATCH_VOID( 01, A, MakeHermitian, uplo, A ) }

void MakeReal( M& A )
{ DISPATCH_VOID( 1, A, MakeReal, A ) }

void MakeSymmetric( UpperOrLower uplo, M& A )
{ DISPATCH_VOID( 01, A, MakeSymmetric, uplo, A ) }

void MakeTrapezoidal( LeftOrRight side, UpperOrLower uplo, int offset, M& A )
{ DISPATCH_VOID( 0001, A, MakeTrapezoidal, side, uplo, offset, A ) }

void MakeTriangular( UpperOrLower uplo, M& A )
{ DISPATCH_VOID( 01, A, MakeTriangular, uplo, A ) }

S Nrm2( const M& A )
{ DISPATCH_SCALAR( 1, A, Nrm2, A ) }

void Scale( const S& alpha, M& A )
{ DISPATCH_VOID( 11, A, Scale, alpha, A ) } 

void ScaleTrapezoid( const S& alpha, LeftOrRight side, UpperOrLower uplo, int offset, M& A )
{ DISPATCH_VOID( 10001, A, ScaleTrapezoid, alpha, side, uplo, offset, A ) }

void SetDiagonal( M& A, const S& v )
{ DISPATCH_VOID( 11, A, SetDiagonal, A, v ) }

void Transpose( const M& A, M& B, bool conjugate )
{ DISPATCH_VOID( 110, A, Transpose, A, B, conjugate ) } 

void Zero( M& A )
{ DISPATCH_VOID( 1, A, Zero, A ) }

} // namespace elem

#endif // ifndef BLAS_ADJOINT_CPP
