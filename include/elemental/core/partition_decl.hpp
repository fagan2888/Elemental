/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_PARTITION_DECL_HPP
#define CORE_PARTITION_DECL_HPP

namespace elem {

// To make our life easier. Undef'd at the bottom of the header
#define AM AutoMatrix<Int>
#define M Matrix<T,Int>
#define DM DistMatrix<T,U,V,Int>
#define ADM AutoDistMatrix<Int>

//
// PartitionUp
// 

template<typename Int>
void PartitionUp
( AM& A, AM& AT,
         AM& AB, Int heightAB=Blocksize() );

template<typename T,typename Int>
void PartitionUp
( M& A, M& AT,
        M& AB, Int heightAB=Blocksize() );

template<typename Int>
void PartitionUp
( ADM& A, ADM& AT,
          ADM& AB, Int heightAB=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionUp
( DM& A, DM& AT,
         DM& AB, Int heightAB=Blocksize() );

template<typename Int>
void LockedPartitionUp
( const AM& A, AM& AT,
               AM& AB, Int heightAB=Blocksize() );

template<typename T,typename Int>
void LockedPartitionUp
( const M& A, M& AT,
              M& AB, Int heightAB=Blocksize() );

template<typename Int>
void LockedPartitionUp
( const ADM& A, ADM& AT,
                ADM& AB, Int heightAB=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionUp
( const DM& A, DM& AT,
               DM& AB, Int heightAB=Blocksize() );

//
// PartitionDown
//

template<typename Int>
void PartitionDown
( AM& A, AM& AT,
         AM& AB, Int heightAT=Blocksize() );

template<typename T,typename Int>
void PartitionDown
( M& A, M& AT,
        M& AB, Int heightAT=Blocksize() );

template<typename Int>
void PartitionDown
( ADM& A, ADM& AT,
          ADM& AB, Int heightAT=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionDown
( DM& A, DM& AT,
         DM& AB, Int heightAT=Blocksize() );

template<typename Int>
void LockedPartitionDown
( const AM& A, AM& AT,
               AM& AB, Int heightAT=Blocksize() );

template<typename T,typename Int>
void LockedPartitionDown
( const M& A, M& AT,
              M& AB, Int heightAT=Blocksize() );

template<typename Int>
void LockedPartitionDown
( const ADM& A, ADM& AT,
                ADM& AB, Int heightAT=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionDown
( const DM& A, DM& AT,
               DM& AB, Int heightAT=Blocksize() );

//
// PartitionLeft
//

template<typename Int>
void PartitionLeft
( AM& A, AM& AL, AM& AR, Int widthAR=Blocksize() );

template<typename T,typename Int>
void PartitionLeft
( M& A, M& AL, M& AR, Int widthAR=Blocksize() );

template<typename Int>
void PartitionLeft
( ADM& A, ADM& AL, ADM& AR, Int widthAR=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionLeft
( DM& A, DM& AL, DM& AR, Int widthAR=Blocksize() );

template<typename Int>
void LockedPartitionLeft
( const AM& A, AM& AL, AM& AR, Int widthAR=Blocksize() );

template<typename T,typename Int>
void LockedPartitionLeft
( const M& A, M& AL, M& AR, Int widthAR=Blocksize() );

template<typename Int>
void LockedPartitionLeft
( const ADM& A, ADM& AL, ADM& AR, Int widthAR=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionLeft
( const DM& A, DM& AL, DM& AR, Int widthAR=Blocksize() );

//
// PartitionRight
//

template<typename Int>
void PartitionRight
( AM& A, AM& AL, AM& AR, Int widthAL=Blocksize() );

template<typename T,typename Int>
void PartitionRight
( M& A, M& AL, M& AR, Int widthAL=Blocksize() );

template<typename Int>
void PartitionRight
( ADM& A, ADM& AL, ADM& AR, Int widthAL=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionRight
( DM& A, DM& AL, DM& AR, Int widthAL=Blocksize() );

template<typename Int>
void LockedPartitionRight
( const AM& A, AM& AL, AM& AR, Int widthAL=Blocksize() );

template<typename T,typename Int>
void LockedPartitionRight
( const M& A, M& AL, M& AR, Int widthAL=Blocksize() );

template<typename Int>
void LockedPartitionRight
( const ADM& A, ADM& AL, ADM& AR, Int widthAL=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionRight
( const DM& A, DM& AL, DM& AR, Int widthAL=Blocksize() );

//
// PartitionUpDiagonal
//

template<typename Int>
void PartitionUpDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagABR=Blocksize() );

template<typename T,typename Int>
void PartitionUpDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagABR=Blocksize() );

template<typename Int>
void PartitionUpDiagonal
( AM& A, ADM& ATL, ADM& ATR,
         ADM& ABL, ADM& ABR, Int diagABR=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionUpDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR=Blocksize() );

template<typename Int>
void LockedPartitionUpDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR=Blocksize() );

template<typename T,typename Int>
void LockedPartitionUpDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR=Blocksize() );

template<typename Int>
void LockedPartitionUpDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagABR=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionUpDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR=Blocksize() );

//
// PartitionUpLeftDiagonal
//

template<typename Int>
void PartitionUpLeftDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagABR=Blocksize() );

template<typename T,typename Int>
void PartitionUpLeftDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagABR=Blocksize() );

template<typename Int>
void PartitionUpLeftDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagABR=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionUpLeftDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR=Blocksize() );

template<typename Int>
void LockedPartitionUpLeftDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR=Blocksize() );

