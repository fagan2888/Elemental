/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#ifndef BLAS_LIKE_LEVEL3_CPP
#define BLAS_LIKE_LEVEL3_CPP

#include "elemental-lite.hpp"
#include "elemental/blas-like/level3.hpp"
#include "elemental/dispatch.hpp"

namespace elem {

#define M AutoMatrix<int>
#define S Scalar<int>

void Gemm
( Orientation orientationOfA, Orientation orientationOfB,
  const S& alpha, const M& A, const M& B, 
  const S& beta, M& Z )
{ DISPATCH_VOID( 0011111, A, Gemm, orientationOfA, orientationOfB, alpha, A, B, beta, Z ) }

void Hemm
( LeftOrRight side, UpperOrLower uplo,
  const S& alpha, const M& A, const M& B, 
  const S& beta, M& Z )
{ DISPATCH_VOID( 0011111, A, Hemm, side, uplo, alpha, A, B, beta, Z ) }

void Her2k
( UpperOrLower uplo, Orientation orientation,
  const S& alpha, const M& A, const M& B, 
  const S& beta, M& Z )
{ DISPATCH_VOID( 0011111, A, Her2k, uplo, orientation, alpha, A, B, beta, Z ) }

void Herk
( UpperOrLower uplo, Orientation orientation,
  const S& alpha, const M& A,
  const S& beta, M& Z )
{ DISPATCH_VOID( 001111, A, Herk, uplo, orientation, alpha, A, beta, Z ) }

void Symm
( LeftOrRight side, UpperOrLower uplo,
  const S& alpha, const M& A, const M& B, 
  const S& beta, M& Z, bool conjugate )
{ DISPATCH_VOID( 00111110, A, Symm, side, uplo, alpha, A, B, beta, Z, conjugate ) }

void Syr2k
( UpperOrLower uplo, Orientation orientation,
  const S& alpha, const M& A, const M& B, 
  const S& beta, M& Z, bool conjugate )
{ DISPATCH_VOID( 00111110, A, Syr2k, uplo, orientation, alpha, A, B, beta, Z, conjugate ) }

void Syrk
( UpperOrLower uplo, Orientation orientation,
  const S& alpha, const M& A,
  const S& beta, M& Z, bool conjugate )
{ DISPATCH_VOID( 0011110, A, Syrk, uplo, orientation, alpha, A, beta, Z, conjugate ) }

void Trdtrmm
( Orientation orientation, UpperOrLower uplo, M& A )
{ DISPATCH_VOID( 001, A, Trdtrmm, orientation, uplo, A ) }

void Trmm
( LeftOrRight side, UpperOrLower uplo,
  Orientation orientation, UnitOrNonUnit diag,
  const S& alpha, const M& A, M& B )
{ DISPATCH_VOID( 0000111, A, Trmm, side, uplo, orientation, diag, alpha, A, B ) }

void Trsm
( LeftOrRight side, UpperOrLower uplo,
  Orientation orientation, UnitOrNonUnit diag,
  const S& alpha, const M& A, M& B,
  bool checkIfSingular )
{ DISPATCH_VOID( 00001110, A, Trsm, side, uplo, orientation, diag, alpha, A, B, checkIfSingular ) }

void Trtrmm
( Orientation orientation, UpperOrLower uplo, M& A )
{ DISPATCH_VOID( 001, A, Trtrmm, orientation, uplo, A ) }

void Trtrsm
( LeftOrRight side, UpperOrLower uplo,
  Orientation orientation, UnitOrNonUnit diag,
  const S& alpha, const M& A, M& X,
  bool checkIfSingular )
{ DISPATCH_VOID( 00001110, A, Trtrsm, side, uplo, orientation, diag, alpha, A, X, checkIfSingular ) }

void TwoSidedTrmm
( UpperOrLower uplo, UnitOrNonUnit diag, M& A, const M& B )
{ DISPATCH_VOID( 0011, A, TwoSidedTrmm, uplo, diag, A, B ) }

void TwoSidedTrsm
( UpperOrLower uplo, UnitOrNonUnit diag, M& A, const M& B )
{ DISPATCH_VOID( 0011, A, TwoSidedTrsm, uplo, diag, A, B ) }

} // namespace elem

#endif // ifndef BLAS_ADJOINT_CPP
