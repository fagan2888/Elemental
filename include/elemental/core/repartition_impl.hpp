/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_REPARTITION_IMPL_HPP
#define CORE_REPARTITION_IMPL_HPP

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
// RepartitionUp
//

template<typename Q,typename Int> inline
void RepartitionUp__
( const Q& AT, Q& A0,
               Q& A1,
  const Q& AB, Q& A2, Int A1Height, bool lock )
{
    A1Height = std::min(A1Height,AT.Height());
    const Int offset = AT.Height()-A1Height; 
    View__( A0, AT, 0,      0, offset,   AT.Width(), lock );
    View__( A1, AT, offset, 0, A1Height, AT.Width(), lock );
    View__( A2, AB, lock );
}

template<typename Q,typename Int> inline
void RepartitionUp_
( const Q& AT, Q& A0,
               Q& A1,
  const Q& AB, Q& A2, Int A1Height, bool lock )
{
    PushCallStack("RepartitionUp");
    AssertNonnegative( A1Height, "height of A1 block" );
    AssertDataTypes( 5, &AT, &AB, &A0, &A1, &A2 );
    if ( !lock )
    	AssertUnlocked( 2, &AT, &AB );
    AssertContiguous2x1( AT, AB );
	RepartitionUp__( AT, A0, A1, AB, A2, A1Height, lock );
    PopCallStack();
}

template<typename Int>
inline void
RepartitionUp
( AM& AT, AM& A0,
          AM& A1,
  AM& AB, AM& A2, Int A1Height )
{ RepartitionUp_( AT, A0, A1, AB, A2, A1Height, false ); }

template<typename T,typename Int>
inline void
RepartitionUp
( M& AT, M& A0,
         M& A1,
  M& AB, M& A2, Int A1Height )
{ RUNDERSCORE(RepartitionUp_)( AT, A0, A1, AB, A2, A1Height, false ); }

template<typename Int>
inline void
RepartitionUp
( ADM& AT, ADM& A0,
           ADM& A1,
  ADM& AB, ADM& A2, Int A1Height )
{ RepartitionUp_( AT, A0, A1, AB, A2, A1Height, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionUp
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2, Int A1Height )
{ RUNDERSCORE(RepartitionUp_)( AT, A0, A1, AB, A2, A1Height, false ); }

template<typename Int>
inline void
LockedRepartitionUp
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2, Int A1Height )
{ RepartitionUp_( AT, A0, A1, AB, A2, A1Height, true ); }

template<typename T,typename Int>
inline void
LockedRepartitionUp
( const M& AT, M& A0,
               M& A1,
  const M& AB, M& A2, Int A1Height )
{ RUNDERSCORE(RepartitionUp_)( AT, A0, A1, AB, A2, A1Height, true ); }

