/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "elemental-lite.hpp"

namespace elem {

template<typename T,typename Int>
DistMatrix<T,STAR,STAR,Int>::DistMatrix( const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (0,0,false,false,0,0,0,0,0,0,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,STAR,Int>::DistMatrix
( Int height, Int width, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,false,false,0,0,0,0,height,width,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,STAR,Int>::DistMatrix
( Int height, Int width, Int ldim, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,false,false,0,0,0,0,height,width,ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,STAR,Int>::DistMatrix
( Int height, Int width, const T* buffer, Int ldim, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,0,0,0,0,height,width,buffer,ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,STAR,Int>::DistMatrix
( Int height, Int width, T* buffer, Int ldim, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,0,0,0,0,height,width,buffer,ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,STAR,Int>::DistMatrix( const DistMatrix<T,STAR,STAR,Int>& A )
: AbstractDistMatrix<T,Int>(0,0,false,false,0,0,0,0,0,0,A.Grid())
{
#ifndef RELEASE
    PushCallStack("DistMatrix[* ,* ]::DistMatrix");
#endif
    if( &A != this )
        *this = A;
    else
        throw std::logic_error("Tried to construct [* ,* ] with itself");
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
template<Distribution U,Distribution V>
DistMatrix<T,STAR,STAR,Int>::DistMatrix( const DistMatrix<T,U,V,Int>& A )
: AbstractDistMatrix<T,Int>(0,0,false,false,0,0,0,0,0,0,A.Grid())
{
#ifndef RELEASE
    PushCallStack("DistMatrix[* ,* ]::DistMatrix");
#endif
    if( STAR != U || STAR != V || 
        reinterpret_cast<const DistMatrix<T,STAR,STAR,Int>*>(&A) != this )
        *this = A;
    else
        throw std::logic_error("Tried to construct [* ,* ] with itself");
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
DistMatrix<T,STAR,STAR,Int>::~DistMatrix()
{ }

template<typename T,typename Int>
elem::DistData<Int>
DistMatrix<T,STAR,STAR,Int>::DistData() const
{
    elem::DistData<Int> data;
    data.colDist = STAR;
    data.rowDist = STAR;
    data.colAlignment = 0;
    data.rowAlignment = 0;
    data.diagPath = 0;
    data.grid = this->grid_;
    return data;
}

template<typename T,typename Int>
Int
DistMatrix<T,STAR,STAR,Int>::ColStride() const
{ return 1; }

template<typename T,typename Int>
Int
DistMatrix<T,STAR,STAR,Int>::RowStride() const
{ return 1; }

template<typename T,typename Int>
Int
DistMatrix<T,STAR,STAR,Int>::ColRank() const
{ return 0; }

template<typename T,typename Int>
Int
DistMatrix<T,STAR,STAR,Int>::RowRank() const
{ return 0; }

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::PrintBase
( std::ostream& os, const std::string msg ) const
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::PrintBase");
#endif
    const elem::Grid& g = this->Grid();
    if( g.Rank() == 0 && msg != "" )
        os << msg << std::endl;

    const Int height = this->Height();
    const Int width  = this->Width();

    if( height == 0 || width == 0 || !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    if( g.Rank() == 0 )
    {
        for( Int i=0; i<height; ++i )
        {
            for( Int j=0; j<width; ++j )
                os << this->GetLocal(i,j) << " ";
            os << "\n";
        }
        os << std::endl;
    }
    mpi::Barrier( g.Comm() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::Attach
( Int height, Int width, 
  T* buffer, Int ldim, const elem::Grid& grid )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::Attach");
#endif
    this->Empty();

    this->grid_ = &grid;
    this->height_ = height;
    this->width_ = width;
    this->viewing_ = true;
    if( this->Participating() )
        this->matrix_.Attach( height, width, buffer, ldim );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::LockedAttach
( Int height, Int width, 
  const T* buffer, Int ldim, const elem::Grid& grid )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::LockedAttach");
#endif
    this->Empty();

    this->grid_ = &grid;
    this->height_ = height;
    this->width_ = width;
    this->viewing_ = true;
    this->locked_ = true;
    if( this->Participating() )
        this->matrix_.LockedAttach( height, width, buffer, ldim );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::ResizeTo( Int height, Int width )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::ResizeTo");
    this->AssertNotLocked();
    if( height < 0 || width < 0 )
        throw std::logic_error("Height and width must be non-negative");
#endif
    this->height_ = height;
    this->width_ = width;
    if( this->Participating() )
        this->matrix_.ResizeTo( height, width );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
T
DistMatrix<T,STAR,STAR,Int>::Get( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::Get");
    this->AssertValidEntry( i, j );
#endif
    const Int viewingSize = mpi::CommSize( this->Grid().ViewingComm() );
    const Int owningSize = mpi::GroupSize( this->Grid().OwningGroup() );
    T u;
    if( viewingSize == owningSize )
    {
        // Everyone can just grab their own copy of the data
        u = this->GetLocal(i,j);
    }
    else
    {
        // Have the root broadcast its data
        if( this->Grid().VCRank() == 0 )
            u = this->GetLocal(i,j);
        mpi::Broadcast
        ( &u, 1, this->Grid().VCToViewingMap(0), 
          this->Grid().ViewingComm() );
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::Set( Int i, Int j, T u )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::Set");
    this->AssertValidEntry( i, j );
#endif
    if( this->Participating() )
        this->SetLocal(i,j,u);
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::Update( Int i, Int j, T u )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::Update");
    this->AssertValidEntry( i, j );
#endif
    if( this->Participating() )
        this->UpdateLocal(i,j,u);
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// Utility functions, e.g., operator=
//

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,MC,MR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [MC,MR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int r = g.Height();
    const Int c = g.Width(); 
    const Int p = g.Size();

    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeightOfA = A.LocalHeight();
    const Int localWidthOfA = A.LocalWidth();
    const Int maxLocalHeight = MaxLength(height,r);
    const Int maxLocalWidth = MaxLength(width,c);

    const Int portionSize = 
        std::max(maxLocalHeight*maxLocalWidth,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (p+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
    {
        const T* ACol = &ABuffer[jLocal*ALDim];
        T* originalDataCol = &originalData[jLocal*localHeightOfA];
        MemCopy( originalDataCol, ACol, localHeightOfA );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.VCComm() );

    // Unpack
    const Int colAlignmentOfA = A.ColAlignment();
    const Int rowAlignmentOfA = A.RowAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int l=0; l<c; ++l )
    {
        const Int rowShift = Shift_( l, rowAlignmentOfA, c );
        const Int localWidth = Length_( width, rowShift, c );

        for( Int k=0; k<r; ++k )
        {
            const T* data = &gatheredData[(k+l*r)*portionSize];

            const Int colShift = Shift_( k, colAlignmentOfA, r );
            const Int localHeight = Length_( height, colShift, r );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidth; ++jLocal )
            {
                T* destCol = &thisBuffer[colShift+(rowShift+jLocal*c)*thisLDim];
                const T* sourceCol = &data[jLocal*localHeight];
                for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                    destCol[iLocal*r] = sourceCol[iLocal];
            }
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,MC,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [MC,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int r = g.Height();
    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeightOfA = A.LocalHeight();
    const Int maxLocalHeight = MaxLength(height,r);

    const Int portionSize = std::max(maxLocalHeight*width,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (r+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j=0; j<width; ++j )
    {
        const T* ACol = &ABuffer[j*ALDim];
        T* originalDataCol = &originalData[j*localHeightOfA];
        MemCopy( originalDataCol, ACol, localHeightOfA );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.ColComm() );

    // Unpack
    const Int colAlignmentOfA = A.ColAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<r; ++k )
    {
        const T* data = &gatheredData[k*portionSize];

        const Int colShift = Shift_( k, colAlignmentOfA, r );
        const Int localHeight = Length_( height, colShift, r );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            T* destCol = &thisBuffer[colShift+j*thisLDim];
            const T* sourceCol = &data[j*localHeight];
            for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                destCol[iLocal*r] = sourceCol[iLocal];
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,STAR,MR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [* ,MR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int c = g.Width();
    const Int height = this->Height();
    const Int width = this->Width();
    const Int localWidthOfA = A.LocalWidth();
    const Int maxLocalWidth = MaxLength(width,c);

    const Int portionSize = std::max(height*maxLocalWidth,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (c+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
    {
        const T* ACol = &ABuffer[jLocal*ALDim];
        T* originalDataCol = &originalData[jLocal*height];
        MemCopy( originalDataCol, ACol, height );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.RowComm() );

    // Unpack
    const Int rowAlignmentOfA = A.RowAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<c; ++k )
    {
        const T* data = &gatheredData[k*portionSize];

        const Int rowShift = Shift_( k, rowAlignmentOfA, c );
        const Int localWidth = Length_( width, rowShift, c );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int jLocal=0; jLocal<localWidth; ++jLocal )
        {
            const T* dataCol = &data[jLocal*height];
            T* thisCol = &thisBuffer[(rowShift+jLocal*c)*thisLDim];
            MemCopy( thisCol, dataCol, height );
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,MD,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [MD,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int p = g.Size();
    const Int lcm = g.LCM();
    const Int ownerPath = A.diagPath_;
    const Int ownerPathRank = A.colAlignment_;

    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeight = A.LocalHeight();
    const Int maxLocalHeight = MaxLength( height, lcm );
    const Int portionSize = std::max( maxLocalHeight*width, mpi::MIN_COLL_MSG );

    // Since a MD communicator has not been implemented, we will take
    // the suboptimal route of 'rounding up' everyone's contribution over 
    // the VC communicator.
    this->auxMemory_.Require( (p+1)*portionSize );
    T* buffer = this->auxMemory_.Buffer();
    T* sendBuf = &buffer[0];
    T* recvBuf = &buffer[portionSize];

    // Pack
    if( A.Participating() )
    {
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            const T* ACol = &ABuffer[j*ALDim];
            T* sendBufCol = &sendBuf[j*localHeight];
            MemCopy( sendBufCol, ACol, localHeight );
        }
    }

    // Communicate
    mpi::AllGather
    ( sendBuf, portionSize,
      recvBuf, portionSize, g.VCComm() );

    // Unpack
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<p; ++k )
    {
        if( g.DiagPath( k ) == ownerPath )
        {
            const T* data = &recvBuf[k*portionSize];

            const Int thisPathRank = g.DiagPathRank( k );
            const Int thisColShift = 
                Shift_( thisPathRank, ownerPathRank, lcm );
            const Int thisLocalHeight = Length_( height, thisColShift, lcm );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int j=0; j<width; ++j )
            {
                T* destCol = &thisBuffer[thisColShift+j*thisLDim];
                const T* sourceCol = &data[j*thisLocalHeight];
                for( Int iLocal=0; iLocal<thisLocalHeight; ++iLocal )
                    destCol[iLocal*lcm] = sourceCol[iLocal];
            }
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,STAR,MD,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,* ] = [* ,MD]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int p = g.Size();
    const Int lcm = g.LCM();
    const Int ownerPath = A.diagPath_;
    const Int ownerPathRank = A.rowAlignment_;

    const Int height = this->Height();
    const Int width = this->Width();
    const Int localWidth = A.LocalWidth();
    const Int maxLocalWidth = MaxLength( width, lcm );
    const Int portionSize = std::max( height*maxLocalWidth, mpi::MIN_COLL_MSG );

    // Since a MD communicator has not been implemented, we will take
    // the suboptimal route of 'rounding up' everyone's contribution over 
    // the VC communicator.
    this->auxMemory_.Require( (p+1)*portionSize );
    T* buffer = this->auxMemory_.Buffer();
    T* sendBuf = &buffer[0];
    T* recvBuf = &buffer[portionSize];

    // Pack
    if( A.Participating() )
    {
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int jLocal=0; jLocal<localWidth; ++jLocal )
        {
            const T* ACol = &ABuffer[jLocal*ALDim];
            T* sendBufCol = &sendBuf[jLocal*height];
            MemCopy( sendBufCol, ACol, height );
        }
    }

    // Communicate
    mpi::AllGather
    ( sendBuf, portionSize,
      recvBuf, portionSize, g.VCComm() );

    // Unpack
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<p; ++k )
    {
        if( g.DiagPath( k ) == ownerPath )
        {
            const T* data = &recvBuf[k*portionSize];

            const Int thisPathRank = g.DiagPathRank( k );
            const Int thisRowShift = 
                Shift_( thisPathRank, ownerPathRank, lcm );
            const Int thisLocalWidth = Length_( width, thisRowShift, lcm );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<thisLocalWidth; ++jLocal )
            {
                const T* dataCol = &data[jLocal*height];
                T* thisCol = &thisBuffer[(thisRowShift+jLocal*lcm)*thisLDim];
                MemCopy( thisCol, dataCol, height );
            }
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,MR,MC,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [MR,MC]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int r = g.Height();
    const Int c = g.Width();
    const Int p = g.Size();

    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeightOfA = A.LocalHeight();
    const Int localWidthOfA = A.LocalWidth();
    const Int maxLocalHeight = MaxLength(height,c);
    const Int maxLocalWidth = MaxLength(width,r);

    const Int portionSize = 
        std::max(maxLocalHeight*maxLocalWidth,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (p+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
    {
        const T* ACol = &ABuffer[jLocal*ALDim];
        T* originalDataCol = &originalData[jLocal*localHeightOfA];
        MemCopy( originalDataCol, ACol, localHeightOfA );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.VRComm() );

    // Unpack
    const Int colAlignmentOfA = A.ColAlignment();
    const Int rowAlignmentOfA = A.RowAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int l=0; l<r; ++l )
    {
        const Int rowShift = Shift_( l, rowAlignmentOfA, r );
        const Int localWidth = Length_( width, rowShift, r );

        for( Int k=0; k<c; ++k )
        {
            const T* data = &gatheredData[(k+l*c)*portionSize];

            const Int colShift = Shift_( k, colAlignmentOfA, c );
            const Int localHeight = Length_( height, colShift, c );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidth; ++jLocal )
            {
                T* destCol = &thisBuffer[colShift+(rowShift+jLocal*r)*thisLDim];
                const T* sourceCol = &data[jLocal*localHeight];
                for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                    destCol[iLocal*c] = sourceCol[iLocal];
            }
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,MR,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [MR,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int c = g.Width();
    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeightOfA = A.LocalHeight();
    const Int maxLocalHeight = MaxLength(height,c);

    const Int portionSize = std::max(maxLocalHeight*width,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (c+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j=0; j<width; ++j )
    {
        const T* ACol = &ABuffer[j*ALDim];
        T* originalDataCol = &originalData[j*localHeightOfA];
        MemCopy( originalDataCol, ACol, localHeightOfA );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.RowComm() );

    // Unpack
    const Int colAlignmentOfA = A.ColAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<c; ++k )
    {
        const T* data = &gatheredData[k*portionSize];

        const Int colShift = Shift_( k, colAlignmentOfA, c );
        const Int localHeight = Length_( height, colShift, c );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            T* destCol = &thisBuffer[colShift+j*thisLDim];
            const T* sourceCol = &data[j*localHeight];
            for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                destCol[iLocal*c] = sourceCol[iLocal];
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,STAR,MC,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [* ,MC]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int r = g.Height();
    const Int height = this->Height();
    const Int width = this->Width();
    const Int localWidthOfA = A.LocalWidth();
    const Int maxLocalWidth = MaxLength(width,r);

    const Int portionSize = std::max(height*maxLocalWidth,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (r+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
    {
        const T* ACol = &ABuffer[jLocal*ALDim];
        T* originalDataCol = &originalData[jLocal*height];
        MemCopy( originalDataCol, ACol, height );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.ColComm() );

    // Unpack
    const Int rowAlignmentOfA = A.RowAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<r; ++k )
    {
        const T* data = &gatheredData[k*portionSize];

        const Int rowShift = Shift_( k, rowAlignmentOfA, r );
        const Int localWidth = Length_( width, rowShift, r );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int jLocal=0; jLocal<localWidth; ++jLocal )
        {
            const T* dataCol = &data[jLocal*height];
            T* thisCol = &thisBuffer[(rowShift+jLocal*r)*thisLDim];
            MemCopy( thisCol, dataCol, height );
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,VC,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [VC,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int p = g.Size();
    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeightOfA = A.LocalHeight();
    const Int maxLocalHeight = MaxLength(height,p);

    const Int portionSize = std::max(maxLocalHeight*width,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (p+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j=0; j<width; ++j )
    {
        const T* ACol = &ABuffer[j*ALDim];
        T* originalDataCol = &originalData[j*localHeightOfA];
        MemCopy( originalDataCol, ACol, localHeightOfA );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.VCComm() );

    // Unpack
    const Int colAlignmentOfA = A.ColAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<p; ++k )
    {
        const T* data = &gatheredData[k*portionSize];

        const Int colShift = Shift_( k, colAlignmentOfA, p );
        const Int localHeight = Length_( height, colShift, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for 
#endif
        for( Int j=0; j<width; ++j )
        {
            T* destCol = &thisBuffer[colShift+j*thisLDim];
            const T* sourceCol = &data[j*localHeight];
            for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                destCol[iLocal*p] = sourceCol[iLocal];
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,STAR,VC,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [* ,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int p = g.Size();
    const Int height = this->Height();
    const Int width = this->Width();
    const Int localWidthOfA = A.LocalWidth();
    const Int maxLocalWidth = MaxLength(width,p);

    const Int portionSize = std::max(height*maxLocalWidth,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (p+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
    {
        const T* ACol = &ABuffer[jLocal*ALDim];
        T* originalDataCol = &originalData[jLocal*height];
        MemCopy( originalDataCol, ACol, height );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.VCComm() );

    // Unpack
    const Int rowAlignmentOfA = A.RowAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<p; ++k )
    {
        const T* data = &gatheredData[k*portionSize];

        const Int rowShift = Shift_( k, rowAlignmentOfA, p );
        const Int localWidth = Length_( width, rowShift, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int jLocal=0; jLocal<localWidth; ++jLocal )
        {
            const T* dataCol = &data[jLocal*height];
            T* thisCol = &thisBuffer[(rowShift+jLocal*p)*thisLDim];
            MemCopy( thisCol, dataCol, height );
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,VR,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [VR,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int p = g.Size();
    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeightOfA = A.LocalHeight();
    const Int maxLocalHeight = MaxLength(height,p);

    const Int portionSize = std::max(maxLocalHeight*width,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (p+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j=0; j<width; ++j )
    {
        const T* ACol = &ABuffer[j*ALDim];
        T* originalDataCol = &originalData[j*localHeightOfA];
        MemCopy( originalDataCol, ACol, localHeightOfA );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.VRComm() );

    // Unpack
    const Int colAlignmentOfA = A.ColAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<p; ++k )
    {
        const T* data = &gatheredData[k*portionSize];

        const Int colShift = Shift_( k, colAlignmentOfA, p );
        const Int localHeight = Length_( height, colShift, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for 
#endif
        for( Int j=0; j<width; ++j )
        {
            T* destCol = &thisBuffer[colShift+j*thisLDim];
            const T* sourceCol = &data[j*localHeight];
            for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                destCol[iLocal*p] = sourceCol[iLocal];
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,STAR,VR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [* ,VR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int p = g.Size();
    const Int height = this->Height();
    const Int width = this->Width();
    const Int localWidthOfA = A.LocalWidth();
    const Int maxLocalWidth = MaxLength(width,p);

    const Int portionSize = std::max(height*maxLocalWidth,mpi::MIN_COLL_MSG);

    this->auxMemory_.Require( (p+1)*portionSize );

    T* buffer = this->auxMemory_.Buffer();
    T* originalData = &buffer[0];
    T* gatheredData = &buffer[portionSize];

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
    {
        const T* ACol = &ABuffer[jLocal*ALDim];
        T* originalDataCol = &originalData[jLocal*height];
        MemCopy( originalDataCol, ACol, height );
    }

    // Communicate
    mpi::AllGather
    ( originalData, portionSize,
      gatheredData, portionSize, g.VRComm() );

    // Unpack
    const Int rowAlignmentOfA = A.RowAlignment();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<p; ++k )
    {
        const T* data = &gatheredData[k*portionSize];

        const Int rowShift = Shift_( k, rowAlignmentOfA, p );
        const Int localWidth = Length_( width, rowShift, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int jLocal=0; jLocal<localWidth; ++jLocal )
        {
            const T* dataCol = &data[jLocal*height];
            T* thisCol = &thisBuffer[(rowShift+jLocal*p)*thisLDim];
            MemCopy( thisCol, dataCol, height );
        }
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,STAR,Int>&
DistMatrix<T,STAR,STAR,Int>::operator=( const DistMatrix<T,STAR,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,* ] = [* ,* ]");
    this->AssertNotLocked();
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );

    if( this->Grid() == A.Grid() )
    {
        this->matrix_ = A.LockedMatrix();
    }
    else
    {
        // TODO: Remember why I wrote this...
        if( !mpi::CongruentComms( A.Grid().ViewingComm(),
                                  this->Grid().ViewingComm() ) )
            throw std::logic_error
            ("Redistributing between nonmatching grids currently requires"
             " the viewing communicators to match.");

        // Compute and allocate the amount of required memory
        Int requiredMemory = 0;
        if( A.Grid().VCRank() == 0 )
            requiredMemory += A.Height()*A.Width();
        if( this->Participating() )
            requiredMemory += A.Height()*A.Width();
        this->auxMemory_.Require( requiredMemory );
        T* buffer = this->auxMemory_.Buffer();
        Int offset = 0;
        T* sendBuffer = &buffer[offset];
        if( A.Grid().VCRank() == 0 )
            offset += A.Height()*A.Width();
        T* bcastBuffer = &buffer[offset];

        // Send from the root of A to the root of this matrix's grid
        mpi::Request sendRequest;
        if( A.Grid().VCRank() == 0 )
        {
            for( Int j=0; j<A.Width(); ++j ) 
                for( Int i=0; i<A.Height(); ++i )
                    sendBuffer[i+j*A.Height()] = A.GetLocal(i,j);
            const Int recvViewingRank = this->Grid().VCToViewingMap(0);
            mpi::ISend
            ( sendBuffer, A.Height()*A.Width(), recvViewingRank, 0,
              this->Grid().ViewingComm(), sendRequest );
        }

        // Receive on the root of this matrix's grid and then broadcast
        // over this matrix's owning communicator
        if( this->Participating() )
        {
            if( this->Grid().VCRank() == 0 )
            {
                const Int sendViewingRank = A.Grid().VCToViewingMap(0);
                mpi::Recv
                ( bcastBuffer, A.Height()*A.Width(), sendViewingRank, 0,
                  this->Grid().ViewingComm() );
            }

            mpi::Broadcast
            ( bcastBuffer, A.Height()*A.Width(), 0, this->Grid().VCComm() );

            for( Int j=0; j<A.Width(); ++j )
                for( Int i=0; i<A.Height(); ++i )
                    this->SetLocal(i,j,bcastBuffer[i+j*A.Height()]);
        }

        if( A.Grid().VCRank() == 0 )
            mpi::Wait( sendRequest );
        this->auxMemory_.Release();
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::SumOverCol()
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::SumOverCol");
    this->AssertNotLocked();
#endif
    const Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int localHeight = this->LocalHeight();
    const Int localWidth = this->LocalWidth();
    const Int localSize = localHeight*localWidth;
    this->auxMemory_.Require( 2*localSize );
    T* buffer = this->auxMemory_.Buffer();
    T* sendBuf = &buffer[0];
    T* recvBuf = &buffer[localSize];

    // Pack
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidth; ++jLocal )
    {
        const T* thisCol = &thisBuffer[jLocal*thisLDim];
        T* sendBufCol = &sendBuf[jLocal*localHeight];
        MemCopy( sendBufCol, thisCol, localHeight );
    }

    // Sum
    mpi::AllReduce( sendBuf, recvBuf, localSize, mpi::SUM, g.ColComm() );

    // Unpack
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidth; ++jLocal )
    {
        const T* recvBufCol = &recvBuf[jLocal*localHeight];
        T* thisCol = &thisBuffer[jLocal*thisLDim];
        MemCopy( thisCol, recvBufCol, localHeight );
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::SumOverRow()
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::SumOverRow");
    this->AssertNotLocked();
#endif
    const Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int localHeight = this->LocalHeight();
    const Int localWidth = this->LocalWidth();
    const Int localSize = localHeight*localWidth;
    this->auxMemory_.Require( 2*localSize );
    T* buffer = this->auxMemory_.Buffer();
    T* sendBuf = &buffer[0];
    T* recvBuf = &buffer[localSize];

    // Pack
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidth; ++jLocal )
    {
        const T* thisCol = &thisBuffer[jLocal*thisLDim];
        T* sendBufCol = &sendBuf[jLocal*localHeight];
        MemCopy( sendBufCol, thisCol, localHeight );
    }

    // Sum
    mpi::AllReduce( sendBuf, recvBuf, localSize, mpi::SUM, g.RowComm() );

    // Unpack
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidth; ++jLocal )
    {
        const T* recvBufCol = &recvBuf[jLocal*localHeight];
        T* thisCol = &thisBuffer[jLocal*thisLDim];
        MemCopy( thisCol, recvBufCol, localHeight );
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::SumOverGrid()
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::SumOverGrid");
    this->AssertNotLocked();
#endif
    const Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int localHeight = this->LocalHeight();
    const Int localWidth = this->LocalWidth();
    const Int localSize = localHeight*localWidth;
    this->auxMemory_.Require( 2*localSize );
    T* buffer = this->auxMemory_.Buffer();
    T* sendBuf = &buffer[0];
    T* recvBuf = &buffer[localSize];

    // Pack
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidth; ++jLocal )
    {
        const T* thisCol = &thisBuffer[jLocal*thisLDim];
        T* sendBufCol = &sendBuf[jLocal*localHeight];
        MemCopy( sendBufCol, thisCol, localHeight );
    }

    // Sum
    mpi::AllReduce( sendBuf, recvBuf, localSize, mpi::SUM, g.VCComm() );

    // Unpack
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidth; ++jLocal )
    {
        const T* recvBufCol = &recvBuf[jLocal*localHeight];
        T* thisCol = &thisBuffer[jLocal*thisLDim];
        MemCopy( thisCol, recvBufCol, localHeight );
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// Routines which explicitly work in the complex plane
//

template<typename T,typename Int>
typename Base<T>::type
DistMatrix<T,STAR,STAR,Int>::GetRealPart( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::GetRealPart");
    this->AssertValidEntry( i, j );
#endif
    typedef typename Base<T>::type R;

    const Grid& g = this->Grid();
    const Int viewingSize = mpi::CommSize( g.ViewingComm() );
    const Int owningSize = mpi::GroupSize( g.OwningGroup() );

    R u;
    if( viewingSize == owningSize )
    {
        // Everyone can just grab their own copy of the data
        u = this->GetLocalRealPart(i,j);
    }
    else
    {
        // Have the root broadcast its data
        if( g.VCRank() == 0 )
            u = this->GetLocalRealPart(i,j);
        mpi::Broadcast( &u, 1, g.VCToViewingMap(0), g.ViewingComm() );
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
typename Base<T>::type
DistMatrix<T,STAR,STAR,Int>::GetImagPart( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::GetImagPart");
    this->AssertValidEntry( i, j );
#endif
    typedef typename Base<T>::type R;

    const Grid& g = this->Grid();
    const Int viewingSize = mpi::CommSize( g.ViewingComm() );
    const Int owningSize = mpi::GroupSize( g.OwningGroup() );

    R u;
    if( viewingSize == owningSize )
    {
        // Everyone can just grab their own copy of the data
        u = this->GetLocalImagPart(i,j);
    }
    else
    {
        // Have the root broadcast its data
        if( g.VCRank() == 0 )
            u = this->GetLocalImagPart(i,j);
        mpi::Broadcast( &u, 1, g.VCToViewingMap(0), g.ViewingComm() );
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::SetRealPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::SetRealPart");
    this->AssertValidEntry( i, j );
#endif
    if( this->Participating() )
        this->SetLocalRealPart(i,j,u);
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::SetImagPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::SetImagPart");
    this->AssertValidEntry( i, j );
#endif
    if( !IsComplex<T>::val )
        throw std::logic_error("Called complex-only routine with real data");
    if( this->Participating() )
        this->SetLocalImagPart(i,j,u);
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::UpdateRealPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::UpdateRealPart");
    this->AssertValidEntry( i, j );
#endif
    if( this->Participating() )
        this->UpdateLocalRealPart(i,j,u);
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,STAR,Int>::UpdateImagPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[* ,* ]::UpdateImagPart");
    this->AssertValidEntry( i, j );
#endif
    if( !IsComplex<T>::val )
        throw std::logic_error("Called complex-only routine with real data");
    if( this->Participating() )
        this->UpdateLocalImagPart(i,j,u);
#ifndef RELEASE
    PopCallStack();
#endif
}

template class DistMatrix<int,STAR,STAR,int>;
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,MC,  MR,  int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,MC,  STAR,int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,MD,  STAR,int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,MR,  MC,  int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,MR,  STAR,int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,STAR,MC,  int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,STAR,MD,  int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,STAR,MR,  int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,STAR,VC,  int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,STAR,VR,  int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,VC,  STAR,int>& A );
template DistMatrix<int,STAR,STAR,int>::DistMatrix( const DistMatrix<int,VR,  STAR,int>& A );

#ifndef DISABLE_FLOAT
template class DistMatrix<float,STAR,STAR,int>;
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,MC,  MR,  int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,MC,  STAR,int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,MD,  STAR,int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,MR,  MC,  int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,MR,  STAR,int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,STAR,MC,  int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,STAR,MD,  int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,STAR,MR,  int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,STAR,VC,  int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,STAR,VR,  int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,VC,  STAR,int>& A );
template DistMatrix<float,STAR,STAR,int>::DistMatrix( const DistMatrix<float,VR,  STAR,int>& A );
#endif // ifndef DISABLE_FLOAT

template class DistMatrix<double,STAR,STAR,int>;
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,MC,  MR,  int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,MC,  STAR,int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,MD,  STAR,int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,MR,  MC,  int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,MR,  STAR,int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,STAR,MC,  int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,STAR,MD,  int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,STAR,MR,  int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,STAR,VC,  int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,STAR,VR,  int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,VC,  STAR,int>& A );
template DistMatrix<double,STAR,STAR,int>::DistMatrix( const DistMatrix<double,VR,  STAR,int>& A );

#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
template class DistMatrix<Complex<float>,STAR,STAR,int>;
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MC,  MR,  int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MC,  STAR,int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MD,  STAR,int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MR,  MC,  int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MR,  STAR,int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,MC,  int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,MD,  int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,MR,  int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,VC,  int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,VR,  int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,VC,  STAR,int>& A );
template DistMatrix<Complex<float>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,VR,  STAR,int>& A );
#endif // ifndef DISABLE_FLOAT
template class DistMatrix<Complex<double>,STAR,STAR,int>;
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MC,  MR,  int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MC,  STAR,int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MD,  STAR,int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MR,  MC,  int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MR,  STAR,int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,MC,  int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,MD,  int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,MR,  int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,VC,  int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,VR,  int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,VC,  STAR,int>& A );
template DistMatrix<Complex<double>,STAR,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,VR,  STAR,int>& A );
#endif // ifndef DISABLE_COMPLEX

} // namespace elem
