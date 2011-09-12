/*
   Copyright (c) 2009-2011, Jack Poulson
   All rights reserved.

   This file is part of Elemental.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    - Neither the name of the owner nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/

namespace elemental {
using namespace std;
using namespace elemental::imports;

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetToRandomHermitianHelper<Z>::Func
( DistMatrix<Z,MR,MC>& parent )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetToRandomHermitian");
    parent.AssertNotLockedView();
    if( parent.Height() != parent.Width() )
        throw logic_error( "Hermitian matrices must be square." );
#endif
    parent.SetToRandom();
#ifndef RELEASE
    PopCallStack();
#endif
}

#ifndef WITHOUT_COMPLEX
template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetToRandomHermitianHelper<complex<Z> >::Func
( DistMatrix<complex<Z>,MR,MC>& parent )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetToRandomHermitian");
    parent.AssertNotLockedView();
    if( parent.Height() != parent.Width() )
        throw logic_error( "Hermitian matrices must be square." );
#endif
    const elemental::Grid& g = parent.Grid();

    const int localHeight = parent.LocalHeight();
    const int localWidth = parent.LocalWidth();
    const int r = g.Height();
    const int c = g.Width();
    const int colShift = parent.ColShift();
    const int rowShift = parent.RowShift();

    parent.SetToRandom();

    complex<Z>* thisLocalBuffer = parent.LocalBuffer();
    const int thisLDim = parent.LocalLDim();
#ifdef _OPENMP
    #pragma omp parallel for
#endif
    for( int iLocal=0; iLocal<localHeight; ++iLocal )
    {
        const int i = colShift + iLocal*c;
        if( i % r == rowShift )
        {
            const int jLocal = (i-rowShift) / r;
            if( jLocal < localWidth )
            {
                const Z value = real(thisLocalBuffer[iLocal+jLocal*thisLDim]);
                thisLocalBuffer[iLocal+jLocal*thisLDim] = value;
            }
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}
#endif // WITHOUT_COMPLEX

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetToRandomHPDHelper<Z>::Func
( DistMatrix<Z,MR,MC>& parent )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetToRandomHPD");
    parent.AssertNotLockedView();
    if( parent.Height() != parent.Width() )
        throw logic_error( "Positive-definite matrices must be square." );
#endif
    const elemental::Grid& g = parent.Grid();

    const int width = parent.Width();
    const int localHeight = parent.LocalHeight();
    const int localWidth = parent.LocalWidth();
    const int r = g.Height();
    const int c = g.Width();
    const int colShift = parent.ColShift();
    const int rowShift = parent.RowShift();

    parent.SetToRandom();

    Z* thisLocalBuffer = parent.LocalBuffer();
    const int thisLDim = parent.LocalLDim();
#ifdef _OPENMP
    #pragma omp parallel for
#endif
    for( int iLocal=0; iLocal<localHeight; ++iLocal )
    {
        const int i = colShift + iLocal*c;
        if( i % r == rowShift )
        {
            const int jLocal = (i-rowShift) / r;
            if( jLocal < localWidth )
                thisLocalBuffer[iLocal+jLocal*thisLDim] += width;
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

#ifndef WITHOUT_COMPLEX
template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetToRandomHPDHelper<complex<Z> >::Func
( DistMatrix<complex<Z>,MR,MC>& parent )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetToRandomHPD");
    parent.AssertNotLockedView();
    if( parent.Height() != parent.Width() )
        throw logic_error( "Positive-definite matrices must be square." );
#endif
    const elemental::Grid& g = parent.Grid();

    const int width = parent.Width();
    const int localHeight = parent.LocalHeight();
    const int localWidth = parent.LocalWidth();
    const int r = g.Height();
    const int c = g.Width();
    const int colShift = parent.ColShift();
    const int rowShift = parent.RowShift();

    parent.SetToRandom();

    complex<Z>* thisLocalBuffer = parent.LocalBuffer();
    const int thisLDim = parent.LocalLDim();
#ifdef _OPENMP
    #pragma omp parallel for
#endif
    for( int iLocal=0; iLocal<localHeight; ++iLocal )
    {
        const int i = colShift + iLocal*c;
        if( i % r == rowShift )
        {
            const int jLocal = (i-rowShift) / r;
            if( jLocal < localWidth )
            {
                const Z value = real(thisLocalBuffer[iLocal+jLocal*thisLDim]);
                thisLocalBuffer[iLocal+jLocal*thisLDim] = value + width;
            }
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline Z
DistMatrix<T,MR,MC>::GetRealHelper<complex<Z> >::Func
( const DistMatrix<complex<Z>,MR,MC>& parent, int i, int j ) 
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::GetReal");
    parent.AssertValidEntry( i, j );
#endif
    // We will determine the owner of the (i,j) entry and have him Broadcast
    // throughout the entire process grid
    const elemental::Grid& g = parent.Grid();
    const int ownerRow = (j + parent.RowAlignment()) % g.Height();
    const int ownerCol = (i + parent.ColAlignment()) % g.Width();
    const int ownerRank = ownerRow + ownerCol * g.Height();

    Z u;
    if( g.VCRank() == ownerRank )
    {
        const int iLoc = (i-parent.ColShift()) / g.Width();
        const int jLoc = (j-parent.RowShift()) / g.Height();
        u = parent.GetRealLocalEntry(iLoc,jLoc);
    }
    mpi::Broadcast( &u, 1, ownerRank, g.VCComm() );

#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T>
template<typename Z>
inline Z
DistMatrix<T,MR,MC>::GetImagHelper<complex<Z> >::Func
( const DistMatrix<complex<Z>,MR,MC>& parent, int i, int j ) 
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::GetImag");
    parent.AssertValidEntry( i, j );
#endif
    // We will determine the owner of the (i,j) entry and have him Broadcast
    // throughout the entire process grid
    const elemental::Grid& g = parent.Grid();
    const int ownerRow = (j + parent.RowAlignment()) % g.Height();
    const int ownerCol = (i + parent.ColAlignment()) % g.Width();
    const int ownerRank = ownerRow + ownerCol * g.Height();

    Z u;
    if( g.VCRank() == ownerRank )
    {
        const int iLoc = (i-parent.ColShift()) / g.Width();
        const int jLoc = (j-parent.RowShift()) / g.Height();
        u = parent.GetImagLocalEntry(iLoc,jLoc);
    }
    mpi::Broadcast( &u, 1, ownerRank, g.VCComm() );

#ifndef RELEASE
    PopCallStack();
#endif
    return u;
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetRealHelper<complex<Z> >::Func
( DistMatrix<complex<Z>,MR,MC>& parent, int i, int j, Z u )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetReal");
    parent.AssertValidEntry( i, j );
#endif
    const elemental::Grid& g = parent.Grid();
    const int ownerRow = (j + parent.RowAlignment()) % g.Height();
    const int ownerCol = (i + parent.ColAlignment()) % g.Width();
    const int ownerRank = ownerRow + ownerCol * g.Height();

    if( g.VCRank() == ownerRank )
    {
        const int iLoc = (i-parent.ColShift()) / g.Width();
        const int jLoc = (j-parent.RowShift()) / g.Height();
        parent.SetRealLocalEntry(iLoc,jLoc,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetImagHelper<complex<Z> >::Func
( DistMatrix<complex<Z>,MR,MC>& parent, int i, int j, Z u )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetImag");
    parent.AssertValidEntry( i, j );
#endif
    const elemental::Grid& g = parent.Grid();
    const int ownerRow = (j + parent.RowAlignment()) % g.Height();
    const int ownerCol = (i + parent.ColAlignment()) % g.Width();
    const int ownerRank = ownerRow + ownerCol * g.Height();

    if( g.VCRank() == ownerRank )
    {
        const int iLoc = (i-parent.ColShift()) / g.Width();
        const int jLoc = (j-parent.RowShift()) / g.Height();
        parent.SetImagLocalEntry(iLoc,jLoc,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::UpdateRealHelper<complex<Z> >::Func
( DistMatrix<complex<Z>,MR,MC>& parent, int i, int j, Z u )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::UpdateReal");
    parent.AssertValidEntry( i, j );
#endif
    const elemental::Grid& g = parent.Grid();
    const int ownerRow = (j + parent.RowAlignment()) % g.Height();
    const int ownerCol = (i + parent.ColAlignment()) % g.Width();
    const int ownerRank = ownerRow + ownerCol * g.Height();

    if( g.VCRank() == ownerRank )
    {
        const int iLoc = (i-parent.ColShift()) / g.Width();
        const int jLoc = (j-parent.RowShift()) / g.Height();
        parent.UpdateRealLocalEntry(iLoc,jLoc,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::UpdateImagHelper<complex<Z> >::Func
( DistMatrix<complex<Z>,MR,MC>& parent, int i, int j, Z u )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::UpdateImag");
    parent.AssertValidEntry( i, j );
#endif
    const elemental::Grid& g = parent.Grid();
    const int ownerRow = (j + parent.RowAlignment()) % g.Height();
    const int ownerCol = (i + parent.ColAlignment()) % g.Width();
    const int ownerRank = ownerRow + ownerCol * g.Height();

    if( g.VCRank() == ownerRank )
    {
        const int iLoc = (i-parent.ColShift()) / g.Width();
        const int jLoc = (j-parent.RowShift()) / g.Height();
        parent.UpdateImagLocalEntry(iLoc,jLoc,u);
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::GetRealDiagonalHelper<complex<Z> >::Func
( const DistMatrix<complex<Z>,MR,MC>& parent, 
        DistMatrix<Z,MD,STAR>& d, int offset ) 
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::GetRealDiagonal([MD,* ])");
    parent.AssertNotLockedView();
#endif
    int height = parent.Height();
    int width = parent.Width();
    int length;
    if( offset > 0 )
    {
        const int remainingWidth = max(width-offset,0);
        length = min(height,remainingWidth);
    }
    else
    {
        const int remainingHeight = max(height+offset,0);
        length = min(remainingHeight,width);
    }
#ifndef RELEASE
    if( d.Viewing() && length != d.Height() )
        throw logic_error( "d is not of the correct length." );
#endif
    if( !d.Viewing() )
    {
        if( !d.ConstrainedColAlignment() )
            d.AlignWithDiag( parent, offset );
        d.ResizeTo( length, 1 );
    }

    if( d.InDiagonal() )
    {
        const elemental::Grid& g = parent.Grid();
        const int r = g.Height();
        const int c = g.Width();
        const int lcm = g.LCM();
        const int colShift = parent.ColShift();
        const int rowShift = parent.RowShift();
        const int diagShift = d.ColShift();

        int iStart,jStart;
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

        const int iLocalStart = (iStart-colShift) / c;
        const int jLocalStart = (jStart-rowShift) / r;

        const int localDiagLength = d.LocalHeight();

        Z* dLocalBuffer = d.LocalBuffer();
        const complex<Z>* thisLocalBuffer = parent.LockedLocalBuffer();
        const int thisLDim = parent.LocalLDim();
#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for( int k=0; k<localDiagLength; ++k )
        {
            const int iLocal = iLocalStart + k*(lcm/c);
            const int jLocal = jLocalStart + k*(lcm/r);
            dLocalBuffer[k] = real(thisLocalBuffer[iLocal+jLocal*thisLDim]);
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::GetImagDiagonalHelper<complex<Z> >::Func
( const DistMatrix<complex<Z>,MR,MC>& parent, 
        DistMatrix<Z,MD,STAR>& d, int offset ) 
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::GetImagDiagonal([MD,* ])");
    parent.AssertNotLockedView();
#endif
    int height = parent.Height();
    int width = parent.Width();
    int length;
    if( offset > 0 )
    {
        const int remainingWidth = max(width-offset,0);
        length = min(height,remainingWidth);
    }
    else
    {
        const int remainingHeight = max(height+offset,0);
        length = min(remainingHeight,width);
    }
#ifndef RELEASE
    if( d.Viewing() && length != d.Height() )
        throw logic_error( "d is not of the correct length." );
#endif
    if( !d.Viewing() )
    {
        if( !d.ConstrainedColAlignment() )
            d.AlignWithDiag( parent, offset );
        d.ResizeTo( length, 1 );
    }

    if( d.InDiagonal() )
    {
        const elemental::Grid& g = parent.Grid();
        const int r = g.Height();
        const int c = g.Width();
        const int lcm = g.LCM();
        const int colShift = parent.ColShift();
        const int rowShift = parent.RowShift();
        const int diagShift = d.ColShift();

        int iStart,jStart;
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

        const int iLocalStart = (iStart-colShift) / c;
        const int jLocalStart = (jStart-rowShift) / r;

        const int localDiagLength = d.LocalHeight();

        Z* dLocalBuffer = d.LocalBuffer();
        const complex<Z>* thisLocalBuffer = parent.LockedLocalBuffer();
        const int thisLDim = parent.LocalLDim();
#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for( int k=0; k<localDiagLength; ++k )
        {
            const int iLocal = iLocalStart + k*(lcm/c);
            const int jLocal = jLocalStart + k*(lcm/r);
            dLocalBuffer[k] = imag(thisLocalBuffer[iLocal+jLocal*thisLDim]);
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::GetRealDiagonalHelper<complex<Z> >::Func
( const DistMatrix<complex<Z>,MR,MC>& parent, 
        DistMatrix<Z,STAR,MD>& d, int offset )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::GetRealDiagonal([* ,MD])");
    parent.AssertNotLockedView();
#endif
    int height = parent.Height();
    int width = parent.Width();
    int length;
    if( offset > 0 )
    {
        const int remainingWidth = max(width-offset,0);
        length = min(height,remainingWidth);
    }
    else
    {
        const int remainingHeight = max(height+offset,0);
        length = min(remainingHeight,width);
    }
#ifndef RELEASE
    if( d.Viewing() && length != d.Width() )
        throw logic_error( "d is not of the correct length." );
#endif

    if( !d.Viewing() )
    {
        if( !d.ConstrainedRowAlignment() )
            d.AlignWithDiag( parent, offset );
        d.ResizeTo( 1, length );
    }

    if( d.InDiagonal() )
    {
        const elemental::Grid& g = parent.Grid();
        const int r = g.Height();
        const int c = g.Width();
        const int lcm = g.LCM();
        const int colShift = parent.ColShift();
        const int rowShift = parent.RowShift();
        const int diagShift = d.RowShift();

        int iStart, jStart;
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

        const int iLocalStart = (iStart-colShift) / c;
        const int jLocalStart = (jStart-rowShift) / r;

        const int localDiagLength = d.LocalWidth();

        Z* dLocalBuffer = d.LocalBuffer();
        const int dLDim = d.LocalLDim();
        const complex<Z>* thisLocalBuffer = parent.LockedLocalBuffer();
        const int thisLDim = parent.LocalLDim();
#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for( int k=0; k<localDiagLength; ++k )
        {
            const int iLocal = iLocalStart + k*(lcm/c);
            const int jLocal = jLocalStart + k*(lcm/r);
            dLocalBuffer[k*dLDim] = 
                real(thisLocalBuffer[iLocal+jLocal*thisLDim]);
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::GetImagDiagonalHelper<complex<Z> >::Func
( const DistMatrix<complex<Z>,MR,MC>& parent, 
        DistMatrix<Z,STAR,MD>& d, int offset ) 
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::GetImagDiagonal([* ,MD])");
    parent.AssertNotLockedView();
#endif
    int height = parent.Height();
    int width = parent.Width();
    int length;
    if( offset > 0 )
    {
        const int remainingWidth = max(width-offset,0);
        length = min(height,remainingWidth);
    }
    else
    {
        const int remainingHeight = max(height+offset,0);
        length = min(remainingHeight,width);
    }
#ifndef RELEASE
    if( d.Viewing() && length != d.Width() )
        throw logic_error( "d is not of the correct length." );
#endif

    if( !d.Viewing() )
    {
        if( !d.ConstrainedRowAlignment() )
            d.AlignWithDiag( parent, offset );
        d.ResizeTo( 1, length );
    }

    if( d.InDiagonal() )
    {
        const elemental::Grid& g = parent.Grid();
        const int r = g.Height();
        const int c = g.Width();
        const int lcm = g.LCM();
        const int colShift = parent.ColShift();
        const int rowShift = parent.RowShift();
        const int diagShift = d.RowShift();

        int iStart, jStart;
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

        const int iLocalStart = (iStart-colShift) / c;
        const int jLocalStart = (jStart-rowShift) / r;

        const int localDiagLength = d.LocalWidth();

        Z* dLocalBuffer = d.LocalBuffer();
        const int dLDim = d.LocalLDim();
        const complex<Z>* thisLocalBuffer = parent.LockedLocalBuffer();
        const int thisLDim = parent.LocalLDim();
#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for( int k=0; k<localDiagLength; ++k )
        {
            const int iLocal = iLocalStart + k*(lcm/c);
            const int jLocal = jLocalStart + k*(lcm/r);
            dLocalBuffer[k*dLDim] = 
                imag(thisLocalBuffer[iLocal+jLocal*thisLDim]);
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetRealDiagonalHelper<complex<Z> >::Func
(       DistMatrix<complex<Z>,MR,MC>& parent,
  const DistMatrix<Z,MD,STAR>& d, int offset )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetRealDiagonal([MD,* ])");
    if( d.Width() != 1 )
        throw logic_error( "d must be a column vector." );
    {
        int height = parent.Height();
        int width = parent.Width();
        int length;
        if( offset >= 0 )
        {
            const int remainingWidth = max(width-offset,0);
            length = min(remainingWidth,height);
        }
        else
        {
            const int remainingHeight = max(height+offset,0);
            length = min(remainingHeight,width);
        }
        if( length != d.Height() )
        {
            ostringstream msg;
            msg << "d is not of the same length as the diagonal:" << endl
                << "  A ~ " << parent.Height() << " x " << parent.Width() << endl
                << "  d ~ " << d.Height() << " x " << d.Width() << endl
                << "  A diag length: " << length << endl;
            throw logic_error( msg.str() );
        }
    }
#endif
    if( d.InDiagonal() )
    {
        const elemental::Grid& g = parent.Grid();
        const int r = g.Height();
        const int c = g.Width();
        const int lcm = g.LCM();
        const int colShift = parent.ColShift();
        const int rowShift = parent.RowShift();
        const int diagShift = d.ColShift();

        int iStart, jStart;
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

        const int iLocalStart = (iStart-colShift) / c;
        const int jLocalStart = (jStart-rowShift) / r;

        const int localDiagLength = d.LocalHeight();

        complex<Z>* thisLocalBuffer = parent.LocalBuffer();
        const int thisLDim = parent.LocalLDim();
        const Z* dLocalBuffer = d.LockedLocalBuffer();
#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for( int k=0; k<localDiagLength; ++k )
        {
            const int iLocal = iLocalStart + k*(lcm/c);
            const int jLocal = jLocalStart + k*(lcm/r);
            const Z u = dLocalBuffer[k];
            const Z v = imag(thisLocalBuffer[iLocal+jLocal*thisLDim]);
            thisLocalBuffer[iLocal+jLocal*thisLDim] = complex<Z>(u,v);
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetImagDiagonalHelper<complex<Z> >::Func
(       DistMatrix<complex<Z>,MR,MC>& parent,
  const DistMatrix<Z,MD,STAR>& d, int offset )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetImagDiagonal([MD,* ])");
    if( d.Width() != 1 )
        throw logic_error( "d must be a column vector." );
    {
        int height = parent.Height();
        int width = parent.Width();
        int length;
        if( offset >= 0 )
        {
            const int remainingWidth = max(width-offset,0);
            length = min(remainingWidth,height);
        }
        else
        {
            const int remainingHeight = max(height+offset,0);
            length = min(remainingHeight,width);
        }
        if( length != d.Height() )
        {
            ostringstream msg;
            msg << "d is not of the same length as the diagonal:" << endl
                << "  A ~ " << parent.Height() << " x " << parent.Width() << endl
                << "  d ~ " << d.Height() << " x " << d.Width() << endl
                << "  A diag length: " << length << endl;
            throw logic_error( msg.str() );
        }
    }
#endif
    if( d.InDiagonal() )
    {
        const elemental::Grid& g = parent.Grid();
        const int r = g.Height();
        const int c = g.Width();
        const int lcm = g.LCM();
        const int colShift = parent.ColShift();
        const int rowShift = parent.RowShift();
        const int diagShift = d.ColShift();

        int iStart, jStart;
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

        const int iLocalStart = (iStart-colShift) / c;
        const int jLocalStart = (jStart-rowShift) / r;

        const int localDiagLength = d.LocalHeight();

        complex<Z>* thisLocalBuffer = parent.LocalBuffer();
        const int thisLDim = parent.LocalLDim();
        const Z* dLocalBuffer = d.LockedLocalBuffer();
#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for( int k=0; k<localDiagLength; ++k )
        {
            const int iLocal = iLocalStart + k*(lcm/c);
            const int jLocal = jLocalStart + k*(lcm/r);
            const Z u = real(thisLocalBuffer[iLocal+jLocal*thisLDim]);
            const Z v = dLocalBuffer[k];
            thisLocalBuffer[iLocal+jLocal*thisLDim] = complex<Z>(u,v);
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetRealDiagonalHelper<complex<Z> >::Func
(       DistMatrix<complex<Z>,MR,MC>& parent,
  const DistMatrix<Z,STAR,MD>& d, int offset )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetRealDiagonal([* ,MD])");
    if( d.Height() != 1 )
        throw logic_error( "d must be a row vector." );
    {
        int height = parent.Height();
        int width = parent.Width();
        int length;
        if( offset >= 0 )
        {
            const int remainingWidth = max(width-offset,0);
            length = min(remainingWidth,height);
        }
        else
        {
            const int remainingHeight = max(height+offset,0);
            length = min(remainingHeight,width);
        }
        if( length != d.Width() )
        {
            ostringstream msg;
            msg << "d is not of the same length as the diagonal:" << endl
                << "  A ~ " << parent.Height() << " x " << parent.Width() << endl
                << "  d ~ " << d.Height() << " x " << d.Width() << endl
                << "  A diag length: " << length << endl;
            throw logic_error( msg.str() );
        }
    }
#endif
    if( d.InDiagonal() )
    {
        const elemental::Grid& g = parent.Grid();
        const int r = g.Height();
        const int c = g.Width();
        const int lcm = g.LCM();
        const int colShift = parent.ColShift();
        const int rowShift = parent.RowShift();
        const int diagShift = d.RowShift();

        int iStart, jStart;
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

        const int iLocalStart = (iStart-colShift) / c;
        const int jLocalStart = (jStart-rowShift) / r;

        const int localDiagLength = d.LocalWidth();

        complex<Z>* thisLocalBuffer = parent.LocalBuffer();
        const int thisLDim = parent.LocalLDim();
        const Z* dLocalBuffer = d.LockedLocalBuffer();
        const int dLDim = d.LocalLDim();
#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for( int k=0; k<localDiagLength; ++k )
        {
            const int iLocal = iLocalStart + k*(lcm/c);
            const int jLocal = jLocalStart + k*(lcm/r);
            const Z u = dLocalBuffer[k*dLDim];
            const Z v = imag(thisLocalBuffer[iLocal+jLocal*thisLDim]);
            thisLocalBuffer[iLocal+jLocal*thisLDim] = complex<Z>(u,v);
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T>
template<typename Z>
inline void
DistMatrix<T,MR,MC>::SetImagDiagonalHelper<complex<Z> >::Func
(       DistMatrix<complex<Z>,MR,MC>& parent,
  const DistMatrix<Z,STAR,MD>& d, int offset )
{
#ifndef RELEASE
    PushCallStack("[MR,MC]::SetImagDiagonal([* ,MD])");
    if( d.Height() != 1 )
        throw logic_error( "d must be a row vector." );
    {
        int height = parent.Height();
        int width = parent.Width();
        int length;
        if( offset >= 0 )
        {
            const int remainingWidth = max(width-offset,0);
            length = min(remainingWidth,height);
        }
        else
        {
            const int remainingHeight = max(height+offset,0);
            length = min(remainingHeight,width);
        }
        if( length != d.Width() )
        {
            ostringstream msg;
            msg << "d is not of the same length as the diagonal:" << endl
                << "  A ~ " << parent.Height() << " x " << parent.Width() << endl
                << "  d ~ " << d.Height() << " x " << d.Width() << endl
                << "  A diag length: " << length << endl;
            throw logic_error( msg.str() );
        }
    }
#endif
    if( d.InDiagonal() )
    {
        const elemental::Grid& g = parent.Grid();
        const int r = g.Height();
        const int c = g.Width();
        const int lcm = g.LCM();
        const int colShift = parent.ColShift();
        const int rowShift = parent.RowShift();
        const int diagShift = d.RowShift();

        int iStart, jStart;
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

        const int iLocalStart = (iStart-colShift) / c;
        const int jLocalStart = (jStart-rowShift) / r;

        const int localDiagLength = d.LocalWidth();

        complex<Z>* thisLocalBuffer = parent.LocalBuffer();
        const int thisLDim = parent.LocalLDim();
        const Z* dLocalBuffer = d.LockedLocalBuffer();
        const int dLDim = d.LocalLDim();
#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for( int k=0; k<localDiagLength; ++k )
        {
            const int iLocal = iLocalStart + k*(lcm/c);
            const int jLocal = jLocalStart + k*(lcm/r);
            const Z u = real(thisLocalBuffer[iLocal+jLocal*thisLDim]);
            const Z v = dLocalBuffer[k*dLDim];
            thisLocalBuffer[iLocal+jLocal*thisLDim] = complex<Z>(u,v);
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}
#endif // WITHOUT_COMPLEX

} // namespace elemental