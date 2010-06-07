/*
   This file is part of elemental, a library for distributed-memory dense 
   linear algebra.

   Copyright (C) 2009-2010 Jack Poulson <jack.poulson@gmail.com>

   This program is released under the terms of the license contained in the 
   file LICENSE.
*/
#include "elemental/blas_internal.hpp"
using namespace std;
using namespace elemental;

template<typename T>
void
elemental::blas::internal::SyrkUN
( T alpha, const DistMatrix<T,MC,MR>& A,
  T beta,        DistMatrix<T,MC,MR>& C )
{
#ifndef RELEASE
    PushCallStack("blas::internal::SyrkUN");
    if( A.GetGrid() != C.GetGrid() )
        throw logic_error( "A and C must be distributed over the same grid." );
    if( A.Height() != C.Height() || A.Height() != C.Width() )
    {
        ostringstream msg;
        msg << "Nonconformal SyrkUN:" << endl
            << "  A ~ " << A.Height() << " x " << A.Width() << endl
            << "  C ~ " << C.Height() << " x " << C.Width() << endl;
        throw logic_error( msg.str() );
    }
#endif
    const Grid& g = A.GetGrid();

    // Matrix views
    DistMatrix<T,MC,MR> AL(g), AR(g),
                        A0(g), A1(g), A2(g);

    // Temporary distributions
    DistMatrix<T,MC,  Star> A1_MC_Star(g);
    DistMatrix<T,VR,  Star> A1_VR_Star(g);
    DistMatrix<T,Star,MR  > A1Trans_Star_MR(g);

    // Start the algorithm
    blas::Scal( beta, C );
    LockedPartitionRight( A, AL, AR, 0 );
    while( AR.Width() > 0 )
    {
        LockedRepartitionRight
        ( AL, /**/ AR,
          A0, /**/ A1, A2 );

        A1_MC_Star.AlignWith( C );
        A1_VR_Star.AlignWith( C );
        A1Trans_Star_MR.AlignWith( C );
        //--------------------------------------------------------------------//
        A1_VR_Star = A1_MC_Star = A1;
        A1Trans_Star_MR.TransposeFrom( A1_VR_Star );

        blas::internal::LocalTriangularRankK
        ( Upper, alpha, A1_MC_Star, A1Trans_Star_MR, (T)1, C ); 
        //--------------------------------------------------------------------//
        A1_MC_Star.FreeAlignments();
        A1_VR_Star.FreeAlignments();
        A1Trans_Star_MR.FreeAlignments();

        SlideLockedPartitionRight
        ( AL,     /**/ AR,
          A0, A1, /**/ A2 );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template void elemental::blas::internal::SyrkUN
( float alpha, const DistMatrix<float,MC,MR>& A,
  float beta,        DistMatrix<float,MC,MR>& C );

template void elemental::blas::internal::SyrkUN
( double alpha, const DistMatrix<double,MC,MR>& A,
  double beta,        DistMatrix<double,MC,MR>& C );

#ifndef WITHOUT_COMPLEX
template void elemental::blas::internal::SyrkUN
( scomplex alpha, const DistMatrix<scomplex,MC,MR>& A,
  scomplex beta,        DistMatrix<scomplex,MC,MR>& C );

template void elemental::blas::internal::SyrkUN
( dcomplex alpha, const DistMatrix<dcomplex,MC,MR>& A,
  dcomplex beta,        DistMatrix<dcomplex,MC,MR>& C );
#endif
