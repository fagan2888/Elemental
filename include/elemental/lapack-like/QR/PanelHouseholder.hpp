/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef LAPACK_QR_PANEL_HPP
#define LAPACK_QR_PANEL_HPP

#include "elemental/blas-like/level2/Gemv.hpp"
#include "elemental/blas-like/level2/Ger.hpp"
#include "elemental/lapack-like/Reflector.hpp"
#include "elemental/matrices/Zeros.hpp"

namespace elem {
namespace qr {

template<typename Real>
inline void
PanelHouseholder( Matrix<Real>& A )
{
#ifndef RELEASE
    PushCallStack("qr::PanelHouseholder");
#endif
    Matrix<Real>
        ATL, ATR,  A00, a01,     A02,  aLeftCol, ARightPan,
        ABL, ABR,  a10, alpha11, a12,
                   A20, a21,     A22;

    Matrix<Real> z;

    PushBlocksizeStack( 1 );
    PartitionDownLeftDiagonal
    ( A, ATL, ATR,
         ABL, ABR, 0 );
    while( ATL.Height() < A.Height() && ATL.Width() < A.Width() )
    {
        RepartitionDownDiagonal
        ( ATL, /**/ ATR,  A00, /**/ a01,     A02,
         /*************/ /**********************/
               /**/       a10, /**/ alpha11, a12,
          ABL, /**/ ABR,  A20, /**/ a21,     A22 );

        View2x1( aLeftCol, alpha11,
                           a21 );

        View2x1( ARightPan, a12,
                            A22 );

        Zeros( ARightPan.Width(), 1, z );
        //--------------------------------------------------------------------//
        const Real tau = Reflector( alpha11, a21 );
        const Real alpha = alpha11.Get(0,0);
        alpha11.Set(0,0,1);

        Gemv( TRANSPOSE, Real(1), ARightPan, aLeftCol, Real(0), z );
        Ger( -tau, aLeftCol, z, ARightPan );

        alpha11.Set(0,0,alpha);
        //--------------------------------------------------------------------//

        SlidePartitionDownDiagonal
        ( ATL, /**/ ATR,  A00, a01,     /**/ A02,
               /**/       a10, alpha11, /**/ a12,
         /*************/ /**********************/
          ABL, /**/ ABR,  A20, a21,     /**/ A22 );
    }
    PopBlocksizeStack();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Real>
inline void
PanelHouseholder( DistMatrix<Real>& A )
{
#ifndef RELEASE
    PushCallStack("qr::PanelHouseholder");
#endif
    const Grid& g = A.Grid();

    // Matrix views
    DistMatrix<Real>
        ATL(g), ATR(g),  A00(g), a01(g),     A02(g),  aLeftCol(g), ARightPan(g),
        ABL(g), ABR(g),  a10(g), alpha11(g), a12(g),
                         A20(g), a21(g),     A22(g);

    // Temporary distributions
    DistMatrix<Real,MC,STAR> aLeftCol_MC_STAR(g);
    DistMatrix<Real,MR,STAR> z_MR_STAR(g);

    PushBlocksizeStack( 1 );
    PartitionDownLeftDiagonal
    ( A, ATL, ATR,
         ABL, ABR, 0 );
    while( ATL.Height() < A.Height() && ATL.Width() < A.Width() )
    {
        RepartitionDownDiagonal
        ( ATL, /**/ ATR,  A00, /**/ a01,     A02,
         /*************/ /**********************/
               /**/       a10, /**/ alpha11, a12,
          ABL, /**/ ABR,  A20, /**/ a21,     A22 );

        View2x1( aLeftCol, alpha11,
                           a21 );

        View2x1( ARightPan, a12,
                            A22 );

        aLeftCol_MC_STAR.AlignWith( ARightPan );
        z_MR_STAR.AlignWith( ARightPan );
        Zeros( ARightPan.Width(), 1, z_MR_STAR );
        //--------------------------------------------------------------------//
        const Real tau = Reflector( alpha11, a21 );

        const bool myDiagonalEntry = ( g.Row() == alpha11.ColAlignment() && 
                                       g.Col() == alpha11.RowAlignment() );
        Real alpha = 0;
        if( myDiagonalEntry )
        {
            alpha = alpha11.GetLocal(0,0);
            alpha11.SetLocal(0,0,1);
        }

        aLeftCol_MC_STAR = aLeftCol;

        Gemv
        ( TRANSPOSE, 
          Real(1), ARightPan.LockedMatrix(), 
                   aLeftCol_MC_STAR.LockedMatrix(),
          Real(0), z_MR_STAR.Matrix() );
        z_MR_STAR.SumOverCol(); 

        Ger
        ( -tau, 
          aLeftCol_MC_STAR.LockedMatrix(), 
          z_MR_STAR.LockedMatrix(),
          ARightPan.Matrix() );

        if( myDiagonalEntry )
            alpha11.SetLocal(0,0,alpha);
        //--------------------------------------------------------------------//
        aLeftCol_MC_STAR.FreeAlignments();
        z_MR_STAR.FreeAlignments();

        SlidePartitionDownDiagonal
        ( ATL, /**/ ATR,  A00, a01,     /**/ A02,
               /**/       a10, alpha11, /**/ a12,
         /*************/ /**********************/
          ABL, /**/ ABR,  A20, a21,     /**/ A22 );
    }
    PopBlocksizeStack();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename Real> 
inline void
PanelHouseholder
( Matrix<Complex<Real> >& A,
  Matrix<Complex<Real> >& t )
{
#ifndef RELEASE
    PushCallStack("qr::PanelHouseholder");
    if( t.Height() != std::min(A.Height(),A.Width()) || t.Width() != 1 )
        throw std::logic_error
        ("t must be a vector of height equal to the minimum dimension of A");
#endif
    typedef Complex<Real> C;

    Matrix<C>
        ATL, ATR,  A00, a01,     A02,  aLeftCol, ARightPan,
        ABL, ABR,  a10, alpha11, a12,
                   A20, a21,     A22;
    Matrix<C>
        tT,  t0,
        tB,  tau1,
             t2;

    Matrix<C> z;

    PushBlocksizeStack( 1 );
    PartitionDownLeftDiagonal
    ( A, ATL, ATR,
         ABL, ABR, 0 );
    PartitionDown
    ( t, tT,
         tB, 0 );
    while( ATL.Height() < A.Height() && ATL.Width() < A.Width() )
    {
        RepartitionDownDiagonal
        ( ATL, /**/ ATR,  A00, /**/ a01,     A02,
         /*************/ /**********************/
               /**/       a10, /**/ alpha11, a12,
          ABL, /**/ ABR,  A20, /**/ a21,     A22 );

        RepartitionDown
        ( tT,  t0,
         /**/ /****/
               tau1, 
          tB,  t2 );

        View2x1( aLeftCol, alpha11,
                           a21 );

        View2x1( ARightPan, a12,
                            A22 );

        Zeros( ARightPan.Width(), 1, z );
        //--------------------------------------------------------------------//
        const C tau = Reflector( alpha11, a21 );
        tau1.Set( 0, 0, tau );
        const C alpha = alpha11.Get(0,0);
        alpha11.Set(0,0,1);

        Gemv( ADJOINT, C(1), ARightPan, aLeftCol, C(0), z );
        Ger( -Conj(tau), aLeftCol, z, ARightPan );

        alpha11.Set(0,0,alpha);
        //--------------------------------------------------------------------//

        SlidePartitionDown
        ( tT,  t0,
               tau1,
         /**/ /****/
          tB,  t2 );

        SlidePartitionDownDiagonal
        ( ATL, /**/ ATR,  A00, a01,     /**/ A02,
               /**/       a10, alpha11, /**/ a12,
         /*************/ /**********************/
          ABL, /**/ ABR,  A20, a21,     /**/ A22 );
    }
    PopBlocksizeStack();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename R> 
inline void
PanelHouseholder
( DistMatrix<Complex<R> >& A,
  DistMatrix<Complex<R>,MD,STAR>& t )
{
#ifndef RELEASE
    PushCallStack("qr::PanelHouseholder");
    if( A.Grid() != t.Grid() )
        throw std::logic_error("{A,t} must be distributed over the same grid");
    if( t.Height() != std::min(A.Height(),A.Width()) || t.Width() != 1 )
        throw std::logic_error
        ("t must be a vector of height equal to the minimum dimension of A");
    if( !t.AlignedWithDiagonal( A, 0 ) )
        throw std::logic_error("t must be aligned with A's main diagonal");
#endif
    typedef Complex<R> C;
    const Grid& g = A.Grid();

    // Matrix views
    DistMatrix<C>
        ATL(g), ATR(g),  A00(g), a01(g),     A02(g),  aLeftCol(g), ARightPan(g),
        ABL(g), ABR(g),  a10(g), alpha11(g), a12(g),
                         A20(g), a21(g),     A22(g);
    DistMatrix<C,MD,STAR>
        tT(g),  t0(g),
        tB(g),  tau1(g),
                t2(g);

    // Temporary distributions
    DistMatrix<C,MC,STAR> aLeftCol_MC_STAR(g);
    DistMatrix<C,MR,STAR> z_MR_STAR(g);

    PushBlocksizeStack( 1 );
    PartitionDownLeftDiagonal
    ( A, ATL, ATR,
         ABL, ABR, 0 );
    PartitionDown
    ( t, tT,
         tB, 0 );
    while( ATL.Height() < A.Height() && ATL.Width() < A.Width() )
    {
        RepartitionDownDiagonal
        ( ATL, /**/ ATR,  A00, /**/ a01,     A02,
         /*************/ /**********************/
               /**/       a10, /**/ alpha11, a12,
          ABL, /**/ ABR,  A20, /**/ a21,     A22 );

        RepartitionDown
        ( tT,  t0,
         /**/ /****/
               tau1, 
          tB,  t2 );

        View2x1( aLeftCol, alpha11,
                           a21 );

        View2x1( ARightPan, a12,
                            A22 );

        aLeftCol_MC_STAR.AlignWith( ARightPan );
        z_MR_STAR.AlignWith( ARightPan );
        Zeros( ARightPan.Width(), 1, z_MR_STAR );
        //--------------------------------------------------------------------//
        const C tau = Reflector( alpha11, a21 );
        tau1.Set( 0, 0, tau );

        const bool myDiagonalEntry = ( g.Row() == alpha11.ColAlignment() && 
                                       g.Col() == alpha11.RowAlignment() );
        C alpha = 0;
        if( myDiagonalEntry )
        {
            alpha = alpha11.GetLocal(0,0);
            alpha11.SetLocal(0,0,1);
        }

        aLeftCol_MC_STAR = aLeftCol;

        Gemv
        ( ADJOINT, 
          C(1), ARightPan.LockedMatrix(), 
                aLeftCol_MC_STAR.LockedMatrix(),
          C(0), z_MR_STAR.Matrix() );
        z_MR_STAR.SumOverCol(); 

        Ger
        ( -Conj(tau), 
          aLeftCol_MC_STAR.LockedMatrix(), 
          z_MR_STAR.LockedMatrix(),
          ARightPan.Matrix() );

        if( myDiagonalEntry )
            alpha11.SetLocal(0,0,alpha);
        //--------------------------------------------------------------------//
        aLeftCol_MC_STAR.FreeAlignments();
        z_MR_STAR.FreeAlignments();

        SlidePartitionDown
        ( tT,  t0,
               tau1,
         /**/ /****/
          tB,  t2 );

        SlidePartitionDownDiagonal
        ( ATL, /**/ ATR,  A00, a01,     /**/ A02,
               /**/       a10, alpha11, /**/ a12,
         /*************/ /**********************/
          ABL, /**/ ABR,  A20, a21,     /**/ A22 );
    }
    PopBlocksizeStack();
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace qr
} // namespace elem

#endif // ifndef LAPACK_QR_PANEL_HPP
