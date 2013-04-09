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

///
// HandleDiagPath
//

template<typename T,Distribution U,Distribution V,typename Int>
inline void HandleDiagPath
( DM& A, const DM& B )
{ }

template<typename T,typename Int>
inline void HandleDiagPath
( DistMatrix<T,MD,STAR,Int>& A, const DistMatrix<T,MD,STAR,Int>& B )
{ A.diagPath_ = B.diagPath_; } 

template<typename T,typename Int>
inline void HandleDiagPath
( DistMatrix<T,STAR,MD,Int>& A, const DistMatrix<T,STAR,MD,Int>& B )
{ A.diagPath_ = B.diagPath_; } 

//
// View
//

template<typename Int> inline
void View__( AM& A, const AM& B, bool lock )
{ 
	A.Attach__( B, 0, 0, B.Height(), B.Width(), lock );
}

template<typename Int> inline
void View_( AM& A, const AM& B, bool lock )
{
    PushCallStack("View [Matrix]");
    A.AssertDataTypes( B );
    if ( !lock ) 
    	B.AssertUnlocked( AM::ViewLock );
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void View
( DM& A, DM& B )
{
#ifndef RELEASE
    PushCallStack("View");
#endif
    A.Empty();
    A.grid_ = B.grid_;
    A.height_ = B.Height();
    A.width_ = B.Width();
    A.colAlignment_ = B.ColAlignment();
    A.rowAlignment_ = B.RowAlignment();
    HandleDiagPath( A, B );
    A.viewing_ = true;
    if( A.Participating() )
    {
        A.colShift_ = B.ColShift();
        A.rowShift_ = B.RowShift();
        View( A.Matrix(), B.Matrix() );
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template <typename Int>
inline void LockedView
( AM& A, const AM& B )
{ View_( A, B, true ); }

template<typename T,typename Int>
inline void LockedView
( M& A, const M& B )
{ RUNDERSCORE(View_)( A, B, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView
( DM& A, const DM& B )
{
#ifndef RELEASE
    PushCallStack("LockedView");
#endif
    A.Empty();
    A.grid_ = B.grid_;
    A.height_ = B.Height();
    A.width_ = B.Width();
    A.colAlignment_ = B.ColAlignment();
    A.rowAlignment_ = B.RowAlignment();
    HandleDiagPath( A, B );
    A.viewing_ = true;
    A.locked_ = true;
    if( A.Participating() )
    {
        A.colShift_ = B.ColShift();
        A.rowShift_ = B.RowShift();
        LockedView( A.Matrix(), B.LockedMatrix() );
    }
    else 
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE 
    PopCallStack();
#endif
}

//
// View(i,j,h,w)
//

template<typename Int> inline
void View__( AM& A, const AM& B, Int i, Int j, Int height, Int width, bool lock )
{ 
	A.Attach__( B, i, j, height, width, lock );
}

template<typename Int> inline
void View_( AM& A, const AM& B, Int i, Int j, Int height, Int width, bool lock )
{
    PushCallStack("View(i,j,h,w) [Matrix]");
    A.AssertDataTypes( B );
	if ( !lock ) 
		B.AssertUnlocked( AM::ViewLock );
	B.AssertView( i, j, height, width );
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void View
( DM& A, DM& B,
  Int i, Int j, Int height, Int width )
{
#ifndef RELEASE
    PushCallStack("View");
    B.AssertValidSubmatrix( i, j, height, width );
#endif
    A.Empty();

    const elem::Grid& g = B.Grid();
    const Int colStride = B.ColStride();
    const Int rowStride = B.RowStride();

    A.grid_ = &g;
    A.height_ = height;
    A.width_  = width;

    A.colAlignment_ = (B.ColAlignment()+i) % colStride;
    A.rowAlignment_ = (B.RowAlignment()+j) % rowStride;
    HandleDiagPath( A, B );
    A.viewing_ = true;

    if( A.Participating() )
    {
        const Int colRank = B.ColRank();
        const Int rowRank = B.RowRank();
        A.colShift_ = Shift( colRank, A.ColAlignment(), colStride );
        A.rowShift_ = Shift( rowRank, A.RowAlignment(), rowStride );

        const Int localHeightBehind = Length(i,B.ColShift(),colStride);
        const Int localWidthBehind  = Length(j,B.RowShift(),rowStride);

        const Int localHeight = Length( height, A.ColShift(), colStride );
        const Int localWidth  = Length( width,  A.RowShift(), rowStride );

        View
        ( A.Matrix(), B.Matrix(), 
          localHeightBehind, localWidthBehind, localHeight, localWidth );
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template <typename Int>
inline void LockedView
( AM& A, const AM& B, Int i, Int j, Int height, Int width )
{ View_( A, B, i, j, height, width, true ); }

template<typename T,typename Int>
inline void LockedView
( M& A, const M& B, Int i, Int j, Int height, Int width )
{ RUNDERSCORE(View_)( A, B, i, j, height, width, true ); }

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView
( DM& A, const DM& B,
  Int i, Int j, Int height, Int width )
{
#ifndef RELEASE
    PushCallStack("LockedView");
    B.AssertValidSubmatrix( i, j, height, width );
#endif
    A.Empty();

    const elem::Grid& g = B.Grid();
    const Int colStride = B.ColStride();
    const Int rowStride = B.RowStride();
    const Int colRank = B.ColRank();
    const Int rowRank = B.RowRank();

    A.grid_ = &g;
    A.height_ = height;
    A.width_  = width;

    A.colAlignment_ = (B.ColAlignment()+i) % colStride;
    A.rowAlignment_ = (B.RowAlignment()+j) % rowStride;
    HandleDiagPath( A, B );
    A.viewing_ = true;
    A.locked_ = true;

    if( A.Participating() )
    {
        A.colShift_ = Shift( colRank, A.ColAlignment(), colStride );
        A.rowShift_ = Shift( rowRank, A.RowAlignment(), rowStride );

        const Int localHeightBehind = Length(i,B.ColShift(),colStride);
        const Int localWidthBehind  = Length(j,B.RowShift(),rowStride);

        const Int localHeight = Length( height, A.ColShift(), colStride );
        const Int localWidth  = Length( width,  A.RowShift(), rowStride );

        LockedView
        ( A.Matrix(), B.LockedMatrix(), 
          localHeightBehind, localWidthBehind, localHeight, localWidth );
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// View1x2
//

template<typename Int> inline
void View1x2__( AM& A, const AM& BL, const AM& BR, bool lock )
{ 
	A.Attach__( BL, 0, 0, BL.Height(), BL.Width() + BR.Width(), lock );
}

template<typename Int> inline
void View1x2_( AM& A, const AM& BL, const AM& BR, bool lock )
{
    PushCallStack("View1x2 [AutoMatrix]");
    A.AssertDataTypes( BL );
    A.AssertDataTypes( BR );
	if ( !lock ) {
		BL.AssertUnlocked( AM::ViewLock );
		BR.AssertUnlocked( AM::ViewLock );
	}
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void View1x2
( DM& A,
  DM& BL, DM& BR )
{
#ifndef RELEASE
    PushCallStack("View1x2");
    AssertConforming1x2( BL, BR );
    BL.AssertSameGrid( BR.Grid() );
#endif
    A.Empty();
    A.grid_ = BL.grid_;
    A.height_ = BL.Height();
    A.width_ = BL.Width() + BR.Width();
    A.colAlignment_ = BL.ColAlignment();
    A.rowAlignment_ = BL.RowAlignment();
    HandleDiagPath( A, BL );
    A.viewing_ = true;
    if( A.Participating() )
    {
        A.colShift_ = BL.ColShift();
        A.rowShift_ = BL.RowShift();
        View1x2( A.Matrix(), BL.Matrix(), BR.Matrix() );
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView1x2
(       DM& A,
  const DM& BL,
  const DM& BR )
{
#ifndef RELEASE
    PushCallStack("LockedView1x2");
    AssertConforming1x2( BL, BR );
    BL.AssertSameGrid( BR.Grid() );
#endif
    A.Empty();
    A.grid_ = BL.grid_;
    A.height_ = BL.Height();
    A.width_ = BL.Width() + BR.Width();
    A.colAlignment_ = BL.ColAlignment();
    A.rowAlignment_ = BL.RowAlignment();
    HandleDiagPath( A, BL );
    A.viewing_ = true;
    A.locked_ = true;
    if( A.Participating() )
    {
        A.colShift_ = BL.ColShift();
        A.rowShift_ = BL.RowShift();
        LockedView1x2( A.Matrix(), BL.LockedMatrix(), BR.LockedMatrix() );
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// View2x1
//

template<typename Int> inline
void View2x1__( AM& A, const AM& BT, 
                       const AM& BB, bool lock )
{ 
	A.Attach__( BT, 0, 0, BT.Height() + BB.Height(), BT.Width(), lock );
}

template<typename Int> inline
void View2x1_( AM& A, const AM& BT, const AM& BB, bool lock )
{
    PushCallStack("View2x1 [Matrix]");
    A.AssertDataTypes( BT );
    A.AssertDataTypes( BB );
	if ( !lock ) {
		BT.AssertUnlocked( AM::ViewLock );
		BB.AssertUnlocked( AM::ViewLock );
	}
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void View2x1
( DM& A,
  DM& BT,
  DM& BB )
{
#ifndef RELEASE
    PushCallStack("View2x1");
    AssertConforming2x1( BT, BB );
    BT.AssertSameGrid( BB.Grid() );
#endif
    A.Empty();
    A.grid_ = BT.grid_;
    A.height_ = BT.Height() + BB.Height();
    A.width_ = BT.Width();
    A.colAlignment_ = BT.ColAlignment();
    A.rowAlignment_ = BT.RowAlignment();
    HandleDiagPath( A, BT );
    A.viewing_ = true;
    if( A.Participating() )
    {
        A.colShift_ = BT.ColShift();
        A.rowShift_ = BT.RowShift();
        View2x1( A.Matrix(), BT.Matrix(), BB.Matrix() );
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView2x1
(       DM& A,
  const DM& BT,
  const DM& BB )
{
#ifndef RELEASE
    PushCallStack("LockedView2x1");
    AssertConforming2x1( BT, BB );
    BT.AssertSameGrid( BB.Grid() );
#endif
    A.Empty();
    A.grid_ = BT.grid_;
    A.height_ = BT.Height() + BB.Height();
    A.width_ = BT.Width();
    A.colAlignment_ = BT.ColAlignment();
    A.rowAlignment_ = BT.RowAlignment();
    HandleDiagPath( A, BT );
    A.viewing_ = true;
    A.locked_ = true;
    if( A.Participating() )
    {
        A.colShift_ = BT.ColShift();
        A.rowShift_ = BT.RowShift();
        LockedView2x1( A.Matrix(), BT.LockedMatrix(), BB.LockedMatrix() );
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// View2x2
//

template<typename Int> inline
void View2x2__( AM& A, const AM& BTL, const AM& BTR, 
                       const AM& BBL, const AM& BBR, bool lock )
{ 
	A.Attach__( BTL, 0, 0, BTL.Height() + BBL.Height(), BTL.Width() + BTR.Width(), lock ); 
}

template<typename Int> inline
void View2x2_( AM& A, const AM& BTL, const AM& BTR,
                      const AM& BBL, const AM& BBR,
                      bool lock )
{
    PushCallStack( "View2x2 [Matrix]" );
    A.AssertDataTypes( BTL );
	A.AssertDataTypes( BTR );
	A.AssertDataTypes( BBL );
	A.AssertDataTypes( BBR );
	if ( !lock ) {
		BTL.AssertUnlocked( AM::ViewLock );
		BTR.AssertUnlocked( AM::ViewLock );
		BBL.AssertUnlocked( AM::ViewLock );
		BBR.AssertUnlocked( AM::ViewLock );
	}
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void View2x2
( DM& A,
  DM& BTL, DM& BTR,
  DM& BBL, DM& BBR )
{
#ifndef RELEASE
    PushCallStack("View2x2");
    AssertConforming2x2( BTL, BTR, BBL, BBR );
    BTL.AssertSameGrid( BTR.Grid() );
    BTL.AssertSameGrid( BBL.Grid() );
    BTL.AssertSameGrid( BBR.Grid() );
#endif
    A.Empty();
    A.grid_ = BTL.grid_;
    A.height_ = BTL.Height() + BBL.Height();
    A.width_ = BTL.Width() + BTR.Width();
    A.colAlignment_ = BTL.ColAlignment();
    A.rowAlignment_ = BTL.RowAlignment();
    HandleDiagPath( A, BTL );
    A.viewing_ = true;
    if( A.Participating() )
    {
        A.colShift_ = BTL.ColShift();
        A.rowShift_ = BTL.RowShift();
        View2x2
        ( A.Matrix(), BTL.Matrix(), BTR.Matrix(),
                      BBL.Matrix(), BBR.Matrix() ); 
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void LockedView2x2
(       DM& A,
  const DM& BTL,
  const DM& BTR,
  const DM& BBL,
  const DM& BBR )
{
#ifndef RELEASE
    PushCallStack("LockedView2x2");
    AssertConforming2x2( BTL, BTR, BBL, BBR );
    BTL.AssertSameGrid( BTR.Grid() );
    BTL.AssertSameGrid( BBL.Grid() );
    BTL.AssertSameGrid( BBR.Grid() );
#endif
    A.Empty();
    A.grid_ = BTL.grid_;
    A.height_ = BTL.Height() + BBL.Height();
    A.width_ = BTL.Width() + BTR.Width();
    A.colAlignment_ = BTL.ColAlignment();
    A.rowAlignment_ = BTL.RowAlignment();
    HandleDiagPath( A, BTL );
    A.viewing_ = true;
    A.locked_ = true;
    if( A.Participating() )
    {
        A.colShift_ = BTL.ColShift();
        A.rowShift_ = BTL.RowShift();
        LockedView2x2
        ( A.Matrix(), BTL.LockedMatrix(), BTR.LockedMatrix(),
                      BBL.LockedMatrix(), BBR.LockedMatrix() );
    }
    else
    {
        A.colShift_ = 0;
        A.rowShift_ = 0;
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

#undef RUNDERSCORE
#undef AM
#undef DM
#undef M

} // namespace elem

#endif // ifndef CORE_VIEW_IMPL_HPP
