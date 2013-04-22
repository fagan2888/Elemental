/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_PARTITION_IMPL_HPP
#define CORE_PARTITION_IMPL_HPP

namespace elem {

// To make our life easier. Undef'd at the bottom of the header
#define AM AutoMatrix<Int>
#define M  Matrix<T,Int>
#define DM DistMatrix<T,U,V,Int>
#define ADM AutoDistMatrix<Int>

//
// Legend:
//      Function__: no consistency checks.
//      Function_: full consistency checking.
//      Function( AutoMatrix<Int>& ... )
//      Function( AutoDistMatrix<Int>& ... )
//			Inlined to Function_, so fully checked in both Release and Debug.
//			This is for use with dynamically-typed languages.
//      Function( Matrix<T,Int>& ... ):
//      Function( DistMatrix<T,U,V,Int>& ... ):
//			Inlined to Function__ in Release mode, for high performance.
//			Inlined to Function_  in Debug mode, for full consistency checking.
//

//
// PartitionUp
//

template<typename Q,typename Int> inline
void PartitionUp__( const Q& A, Q& AT, Q& AB, Int heightAB, bool lock )
{
    heightAB = std::min(heightAB,A.Height());
    const Int heightAT = A.Height() - heightAB;
    View__( AT, A, 0,        0, heightAT, A.Width(), lock );
    View__( AB, A, heightAT, 0, heightAB, A.Width(), lock );
}

template<typename Q,typename Int> inline
void PartitionUp_( const Q& A, Q& AT, Q& AB, Int heightAB, bool lock )
{
	PushCallStack( "PartitionUp" );
    AssertNonnegative( heightAB, "height of bottom partition" );
	AssertDataTypes( 3, &A, &AT, &AB );
	if ( !lock ) 
		AssertUnlocked( A );
	PartitionUp__( A, AT, AB, heightAB, lock );
	PopCallStack();
}

template<typename Int> inline void PartitionUp
( AM& A, AM& AT,
         AM& AB, Int heightAB )
{ PartitionUp_( A, AT, AB, heightAB, false ); }        
        
template<typename T,typename Int>
inline void
PartitionUp
( M& A, M& AT,
        M& AB, Int heightAB )
{ RUNDERSCORE(PartitionUp_)( A, AT, AB, heightAB, false ); }

template<typename Int>
inline void
PartitionUp
( ADM& A, ADM& AT,
          ADM& AB, Int heightAB )
{ PartitionUp_( A, AT, AB, heightAB, false ); }        
        
template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionUp
( DM& A, DM& AT,
         DM& AB, Int heightAB )
{ RUNDERSCORE(PartitionUp_)( A, AT, AB, heightAB, false ); }

template<typename Int>
inline void
LockedPartitionUp
( const AM& A, AM& AT,
               AM& AB, Int heightAB )
{ PartitionUp_( A, AT, AB, heightAB, true ); }        
        
template<typename T,typename Int>
inline void
LockedPartitionUp
( const M& A, M& AT,
              M& AB, Int heightAB )
{ RUNDERSCORE(PartitionUp_)( A, AT, AB, heightAB, true ); }

template<typename Int>
inline void
LockedPartitionUp
( const ADM& A, ADM& AT,
                ADM& AB, Int heightAB )
{ PartitionUp_( A, AT, AB, heightAB, true ); }        
        
template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionUp
( const DM& A, DM& AT,
               DM& AB, Int heightAB )
{ RUNDERSCORE(PartitionUp_)( A, AT, AB, heightAB, true ); }

//
// PartitionDown
//

template<typename Q,typename Int> inline
void PartitionDown__( const Q& A, Q& AT, Q& AB, Int heightAT, bool lock )
{
    heightAT = std::min(heightAT,A.Height());
    const Int heightAB = A.Height() - heightAT;
    View__( AT, A, 0,        0, heightAT, A.Width(), lock );
    View__( AB, A, heightAT, 0, heightAB, A.Width(), lock );
}

template<typename Q,typename Int> inline
void PartitionDown_( const Q& A, Q& AT, Q& AB, Int heightAT, bool lock )
{
	PushCallStack( "PartitionDown" );
    AssertNonnegative( heightAT, "height of top partition" );
	AssertDataTypes( 3, &A, &AT, &AB );
	if ( !lock ) 
		AssertUnlocked( A );
	PartitionDown__( A, AT, AB, heightAT, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionDown
( AM& A, AM& AT,
         AM& AB, Int heightAT ) 
{ PartitionDown_( A, AT, AB, heightAT, false ); }

template<typename T,typename Int>
inline void
PartitionDown
( M& A, M& AT,
        M& AB, Int heightAT ) 
{ RUNDERSCORE(PartitionDown_)( A, AT, AB, heightAT, false ); }

template<typename Int>
inline void
PartitionDown
( ADM& A, ADM& AT,
          ADM& AB, Int heightAT ) 
{ PartitionDown_( A, AT, AB, heightAT, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionDown
( DM& A, DM& AT,
         DM& AB, Int heightAT ) 
{ RUNDERSCORE(PartitionDown_)( A, AT, AB, heightAT, false ); }

template<typename Int>
inline void
LockedPartitionDown
( const AM& A, AM& AT,
               AM& AB, Int heightAT ) 
{ PartitionDown_( A, AT, AB, heightAT, true ); }

template<typename T,typename Int>
inline void
LockedPartitionDown
( const M& A, M& AT,
              M& AB, Int heightAT ) 
{ RUNDERSCORE(PartitionDown_)( A, AT, AB, heightAT, true ); }

template<typename Int>
inline void
LockedPartitionDown
( const ADM& A, ADM& AT,
                ADM& AB, Int heightAT ) 
{ PartitionDown_( A, AT, AB, heightAT, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionDown
( const DM& A, DM& AT,
               DM& AB, Int heightAT ) 
{ RUNDERSCORE(PartitionDown_)( A, AT, AB, heightAT, true ); }

//
// PartitionLeft
//

template<typename Q,typename Int> inline
void PartitionLeft__( const Q& A, Q& AL, Q& AR, Int widthAR, bool lock )
{
    widthAR = std::min(widthAR,A.Width());
    const Int widthAL = A.Width() - widthAR;
    View__( AL, A, 0, 0,       A.Height(), widthAL, lock );
    View__( AR, A, 0, widthAL, A.Height(), widthAR, lock );
}

template<typename Q,typename Int> inline
void PartitionLeft_( const Q& A, Q& AL, Q& AR, Int widthAR, bool lock )
{
	PushCallStack( "PartitionLeft" );
    AssertNonnegative( widthAR, "width of right partition" );
	AssertDataTypes( 3, &A, &AL, &AR );
	if ( !lock ) 
		AssertUnlocked( A );
	PartitionLeft__( A, AL, AR, widthAR, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionLeft( AM& A, AM& AL, AM& AR, Int widthAR )
{ PartitionLeft_( A, AL, AR, widthAR, false ); }

template<typename T,typename Int>
inline void
PartitionLeft( M& A, M& AL, M& AR, Int widthAR )
{ RUNDERSCORE(PartitionLeft_)( A, AL, AR, widthAR, false ); }

template<typename Int>
inline void
PartitionLeft( ADM& A, ADM& AL, ADM& AR, Int widthAR )
{ PartitionLeft_( A, AL, AR, widthAR, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionLeft( DM& A, DM& AL, DM& AR, Int widthAR )
{ RUNDERSCORE(PartitionLeft_)( A, AL, AR, widthAR, false ); }

template<typename Int>
inline void
LockedPartitionLeft( const AM& A, AM& AL, AM& AR, Int widthAR )
{ PartitionLeft_( A, AL, AR, widthAR, true ); }

template<typename T,typename Int>
inline void
LockedPartitionLeft( const M& A, M& AL, M& AR, Int widthAR )
{ RUNDERSCORE(PartitionLeft_)( A, AL, AR, widthAR, true ); }

template<typename Int>
inline void
LockedPartitionLeft( const ADM& A, ADM& AL, ADM& AR, Int widthAR )
{ PartitionLeft_( A, AL, AR, widthAR, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionLeft( const DM& A, DM& AL, DM& AR, Int widthAR )
{ RUNDERSCORE(PartitionLeft_)( A, AL, AR, widthAR, true ); }

//
// PartitionRight
//

template<typename Q,typename Int> inline
void PartitionRight__( const Q& A, Q& AL, Q& AR, Int widthAL, bool lock )
{
    widthAL = std::min(widthAL,A.Width());
    const Int widthAR = A.Width() - widthAL;
    View__( AL, A, 0, 0,       A.Height(), widthAL, lock );
    View__( AR, A, 0, widthAL, A.Height(), widthAR, lock );
}

template<typename Q,typename Int> inline
void PartitionRight_( const Q& A, Q& AL, Q& AR, Int widthAL, bool lock )
{
	PushCallStack( "PartitionRight" );
    AssertNonnegative( widthAL, "width of left partition" );
	AssertDataTypes( 3, &A, &AL, &AR );
	if ( !lock ) 
		AssertUnlocked( A );
	PartitionRight__( A, AL, AR, widthAL, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionRight( AM& A, AM& AL, AM& AR, Int widthAL )
{ PartitionRight_( A, AL, AR, widthAL, false ); }

template<typename T,typename Int>
inline void
PartitionRight( M& A, M& AL, M& AR, Int widthAL )
{ RUNDERSCORE(PartitionRight_)( A, AL, AR, widthAL, false ); }

template<typename Int>
inline void
PartitionRight( ADM& A, ADM& AL, ADM& AR, Int widthAL )
{ PartitionRight_( A, AL, AR, widthAL, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionRight( DM& A, DM& AL, DM& AR, Int widthAL )
{ RUNDERSCORE(PartitionRight_)( A, AL, AR, widthAL, false ); }

template<typename Int>
inline void
LockedPartitionRight( const AM& A, AM& AL, AM& AR, Int widthAL )
{ PartitionRight_( A, AL, AR, widthAL, true ); }

template<typename T,typename Int>
inline void
LockedPartitionRight( const M& A, M& AL, M& AR, Int widthAL )
{ RUNDERSCORE(PartitionRight_)( A, AL, AR, widthAL, true ); }

template<typename Int>
inline void
LockedPartitionRight( const ADM& A, ADM& AL, ADM& AR, Int widthAL )
{ PartitionRight_( A, AL, AR, widthAL, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionRight( const DM& A, DM& AL, DM& AR, Int widthAL )
{ RUNDERSCORE(PartitionRight_)( A, AL, AR, widthAL, true ); }

//
// PartitionUpDiagonal (synonymous with PartitionUpLeftDiagonal)
//

template<typename Q,typename Int> inline
void PartitionUpLeftDiagonal__( const Q& A, Q& ATL, Q& ATR, Q& ABL, Q& ABR, Int diagABR, bool lock )
{
	const Int 
		minDim = std::min(A.Width(),A.Height()),
		H1 = minDim - std::min(diagABR,minDim), 
		W1 = H1,
		H2 = A.Height() - H1, 
		W2 = A.Width() - W1;
	View__( ATL, A, 0,   0, H1, W1, lock );
	View__( ATR, A, 0,  W1, H1, W2, lock );
	View__( ABL, A, H1,  0, H2, W1, lock );
	View__( ABR, A, H1, W1, H2, W2, lock );
}

template<typename Q,typename Int> inline
void PartitionUpLeftDiagonal_( const Q& A, Q& ATL, Q& ATR, Q& ABL, Q& ABR, Int diagABR, bool lock )
{
	PushCallStack( "PartitionUpLeftDiagonal" );
    AssertNonnegative( diagABR, "size of bottom-right block" );
	AssertDataTypes( 5, &A, &ATL, &ATR, &ABL, &ABR );
	if ( !lock ) 
		AssertUnlocked( A );
	PartitionUpLeftDiagonal__( A, ATL, ATR, ABL, ABR, diagABR, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionUpDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename T,typename Int>
inline void
PartitionUpDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename Int>
inline void
PartitionUpDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionUpDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename Int>
inline void
LockedPartitionUpDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T,typename Int>
inline void
LockedPartitionUpDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename Int>
inline void
LockedPartitionUpDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionUpDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

//
// PartitionUpLeftDiagonal
//

template<typename Int>
inline void
PartitionUpLeftDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename T,typename Int>
inline void
PartitionUpLeftDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename Int>
inline void
PartitionUpLeftDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionUpLeftDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename Int>
inline void
LockedPartitionUpLeftDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T,typename Int>
inline void
LockedPartitionUpLeftDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename Int>
inline void
LockedPartitionUpLeftDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionUpLeftDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

//
// PartitionUpRightDiagonal
//

template<typename Q,typename Int> inline
void PartitionUpRightDiagonal__( const Q& A, Q& ATL, Q& ATR, Q& ABL, Q& ABR, Int diagABR, bool lock )
{
    const Int 
    	minDim = std::min(A.Height(),A.Width()),
		H2 = std::min(diagABR,minDim),
		W2 = H2,
		H1 = A.Height() - H2, 
		W1 = A.Width() - W2;
	View__( ATL, A, 0,   0, H1, W1, lock );
	View__( ATR, A, 0,  W1, H1, W2, lock );
	View__( ABL, A, H1,  0, H2, W1, lock );
	View__( ABR, A, H1, W1, H2, W2, lock );
}

template<typename Q,typename Int> inline
void PartitionUpRightDiagonal_( const Q& A, Q& ATL, Q& ATR, Q& ABL, Q& ABR, Int diagABR, bool lock )
{
	PushCallStack( "PartitionUpRightDiagonal" );
    AssertNonnegative( diagABR, "size of bottom-right block" );
	AssertDataTypes( 5, &A, &ATL, &ATR, &ABL, &ABR );
	if ( !lock ) 
		AssertUnlocked( A );
	PartitionUpRightDiagonal__( A, ATL, ATR, ABL, ABR, diagABR, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionUpRightDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpRightDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename T,typename Int>
inline void
PartitionUpRightDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpRightDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename Int>
inline void
PartitionUpRightDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagABR )
{ PartitionUpRightDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionUpRightDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpRightDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, false ); }

template<typename Int>
inline void
LockedPartitionUpRightDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpRightDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T,typename Int>
inline void
LockedPartitionUpRightDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpRightDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename Int>
inline void
LockedPartitionUpRightDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagABR )
{ PartitionUpRightDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionUpRightDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpRightDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

//
// PartitionDownDiagonal (synonymous with PartitionDownLeftDiagonal)
//

template<typename Q,typename Int> inline
void PartitionDownLeftDiagonal__( const Q& A, Q& ATL, Q& ATR, Q& ABL, Q& ABR, Int diagATL, bool lock )
{
    const Int 
    	minDim = std::min(A.Height(),A.Width()),
		H1 = std::min(diagATL,minDim),
		W1 = H1,
		H2 = A.Height() - H1, 
		W2 = A.Width() - W1;
	View__( ATL, A, 0,   0, H1, W1, lock );
	View__( ATR, A, 0,  W1, H1, W2, lock );
	View__( ABL, A, H1,  0, H2, W1, lock );
	View__( ABR, A, H1, W1, H2, W2, lock );
}

template<typename Q,typename Int> inline
void PartitionDownLeftDiagonal_( const Q& A, Q& ATL, Q& ATR, Q& ABL, Q& ABR, Int diagATL, bool lock )
{
	PushCallStack( "PartitionDownLeftDiagonal" );
    AssertNonnegative( diagATL, "size of top-left block" );
	AssertDataTypes( 5, &A, &ATL, &ATR, &ABL, &ABR );
	if ( !lock ) 
		AssertUnlocked( A );
	PartitionDownLeftDiagonal__( A, ATL, ATR, ABL, ABR, diagATL, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionDownDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagATL )
{ PartitionDownLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename T,typename Int>
inline void
PartitionDownDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename Int>
inline void
PartitionDownDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagATL )
{ PartitionDownLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionDownDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename Int>
inline void
LockedPartitionDownDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagATL )
{ PartitionDownLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, true ); }

template<typename T,typename Int>
inline void
LockedPartitionDownDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, true ); }

template<typename Int>
inline void
LockedPartitionDownDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagATL )
{ PartitionDownLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionDownDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, true ); }

//
// PartitionDownLeftDiagonal
//

template<typename Int>
inline void
PartitionDownLeftDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagATL )
{ PartitionDownLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename T,typename Int>
inline void
PartitionDownLeftDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename Int>
inline void
PartitionDownLeftDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagATL )
{ PartitionDownLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionDownLeftDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename Int>
inline void
LockedPartitionDownLeftDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagATL )
{ PartitionDownLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, true ); }

template<typename T,typename Int>
inline void
LockedPartitionDownLeftDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, true ); }

template<typename Int>
inline void
LockedPartitionDownLeftDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagATL )
{ PartitionDownLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionDownLeftDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, true ); }

//
// PartitionDownRightDiagonal
//

template<typename Q,typename Int> inline
void PartitionDownRightDiagonal__( const Q& A, Q& ATL, Q& ATR, Q& ABL, Q& ABR, Int diagATL, bool lock )
{
	const Int 
		minDim = std::min(A.Width(),A.Height()),
		H2 = minDim - std::min(diagATL,minDim),
		W2 = H2,
		H1 = A.Height() - H2, 
		W1 = A.Width() - W2;
	View__( ATL, A, 0,   0, H1, W1, lock );
	View__( ATR, A, 0,  W1, H1, W2, lock );
	View__( ABL, A, H1,  0, H2, W1, lock );
	View__( ABR, A, H1, W1, H2, W2, lock );
}

template<typename Q,typename Int> inline
void PartitionDownRightDiagonal_( const Q& A, Q& ATL, Q& ATR, Q& ABL, Q& ABR, Int diagATL, bool lock )
{
	PushCallStack( "PartitionDownRightDiagonal" );
    AssertNonnegative( diagATL, "size of top-left block" );
	AssertDataTypes( 5, &A, &ATL, &ATR, &ABL, &ABR );
	if ( !lock ) 
		AssertUnlocked( A );
	PartitionDownRightDiagonal__( A, ATL, ATR, ABL, ABR, diagATL, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionDownRightDiagonal
( AM& A, AM& ATL, AM& ATR,
         AM& ABL, AM& ABR, Int diagATL )
{ PartitionDownRightDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename T,typename Int>
inline void
PartitionDownRightDiagonal
( M& A, M& ATL, M& ATR,
        M& ABL, M& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownRightDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename Int>
inline void
PartitionDownRightDiagonal
( ADM& A, ADM& ATL, ADM& ATR,
          ADM& ABL, ADM& ABR, Int diagATL )
{ PartitionDownRightDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionDownRightDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownRightDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, false ); }

template<typename Int>
inline void
LockedPartitionDownRightDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagATL )
{ PartitionDownRightDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, true ); }

template<typename T,typename Int>
inline void
LockedPartitionDownRightDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagATL )
{ RUNDERSCORE(PartitionDownRightDiagonal_)( A, ATL, ATR, ABL, ABR, diagATL, true ); }

template<typename Int>
inline void
LockedPartitionDownRightDiagonal
( const ADM& A, ADM& ATL, ADM& ATR,
                ADM& ABL, ADM& ABR, Int diagATL )
{ PartitionDownRightDiagonal_( A, ATL, ATR, ABL, ABR, diagATL, true ); }

#undef AM
#undef DM
#undef M
#undef ADM

} // namespace elem

#endif // ifndef CORE_PARTITION_IMPL_HPP
