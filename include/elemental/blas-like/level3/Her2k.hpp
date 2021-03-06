/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef BLAS_HER2K_HPP
#define BLAS_HER2K_HPP

#include "elemental/blas-like/level3/Syr2k.hpp"

namespace elem {

template<typename Int>
void Her2k
( UpperOrLower uplo, Orientation orientation,
  const Scalar<Int>& alpha, const AutoMatrix<Int>& A, const AutoMatrix<Int>& B, 
  const Scalar<Int>& beta, AutoMatrix<Int>& C );

template<typename T>
inline void
Her2k
( UpperOrLower uplo, Orientation orientation,
  T alpha, const Matrix<T>& A, const Matrix<T>& B, T beta, Matrix<T>& C )
{
#ifndef RELEASE
    PushCallStack("Her2k");
#endif
    Syr2k( uplo, orientation, alpha, A, B, beta, C, true );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
inline void
Her2k
( UpperOrLower uplo, Orientation orientation,
  T alpha, const DistMatrix<T>& A, const DistMatrix<T>& B,
  T beta,        DistMatrix<T>& C )
{
#ifndef RELEASE
    PushCallStack("Her2k");
#endif
    Syr2k( uplo, orientation, alpha, A, B, beta, C, true );
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace elem

#endif // ifndef BLAS_HER2K_HPP
