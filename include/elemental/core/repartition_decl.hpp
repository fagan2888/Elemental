/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_REPARTITION_DECL_HPP
#define CORE_REPARTITION_DECL_HPP

namespace elem {

// To make our life easier. Undef'd at the bottom of the header
#define AM AutoMatrix<Int>
#define M  Matrix<T,Int>
#define DM DistMatrix<T,U,V,Int>
#define ADM AutoDistMatrix<Int>

//
// RepartitionUp
//

template<typename Int>
void RepartitionUp
( AM& AT, AM& A0,
          AM& A1,
  AM& AB, AM& A2, Int bsize=Blocksize() );

template<typename T,typename Int>
void RepartitionUp
( M& AT, M& A0,
         M& A1,
  M& AB, M& A2, Int bsize=Blocksize() );

template<typename Int>
void RepartitionUp
( ADM& AT, ADM& A0,
           ADM& A1,
  ADM& AB, ADM& A2, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void RepartitionUp
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionUp
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2, Int bsize=Blocksize() );

template<typename T,typename Int>
void LockedRepartitionUp
( const M& AT, M& A0,
               M& A1,
  const M& AB, M& A2, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionUp
( const ADM& AT, ADM& A0,
                 ADM& A1,
  const ADM& AB, ADM& A2, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void LockedRepartitionUp
( const DM& AT, DM& A0,
                DM& A1,
  const DM& AB, DM& A2, Int bsize=Blocksize() );

//
// RepartitionDown
//

template<typename Int>
void RepartitionDown
( AM& AT, AM& A0,
          AM& A1,
  AM& AB, AM& A2, Int bsize=Blocksize() );

template<typename T,typename Int>
void RepartitionDown
( M& AT, M& A0,
         M& A1,
  M& AB, M& A2, Int bsize=Blocksize() );

template<typename Int>
void RepartitionDown
( ADM& AT, ADM& A0,
           ADM& A1,
  ADM& AB, ADM& A2, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void RepartitionDown
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionDown
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2, Int bsize=Blocksize() );

template<typename T,typename Int>
void LockedRepartitionDown
( const M& AT, M& A0,
               M& A1,
  const M& AB, M& A2, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionDown
( const ADM& AT, ADM& A0,
                 ADM& A1,
  const ADM& AB, ADM& A2, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void LockedRepartitionDown
( const DM& AT, DM& A0,
                DM& A1,
  const DM& AB, DM& A2, Int bsize=Blocksize() );

//
// RepartitionLeft
//

template<typename Int>
void RepartitionLeft
( AM& AL, AM& AR,
  AM& A0, AM& A1, AM& A2, Int bsize=Blocksize() );

template<typename T,typename Int>
void RepartitionLeft
( M& AL, M& AR,
  M& A0, M& A1, M& A2, Int bsize=Blocksize() );

template<typename Int>
void RepartitionLeft
( ADM& AL, ADM& AR,
  ADM& A0, ADM& A1, ADM& A2, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void RepartitionLeft
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionLeft
( const AM& AL, const AM& AR,
  AM& A0, AM& A1, AM& A2, Int bsize=Blocksize() );

template<typename T,typename Int>
void LockedRepartitionLeft
( const M& AL, const M& AR,
  M& A0, M& A1, M& A2, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionLeft
( const ADM& AL, const ADM& AR,
  ADM& A0, ADM& A1, ADM& A2, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void LockedRepartitionLeft
( const DM& AL, const DM& AR,
  DM& A0, DM& A1, DM& A2, Int bsize=Blocksize() );

//
// RepartitionRight
//

template<typename Int>
void RepartitionRight
( AM& AL, AM& AR,
  AM& A0, AM& A1, AM& A2, Int bsize=Blocksize() );

template<typename T,typename Int>
void RepartitionRight
( M& AL, M& AR,
  M& A0, M& A1, M& A2, Int bsize=Blocksize() );

template<typename Int>
void RepartitionRight
( ADM& AL, ADM& AR,
  ADM& A0, ADM& A1, ADM& A2, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void RepartitionRight
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionRight
( const AM& AL, const AM& AR,
  AM& A0, AM& A1, AM& A2, Int bsize=Blocksize() );

template<typename T,typename Int>
void LockedRepartitionRight
( const M& AL, const M& AR,
  M& A0, M& A1, M& A2, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionRight
( const ADM& AL, const ADM& AR,
  ADM& A0, ADM& A1, ADM& A2, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void LockedRepartitionRight
( const DM& AL, const DM& AR,
  DM& A0, DM& A1, DM& A2, Int bsize=Blocksize() );

//
// RepartitionUpDiagonal
//

template<typename Int>
void RepartitionUpDiagonal
( AM& ATL, AM& ATR, AM& A00, AM& A01, AM& A02,
                    AM& A10, AM& A11, AM& A12,
  AM& ABL, AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize=Blocksize() );

template<typename T,typename Int>
void RepartitionUpDiagonal
( M& ATL, M& ATR, M& A00, M& A01, M& A02,
                  M& A10, M& A11, M& A12,
  M& ABL, M& ABR, M& A20, M& A21, M& A22, Int bsize=Blocksize() );

template<typename Int>
void RepartitionUpDiagonal
( ADM& ATL, ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                      ADM& A10, ADM& A11, ADM& A12,
  ADM& ABL, ADM& ABR, ADM& A20, ADM& A21, ADM& A22, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void RepartitionUpDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionUpDiagonal
( const AM& ATL, const AM& ATR, AM& A00, AM& A01, AM& A02,
                                AM& A10, AM& A11, AM& A12,
  const AM& ABL, const AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize=Blocksize() );

template<typename T,typename Int>
void LockedRepartitionUpDiagonal
( const M& ATL, const M& ATR, M& A00, M& A01, M& A02,
                              M& A10, M& A11, M& A12,
  const M& ABL, const M& ABR, M& A20, M& A21, M& A22, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionUpDiagonal
( const AM& ATL, const AM& ATR, ADM& A00, ADM& A01, ADM& A02,
                                ADM& A10, ADM& A11, ADM& A12,
  const AM& ABL, const AM& ABR, ADM& A20, ADM& A21, ADM& A22, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void LockedRepartitionUpDiagonal
( const DM& ATL, const DM& ATR, DM& A00, DM& A01, DM& A02,
                                DM& A10, DM& A11, DM& A12,
  const DM& ABL, const DM& ABR, DM& A20, DM& A21, DM& A22, 
  Int bsize=Blocksize() );

//
// RepartitionDownDiagonal
//

template<typename Int>
void RepartitionDownDiagonal
( AM& ATL, AM& ATR, AM& A00, AM& A01, AM& A02,
                    AM& A10, AM& A11, AM& A12,
  AM& ABL, AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize=Blocksize() );

template<typename T,typename Int>
void RepartitionDownDiagonal
( M& ATL, M& ATR, M& A00, M& A01, M& A02,
                  M& A10, M& A11, M& A12,
  M& ABL, M& ABR, M& A20, M& A21, M& A22, Int bsize=Blocksize() );

template<typename Int>
void RepartitionDownDiagonal
( ADM& ATL, ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                      ADM& A10, ADM& A11, ADM& A12,
  ADM& ABL, ADM& ABR, ADM& A20, ADM& A21, ADM& A22, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void RepartitionDownDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionDownDiagonal
( const AM& ATL, const AM& ATR, AM& A00, AM& A01, AM& A02,
                                AM& A10, AM& A11, AM& A12,
  const AM& ABL, const AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize=Blocksize() );

template<typename T,typename Int>
void LockedRepartitionDownDiagonal
( const M& ATL, const M& ATR, M& A00, M& A01, M& A02,
                              M& A10, M& A11, M& A12,
  const M& ABL, const M& ABR, M& A20, M& A21, M& A22, Int bsize=Blocksize() );

template<typename Int>
void LockedRepartitionDownDiagonal
( const ADM& ATL, const ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                                  ADM& A10, ADM& A11, ADM& A12,
  const ADM& ABL, const ADM& ABR, ADM& A20, ADM& A21, ADM& A22, Int bsize=Blocksize() );

template<typename T,Distribution U,Distribution V,typename Int>
void LockedRepartitionDownDiagonal
( const DM& ATL, const DM& ATR, DM& A00, DM& A01, DM& A02,
                                DM& A10, DM& A11, DM& A12,
  const DM& ABL, const DM& ABR, DM& A20, DM& A21, DM& A22, 
  Int bsize=Blocksize() );

#undef AM
#undef DM
#undef M
#undef ADM

} // namespace elem

#endif // ifndef CORE_REPARTITION_DECL_HPP
