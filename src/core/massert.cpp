/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#include "elemental-lite.hpp"

namespace elem {

#define AM AutoMatrix<Int>
#define ADM AutoDistMatrix<Int>

void AssertUnlocked( bool locked )
{
	if ( locked )
		throw std::logic_error( "Expected an unlocked matrix here" );
}

template <class Int>
void AssertNonnegative( Int i, const char* s )
{
	if ( i < 0 ) {
        std::ostringstream msg;
        msg << s << " must be non-negative";
        throw std::logic_error( msg.str() );
    }
}

template <typename Int>
void AssertIndices( Int i, Int j )
{
    if( i < 0 || j < 0 )
        throw std::logic_error("Indices must be non-negative");
}

template <typename Int>
void AssertValidEntry( Int i, Int j, Int height, Int width )
{
    if( i < 0 || j < 0 )
        throw std::logic_error("Indices must be non-negative");
    if( i > height || j > width )
    {
        std::ostringstream msg;
        msg << "Out of bounds: "
            << "(" << i << "," << j << ") of " << height
            << " x " << width << " Matrix.";
        throw std::logic_error( msg.str() );
    }
}

template <typename Int>
void AssertLDim( Int height, Int ldim )
{
	if ( ldim == 0 )
		throw std::logic_error( "Leading dimension cannot be zero (for BLAS compatibility)" );
	if ( ldim < height ) {
		std::ostringstream msg;
		msg << "Initialized with ldim(" << ldim << ") < "
			<< "height(" << height << ").";
		throw std::logic_error( msg.str() );
	}
}

template <typename Int>
void AssertDimensions( Int height, Int width, Int ldim, bool zero_ok )
{
	AssertNonnegative( height, "height" );
	AssertNonnegative( width, "width" );
	if ( ldim != 0 || !zero_ok )
		AssertLDim( height, ldim );
}

template <typename Int>
void AssertValidSubmatrix( Int i, Int j, Int height, Int width, Int height_, Int width_ )
{
	AssertValidEntry( i, j, height_, width_ );
    if( (i+height) > height_ || (j+width) > width_ )
    {
        std::ostringstream msg;
        msg << "Trying to view outside of a Matrix: "
            << "(" << i << "," << j << ") up to (" 
            << i+height-1 << "," << j+width-1 << ") "
            << "of " << height_ << " x " << width_ << " Matrix.";
        throw std::logic_error( msg.str() );
    }
}

template <typename Int>
void AssertSameSize( Int h1, Int w1, Int h2, Int w2 )
{
	if ( h1 != h2 || w1 != h2 )
	{
		std::ostringstream msg;
		msg << "Cannot assign a matrix of size (" << h2 << "," << w2 << ")"
		    << " to a view of size (" << h1 << "," << w1 << ")";
		throw std::logic_error( msg.str() );
	}
}

static const char *TypeNames[] = {
	"Integral",
#ifndef DISABLE_FLOAT
	"Single",
#endif
	"Double",
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
	"SComplex",
#endif
	"DComplex",
#endif
	"UNKNOWN"
};

static const ScalarTypes RealTypes[] = {
	INTEGRAL,
#ifndef DISABLE_FLOAT
	SINGLE,
#endif
	DOUBLE,
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
	SINGLE,
#endif
	DOUBLE,
#endif
	UNKNOWN
};

static bool ComplexFlag[] = {
	false,
#ifndef DISABLE_FLOAT
	false,
#endif
	false,
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
	true,
#endif
	true,
#endif
	false
};

void AssertDataTypes( ScalarTypes A, ScalarTypes B )
{
	if ( A != B ) 
	{
		std::ostringstream msg;
		msg << "Matrix type mismach: " << TypeNames[A] << " != " << TypeNames[B];
		throw std::logic_error( msg.str() );
	}
}

void AssertDistributions( Distribution AR, Distribution AC, Distribution BR, Distribution BC )
{
	if ( AR != BR || AC != BC )
	{
		std::ostringstream msg;
		msg << "Distribution mismatch: (" 
			<< DistToString( AR ) << "," << DistToString( AC )<< ") != ("
			<< DistToString( BR ) << "," << DistToString( BC ) << ")";
		throw std::logic_error( msg.str() );
	}
}			

void AssertCRDataTypes( ScalarTypes A, ScalarTypes B, bool imag_bad )
{
	if ( RealTypes[A] != B ) {
		std::ostringstream msg;
		msg << "Data type mismatch: Real(" << TypeNames[A] << ") != " << TypeNames[B];
		throw std::logic_error( msg.str() );
	}
	if ( imag_bad && !ComplexFlag[A] )
		throw std::logic_error( "Cannot use this method with real data" );
}

template <typename Int>
void AssertDataTypes( Int N, const AM* A, ... )
{
	va_list arguments;
	va_start( arguments, A );
	ScalarTypes AType = A->DataType();
	for ( Int k = 1 ; k < N ; ++k )
		elem::AssertDataTypes( AType, va_arg( arguments, const AM* )->DataType() );
	va_end( arguments );
}

template <typename Int>
void AssertDataTypes( Int N, const ADM* A, ... )
{
	va_list arguments;
	va_start( arguments, A );
	ScalarTypes AType = A->DataType();
	Distribution ARow = A->RowDist(), ACol = A->ColDist();
	for ( Int k = 1 ; k < N ; ++k ) {
		const ADM* B = va_arg( arguments, const ADM* );
		elem::AssertDataTypes( AType, B->DataType() );
		elem::AssertDistributions( ARow, ACol, B->RowDist(), B->ColDist() );
	}
	va_end( arguments );
}

template <typename Int>
void AssertUnlocked( Int N, const AM* A, ... )
{
	va_list arguments;
	va_start( arguments, A );
	AssertUnlocked( *A );
	for ( Int k = 1 ; k < N ; ++k )
		AssertUnlocked( *va_arg( arguments, const AM* ) );
	va_end( arguments );
}

template <typename Int>
void AssertUnlocked( Int N, const ADM* A, ... )
{
	va_list arguments;
	va_start( arguments, A );
	AssertUnlocked( *A );
	for ( Int k = 1 ; k < N ; ++k )
		AssertUnlocked( *va_arg( arguments, const ADM* ) );
	va_end( arguments );
}

template <typename Int>
void AssertContiguous1x2( const AM& A, const AM& B )
{
    if( A.Height() != B.Height() || A.LDim() != B.LDim() )
        throw std::logic_error("1x2 must have consistent heights");
    if( A.LDim() != B.LDim() )
        throw std::logic_error("1x2 must have consistent ldims");
    if( A.LockedBuffer_(0,A.Width()) != B.LockedBuffer_() )
        throw std::logic_error("1x2 must have contiguous memory");
}

template<typename Int> 
void
AssertContiguous1x2
( const ADM& A00, 
  const ADM& A01 )
{
    if( A00.Height() != A01.Height() )
    {
        std::ostringstream msg;
        msg << "1x2 not conformant: " << std::endl << "    "
        	<< "(" << A00.Height() << " x " << A00.Width() << ") "
        	<< "(" << A01.Height() << " x " << A01.Width() << ") ";
        throw std::logic_error( msg.str().c_str() );
    }
    if( A00.ColAlignment() != A01.ColAlignment() )
        throw std::logic_error("1x2 is misaligned");
}

template <typename Int>
void AssertContiguous1x3( const AM& A, const AM& B, const AM& C )
{
    if( A.Height() != B.Height() || A.Height() != C.Height() )
        throw std::logic_error("1x3 must have consistent heights");
    if( A.LDim() != B.LDim() || A.LDim() != C.LDim() )
        throw std::logic_error("1x3 must have consistent ldims");
    if( A.LockedBuffer_(0,A.Width()) != B.LockedBuffer_() ||
        B.LockedBuffer_(0,B.Width()) != C.LockedBuffer_() )
        throw std::logic_error("1x3 must have contiguous memory");
}

template<typename Int> 
void
AssertContiguous1x3
( const ADM& A00, 
  const ADM& A01,
  const ADM& A02 )
{
    if( A00.Height() != A01.Height() || A00.Height() != A02.Height() )    
    {
        std::ostringstream msg;
        msg << "1x3 not conformant: " << std::endl << "    "
        	<< "(" << A00.Height() << " x " << A00.Width() << ") "
        	<< "(" << A01.Height() << " x " << A01.Width() << ") "
        	<< "(" << A02.Height() << " x " << A02.Width() << ") ";
        throw std::logic_error( msg.str().c_str() );
    }
    if( A00.ColAlignment() != A01.ColAlignment() ||
        A00.ColAlignment() != A02.ColAlignment() )
        throw std::logic_error("1x3 is misaligned");
}

template <typename Int>
void AssertContiguous2x1( const AM& A, const AM& B )
{
    if( A.Width() != B.Width() )
        throw std::logic_error("2x1 must have consistent widths");
    if( A.LDim() != B.LDim() )
        throw std::logic_error("2x1 must have consistent ldims");
    if( A.LockedBuffer_(A.Height(),0) != B.LockedBuffer() )
        throw std::logic_error("2x1 must have contiguous memory");
}

template<typename Int> 
void
AssertContiguous2x1
( const ADM& A00,
  const ADM& A10 )
{
    if( A00.Width() != A10.Width() )
    {
        std::ostringstream msg;        
        msg << "2x1 is not conformant: " << std::endl << "    "
        	<< "(" << A00.Height() << " x " << A00.Width() << ") " << std::endl << "    "
        	<< "(" << A10.Height() << " x " << A10.Width() << ") ";
        throw std::logic_error( msg.str().c_str() );
    }
    if( A00.RowAlignment() != A10.RowAlignment() )
        throw std::logic_error("2x1 is not aligned");
}

template <typename Int>
void AssertContiguous3x1( const AM& A, const AM& B, const AM& C )
{
    if( A.Width() != B.Width() || A.Width() != C.Width() )
        throw std::logic_error("3x1 must have consistent widths");
    if( A.LDim() != B.LDim() || A.LDim() != C.LDim() )
        throw std::logic_error("3x1 must have consistent ldims");
    if( A.LockedBuffer_(A.Height(),0) != B.LockedBuffer() ||
        B.LockedBuffer_(B.Height(),0) != C.LockedBuffer() )
        throw std::logic_error("3x1 must have contiguous memory");
}

template<typename Int> 
void
AssertContiguous3x1
( const ADM& A00,
  const ADM& A10,
  const ADM& A20 )
{
    if( A00.Width() != A10.Width() || A00.Width() != A20.Width() )
    {
        std::ostringstream msg;        
        msg << "3x1 not conformant: " << std::endl << "   "
        	<< "(" << A00.Height() << " x " << A00.Width() << ") " << std::endl << "    "
        	<< "(" << A10.Height() << " x " << A10.Width() << ") " << std::endl << "    "
        	<< "(" << A20.Height() << " x " << A20.Width() << ") ";
        throw std::logic_error( msg.str().c_str() );
    }
    if( A00.RowAlignment() != A10.RowAlignment() ||
        A00.RowAlignment() != A20.ColAlignment() )
        throw std::logic_error("3x1 is not aligned");
}

template <typename Int>
void AssertContiguous2x2
( const AM& BTL, const AM& BTR, 
  const AM& BBL, const AM& BBR )
{
    if( BTL.Width()  != BBL.Width()   ||
        BTR.Width()  != BBR.Width()   ||
        BTL.Height() != BTR.Height() ||
        BBL.Height() != BBR.Height()   )
        throw std::logic_error("2x2 must have consistent dimensions");
    if( BTL.LDim() != BTR.LDim() ||
        BTR.LDim() != BBL.LDim() ||
        BBL.LDim() != BBR.LDim()   )
        throw std::logic_error("2x2 must have consistent ldims");
    if( BBL.LockedBuffer_() != BTL.LockedBuffer_(BTL.Height(),0) ||
        BTR.LockedBuffer_() != BTL.LockedBuffer_(0,BTL.Width())  ||
        BBR.LockedBuffer_() != BTR.LockedBuffer_(BTR.Height(),0) )
        throw std::logic_error("2x2 must have contiguous memory");
}

template<typename Int> 
void
AssertContiguous2x2
( const ADM& A00, 
  const ADM& A01,
  const ADM& A10, 
  const ADM& A11 ) 
{
    if( A00.Width()  != A10.Width()  || A10.Width()  != A11.Width() ||
        A00.Height() != A01.Height() || A01.Height() != A11.Height() )
    {
        std::ostringstream msg;
        msg << "2x2 is not conformant: " << std::endl << "    "
        	<< "(" << A00.Height() << " x " << A00.Width() << ") "
        	<< "(" << A01.Height() << " x " << A01.Width() << ") " << std::endl << "    "
        	<< "(" << A10.Height() << " x " << A10.Width() << ") "
        	<< "(" << A11.Height() << " x " << A11.Width() << ") ";
        throw std::logic_error( msg.str().c_str() );
    }
    if( A00.ColAlignment() != A01.ColAlignment() ||
        A10.ColAlignment() != A11.ColAlignment() ||
        A00.RowAlignment() != A10.RowAlignment() ||
        A01.RowAlignment() != A11.RowAlignment() )
        throw std::logic_error
        ("2x2 set of matrices must aligned to combine");
}

template <typename Int>
void AssertContiguous3x3(
		const AM& A00, const AM& A01, const AM& A02,
		const AM& A10, const AM& A11, const AM& A12,
		const AM& A20, const AM& A21, const AM& A22 )
{
	if ( A00.Width()  != A10.Width()  || A00.Width()  != A20.Width()  ||
	     A01.Width()  != A11.Width()  || A01.Width()  != A21.Width()  ||
	     A02.Width()  != A12.Width()  || A02.Width()  != A22.Width()  ||
	     A00.Height() != A01.Height() || A00.Height() != A02.Height() ||
	     A10.Height() != A11.Height() || A10.Height() != A12.Height() ||
	     A20.Height() != A21.Height() || A20.Height() != A22.Height() )
        throw std::logic_error("3x3 must have consistent dimensions");
	if ( A00.LDim() != A10.LDim() || A00.LDim() != A20.LDim() ||
	     A01.LDim() != A11.LDim() || A01.LDim() != A21.LDim() ||
	     A02.LDim() != A12.LDim() || A02.LDim() != A22.LDim() )
        throw std::logic_error("3x3 must have consistent ldims");
    if(  A01.LockedBuffer_() != A00.LockedBuffer_(0,A00.Width())  ||
    	 A02.LockedBuffer_() != A01.LockedBuffer_(0,A01.Width())  ||
    	 A10.LockedBuffer_() != A00.LockedBuffer_(A00.Height(),0) ||
    	 A11.LockedBuffer_() != A01.LockedBuffer_(A01.Height(),0) ||
    	 A12.LockedBuffer_() != A02.LockedBuffer_(A02.Height(),0) ||
    	 A20.LockedBuffer_() != A10.LockedBuffer_(A10.Height(),0) ||
    	 A21.LockedBuffer_() != A11.LockedBuffer_(A11.Height(),0) ||
    	 A22.LockedBuffer_() != A12.LockedBuffer_(A12.Height(),0) )
        throw std::logic_error("3x3 must have contiguous memory");
}

template<typename Int> 
void
AssertContiguous3x3
( const ADM& A00, const ADM& A01, const ADM& A02,
  const ADM& A10, const ADM& A11, const ADM& A12,
  const ADM& A20, const ADM& A21, const ADM& A22 )
{
	if ( A00.Width() != A10.Width()  || A00.Width() != A20.Width() ||
	     A01.Width() != A11.Width()  || A01.Width() != A21.Width() ||
	     A02.Width() != A12.Width()  || A02.Width() != A22.Width() ||
	     A00.Height() != A01.Height() || A00.Height() != A02.Height() ||
	     A10.Height() != A11.Height() || A10.Height() != A12.Height() ||
	     A20.Height() != A21.Height() || A20.Height() != A22.Height() )
    {
        std::ostringstream msg;
        msg << "3x3 is not conformant: " << std::endl << "    "
        	<< "(" << A00.Height() << " x " << A00.Width() << ") "
        	<< "(" << A01.Height() << " x " << A01.Width() << ") "
        	<< "(" << A02.Height() << " x " << A02.Width() << ") " << std::endl << "    "
        	<< "(" << A10.Height() << " x " << A10.Width() << ") "
        	<< "(" << A11.Height() << " x " << A11.Width() << ") "
        	<< "(" << A12.Height() << " x " << A12.Width() << ") " << std::endl << "    "
        	<< "(" << A20.Height() << " x " << A20.Width() << ") "
        	<< "(" << A21.Height() << " x " << A21.Width() << ") "
        	<< "(" << A22.Height() << " x " << A22.Width() << ") ";
        throw std::logic_error( msg.str().c_str() );
    }
    if( A00.ColAlignment() != A01.ColAlignment() || A00.ColAlignment() != A02.ColAlignment() ||
        A10.ColAlignment() != A11.ColAlignment() || A10.ColAlignment() != A12.ColAlignment() ||
        A20.ColAlignment() != A21.ColAlignment() || A20.ColAlignment() != A22.ColAlignment() ||
        A00.RowAlignment() != A10.RowAlignment() || A00.RowAlignment() != A20.RowAlignment() ||
        A01.RowAlignment() != A11.RowAlignment() || A01.RowAlignment() != A21.RowAlignment() ||
        A02.RowAlignment() != A12.RowAlignment() || A02.RowAlignment() != A22.RowAlignment() )
        throw std::logic_error
        ("3x3 set of matrices must aligned to combine");
}

//
// Explicit instantiations
//

#define Int int

//
// This has to be explicitly instantiated.
//

void NotImplemented(
	bool is_method,
	const char* func_name,
	...
) {
	std::ostringstream msg;
	msg << ( is_method ? "Method " : "Function " ) << func_name << " not implemented for this type combination:" << std::endl;
	va_list arguments;
	va_start( arguments, func_name );
	NotImplementedTypes type = static_cast<NotImplementedTypes>( va_arg( arguments, int ) );
	while ( type != NIDone ) {
		const char* aname = va_arg( arguments, const char* );
		msg << "    " << aname << ": ";
		switch ( type ) {
		case NIBool:
			msg << " Boolean = " << ( *va_arg(arguments,const bool*) ? "True" : "False" );
			break;
		case NIInt:
			msg << " Int = " << *va_arg(arguments,const Int*);
		case NIScalar:
			va_arg(arguments,const Scalar<int>*)->Print( msg );
			break;
		case NIMatrix:
			va_arg(arguments,const AutoMatrix<int>*)->PrintShort( msg );
			break;
		case NIDistData:
			va_arg(arguments,const DistData<int>*)->PrintShort( msg );
			break;
		case NIDistMatrix:
			va_arg(arguments,const AutoDistMatrix<int>*)->PrintShort( msg );
			break;
		default: 
			msg << "(unknown)";
			type = NIDone;
			break;
		}
		msg << std::endl;
	}
	throw std::logic_error( msg.str() );
}

template void AssertIndices<Int>( Int, Int );
template void AssertValidEntry<Int>( Int, Int, Int, Int );
template void AssertSameSize<Int>( Int, Int, Int, Int );
template void AssertLDim<Int>( Int, Int );
template void AssertValidSubmatrix<Int>( Int, Int, Int, Int, Int, Int );
template void AssertDimensions<Int>( Int, Int, Int, bool );
template void AssertDataTypes<Int>( const AM&, const AM& ); 
template void AssertCRDataTypes<Int>( const AM&, const AM& ); 
template void AssertDataTypes<Int>( Int, const AM*, ... ); 
template void AssertUnlocked<Int>( const AM& );
template void AssertUnlocked( Int, const AM*, ... ); 
template void AssertContiguous1x2<Int>( const AM&, const AM& );
template void AssertContiguous2x1<Int>( const AM&, const AM& );
template void AssertContiguous1x3<Int>( const AM&, const AM&, const AM& );
template void AssertContiguous3x1<Int>( const AM&, const AM&, const AM& );
template void AssertContiguous2x2<Int>( const AM&, const AM&, const AM&, const AM& );
template void AssertContiguous3x3<Int>( const AM&, const AM&, const AM&, const AM&, const AM&, const AM&, const AM&, const AM&, const AM& );
template void AssertDataTypes<Int>( const ADM&, const ADM& ); 
template void AssertCRDataTypes<Int>( const ADM&, const ADM& ); 
template void AssertDataTypes<Int>( Int, const ADM*, ... ); 
template void AssertUnlocked<Int>( const ADM& );
template void AssertUnlocked( Int, const ADM*, ... ); 
template void AssertContiguous1x2<Int>( const ADM&, const ADM& );
template void AssertContiguous2x1<Int>( const ADM&, const ADM& );
template void AssertContiguous1x3<Int>( const ADM&, const ADM&, const ADM& );
template void AssertContiguous3x1<Int>( const ADM&, const ADM&, const ADM& );
template void AssertContiguous2x2<Int>( const ADM&, const ADM&, const ADM&, const ADM& );
template void AssertContiguous3x3<Int>( const ADM&, const ADM&, const ADM&, const ADM&, const ADM&, const ADM&, const ADM&, const ADM&, const ADM& );

} // namespace elem
