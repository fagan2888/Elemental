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
DistMatrix<T,STAR,MR,Int>::DistMatrix( const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (0,0,false,false,0,0,
   0,(g.InGrid() ? g.Col() : 0 ),
   0,0,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,MR,Int>::DistMatrix
( Int height, Int width, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,false,false,0,0,
   0,(g.InGrid() ? g.Col() : 0),
   height,(g.InGrid() ? Length(width,g.Col(),0,g.Width()) : 0),
   g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,MR,Int>::DistMatrix
( Int height, Int width, Int rowAlignment, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,false,true,0,rowAlignment,
   0,(g.InGrid() ? Shift(g.Col(),rowAlignment,g.Width()) : 0),
   height,
   (g.InGrid() ? Length(width,g.Col(),rowAlignment,g.Width()) : 0),
   g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,MR,Int>::DistMatrix
( Int height, Int width, Int rowAlignment, Int ldim, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,false,true,0,rowAlignment,
   0,(g.InGrid() ? Shift(g.Col(),rowAlignment,g.Width()) : 0),
   height,
   (g.InGrid() ? Length(width,g.Col(),rowAlignment,g.Width()) : 0),
   ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,MR,Int>::DistMatrix
( Int height, Int width, Int rowAlignment, const T* buffer, Int ldim, 
  const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,0,rowAlignment,
   0,(g.InGrid() ? Shift(g.Col(),rowAlignment,g.Width()) : 0),
   height,
   (g.InGrid() ? Length(width,g.Col(),rowAlignment,g.Width()) : 0),
   buffer,ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,MR,Int>::DistMatrix
( Int height, Int width, Int rowAlignment, T* buffer, Int ldim, 
  const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,0,rowAlignment,
   0,(g.InGrid() ? Shift(g.Col(),rowAlignment,g.Width()) : 0),
   height,
   (g.InGrid() ? Length(width,g.Col(),rowAlignment,g.Width()) : 0),
   buffer,ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,STAR,MR,Int>::DistMatrix( const DistMatrix<T,STAR,MR,Int>& A )
: AbstractDistMatrix<T,Int>(0,0,false,false,0,0,
  0,(A.Participating() ? A.RowRank() : 0),
  0,0,A.Grid())
{
#ifndef RELEASE
    PushCallStack("DistMatrix[* ,MR]::DistMatrix");
#endif
    if( &A != this )
        *this = A;
    else
        throw std::logic_error("Tried to construct [* ,MR] with itself");
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
template<Distribution U,Distribution V>
DistMatrix<T,STAR,MR,Int>::DistMatrix( const DistMatrix<T,U,V,Int>& A )
: AbstractDistMatrix<T,Int>(0,0,false,false,0,0,
  0,(A.Participating() ? A.RowRank() : 0),
  0,0,A.Grid())
{
#ifndef RELEASE
    PushCallStack("DistMatrix[* ,MR]::DistMatrix");
#endif
    if( STAR != U || MR != V || 
        reinterpret_cast<const DistMatrix<T,STAR,MR,Int>*>(&A) != this )   
        *this = A;
    else
        throw std::logic_error("Tried to construct [* ,MR] with itself");
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
DistMatrix<T,STAR,MR,Int>::~DistMatrix()
{ }

template<typename T,typename Int>
elem::DistData<Int>
DistMatrix<T,STAR,MR,Int>::DistData() const
{
    elem::DistData<Int> data;
    data.colDist = STAR;
    data.rowDist = MR;
    data.colAlignment = 0;
    data.rowAlignment = this->rowAlignment_;
    data.diagPath = 0;
    data.grid = this->grid_;
    return data;
}

template<typename T,typename Int>
Int
DistMatrix<T,STAR,MR,Int>::ColStride() const
{ return 1; }

template<typename T,typename Int>
Int
DistMatrix<T,STAR,MR,Int>::RowStride() const
{ return this->grid_->Width(); }

template<typename T,typename Int>
Int
DistMatrix<T,STAR,MR,Int>::ColRank() const
{ return 0; }

template<typename T,typename Int>
Int
DistMatrix<T,STAR,MR,Int>::RowRank() const
{ return this->grid_->Col(); }

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::AlignWith( const elem::DistData<Int>& data )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::AlignWith");
    this->AssertFreeRowAlignment();
#endif
    const Grid& grid = *data.grid;
    this->SetGrid( grid );

    if( data.colDist == MR )
        this->rowAlignment_ = data.colAlignment;
    else if( data.rowDist == MR )
        this->rowAlignment_ = data.rowAlignment;
    else if( data.colDist == VR )
        this->rowAlignment_ = data.colAlignment % this->RowStride();
    else if( data.rowDist == VR )
        this->rowAlignment_ = data.rowAlignment % this->RowStride();
#ifndef RELEASE
    else throw std::logic_error("Nonsensical alignment");
#endif
    this->constrainedRowAlignment_ = true;
    this->SetShifts();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::AlignWith( const AutoDistMatrix<Int>& A )
{ this->AlignWith( A.DistData() ); }

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::AlignRowsWith( const elem::DistData<Int>& data )
{ this->AlignWith( data ); }

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::AlignRowsWith( const AutoDistMatrix<Int>& A )
{ this->AlignWith( A.DistData() ); }

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::PrintBase
( std::ostream& os, const std::string msg ) const
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::PrintBase");
#endif
    const elem::Grid& g = this->Grid();
    if( g.Rank() == 0 && msg != "" )
        os << msg << std::endl;

    const Int height     = this->Height();
    const Int width      = this->Width();
    const Int localWidth = this->LocalWidth();
    const Int c          = g.Width();
    const Int rowShift   = this->RowShift();

    if( height == 0 || width == 0 || !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    // Only one process row needs to participate
    if( g.Row() == 0 )
    {
        std::vector<T> sendBuf(height*width,0);
        const T* thisBuffer = this->LockedBuffer();
        const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for 
#endif
        for( Int jLocal=0; jLocal<localWidth; ++jLocal )
        {
            T* destCol = &sendBuf[(rowShift+jLocal*c)*height];
            const T* sourceCol = &thisBuffer[jLocal*thisLDim];
            MemCopy( destCol, sourceCol, height );
        }

        // If we are the root, allocate the receive buffer
        std::vector<T> recvBuf;
        if( g.Col() == 0 )
            recvBuf.resize( height*width );

        // Sum the contributions and send to the root
        mpi::Reduce
        ( &sendBuf[0], &recvBuf[0], height*width, mpi::SUM, 0, g.RowComm() );

        if( g.Col() == 0 )
        {
            // Print the data
            for( Int i=0; i<height; ++i )
            {
                for( Int j=0; j<width; ++j )
                    os << recvBuf[i+j*height] << " ";
                os << "\n";
            }
            os << std::endl;
        }
    }
    mpi::Barrier( g.VCComm() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::Attach
( Int height, Int width, Int rowAlignment,
  T* buffer, Int ldim, const elem::Grid& g )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::Attach");
#endif
    this->Empty();

    this->grid_ = &g;
    this->height_ = height;
    this->width_ = width;
    this->rowAlignment_ = rowAlignment;
    this->viewing_ = true;
    this->SetRowShift();
    if( g.InGrid() )
    {
        const Int localWidth = Length(width,this->rowShift_,g.Width());
        this->matrix_.Attach( height, localWidth, buffer, ldim );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::LockedAttach
( Int height, Int width, Int rowAlignment,
  const T* buffer, Int ldim, const elem::Grid& g )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::LockedAttach");
#endif
    this->Empty();

    this->grid_ = &g;
    this->height_ = height;
    this->width_ = width;
    this->rowAlignment_ = rowAlignment;
    this->viewing_ = true;
    this->locked_ = true;
    this->SetRowShift();
    if( g.InGrid() )
    {
        const Int localWidth = Length(width,this->rowShift_,g.Width());
        this->matrix_.LockedAttach( height, localWidth, buffer, ldim );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::ResizeTo( Int height, Int width )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::ResizeTo");
    this->AssertNotLocked();
    if( height < 0 || width < 0 )
        throw std::logic_error("Height and width must be non-negative");
#endif
    this->height_ = height;
    this->width_ = width;
    if( this->Participating() )
        this->matrix_.ResizeTo
        ( height, Length(width,this->RowShift(),this->Grid().Width()) );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
T
DistMatrix<T,STAR,MR,Int>::Get( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::Get");
    this->AssertValidEntry( i, j );
    if( !this->Participating() )
        throw std::logic_error("Should only be called by members of grid");
#endif
    // We will determine the owner column of entry (i,j) and broadcast from that
    // column within each process row
    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    T u;
    if( g.Col() == ownerCol )
    {
        const Int jLoc = (j-this->RowShift()) / g.Width();
        u = this->GetLocal(i,jLoc);
    }
    mpi::Broadcast( &u, 1, ownerCol, g.RowComm() );
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::Set( Int i, Int j, T u )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::Set");
    this->AssertValidEntry( i, j );
#endif
    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    if( g.Col() == ownerCol )
    {
        const Int jLoc = (j-this->RowShift()) / g.Width();
        this->SetLocal(i,jLoc,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::Update( Int i, Int j, T u )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::Update");
    this->AssertValidEntry( i, j );
#endif
    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    if( g.Col() == ownerCol )
    {
        const Int jLoc = (j-this->RowShift()) / g.Width();
        this->UpdateLocal(i,jLoc,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// Utility functions, e.g., SumOverCol
//

template<typename T,typename Int> 
void
DistMatrix<T,STAR,MR,Int>::SumOverCol()
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::SumOverCol");
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
    const Int localSize = std::max( localHeight*localWidth, mpi::MIN_COLL_MSG );

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

    // AllReduce col
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
DistMatrix<T,STAR,MR,Int>::AdjointFrom( const DistMatrix<T,VR,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR]::AdjointFrom");
#endif
    this->TransposeFrom( A, true );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::TransposeFrom
( const DistMatrix<T,VR,STAR,Int>& A, bool conjugate )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR]::TransposeFrom");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Width(), A.Height() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
    {
        if( !this->ConstrainedRowAlignment() )
        {
            this->rowAlignment_ = A.ColAlignment() % g.Width();
            this->SetRowShift();
        }
        this->ResizeTo( A.Width(), A.Height() );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    if( this->RowAlignment() == A.ColAlignment() % g.Width() )
    {
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = g.Size();
        const Int col = g.Col();

        const Int width = this->Width();
        const Int height = this->Height();
        const Int localHeightOfA = A.LocalHeight();
        const Int maxLocalHeightOfA = MaxLength(width,p);

        const Int portionSize = 
            std::max(height*maxLocalHeightOfA,mpi::MIN_COLL_MSG);

        this->auxMemory_.Require( (r+1)*portionSize );

        T* buffer = this->auxMemory_.Buffer();
        T* originalData = &buffer[0];
        T* gatheredData = &buffer[portionSize];

        // Pack
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
        if( conjugate )
        {
#ifdef HAVE_OPENMP
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localHeightOfA; ++jLocal )
            {
                T* destCol = &originalData[jLocal*height];
                const T* sourceCol = &ABuffer[jLocal];
                for( Int i=0; i<height; ++i )
                    destCol[i] = Conj( sourceCol[i*ALDim] );
            }
        }
        else
        {
#ifdef HAVE_OPENMP
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localHeightOfA; ++jLocal )
            {
                T* destCol = &originalData[jLocal*height];
                const T* sourceCol = &ABuffer[jLocal];
                for( Int i=0; i<height; ++i )
                    destCol[i] = sourceCol[i*ALDim];
            }
        }

        // Communicate
        mpi::AllGather
        ( originalData, portionSize,
          gatheredData, portionSize, g.ColComm() );

        // Unpack
        const Int rowShift = this->RowShift();
        const Int colAlignmentOfA = A.ColAlignment();
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<r; ++k )
        {
            const T* data = &gatheredData[k*portionSize];

            const Int colShiftOfA = Shift_( col+k*c, colAlignmentOfA, p );
            const Int rowOffset = (colShiftOfA-rowShift) / c;
            const Int localWidth = Length_( width, colShiftOfA, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidth; ++jLocal ) 
            {
                const T* dataCol = &data[jLocal*height];
                T* thisCol = &thisBuffer[(rowOffset+jLocal*r)*thisLDim];
                MemCopy( thisCol, dataCol, height );
            }
        }
        this->auxMemory_.Release();
    }
    else
    {
#ifdef UNALIGNED_WARNINGS
        if( g.Rank() == 0 )
            std::cerr << "Unaligned [* ,MR].TransposeFrom[VR,* ]." << std::endl;
#endif
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = g.Size();
        const Int col = g.Col();
        const Int rank = g.VRRank();

        // Perform the SendRecv to make A have the same rowAlignment
        const Int rowAlignment = this->RowAlignment();
        const Int colAlignmentOfA = A.ColAlignment();
        const Int rowShift = this->RowShift();

        const Int sendRank = (rank+p+rowAlignment-colAlignmentOfA) % p;
        const Int recvRank = (rank+p+colAlignmentOfA-rowAlignment) % p;

        const Int width = this->Width();
        const Int height = this->Height();
        const Int localHeightOfA = A.LocalHeight();
        const Int maxLocalHeightOfA = MaxLength(width,p);

        const Int portionSize = 
            std::max(height*maxLocalHeightOfA,mpi::MIN_COLL_MSG);

        this->auxMemory_.Require( (r+1)*portionSize );

        T* buffer = this->auxMemory_.Buffer();
        T* firstBuffer = &buffer[0];
        T* secondBuffer = &buffer[portionSize];

        // Pack
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
        if( conjugate )
        {
#ifdef HAVE_OPENMP
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localHeightOfA; ++jLocal )
            {
                T* destCol = &secondBuffer[jLocal*height];
                const T* sourceCol = &ABuffer[jLocal];
                for( Int i=0; i<height; ++i )
                    destCol[i] = Conj( sourceCol[i*ALDim] );
            }
        }
        else
        {
#ifdef HAVE_OPENMP
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localHeightOfA; ++jLocal )
            {
                T* destCol = &secondBuffer[jLocal*height];
                const T* sourceCol = &ABuffer[jLocal];
                for( Int i=0; i<height; ++i )
                    destCol[i] = sourceCol[i*ALDim];
            }
        }

        // Perform the SendRecv: puts the new data into the first buffer
        mpi::SendRecv
        ( secondBuffer, portionSize, sendRank, 0,
          firstBuffer,  portionSize, recvRank, mpi::ANY_TAG, g.VRComm() );

        // Use the SendRecv as input to the AllGather
        mpi::AllGather
        ( firstBuffer,  portionSize,
          secondBuffer, portionSize, g.ColComm() );

        // Unpack
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<r; ++k )
        {
            const T* data = &secondBuffer[k*portionSize];

            const Int colShiftOfA = Shift_( col+c*k, rowAlignment, p );
            const Int rowOffset = (colShiftOfA-rowShift) / c;
            const Int localWidth = Length_( width, colShiftOfA, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidth; ++jLocal )
            {
                const T* dataCol = &data[jLocal*height];
                T* thisCol = &thisBuffer[(rowOffset+jLocal*r)*thisLDim];
                MemCopy( thisCol, dataCol, height );
            }
        }
        this->auxMemory_.Release();
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,MC,MR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [MC,MR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
#ifdef CACHE_WARNINGS
    if( A.Height() != 1 && g.Rank() == 0 )
    {
        std::cerr << 
          "The matrix redistribution [* ,MR] <- [MC,MR] potentially causes a "
          "large amount of cache-thrashing. If possible, avoid it by "
          "performing the redistribution with a (conjugate)-transpose: \n"
          << "  [MR,* ].(Conjugate)TransposeFrom([MC,MR])" << std::endl;
    }
#endif
    if( !this->Viewing() )
    {
        if( !this->ConstrainedRowAlignment() )
        {
            this->rowAlignment_ = A.RowAlignment();
            this->SetRowShift();
        }
        this->ResizeTo( A.Height(), A.Width() );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    if( this->RowAlignment() == A.RowAlignment() )
    {
        if( A.Height() == 1 )
        {
            const Int localWidth = this->LocalWidth();

            this->auxMemory_.Require( localWidth );
            T* bcastBuf = this->auxMemory_.Buffer();

            if( g.Row() == A.ColAlignment() )
            {
                this->matrix_ = A.LockedMatrix();

                // Pack
                const T* thisBuffer = this->LockedBuffer();
                const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
                #pragma omp parallel for
#endif
                for( Int jLocal=0; jLocal<localWidth; ++jLocal )
                    bcastBuf[jLocal] = thisBuffer[jLocal*thisLDim];
            }

            // Communicate
            mpi::Broadcast
            ( bcastBuf, localWidth, A.ColAlignment(), g.ColComm() );

            // Unpack
            T* thisBuffer = this->Buffer();
            const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidth; ++jLocal )
                thisBuffer[jLocal*thisLDim] = bcastBuf[jLocal];

            this->auxMemory_.Release();
        }
        else
        {
            const Int r = g.Height();
            const Int height = this->Height();
            const Int localWidth = this->LocalWidth();
            const Int localHeightOfA = A.LocalHeight();
            const Int maxLocalHeight = MaxLength(height,r);

            const Int portionSize = 
                std::max(maxLocalHeight*localWidth,mpi::MIN_COLL_MSG);

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
            for( Int jLocal=0; jLocal<localWidth; ++jLocal )
            {
                const T* ACol = &ABuffer[jLocal*ALDim];
                T* originalDataCol = &originalData[jLocal*localHeightOfA];
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
                for( Int jLocal=0; jLocal<localWidth; ++jLocal )
                {
                    T* destCol = &thisBuffer[colShift+jLocal*thisLDim];
                    const T* sourceCol = &data[jLocal*localHeight];
                    for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                        destCol[iLocal*r] = sourceCol[iLocal];
                }
            }
            this->auxMemory_.Release();
        }
    }
    else
    {
#ifdef UNALIGNED_WARNINGS
        if( g.Rank() == 0 )
            std::cerr << "Unaligned [* ,MR] <- [MC,MR]." << std::endl;
#endif
        const Int r = g.Height();
        const Int c = g.Width();
        const Int col = g.Col();

        const Int rowAlignment = this->RowAlignment();
        const Int rowAlignmentOfA = A.RowAlignment();
        const Int sendCol = (col+c+rowAlignment-rowAlignmentOfA) % c;
        const Int recvCol = (col+c+rowAlignmentOfA-rowAlignment) % c;

        if( A.Height() == 1 )
        {
            const Int localWidth = this->LocalWidth();
            T* bcastBuf;

            if( g.Row() == A.ColAlignment() )
            {
                const Int localWidthOfA = A.LocalWidth();

                this->auxMemory_.Require( localWidth+localWidthOfA );
                T* buffer = this->auxMemory_.Buffer();
                T* sendBuf = &buffer[0];
                bcastBuf   = &buffer[localWidthOfA];

                // Pack
                const T* ABuffer = A.LockedBuffer();
                const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
                #pragma omp parallel for
#endif
                for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
                    sendBuf[jLocal] = ABuffer[jLocal*ALDim];

                // Communicate
                mpi::SendRecv
                ( sendBuf,  localWidthOfA, sendCol, 0,
                  bcastBuf, localWidth,    recvCol, mpi::ANY_TAG,
                  g.RowComm() );
            }
            else
            {
                this->auxMemory_.Require( localWidth );
                bcastBuf = this->auxMemory_.Buffer();
            }

            // Communicate
            mpi::Broadcast
            ( bcastBuf, localWidth, A.ColAlignment(), g.ColComm() );

            // Unpack
            T* thisBuffer = this->Buffer();
            const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidth; ++jLocal )
                thisBuffer[jLocal*thisLDim] = bcastBuf[jLocal];
            this->auxMemory_.Release();
        }
        else
        {
            const Int height = this->Height();
            const Int localWidth  = this->LocalWidth();
            const Int localHeightOfA = A.LocalHeight();
            const Int localWidthOfA  = A.LocalWidth();
            const Int maxLocalHeight = MaxLength(height,r);

            const Int portionSize = 
                std::max(maxLocalHeight*localWidth,mpi::MIN_COLL_MSG);

            this->auxMemory_.Require( (r+1)*portionSize );

            T* buffer = this->auxMemory_.Buffer();
            T* firstBuffer = &buffer[0];
            T* secondBuffer = &buffer[portionSize];

            // Pack
            const T* ABuffer = A.LockedBuffer();
            const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
            {
                const T* ACol = &ABuffer[jLocal*ALDim];
                T* secondBufferCol = &secondBuffer[jLocal*localHeightOfA];
                MemCopy( secondBufferCol, ACol, localHeightOfA );
            }

            // Perform the SendRecv: puts the new data into the first buffer
            mpi::SendRecv
            ( secondBuffer, portionSize, sendCol, 0,
              firstBuffer,  portionSize, recvCol, mpi::ANY_TAG, g.RowComm() );

            // Use the output of the SendRecv as input to the AllGather
            mpi::AllGather
            ( firstBuffer,  portionSize,
              secondBuffer, portionSize, g.ColComm() );

            // Unpack the contents of each member of the process col
            const Int colAlignmentOfA = A.ColAlignment();
            T* thisBuffer = this->Buffer();
            const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int k=0; k<r; ++k )
            {
                const T* data = &secondBuffer[k*portionSize];

                const Int colShift = Shift_( k, colAlignmentOfA, r );
                const Int localHeight = Length_( height, colShift, r );
#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
                #pragma omp parallel for 
#endif
                for( Int jLocal=0; jLocal<localWidth; ++jLocal )
                {
                    T* destCol = &thisBuffer[colShift+jLocal*thisLDim];
                    const T* sourceCol = &data[jLocal*localHeight];
                    for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                        destCol[iLocal*r] = sourceCol[iLocal];
                }
            }
            this->auxMemory_.Release();
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,MC,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [MC,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    DistMatrix<T,MC,MR,Int> A_MC_MR(false,true,0,this->RowAlignment(),g);

    A_MC_MR = A;
    *this = A_MC_MR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,STAR,MR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [* ,MR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
    {
        if( !this->ConstrainedRowAlignment() )
        {
            this->rowAlignment_ = A.RowAlignment();
            if( g.InGrid() )
                this->rowShift_ = A.RowShift();
        }
        this->ResizeTo( A.Height(), A.Width() );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    if( this->RowAlignment() == A.RowAlignment() )
    {
        this->matrix_ = A.LockedMatrix();
    }
    else
    {
#ifdef UNALIGNED_WARNINGS
        if( g.Rank() == 0 )
            std::cerr << "Unaligned [* ,MR] <- [* ,MR]." << std::endl;
#endif
        const Int rank = g.Col();
        const Int c = g.Width();

        const Int rowAlignment = this->RowAlignment();
        const Int rowAlignmentOfA = A.RowAlignment();

        const Int sendRank = (rank+c+rowAlignment-rowAlignmentOfA) % c;
        const Int recvRank = (rank+c+rowAlignmentOfA-rowAlignment) % c;

        const Int height = this->Height();
        const Int localWidth = this->LocalWidth();
        const Int localWidthOfA = A.LocalWidth();

        const Int sendSize = height * localWidthOfA;
        const Int recvSize = height * localWidth;

        this->auxMemory_.Require( sendSize + recvSize );

        T* buffer = this->auxMemory_.Buffer();
        T* sendBuffer = &buffer[0];
        T* recvBuffer = &buffer[sendSize];

        // Pack
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
        {
            const T* ACol = &ABuffer[jLocal*ALDim];
            T* sendBufferCol = &sendBuffer[jLocal*height];
            MemCopy( sendBufferCol, ACol, height );
        }

        // Communicate
        mpi::SendRecv
        ( sendBuffer, sendSize, sendRank, 0,
          recvBuffer, recvSize, recvRank, mpi::ANY_TAG, g.RowComm() );

        // Unpack
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int jLocal=0; jLocal<localWidth; ++jLocal )
        {
            const T* recvBufferCol = &recvBuffer[jLocal*height];
            T* thisCol = &thisBuffer[jLocal*thisLDim];
            MemCopy( thisCol, recvBufferCol, height );
        }
        this->auxMemory_.Release();
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,MD,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,MR] = [MD,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    throw std::logic_error("[* ,MR] = [MD,* ] not yet implemented");
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,STAR,MD,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,MR] = [* ,MD]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    throw std::logic_error("[* ,MR] = [* ,MD] not yet implemented");
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,MR,MC,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [MR,MC]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    std::auto_ptr<DistMatrix<T,STAR,VC,Int> > A_STAR_VC
    ( new DistMatrix<T,STAR,VC,Int>(g) );
    *A_STAR_VC = A;

    std::auto_ptr<DistMatrix<T,STAR,VR,Int> > A_STAR_VR
    ( new DistMatrix<T,STAR,VR,Int>(true,this->RowAlignment(),g) );
    *A_STAR_VR = *A_STAR_VC;
    delete A_STAR_VC.release(); // lowers memory highwater

    *this = *A_STAR_VR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,MR,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [MR,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    std::auto_ptr<DistMatrix<T,VR,STAR,Int> > A_VR_STAR
    ( new DistMatrix<T,VR,STAR,Int>(g) );
    *A_VR_STAR = A;

    std::auto_ptr<DistMatrix<T,VC,STAR,Int> > A_VC_STAR
    ( new DistMatrix<T,VC,STAR,Int>(g) );
    *A_VC_STAR = *A_VR_STAR;
    delete A_VR_STAR.release(); // lowers memory highwater

    std::auto_ptr<DistMatrix<T,MC,MR,Int> > A_MC_MR
    ( new DistMatrix<T,MC,MR,Int>(false,true,0,this->RowAlignment(),g) );
    *A_MC_MR = *A_VC_STAR;
    delete A_VC_STAR.release(); // lowers memory highwater

    *this = *A_MC_MR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,STAR,MC,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [* ,MC]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    std::auto_ptr<DistMatrix<T,STAR,VC,Int> > A_STAR_VC
    ( new DistMatrix<T,STAR,VC,Int>(g) );
    *A_STAR_VC = A;

    std::auto_ptr<DistMatrix<T,STAR,VR,Int> > A_STAR_VR
    ( new DistMatrix<T,STAR,VR,Int>(true,this->RowAlignment(),g) );
    *A_STAR_VR = *A_STAR_VC;
    delete A_STAR_VC.release(); // lowers memory highwater

    std::auto_ptr<DistMatrix<T,MC,MR,Int> > A_MC_MR
    ( new DistMatrix<T,MC,MR,Int>(g) );
    *A_MC_MR = *A_STAR_VR;
    delete A_STAR_VR.release(); // lowers memory highwater

    *this = *A_MC_MR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,VC,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [VC,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    DistMatrix<T,MC,MR,Int> A_MC_MR(false,true,0,this->RowAlignment(),g);

    A_MC_MR = A;
    *this = A_MC_MR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,STAR,VC,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [* ,VC]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    DistMatrix<T,STAR,VR,Int> A_STAR_VR(true,this->RowAlignment(),g);

    A_STAR_VR = A;
    *this = A_STAR_VR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,VR,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [VR,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    std::auto_ptr<DistMatrix<T,VC,STAR,Int> > A_VC_STAR
    ( new DistMatrix<T,VC,STAR,Int>(g) );
    *A_VC_STAR = A;

    std::auto_ptr<DistMatrix<T,MC,MR,Int> > A_MC_MR
    ( new DistMatrix<T,MC,MR,Int>(false,true,0,this->RowAlignment(),g) );
    *A_MC_MR = *A_VC_STAR;
    delete A_VC_STAR.release(); // lowers memory highwater

    *this = *A_MC_MR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,STAR,VR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[* ,MR] = [* ,VR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
    {
        if( !this->ConstrainedRowAlignment() )
        {
            this->rowAlignment_ = A.RowAlignment() % g.Width();
            this->SetRowShift();
        }
        this->ResizeTo( A.Height(), A.Width() );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    if( this->RowAlignment() == A.RowAlignment() % g.Width() )
    {
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = g.Size();
        const Int col = g.Col();

        const Int width = this->Width();
        const Int height = this->Height();
        const Int localWidthOfA = A.LocalWidth();
        const Int maxLocalWidthOfA = MaxLength(width,p);

        const Int portionSize = 
            std::max(height*maxLocalWidthOfA,mpi::MIN_COLL_MSG);

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
        const Int rowShift = this->RowShift();
        const Int rowAlignmentOfA = A.RowAlignment();
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<r; ++k )
        {
            const T* data = &gatheredData[k*portionSize];

            const Int rowShiftOfA = Shift_( col+k*c, rowAlignmentOfA, p );
            const Int rowOffset = (rowShiftOfA-rowShift) / c;
            const Int localWidth = Length_( width, rowShiftOfA, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidth; ++jLocal )
            {
                const T* dataCol = &data[jLocal*height];
                T* thisCol = &thisBuffer[(rowOffset+jLocal*r)*thisLDim];
                MemCopy( thisCol, dataCol, height );
            }
        }
        this->auxMemory_.Release();
    }
    else
    {
#ifdef UNALIGNED_WARNINGS
        if( g.Rank() == 0 )
            std::cerr << "Unaligned [* ,MR] <- [* ,VR]." << std::endl;
#endif
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = g.Size();
        const Int col = g.Col();
        const Int rank = g.VRRank();

        // Perform the SendRecv to make A have the same rowAlignment
        const Int rowAlignment = this->RowAlignment();
        const Int rowAlignmentOfA = A.RowAlignment();
        const Int rowShift = this->RowShift();

        const Int sendRank = (rank+p+rowAlignment-rowAlignmentOfA) % p;
        const Int recvRank = (rank+p+rowAlignmentOfA-rowAlignment) % p;

        const Int width = this->Width();
        const Int height = this->Height();
        const Int localWidthOfA = A.LocalWidth();
        const Int maxLocalWidthOfA = MaxLength(width,p);

        const Int portionSize = 
            std::max(height*maxLocalWidthOfA,mpi::MIN_COLL_MSG);

        this->auxMemory_.Require( (r+1)*portionSize );

        T* buffer = this->auxMemory_.Buffer();
        T* firstBuffer = &buffer[0];
        T* secondBuffer = &buffer[portionSize];

        // Pack
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
        {
            const T* ACol = &ABuffer[jLocal*ALDim];
            T* secondBufferCol = &secondBuffer[jLocal*height];
            MemCopy( secondBufferCol, ACol, height );
        }

        // Perform the SendRecv: puts the new data into the first buffer
        mpi::SendRecv
        ( secondBuffer, portionSize, sendRank, 0,
          firstBuffer,  portionSize, recvRank, mpi::ANY_TAG, g.VRComm() );

        // Use the SendRecv as input to the AllGather
        mpi::AllGather
        ( firstBuffer,  portionSize,
          secondBuffer, portionSize, g.ColComm() );

        // Unpack
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<r; ++k )
        {
            const T* data = &secondBuffer[k*portionSize];

            const Int rowShiftOfA = Shift_( col+c*k, rowAlignment, p );
            const Int rowOffset = (rowShiftOfA-rowShift) / c;
            const Int localWidth = Length_( width, rowShiftOfA, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<localWidth; ++jLocal )
            {
                const T* dataCol = &data[jLocal*height];
                T* thisCol = &thisBuffer[(rowOffset+jLocal*r)*thisLDim];
                MemCopy( thisCol, dataCol, height );
            }
        }
        this->auxMemory_.Release();
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,STAR,MR,Int>&
DistMatrix<T,STAR,MR,Int>::operator=( const DistMatrix<T,STAR,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[* ,MR] = [* ,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !this->Participating() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }

    const Int c = this->Grid().Width();
    const Int rowShift = this->RowShift();

    const Int localHeight = this->LocalHeight();
    const Int localWidth = this->LocalWidth();

    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int jLocal=0; jLocal<localWidth; ++jLocal )
    {
        const T* ACol = &ABuffer[(rowShift+jLocal*c)*ALDim];
        T* thisCol = &thisBuffer[jLocal*thisLDim];
        MemCopy( thisCol, ACol, localHeight );
    }

#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

//
// Routines which explicitly work in the complex plane
//

template<typename T,typename Int>
typename Base<T>::type
DistMatrix<T,STAR,MR,Int>::GetRealPart( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::GetRealPart");
    this->AssertValidEntry( i, j );
    if( !this->Participating() )
        throw std::logic_error("Should only be called by processed in grid");
#endif
    typedef typename Base<T>::type R;

    // We will determine the owner column of entry (i,j) and broadcast from that
    // column within each process row
    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    R u;
    if( g.Col() == ownerCol )
    {
        const Int jLocal = (j-this->RowShift()) / g.Width();
        u = this->GetLocalRealPart( i, jLocal );
    }
    mpi::Broadcast( &u, 1, ownerCol, g.RowComm() );
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
typename Base<T>::type
DistMatrix<T,STAR,MR,Int>::GetImagPart( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::GetImagPart");
    this->AssertValidEntry( i, j );
    if( !this->Participating() )
        throw std::logic_error("Should only be called by processed in grid");
#endif
    typedef typename Base<T>::type R;

    // We will determine the owner column of entry (i,j) and broadcast from that
    // column within each process row
    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    R u;
    if( g.Col() == ownerCol )
    {
        const Int jLocal = (j-this->RowShift()) / g.Width();
        u = this->GetLocalImagPart( i, jLocal );
    }
    mpi::Broadcast( &u, 1, ownerCol, g.RowComm() );
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::SetRealPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::SetRealPart");
    this->AssertValidEntry( i, j );
#endif
    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    if( g.Col() == ownerCol )
    {
        const Int jLocal = (j-this->RowShift()) / g.Width();
        this->SetLocalRealPart( i, jLocal, u );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::SetImagPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::SetImagPart");
    this->AssertValidEntry( i, j );
#endif
    if( !IsComplex<T>::val )
        throw std::logic_error("Called complex-only routine with real data");

    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    if( g.Col() == ownerCol )
    {
        const Int jLocal = (j-this->RowShift()) / g.Width();
        this->SetLocalImagPart( i, jLocal, u );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::UpdateRealPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::UpdateRealPart");
    this->AssertValidEntry( i, j );
#endif
    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    if( g.Col() == ownerCol )
    {
        const Int jLocal = (j-this->RowShift()) / g.Width();
        this->UpdateLocalRealPart( i, jLocal, u );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,STAR,MR,Int>::UpdateImagPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[* ,MR]::UpdateImagPart");
    this->AssertValidEntry( i, j );
#endif
    if( !IsComplex<T>::val )
        throw std::logic_error("Called complex-only routine with real data");

    const elem::Grid& g = this->Grid();
    const Int ownerCol = (j + this->RowAlignment()) % g.Width();

    if( g.Col() == ownerCol )
    {
        const Int jLocal = (j-this->RowShift()) / g.Width();
        this->UpdateLocalImagPart( i, jLocal, u );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template class DistMatrix<int,STAR,MR,int>;
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,MC,  MR,  int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,MC,  STAR,int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,MD,  STAR,int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,MR,  MC,  int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,MR,  STAR,int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,STAR,MC,  int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,STAR,MD,  int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,STAR,STAR,int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,STAR,VC,  int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,STAR,VR,  int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,VC,  STAR,int>& A );
template DistMatrix<int,STAR,MR,int>::DistMatrix( const DistMatrix<int,VR,  STAR,int>& A );

#ifndef DISABLE_FLOAT
template class DistMatrix<float,STAR,MR,int>;
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,MC,  MR,  int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,MC,  STAR,int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,MD,  STAR,int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,MR,  MC,  int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,MR,  STAR,int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,STAR,MC,  int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,STAR,MD,  int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,STAR,STAR,int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,STAR,VC,  int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,STAR,VR,  int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,VC,  STAR,int>& A );
template DistMatrix<float,STAR,MR,int>::DistMatrix( const DistMatrix<float,VR,  STAR,int>& A );
#endif // ifndef DISABLE_FLOAT

template class DistMatrix<double,STAR,MR,int>;
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,MC,  MR,  int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,MC,  STAR,int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,MD,  STAR,int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,MR,  MC,  int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,MR,  STAR,int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,STAR,MC,  int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,STAR,MD,  int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,STAR,STAR,int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,STAR,VC,  int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,STAR,VR,  int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,VC,  STAR,int>& A );
template DistMatrix<double,STAR,MR,int>::DistMatrix( const DistMatrix<double,VR,  STAR,int>& A );

#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
template class DistMatrix<Complex<float>,STAR,MR,int>;
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,MC,  MR,  int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,MC,  STAR,int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,MD,  STAR,int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,MR,  MC,  int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,MR,  STAR,int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,MC,  int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,MD,  int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,STAR,int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,VC,  int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,VR,  int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,VC,  STAR,int>& A );
template DistMatrix<Complex<float>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<float>,VR,  STAR,int>& A );
#endif // ifndef DISABLE_FLOAT
template class DistMatrix<Complex<double>,STAR,MR,int>;
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,MC,  MR,  int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,MC,  STAR,int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,MD,  STAR,int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,MR,  MC,  int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,MR,  STAR,int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,MC,  int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,MD,  int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,STAR,int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,VC,  int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,VR,  int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,VC,  STAR,int>& A );
template DistMatrix<Complex<double>,STAR,MR,int>::DistMatrix( const DistMatrix<Complex<double>,VR,  STAR,int>& A );
#endif // ifndef DISABLE_COMPLEX

} // namespace elem
