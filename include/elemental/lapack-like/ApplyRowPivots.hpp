/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef LAPACK_APPLYROWPIVOTS_HPP
#define LAPACK_APPLYROWPIVOTS_HPP

#include "elemental/lapack-like/ComposePivots.hpp"

namespace elem {

template<typename F>
inline void
ApplyRowPivots( Matrix<F>& A, const Matrix<int>& p )
{
#ifndef RELEASE
    PushCallStack("ApplyRowPivots");
    if( p.Width() != 1 )
        throw std::logic_error("p must be a column vector");
    if( p.Height() != A.Height() )
        throw std::logic_error("p must be the same length as the height of A");
#endif
    const int height = A.Height();
    const int width = A.Width();
    if( height == 0 || width == 0 )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const int ldim = A.LDim();
    for( int i=0; i<height; ++i )
    {
        const int k = p.Get(i,0);
        F* Ai = A.Buffer(i,0);
        F* Ak = A.Buffer(k,0);
        for( int j=0; j<width; ++j )
        {
            F temp = Ai[j*ldim];
            Ai[j*ldim] = Ak[j*ldim];
            Ak[j*ldim] = temp;
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
ApplyInverseRowPivots( Matrix<F>& A, const Matrix<int>& p )
{
#ifndef RELEASE
    PushCallStack("ApplyInverseRowPivots");
    if( p.Width() != 1 )
        throw std::logic_error("p must be a column vector");
    if( p.Height() != A.Height() )
        throw std::logic_error("p must be the same length as the height of A");
#endif
    const int height = A.Height();
    const int width = A.Width();
    if( height == 0 || width == 0 )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    const int ldim = A.LDim();
    for( int i=height-1; i>=0; --i )
    {
        const int k = p.Get(i,0);
        F* Ai = A.Buffer(i,0);
        F* Ak = A.Buffer(k,0);
        for( int j=0; j<width; ++j )
        {
            F temp = Ai[j*ldim];
            Ai[j*ldim] = Ak[j*ldim];
            Ak[j*ldim] = temp;
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
ApplyRowPivots( DistMatrix<F>& A, const DistMatrix<int,VC,STAR>& p )
{
#ifndef RELEASE
    PushCallStack("ApplyRowPivots");
#endif
    DistMatrix<int,STAR,STAR> p_STAR_STAR( p );
    ApplyRowPivots( A, p_STAR_STAR );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
ApplyInverseRowPivots
( DistMatrix<F>& A, const DistMatrix<int,VC,STAR>& p )
{
#ifndef RELEASE
    PushCallStack("ApplyInverseRowPivots");
#endif
    DistMatrix<int,STAR,STAR> p_STAR_STAR( p );
    ApplyInverseRowPivots( A, p_STAR_STAR );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
ApplyRowPivots( DistMatrix<F>& A, const DistMatrix<int,STAR,STAR>& p )
{
#ifndef RELEASE
    PushCallStack("ApplyRowPivots");
#endif
    std::vector<int> image, preimage;
    ComposePivots( p, image, preimage );
    ApplyRowPivots( A, image, preimage );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F>
inline void
ApplyInverseRowPivots
( DistMatrix<F>& A, const DistMatrix<int,STAR,STAR>& p )
{
#ifndef RELEASE
    PushCallStack("ApplyInverseRowPivots");
#endif
    std::vector<int> image, preimage;
    ComposePivots( p, image, preimage );
    ApplyRowPivots( A, preimage, image );
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F> 
inline void
ApplyRowPivots
( Matrix<F>& A, 
  const std::vector<int>& image,
  const std::vector<int>& preimage )
{
    const int b = image.size();
#ifndef RELEASE
    PushCallStack("ApplyRowPivots");
    if( A.Height() < b || b != (int)preimage.size() )
        throw std::logic_error
        ("image and preimage must be vectors of equal length that are not "
         "taller than A.");
#endif
    const int m = A.Height();
    const int n = A.Width();
    if( m == 0 || n == 0 )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }

    // TODO: Optimize this routine

    // Make a copy of the first b rows
    Matrix<F> ARowPanView;
    LockedView( ARowPanView, A, 0, 0, b, n );
    Matrix<F> ARowPanCopy( ARowPanView );

    // Make a copy of the preimage rows
    Matrix<F> APreimageCopy( b, n );
    for( int i=0; i<b; ++i ) 
    {
        const int iPre = preimage[i];
        if( iPre >= b )
            for( int j=0; j<n; ++j )
                APreimageCopy.Set(i,j,A.Get(iPre,j));
    }

    // Apply the permutations
    for( int i=0; i<b; ++i )
    {
        const int iPre = preimage[i];
        const int iPost = image[i];
        // Move row[i] into row[image[i]]
        for( int j=0; j<n; ++j )
            A.Set(iPost,j,ARowPanCopy.Get(i,j));
        if( iPre >= b )
        {
            // Move row[preimage[i]] into row[i]
            for( int j=0; j<n; ++j )
                A.Set(i,j,APreimageCopy.Get(i,j));
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename F> 
inline void
ApplyRowPivots
( DistMatrix<F>& A, 
  const std::vector<int>& image,
  const std::vector<int>& preimage )
{
    const int b = image.size();
#ifndef RELEASE
    PushCallStack("ApplyRowPivots");
    if( A.Height() < b || b != (int)preimage.size() )
        throw std::logic_error
        ("image and preimage must be vectors of equal length that are not "
         "taller than A.");
#endif
    const int localWidth = A.LocalWidth();
    if( A.Height() == 0 || A.Width() == 0 )
    {
#ifndef RELEASE
        PopCallStack();
#endif
        return;
    }
    
    // Extract the relevant process grid information
    const Grid& g = A.Grid();
    const int r = g.Height();
    const int colAlignment = A.ColAlignment();
    const int colShift = A.ColShift();
    const int myRow = g.Row();

    // Extract the send and recv counts from the image and preimage.
    // This process's sends may be logically partitioned into two sets:
    //   (a) sends from rows [0,...,b-1]
    //   (b) sends from rows [b,...]
    // The latter is analyzed with preimage, the former deduced with image.
    std::vector<int> sendCounts(r,0), recvCounts(r,0);
    for( int i=colShift; i<b; i+=r )
    {
        const int sendRow = image[i];         
        const int sendTo = (colAlignment+sendRow) % r; 
        sendCounts[sendTo] += localWidth;

        const int recvRow = preimage[i];
        const int recvFrom = (colAlignment+recvRow) % r;
        recvCounts[recvFrom] += localWidth;
    }
    for( int i=0; i<b; ++i )
    {
        const int sendRow = image[i];
        if( sendRow >= b )
        {
            const int sendTo = (colAlignment+sendRow) % r;
            if( sendTo == myRow )
            {
                const int sendFrom = (colAlignment+i) % r;
                recvCounts[sendFrom] += localWidth;
            }
        }

        const int recvRow = preimage[i];
        if( recvRow >= b )
        {
            const int recvFrom = (colAlignment+recvRow) % r;
            if( recvFrom == myRow )
            {
                const int recvTo = (colAlignment+i) % r;
                sendCounts[recvTo] += localWidth;
            }
        }
    }

    // Construct the send and recv displacements from the counts
    std::vector<int> sendDispls(r), recvDispls(r);
    int totalSend=0, totalRecv=0;
    for( int i=0; i<r; ++i )
    {
        sendDispls[i] = totalSend;
        recvDispls[i] = totalRecv;
        totalSend += sendCounts[i];
        totalRecv += recvCounts[i];
    }
#ifndef RELEASE
    if( totalSend != totalRecv )
    {
        std::ostringstream msg;
        msg << "Send and recv counts do not match: (send,recv)=" 
             << totalSend << "," << totalRecv;
        throw std::logic_error( msg.str().c_str() );
    }
#endif

    // Fill vectors with the send data
    const int ALDim = A.LDim();
    std::vector<F> sendData(std::max(1,totalSend));
    std::vector<int> offsets(r,0);
    const int localHeight = Length( b, colShift, r );
    for( int iLocal=0; iLocal<localHeight; ++iLocal )
    {
        const int sendRow = image[colShift+iLocal*r];
        const int sendTo = (colAlignment+sendRow) % r;
        const int offset = sendDispls[sendTo]+offsets[sendTo];
        const F* ABuffer = A.Buffer(iLocal,0);
        for( int jLocal=0; jLocal<localWidth; ++jLocal )
            sendData[offset+jLocal] = ABuffer[jLocal*ALDim];
        offsets[sendTo] += localWidth;
    }
    for( int i=0; i<b; ++i )
    {
        const int recvRow = preimage[i];
        if( recvRow >= b )
        {
            const int recvFrom = (colAlignment+recvRow) % r; 
            if( recvFrom == myRow )
            {
                const int recvTo = (colAlignment+i) % r;
                const int iLocal = (recvRow-colShift) / r;
                const int offset = sendDispls[recvTo]+offsets[recvTo];
                const F* ABuffer = A.Buffer(iLocal,0);
                for( int jLocal=0; jLocal<localWidth; ++jLocal )
                    sendData[offset+jLocal] = ABuffer[jLocal*ALDim];
                offsets[recvTo] += localWidth;
            }
        }
    }

    // Communicate all pivot rows
    std::vector<F> recvData(std::max(1,totalRecv));
    mpi::AllToAll
    ( &sendData[0], &sendCounts[0], &sendDispls[0],
      &recvData[0], &recvCounts[0], &recvDispls[0], g.ColComm() );

    // Unpack the recv data
    for( int k=0; k<r; ++k )
    {
        offsets[k] = 0;
        int thisColShift = Shift( k, colAlignment, r );
        for( int i=thisColShift; i<b; i+=r )
        {
            const int sendRow = image[i];
            const int sendTo = (colAlignment+sendRow) % r;
            if( sendTo == myRow )
            {
                const int offset = recvDispls[k]+offsets[k];
                const int iLocal = (sendRow-colShift) / r;
                F* ABuffer = A.Buffer(iLocal,0);
                for( int jLocal=0; jLocal<localWidth; ++jLocal )
                    ABuffer[jLocal*ALDim] = recvData[offset+jLocal];
                offsets[k] += localWidth;
            }
        }
    }
    for( int i=0; i<b; ++i )
    {
        const int recvRow = preimage[i];
        if( recvRow >= b )
        {
            const int recvTo = (colAlignment+i) % r;
            if( recvTo == myRow )
            {
                const int recvFrom = (colAlignment+recvRow) % r; 
                const int iLocal = (i-colShift) / r;
                const int offset = recvDispls[recvFrom]+offsets[recvFrom];
                F* ABuffer = A.Buffer(iLocal,0);
                for( int jLocal=0; jLocal<localWidth; ++jLocal )
                    ABuffer[jLocal*ALDim] = recvData[offset+jLocal];
                offsets[recvFrom] += localWidth;
            }
        }
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace elem

#endif // ifndef LAPACK_APPLYROWPIVOTS_HPP
