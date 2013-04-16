/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#ifndef BLAS_LIKE_LEVEL2_CPP
#define BLAS_LIKE_LEVEL2_CPP

#include "elemental-lite.hpp"
#include "elemental/blas-like/level2.hpp"
#include "elemental/dispatch.hpp"

namespace elem {

#define M AutoMatrix<int>
#define S Scalar<int>

void Gemv( Orientation orientation, const S& alpha, const M& A, const M& x, const S& beta, M& y )
{ DISPATCH_VOID( 011111, A, Gemv, orientation, alpha, A, x, beta, y )  }

void Ger( const S& alpha, const M& x, const M& y, M& A )
{ DISPATCH_VOID( 1111, x, Ger, alpha, x, y, A ) }

void Geru( const S& alpha, const M& x, const M& y, M& A )
{ DISPATCH_VOID( 1111, x, Geru, alpha, x, y, A ) }

void Hemv( UpperOrLower uplo, const S& alpha, const M& A, const M& x, const S& beta, M& y )
{ DISPATCH_VOID( 011111, A, Hemv, uplo, alpha, A, x, beta, y ) }

void Her( UpperOrLower uplo, const S& alpha, const M& x, M& A )
{ DISPATCH_VOID( 0111, x, Her, uplo, alpha, x, A ); }

void Her2( UpperOrLower uplo, const S& alpha, const M& x, const M& y, M& A )
{ DISPATCH_VOID( 01111, x, Her2, uplo, alpha, x, y, A ); }

void Symv( UpperOrLower uplo, const S& alpha, const M& A, const M& x, const S& beta, M& y, bool conjugate )
{ DISPATCH_VOID( 0111110, A, Symv, uplo, alpha, A, x, beta, y, conjugate ); }

void Syr( UpperOrLower uplo, const S& alpha, const M& x, M& A )
{ DISPATCH_VOID( 0111, x, Syr, uplo, alpha, x, A ); }

void Syr2( UpperOrLower uplo, const S& alpha, const M& x, const M& y, M& A )
{ DISPATCH_VOID( 01111, x, Syr2, uplo, alpha, x, y, A ); }

void Trmv( UpperOrLower uplo, Orientation orientation, UnitOrNonUnit diag, const M& A, M& x )
{ DISPATCH_VOID( 00011, A, Trmv, uplo, orientation, diag, A, x ) }
  
void Trsv( UpperOrLower uplo, Orientation orientation, UnitOrNonUnit diag, const M& A, M& x )
{ DISPATCH_VOID( 00011, A, Trsv, uplo, orientation, diag, A, x ) }
  
} // namespace elem

#endif // ifndef BLAS_ADJOINT_CPP