template<typename Int>
inline void
LockedRepartitionUp
( const ADM& AT, ADM& A0,
                 ADM& A1,
  const ADM& AB, ADM& A2, Int A1Height )
{ RepartitionUp_( AT, A0, A1, AB, A2, A1Height, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionUp
( const DM& AT, DM& A0,
                DM& A1,
  const DM& AB, DM& A2, Int A1Height )
{ RUNDERSCORE(RepartitionUp_)( AT, A0, A1, AB, A2, A1Height, true ); }

//
// RepartitionDown
//

template<typename Q,typename Int>
inline void RepartitionDown__
( const Q& AT, Q& A0,
               Q& A1,
  const Q& AB, Q& A2, Int A1Height, bool lock )
{
    A1Height = std::min(A1Height,AB.Height());
    const Int offset = AB.Height()-A1Height; 
    View__( A0, AT, lock );
    View__( A1, AB, 0,        0, A1Height, AB.Width(), lock );
    View__( A2, AB, A1Height, 0, offset,   AB.Width(), lock );
}
  
template<typename Q,typename Int>
inline void RepartitionDown_
( const Q& AT, Q& A0,
               Q& A1,
  const Q& AB, Q& A2, Int A1Height, bool lock )
{
    PushCallStack("RepartitionDown");
    AssertNonnegative( A1Height, "height of A1 block" );
    AssertDataTypes( 5, &AT, &AB, &A0, &A1, &A2 );
    if ( !lock )
    	AssertUnlocked( 2, &AT, &AB );
    AssertContiguous2x1( AT, AB );
    RepartitionDown__( AT, A0, A1, AB, A2, A1Height, lock );
    PopCallStack();
}

template<typename Int>
inline void
RepartitionDown
( AM& AT, AM& A0,
          AM& A1,
  AM& AB, AM& A2, Int A1Height )
{ RepartitionDown_( AT, A0, A1, AB, A2, A1Height, false ); }

template<typename T,typename Int>
inline void
RepartitionDown
( M& AT, M& A0,
         M& A1,
  M& AB, M& A2, Int A1Height )
{ RUNDERSCORE(RepartitionDown_)( AT, A0, A1, AB, A2, A1Height, false ); }

template<typename Int>
inline void
RepartitionDown
( ADM& AT, ADM& A0,
           ADM& A1,
  ADM& AB, ADM& A2, Int A1Height )
{ RepartitionDown_( AT, A0, A1, AB, A2, A1Height, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionDown
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2, Int A1Height )
{ RUNDERSCORE(RepartitionDown_)( AT, A0, A1, AB, A2, A1Height, false ); }

template<typename Int>
inline void
LockedRepartitionDown
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2, Int A1Height )
{ RepartitionDown_( AT, A0, A1, AB, A2, A1Height, true ); }

template<typename T,typename Int>
inline void
LockedRepartitionDown
( const M& AT, M& A0,
               M& A1,
  const M& AB, M& A2, Int A1Height )
{ RUNDERSCORE(RepartitionDown_)( AT, A0, A1, AB, A2, A1Height, true ); }

template<typename Int>
inline void
LockedRepartitionDown
( const ADM& AT, ADM& A0,
                 ADM& A1,
  const ADM& AB, ADM& A2, Int A1Height )
{ RepartitionDown_( AT, A0, A1, AB, A2, A1Height, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionDown
( const DM& AT, DM& A0,
                DM& A1,
  const DM& AB, DM& A2, Int A1Height )
{ RUNDERSCORE(RepartitionDown_)( AT, A0, A1, AB, A2, A1Height, true ); }

//
// RepartitionLeft
//

template<typename Q,typename Int>
inline void
RepartitionLeft__
( const Q& AL, const Q& AR,
  Q& A0, Q& A1, Q& A2, Int A1Width, bool lock )
{
    A1Width = std::min(A1Width,AL.Width());
    const Int offset = AL.Width()-A1Width;
    View__( A0, AL, 0, 0,      AL.Height(), offset , lock );
    View__( A1, AL, 0, offset, AL.Height(), A1Width, lock );
    View__( A2, AR, lock );
}

template<typename Q,typename Int>
inline void
RepartitionLeft_
( const Q& AL, const Q& AR,
  Q& A0, Q& A1, Q& A2, Int A1Width, bool lock )
{
    PushCallStack("RepartitionLeft");
    AssertNonnegative( A1Width, "width of A1 block" );
    AssertDataTypes( 5, &AL, &AR, &A0, &A1, &A2 );
    if ( !lock )
    	AssertUnlocked( 2, &AL, &AR );
    AssertContiguous1x2( AL, AR );
    RepartitionLeft__( AL, AR, A0, A1, A2, A1Width, lock );
    PopCallStack();
}

template<typename Int>
inline void
RepartitionLeft
( AM& AL, AM& AR,
  AM& A0, AM& A1, AM& A2, Int A1Width )
{ RepartitionLeft_( AL, AR, A0, A1, A2, A1Width, false ); }

template<typename T,typename Int>
inline void
RepartitionLeft
( M& AL, M& AR,
  M& A0, M& A1, M& A2, Int A1Width )
{ RUNDERSCORE(RepartitionLeft_)( AL, AR, A0, A1, A2, A1Width, false ); }

template<typename Int>
inline void
RepartitionLeft
( ADM& AL, ADM& AR,
  ADM& A0, ADM& A1, ADM& A2, Int A1Width )
{ RepartitionLeft_( AL, AR, A0, A1, A2, A1Width, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionLeft
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2, Int A1Width )
{ RUNDERSCORE(RepartitionLeft_)( AL, AR, A0, A1, A2, A1Width, false ); }

template<typename Int>
inline void
LockedRepartitionLeft
( const AM& AL, const AM& AR,
  AM& A0, AM& A1, AM& A2, Int A1Width )
{ RepartitionLeft_( AL, AR, A0, A1, A2, A1Width, true ); }

template<typename T,typename Int>
inline void
LockedRepartitionLeft
( const M& AL, const M& AR,
  M& A0, M& A1, M& A2, Int A1Width )
{ RUNDERSCORE(RepartitionLeft_)( AL, AR, A0, A1, A2, A1Width, true ); }

template<typename Int>
inline void
LockedRepartitionLeft
( const ADM& AL, const ADM& AR,
  ADM& A0, ADM& A1, ADM& A2, Int A1Width )
{ RepartitionLeft_( AL, AR, A0, A1, A2, A1Width, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionLeft
( const DM& AL, const DM& AR,
  DM& A0, DM& A1, DM& A2, Int A1Width )
{ RUNDERSCORE(RepartitionLeft_)( AL, AR, A0, A1, A2, A1Width, true ); }


//
// RepartitionRight
//

template<typename Q,typename Int>
inline void
RepartitionRight__
( const Q& AL, const Q& AR,
  Q& A0, Q& A1, Q& A2, Int A1Width, bool lock )
{
    A1Width = std::min(A1Width,AR.Width());
    const Int offset = AR.Width()-A1Width;
    View__( A0, AL, lock );
    View__( A1, AR, 0, 0,       AR.Height(), A1Width, lock );
    View__( A2, AR, 0, A1Width, AR.Height(), offset , lock );
}

template<typename Q,typename Int>
inline void
RepartitionRight_
( const Q& AL, const Q& AR,
  Q& A0, Q& A1, Q& A2, Int A1Width, bool lock )
{
    PushCallStack("RepartitionRight");
    AssertNonnegative( A1Width, "width of A1 block" );
    AssertDataTypes( 5, &AL, &AR, &A0, &A1, &A2 );
    if ( !lock )
    	AssertUnlocked( 2, &AL, &AR );
    AssertContiguous1x2( AL, AR );
    RepartitionRight__( AL, AR, A0, A1, A2, A1Width, lock );
    PopCallStack();
}

template<typename Int>
inline void
RepartitionRight
( AM& AL, AM& AR,
  AM& A0, AM& A1, AM& A2, Int A1Width )
{ RepartitionRight_( AL, AR, A0, A1, A2, A1Width, false ); }

template<typename T,typename Int>
inline void
RepartitionRight
( M& AL, M& AR,
  M& A0, M& A1, M& A2, Int A1Width )
{ RUNDERSCORE(RepartitionRight_)( AL, AR, A0, A1, A2, A1Width, false ); }

template<typename Int>
inline void
RepartitionRight
( ADM& AL, ADM& AR,
  ADM& A0, ADM& A1, ADM& A2, Int A1Width )
{ RepartitionRight_( AL, AR, A0, A1, A2, A1Width, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionRight
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2, Int A1Width )
{ RUNDERSCORE(RepartitionRight_)( AL, AR, A0, A1, A2, A1Width, false ); }

template<typename Int>
inline void
LockedRepartitionRight
( const AM& AL, const AM& AR,
  AM& A0, AM& A1, AM& A2, Int A1Width )
{ RepartitionRight_( AL, AR, A0, A1, A2, A1Width, true ); }

template<typename T,typename Int>
inline void
LockedRepartitionRight
( const M& AL, const M& AR,
  M& A0, M& A1, M& A2, Int A1Width )
{ RUNDERSCORE(RepartitionRight_)( AL, AR, A0, A1, A2, A1Width, true ); }

template<typename Int>
inline void
LockedRepartitionRight
( const ADM& AL, const ADM& AR,
  ADM& A0, ADM& A1, ADM& A2, Int A1Width )
{ RepartitionRight_( AL, AR, A0, A1, A2, A1Width, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionRight
( const DM& AL, const DM& AR,
  DM& A0, DM& A1, DM& A2, Int A1Width )
{ RUNDERSCORE(RepartitionRight_)( AL, AR, A0, A1, A2, A1Width, true ); }

//
// RepartitionUpDiagonal
//

template<typename Q,typename Int>
inline void
RepartitionUpDiagonal__
( const Q& ATL, const Q& ATR, Q& A00, Q& A01, Q& A02,
                      Q& A10, Q& A11, Q& A12,
  const Q& ABL, const Q& ABR, Q& A20, Q& A21, Q& A22, Int bsize, bool lock )
{
    bsize = std::min(bsize,std::min(ATL.Height(),ATL.Width()));
    const Int vOffset = ATL.Height()-bsize;
    const Int hOffset = ATL.Width()-bsize;
    View__( A00, ATL, 0,       0,       vOffset,      hOffset    , lock );
    View__( A01, ATL, 0,       hOffset, vOffset,      bsize      , lock );
    View__( A02, ATR, 0,       0,       vOffset,      ATR.Width(), lock );
    View__( A10, ATL, vOffset, 0,       bsize,        hOffset    , lock );
    View__( A11, ATL, vOffset, hOffset, bsize,        bsize      , lock );
    View__( A12, ATR, vOffset, 0,       bsize,        ATR.Width(), lock );
    View__( A20, ABL, 0,       0,       ABL.Height(), hOffset    , lock );
    View__( A21, ABL, 0,       hOffset, ABL.Height(), bsize      , lock );
    View__( A22, ABR, lock );
}

template<typename Q,typename Int>
inline void
RepartitionUpDiagonal_
( const Q& ATL, const Q& ATR, Q& A00, Q& A01, Q& A02,
                              Q& A10, Q& A11, Q& A12,
  const Q& ABL, const Q& ABR, Q& A20, Q& A21, Q& A22, Int bsize, bool lock )
{
    PushCallStack("RepartitionUpDiagonal");
    AssertDataTypes( 13, &ATL, &ATR, &A00, &A01, &A02, &A10, &A11, &A12, &ABL, &ABR, &A20, &A21, &A22 );
    if ( !lock )
    	AssertUnlocked( 4, &ATL, &ATR, &ABL, &ABR );
    AssertContiguous2x2( ATL, ATR, ABL, ABR );
    RepartitionUpDiagonal__( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, lock );
    PopCallStack();
}

template<typename Int>
inline void
RepartitionUpDiagonal
( AM& ATL, AM& ATR, AM& A00, AM& A01, AM& A02,
                    AM& A10, AM& A11, AM& A12,
  AM& ABL, AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize )
{ RepartitionUpDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, false ); }

template<typename T,typename Int>
inline void
RepartitionUpDiagonal
( M& ATL, M& ATR, M& A00, M& A01, M& A02,
                  M& A10, M& A11, M& A12,
  M& ABL, M& ABR, M& A20, M& A21, M& A22, Int bsize )
{ RUNDERSCORE(RepartitionUpDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, false ); }

template<typename Int>
inline void
RepartitionUpDiagonal
( ADM& ATL, ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                      ADM& A10, ADM& A11, ADM& A12,
  ADM& ABL, ADM& ABR, ADM& A20, ADM& A21, ADM& A22, Int bsize )
{ RepartitionUpDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionUpDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize )
{ RUNDERSCORE(RepartitionUpDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, false ); }

template<typename Int>
inline void
LockedRepartitionUpDiagonal
( const AM& ATL, const AM& ATR, AM& A00, AM& A01, AM& A02,
                                AM& A10, AM& A11, AM& A12,
  const AM& ABL, const AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize )
{ RepartitionUpDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, true ); }

template<typename T,typename Int>
inline void
LockedRepartitionUpDiagonal
( const M& ATL, const M& ATR, M& A00, M& A01, M& A02,
                              M& A10, M& A11, M& A12,
  const M& ABL, const M& ABR, M& A20, M& A21, M& A22, Int bsize )
{ RUNDERSCORE(RepartitionUpDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, true ); }

template<typename Int>
inline void
LockedRepartitionUpDiagonal
( const ADM& ATL, const ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                                ADM& A10, ADM& A11, ADM& A12,
  const ADM& ABL, const ADM& ABR, ADM& A20, ADM& A21, ADM& A22, Int bsize )
{ RepartitionUpDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionUpDiagonal
( const DM& ATL, const DM& ATR, DM& A00, DM& A01, DM& A02,
                              DM& A10, DM& A11, DM& A12,
  const DM& ABL, const DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize )
{ RUNDERSCORE(RepartitionUpDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, true ); }

//
// RepartitionDownDiagonal
//

template<typename Q,typename Int>
inline void
RepartitionDownDiagonal__
( const Q& ATL, const Q& ATR, Q& A00, Q& A01, Q& A02,
                              Q& A10, Q& A11, Q& A12,
  const Q& ABL, const Q& ABR, Q& A20, Q& A21, Q& A22, Int bsize, bool lock )
{
    bsize = std::min(bsize,std::min(ABR.Height(),ABR.Width()));
    const Int vOffset = ABR.Height()-bsize;
    const Int hOffset = ABR.Width()-bsize;
    View__( A00, ATL, lock );
    View__( A01, ATR, 0,     0,     ATL.Height(), bsize      , lock );
    View__( A02, ATR, 0,     bsize, ATL.Height(), hOffset    , lock );
    View__( A10, ABL, 0,     0,     bsize,        ABL.Width(), lock );
    View__( A11, ABR, 0,     0,     bsize,        bsize      , lock );
    View__( A12, ABR, 0,     bsize, bsize,        hOffset    , lock );
    View__( A20, ABL, bsize, 0,     vOffset,      ABL.Width(), lock );
    View__( A21, ABR, bsize, 0,     vOffset,      bsize      , lock );
    View__( A22, ABR, bsize, bsize, vOffset,      hOffset    , lock );
}

template<typename Q,typename Int>
inline void
RepartitionDownDiagonal_
( const Q& ATL, const Q& ATR, Q& A00, Q& A01, Q& A02,
                              Q& A10, Q& A11, Q& A12,
  const Q& ABL, const Q& ABR, Q& A20, Q& A21, Q& A22, Int bsize, bool lock )
{
    PushCallStack("RepartitionDownDiagonal");
    AssertDataTypes( 13, &ATL, &ATR, &A00, &A01, &A02, &A10, &A11, &A12, &ABL, &ABR, &A20, &A21, &A22 );
    if ( !lock )
    	AssertUnlocked( 4, &ATL, &ATR, &ABL, &ABR );
    AssertContiguous2x2( ATL, ATR, ABL, ABR );
    RepartitionDownDiagonal__( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, lock );
    PopCallStack();
}

template<typename Int>
inline void
RepartitionDownDiagonal
( AM& ATL, AM& ATR, AM& A00, AM& A01, AM& A02,
                    AM& A10, AM& A11, AM& A12,
  AM& ABL, AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize )
{ RepartitionDownDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, false ); }

template<typename T,typename Int>
inline void
RepartitionDownDiagonal
( M& ATL, M& ATR, M& A00, M& A01, M& A02,
                  M& A10, M& A11, M& A12,
  M& ABL, M& ABR, M& A20, M& A21, M& A22, Int bsize )
{ RUNDERSCORE(RepartitionDownDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, false ); }

template<typename Int>
inline void
RepartitionDownDiagonal
( ADM& ATL, ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                      ADM& A10, ADM& A11, ADM& A12,
  ADM& ABL, ADM& ABR, ADM& A20, ADM& A21, ADM& A22, Int bsize )
{ RepartitionDownDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionDownDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize )
{ RUNDERSCORE(RepartitionDownDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, false ); }

template<typename Int>
inline void
LockedRepartitionDownDiagonal
( const AM& ATL, const AM& ATR, AM& A00, AM& A01, AM& A02,
                                AM& A10, AM& A11, AM& A12,
  const AM& ABL, const AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize )
{ RepartitionDownDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, true ); }

template<typename T,typename Int>
inline void
LockedRepartitionDownDiagonal
( const M& ATL, const M& ATR, M& A00, M& A01, M& A02,
                              M& A10, M& A11, M& A12,
  const M& ABL, const M& ABR, M& A20, M& A21, M& A22, Int bsize )
{ RUNDERSCORE(RepartitionDownDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, true ); }

template<typename Int>
inline void
LockedRepartitionDownDiagonal
( const ADM& ATL, const ADM& ATR, ADM& A00, ADM& A01, ADM& A02,
                                  ADM& A10, ADM& A11, ADM& A12,
  const ADM& ABL, const ADM& ABR, ADM& A20, ADM& A21, ADM& A22, Int bsize )
{ RepartitionDownDiagonal_( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionDownDiagonal
( const DM& ATL, const DM& ATR, DM& A00, DM& A01, DM& A02,
                                DM& A10, DM& A11, DM& A12,
  const DM& ABL, const DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize )
{ RUNDERSCORE(RepartitionDownDiagonal_)( ATL, ATR, A00, A01, A02, A10, A11, A12, ABL, ABR, A20, A21, A22, bsize, true ); }

#undef AM
#undef DM
#undef M
#undef ADM

} // namespace elem

#endif // ifndef CORE_REPARTITION_IMPL_HPP
