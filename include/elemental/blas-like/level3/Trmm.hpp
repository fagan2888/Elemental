/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef BLAS_TRMM_HPP
#define BLAS_TRMM_HPP

namespace elem {

template<typename T,Distribution BColDist,Distribution BRowDist>
inline void
LocalTrmm
( LeftOrRight side, UpperOrLower uplo,
  Orientation orientation, UnitOrNonUnit diag,
  T alpha, const DistMatrix<T,STAR,STAR>& A,
                 DistMatrix<T,BColDist,BRowDist>& B )
{
#ifndef RELEASE
    PushCallStack("LocalTrmm");
    if( (side == LEFT && BColDist != STAR) ||
        (side == RIGHT && BRowDist != STAR) )
        throw std::logic_error
        ("Distribution of RHS must conform with that of triangle");
#endif
    Trmm
    ( side, uplo, orientation, diag, alpha, A.LockedMatrix(), B.Matrix() );
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace elem

#include "./Trmm/LLN.hpp"
#include "./Trmm/LLT.hpp"
#include "./Trmm/LUN.hpp"
#include "./Trmm/LUT.hpp"
#include "./Trmm/RLN.hpp"
#include "./Trmm/RLT.hpp"
#include "./Trmm/RUN.hpp"
#include "./Trmm/RUT.hpp"

namespace elem {

template<typename Int>
void Trmm
( LeftOrRight side, UpperOrLower uplo,
  Orientation orientation, UnitOrNonUnit diag,
  const Scalar<Int>& alpha, const AutoMatrix<Int>& A, AutoMatrix<Int>& B );
 
template<typename T>
inline void
Trmm
( LeftOrRight side, UpperOrLower uplo,
  Orientation orientation, UnitOrNonUnit diag,
  T alpha, const Matrix<T>& A, Matrix<T>& B )
{
#ifndef RELEASE
    PushCallStack("Trmm");
    if( A.Height() != A.Width() )
        throw std::logic_error("Triangular matrix must be square");
    if( side == LEFT )
    {
        if( A.Height() != B.Height() )
            throw std::logic_error("Nonconformal Trmm");
    }
    else
    {
        if( A.Height() != B.Width() )
            throw std::logic_error("Nonconformal Trmm");
    }
#endif
    const char sideChar = LeftOrRightToChar( side );
    const char uploChar = UpperOrLowerToChar( uplo );
    const char transChar = OrientationToChar( orientation );
    const char diagChar = UnitOrNonUnitToChar( diag );
    blas::Trmm
    ( sideChar, uploChar, transChar, diagChar, B.Height(), B.Width(),
      alpha, A.LockedBuffer(), A.LDim(), B.Buffer(), B.LDim() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
inline void
Trmm
( LeftOrRight side, UpperOrLower uplo, 
  Orientation orientation, UnitOrNonUnit diag,
  T alpha, const DistMatrix<T>& A, DistMatrix<T>& X )
{
#ifndef RELEASE
    PushCallStack("Trmm");
#endif
    if( side == LEFT && uplo == LOWER )
    {
        if( orientation == NORMAL )
            internal::TrmmLLN( diag, alpha, A, X );
        else
            internal::TrmmLLT( orientation, diag, alpha, A, X );
    }
    else if( side == LEFT )
    {
        if( orientation == NORMAL )
            internal::TrmmLUN( diag, alpha, A, X );
        else
            internal::TrmmLUT( orientation, diag, alpha, A, X );
    }
    else if( uplo == LOWER )
    {
        if( orientation == NORMAL )
            internal::TrmmRLN( diag, alpha, A, X );
        else
            internal::TrmmRLT( orientation, diag, alpha, A, X );
    }
    else
    {
        if( orientation == NORMAL )
            internal::TrmmRUN( diag, alpha, A, X );
        else
            internal::TrmmRUT( orientation, diag, alpha, A, X );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace elem

#endif // ifndef BLAS_TRMM_HPP