template<typename T,typename Int>
void LockedPartitionUpLeftDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR=Blocksize() );

template<typename Int>
void LockedPartitionUpLeftDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagABR=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionUpLeftDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR=Blocksize() );

//
// PartitionUpRightDiagonal
//

template<typename Int>
void PartitionUpRightDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagABR=Blocksize() );

template<typename T,typename Int>
void PartitionUpRightDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagABR=Blocksize() );

template<typename Int>
void PartitionUpRightDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagABR=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionUpRightDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR=Blocksize() );

template<typename Int>
void LockedPartitionUpRightDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR=Blocksize() );

template<typename T,typename Int>
void LockedPartitionUpRightDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR=Blocksize() );

template<typename Int>
void LockedPartitionUpRightDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagABR=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionUpRightDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR=Blocksize() );

//
// PartitionDownDiagonal
//

template<typename Int>
void PartitionDownDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagATL=Blocksize() );

template<typename T,typename Int>
void PartitionDownDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagATL=Blocksize() );

template<typename Int>
void PartitionDownDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagATL=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionDownDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL=Blocksize() );

template<typename Int>
void LockedPartitionDownDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagATL=Blocksize() );

template<typename T,typename Int>
void LockedPartitionDownDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagATL=Blocksize() );

template<typename Int>
void LockedPartitionDownDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagATL=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionDownDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagATL=Blocksize() );

//
// PartitionDownLeftDiagonal
//

template<typename Int>
void PartitionDownLeftDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagATL=Blocksize() );

template<typename T,typename Int>
void PartitionDownLeftDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagATL=Blocksize() );

template<typename Int>
void PartitionDownLeftDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagATL=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionDownLeftDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL=Blocksize() );

template<typename Int>
void LockedPartitionDownLeftDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagATL=Blocksize() );

template<typename T,typename Int>
void LockedPartitionDownLeftDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagATL=Blocksize() );

template<typename Int>
void LockedPartitionDownLeftDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagATL=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionDownLeftDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagATL=Blocksize() );

//
// PartitionDownRightDiagonal
//

template<typename Int>
void PartitionDownRightDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagATL=Blocksize() );

template<typename T,typename Int>
void PartitionDownRightDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagATL=Blocksize() );

template<typename Int>
void PartitionDownRightDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagATL=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void PartitionDownRightDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL=Blocksize() );

template<typename Int>
void LockedPartitionDownRightDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagATL=Blocksize() );

template<typename T,typename Int>
void LockedPartitionDownRightDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagATL=Blocksize() );

template<typename Int>
void LockedPartitionDownRightDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagATL=Blocksize() );

template<typename T, Distribution U, Distribution V,typename Int>
void LockedPartitionDownRightDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagATL=Blocksize() );

#undef M
#undef AM
#undef DM
#undef ADM

} // namespace elem

#endif // ifndef CORE_PARTITION_DECL_HPP
