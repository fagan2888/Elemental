/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef BLAS_ZERO_HPP
#define BLAS_ZERO_HPP

namespace elem {

template <typename Int>
void Zero( AutoMatrix<Int>& A )
{
#ifndef RELEASE
	PushCallStack("Zero");
#endif
	const size_t dsize = A.DataSize();
	const size_t height = A.Height() * dsize;
	const size_t ldim = A.LDim() * dsize;
	const int width = A.Width();
	byte* buffer = static_cast<byte*>( A.Buffer() );
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( int j=0; j<width; ++j )
    	MemZero( buffer + j * ldim, height );
#ifndef RELEASE    	
    PopCallStack();
#endif
}

//
// The specialized form is no longer necessary.
//

#if 0
template<typename T>
inline void
Zero( Matrix<T>& A )
{
#ifndef RELEASE
    PushCallStack("Zero");
#endif
    const int height = A.Height();
    const int width = A.Width();
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( int j=0; j<width; ++j )
        MemZero( A.Buffer(0,j), height );
#ifndef RELEASE
    PopCallStack();
#endif
}
#endif

template<typename T,Distribution U,Distribution V>
inline void
Zero( DistMatrix<T,U,V>& A )
{
#ifndef RELEASE
    PushCallStack("Zero");
#endif
    Zero( A.Matrix() );
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace elem

#endif // ifndef BLAS_ZERO_HPP
