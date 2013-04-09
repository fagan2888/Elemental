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
// RepartitionUp
//

template<typename Int> inline
void RepartitionUp__
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2, Int A1Height, bool lock )
{
    A1Height = std::min(A1Height,AT.Height());
    const Int offset = AT.Height()-A1Height; 
    View__( A0, AT, 0,      0, offset,   AT.Width(), lock );
    View__( A1, AT, offset, 0, A1Height, AT.Width(), lock );
    View__( A2, AB, lock );
}

template<typename Int> inline
void RepartitionUp_
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2, Int A1Height, bool lock )
{
    PushCallStack("RepartitionUp [Matrix]");
    AssertNonnegative( A1Height, "height of A1 block" );
    AT.AssertDataTypes( AB );
    AT.AssertDataTypes( A0 );
    AT.AssertDataTypes( A1 );
    AT.AssertDataTypes( A2 );
    if ( !lock ) {
    	AT.AssertUnlocked( AM::PartitionLock );
    	AB.AssertUnlocked( AM::PartitionLock );
    }
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionUp
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2, Int A1Height )
{
#ifndef RELEASE
    PushCallStack("RepartitionUp [DistMatrix]");
    if( (AT.Matrix().Buffer() + AT.LocalHeight()) != AB.Matrix().Buffer() )
        throw std::logic_error
        ("Noncontiguous 2x1 array of distributed matrices");
#endif
    A1Height = std::min(A1Height,AT.Height());
    const Int offset = AT.Height()-A1Height; 
    View( A0, AT, 0,      0, offset,   AT.Width() );
    View( A1, AT, offset, 0, A1Height, AT.Width() );
    View( A2, AB );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionUp
( const DM& AT, DM& A0,
                DM& A1,
  const DM& AB, DM& A2, Int A1Height )
{
#ifndef RELEASE
    PushCallStack("LockedRepartitionUp [DistMatrix]");
    if( (AT.LockedMatrix().LockedBuffer() + AT.LocalHeight()) != 
         AB.LockedMatrix().LockedBuffer() )
        throw std::logic_error
        ("Noncontiguous 2x1 array of distributed matrices");
#endif
    A1Height = std::min(A1Height,AT.Height());
    const Int offset = AT.Height()-A1Height;
    LockedView( A0, AT, 0,      0, offset,   AT.Width() );
    LockedView( A1, AT, offset, 0, A1Height, AT.Width() );
    LockedView( A2, AB );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// RepartitionDown
//

template<typename Int>
inline void RepartitionDown__
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2, Int A1Height, bool lock )
{
    A1Height = std::min(A1Height,AB.Height());
    const Int offset = AB.Height()-A1Height; 
    View__( A0, AT, lock );
    View__( A1, AB, 0,        0, A1Height, AB.Width(), lock );
    View__( A2, AB, A1Height, 0, offset,   AB.Width(), lock );
}
  
template<typename Int>
inline void RepartitionDown_
( const AM& AT, AM& A0,
                AM& A1,
  const AM& AB, AM& A2, Int A1Height, bool lock )
{
    PushCallStack("RepartitionDown [Matrix]");
    AssertNonnegative( A1Height, "height of A1 block" );
    AT.AssertDataTypes( AB );
    AT.AssertDataTypes( A0 );
    AT.AssertDataTypes( A1 );
    AT.AssertDataTypes( A2 );
    if ( !lock ) {
    	AT.AssertUnlocked( AM::PartitionLock );
    	AB.AssertUnlocked( AM::PartitionLock );
    }
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionDown
( DM& AT, DM& A0,
          DM& A1,
  DM& AB, DM& A2, Int A1Height )
{
#ifndef RELEASE
    PushCallStack("RepartitionDown [DistMatrix]");
    if( (AT.Matrix().Buffer() + AT.LocalHeight()) != 
         AB.Matrix().Buffer() )
        throw std::logic_error
        ("Noncontiguous 2x1 array of distributed matrices");
#endif
    A1Height = std::min(A1Height,AB.Height());
    const Int offset = AB.Height()-A1Height; 
    View( A0, AT );
    View( A1, AB, 0,        0, A1Height, AB.Width() );
    View( A2, AB, A1Height, 0, offset, AB.Width() );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionDown
( const DM& AT, DM& A0,
                DM& A1,
  const DM& AB, DM& A2, Int A1Height )
{
#ifndef RELEASE
    PushCallStack("LockedRepartitionDown [DistMatrix]");
    if( (AT.LockedMatrix().LockedBuffer() + AT.LocalHeight()) != 
         AB.LockedMatrix().LockedBuffer() )
        throw std::logic_error
        ("Noncontiguous 2x1 array of distributed matrices");
#endif
    A1Height = std::min(A1Height,AB.Height());
    const Int offset = AB.Height()-A1Height;
    LockedView( A0, AT );
    LockedView( A1, AB, 0,        0, A1Height, AB.Width() );
    LockedView( A2, AB, A1Height, 0, offset,   AB.Width() );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// RepartitionLeft
//

template<typename Int>
inline void
RepartitionLeft__
( const AM& AL, const AM& AR,
  AM& A0, AM& A1, AM& A2, Int A1Width, bool lock )
{
    A1Width = std::min(A1Width,AL.Width());
    const Int offset = AL.Width()-A1Width;
    View__( A0, AL, 0, 0,      AL.Height(), offset , lock );
    View__( A1, AL, 0, offset, AL.Height(), A1Width, lock );
    View__( A2, AR, lock );
}

template<typename Int>
inline void
RepartitionLeft_
( const AM& AL, const AM& AR,
  AM& A0, AM& A1, AM& A2, Int A1Width, bool lock )
{
    PushCallStack("RepartitionLeft [Matrix]");
    AssertNonnegative( A1Width, "width of A1 block" );
    AL.AssertDataTypes( AL );
    AL.AssertDataTypes( A0 );
    AL.AssertDataTypes( A1 );
    AL.AssertDataTypes( A2 );
    if ( !lock ) {
    	AL.AssertUnlocked( AM::PartitionLock );
    	AR.AssertUnlocked( AM::PartitionLock );
    }
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionLeft
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2, Int A1Width )
{
#ifndef RELEASE
    PushCallStack("RepartitionLeft [DistMatrix]");
    if( (AL.Matrix().Buffer() + AL.LocalWidth()*AL.LDim()) !=
         AR.Matrix().Buffer() )
        throw std::logic_error
        ("Noncontiguous 1x2 array of distributed matrices");
#endif
    A1Width = std::min(A1Width,AL.Width());
    const Int offset = AL.Width()-A1Width;
    View( A0, AL, 0, 0,      AL.Height(), offset  );
    View( A1, AL, 0, offset, AL.Height(), A1Width );
    View( A2, AR );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionLeft
( const DM& AL, const DM& AR,
  DM& A0, DM& A1, DM& A2, Int A1Width )
{
#ifndef RELEASE
    PushCallStack("LockedRepartitionLeft [DistMatrix]");
    if( (AL.LockedMatrix().LockedBuffer() + AL.LocalWidth()*AL.LDim()) !=
         AR.LockedMatrix().LockedBuffer() )
        throw std::logic_error
        ("Noncontiguous 1x1 array of distributed matrices");
#endif
    A1Width = std::min(A1Width,AL.Width());
    const Int offset = AL.Width()-A1Width;
    LockedView( A0, AL, 0, 0,      AL.Height(), offset  );
    LockedView( A1, AL, 0, offset, AL.Height(), A1Width );
    LockedView( A2, AR );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// RepartitionRight
//

template<typename Int>
inline void
RepartitionRight__
( const AM& AL, const AM& AR,
  AM& A0, AM& A1, AM& A2, Int A1Width, bool lock )
{
    A1Width = std::min(A1Width,AR.Width());
    const Int offset = AR.Width()-A1Width;
    View__( A0, AL, lock );
    View__( A1, AR, 0, 0,       AR.Height(), A1Width, lock );
    View__( A2, AR, 0, A1Width, AR.Height(), offset , lock );
}

template<typename Int>
inline void
RepartitionRight_
( const AM& AL, const AM& AR,
  AM& A0, AM& A1, AM& A2, Int A1Width, bool lock )
{
    PushCallStack("RepartitionRight [Matrix]");
    AssertNonnegative( A1Width, "width of A1 block" );
    AL.AssertDataTypes( AL );
    AL.AssertDataTypes( A0 );
    AL.AssertDataTypes( A1 );
    AL.AssertDataTypes( A2 );
    if ( !lock ) {
    	AL.AssertUnlocked( AM::PartitionLock );
    	AR.AssertUnlocked( AM::PartitionLock );
    }
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionRight
( DM& AL, DM& AR,
  DM& A0, DM& A1, DM& A2, Int A1Width )
{
#ifndef RELEASE
    PushCallStack("RepartitionRight [DistMatrix]");
    if( (AL.Matrix().Buffer() + AL.LocalWidth()*AL.LDim()) !=
         AR.Matrix().Buffer() )
        throw std::logic_error
        ("Noncontiguous 1x2 array of distributed matrices");
#endif
    A1Width = std::min(A1Width,AR.Width());
    const Int offset = AR.Width()-A1Width;
    View( A0, AL );
    View( A1, AR, 0, 0,       AR.Height(), A1Width );
    View( A2, AR, 0, A1Width, AR.Height(), offset  );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionRight
( const DM& AL, const DM& AR,
  DM& A0, DM& A1, DM& A2, Int A1Width )
{
#ifndef RELEASE
    PushCallStack("LockedRepartitionRight [DistMatrix]");
    if( (AL.LockedMatrix().LockedBuffer() + AL.LocalWidth()*AL.LDim()) !=
         AR.LockedMatrix().LockedBuffer() )
        throw std::logic_error
        ("Noncontiguous 1x2 DistMatrices in LockedRepartitionRight");
#endif
    A1Width = std::min(A1Width,AR.Width());
    const Int offset = AR.Width()-A1Width;
    LockedView( A0, AL );
    LockedView( A1, AR, 0, 0,       AR.Height(), A1Width );
    LockedView( A2, AR, 0, A1Width, AR.Height(), offset  );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// RepartitionUpDiagonal
//

template<typename Int>
inline void
RepartitionUpDiagonal__
( const AM& ATL, const AM& ATR, AM& A00, AM& A01, AM& A02,
                       AM& A10, AM& A11, AM& A12,
  const AM& ABL, const AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize, bool lock )
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

template<typename Int>
inline void
RepartitionUpDiagonal_
( const AM& ATL, const AM& ATR, AM& A00, AM& A01, AM& A02,
                       AM& A10, AM& A11, AM& A12,
  const AM& ABL, const AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize, bool lock )
{
    PushCallStack("RepartitionUpDiagonal [Matrix]");
    ATL.AssertDataTypes( ATR );
    ATL.AssertDataTypes( ABL );
    ATL.AssertDataTypes( ABR );
    ATL.AssertDataTypes( A00 );
    ATL.AssertDataTypes( A01 );
    ATL.AssertDataTypes( A02 );
    ATL.AssertDataTypes( A10 );
    ATL.AssertDataTypes( A11 );
    ATL.AssertDataTypes( A12 );
    ATL.AssertDataTypes( A20 );
    ATL.AssertDataTypes( A21 );
    ATL.AssertDataTypes( A22 );
    if ( !lock ) {
    	ATL.AssertUnlocked( AM::PartitionLock );
    	ATR.AssertUnlocked( AM::PartitionLock );
    	ABL.AssertUnlocked( AM::PartitionLock );
    	ABR.AssertUnlocked( AM::PartitionLock );
    }
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionUpDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize )
{
#ifndef RELEASE
    PushCallStack("RepartitionUpDiagonal [DistMatrix]");
    if( (ATL.Matrix().Buffer() + ATL.LocalHeight()) != 
         ABL.Matrix().Buffer() ||
        (ATR.Matrix().Buffer() + ATR.LocalHeight()) != 
         ABR.Matrix().Buffer() ||
        (ATL.Matrix().Buffer() + ATL.LocalWidth()*ATL.LDim()) !=
         ATR.Matrix().Buffer() ||
        (ABL.Matrix().Buffer() + ABL.LocalWidth()*ABL.LDim()) !=
         ABR.Matrix().Buffer() )
        throw std::logic_error
        ("Noncontiguous 2x2 grid of distributed matrices");
#endif
    bsize = std::min(bsize,std::min(ATL.Height(),ATL.Width()));
    const Int vOffset = ATL.Height()-bsize;
    const Int hOffset = ATL.Width()-bsize;
    View( A00, ATL, 0,       0,       vOffset,      hOffset     );
    View( A01, ATL, 0,       hOffset, vOffset,      bsize       );
    View( A02, ATR, 0,       0,       vOffset,      ATR.Width() );
    View( A10, ATL, vOffset, 0,       bsize,        hOffset     );
    View( A11, ATL, vOffset, hOffset, bsize,        bsize       );
    View( A12, ATR, vOffset, 0,       bsize,        ATR.Width() );
    View( A20, ABL, 0,       0,       ABL.Height(), hOffset     );
    View( A21, ABL, 0,       hOffset, ABL.Height(), bsize       );
    View( A22, ABR );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionUpDiagonal
( const DM& ATL, const DM& ATR, DM& A00, DM& A01, DM& A02,
                                DM& A10, DM& A11, DM& A12,
  const DM& ABL, const DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize )
{
#ifndef RELEASE
    PushCallStack("LockedRepartitionUpDiagonal [DistMatrix]");
    if( (ATL.LockedMatrix().LockedBuffer()+ATL.LocalHeight()) != 
         ABL.LockedMatrix().LockedBuffer() ||
        (ATR.LockedMatrix().LockedBuffer()+ATR.LocalHeight()) != 
         ABR.LockedMatrix().LockedBuffer() ||
        (ATL.LockedMatrix().LockedBuffer()+
         ATL.LocalWidth()*ATL.LDim()) != 
         ATR.LockedMatrix().LockedBuffer() ||
        (ABL.LockedMatrix().LockedBuffer()+
         ABL.LocalWidth()*ABL.LDim()) !=
         ABR.LockedMatrix().LockedBuffer() )
        throw std::logic_error
        ("Noncontiguous 2x2 grid of distributed matrices");
#endif
    bsize = std::min(bsize,std::min(ATL.Height(),ATL.Width()));
    const Int vOffset = ATL.Height()-bsize;
    const Int hOffset = ATL.Width()-bsize;
    LockedView( A00, ATL, 0,       0,       vOffset,      hOffset     );
    LockedView( A01, ATL, 0,       hOffset, vOffset,      bsize       );
    LockedView( A02, ATR, 0,       0,       vOffset,      ATR.Width() );
    LockedView( A10, ATL, vOffset, 0,       bsize,        hOffset     );
    LockedView( A11, ATL, vOffset, hOffset, bsize,        bsize       );
    LockedView( A12, ATR, vOffset, 0,       bsize,        ATR.Width() );
    LockedView( A20, ABL, 0,       0,       ABL.Height(), hOffset     );
    LockedView( A21, ABL, 0,       hOffset, ABL.Height(), bsize       );
    LockedView( A22, ABR );
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// RepartitionDownDiagonal
//

template<typename Int>
inline void
RepartitionDownDiagonal__
( const AM& ATL, const AM& ATR, AM& A00, AM& A01, AM& A02,
                                AM& A10, AM& A11, AM& A12,
  const AM& ABL, const AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize, bool lock )
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

template<typename Int>
inline void
RepartitionDownDiagonal_
( const AM& ATL, const AM& ATR, AM& A00, AM& A01, AM& A02,
                       AM& A10, AM& A11, AM& A12,
  const AM& ABL, const AM& ABR, AM& A20, AM& A21, AM& A22, Int bsize, bool lock )
{
    PushCallStack("RepartitionDownDiagonal [Matrix]");
    ATL.AssertDataTypes( ATR );
    ATL.AssertDataTypes( ABL );
    ATL.AssertDataTypes( ABR );
    ATL.AssertDataTypes( A00 );
    ATL.AssertDataTypes( A01 );
    ATL.AssertDataTypes( A02 );
    ATL.AssertDataTypes( A10 );
    ATL.AssertDataTypes( A11 );
    ATL.AssertDataTypes( A12 );
    ATL.AssertDataTypes( A20 );
    ATL.AssertDataTypes( A21 );
    ATL.AssertDataTypes( A22 );
    if ( !lock ) {
    	ATL.AssertUnlocked( AM::PartitionLock );
    	ATR.AssertUnlocked( AM::PartitionLock );
    	ABL.AssertUnlocked( AM::PartitionLock );
    	ABR.AssertUnlocked( AM::PartitionLock );
    }
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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
RepartitionDownDiagonal
( DM& ATL, DM& ATR, DM& A00, DM& A01, DM& A02,
                    DM& A10, DM& A11, DM& A12,
  DM& ABL, DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize )
{
#ifndef RELEASE
    PushCallStack("RepartitionDownDiagonal [DistMatrix]");
    if( (ATL.Matrix().Buffer() + ATL.LocalHeight()) != 
         ABL.Matrix().Buffer() ||
        (ATR.Matrix().Buffer() + ATR.LocalHeight()) != 
         ABR.Matrix().Buffer() ||
        (ATL.Matrix().Buffer() + ATL.LocalWidth()*ATL.LDim()) !=
         ATR.Matrix().Buffer() ||
        (ABL.Matrix().Buffer() + ABL.LocalWidth()*ABL.LDim()) != 
         ABR.Matrix().Buffer() )
        throw std::logic_error
        ("Noncontiguous 2x2 grid of distributed matrices");
#endif
    bsize = std::min(bsize,std::min(ABR.Height(),ABR.Width()));
    const Int vOffset = ABR.Height()-bsize;
    const Int hOffset = ABR.Width()-bsize;
    View( A00, ATL );
    View( A01, ATR, 0,     0,     ATL.Height(), bsize       );
    View( A02, ATR, 0,     bsize, ATL.Height(), hOffset     );
    View( A10, ABL, 0,     0,     bsize,        ABL.Width() );
    View( A11, ABR, 0,     0,     bsize,        bsize       );
    View( A12, ABR, 0,     bsize, bsize,        hOffset     );
    View( A20, ABL, bsize, 0,     vOffset,      ABL.Width() );
    View( A21, ABR, bsize, 0,     vOffset,      bsize       );
    View( A22, ABR, bsize, bsize, vOffset,      hOffset     );
#ifndef RELEASE
    PopCallStack();
#endif
}

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

template<typename T,Distribution U,Distribution V,typename Int>
inline void
LockedRepartitionDownDiagonal
( const DM& ATL, const DM& ATR, DM& A00, DM& A01, DM& A02,
                                DM& A10, DM& A11, DM& A12,
  const DM& ABL, const DM& ABR, DM& A20, DM& A21, DM& A22, Int bsize )
{
#ifndef RELEASE
    PushCallStack("LockedRepartitionDownDiagonal [DistMatrix]");
    if( (ATL.LockedMatrix().LockedBuffer()+ATL.LocalHeight()) != 
         ABL.LockedMatrix().LockedBuffer() ||
        (ATR.LockedMatrix().LockedBuffer()+ATR.LocalHeight()) != 
         ABR.LockedMatrix().LockedBuffer() ||
        (ATL.LockedMatrix().LockedBuffer()+
         ATL.LocalWidth()*ATL.LDim()) !=
         ATR.LockedMatrix().LockedBuffer() ||
        (ABL.LockedMatrix().LockedBuffer()+
         ABL.LocalWidth()*ABL.LDim()) !=
         ABR.LockedMatrix().LockedBuffer() )
        throw std::logic_error
        ("Noncontiguous 2x2 grid of distributed matrices");
#endif
    bsize = std::min(bsize,std::min(ABR.Height(),ABR.Width()));
    const Int vOffset = ABR.Height()-bsize;
    const Int hOffset = ABR.Width()-bsize;
    LockedView( A00, ATL );
    LockedView( A01, ATR, 0,     0,     ATR.Height(), bsize  );
    LockedView( A02, ATR, 0,     bsize, ATR.Height(), hOffset     );
    LockedView( A10, ABL, 0,     0,     bsize,        ABL.Width() );
    LockedView( A11, ABR, 0,     0,     bsize,        bsize       );
    LockedView( A12, ABR, 0,     bsize, bsize,        hOffset     );
    LockedView( A20, ABL, bsize, 0,     vOffset,      ABL.Width() );
    LockedView( A21, ABR, bsize, 0,     vOffset,      bsize       );
    LockedView( A22, ABR, bsize, bsize, vOffset,      hOffset     );
#ifndef RELEASE
    PopCallStack();
#endif
}

#undef AM
#undef DM
#undef M

} // namespace elem

#endif // ifndef CORE_REPARTITION_IMPL_HPP
