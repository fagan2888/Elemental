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

//
// Added a new syntax for the non-Locked cases that accepts const B matrix inputs and
// toggles locking with a boolean variable. This syntax serves two purposes: first, it
// improves code reuse. Second, it facilitates our strategy of separating consistency
// checking from processing to maximize performance in statically typed programs.
// 
// The doubly underscored versions PERFORM NO TYPE CHECKING. So they should only be used 
// in release mode, and only then in statically typed code. To automate this, we use
// a macro to swap in the fully checked version in debug mode.
//

#ifdef RELEASE
#define RUNDERSCORE(x) x ## _
#else
#define RUNDERSCORE(x) x
#endif

//
// PartitionUp
//

template<typename Int> inline
void PartitionUp__( const AM& A, AM& AT, AM& AB, Int heightAB, bool lock )
{
    heightAB = std::min(heightAB,A.Height());
    const Int heightAT = A.Height() - heightAB;
    View__( AT, A, 0, 0, A.Width(), heightAT, lock );
    View__( AB, A, heightAT, 0, A.Width(), heightAB, lock );
}

template<typename Int> inline
void PartitionUp_( const AM& A, AM& AT, AM& AB, Int heightAB, bool lock )
{
	PushCallStack( "PartitionUp [Matrix]" );
    A.AssertNonnegative( heightAB, "height of bottom partition" );
	A.AssertDataTypes( AT );
	A.AssertDataTypes( AB );
	if ( !lock ) A.AssertUnlocked( AM::PartitionLock );
	PartitionUp__( AT, AB, heightAB, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionUp
( AM& A, AM& AT,
         AM& AB, Int heightAB )
{ PartitionUp_( A, AT, AB, heightAB, false ); }        
        
template<typename T,typename Int>
inline void
PartitionUp
( M& A, M& AT,
        M& AB, Int heightAB )
{ RUNDERSCORE(PartitionUp_)( A, AT, AB, heightAB, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionUp
( DM& A, DM& AT,
         DM& AB, Int heightAB )
{
#ifndef RELEASE
    PushCallStack("PartitionUp [DistMatrix]");
    if( heightAB < 0 )
        throw std::logic_error
        ("Height of bottom partition must be non-negative");
#endif
    heightAB = std::min(heightAB,A.Height());
    const Int heightAT = A.Height()-heightAB;
    View( AT, A, 0,        0, heightAT, A.Width() );
    View( AB, A, heightAT, 0, heightAB, A.Width() );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionUp
( const DM& A, DM& AT,
               DM& AB, Int heightAB )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionUp [DistMatrix]");
    if( heightAB < 0 )
        throw std::logic_error
        ("Height of bottom partition must be non-negative");
#endif
    heightAB = std::min(heightAB,A.Height());
    const Int heightAT = A.Height()-heightAB;
    LockedView( AT, A, 0,        0, heightAT, A.Width() );
    LockedView( AB, A, heightAT, 0, heightAB, A.Width() );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// PartitionDown
//

template<typename Int> inline
void PartitionDown__( const AM& A, AM& AT, AM& AB, Int heightAT, bool lock )
{
    heightAT = std::min(heightAT,A.Height());
    const Int heightAB = A.Height() - heightAT;
    View__( AT, A, 0, 0, A.Width(), heightAT, lock );
    View__( AB, A, heightAT, 0, A.Width(), heightAB, lock );
}

template<typename Int> inline
void PartitionDown_( const AM& A, AM& AT, AM& AB, Int heightAT, bool lock )
{
	PushCallStack( "PartitionDown [Matrix]" );
    A.AssertNonnegative( heightAT, "height of top partition" );
	A.AssertDataTypes( AT );
	A.AssertDataTypes( AB );
	if ( !lock ) A.AssertUnlocked( AM::PartitionLock );
	PartitionDown__( AT, AB, heightAT, lock );
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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionDown
( DM& A, DM& AT,
         DM& AB, Int heightAT )
{
#ifndef RELEASE
    PushCallStack("PartitionDown [DistMatrix]");
    if( heightAT < 0 )
        throw std::logic_error("Height of top partition must be non-negative");
#endif
    heightAT = std::min(heightAT,A.Height());
    const Int heightAB = A.Height()-heightAT;
    View( AT, A, 0,        0, heightAT, A.Width() );
    View( AB, A, heightAT, 0, heightAB, A.Width() );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionDown
( const DM& A, DM& AT,
               DM& AB, Int heightAT )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionDown [DistMatrix]");
    if( heightAT < 0 )
        throw std::logic_error("Height of top partition must be non-negative");
#endif
    heightAT = std::min(heightAT,A.Height());
    const Int heightAB = A.Height()-heightAT;
    LockedView( AT, A, 0,        0, heightAT, A.Width() );
    LockedView( AB, A, heightAT, 0, heightAB, A.Width() );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// PartitionLeft
//

template<typename Int> inline
void PartitionLeft__( const AM& A, AM& AL, AM& AR, Int widthAR, bool lock )
{
    widthAR = std::min(widthAR,A.Width());
    const Int widthAL = A.Width() - widthAR;
    View__( AL, A, 0, 0, A.Height(), widthAL, lock );
    View__( AR, A, 0, widthAL, A.Height(), widthAR, lock );
}

template<typename Int> inline
void PartitionLeft_( const AM& A, AM& AL, AM& AR, Int widthAR, bool lock )
{
	PushCallStack( "PartitionLeft [Matrix]" );
    A.AssertNonnegative( widthAR, "width of right partition" );
	A.AssertDataTypes( AL );
	A.AssertDataTypes( AR );	
	if ( !lock ) A.AssertUnlocked( AM::PartitionLock );
	PartitionLeft__( AL, AR, widthAR, lock );
	PopCallStack();
}

template<typename Int>
inline void
PartitionLeft( AM& A, AM& AL, AM& AR, int widthAR )
{ PartitionLeft_( A, AL, AR, widthAR, false ); }

template<typename Int,typename T>
inline void
PartitionLeft( M& A, M& AL, M& AR, int widthAR )
{ RUNDERSCORE(PartitionLeft_)( A, AL, AR, widthAR, false ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionLeft( DM& A, DM& AL, DM& AR, Int widthAR )
{
#ifndef RELEASE
    PushCallStack("PartitionLeft [DistMatrix]");
    if( widthAR < 0 )
        throw std::logic_error("Width of right partition must be non-negative");
#endif
    widthAR = std::min(widthAR,A.Width());
    const Int widthAL = A.Width()-widthAR;
    View( AL, A, 0, 0,       A.Height(), widthAL );
    View( AR, A, 0, widthAL, A.Height(), widthAR );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
LockedPartitionLeft( const AM& A, AM& AL, AM& AR, int widthAR )
{ PartitionLeft_( A, AL, AR, widthAR, true ); }

template<typename Int,typename T>
inline void
LockedPartitionLeft( const M& A, M& AL, M& AR, int widthAR )
{ RUNDERSCORE(PartitionLeft_)( A, AL, AR, widthAR, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionLeft( const DM& A, DM& AL, DM& AR, Int widthAR )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionLeft [DistMatrix]");
    if( widthAR < 0 )
        throw std::logic_error("Width of right partition must be non-negative");
#endif
    widthAR = std::min(widthAR,A.Width());
    const Int widthAL = A.Width()-widthAR;
    LockedView( AL, A, 0, 0,       A.Height(), widthAL );
    LockedView( AR, A, 0, widthAL, A.Height(), widthAR );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// PartitionRight
//

template<typename Int> inline
void PartitionRight__( const AM& A, AM& AL, AM& AR, Int widthAL, bool lock )
{
    widthAL = std::min(widthAL,A.Width());
    const Int widthAR = A.Width() - widthAL;
    View__( AL, A, 0, 0, A.Height(), widthAL, lock );
    View__( AR, A, 0, widthAL, A.Height(), widthAR, lock );
}

template<typename Int> inline
void PartitionRight_( const AM& A, AM& AL, AM& AR, Int widthAL, bool lock )
{
	PushCallStack( "PartitionRight [Matrix]" );
    A.AssertNonnegative( widthAL, "width of left partition" );
	A.AssertDataTypes( AL );
	A.AssertDataTypes( AR );		
	if ( !lock ) A.AssertUnlocked( AM::PartitionLock );
	PartitionRight__( AL, AR, widthAL, lock );
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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionRight( DM& A, DM& AL, DM& AR, Int widthAL )
{
#ifndef RELEASE
    PushCallStack("PartitionRight [DistMatrix]");
    if( widthAL < 0 )
        throw std::logic_error("Width of left partition must be non-negative");
#endif
    widthAL = std::min(widthAL,A.Width());
    const Int widthAR = A.Width()-widthAL;
    View( AL, A, 0, 0,       A.Height(), widthAL );
    View( AR, A, 0, widthAL, A.Height(), widthAR );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
LockedPartitionRight( const AM& A, AM& AL, AM& AR, Int widthAL )
{ PartitionRight_( A, AL, AR, widthAL, true ); }

template<typename T,typename Int>
inline void
LockedPartitionRight( const M& A, M& AL, M& AR, Int widthAL )
{ RUNDERSCORE(PartitionRight_)( A, AL, AR, widthAL, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionRight( const DM& A, DM& AL, DM& AR, Int widthAL )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionRight [DistMatrix]");
    if( widthAL < 0 )
        throw std::logic_error("Width of left partition must be non-negative");
#endif
    widthAL = std::min(widthAL,A.Width());
    const Int widthAR = A.Width()-widthAL;
    LockedView( AL, A, 0, 0,       A.Height(), widthAL );
    LockedView( AR, A, 0, widthAL, A.Height(), widthAR );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// PartitionUpDiagonal (synonymous with PartitionUpLeftDiagonal)
//

template<typename Int> inline
void PartitionUpLeftDiagonal__( const AM& A, AM& ATL, AM& ATR, AM& ABL, AM& ABR, Int diagABR, bool lock )
{
	const Int 
		minDim = std::min(A.Width(),A.Height()),
		H1 = minDim - std::min(diagABR,minDim), 
		W1 = H1,
		H2 = A.Height() - H1, 
		W2 = A.Width() - W1;
	View__( ATL, A, 0,  0, H1, W1, lock );
	View__( ATR, A, 0, W1, H1, W2, lock );
	View__( ABL, A, H1, 0, H2, W1, lock );
	View__( ABR, A, H1, 0, H2, W2, lock );
}

template<typename Int> inline
void PartitionUpLeftDiagonal_( const AM& A, AM& ATL, AM& ATR, AM& ABL, AM& ABR, Int diagABR, bool lock )
{
	PushCallStack( "PartitionUpLeftDiagonal [Matrix]" );
    A.AssertNonnegative( diagABR, "size of bottom-right block" );
	A.AssertDataTypes( ATL );
	A.AssertDataTypes( ATR );		
	A.AssertDataTypes( ABL );
	A.AssertDataTypes( ABR );
	if ( !lock ) A.AssertUnlocked( AM::PartitionLock );
	PartitionUpLeftDiagonal__( ATL, ATR, ABL, ABR, diagABR, lock );
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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionUpDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR )
{
#ifndef RELEASE
    PushCallStack("PartitionUpDiagonal [DistMatrix]");
#endif
    PartitionUpLeftDiagonal
    ( A, ATL, ATR,
         ABL, ABR, diagABR );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
LockedPartitionUpDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T,typename Int>
inline void
PartitionUpDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionUpDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionUpDiagonal [DistMatrix]");
#endif
    LockedPartitionUpLeftDiagonal
    ( A, ATL, ATR,
         ABL, ABR, diagABR );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionUpLeftDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR )
{
#ifndef RELEASE
    PushCallStack("PartitionUpLeftDiagonal [DistMatrix]");
    if( diagABR < 0 )
        throw std::logic_error("Bottom-right size must be non-negative");
#endif
    const Int minDim = std::min( A.Height(), A.Width() );
    diagABR = std::min(diagABR,minDim);
    const Int sizeATL = minDim - diagABR;
    const Int remHeight = A.Height()-sizeATL;
    const Int remWidth = A.Width()-sizeATL;
    View( ATL, A, 0,       0,       sizeATL,   sizeATL  );
    View( ATR, A, 0,       sizeATL, sizeATL,   remWidth );
    View( ABL, A, sizeATL, 0,       remHeight, sizeATL  );
    View( ABR, A, sizeATL, sizeATL, remHeight, remWidth );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
LockedPartitionUpLeftDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpLeftDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T,typename Int>
inline void
PartitionUpLeftDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpLeftDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionUpLeftDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionUpLeftDiagonal [DistMatrix]");
    if( diagABR < 0 )
        throw std::logic_error("Bottom-right size must be non-negative");
#endif
    const Int minDim = std::min( A.Height(), A.Width() );
    diagABR = std::min(diagABR,minDim);
    const Int sizeATL = minDim - diagABR;
    const Int remHeight = A.Height()-sizeATL;
    const Int remWidth = A.Width()-sizeATL;
    LockedView( ATL, A, 0,       0,       sizeATL,   sizeATL  );
    LockedView( ATR, A, 0,       sizeATL, sizeATL,   remWidth );
    LockedView( ABL, A, sizeATL, 0,       remHeight, sizeATL  );
    LockedView( ABR, A, sizeATL, sizeATL, remHeight, remWidth );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// PartitionUpRightDiagonal
//

template<typename Int> inline
void PartitionUpRightDiagonal__( const AM& A, AM& ATL, AM& ATR, AM& ABL, AM& ABR, Int diagABR, bool lock )
{
    const Int 
    	minDim = std::min(A.Height(),A.Width()),
		H2 = std::min(diagABR,minDim),
		W2 = H2,
		H1 = A.Height() - H2, 
		W1 = A.Width() - W2;
	View__( ATL, A, 0,  0, H1, W1, lock );
	View__( ATR, A, 0, W1, H1, W2, lock );
	View__( ABL, A, H1, 0, H2, W1, lock );
	View__( ABR, A, H1, 0, H2, W2, lock );
}

template<typename Int> inline
void PartitionUpRightDiagonal_( const AM& A, AM& ATL, AM& ATR, AM& ABL, AM& ABR, Int diagABR, bool lock )
{
	PushCallStack( "PartitionUpRightDiagonal [Matrix]" );
    A.AssertNonnegative( diagABR, "size of bottom-right block" );
	A.AssertDataTypes( ATL );
	A.AssertDataTypes( ATR );		
	A.AssertDataTypes( ABL );
	A.AssertDataTypes( ABR );
	if ( !lock ) A.AssertUnlocked( AM::PartitionLock );
	PartitionUpRightDiagonal__( ATL, ATR, ABL, ABR, diagABR, lock );
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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionUpRightDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagABR )
{
#ifndef RELEASE
    PushCallStack("PartitionUpRightDiagonal [DistMatrix]");
    if( diagABR < 0 )
        throw std::logic_error("Bottom-right size must be non-negative");
#endif
    const Int minDim = std::min(A.Height(),A.Width());
    diagABR = std::min(diagABR,minDim);
    const Int remHeight = A.Height()-diagABR;
    const Int remWidth = A.Width()-diagABR;
    View( ATL, A, 0,         0,        remHeight, remWidth );
    View( ATR, A, 0,         remWidth, remHeight, diagABR  );
    View( ABL, A, remHeight, 0,        diagABR,   remWidth );
    View( ABR, A, remHeight, remWidth, diagABR,   diagABR  );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
LockedPartitionUpRightDiagonal
( const AM& A, AM& ATL, AM& ATR,
               AM& ABL, AM& ABR, Int diagABR )
{ PartitionUpRightDiagonal_( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T,typename Int>
inline void
PartitionUpRightDiagonal
( const M& A, M& ATL, M& ATR,
              M& ABL, M& ABR, Int diagABR )
{ RUNDERSCORE(PartitionUpRightDiagonal_)( A, ATL, ATR, ABL, ABR, diagABR, true ); }

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionUpRightDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagABR )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionUpRightDiagonal [DistMatrix]");
    if( diagABR < 0 )
        throw std::logic_error("Bottom-right size must be non-negative");
#endif
    const Int minDim = std::min(A.Height(),A.Width());
    diagABR = std::min(diagABR,minDim);
    const Int remHeight = A.Height()-diagABR;
    const Int remWidth = A.Width()-diagABR;
    LockedView( ATL, A, 0,         0,        remHeight, remWidth );
    LockedView( ATR, A, 0,         remWidth, remHeight, diagABR  );
    LockedView( ABL, A, remHeight, 0,        diagABR,   remWidth );
    LockedView( ABR, A, remHeight, remWidth, diagABR,   diagABR  );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// PartitionDownDiagonal (synonymous with PartitionDownLeftDiagonal)
//

template<typename Int> inline
void PartitionDownLeftDiagonal__( const AM& A, AM& ATL, AM& ATR, AM& ABL, AM& ABR, Int diagATL, bool lock )
{
    const Int 
    	minDim = std::min(A.Height(),A.Width()),
		H1 = std::min(diagATL,minDim),
		W1 = H1,
		H2 = A.Height() - H1, 
		W2 = A.Width() - W1;
	View__( ATL, A, 0,  0, H1, W1, lock );
	View__( ATR, A, 0, W1, H1, W2, lock );
	View__( ABL, A, H1, 0, H2, W1, lock );
	View__( ABR, A, H1, 0, H2, W2, lock );
}

template<typename Int> inline
void PartitionDownLeftDiagonal_( const AM& A, AM& ATL, AM& ATR, AM& ABL, AM& ABR, Int diagATL, bool lock )
{
	PushCallStack( "PartitionDownLeftDiagonal [Matrix]" );
    A.AssertNonnegative( diagATL, "size of top-left block" );
	A.AssertDataTypes( ATL );
	A.AssertDataTypes( ATR );		
	A.AssertDataTypes( ABL );
	A.AssertDataTypes( ABR );
	if ( !lock ) A.AssertUnlocked( AM::PartitionLock );
	PartitionDownLeftDiagonal__( ATL, ATR, ABL, ABR, diagATL, lock );
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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionDownDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL )
{
#ifndef RELEASE
    PushCallStack("PartitionDownDiagonal [DistMatrix]");
#endif
    PartitionDownLeftDiagonal
    ( A, ATL, ATR,
         ABL, ABR, diagATL );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionDownDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagATL )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionDownDiagonal [DistMatrix]");
#endif
    LockedPartitionDownLeftDiagonal
    ( A, ATL, ATR,
         ABL, ABR, diagATL );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionDownLeftDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL )
{
#ifndef RELEASE
    PushCallStack("PartitionDownLeftDiagonal [DistMatrix]");
    if( diagATL < 0 )
        throw std::logic_error("Top-left size must be non-negative");
#endif
    const Int minDim = std::min(A.Height(),A.Width());
    diagATL = std::min(diagATL,minDim);
    const Int heightABR = A.Height()-diagATL;
    const Int widthABR = A.Width()-diagATL;
    View( ATL, A, 0,       0,       diagATL,   diagATL  );
    View( ATR, A, 0,       diagATL, diagATL,   widthABR );
    View( ABL, A, diagATL, 0,       heightABR, diagATL  );
    View( ABR, A, diagATL, diagATL, heightABR, widthABR );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionDownLeftDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagATL )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionDownLeftDiagonal [DistMatrix]");
    if( diagATL < 0 )
        throw std::logic_error("Top-left size must be non-negative");
#endif
    const Int minDim = std::min(A.Height(),A.Width());
    diagATL = std::min(diagATL,minDim);
    const Int heightABR = A.Height()-diagATL;
    const Int widthABR = A.Width()-diagATL;
    LockedView( ATL, A, 0,       0,       diagATL,   diagATL  );
    LockedView( ATR, A, 0,       diagATL, diagATL,   widthABR );
    LockedView( ABL, A, diagATL, 0,       heightABR, diagATL  );
    LockedView( ABR, A, diagATL, diagATL, heightABR, widthABR );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// PartitionDownRightDiagonal
//

template<typename Int> inline
void PartitionDownRightDiagonal__( const AM& A, AM& ATL, AM& ATR, AM& ABL, AM& ABR, Int diagATL, bool lock )
{
	const Int 
		minDim = std::min(A.Width(),A.Height()),
		H2 = minDim - std::min(diagATL,minDim),
		W2 = H2,
		H1 = A.Height() - H2, 
		W1 = A.Width() - W2;
	View__( ATL, A, 0,  0, H1, W1, lock );
	View__( ATR, A, 0, W1, H1, W2, lock );
	View__( ABL, A, H1, 0, H2, W1, lock );
	View__( ABR, A, H1, 0, H2, W2, lock );
}

template<typename Int> inline
void PartitionDownRightDiagonal_( const AM& A, AM& ATL, AM& ATR, AM& ABL, AM& ABR, Int diagATL, bool lock )
{
	PushCallStack( "PartitionDownRightDiagonal [Matrix]" );
    A.AssertNonnegative( diagATL, "size of top-left block" );
	A.AssertDataTypes( ATL );
	A.AssertDataTypes( ATR );		
	A.AssertDataTypes( ABL );
	A.AssertDataTypes( ABR );
	if ( !lock ) A.AssertUnlocked( AM::PartitionLock );
	PartitionDownRightDiagonal__( ATL, ATR, ABL, ABR, diagATL, lock );
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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
PartitionDownRightDiagonal
( DM& A, DM& ATL, DM& ATR,
         DM& ABL, DM& ABR, Int diagATL )
{
#ifndef RELEASE
    PushCallStack("PartitionDownRightDiagonal [DistMatrix]");
    if( diagATL < 0 )
        throw std::logic_error("Top-left size must be non-negative");
#endif
    const Int minDim = std::min( A.Height(), A.Width() );
    diagATL = std::min(diagATL,minDim);
    const Int sizeABR = minDim-diagATL;
    const Int remHeight = A.Height()-sizeABR;
    const Int remWidth = A.Width()-sizeABR;
    View( ATL, A, 0,         0,        remHeight, remWidth );
    View( ATR, A, 0,         remWidth, remHeight, sizeABR  );
    View( ABL, A, remHeight, 0,        sizeABR,   remWidth );
    View( ABR, A, remHeight, remWidth, sizeABR,   sizeABR  );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T, Distribution U, Distribution V,typename Int>
inline void
LockedPartitionDownRightDiagonal
( const DM& A, DM& ATL, DM& ATR,
               DM& ABL, DM& ABR, Int diagATL )
{
#ifndef RELEASE
    PushCallStack("LockedPartitionDownRightDiagonal [DistMatrix]");
    if( diagATL < 0 )
        throw std::logic_error("Top-left size must be non-negative");
#endif
    const Int minDim = std::min( A.Height(), A.Width() );
    diagATL = std::min(diagATL,minDim);
    const Int sizeABR = minDim-diagATL;
    const Int remHeight = A.Height()-sizeABR;
    const Int remWidth = A.Width()-sizeABR;
    LockedView( ATL, A, 0,         0,        remHeight, remWidth );
    LockedView( ATR, A, 0,         remWidth, remHeight, sizeABR  );
    LockedView( ABL, A, remHeight, 0,        sizeABR,   remWidth );
    LockedView( ABR, A, remHeight, remWidth, sizeABR,   sizeABR  );
#ifndef RELEASE
    PopCallStack();
#endif
}

#undef RUNDERSCORE
#undef AM
#undef DM
#undef M

} // namespace elem

#endif // ifndef CORE_PARTITION_IMPL_HPP
