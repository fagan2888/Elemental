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
DistMatrix<T,VC,STAR,Int>::DistMatrix( const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (0,0,false,false,0,0,
   (g.InGrid() ? g.VCRank() : 0 ),0,
   0,0,g)
{ }

template<typename T,typename Int>
DistMatrix<T,VC,STAR,Int>::DistMatrix
( Int height, Int width, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,false,false,0,0,
   (g.InGrid() ? g.VCRank() : 0),0,
   (g.InGrid() ? Length(height,g.VCRank(),0,g.Size()) : 0),width,
   g)
{ }

template<typename T,typename Int>
DistMatrix<T,VC,STAR,Int>::DistMatrix
( Int height, Int width, Int colAlignment, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,true,false,colAlignment,0,
   (g.InGrid() ? Shift(g.VCRank(),colAlignment,g.Size()) : 0),0,
   (g.InGrid() ? Length(height,g.VCRank(),colAlignment,g.Size()) : 0),
   width,g)
{ }

template<typename T,typename Int>
DistMatrix<T,VC,STAR,Int>::DistMatrix
( Int height, Int width, Int colAlignment, Int ldim, const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,true,false,colAlignment,0,
   (g.InGrid() ? Shift(g.VCRank(),colAlignment,g.Size()) : 0),0,
   (g.InGrid() ? Length(height,g.VCRank(),colAlignment,g.Size()) : 0),
   width,ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,VC,STAR,Int>::DistMatrix
( Int height, Int width, Int colAlignment, const T* buffer, Int ldim,
  const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,colAlignment,0,
   (g.InGrid() ? Shift(g.VCRank(),colAlignment,g.Size()) : 0),0,
   (g.InGrid() ? Length(height,g.VCRank(),colAlignment,g.Size()) : 0),
   width,buffer,ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,VC,STAR,Int>::DistMatrix
( Int height, Int width, Int colAlignment, T* buffer, Int ldim,
  const elem::Grid& g )
: AbstractDistMatrix<T,Int>
  (height,width,colAlignment,0,
   (g.InGrid() ? Shift(g.VCRank(),colAlignment,g.Size()) : 0),0,
   (g.InGrid() ? Length(height,g.VCRank(),colAlignment,g.Size()) : 0),
   width,buffer,ldim,g)
{ }

template<typename T,typename Int>
DistMatrix<T,VC,STAR,Int>::DistMatrix( const DistMatrix<T,VC,STAR,Int>& A )
: AbstractDistMatrix<T,Int>(0,0,false,false,0,0,
  (A.Participating() ? A.ColRank() : 0),0,
  0,0,A.Grid())
{
#ifndef RELEASE
    PushCallStack("DistMatrix[VC,* ]::DistMatrix");
#endif
    if( &A != this )
        *this = A;
    else
        throw std::logic_error("Tried to construct [VC,* ] with itself");
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
template<Distribution U,Distribution V>
DistMatrix<T,VC,STAR,Int>::DistMatrix( const DistMatrix<T,U,V,Int>& A )
: AbstractDistMatrix<T,Int>(0,0,false,false,0,0,
  (A.Participating() ? A.ColRank() : 0),0,
  0,0,A.Grid())
{
#ifndef RELEASE
    PushCallStack("DistMatrix[VC,* ]::DistMatrix");
#endif
    if( VC != U || STAR != V || 
        reinterpret_cast<const DistMatrix<T,VC,STAR,Int>*>(&A) != this )
        *this = A;
    else
        throw std::logic_error("Tried to construct [VC,* ] with itself");
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
DistMatrix<T,VC,STAR,Int>::~DistMatrix()
{ }

template<typename T,typename Int>
elem::DistData<Int>
DistMatrix<T,VC,STAR,Int>::DistData() const
{
    elem::DistData<Int> data;
    data.colDist = VC;
    data.rowDist = STAR;
    data.colAlignment = this->colAlignment_;
    data.rowAlignment = 0;
    data.diagPath = 0;
    data.grid = this->grid_;
    return data;
}

template<typename T,typename Int>
Int
DistMatrix<T,VC,STAR,Int>::ColStride() const
{ return this->grid_->Size(); }

template<typename T,typename Int>
Int
DistMatrix<T,VC,STAR,Int>::RowStride() const
{ return 1; }

template<typename T,typename Int>
Int
DistMatrix<T,VC,STAR,Int>::ColRank() const
{ return this->grid_->VCRank(); }

template<typename T,typename Int>
Int
DistMatrix<T,VC,STAR,Int>::RowRank() const
{ return 0; }

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::AlignWith( const elem::DistData<Int>& data )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::AlignWith");
    this->AssertFreeColAlignment();
#endif
    const Grid& grid = *data.grid;
    this->SetGrid( grid );
    
    if( data.colDist == MC || data.colDist == VC )
        this->colAlignment_ = data.colAlignment;
    else if( data.rowDist == MC || data.rowDist == VC )
        this->colAlignment_ = data.rowAlignment;
#ifndef RELEASE
    else throw std::logic_error("Nonsensical alignment");
#endif
    this->constrainedColAlignment_ = true;
    this->SetShifts();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::AlignWith( const AutoDistMatrix<Int>& A )
{ this->AlignWith( A.DistData() ); }

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::AlignColsWith( const elem::DistData<Int>& data )
{ this->AlignWith( data ); }

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::AlignColsWith( const AutoDistMatrix<Int>& A )
{ this->AlignWith( A.DistData() ); }

template<typename T,typename Int>
bool
DistMatrix<T,VC,STAR,Int>::AlignedWithDiagonal
( const elem::DistData<Int>& data, Int offset ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::AlignedWithDiagonal");
#endif
    const Grid& grid = this->Grid();
    if( grid != *data.grid )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return false;
    }

    bool aligned;
    if( (data.colDist == VC   && data.rowDist == STAR) ||
        (data.colDist == STAR && data.rowDist == VC  ) )
    {
        const Int alignment = ( data.colDist==VC ? data.colAlignment
                                                 : data.rowAlignment );
        if( offset >= 0 )
        {
            const Int proc = alignment;
            aligned = ( this->ColAlignment() == proc );
        }
        else
        {
            const Int proc = (alignment-offset) % this->ColStride();
            aligned = ( this->ColAlignment() == proc );
        }
    }
    else aligned = false;
#ifndef RELEASE
    PopCallStack();
#endif
    return aligned;
}

template<typename T,typename Int>
bool
DistMatrix<T,VC,STAR,Int>::AlignedWithDiagonal
( const AutoDistMatrix<Int>& A, Int offset ) const
{ return this->AlignedWithDiagonal( A.DistData(), offset ); }

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::AlignWithDiagonal
( const elem::DistData<Int>& data, Int offset )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::AlignWithDiagonal");
    this->AssertFreeColAlignment();
#endif
    const Grid& grid = *data.grid;
    this->SetGrid( grid );

    if( (data.colDist == VC   && data.rowDist == STAR) ||
        (data.colDist == STAR && data.rowDist == VC  ) )
    {
        const Int alignment = ( data.colDist==VC ? data.colAlignment
                                                 : data.rowAlignment );
        if( offset >= 0 )
        {
            const Int proc = alignment;
            this->colAlignment_ = proc;
        }
        else
        {
            const Int proc = (alignment-offset) % this->ColStride();
            this->colAlignment_ = proc;
        }
        this->constrainedColAlignment_ = true;
        this->SetShifts();
    }
#ifndef RELEASE
    else throw std::logic_error("Invalid diagonal alignment");
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::AlignWithDiagonal
( const AutoDistMatrix<Int>& A, Int offset )
{ this->AlignWithDiagonal( A.DistData(), offset ); }

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::PrintBase
( std::ostream& os, const std::string msg ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::PrintBase");
#endif
    const elem::Grid& g = this->Grid();
    if( g.Rank() == 0 && msg != "" )
        os << msg << std::endl;

    const Int height      = this->Height();
    const Int width       = this->Width();
    const Int localHeight = this->LocalHeight();
    const Int p           = g.Size();
    const Int colShift    = this->ColShift();

    if( height == 0 || width == 0 || !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    std::vector<T> sendBuf(height*width,0);
    const T* thisBuffer = this->LockedBuffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j=0; j<width; ++j )
    {
        T* destCol = &sendBuf[colShift+j*height];
        const T* sourceCol = &thisBuffer[j*thisLDim];
        for( Int iLocal=0; iLocal<localHeight; ++iLocal )
            destCol[iLocal*p] = sourceCol[iLocal];
    }

    // If we are the root, allocate a receive buffer
    std::vector<T> recvBuf;
    if( g.VCRank() == 0 )
        recvBuf.resize( height*width );

    // Sum the contributions and send to the root
    mpi::Reduce
    ( &sendBuf[0], &recvBuf[0], height*width, mpi::SUM, 0, g.VCComm() );

    if( g.VCRank() == 0 )
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
    mpi::Barrier( g.VCComm() );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::Attach
( Int height, Int width, Int colAlignment,
  T* buffer, Int ldim, const elem::Grid& g )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::Attach");
#endif
    this->Empty();

    this->grid_ = &g;
    this->height_ = height;
    this->width_ = width;
    this->colAlignment_ = colAlignment;
    this->viewing_ = true;
    this->SetColShift();
    if( g.InGrid() )
    {
        const Int localHeight = Length(height,this->colShift_,g.Size());
        this->matrix_.Attach( localHeight, width, buffer, ldim );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::LockedAttach
( Int height, Int width, Int colAlignment,
  const T* buffer, Int ldim, const elem::Grid& g )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::LockedAttach");
#endif
    this->Empty();

    this->grid_ = &g;
    this->height_ = height;
    this->width_ = width;
    this->colAlignment_ = colAlignment;
    this->viewing_ = true;
    this->locked_ = true;
    this->SetColShift();
    if( g.InGrid() )
    {
        const Int localHeight = Length(height,this->colShift_,g.Size());
        this->matrix_.LockedAttach( localHeight, width, buffer, ldim );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::ResizeTo( Int height, Int width )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::ResizeTo");
    this->AssertNotLocked();
    if( height < 0 || width < 0 )
        throw std::logic_error("Height and width must be non-negative");
#endif
    const elem::Grid& g = this->Grid();
    this->height_ = height;
    this->width_  = width;
    if( g.InGrid() )
        this->matrix_.ResizeTo
        ( Length(height,this->ColShift(),g.Size()) ,width );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
T
DistMatrix<T,VC,STAR,Int>::Get( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::Get");
    this->AssertValidEntry( i, j );
#endif
    // We will determine the owner rank of entry (i,j) and broadcast from that
    // process over the entire g
    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    T u;
    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        u = this->GetLocal(iLoc,j);
    }
    mpi::Broadcast( &u, 1, g.VCToViewingMap(ownerRank), g.ViewingComm() );
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::Set( Int i, Int j, T u )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::Set");
    this->AssertValidEntry( i, j );
#endif
    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        this->SetLocal(iLoc,j,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::Update( Int i, Int j, T u )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::Update");
    this->AssertValidEntry( i, j );
#endif
    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        this->UpdateLocal(iLoc,j,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::GetDiagonal
( DistMatrix<T,VC,STAR,Int>& d, Int offset ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::GetDiagonal");
    if( d.Viewing() )
        this->AssertSameGrid( d.Grid() );
#endif
    const Int diagLength = this->DiagonalLength(offset);
#ifndef RELEASE
    if( d.Viewing() && (diagLength != d.Height() || d.Width() != 1) )
    {
        std::ostringstream msg;
        msg << "d is not a column vec of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << diagLength << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( (d.Viewing() || d.ConstrainedColAlignment() ) &&
        !d.AlignedWithDiagonal( *this, offset ) )
        throw std::logic_error("d must be aligned with the 'offset' diagonal");
#endif
    const elem::Grid& g = this->Grid();
    if( !d.Viewing() )
    {
        d.SetGrid( g );
        if( !d.ConstrainedColAlignment() )
            d.AlignWithDiagonal( *this, offset );
        d.ResizeTo( diagLength, 1 );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.ColShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift) / p;
    const Int localDiagLength = d.LocalHeight();
    T* dBuffer = d.Buffer();
    const T* thisBuffer = this->LockedBuffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        dBuffer[k] = thisBuffer[iLocal+jLocal*thisLDim];
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::GetDiagonal
( DistMatrix<T,STAR,VC>& d, Int offset ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::GetDiagonal");
    if( d.Viewing() )
        this->AssertSameGrid( d.Grid() );
#endif
    const Int diagLength = this->DiagonalLength(offset);
#ifndef RELEASE
    if( d.Viewing() && (diagLength != d.Width() || d.Height() != 1) )
    {
        std::ostringstream msg;
        msg << "d is not a row vec of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << diagLength << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( ( d.Viewing() || d.ConstrainedRowAlignment() ) &&
        !d.AlignedWithDiagonal( *this, offset ) )
        throw std::logic_error("d must be aligned with the 'offset' diagonal");
#endif
    const elem::Grid& g = this->Grid();
    if( !d.Viewing() )
    {
        d.SetGrid( g );
        if( !d.ConstrainedRowAlignment() )
            d.AlignWithDiagonal( *this, offset );
        d.ResizeTo( 1, diagLength );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.RowShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift) / p;
    const Int localDiagLength = d.LocalWidth();
    T* dBuffer = d.Buffer();
    const Int dLDim = d.LDim();
    const T* thisBuffer = this->LockedBuffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        dBuffer[k*dLDim] = thisBuffer[iLocal+jLocal*thisLDim];
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SetDiagonal
( const DistMatrix<T,VC,STAR,Int>& d, Int offset )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::SetDiagonal");
    this->AssertSameGrid( d.Grid() );
    if( d.Width() != 1 )
        throw std::logic_error("d must be a column vector");
    const Int diagLength = this->DiagonalLength(offset);
    if( diagLength != d.Height() )
    {
        std::ostringstream msg;
        msg << "d is not of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << diagLength << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( !d.AlignedWithDiagonal( *this, offset ) )
        throw std::logic_error("d must be aligned with the 'offset' diagonal");
#endif
    const elem::Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.ColShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift)/p;
    const Int localDiagLength = d.LocalHeight();
    const T* dBuffer = d.LockedBuffer();
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        thisBuffer[iLocal+jLocal*thisLDim] = dBuffer[k];
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SetDiagonal
( const DistMatrix<T,STAR,VC>& d, Int offset )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::SetDiagonal");
    this->AssertSameGrid( d.Grid() );
    if( d.Height() != 1 )
        throw std::logic_error("d must be a row vector");
    const Int diagLength = this->DiagonalLength(offset);
    if( diagLength != d.Width() )
    {
        std::ostringstream msg;
        msg << "d is not of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << diagLength << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( !d.AlignedWithDiagonal( *this, offset ) )
        throw std::logic_error("d must be aligned with the 'offset' diagonal");
#endif
    const elem::Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.RowShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift)/p;
    const Int localDiagLength = d.LocalWidth();
    const T* dBuffer = d.LockedBuffer();
    T* thisBuffer = this->Buffer();
    const Int dLDim = d.LDim();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        thisBuffer[iLocal+jLocal*thisLDim] = dBuffer[k*dLDim];
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

//
// Utility functions, e.g., operator=
//

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,MC,MR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [MC,MR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
    {
        if( !this->ConstrainedColAlignment() )
        {
            this->colAlignment_ = A.ColAlignment();
            this->SetColShift();
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

    if( this->ColAlignment() % g.Height() == A.ColAlignment() )
    {
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = g.Size();
        const Int row = g.Row();
        const Int colShiftOfA = A.ColShift();
        const Int colAlignment = this->ColAlignment();
        const Int rowAlignmentOfA = A.RowAlignment();

        const Int height = this->Height();
        const Int width = this->Width();
        const Int localHeight = this->LocalHeight();
        const Int localWidthOfA = A.LocalWidth();

        const Int maxHeight = MaxLength(height,p);
        const Int maxWidth = MaxLength(width,c);
        const Int portionSize = std::max(maxHeight*maxWidth,mpi::MIN_COLL_MSG);

        this->auxMemory_.Require( 2*c*portionSize );

        T* buffer = this->auxMemory_.Buffer();
        T* sendBuffer = &buffer[0];
        T* recvBuffer = &buffer[c*portionSize];

        // Pack
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<c; ++k )
        {
            T* data = &sendBuffer[k*portionSize];

            const Int thisRank = row+k*r;
            const Int thisColShift = Shift_(thisRank,colAlignment,p); 
            const Int thisColOffset = (thisColShift-colShiftOfA) / r;
            const Int thisLocalHeight = Length_(height,thisColShift,p);

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for 
#endif
            for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
            {
                T* destCol = &data[jLocal*thisLocalHeight];
                const T* sourceCol = &ABuffer[thisColOffset+jLocal*ALDim];
                for( Int iLocal=0; iLocal<thisLocalHeight; ++iLocal )
                    destCol[iLocal] = sourceCol[iLocal*c];
            }
        }

        // Communicate
        mpi::AllToAll
        ( sendBuffer, portionSize,
          recvBuffer, portionSize, g.RowComm() );

        // Unpack
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<c; ++k )
        {
            const T* data = &recvBuffer[k*portionSize];

            const Int thisRowShift = Shift_(k,rowAlignmentOfA,c);
            const Int thisLocalWidth = Length_(width,thisRowShift,c);

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<thisLocalWidth; ++jLocal )
            {
                const T* dataCol = &data[jLocal*localHeight];
                T* thisCol = &thisBuffer[(thisRowShift+jLocal*c)*thisLDim];
                MemCopy( thisCol, dataCol, localHeight );
            }
        }
        this->auxMemory_.Release();
    }
    else
    {
#ifdef UNALIGNED_WARNINGS
        if( g.Rank() == 0 )
            std::cerr << "Unaligned [VC,* ] <- [MC,MR]." << std::endl;
#endif
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = g.Size();
        const Int row = g.Row();
        const Int colShiftOfA = A.ColShift();
        const Int colAlignment = this->ColAlignment();
        const Int colAlignmentOfA = A.ColAlignment();
        const Int rowAlignmentOfA = A.RowAlignment();
        
        const Int sendRow = (row+r+(colAlignment%r)-colAlignmentOfA) % r;
        const Int recvRow = (row+r+colAlignmentOfA-(colAlignment%r)) % r;

        const Int height = this->Height();
        const Int width = this->Width();
        const Int localHeight = this->LocalHeight();
        const Int localWidthOfA = A.LocalWidth();

        const Int maxHeight = MaxLength(height,p);
        const Int maxWidth = MaxLength(width,c);
        const Int portionSize = std::max(maxHeight*maxWidth,mpi::MIN_COLL_MSG);

        this->auxMemory_.Require( 2*c*portionSize );

        T* buffer = this->auxMemory_.Buffer();
        T* firstBuffer = &buffer[0];
        T* secondBuffer = &buffer[c*portionSize];

        // Pack
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<c; ++k )
        {
            T* data = &secondBuffer[k*portionSize];

            const Int thisRank = sendRow+k*r;
            const Int thisColShift = Shift_(thisRank,colAlignment,p);
            const Int thisColOffset = (thisColShift-colShiftOfA) / r;
            const Int thisLocalHeight = Length_(height,thisColShift,p);

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for 
#endif
            for( Int jLocal=0; jLocal<localWidthOfA; ++jLocal )
            {
                T* destCol = &data[jLocal*thisLocalHeight];
                const T* sourceCol = &ABuffer[thisColOffset+jLocal*ALDim];
                for( Int iLocal=0; iLocal<thisLocalHeight; ++iLocal )
                    destCol[iLocal] = sourceCol[iLocal*c];
            }
        }

        // AllToAll to gather all of the unaligned [VC,*] data into firstBuffer
        mpi::AllToAll
        ( secondBuffer, portionSize, 
          firstBuffer,  portionSize, g.RowComm() );

        // SendRecv: properly align the [VC,*] via a trade in the column
        mpi::SendRecv
        ( firstBuffer,  portionSize, sendRow, 0,
          secondBuffer, portionSize, recvRow, mpi::ANY_TAG, g.ColComm() );

        // Unpack
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<c; ++k )
        {
            const T* data = &secondBuffer[k*portionSize];

            const Int thisRowShift = Shift_(k,rowAlignmentOfA,c);
            const Int thisLocalWidth = Length_(width,thisRowShift,c);

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for
#endif
            for( Int jLocal=0; jLocal<thisLocalWidth; ++jLocal )
            {
                const T* dataCol = &data[jLocal*localHeight];
                T* thisCol = &thisBuffer[(thisRowShift+jLocal*c)*thisLDim];
                MemCopy( thisCol, dataCol, localHeight );
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
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,MC,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [MC,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !this->Viewing() )
    {
        if( !this->ConstrainedColAlignment() )
        {
            this->colAlignment_ = A.ColAlignment();
            this->SetColShift();
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

    if( this->ColAlignment() % g.Height() == A.ColAlignment() )
    {
        const Int r = g.Height();
        const Int c = g.Width();
        const Int colShift = this->ColShift();
        const Int colShiftOfA = A.ColShift();
        const Int colOffset = (colShift-colShiftOfA) / r;
        
        const Int width = this->Width();
        const Int localHeight = this->LocalHeight();

        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for 
#endif
        for( Int j=0; j<width; ++j )
        {
            T* destCol = &thisBuffer[j*thisLDim];
            const T* sourceCol = &ABuffer[colOffset+j*ALDim];
            for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                destCol[iLocal] = sourceCol[iLocal*c];
        }
    }
    else
    {
#ifdef UNALIGNED_WARNINGS
        if( g.Rank() == 0 )
            std::cerr << "Unaligned [VC,* ] <- [MC,* ]." << std::endl;
#endif
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = g.Size();
        const Int row = g.Row();
        const Int col = g.Col();
        const Int colShiftOfA = A.ColShift();
        const Int colAlignment = this->ColAlignment();
        const Int colAlignmentOfA = A.ColAlignment();

        // We will SendRecv A[VC,*] within our process column to fix alignments.
        const Int sendRow = (row+r+(colAlignment%r)-colAlignmentOfA) % r;
        const Int recvRow = (row+r+colAlignmentOfA-(colAlignment%r)) % r;
        const Int sendRank = sendRow + r*col;

        const Int sendColShift = Shift( sendRank, colAlignment, p );
        const Int sendColOffset = (sendColShift-colShiftOfA) / r;

        const Int height = this->Height();
        const Int width = this->Width();
        const Int localHeight = this->LocalHeight();
        const Int localHeightOfSend = Length(height,sendColShift,p);

        const Int sendSize = localHeightOfSend * width;
        const Int recvSize = localHeight * width;

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
        for( Int j=0; j<width; ++j )
        {
            T* destCol = &sendBuffer[j*localHeightOfSend];
            const T* sourceCol = &ABuffer[sendColOffset+j*ALDim];
            for( Int iLocal=0; iLocal<localHeightOfSend; ++iLocal )
                destCol[iLocal] = sourceCol[iLocal*c];
        }

        // Communicate
        mpi::SendRecv
        ( sendBuffer, sendSize, sendRow, 0,
          recvBuffer, recvSize, recvRow, mpi::ANY_TAG, g.ColComm() );

        // Unpack
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            const T* recvBufferCol = &recvBuffer[j*localHeight];
            T* thisCol = &thisBuffer[j*thisLDim];
            MemCopy( thisCol, recvBufferCol, localHeight );
        }
        this->auxMemory_.Release();
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,STAR,MR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [* ,MR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    DistMatrix<T,MC,MR,Int> A_MC_MR(g);

    A_MC_MR = A;
    *this = A_MC_MR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,MD,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[VC,* ] = [MD,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    // TODO: Optimize this later if important
    DistMatrix<T,STAR,STAR> A_STAR_STAR( A );
    *this = A_STAR_STAR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,STAR,MD,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [* ,MD]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    // TODO: Optimize this later if important
    DistMatrix<T,STAR,STAR> A_STAR_STAR( A );
    *this = A_STAR_STAR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,MR,MC,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [MR,MC]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    DistMatrix<T,VR,STAR,Int> A_VR_STAR(g);

    A_VR_STAR = A;
    *this = A_VR_STAR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,MR,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [MR,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    DistMatrix<T,VR,STAR,Int> A_VR_STAR(g);

    A_VR_STAR = A;
    *this = A_VR_STAR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,STAR,MC,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [* ,MC]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    std::auto_ptr<DistMatrix<T,MR,MC,Int> > A_MR_MC
    ( new DistMatrix<T,MR,MC,Int>(g) );
    *A_MR_MC = A;

    std::auto_ptr<DistMatrix<T,VR,STAR,Int> > A_VR_STAR
    ( new DistMatrix<T,VR,STAR,Int>(g) );
    *A_VR_STAR = *A_MR_MC;
    delete A_MR_MC.release(); // lowers memory highwater

    *this = *A_VR_STAR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,VC,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [VC,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const Grid& g = this->Grid();
    if( !this->Viewing() )
    {
        if( !this->ConstrainedColAlignment() )
        {
            this->colAlignment_ = A.ColAlignment();
            if( g.InGrid() )
                this->colShift_ = A.ColShift();
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

    if( this->ColAlignment() == A.ColAlignment() )
    {
        this->matrix_ = A.LockedMatrix();
    }
    else
    {
#ifdef UNALIGNED_WARNINGS
        if( g.Rank() == 0 )
            std::cerr << "Unaligned [VC,* ] <- [VC,* ]." << std::endl;
#endif
        const Int rank = g.VCRank();
        const Int p = g.Size();

        const Int colAlignment = this->ColAlignment();
        const Int colAlignmentOfA = A.ColAlignment();

        const Int sendRank = (rank+p+colAlignment-colAlignmentOfA) % p;
        const Int recvRank = (rank+p+colAlignmentOfA-colAlignment) % p;

        const Int width = this->Width();
        const Int localHeight = this->LocalHeight();
        const Int localHeightOfA = A.LocalHeight();

        const Int sendSize = localHeightOfA * width;
        const Int recvSize = localHeight * width;

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
        for( Int j=0; j<width; ++j )
        {
            const T* ACol = &ABuffer[j*ALDim];
            T* sendBufferCol = &sendBuffer[j*localHeightOfA];
            MemCopy( sendBufferCol, ACol, localHeightOfA );
        }

        // Communicate
        mpi::SendRecv
        ( sendBuffer, sendSize, sendRank, 0,
          recvBuffer, recvSize, recvRank, mpi::ANY_TAG, g.VCComm() );

        // Unpack
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            const T* recvBufferCol = &recvBuffer[j*localHeight];
            T* thisCol = &thisBuffer[j*thisLDim];
            MemCopy( thisCol, recvBufferCol, localHeight );
        }
        this->auxMemory_.Release();
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,STAR,VC,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [* ,VC]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    std::auto_ptr<DistMatrix<T,MR,MC,Int> > A_MR_MC
    ( new DistMatrix<T,MR,MC,Int>(g) );
    *A_MR_MC = A;

    std::auto_ptr<DistMatrix<T,VC,STAR,Int> > A_VR_STAR
    ( new DistMatrix<T,VC,STAR,Int>(g) );
    *A_VR_STAR = *A_MR_MC; 
    delete A_MR_MC.release(); // lowers memory highwater

    *this = *A_VR_STAR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,VR,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [VR,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const Grid& g = this->Grid();
    if( !this->Viewing() )
        this->ResizeTo( A.Height(), A.Width() );
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return *this;
    }
    
    const Int width = this->Width();
    const Int localHeight = this->LocalHeight();
    const Int localHeightOfA = A.LocalHeight();

    const Int sendSize = localHeightOfA * width;
    const Int recvSize = localHeight * width;

    const Int r = g.Height();
    const Int c = g.Width();
    const Int p = g.Size();
    const Int rankCM = g.VCRank();
    const Int rankRM = g.VRRank();

    const Int colShift = this->ColShift();
    const Int colShiftOfA = A.ColShift();

    // Compute which colmajor rank has the colShift equal to our colShiftOfA
    const Int sendRankCM = (rankCM+(p+colShiftOfA-colShift)) % p;

    // Compute which colmajor rank has the A colShift that we need
    const Int recvRankRM = (rankRM+(p+colShift-colShiftOfA)) % p;
    const Int recvRankCM = (recvRankRM/c)+r*(recvRankRM%c);

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
    for( Int j=0; j<width; ++j )
    {
        const T* ACol = &ABuffer[j*ALDim];
        T* sendBufferCol = &sendBuffer[j*localHeightOfA];
        MemCopy( sendBufferCol, ACol, localHeightOfA );
    }

    // Communicate
    mpi::SendRecv
    ( sendBuffer, sendSize, sendRankCM, 0,
      recvBuffer, recvSize, recvRankCM, mpi::ANY_TAG, g.VCComm() );

    // Unpack
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j=0; j<width; ++j )
    {
        const T* recvBufferCol = &recvBuffer[j*localHeight];
        T* thisCol = &thisBuffer[j*thisLDim];
        MemCopy( thisCol, recvBufferCol, localHeight );
    }

    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,STAR,VR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ] = [* ,VR]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    DistMatrix<T,MC,MR,Int> A_MC_MR(g);

    A_MC_MR = A;
    *this = A_MC_MR;
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
const DistMatrix<T,VC,STAR,Int>&
DistMatrix<T,VC,STAR,Int>::operator=( const DistMatrix<T,STAR,STAR,Int>& A )
{
#ifndef RELEASE
    PushCallStack("[VC,* ] = [* ,* ]");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const Grid& g = this->Grid();
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
    const Int colShift = this->ColShift();

    const Int localHeight = this->LocalHeight();
    const Int width = this->Width();

    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for 
#endif
    for( Int j=0; j<width; ++j )
    {
        T* destCol = &thisBuffer[j*thisLDim];
        const T* sourceCol = &ABuffer[colShift+j*ALDim];
        for( Int iLocal=0; iLocal<localHeight; ++iLocal )
            destCol[iLocal] = sourceCol[iLocal*p];
    }
#ifndef RELEASE
    PopCallStack();
#endif
    return *this;
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SumScatterFrom
( const DistMatrix<T,MC,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ]::SumScatterFrom( [MC,* ] )");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    if( this->Viewing() )
        this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
#ifdef CACHE_WARNINGS
    if( A.Width() != 1 && A.Grid().Rank() == 0 )
    {
        std::cerr <<
          "[VC,* ]::SumScatterFrom([MC,* ]) potentially causes a large amount "
          "of cache-thrashing. If possible, avoid it by forming the "
          "(conjugate-)transpose of the [MC,* ] matrix instead." << std::endl;
    }
#endif
    if( !this->Viewing() )
    {
        if( !this->ConstrainedColAlignment() )
        {
            this->colAlignment_ = A.ColAlignment();
            this->SetColShift();
        }
        this->ResizeTo( A.Height(), A.Width() );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    if( this->ColAlignment() % g.Height() == A.ColAlignment() )
    {
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = r * c;
        const Int myRow = g.Row();
        const Int colAlignment = this->ColAlignment();
        const Int colShiftOfA = A.ColShift();

        const Int height = this->Height();
        const Int width = this->Width();
        const Int localHeight = this->LocalHeight();
        const Int maxLocalHeight = MaxLength( height, p );

        const Int recvSize = std::max(maxLocalHeight*width,mpi::MIN_COLL_MSG);
        const Int sendSize = c*recvSize;

        this->auxMemory_.Require( sendSize );
        T* buffer = this->auxMemory_.Buffer();

        // Pack
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<c; ++k )
        {
            T* data = &buffer[k*recvSize];

            const Int thisRank = myRow+k*r;
            const Int thisColShift = Shift_( thisRank, colAlignment, p );
            const Int thisColOffset = (thisColShift-colShiftOfA) / r;
            const Int thisLocalHeight = Length_( height, thisColShift, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for 
#endif
            for( Int j=0; j<width; ++j )
            {
                T* destCol = &data[j*thisLocalHeight];
                const T* sourceCol = &ABuffer[thisColOffset+j*ALDim];
                for( Int iLocal=0; iLocal<thisLocalHeight; ++iLocal )
                    destCol[iLocal] = sourceCol[iLocal*c];
            }
        }

        // Communicate
        mpi::ReduceScatter( buffer, recvSize, mpi::SUM, g.RowComm() );

        // Unpack our received data
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            const T* bufferCol = &buffer[j*localHeight];
            T* thisCol = &thisBuffer[j*thisLDim];
            MemCopy( thisCol, bufferCol, localHeight );
        }
        this->auxMemory_.Release();
    }
    else
    {
        throw std::logic_error
        ("Unaligned [VC,* ]::ReduceScatterFrom( [MC,* ] ) not implemented");
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SumScatterFrom
( const DistMatrix<T,STAR,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ]::SumScatterFrom( [* ,* ] )");
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
        return;
    }

    const Int p = g.Size();
    const Int colAlignment = this->ColAlignment();

    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeight = this->LocalHeight();
    const Int maxLocalHeight = MaxLength( height, p );

    const Int recvSize = std::max(maxLocalHeight*width,mpi::MIN_COLL_MSG);
    const Int sendSize = p*recvSize;

    this->auxMemory_.Require( sendSize );
    T* buffer = this->auxMemory_.Buffer();

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<p; ++k )
    {
        T* data = &buffer[k*recvSize];

        const Int thisColShift = Shift_( k, colAlignment, p );
        const Int thisLocalHeight = Length_( height, thisColShift, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            T* destCol = &data[j*thisLocalHeight];
            const T* sourceCol = &ABuffer[thisColShift+j*ALDim];
            for( Int iLocal=0; iLocal<thisLocalHeight; ++iLocal )
                destCol[iLocal] = sourceCol[iLocal*p];
        }
    }

    // Communicate
    mpi::ReduceScatter( buffer, recvSize, mpi::SUM, g.VCComm() );

    // Unpack our received data
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j=0; j<width; ++j )
    {
        const T* bufferCol = &buffer[j*localHeight];
        T* thisCol = &thisBuffer[j*thisLDim];
        MemCopy( thisCol, bufferCol, localHeight );
    }
    this->auxMemory_.Release();
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SumScatterUpdate
( T alpha, const DistMatrix<T,MC,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ]::SumScatterUpdate( [MC,* ] )");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

#ifdef CACHE_WARNINGS
    if( A.Width() != 1 && A.Grid().Rank() == 0 )
    {
        std::cerr <<
          "[VC,* ]::SumScatterUpdate([MC,* ]) potentially causes a large amount"
          " of cache-thrashing. If possible, avoid it by forming the "
          "(conjugate-)transpose of the [MC,* ] matrix instead." << std::endl;
    }
#endif
    if( this->ColAlignment() % g.Height() == A.ColAlignment() )
    {
        const Int r = g.Height();
        const Int c = g.Width();
        const Int p = r * c;
        const Int myRow = g.Row();
        const Int colAlignment = this->ColAlignment();
        const Int colShiftOfA = A.ColShift();

        const Int height = this->Height();
        const Int width = this->Width();
        const Int localHeight = this->LocalHeight();
        const Int maxLocalHeight = MaxLength( height, p );

        const Int recvSize = std::max(maxLocalHeight*width,mpi::MIN_COLL_MSG);
        const Int sendSize = c*recvSize;

        this->auxMemory_.Require( sendSize );
        T* buffer = this->auxMemory_.Buffer();

        // Pack
        const T* ABuffer = A.LockedBuffer();
        const Int ALDim = A.LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int k=0; k<c; ++k )
        {
            T* data = &buffer[k*recvSize];

            const Int thisRank = myRow+k*r;
            const Int thisColShift = Shift_( thisRank, colAlignment, p );
            const Int thisColOffset = (thisColShift-colShiftOfA) / r;
            const Int thisLocalHeight = Length_( height, thisColShift, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
            #pragma omp parallel for 
#endif
            for( Int j=0; j<width; ++j )
            {
                T* destCol = &data[j*thisLocalHeight];
                const T* sourceCol = &ABuffer[thisColOffset+j*ALDim];
                for( Int iLocal=0; iLocal<thisLocalHeight; ++iLocal )
                    destCol[iLocal] = sourceCol[iLocal*c];
            }
        }

        // Communicate
        mpi::ReduceScatter( buffer, recvSize, mpi::SUM, g.RowComm() );

        // Unpack our received data
        T* thisBuffer = this->Buffer();
        const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            const T* bufferCol = &buffer[j*localHeight];
            T* thisCol = &thisBuffer[j*thisLDim];
            for( Int iLocal=0; iLocal<localHeight; ++iLocal )
                thisCol[iLocal] += alpha*bufferCol[iLocal];
        }
        this->auxMemory_.Release();
    }
    else
    {
        throw std::logic_error
        ("Unaligned [VC,* ]::ReduceScatterUpdate( [MC,* ] ) not implemented");
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SumScatterUpdate
( T alpha, const DistMatrix<T,STAR,STAR,Int>& A )
{ 
#ifndef RELEASE
    PushCallStack("[VC,* ]::SumScatterUpdate( [* ,* ] )");
    this->AssertNotLocked();
    this->AssertSameGrid( A.Grid() );
    this->AssertSameSize( A.Height(), A.Width() );
#endif
    const elem::Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colAlignment = this->ColAlignment();

    const Int height = this->Height();
    const Int width = this->Width();
    const Int localHeight = this->LocalHeight();
    const Int maxLocalHeight = MaxLength( height, p );

    const Int recvSize = std::max(maxLocalHeight*width,mpi::MIN_COLL_MSG);
    const Int sendSize = p*recvSize;

    this->auxMemory_.Require( sendSize );
    T* buffer = this->auxMemory_.Buffer();

    // Pack
    const T* ABuffer = A.LockedBuffer();
    const Int ALDim = A.LDim();
#if defined(HAVE_OPENMP) && !defined(PARALLELIZE_INNER_LOOPS)
    #pragma omp parallel for
#endif
    for( Int k=0; k<p; ++k )
    {
        T* data = &buffer[k*recvSize];

        const Int thisColShift = Shift_( k, colAlignment, p );
        const Int thisLocalHeight = Length_( height, thisColShift, p );

#if defined(HAVE_OPENMP) && defined(PARALLELIZE_INNER_LOOPS)
        #pragma omp parallel for
#endif
        for( Int j=0; j<width; ++j )
        {
            T* destCol = &data[j*thisLocalHeight];
            const T* sourceCol = &ABuffer[thisColShift+j*ALDim];
            for( Int iLocal=0; iLocal<thisLocalHeight; ++iLocal )
                destCol[iLocal] = sourceCol[iLocal*p];
        }
    }

    // Communicate
    mpi::ReduceScatter( buffer, recvSize, mpi::SUM, g.VCComm() );

    // Unpack our received data
    T* thisBuffer = this->Buffer();
    const Int thisLDim = this->LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j=0; j<width; ++j )
    {
        const T* bufferCol = &buffer[j*localHeight];
        T* thisCol = &thisBuffer[j*thisLDim];
        for( Int iLocal=0; iLocal<localHeight; ++iLocal )
            thisCol[iLocal] += alpha*bufferCol[iLocal];
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
DistMatrix<T,VC,STAR,Int>::GetRealPart( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::GetRealPart");
    this->AssertValidEntry( i, j );
#endif
    typedef typename Base<T>::type R;

    // We will determine the owner rank of entry (i,j) and broadcast from that
    // process over the entire g
    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    R u;
    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        u = this->GetLocalRealPart(iLoc,j);
    }
    mpi::Broadcast( &u, 1, g.VCToViewingMap(ownerRank), g.ViewingComm() );
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
typename Base<T>::type
DistMatrix<T,VC,STAR,Int>::GetImagPart( Int i, Int j ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::GetImagPart");
    this->AssertValidEntry( i, j );
#endif
    typedef typename Base<T>::type R;

    // We will determine the owner rank of entry (i,j) and broadcast from that
    // process over the entire g
    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    R u;
    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        u = this->GetLocalImagPart(iLoc,j);
    }
    mpi::Broadcast( &u, 1, g.VCToViewingMap(ownerRank), g.ViewingComm() );
#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SetRealPart( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::SetRealPart");
    this->AssertValidEntry( i, j );
#endif
    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        this->SetLocalRealPart(iLoc,j,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SetImagPart( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::SetImagPart");
    this->AssertValidEntry( i, j );
#endif
    if( !IsComplex<T>::val )
        throw std::logic_error("Called complex-only routine with real data");

    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        this->SetLocalImagPart(iLoc,j,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::UpdateRealPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::UpdateRealPart");
    this->AssertValidEntry( i, j );
#endif
    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        this->UpdateLocalRealPart(iLoc,j,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::UpdateImagPart
( Int i, Int j, typename Base<T>::type u )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::UpdateImagPart");
    this->AssertValidEntry( i, j );
#endif
    if( !IsComplex<T>::val )
        throw std::logic_error("Called complex-only routine with real data");

    const elem::Grid& g = this->Grid();
    const Int ownerRank = (i + this->ColAlignment()) % g.Size();

    if( g.VCRank() == ownerRank )
    {
        const Int iLoc = (i-this->ColShift()) / g.Size();
        this->UpdateLocalImagPart(iLoc,j,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::GetRealPartOfDiagonal
( DistMatrix<typename Base<T>::type,VC,STAR,Int>& d, Int offset ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::GetRealPartOfDiagonal");
    if( d.Viewing() )
        this->AssertSameGrid( d.Grid() );
#endif
    const Int length = this->DiagonalLength(offset);
#ifndef RELEASE
    if( d.Viewing() && (length != d.Height() || d.Width() != 1) )
    {
        std::ostringstream msg;
        msg << "d is not a column vec of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << length << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( ( d.Viewing() || d.ConstrainedColAlignment() ) &&
        !d.AlignedWithDiagonal( this->DistData(), offset ) )
        throw std::logic_error("d must be aligned with the offset diag");
#endif
    typedef typename Base<T>::type R;

    const elem::Grid& g = this->Grid();
    if( !d.Viewing() )
    {
        d.SetGrid( g );
        if( !d.ConstrainedColAlignment() )
            d.AlignWithDiagonal( this->DistData(), offset );
        d.ResizeTo( length, 1 );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.ColShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift) / p;
    const Int localDiagLength = d.LocalHeight();
    const T* thisBuffer = this->LockedBuffer();
    const Int thisLDim = this->LDim();
    R* dBuffer = d.Buffer();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        dBuffer[k] = RealPart( thisBuffer[iLocal+jLocal*thisLDim] );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::GetImagPartOfDiagonal
( DistMatrix<typename Base<T>::type,VC,STAR,Int>& d, Int offset ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::GetImagPartOfDiagonal");
    if( d.Viewing() )
        this->AssertSameGrid( d.Grid() );
#endif
    const Int length = this->DiagonalLength(offset);
#ifndef RELEASE
    if( d.Viewing() && (length != d.Height() || d.Width() != 1) )
    {
        std::ostringstream msg;
        msg << "d is not a column vec of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << length << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( ( d.Viewing() || d.ConstrainedColAlignment() ) &&
        !d.AlignedWithDiagonal( this->DistData(), offset ) )
        throw std::logic_error("d must be aligned with the offset diag");
#endif
    typedef typename Base<T>::type R;

    const elem::Grid& g = this->Grid();
    if( !d.Viewing() )
    {
        d.SetGrid( g );
        if( !d.ConstrainedColAlignment() )
            d.AlignWithDiagonal( this->DistData(), offset );
        d.ResizeTo( length, 1 );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.ColShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift) / p;
    const Int localDiagLength = d.LocalHeight();
    const T* thisBuffer = this->LockedBuffer();
    const Int thisLDim = this->LDim();
    R* dBuffer = d.Buffer();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        dBuffer[k] = ImagPart( thisBuffer[iLocal+jLocal*thisLDim] );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::GetRealPartOfDiagonal
( DistMatrix<typename Base<T>::type,STAR,VC,Int>& d, Int offset ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::GetRealPartOfDiagonal");
    if( d.Viewing() )
        this->AssertSameGrid( d.Grid() );
#endif
    const Int length = this->DiagonalLength(offset);
#ifndef RELEASE
    if( d.Viewing() && length != d.Width() )
    {
        std::ostringstream msg;
        msg << "d is not of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << length << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( ( d.Viewing() || d.ConstrainedRowAlignment() ) &&
        !d.AlignedWithDiagonal( this->DistData(), offset ) )
        throw std::logic_error("d must be aligned with the offset diag");
#endif
    typedef typename Base<T>::type R;

    const elem::Grid& g = this->Grid();
    if( !d.Viewing() )
    {
        d.SetGrid( g );
        if( !d.ConstrainedRowAlignment() )
            d.AlignWithDiagonal( this->DistData(), offset );
        d.ResizeTo( 1, length );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.RowShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift) / p;
    const Int localDiagLength = d.LocalWidth();
    const T* thisBuffer = this->LockedBuffer();
    const Int thisLDim = this->LDim();
    R* dBuffer = d.Buffer();
    const Int dLDim = d.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        dBuffer[k*dLDim] = RealPart( thisBuffer[iLocal+jLocal*thisLDim] );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::GetImagPartOfDiagonal
( DistMatrix<typename Base<T>::type,STAR,VC,Int>& d, Int offset ) const
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::GetImagPartOfDiagonal");
    if( d.Viewing() )
        this->AssertSameGrid( d.Grid() );
#endif
    const Int length = this->DiagonalLength(offset);
#ifndef RELEASE
    if( d.Viewing() && length != d.Width() )
    {
        std::ostringstream msg;
        msg << "d is not of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << length << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( ( d.Viewing() || d.ConstrainedRowAlignment() ) &&
        !d.AlignedWithDiagonal( this->DistData(), offset ) )
        throw std::logic_error("d must be aligned with the offset diag");
#endif
    typedef typename Base<T>::type R;

    const elem::Grid& g = this->Grid();
    if( !d.Viewing() )
    {
        d.SetGrid( g );
        if( !d.ConstrainedRowAlignment() )
            d.AlignWithDiagonal( this->DistData(), offset );
        d.ResizeTo( 1, length );
    }
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.RowShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift) / p;
    const Int localDiagLength = d.LocalWidth();
    const T* thisBuffer = this->LockedBuffer();
    const Int thisLDim = this->LDim();
    R* dBuffer = d.Buffer();
    const Int dLDim = d.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        dBuffer[k*dLDim] = ImagPart( thisBuffer[iLocal+jLocal*thisLDim] );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SetRealPartOfDiagonal
( const DistMatrix<typename Base<T>::type,VC,STAR,Int>& d, Int offset )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::SetRealPartOfDiagonal");
    this->AssertSameGrid( d.Grid() );
    if( d.Width() != 1 )
        throw std::logic_error("d must be a column vector");
    const Int length = this->DiagonalLength(offset);
    if( length != d.Height() )
    {
        std::ostringstream msg;
        msg << "d is not of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << length << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( !d.AlignedWithDiagonal( this->DistData(), offset ) )
        throw std::logic_error("d must be aligned with the 'offset' diagonal");
#endif
    typedef typename Base<T>::type R;

    const elem::Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.ColShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift)/p;
    const Int localDiagLength = d.LocalHeight();
    const R* dBuffer = d.LockedBuffer();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        this->SetLocalRealPart( iLocal, jLocal, dBuffer[k] );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SetImagPartOfDiagonal
( const DistMatrix<typename Base<T>::type,VC,STAR,Int>& d, Int offset )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::SetImagPartOfDiagonal");
    this->AssertSameGrid( d.Grid() );
    if( d.Width() != 1 )
        throw std::logic_error("d must be a column vector");
    const Int length = this->DiagonalLength(offset);
    if( length != d.Height() )
    {
        std::ostringstream msg;
        msg << "d is not of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << length << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( !d.AlignedWithDiagonal( this->DistData(), offset ) )
        throw std::logic_error("d must be aligned with the 'offset' diagonal");
#endif
    typedef typename Base<T>::type R;
    if( !IsComplex<T>::val )
        throw std::logic_error("Called complex-only routine with real data");

    const elem::Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.ColShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift)/p;
    const Int localDiagLength = d.LocalHeight();
    const R* dBuffer = d.LockedBuffer();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        this->SetLocalImagPart( iLocal, jLocal, dBuffer[k] );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SetRealPartOfDiagonal
( const DistMatrix<typename Base<T>::type,STAR,VC,Int>& d, Int offset )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::SetRealPartOfDiagonal");
    this->AssertSameGrid( d.Grid() );
    if( d.Height() != 1 )
        throw std::logic_error("d must be a row vector");
    const Int length = this->DiagonalLength(offset);
    if( length != d.Width() )
    {
        std::ostringstream msg;
        msg << "d is not of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << length << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( !d.AlignedWithDiagonal( this->DistData(), offset ) )
        throw std::logic_error("d must be aligned with the 'offset' diagonal");
#endif
    typedef typename Base<T>::type R;

    const elem::Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.RowShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift)/p;
    const Int localDiagLength = d.LocalWidth();
    const R* dBuffer = d.LockedBuffer();
    const Int dLDim = d.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        this->SetLocalRealPart( iLocal, jLocal, dBuffer[k*dLDim] );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,typename Int>
void
DistMatrix<T,VC,STAR,Int>::SetImagPartOfDiagonal
( const DistMatrix<typename Base<T>::type,STAR,VC,Int>& d, Int offset )
{
#ifndef RELEASE
    PushCallStack("[VC,* ]::SetImagPartOfDiagonal");
    this->AssertSameGrid( d.Grid() );
    if( d.Height() != 1 )
        throw std::logic_error("d must be a row vector");
    const Int length = this->DiagonalLength(offset);
    if( length != d.Width() )
    {
        std::ostringstream msg;
        msg << "d is not of the same length as the diagonal:\n"
            << "  A ~ " << this->Height() << " x " << this->Width() << "\n"
            << "  d ~ " << d.Height() << " x " << d.Width() << "\n"
            << "  A diag length: " << length << "\n";
        throw std::logic_error( msg.str().c_str() );
    }
    if( !d.AlignedWithDiagonal( this->DistData(), offset ) )
        throw std::logic_error("d must be aligned with the 'offset' diagonal");
#endif
    typedef typename Base<T>::type R;
    if( !IsComplex<T>::val )
        throw std::logic_error("Called complex-only routine with real data");

    const elem::Grid& g = this->Grid();
    if( !g.InGrid() )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const Int p = g.Size();
    const Int colShift = this->ColShift();
    const Int diagShift = d.RowShift();

    Int iStart, jStart;
    if( offset >= 0 )
    {
        iStart = diagShift;
        jStart = diagShift+offset;
    }
    else
    {
        iStart = diagShift-offset;
        jStart = diagShift;
    }

    const Int iLocalStart = (iStart-colShift)/p;
    const Int localDiagLength = d.LocalWidth();
    const R* dBuffer = d.LockedBuffer();
    const Int dLDim = d.LDim();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int k=0; k<localDiagLength; ++k )
    {
        const Int iLocal = iLocalStart+k;
        const Int jLocal = jStart+k*p;
        this->SetLocalImagPart( iLocal, jLocal, dBuffer[k*dLDim] );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template class DistMatrix<int,VC,STAR,int>;
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,MC,  MR,  int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,MC,  STAR,int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,MD,  STAR,int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,MR,  MC,  int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,MR,  STAR,int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,STAR,MC,  int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,STAR,MD,  int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,STAR,MR,  int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,STAR,STAR,int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,STAR,VC,  int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,STAR,VR,  int>& A );
template DistMatrix<int,VC,STAR,int>::DistMatrix( const DistMatrix<int,VR,  STAR,int>& A );

#ifndef DISABLE_FLOAT
template class DistMatrix<float,VC,STAR,int>;
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,MC,  MR,  int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,MC,  STAR,int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,MD,  STAR,int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,MR,  MC,  int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,MR,  STAR,int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,STAR,MC,  int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,STAR,MD,  int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,STAR,MR,  int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,STAR,STAR,int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,STAR,VC,  int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,STAR,VR,  int>& A );
template DistMatrix<float,VC,STAR,int>::DistMatrix( const DistMatrix<float,VR,  STAR,int>& A );
#endif // ifndef DISABLE_FLOAT

template class DistMatrix<double,VC,STAR,int>;
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,MC,  MR,  int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,MC,  STAR,int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,MD,  STAR,int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,MR,  MC,  int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,MR,  STAR,int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,STAR,MC,  int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,STAR,MD,  int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,STAR,MR,  int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,STAR,STAR,int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,STAR,VC,  int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,STAR,VR,  int>& A );
template DistMatrix<double,VC,STAR,int>::DistMatrix( const DistMatrix<double,VR,  STAR,int>& A );

#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
template class DistMatrix<Complex<float>,VC,STAR,int>;
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MC,  MR,  int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MC,  STAR,int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MD,  STAR,int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MR,  MC,  int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,MR,  STAR,int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,MC,  int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,MD,  int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,MR,  int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,STAR,int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,VC,  int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,STAR,VR,  int>& A );
template DistMatrix<Complex<float>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<float>,VR,  STAR,int>& A );
#endif // ifndef DISABLE_FLOAT
template class DistMatrix<Complex<double>,VC,STAR,int>;
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MC,  MR,  int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MC,  STAR,int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MD,  STAR,int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MR,  MC,  int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,MR,  STAR,int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,MC,  int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,MD,  int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,MR,  int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,STAR,int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,VC,  int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,STAR,VR,  int>& A );
template DistMatrix<Complex<double>,VC,STAR,int>::DistMatrix( const DistMatrix<Complex<double>,VR,  STAR,int>& A );
#endif // ifndef DISABLE_COMPLEX

} // namespace elem
