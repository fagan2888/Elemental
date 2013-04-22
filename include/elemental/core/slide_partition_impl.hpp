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
// SlidePartitionUp
//

template<typename Q>
inline void
SlidePartitionUp__
( Q& AT, const Q& A0,
         const Q& A1,
  Q& AB, const Q& A2, bool lock )
{
    View__(    AT, A0, lock );
    View2x1__( AB, A1, 
                   A2, lock );
}

template<typename Q>
inline void
SlidePartitionUp_
( Q& AT, const Q& A0,
         const Q& A1,
  Q& AB, const Q& A2, bool lock )
{
    PushCallStack("SlidePartitionUp");
    AssertDataTypes( 5, &AT, &AB, &A0, &A1, &A2 );
    if ( !lock )
    	AssertUnlocked( 3, &A0, &A1, &A2 );
    AssertContiguous3x1( A0, A1, A2 );
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

template<typename Int>
inline void
SlidePartitionUp
( ADM& AT, ADM& A0,
          ADM& A1,
  ADM& AB, ADM& A2 )
{ SlidePartitionUp_( AT, A0, A1, AB, A2, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionUp
( DM& AT, DM& A0,
         DM& A1,
  DM& AB, DM& A2 )
{ RUNDERSCORE(SlidePartitionUp_)( AT, A0, A1, AB, A2, false ); }

template<typename Int>
inline void
SlideLockedPartitionUp
( AM& AT, const AM& A0,
          const AM& A1,
  AM& AB, const AM& A2 )
{ SlidePartitionUp_( AT, A0, A1, AB, A2, true ); }

template<typename T,typename Int>
inline void
SlideLockedPartitionUp
( M& AT, const M& A0,
         const M& A1,
  M& AB, const M& A2 )
{ RUNDERSCORE(SlidePartitionUp_)( AT, A0, A1, AB, A2, true ); }

template<typename Int>
inline void
SlideLockedPartitionUp
( ADM& AT, const ADM& A0,
           const ADM& A1,
  ADM& AB, const ADM& A2 )
{ SlidePartitionUp_( AT, A0, A1, AB, A2, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionUp
( DM& AT, const DM& A0,
          const DM& A1,
  DM& AB, const DM& A2 )
{ RUNDERSCORE(SlidePartitionUp_)( AT, A0, A1, AB, A2, true ); }

//
// SlidePartitionDown
//

template<typename Q>
inline void
SlidePartitionDown__
( Q& AT, const Q& A0,
         const Q& A1,
  Q& AB, const Q& A2, bool lock )
{
    View2x1__( AT, A0, 
                   A1, lock );
    View__(    AB, A2, lock );
}

template<typename Q>
inline void
SlidePartitionDown_
( Q& AT, const Q& A0,
         const Q& A1,
  Q& AB, const Q& A2, bool lock )
{
    PushCallStack("SlidePartitionDown");
    AssertDataTypes( 5, &AT, &AB, &A0, &A1, &A2 );
    if ( !lock )
    	AssertUnlocked( 3, &A0, &A1, &A2 );
    AssertContiguous3x1( A0, A1, A2 );
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

template<typename Int>
inline void
SlidePartitionDown
( ADM& AT, ADM& A0,
          ADM& A1,
  ADM& AB, ADM& A2 )
{ SlidePartitionDown_( AT, A0, A1, AB, A2, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionDown
( DM& AT, DM& A0,
         DM& A1,
  DM& AB, DM& A2 )
{ RUNDERSCORE(SlidePartitionDown_)( AT, A0, A1, AB, A2, false ); }

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

template<typename Int>
inline void
SlideLockedPartitionDown
( ADM& AT, const ADM& A0,
           const ADM& A1,
  ADM& AB, const ADM& A2 )
{ SlidePartitionDown_( AT, A0, A1, AB, A2, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionDown
( DM& AT, const DM& A0,
          const DM& A1,
  DM& AB, const DM& A2 )
{ RUNDERSCORE(SlidePartitionDown_)( AT, A0, A1, AB, A2, true ); }

//
// SlidePartitionLeft
//

template<typename Q>
inline void
SlidePartitionLeft__
( Q& AL, Q& AR,
  const Q& A0, const Q& A1, const Q& A2, bool lock )
{
    View__(    AL, A0,         lock );
    View1x2__( AR,     A1, A2, lock );
}

template<typename Q>
inline void
SlidePartitionLeft_
( Q& AL, Q& AR,
  const Q& A0, const Q& A1, const Q& A2, bool lock )
{
    PushCallStack("SlidePartitionLeft");
    AssertDataTypes( 5, &AL, &AR, &A0, &A1, &A2 );
    if ( !lock )
    	AssertUnlocked( 3, &A0, &A1, &A2 );
    AssertContiguous1x3( A0, A1, A2 );
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

template <typename Int>
inline void
SlidePartitionLeft
( ADM& AL, ADM& AR,
  ADM& A0, ADM& A1, ADM& A2 )
{ SlidePartitionLeft_( AL, AR, A0, A1, A2, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionLeft
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2 )
{ RUNDERSCORE(SlidePartitionLeft_)( AL, AR, A0, A1, A2, false ); }

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

template <typename Int>
inline void
SlideLockedPartitionLeft
( ADM& AL, ADM& AR,
  const ADM& A0, const ADM& A1, const ADM& A2 )
{ SlidePartitionLeft_( AL, AR, A0, A1, A2, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionLeft
( DM& AL, DM& AR,
  const DM& A0, const DM& A1, const DM& A2 )
{ RUNDERSCORE(SlidePartitionLeft_)( AL, AR, A0, A1, A2, true ); }

//
// SlidePartitionRight
//

template<typename Q>
inline void
SlidePartitionRight__
( Q& AL, Q& AR,
  const Q& A0, const Q& A1, const Q& A2, bool lock )
{
    View1x2__( AL, A0, A1,     lock );
    View__(    AR,         A2, lock );
}

template<typename Q>
inline void
SlidePartitionRight_
( Q& AL, Q& AR,
  const Q& A0, const Q& A1, const Q& A2, bool lock )
{
    PushCallStack("SlidePartitionRight");
    AssertDataTypes( 5, &AL, &AR, &A0, &A1, &A2 );
    if ( !lock )
    	AssertUnlocked( 3, &A0, &A1, &A2 );
    AssertContiguous1x3( A0, A1, A2 );
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

template <typename Int>
inline void
SlidePartitionRight
( ADM& AL, ADM& AR,
  ADM& A0, ADM& A1, ADM& A2 )
{ SlidePartitionRight_( AL, AR, A0, A1, A2, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionRight
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2 )
{ RUNDERSCORE(SlidePartitionRight_)( AL, AR, A0, A1, A2, false ); }

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

template <typename Int>
inline void
SlideLockedPartitionRight
( ADM& AL, ADM& AR,
  const ADM& A0, const ADM& A1, const ADM& A2 )
{ SlidePartitionRight_( AL, AR, A0, A1, A2, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionRight
( DM& AL, DM& AR,
  const DM& A0, const DM& A1, const DM& A2 )
{ RUNDERSCORE(SlidePartitionRight_)( AL, AR, A0, A1, A2, true ); }

//
// SlidePartitionUpDiagonal
//

template <typename Q>
inline void
SlidePartitionUpDiagonal__
( Q& ATL, Q& ATR, const Q& A00, const Q& A01, const Q& A02,
                  const Q& A10, const Q& A11, const Q& A12,
  Q& ABL, Q& ABR, const Q& A20, const Q& A21, const Q& A22, bool lock )
{
    View__(    ATL, A00,           lock );
    View1x2__( ATR,      A01, A02, lock );
    View2x1__( ABL, A10,
                    A20, lock );
    View2x2__( ABR,      A11, A12,
                         A21, A22, lock );
}

template<typename Q>
inline void
SlidePartitionUpDiagonal_
( Q& ATL, Q& ATR, const Q& A00, const Q& A01, const Q& A02,
                  const Q& A10, const Q& A11, const Q& A12,
  Q& ABL, Q& ABR, const Q& A20, const Q& A21, const Q& A22, bool lock )
{
    PushCallStack("SlidePartitionUpDiagonal");
    AssertDataTypes( 13, &ATL, &ATR, &A00, &A01, &A02, &A10, &A11, &A12, &ABL, &ABR, &A20, &A21, &A22 );
    if ( !lock )
    	AssertUnlocked( 9, &A00, &A01, &A02, &A10, &A11, &A12, &A20, &A21, &A22 );
    AssertContiguous3x3( A00, A01, A02, A10, A11, A12, A20, A21, A22 );
	SlidePartitionUpDiagonal__( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, lock );
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

template<typename Int>
inline void
SlidePartitionUpDiagonal
( ADM& ATL, ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                      ADM& A10, ADM& A11, ADM& A12,
  ADM& ABL, ADM& ABR, ADM& A20, ADM& A21, ADM& A22 )
{ SlidePartitionUpDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionUpDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22 )
{ RUNDERSCORE(SlidePartitionUpDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, false ); }

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

template<typename Int>
inline void
SlideLockedPartitionUpDiagonal
( ADM& ATL, ADM& ATR, const ADM& A00, const ADM& A01, const ADM& A02,
                      const ADM& A10, const ADM& A11, const ADM& A12,
  ADM& ABL, ADM& ABR, const ADM& A20, const ADM& A21, const ADM& A22 )
{ SlidePartitionUpDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionUpDiagonal
( DM& ATL, DM& ATR, const DM& A00, const DM& A01, const DM& A02,
                    const DM& A10, const DM& A11, const DM& A12,
  DM& ABL, DM& ABR, const DM& A20, const DM& A21, const DM& A22 )
{ RUNDERSCORE(SlidePartitionUpDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, true ); }

//
// SlidePartitionDownDiagonal
//

template <typename Q>
inline void
SlidePartitionDownDiagonal__
( Q& ATL, Q& ATR, const Q& A00, const Q& A01, const Q& A02,
                  const Q& A10, const Q& A11, const Q& A12,
  Q& ABL, Q& ABR, const Q& A20, const Q& A21, const Q& A22, bool lock )
{
    View2x2__( ATL, A00, A01,
                    A10, A11,      lock );
    View2x1__( ATR,           A02,
                              A12, lock );
    View1x2__( ABL, A20, A21,      lock );
    View__(    ABR,           A22, lock );
}

template<typename Q>
inline void
SlidePartitionDownDiagonal_
( Q& ATL, Q& ATR, const Q& A00, const Q& A01, const Q& A02,
                  const Q& A10, const Q& A11, const Q& A12,
  Q& ABL, Q& ABR, const Q& A20, const Q& A21, const Q& A22, bool lock )
{
    PushCallStack("SlidePartitionDownDiagonal");
    AssertDataTypes( 13, &ATL, &ATR, &A00, &A01, &A02, &A10, &A11, &A12, &ABL, &ABR, &A20, &A21, &A22 );
    if ( !lock )
    	AssertUnlocked( 9, &A00, &A01, &A02, &A10, &A11, &A12, &A20, &A21, &A22 );
    AssertContiguous3x3( A00, A01, A02, A10, A11, A12, A20, A21, A22 );
    SlidePartitionDownDiagonal__( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, lock );
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

template<typename Int>
inline void
SlidePartitionDownDiagonal
( ADM& ATL, ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                      ADM& A10, ADM& A11, ADM& A12,
  ADM& ABL, ADM& ABR, ADM& A20, ADM& A21, ADM& A22 )
{ SlidePartitionDownDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlidePartitionDownDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22 )
{ RUNDERSCORE(SlidePartitionDownDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, false ); }

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

template<typename Int>
inline void
SlideLockedPartitionDownDiagonal
( ADM& ATL, ADM& ATR, const ADM& A00, const ADM& A01, const ADM& A02,
                      const ADM& A10, const ADM& A11, const ADM& A12,
  ADM& ABL, ADM& ABR, const ADM& A20, const ADM& A21, const ADM& A22 )
{ SlidePartitionDownDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
SlideLockedPartitionDownDiagonal
( DM& ATL, DM& ATR, const DM& A00, const DM& A01, const DM& A02,
                    const DM& A10, const DM& A11, const DM& A12,
  DM& ABL, DM& ABR, const DM& A20, const DM& A21, const DM& A22 )
{ RUNDERSCORE(SlidePartitionDownDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, true ); }

#undef AM
#undef DM
#undef M
#undef ADM

} // namespace elem

#endif // ifndef CORE_SLIDEPARTITION_IMPL_HPP
