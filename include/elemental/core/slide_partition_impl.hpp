/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_SLIDEPARTITION_IMPL_HPP
#define CORE_SLIDEPARTITION_IMPL_HPP

namespace elem {

// To make our life easier. Undef'd at the bottom of the header
#define AM AutoMatrix<Int>
#define M  Matrix<T,Int>
#define DM DistMatrix<T,U,V,Int>

//
// Legend:
//      Function__: no consistency checks.
//      Function_: full consistency checking.
//      Function( AutoMatrix<Int>& ... )
//			Inlined to Function_, so fully checked in both Release and Debug.
//			This is for use with dynamically-typed languages.
//      Function( Matrix<T,Int>& ... ):
//			Inlined to Function__ in Release mode, for high performance.
//			Inlined to Function_  in Debug mode, for full consistency checking.
//

#ifdef RELEASE
#define RUNDERSCORE(x) x ## _
#else
#define RUNDERSCORE(x) x
#endif

//
// SlidePartitionUp
//

template<typename Int>
inline void
SlidePartitionUp__
( AM& AT, const AM& A0,
          const AM& A1,
  AM& AB, const AM& A2, bool lock )
{
    View__( AT, A0 );
    View2x1__( AB, A1, A2 );
}

template<typename Int>
inline void
SlidePartitionUp_
( AM& AT, const AM& A0,
          const AM& A1,
  AM& AB, const AM& A2, bool lock )
{
    PushCallStack("SlidePartitionUp [Matrix]");
    AT.AssertDataTypes( AB );
    AB.AssertDataTypes( A0 );
    AB.AssertDataTypes( A1 );
    AB.AssertDataTypes( A2 );
    if ( !lock ) {
    	A0.AssertUnlocked( AM::PartitionLock );
    	A1.AssertUnlocked( AM::PartitionLock );
    	A2.AssertUnlocked( AM::PartitionLock );
    }
    A0.AssertContiguous2x1( A1 );
    A1.AssertContiguous2x1( A2 );
    SlidePartitionUp__( AT, A0, A1, AB, A2, lock );
    PopCallStack();
}

template<typename Int>
inline void
SlidePartitionUp
( AM& AT, AM& A0,
          AM& A1,
  AM& AB, AM& A2 )
{ SlidePartitionUp_( AT, A0, A1, AB, A2, false ); }

template<typename T,typename Int>
inline void
SlidePartitionUp
( M& AT, M& A0,
         M& A1,
  M& AB, M& A2 )
{ RUNDERSCORE(SlidePartitionUp_)( AT, A0, A1, AB, A2, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionUp
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2 )
{
#ifndef RELEASE
    PushCallStack("SlidePartitionUp [DistMatrix]");
#endif
    View( AT, A0 );
    View2x1( AB, A1,
                 A2 );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
SlideLockedPartitionUp
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2 )
{ SlidePartitionUp_( AT, A0, A1, AB, A2, true ); }

template<typename T,typename Int>
inline void
SlideLockedPartitionUp
( const M& AT, M& A0,
               M& A1,
  const M& AB, M& A2 )
{ RUNDERSCORE(SlidePartitionUp_)( AT, A0, A1, AB, A2, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionUp
( DM& AT, const DM& A0,
          const DM& A1,
  DM& AB, const DM& A2 )
{
#ifndef RELEASE
    PushCallStack("SlideLockedPartitionUp [DistMatrix]");
#endif
    LockedView( AT, A0 );
    LockedView2x1( AB, A1,
                       A2 );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// SlidePartitionDown
//

template<typename Int>
inline void
SlidePartitionDown__
( AM& AT, const AM& A0,
          const AM& A1,
  AM& AB, const AM& A2, bool lock )
{
    View2x1__( AT, A0, A1, lock );
    View__( AB, A2, lock );
}

template<typename Int>
inline void
SlidePartitionDown_
( AM& AT, const AM& A0,
          const AM& A1,
  AM& AB, const AM& A2, bool lock )
{
    PushCallStack("SlidePartitionDown [Matrix]");
    AT.AssertDataTypes( AB );
    AB.AssertDataTypes( A0 );
    AB.AssertDataTypes( A1 );
    AB.AssertDataTypes( A2 );
    if ( !lock ) {
    	A0.AssertUnlocked( AM::PartitionLock );
    	A1.AssertUnlocked( AM::PartitionLock );
    	A2.AssertUnlocked( AM::PartitionLock );
    }
    A0.AssertContiguous2x1( A1 );
    A1.AssertContiguous2x1( A2 );
    SlidePartitionDown__( AT, A0, A1, AB, A2, lock );
    PopCallStack();
}

template<typename Int>
inline void
SlidePartitionDown
( AM& AT, AM& A0,
          AM& A1,
  AM& AB, AM& A2 )
{ SlidePartitionDown_( AT, A0, A1, AB, A2, false ); }

template<typename T,typename Int>
inline void
SlidePartitionDown
( M& AT, M& A0,
         M& A1,
  M& AB, M& A2 )
{ RUNDERSCORE(SlidePartitionDown_)( AT, A0, A1, AB, A2, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionDown
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2 )
{
#ifndef RELEASE
    PushCallStack("SlidePartitionDown [DistMatrix]");
#endif
    View2x1( AT, A0,
                 A1 );
    View( AB, A2 );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
SlideLockedPartitionDown
( AM& AT, const AM& A0,
          const AM& A1,
  AM& AB, const AM& A2 )
{ SlidePartitionDown_( AT, A0, A1, AB, A2, true ); }

template<typename T,typename Int>
inline void
SlideLockedPartitionDown
( M& AT, const M& A0,
         const M& A1,
  M& AB, const M& A2 )
{ RUNDERSCORE(SlidePartitionDown_)( AT, A0, A1, AB, A2, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionDown
( DM& AT, const DM& A0,
          const DM& A1,
  DM& AB, const DM& A2 )
{
#ifndef RELEASE
    PushCallStack("SlideLockedPartitionDown [DistMatrix]");
#endif
    LockedView2x1( AT, A0,
                       A1 );
    LockedView( AB, A2 );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// SlidePartitionLeft
//

template<typename Int>
inline void
SlidePartitionLeft__
( AM& AL, AM& AR,
  const AM& A0, const AM& A1, const AM& A2, bool lock )
{
    View__( AL, A0, lock );
    View1x2__( AR, A1, A2, lock );
}

template<typename Int>
inline void
SlidePartitionLeft_
( AM& AL, AM& AR,
  const AM& A0, const AM& A1, const AM& A2, bool lock )
{
    PushCallStack("SlidePartitionLeft [Matrix]");
    AL.AssertDataTypes( AR );
    AL.AssertDataTypes( A0 );
    AL.AssertDataTypes( A1 );
    AL.AssertDataTypes( A2 );
    if ( !lock ) {
    	A0.AssertUnlocked( AM::PartitionLock );
    	A1.AssertUnlocked( AM::PartitionLock );
    	A2.AssertUnlocked( AM::PartitionLock );
    }
    A0.AssertContiguous1x2( A1 );
    A1.AssertContiguous1x2( A2 );
    SlidePartitionLeft__( AL, AR, A0, A1, A2, lock );
    PopCallStack();
}

template <typename Int>
inline void
SlidePartitionLeft
( AM& AL, AM& AR,
  AM& A0, AM& A1, AM& A2 )
{ SlidePartitionLeft_( AL, AR, A0, A1, A2, false ); }

template <typename T,typename Int>
inline void
SlidePartitionLeft
( M& AL, M& AR,
  M& A0, M& A1, M& A2 )
{ RUNDERSCORE(SlidePartitionLeft_)( AL, AR, A0, A1, A2, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionLeft
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2 )
{
#ifndef RELEASE
    PushCallStack("SlidePartitionLeft [DistMatrix]");
#endif
    View( AL, A0 );
    View1x2( AR, A1, A2 );
#ifndef RELEASE
    PopCallStack();
#endif
}

template <typename Int>
inline void
SlideLockedPartitionLeft
( AM& AL, AM& AR,
  const AM& A0, const AM& A1, const AM& A2 )
{ SlidePartitionLeft_( AL, AR, A0, A1, A2, true ); }

template <typename T,typename Int>
inline void
SlideLockedPartitionLeft
( M& AL, M& AR,
  const M& A0, const M& A1, const M& A2 )
{ RUNDERSCORE(SlidePartitionLeft_)( AL, AR, A0, A1, A2, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionLeft
( DM& AL, DM& AR,
  const DM& A0, const DM& A1, const DM& A2 )
{
#ifndef RELEASE
    PushCallStack("SlideLockedPartitionLeft [DistMatrix]");
#endif
    LockedView( AL, A0 );
    LockedView1x2( AR, A1, A2 );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// SlidePartitionRight
//

template<typename Int>
inline void
SlidePartitionRight__
( AM& AL, AM& AR,
  const AM& A0, const AM& A1, const AM& A2, bool lock )
{
    View1x2__( AL, A0, A1, lock );
    View__( AR, A2, lock );
}

template<typename Int>
inline void
SlidePartitionRight_
( AM& AL, AM& AR,
  const AM& A0, const AM& A1, const AM& A2, bool lock )
{
    PushCallStack("SlidePartitionRight [Matrix]");
    AL.AssertDataTypes( AR );
    AL.AssertDataTypes( A0 );
    AL.AssertDataTypes( A1 );
    AL.AssertDataTypes( A2 );
    if ( !lock ) {
    	A0.AssertUnlocked( AM::PartitionLock );
    	A1.AssertUnlocked( AM::PartitionLock );
    	A2.AssertUnlocked( AM::PartitionLock );
    }
    A0.AssertContiguous1x2( A1 );
    A1.AssertContiguous1x2( A2 );
    SlidePartitionRight__( AL, AR, A0, A1, A2, lock );
    PopCallStack();
}

template <typename Int>
inline void
SlidePartitionRight
( AM& AL, AM& AR,
  AM& A0, AM& A1, AM& A2 )
{ SlidePartitionRight_( AL, AR, A0, A1, A2, false ); }

template <typename T,typename Int>
inline void
SlidePartitionRight
( M& AL, M& AR,
  M& A0, M& A1, M& A2 )
{ RUNDERSCORE(SlidePartitionRight_)( AL, AR, A0, A1, A2, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionRight
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2 )
{
#ifndef RELEASE
    PushCallStack("SlidePartitionRight [DistMatrix]");
#endif
    View1x2( AL, A0, A1 );
    View( AR, A2 );
#ifndef RELEASE
    PopCallStack();
#endif
}

template <typename Int>
inline void
SlideLockedPartitionRight
( AM& AL, AM& AR,
  const AM& A0, const AM& A1, const AM& A2 )
{ SlidePartitionRight_( AL, AR, A0, A1, A2, true ); }

template <typename T,typename Int>
inline void
SlideLockedPartitionRight
( M& AL, M& AR,
  const M& A0, const M& A1, const M& A2 )
{ RUNDERSCORE(SlidePartitionRight_)( AL, AR, A0, A1, A2, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionRight
( DM& AL, DM& AR,
  const DM& A0, const DM& A1, const DM& A2 )
{
#ifndef RELEASE
    PushCallStack("SlideLockedPartitionRight [DistMatrix]");
#endif
    LockedView1x2( AL, A0, A1 );
    LockedView( AR, A2 );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// SlidePartitionUpDiagonal
//

template <typename Int>
inline void
SlidePartitionUpDiagonal__
( AM& ATL, AM& ATR, const AM& A00, const AM& A01, const AM& A02,
                    const AM& A10, const AM& A11, const AM& A12,
  AM& ABL, AM& ABR, const AM& A20, const AM& A21, const AM& A22, bool lock )
{
    View__( ATL, A00, lock );
    View1x2__( ATR, A01, A02, lock );
    View2x1__( ABL, A10,
                    A20, lock );
    View2x2__( ABR, A11, A12,
                    A21, A22, lock );
}

template<typename Int>
inline void
SlidePartitionUpDiagonal_
( AM& ATL, AM& ATR, const AM& A00, const AM& A01, const AM& A02,
                    const AM& A10, const AM& A11, const AM& A12,
  AM& ABL, AM& ABR, const AM& A20, const AM& A21, const AM& A22, bool lock )
{
    PushCallStack("SlidePartitionUpDiagonal [Matrix]");
    ATL.AssertDataTypes( ATR );
    ATL.AssertDataTypes( A00 );
    ATL.AssertDataTypes( A01 );
    ATL.AssertDataTypes( A02 );
    ATL.AssertDataTypes( A10 );
    ATL.AssertDataTypes( A11 );
    ATL.AssertDataTypes( A12 );
    ATL.AssertDataTypes( ABL );
    ATL.AssertDataTypes( ABR );
    ATL.AssertDataTypes( A20 );
    ATL.AssertDataTypes( A21 );
    ATL.AssertDataTypes( A22 );
    if ( !lock ) {
    	A00.AssertUnlocked( AM::PartitionLock );
    	A01.AssertUnlocked( AM::PartitionLock );
    	A02.AssertUnlocked( AM::PartitionLock );
    	A10.AssertUnlocked( AM::PartitionLock );
    	A11.AssertUnlocked( AM::PartitionLock );
    	A12.AssertUnlocked( AM::PartitionLock );
    	A20.AssertUnlocked( AM::PartitionLock );
    	A21.AssertUnlocked( AM::PartitionLock );
    	A22.AssertUnlocked( AM::PartitionLock );
    }
    A00.AssertContiguous2x2( A01, A02, A10, A11, A12, A20, A21, A22 );
    PopCallStack();
}

template<typename Int>
inline void
SlidePartitionUpDiagonal
( AM& ATL, AM& ATR, AM& A00, AM& A01, AM& A02,
                    AM& A10, AM& A11, AM& A12,
  AM& ABL, AM& ABR, AM& A20, AM& A21, AM& A22 )
{ SlidePartitionUpDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, false ); }

template<typename T,typename Int>
inline void
SlidePartitionUpDiagonal
( M& ATL, M& ATR, M& A00, M& A01, M& A02,
                  M& A10, M& A11, M& A12,
  M& ABL, M& ABR, M& A20, M& A21, M& A22 )
{ RUNDERSCORE(SlidePartitionUpDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionUpDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22 )
{
#ifndef RELEASE
    PushCallStack("SlidePartitionUpDiagonal [DistMatrix]");
#endif
    View( ATL, A00 );
    View1x2( ATR, A01, A02 );
    View2x1( ABL, A10,
                  A20 );
    View2x2( ABR, A11, A12,
                  A21, A22 );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
SlideLockedPartitionUpDiagonal
( AM& ATL, AM& ATR, const AM& A00, const AM& A01, const AM& A02,
                    const AM& A10, const AM& A11, const AM& A12,
  AM& ABL, AM& ABR, const AM& A20, const AM& A21, const AM& A22 )
{ SlidePartitionUpDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, true ); }

template<typename T,typename Int>
inline void
SlideLockedPartitionUpDiagonal
( M& ATL, M& ATR, const M& A00, const M& A01, const M& A02,
                  const M& A10, const M& A11, const M& A12,
  M& ABL, M& ABR, const M& A20, const M& A21, const M& A22 )
{ RUNDERSCORE(SlidePartitionUpDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionUpDiagonal
( DM& ATL, DM& ATR, const DM& A00, const DM& A01, const DM& A02,
                    const DM& A10, const DM& A11, const DM& A12,
  DM& ABL, DM& ABR, const DM& A20, const DM& A21, const DM& A22 )
{
#ifndef RELEASE
    PushCallStack("SlideLockedPartitionUpDiagonal [DistMatrix]");
#endif
    LockedView( ATL, A00 );
    LockedView1x2( ATR, A01, A02 );
    LockedView2x1( ABL, A10,
                        A20 );
    LockedView2x2( ABR, A11, A12,
                        A21, A22 );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// SlidePartitionDownDiagonal
//

template <typename Int>
inline void
SlidePartitionDownDiagonal__
( AM& ATL, AM& ATR, const AM& A00, const AM& A01, const AM& A02,
                    const AM& A10, const AM& A11, const AM& A12,
  AM& ABL, AM& ABR, const AM& A20, const AM& A21, const AM& A22, bool lock )
{
    View2x2__( ATL, A00, A01,
                    A10, A11, lock );
    View2x1__( ATR, A02,
                    A12, lock );
    View1x2__( ABL, A20, A21, lock );
    View__( ABR, A22, lock );
}

template<typename Int>
inline void
SlidePartitionDownDiagonal_
( AM& ATL, AM& ATR, const AM& A00, const AM& A01, const AM& A02,
                    const AM& A10, const AM& A11, const AM& A12,
  AM& ABL, AM& ABR, const AM& A20, const AM& A21, const AM& A22, bool lock )
{
    PushCallStack("SlidePartitionDownDiagonal [Matrix]");
    ATL.AssertDataTypes( ATR );
    ATL.AssertDataTypes( A00 );
    ATL.AssertDataTypes( A01 );
    ATL.AssertDataTypes( A02 );
    ATL.AssertDataTypes( A10 );
    ATL.AssertDataTypes( A11 );
    ATL.AssertDataTypes( A12 );
    ATL.AssertDataTypes( ABL );
    ATL.AssertDataTypes( ABR );
    ATL.AssertDataTypes( A20 );
    ATL.AssertDataTypes( A21 );
    ATL.AssertDataTypes( A22 );
    if ( !lock ) {
    	A00.AssertUnlocked( AM::PartitionLock );
    	A01.AssertUnlocked( AM::PartitionLock );
    	A02.AssertUnlocked( AM::PartitionLock );
    	A10.AssertUnlocked( AM::PartitionLock );
    	A11.AssertUnlocked( AM::PartitionLock );
    	A12.AssertUnlocked( AM::PartitionLock );
    	A20.AssertUnlocked( AM::PartitionLock );
    	A21.AssertUnlocked( AM::PartitionLock );
    	A22.AssertUnlocked( AM::PartitionLock );
    }
    A00.AssertContiguous2x2( A01, A02, A10, A11, A12, A20, A21, A22 );
    PopCallStack();
}

template<typename Int>
inline void
SlidePartitionDownDiagonal
( AM& ATL, AM& ATR, AM& A00, AM& A01, AM& A02,
                    AM& A10, AM& A11, AM& A12,
  AM& ABL, AM& ABR, AM& A20, AM& A21, AM& A22 )
{ SlidePartitionDownDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, false ); }

template<typename T,typename Int>
inline void
SlidePartitionDownDiagonal
( M& ATL, M& ATR, M& A00, M& A01, M& A02,
                  M& A10, M& A11, M& A12,
  M& ABL, M& ABR, M& A20, M& A21, M& A22 )
{ RUNDERSCORE(SlidePartitionDownDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionDownDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22 )
{
#ifndef RELEASE
    PushCallStack("SlidePartitionDownDiagonal [DistMatrix]");
#endif
    View2x2( ATL, A00, A01,
                  A10, A11 );
    View2x1( ATR, A02,
                  A12 );
    View1x2( ABL, A20, A21 );
    View( ABR, A22 );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Int>
inline void
SlideLockedPartitionDownDiagonal
( AM& ATL, AM& ATR, const AM& A00, const AM& A01, const AM& A02,
                    const AM& A10, const AM& A11, const AM& A12,
  AM& ABL, AM& ABR, const AM& A20, const AM& A21, const AM& A22 )
{ SlidePartitionDownDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, true ); }

template<typename T,typename Int>
inline void
SlideLockedPartitionDownDiagonal
( M& ATL, M& ATR, const M& A00, const M& A01, const M& A02,
                  const M& A10, const M& A11, const M& A12,
  M& ABL, M& ABR, const M& A20, const M& A21, const M& A22 )
{ RUNDERSCORE(SlidePartitionDownDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionDownDiagonal
( DM& ATL, DM& ATR, const DM& A00, const DM& A01, const DM& A02,
                    const DM& A10, const DM& A11, const DM& A12,
  DM& ABL, DM& ABR, const DM& A20, const DM& A21, const DM& A22 )
{
#ifndef RELEASE
    PushCallStack("SlideLockedPartitionDownDiagonal [DistMatrix]");
#endif
    LockedView2x2( ATL, A00, A01,
                        A10, A11 );
    LockedView2x1( ATR, A02,
                        A12 );
    LockedView1x2( ABL, A20, A21 );
    LockedView( ABR, A22 );
#ifndef RELEASE
    PopCallStack();
#endif
}

#undef RUNDERSCORE
#undef AM
#undef DM
#undef M

} // namespace elem

#endif // ifndef CORE_SLIDEPARTITION_IMPL_HPP
