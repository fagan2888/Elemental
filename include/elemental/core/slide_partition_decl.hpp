/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_SLIDEPARTITION_DECL_HPP
#define CORE_SLIDEPARTITION_DECL_HPP

namespace elem {

// To make our life easier. Undef'd at the bottom of the header
#define AM AutoMatrix<Int>
#define M  Matrix<T,Int>
#define DM DistMatrix<T,U,V,Int>

//
// SlidePartitionUp
//

template<typename Int>
void SlidePartitionUp
( AM& AT, AM& A0,
          AM& A1,
  AM& AB, AM& A2 );

template<typename T,typename Int>
void SlidePartitionUp
( M& AT, M& A0,
         M& A1,
  M& AB, M& A2 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlidePartitionUp
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2 );

template<typename Int>
void SlideLockedPartitionUp
( AM& AT, const AM& A0,
          const AM& A1,
  AM& AB, const AM& A2 );

template<typename T,typename Int>
void SlideLockedPartitionUp
( M& AT, const M& A0,
         const M& A1,
  M& AB, const M& A2 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlideLockedPartitionUp
( DM& AT, const DM& A0,
          const DM& A1,
  DM& AB, const DM& A2 );

//
// SlidePartitionDown
//

template<typename Int>
void SlidePartitionDown
( AM& AT, AM& A0,
          AM& A1,
  AM& AB, AM& A2 );

template<typename T,typename Int>
void SlidePartitionDown
( M& AT, M& A0,
         M& A1,
  M& AB, M& A2 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlidePartitionDown
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2 );

template<typename Int>
void SlideLockedPartitionDown
( AM& AT, const AM& A0,
          const AM& A1,
  AM& AB, const AM& A2 );

template<typename T,typename Int>
void SlideLockedPartitionDown
( M& AT, const M& A0,
         const M& A1,
  M& AB, const M& A2 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlideLockedPartitionDown
( DM& AT, const DM& A0,
          const DM& A1,
  DM& AB, const DM& A2 );

//
// SlidePartitionLeft
//

template<typename Int>
void SlidePartitionLeft
( AM& AL, AM& AR,
  AM& A0, AM& A1, AM& A2 );

template<typename T,typename Int>
void SlidePartitionLeft
( M& AL, M& AR,
  M& A0, M& A1, M& A2 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlidePartitionLeft
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2 );

template<typename Int>
void SlideLockedPartitionLeft
( AM& AL, AM& AR,
  const AM& A0, const AM& A1, const AM& A2 ); 

template<typename T,typename Int>
void SlideLockedPartitionLeft
( M& AL, M& AR,
  const M& A0, const M& A1, const M& A2 ); 

template<typename T,Distribution U,Distribution V,typename Int>
void SlideLockedPartitionLeft
( DM& AL, DM& AR,
  const DM& A0, const DM& A1, const DM& A2 );

//
// SlidePartitionRight
//

template<typename Int>
void SlidePartitionRight
( AM& AL, AM& AR,
  AM& A0, AM& A1, AM& A2 );

template<typename T,typename Int>
void SlidePartitionRight
( M& AL, M& AR,
  M& A0, M& A1, M& A2 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlidePartitionRight
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2 );

template<typename Int>
void SlideLockedPartitionRight
( AM& AL, AM& AR,
  const AM& A0, const AM& A1, const AM& A2 );

template<typename T,typename Int>
void SlideLockedPartitionRight
( M& AL, M& AR,
  const M& A0, const M& A1, const M& A2 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlideLockedPartitionRight
( DM& AL, DM& AR,
  const DM& A0, const DM& A1, const DM& A2 );

//
// SlidePartitionUpDiagonal
//

template<typename Int>
void SlidePartitionUpDiagonal
( AM& ATL, AM& ATR, AM& A00, AM& A01, AM& A02,
                    AM& A10, AM& A11, AM& A12,
  AM& ABL, AM& ABR, AM& A20, AM& A21, AM& A22 );

template<typename T,typename Int>
void SlidePartitionUpDiagonal
( M& ATL, M& ATR, M& A00, M& A01, M& A02,
                  M& A10, M& A11, M& A12,
  M& ABL, M& ABR, M& A20, M& A21, M& A22 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlidePartitionUpDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22 );

template<typename Int>
void SlideLockedPartitionUpDiagonal
( AM& ATL, AM& ATR, const AM& A00, const AM& A01, const AM& A02,
                    const AM& A10, const AM& A11, const AM& A12,
  AM& ABL, AM& ABR, const AM& A20, const AM& A21, const AM& A22 );

template<typename T,typename Int>
void SlideLockedPartitionUpDiagonal
( M& ATL, M& ATR, const M& A00, const M& A01, const M& A02,
                  const M& A10, const M& A11, const M& A12,
  M& ABL, M& ABR, const M& A20, const M& A21, const M& A22 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlideLockedPartitionUpDiagonal
( DM& ATL, DM& ATR, const DM& A00, const DM& A01, const DM& A02,
                    const DM& A10, const DM& A11, const DM& A12,
  DM& ABL, DM& ABR, const DM& A20, const DM& A21, const DM& A22 );

//
// SlidePartitionDownDiagonal
//

template<typename Int>
void SlidePartitionDownDiagonal
( AM& ATL, AM& ATR, AM& A00, AM& A01, AM& A02,
                    AM& A10, AM& A11, AM& A12,
  AM& ABL, AM& ABR, AM& A20, AM& A21, AM& A22 );

template<typename T,typename Int>
void SlidePartitionDownDiagonal
( M& ATL, M& ATR, M& A00, M& A01, M& A02,
                  M& A10, M& A11, M& A12,
  M& ABL, M& ABR, M& A20, M& A21, M& A22 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlidePartitionDownDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22 );

template<typename T,typename Int>
void SlideLockedPartitionDownDiagonal
( AM& ATL, AM& ATR, const AM& A00, const AM& A01, const AM& A02,
                    const AM& A10, const AM& A11, const AM& A12,
  AM& ABL, AM& ABR, const AM& A20, const AM& A21, const AM& A22 );

template<typename T,typename Int>
void SlideLockedPartitionDownDiagonal
( M& ATL, M& ATR, const M& A00, const M& A01, const M& A02,
                  const M& A10, const M& A11, const M& A12,
  M& ABL, M& ABR, const M& A20, const M& A21, const M& A22 );

template<typename T,Distribution U,Distribution V,typename Int>
void SlideLockedPartitionDownDiagonal
( DM& ATL, DM& ATR, const DM& A00, const DM& A01, const DM& A02,
                    const DM& A10, const DM& A11, const DM& A12,
  DM& ABL, DM& ABR, const DM& A20, const DM& A21, const DM& A22 );

#undef AM
#undef DM
#undef M

} // namespace elem

#endif // ifndef CORE_SLIDEPARTITION_DECL_HPP
