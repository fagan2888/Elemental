/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_VIEW_IMPL_HPP
#define CORE_VIEW_IMPL_HPP

namespace elem {

// To make our life easier. Undef'd at the bottom of the header
#define M Matrix<T,Int>
#define AM AutoMatrix<Int>
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
// View
//

template<typename Q> inline
void View__( Q& A, const Q& B, bool lock )
{ 
	A.Attach__( B, 0, 0, B.Height(), B.Width(), lock );
}

template<typename Q> inline
void View_( Q& A, const Q& B, bool lock )
{
    PushCallStack("View");
    AssertDataTypes( A, B );
    if ( !lock ) 
    	AssertUnlocked( B );
    View__( A, B, lock );
    PopCallStack();
}

template<typename Int>
inline void View
( AM& A, AM& B )
{ View_( A, B, false ); }

template<typename T,typename Int>
inline void View
( M& A, M& B )
{ RUNDERSCORE(View_)( A, B, false ); }

template <typename Int> inline
void View( ADM& A, ADM& B )
{ View_( A, B, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void View( DM& A, const DM& B )
{ RUNDERSCORE(View_)( A, B, false ); }

template <typename Int>
inline void LockedView
( AM& A, const AM& B )
{ View_( A, B, true ); }

template<typename T,typename Int>
inline void LockedView
( M& A, const M& B )
{ RUNDERSCORE(View_)( A, B, true ); }

template <typename Int>
inline void LockedView
( ADM& A, const ADM& B )
{ View_( A, B, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView
( DM& A, const DM& B )
{ RUNDERSCORE(View_)( A, B, true ); }

//
// View(i,j,h,w)
//

template<typename Q,typename Int> inline
void View__( Q& A, const Q& B, Int i, Int j, Int height, Int width, bool lock )
{ 
	A.Attach__( B, i, j, height, width, lock );
}

template<typename Q,typename Int> inline
void View_( Q& A, const Q& B, Int i, Int j, Int height, Int width, bool lock )
{
    PushCallStack("View(i,j,h,w)");
    AssertDataTypes( A, B );
	if ( !lock ) 
		AssertUnlocked( B );
	AssertValidSubmatrix( i, j, height, width, B.Height(), B.Width() );
    View__( A, B, i, j, height, width, lock );
    PopCallStack();
}

template<typename Int>
inline void View
( AM& A, AM& B, Int i, Int j, Int height, Int width )
{ View_( A, B, i, j, height, width, false ); }

template<typename T,typename Int>
inline void View
( M& A, M& B, Int i, Int j, Int height, Int width )
{ RUNDERSCORE(View_)( A, B, i, j, height, width, false ); }

template<typename Int>
inline void View
( ADM& A, ADM& B, Int i, Int j, Int height, Int width )
{ View_( A, B, i, j, height, width, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void View
( DM& A, DM& B, Int i, Int j, Int height, Int width )
{ RUNDERSCORE(View_)( A, B, i, j, height, width, false ); }

template <typename Int>
inline void LockedView
( AM& A, const AM& B, Int i, Int j, Int height, Int width )
{ View_( A, B, i, j, height, width, true ); }

template<typename T,typename Int>
inline void LockedView
( M& A, const M& B, Int i, Int j, Int height, Int width )
{ RUNDERSCORE(View_)( A, B, i, j, height, width, true ); }

template <typename Int>
inline void LockedView
( ADM& A, const ADM& B, Int i, Int j, Int height, Int width )
{ View_( A, B, i, j, height, width, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView
( DM& A, const DM& B, Int i, Int j, Int height, Int width )
{ RUNDERSCORE(View_)( A, B, i, j, height, width, true ); }

//
// View1x2
//

template<typename Q> inline
void View1x2__( Q& A, const Q& BL, const Q& BR, bool lock )
{ 
	A.Attach__( BL, 0, 0, BL.Height(), BL.Width() + BR.Width(), lock );
}

template<typename Q> inline
void View1x2_( Q& A, const Q& BL, const Q& BR, bool lock )
{
    PushCallStack("View1x2");
    AssertDataTypes( 3, &A, &BL, &BR );
	if ( !lock )
		AssertUnlocked( 2, &BL, &BR );
	AssertContiguous1x2( BL, BR );
    View1x2__( A, BL, BR, lock );
    PopCallStack();
}

template <typename Int>
inline void View1x2
( AM& A,
  AM& BL, AM& BR )
{ View1x2_( A, BL, BR, false ); }

template<typename T,typename Int>
inline void View1x2
( M& A,
  M& BL, M& BR )
{ RUNDERSCORE(View1x2_)( A, BL, BR, false ); }

template <typename Int>
inline void View1x2
( ADM& A,
  ADM& BL, ADM& BR )
{ View1x2_( A, BL, BR, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void View1x2
( DM& A,
  DM& BL, DM& BR )
{ RUNDERSCORE(View1x2_)( A, BL, BR, false ); }

template<typename Int>
inline void LockedView1x2
(       AM& A,
  const AM& BL,
  const AM& BR )
{ View1x2_( A, BL, BR, true ); }

template<typename T,typename Int>
inline void LockedView1x2
(       M& A,
  const M& BL,
  const M& BR )
{ RUNDERSCORE(View1x2_)( A, BL, BR, true ); }

template<typename Int>
inline void LockedView1x2
(       ADM& A,
  const ADM& BL,
  const ADM& BR )
{ View1x2_( A, BL, BR, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView1x2
(       DM& A,
  const DM& BL,
  const DM& BR )
{ RUNDERSCORE(View1x2_)( A, BL, BR, true ); }

//
// View2x1
//

template<typename Q> inline
void View2x1__( Q& A, const Q& BT, 
                      const Q& BB, bool lock )
{ 
	A.Attach__( BT, 0, 0, BT.Height() + BB.Height(), BT.Width(), lock );
}

template<typename Q> inline
void View2x1_( Q& A, const Q& BT, const Q& BB, bool lock )
{
    PushCallStack("View2x1");
    AssertDataTypes( 3, &A, &BT, &BB );
	if ( !lock )
		AssertUnlocked( 2, &BT, &BB );
	AssertContiguous2x1( BT, BB );
	View2x1__( A, BT, BB, lock );
    PopCallStack();
}

template<typename Int>
inline void View2x1
( AM& A,
  AM& BT,
  AM& BB )
{ View2x1_( A, BT, BB, false ); }

template<typename T,typename Int>
inline void View2x1
( M& A,
  M& BT,
  M& BB )
{ RUNDERSCORE(View2x1_)( A, BT, BB, false ); }

template<typename Int>
inline void View2x1
( ADM& A,
  ADM& BT,
  ADM& BB )
{ View2x1_( A, BT, BB, false ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void View2x1
( DM& A,
  DM& BT,
  DM& BB )
{ RUNDERSCORE(View2x1_)( A, BT, BB, false ); }

template<typename Int>
inline void LockedView2x1
(       AM& A,
  const AM& BT,
  const AM& BB )
{ View2x1_( A, BT, BB, true ); }

template<typename T,typename Int>
inline void LockedView2x1
(       M& A,
  const M& BT,
  const M& BB )
{ RUNDERSCORE(View2x1_)( A, BT, BB, true ); }

template<typename Int>
inline void LockedView2x1
(       ADM& A,
  const ADM& BT,
  const ADM& BB )
{ View2x1_( A, BT, BB, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView2x1
(       DM& A,
  const DM& BT,
  const DM& BB )
{ RUNDERSCORE(View2x1_)( A, BT, BB, true ); }

//
// View2x2
//

template<typename Q> inline
void View2x2__( Q& A, const Q& BTL, const Q& BTR, 
                      const Q& BBL, const Q& BBR, bool lock )
{ 
	A.Attach__( BTL, 0, 0, BTL.Height() + BBL.Height(), BTL.Width() + BTR.Width(), lock ); 
}

template<typename Q> inline
void View2x2_( Q& A, const Q& BTL, const Q& BTR,
                     const Q& BBL, const Q& BBR,
                     bool lock )
{
    PushCallStack( "View2x2" );
    AssertDataTypes( 5, &A, &BTL, &BTR, &BBL, &BBR );
	if ( !lock )
		AssertUnlocked( 4, &BTL, &BTR, &BBL, &BBR );
	AssertContiguous2x2( BTL, BTR, BBL, BBR );
	View2x2__( A, BTL, BTR, BBL, BBR, lock );
    PopCallStack();
}
 
template <typename Int>
inline void View2x2
( AM& A,
  AM& BTL, AM& BTR,
  AM& BBL, AM& BBR )
{ View2x2_(A,BTL,BTR,BBL,BBR,false); }

template<typename T,typename Int>
inline void View2x2
( M& A,
  M& BTL, M& BTR,
  M& BBL, M& BBR )
{ RUNDERSCORE(View2x2_)(A,BTL,BTR,BBL,BBR,false); }

template <typename Int>
inline void View2x2
( ADM& A,
  ADM& BTL, ADM& BTR,
  ADM& BBL, ADM& BBR )
{ View2x2_(A,BTL,BTR,BBL,BBR,false); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void View2x2
( M& A,
  M& BTL, M& BTR,
  M& BBL, M& BBR )
{ RUNDERSCORE(View2x2_)(A,BTL,BTR,BBL,BBR,false); }

template <typename Int>
inline void LockedView2x2
(       AM& A,
  const AM& BTL,
  const AM& BTR,
  const AM& BBL,
  const AM& BBR )
{ View2x2_(A,BTL,BTR,BBL,BBR,true); }

template<typename T,typename Int>
inline void LockedView2x2
(       M& A,
  const M& BTL,
  const M& BTR,
  const M& BBL,
  const M& BBR )
{ RUNDERSCORE(View2x2_)(A,BTL,BTR,BBL,BBR,true); }

template <typename Int>
inline void LockedView2x2
(       ADM& A,
  const ADM& BTL, const ADM& BTR,
  const ADM& BBL, const ADM& BBR )
{ View2x2_(A,BTL,BTR,BBL,BBR,true); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView2x2
(       DM& A,
  const DM& BTL, const DM& BTR,
  const DM& BBL, const DM& BBR )
{ RUNDERSCORE(View2x2_)(A,BTL,BTR,BBL,BBR,true); }

#undef AM
#undef DM
#undef M
#undef ADM

} // namespace elem

#endif // ifndef CORE_VIEW_IMPL_HPP
