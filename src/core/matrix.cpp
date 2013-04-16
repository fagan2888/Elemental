/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#include "elemental-lite.hpp"

namespace elem {

//
// Consistency checkers
//

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
static void AssertIndices( Int i, Int j )
{
    if( i < 0 || j < 0 )
        throw std::logic_error("Indices must be non-negative");
}

template <typename Int>
static void AssertBounds( Int i, Int j, Int height, Int width )
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
void AutoMatrix<Int>::AssertBounds( Int i, Int j ) const
{ elem::AssertBounds( i, j, height_, width_ ); }

template <typename Int>
static void AssertLDim( Int height, Int ldim )
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
static void AssertDimensions( Int height, Int width, Int ldim, bool zero_ok = false )
{
	AssertNonnegative( height, "height" );
	AssertNonnegative( width, "width" );
	if ( ldim != 0 || !zero_ok )
		AssertLDim( height, ldim );
}

template <typename Int>
static void AssertView( Int i, Int j, Int height, Int width, Int height_, Int width_ )
{
	AssertBounds( i, j, height_, width_ );
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
void AutoMatrix<Int>::AssertView( Int i, Int j, Int height, Int width ) const
{ 
	elem::AssertView( i, j, height, width, height_, width_ ); 
}

template <typename Int>
void AssertContiguous1x2( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B )
{
    if( A.Height() != B.Height() )
        throw std::logic_error("1x2 must have consistent heights");
    if( A.LDim() != B.LDim() )
        throw std::logic_error("1x2 must have consistent ldims");
    if( A.LockedBuffer_(0,A.Width()) != B.LockedBuffer_() )
        throw std::logic_error("1x2 must have contiguous memory");
}

template <typename Int>
void AssertContiguous1x3( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B, const AutoMatrix<Int>& C )
{
    if( A.Height() != B.Height() || A.Height() != C.Height() )
        throw std::logic_error("1x3 must have consistent heights");
    if( A.LDim() != B.LDim() || A.LDim() != C.LDim() )
        throw std::logic_error("1x3 must have consistent ldims");
    if( A.LockedBuffer_(0,A.Width()) != B.LockedBuffer_() ||
        B.LockedBuffer_(0,B.Width()) != C.LockedBuffer_() )
        throw std::logic_error("1x3 must have contiguous memory");
}

template <typename Int>
void AssertContiguous2x1( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B )
{
    if( A.Width() != B.Width() )
        throw std::logic_error("2x1 must have consistent widths");
    if( A.LDim() != B.LDim() )
        throw std::logic_error("2x1 must have consistent ldims");
    if( A.LockedBuffer_(A.Height(),0) != B.LockedBuffer() )
        throw std::logic_error("2x1 must have contiguous memory");
}

template <typename Int>
void AssertContiguous3x1( const AutoMatrix<Int>& A, const AutoMatrix<Int>& B, const AutoMatrix<Int>& C )
{
    if( A.Width() != B.Width() || A.Width() != C.Width() )
        throw std::logic_error("3x1 must have consistent widths");
    if( A.LDim() != B.LDim() || A.LDim() != C.LDim() )
        throw std::logic_error("3x1 must have consistent ldims");
    if( A.LockedBuffer_(A.Height(),0) != B.LockedBuffer() ||
        B.LockedBuffer_(B.Height(),0) != C.LockedBuffer() )
        throw std::logic_error("3x1 must have contiguous memory");
}

template <typename Int>
void AssertContiguous2x2
( const AutoMatrix<Int>& BTL, const AutoMatrix<Int>& BTR, 
  const AutoMatrix<Int>& BBL, const AutoMatrix<Int>& BBR )
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

template <typename Int>
void AssertContiguous3x3(
		const AutoMatrix<Int>& A00, const AutoMatrix<Int>& A01, const AutoMatrix<Int>& A02,
		const AutoMatrix<Int>& A10, const AutoMatrix<Int>& A11, const AutoMatrix<Int>& A12,
		const AutoMatrix<Int>& A20, const AutoMatrix<Int>& A21, const AutoMatrix<Int>& A22 )
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

template <typename Int>
bool
AutoMatrix<Int>::IsComplex() const
{
	return ComplexFlag[DataType()];
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

template <typename Int>
void AutoMatrix<Int>::AssertCastingType( ScalarTypes B ) const
{
	ScalarTypes A = DataType();
	if ( A != B ) {
		std::ostringstream msg;
		msg << "Casting error: cannot cast from " << TypeNames[A] << " to " << TypeNames[B];
		throw std::logic_error( msg.str() );
	}
}

template <typename Int>
void AutoMatrix<Int>::AssertDataTypes( ScalarTypes B, bool unknown_ok ) const
{ 
	ScalarTypes A = DataType();
	if ( A != B ) {
		std::ostringstream msg;
		msg << "Data type mismatch: " << TypeNames[A] << " != " << TypeNames[B];
		throw std::logic_error( msg.str() );
	}
	if ( A == UNKNOWN || B == UNKNOWN )
		throw std::logic_error( "Cannot use this method with UNKNOWN data types" );
}

template <typename Int>
void AutoMatrix<Int>::AssertCRDataTypes( ScalarTypes B, bool imag_bad ) const
{
	ScalarTypes A = DataType();
	if ( RealTypes[A] != B ) {
		std::ostringstream msg;
		msg << "Data type mismatch: Real(" << TypeNames[A] << ") != " << TypeNames[B];
		throw std::logic_error( msg.str() );
	}
	if ( imag_bad && !ComplexFlag[A] )
		throw std::logic_error( "Cannot use this method with real data" );
}

template <typename Int>
void AutoMatrix<Int>::AssertUnlocked( LockTypes ltype ) const
{
	if ( locked_ )
		switch ( ltype ) {
		case ViewLock:
			throw std::logic_error( "Cannot take an unlocked view of a locked matrix" );
		case PartitionLock:
			throw std::logic_error( "Cannot create an unlocked partition of a locked matrix" );
		default:
			throw std::logic_error( "Cannot perform this operation on a locked matrix" );
		}
}

template <class T>
static void AssertBuffer( const T* buffer )
{
	if ( buffer == 0 )
		throw std::logic_error("Trying to operate on an empty buffer");
}

static void AssertBuffer( bool buffer )
{
	if ( !buffer )
		throw std::logic_error("Trying to operate on an empty buffer");
}

//
// Constructors
//

template<typename Int>
AutoMatrix<Int>::AutoMatrix( size_t dsize )
: height_(0), width_(0), ldim_(1),
  viewing_(false), locked_(false), 
  dsize_(dsize), numel_(0), data_(0)
{ }

template <typename T,typename Int>
Matrix<T,Int>::Matrix()
: AutoMatrix<Int>( sizeof(T) )
{ }

template<typename Int>
AutoMatrix<Int>::AutoMatrix( size_t dsize, Int height, Int width, Int ldim )
: height_(0), width_(0), ldim_(1),
  viewing_(false), locked_(false), 
  dsize_(dsize), numel_(0), data_(0)
{ 
#ifndef RELEASE
	AssertDimensions( height, width, ldim );
#endif
	ResizeTo_( height, width, ldim );
}

template<typename T,typename Int>
Matrix<T,Int>::Matrix( Int height, Int width )
: AutoMatrix<Int>( sizeof(T), height, width, std::max(height,1) )
{ }

template<typename T,typename Int>
Matrix<T,Int>::Matrix( Int height, Int width, Int ldim )
: AutoMatrix<Int>( sizeof(T), height, width, ldim )
{ }

template<typename Int>
AutoMatrix<Int>::AutoMatrix( size_t dsize, Int height, Int width, void* data, Int ldim )
: height_(0), width_(0), ldim_(1),
  viewing_(false), locked_(false), 
  dsize_(dsize), numel_(0), data_(0)
{
#ifndef RELEASE
	AssertDimensions( height, width, ldim );
#endif
	Attach_( height, width, data, ldim, false );
}

template<typename T,typename Int>
Matrix<T,Int>::Matrix( Int height, Int width, T* data, Int ldim )
: AutoMatrix<Int>( sizeof(T), height, width, data, ldim )
{}

template<typename Int>
AutoMatrix<Int>::AutoMatrix( size_t dsize, Int height, Int width, const void* data, Int ldim )
: height_(0), width_(0), ldim_(1),
  viewing_(false), locked_(false), 
  dsize_(dsize), numel_(0), data_(0)
{
#ifndef RELEASE
	AssertDimensions( height, width, ldim );
#endif
	Attach_( height, width, data, ldim, true );
}

template<typename T,typename Int>
Matrix<T,Int>::Matrix( Int height, Int width, const T* data, Int ldim )
: AutoMatrix<Int>( sizeof(T), height, width, data, ldim )
{}

template<typename Int>
AutoMatrix<Int>* AutoMatrix<Int>::Create( ScalarTypes stype )
{
	switch ( stype ) {
	case INTEGRAL: return new Matrix<Int,Int>();
#ifndef DISABLE_FLOAT
	case SINGLE:    return new Matrix<float,Int>();
#endif	
	case DOUBLE:   return new Matrix<double,Int>();
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT	
	case SCOMPLEX: return new Matrix<scomplex,Int>();
#endif	
	case DCOMPLEX: return new Matrix<dcomplex,Int>();
#endif	
	default: throw std::runtime_error( "Cannot dynamically create a matrix of UNKNOWN type" );
	return 0;
	}
}

template<typename Int>
AutoMatrix<Int>* AutoMatrix<Int>::Create( ScalarTypes stype, Int height, Int width )
{
	PushCallStack("AutoMatrix::Create( height, width )");
	AssertDimensions( height, width, 0, true );
	PopCallStack();
	switch ( stype ) {
	case INTEGRAL: return new Matrix<Int,Int>( height, width );
#ifndef DISABLE_FLOAT
	case SINGLE:    return new Matrix<float,Int>( height, width );
#endif	
	case DOUBLE:   return new Matrix<double,Int>( height, width );
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT	
	case SCOMPLEX: return new Matrix<scomplex,Int>( height, width );
#endif	
	case DCOMPLEX: return new Matrix<dcomplex,Int>( height, width );
#endif	
	default: throw std::runtime_error( "Cannot create AutoMatrix of unknown type" );
	return 0;
	}
}

template<typename Int>
AutoMatrix<Int>* AutoMatrix<Int>::Create( ScalarTypes stype, Int height, Int width, Int ldim )
{
	PushCallStack("AutoMatrix::Create( height, width, ldim )");
	AssertDimensions( height, width, ldim );
	PopCallStack();
	switch ( stype ) {
	case INTEGRAL: return new Matrix<Int,Int>( height, width, ldim );
#ifndef DISABLE_FLOAT
	case SINGLE:    return new Matrix<float,Int>( height, width, ldim );
#endif	
	case DOUBLE:   return new Matrix<double,Int>( height, width, ldim );
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT	
	case SCOMPLEX: return new Matrix<scomplex,Int>( height, width, ldim );
#endif	
	case DCOMPLEX: return new Matrix<dcomplex,Int>( height, width, ldim );
#endif	
	default: throw std::runtime_error( "Cannot create AutoMatrix of unknown type" );
	return 0;
	}
}

template<typename Int>
AutoMatrix<Int>* AutoMatrix<Int>::Create( Int height, Int width, const elem::Buffer<Int>& buffer, Int ldim )
{
	PushCallStack("AutoMatrix::Create( height, width, buffer, ldim )");
	AssertDimensions( height, width, ldim );
	AssertBuffer( buffer );
	PopCallStack();
	switch ( buffer.Type() ) {
	case INTEGRAL: return new Matrix<Int,Int>( height, width, buffer.template Get<Int>(), ldim );
#ifndef DISABLE_FLOAT
	case SINGLE:   return new Matrix<float,Int>( height, width, buffer.template Get<float>(), ldim );
#endif	
	case DOUBLE:   return new Matrix<double,Int>( height, width, buffer.template Get<double>(), ldim );
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT	
	case SCOMPLEX: return new Matrix<scomplex,Int>( height, width, buffer.template Get<scomplex>(), ldim );
#endif	
	case DCOMPLEX: return new Matrix<dcomplex,Int>( height, width, buffer.template Get<dcomplex>(), ldim );
#endif	
	default: throw std::runtime_error( "Cannot create AutoMatrix of unknown type" );
	return 0;
	}
}

template<typename Int>
AutoMatrix<Int>* AutoMatrix<Int>::Create( Int height, Int width, const ConstBuffer<Int>& buffer, Int ldim )
{
	PushCallStack("AutoMatrix::Create( height, width, const buffer, ldim )");
	AssertDimensions( height, width, ldim );
	AssertBuffer( buffer );
	PopCallStack();
	switch ( buffer.Type() ) {
	case INTEGRAL: return new Matrix<Int,Int>( height, width, buffer.template Get<Int>(), ldim );
#ifndef DISABLE_FLOAT
	case SINGLE:   return new Matrix<float,Int>( height, width, buffer.template Get<float>(), ldim );
#endif	
	case DOUBLE:   return new Matrix<double,Int>( height, width, buffer.template Get<double>(), ldim );
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT	
	case SCOMPLEX: return new Matrix<scomplex,Int>( height, width, buffer.template Get<scomplex>(), ldim );
#endif	
	case DCOMPLEX: return new Matrix<dcomplex,Int>( height, width, buffer.template Get<dcomplex>(), ldim );
#endif	
	default: throw std::runtime_error( "Cannot create AutoMatrix of unknown type" );
	return 0;
	}
}

template <typename Int>
AutoMatrix<Int>::AutoMatrix( const Self& A )
: viewing_(false), locked_(false), dsize_(A.dsize_), data_(0)
{ 
	CopyFrom_( A ); 
}

template<typename T,typename Int>
Matrix<T,Int>::Matrix( const Self& A )
: AutoMatrix<Int>( A )
{}

template<typename T,typename Int>
Matrix<T,Int>*
Matrix<T,Int>::CloneEmpty() const
{ return new Self; }

template<typename T,typename Int>
Matrix<T,Int>*
Matrix<T,Int>::Clone() const
{
	return new Self( *this );
}

template <typename Int>
template <typename T>
Matrix<T,Int>& AutoMatrix<Int>::Cast()
{
	AssertCastingType( ScalarType<T>::Enum );
	return reinterpret_cast<elem::Matrix<T,Int>&>(*this);
}

template <typename Int>
template <typename T>
const Matrix<T,Int>& AutoMatrix<Int>::Cast() const
{
	AssertCastingType( ScalarType<T>::Enum );
	return reinterpret_cast<const elem::Matrix<T,Int>&>(*this);
}

//
// Destructor
//

template<typename Int>
AutoMatrix<Int>::~AutoMatrix()
{ 
	if ( !viewing_ )
		delete [] data_;
}

//
// Basic information
//

template<typename Int>
void*
AutoMatrix<Int>::Buffer()
{
    PushCallStack("AutoMatrix::Buffer");
    AssertUnlocked();
    PopCallStack();
    return Buffer_();
}

template<typename Int>
void*
AutoMatrix<Int>::Buffer( Int i, Int j )
{ 
    PushCallStack("AutoMatrix::Buffer");
    AssertBounds( i, j );
    AssertUnlocked();
    PopCallStack();
    return Buffer_( i, j );
}

template<typename Int>
const void*
AutoMatrix<Int>::LockedBuffer( Int i, Int j ) const
{
    PushCallStack("AutoMatrix::LockedBuffer");
    AssertBounds( i, j );
    PopCallStack();
    return LockedBuffer_( i, j );
}

//
// I/O
//

template <typename Int>
void
AutoMatrix<Int>::Print( std::ostream& os, const std::string msg ) const
{
    PushCallStack("AutoMatrix::Print");
    if( msg != "" )
        os << msg << std::endl;
    os << "Matrix: (h,w,l)=(" << height_ << "," << width_ << "," << ldim_ 
       << "), dtype=" << TypeNames[DataType()];
    if ( DataType() == UNKNOWN )
    	os << ", dsize=" << DataSize();
    os << std::endl;
    PopCallStack();
}

template<typename T,typename Int>
void
Matrix<T,Int>::Print( std::ostream& os, const std::string msg ) const
{
    PushCallStack("Matrix::Print");
    Parent::Print( os, msg );
    const Int height = Parent::Height();
    const Int width = Parent::Width();
    for( Int i=0; i<height; ++i )
    {
        for( Int j=0; j<width; ++j )
            os << Get(i,j) << " ";
        os << std::endl;
    }
    os << std::endl;
    PopCallStack();
}

//
// Entry manipulation
//

template <typename Int>
Scalar<Int>
AutoMatrix<Int>::Get( Int i, Int j ) const
{
	Scalar v;
	AssertBounds( i, j );
	Get_( i, j, v );
	return v;
}

template<typename T,typename Int>
void
Matrix<T,Int>::Get_( Int i, Int j, Scalar& v ) const
{ 
	v = Get_( i, j ); 
}

template <typename Int>
void
AutoMatrix<Int>::Set( Int i, Int j, const Scalar& v )
{
	AssertBounds( i, j );
	AssertUnlocked();
	Set_( i, j, v );
}

template<typename T,typename Int>
void
Matrix<T,Int>::Set_( Int i, Int j, const Scalar& v ) 
{ 
	T vv = v.template Cast<T>();
	Set_( i, j, vv ); 
}

template <typename Int>
void
AutoMatrix<Int>::Update( Int i, Int j, const Scalar& v )
{
	AssertBounds( i, j );
	AssertUnlocked();
	Update_( i, j, v );
}

template<typename T,typename Int>
void
Matrix<T,Int>::Update_( Int i, Int j, const Scalar& v ) 
{ 
	T vv = v.template Cast<T>();
	Update_( i, j, vv ); 
}

template <typename Int>
Scalar<Int>
AutoMatrix<Int>::GetRealPart( Int i, Int j ) const
{
	Scalar v;
	AssertBounds( i, j );
	GetRealPart_( i, j, v );
	return v;
}

template<typename T,typename Int>
void
Matrix<T,Int>::GetRealPart_( Int i, Int j, Scalar& v ) const
{ 
	v = GetRealPart_( i, j ); 
}

template <typename Int>
void
AutoMatrix<Int>::SetRealPart( Int i, Int j, const Scalar& v )
{
	AssertBounds( i, j );
	AssertUnlocked();
	SetRealPart_( i, j, v );
}

template<typename T,typename Int>
void
Matrix<T,Int>::SetRealPart_( Int i, Int j, const Scalar& v )
{ 
	RT vv = v.template Cast<RT>();
	SetRealPart_( i, j, vv ); 
}

template <typename Int>
void
AutoMatrix<Int>::UpdateRealPart( Int i, Int j, const Scalar& v )
{
	AssertBounds( i, j );
	AssertUnlocked();
	UpdateRealPart_( i, j, v );
}

template<typename T,typename Int>
void
Matrix<T,Int>::UpdateRealPart_( Int i, Int j, const Scalar& v )
{ 
	RT vv = v.template Cast<RT>();
	UpdateRealPart_( i, j, vv ); 
}

template <typename Int>
Scalar<Int>
AutoMatrix<Int>::GetImagPart( Int i, Int j ) const
{
	Scalar v;
	AssertBounds( i, j );
	GetImagPart_( i, j, v );
	return v;
}

template<typename T,typename Int>
void
Matrix<T,Int>::GetImagPart_( Int i, Int j, Scalar& v ) const
{ 
	v = GetImagPart_( i, j ); 
}

template <typename Int>
void
AutoMatrix<Int>::SetImagPart( Int i, Int j, const Scalar& v )
{
	AssertBounds( i, j );
	AssertUnlocked();
	SetImagPart_( i, j, v );
}

template<typename T,typename Int>
void
Matrix<T,Int>::SetImagPart_( Int i, Int j, const Scalar& v )
{ 
	RT vv = v.template Cast<RT>();
	SetImagPart_( i, j, vv ); 
}

template <typename Int>
void
AutoMatrix<Int>::UpdateImagPart( Int i, Int j, const Scalar& v )
{
	AssertBounds( i, j );
	AssertUnlocked();
	UpdateImagPart_( i, j, v );
}

template<typename T,typename Int>
void
Matrix<T,Int>::UpdateImagPart_( Int i, Int j, const Scalar& v )
{ 
	RT vv = v.template Cast<RT>();
	UpdateImagPart_( i, j, vv );
}

//
// DIAGONAL MANIPULATION
//

template <typename Int>
void
AutoMatrix<Int>::GetDiagonal( Self& d, Int offset ) const
{
	PushCallStack( "AutoMatrix::GetDiagonal" );
	AssertDataTypes( d );
	if ( d.viewing_ && ( d.height_ != DiagonalLength( offset ) || d.width_ != 1 ) )
        throw std::logic_error("d is not a column-vector of the right length");
    GetDiagonal_( d, offset );
}

template<typename T,typename Int>
void
Matrix<T,Int>::GetDiagonal_( Parent& dd, Int offset ) const
{ 
	Self& d = static_cast<Self&>( dd );
    const Int diagLength = Parent::DiagonalLength(offset);
    if( !d.Viewing() )    
        d.ResizeTo( diagLength, 1 );
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            d.Set( j, 0, Get(j,j+offset) );
    else
        for( Int j=0; j<diagLength; ++j )
            d.Set( j, 0, Get(j-offset,j) );
}

template <typename Int>
void
AutoMatrix<Int>::SetDiagonal( const Self& d, Int offset )
{
	PushCallStack( "AutoMatrix::GetDiagonal" );
	AssertDataTypes( d );
	if ( d.height_ != DiagonalLength( offset ) || d.width_ != 1 )
        throw std::logic_error("d is not a column-vector of the right length");
	SetDiagonal_( d, offset );        
}

template<typename T,typename Int>
void
Matrix<T,Int>::SetDiagonal_( const AutoMatrix<Int>& dd, Int offset )
{ 
	const Self& d = static_cast<const Self&>(dd);
    const Int diagLength = Parent::DiagonalLength(offset);
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            Set( j, j+offset, d.Get(j,0) );
    else
        for( Int j=0; j<diagLength; ++j )
            Set( j-offset, j, d.Get(j,0) );
}

template <typename Int>
void
AutoMatrix<Int>::UpdateDiagonal( const AutoMatrix<Int>& d, Int offset )
{
	PushCallStack( "AutoMatrix::GetDiagonal" );
	AssertDataTypes( d );
	if ( d.height_ != DiagonalLength( offset ) || d.width_ != 1 )
        throw std::logic_error("d is not a column-vector of the right length");
    UpdateDiagonal_( d, offset );
}

template <typename T,typename Int>
void
Matrix<T,Int>::UpdateDiagonal_( const AutoMatrix<Int>& dd, Int offset )
{
	const Self& d = static_cast<const Self&>(dd);
    const Int diagLength = Parent::DiagonalLength(offset);
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            Update( j, j+offset, d.Get(j,0) );
    else
        for( Int j=0; j<diagLength; ++j )
            Update( j-offset, j, d.Get(j,0) );
}

template<typename Int>
void
AutoMatrix<Int>::GetRealPartOfDiagonal
( Self& d, Int offset ) const
{
	PushCallStack("AutoMatrix::GetRealPartOfDiagonal");
	AssertCRDataTypes( d );
	if ( d.viewing_ && ( d.height_ != DiagonalLength( offset ) || d.width_ != 1 ) )
        throw std::logic_error("d is not a column-vector of the right length");
    GetRealPartOfDiagonal_( d, offset );
}

template<typename T,typename Int>
void
Matrix<T,Int>::GetRealPartOfDiagonal_
( AutoMatrix<Int>& dd, Int offset ) const
{ 
	RSelf& d = static_cast<RSelf&>(dd);
    const Int diagLength = Parent::DiagonalLength(offset);
    if( !d.Viewing() )    
        d.ResizeTo( diagLength, 1 );
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            d.Set_( j, 0, GetRealPart_(j,j+offset) );
    else
        for( Int j=0; j<diagLength; ++j )
            d.Set_( j, 0, GetRealPart_(j-offset,j) );
}

template<typename Int>
void
AutoMatrix<Int>::GetImagPartOfDiagonal
( Self& d, Int offset ) const
{
	PushCallStack("AutoMatrix::GetImagPartOfDiagonal");
	AssertCRDataTypes( d );
	if ( d.viewing_ && ( d.height_ != DiagonalLength( offset ) || d.width_ != 1 ) )
        throw std::logic_error("d is not a column-vector of the right length");
    GetImagPartOfDiagonal_( d, offset );
}

template<typename T,typename Int>
void
Matrix<T,Int>::GetImagPartOfDiagonal_
( AutoMatrix<Int>& dd, Int offset ) const
{ 
	RSelf& d = static_cast<RSelf&>(dd);
    const Int diagLength = Parent::DiagonalLength(offset);
    if( !d.Viewing() )    
        d.ResizeTo( diagLength, 1 );
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            d.Set_( j, 0, GetImagPart_(j,j+offset) );
    else
        for( Int j=0; j<diagLength; ++j )
            d.Set_( j, 0, GetImagPart_(j-offset,j) );
}

template <typename Int>
void
AutoMatrix<Int>::SetRealPartOfDiagonal
( const AutoMatrix<Int>& d, Int offset )
{
    PushCallStack("AutoMatrix::SetRealPartOfDiagonal");
    AssertCRDataTypes( d );
    if( d.Height() != Parent::DiagonalLength(offset) || d.Width() != 1 )
        throw std::logic_error("d is not a column-vector of the right length");
    SetRealPartOfDiagonal_( d, offset );
    PopCallStack();
}

template<typename T,typename Int>
void
Matrix<T,Int>::SetRealPartOfDiagonal_
( const AutoMatrix<Int>& dd, Int offset )
{ 
	const RSelf& d = static_cast<const RSelf&>(dd);
    const Int diagLength = Parent::DiagonalLength(offset);
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            SetRealPart_( j, j+offset, d.Get_(j,0) );
    else
        for( Int j=0; j<diagLength; ++j )
            SetRealPart_( j-offset, j, d.Get_(j,0) );
}

template <typename Int>
void
AutoMatrix<Int>::SetImagPartOfDiagonal
( const AutoMatrix<Int>& d, Int offset )
{
    PushCallStack("AutoMatrix::SetImagPartOfDiagonal");
    AssertCRDataTypes( d, true );
    if( d.Height() != Parent::DiagonalLength(offset) || d.Width() != 1 )
        throw std::logic_error("d is not a column-vector of the right length");
    SetImagPartOfDiagonal_( d, offset );
    PopCallStack();
}

template<typename T,typename Int>
void
Matrix<T,Int>::SetImagPartOfDiagonal_
( const AutoMatrix<Int>& dd, Int offset )
{ 
	const RSelf& d = static_cast<const RSelf&>(dd);
    const Int diagLength = Parent::DiagonalLength(offset);
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            SetImagPart_( j, j+offset, d.Get_(j,0) );
    else
        for( Int j=0; j<diagLength; ++j )
            SetImagPart_( j-offset, j, d.Get_(j,0) );
}

template <typename Int>
void
AutoMatrix<Int>::UpdateRealPartOfDiagonal
( const AutoMatrix<Int>& d, Int offset )
{
    PushCallStack("AutoMatrix::UpdateRealPartOfDiagonal");
    AssertCRDataTypes( d );
    if( d.Height() != Parent::DiagonalLength(offset) || d.Width() != 1 )
        throw std::logic_error("d is not a column-vector of the right length");
    UpdateRealPartOfDiagonal_( d, offset );
    PopCallStack();
}

template<typename T,typename Int>
void
Matrix<T,Int>::UpdateRealPartOfDiagonal_
( const AutoMatrix<Int>& dd, Int offset )
{ 
	const RSelf& d = static_cast<const RSelf&>(dd);
    const Int diagLength = Parent::DiagonalLength(offset);
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            UpdateRealPart_( j, j+offset, d.Get_(j,0) );
    else
        for( Int j=0; j<diagLength; ++j )
            UpdateRealPart_( j-offset, j, d.Get_(j,0) );
}

template <typename Int>
void
AutoMatrix<Int>::UpdateImagPartOfDiagonal
( const AutoMatrix<Int>& d, Int offset )
{
    PushCallStack("AutoMatrix::UpdateImagPartOfDiagonal");
    AssertCRDataTypes( d, true );
    if( d.Height() != Parent::DiagonalLength(offset) || d.Width() != 1 )
        throw std::logic_error("d is not a column-vector of the right length");
    UpdateImagPartOfDiagonal_( d, offset );
    PopCallStack();
}

template<typename T,typename Int>
void
Matrix<T,Int>::UpdateImagPartOfDiagonal_
( const AutoMatrix<Int>& dd, Int offset )
{ 
	const RSelf& d = static_cast<const RSelf&>(dd);
    const Int diagLength = Parent::DiagonalLength(offset);
    if( offset >= 0 )
        for( Int j=0; j<diagLength; ++j )
            UpdateImagPart_( j, j+offset, d.Get_(j,0) );
    else
        for( Int j=0; j<diagLength; ++j )
            UpdateImagPart_( j-offset, j, d.Get_(j,0) );
}

//
// Utilities
//

template<typename Int>
void AutoMatrix<Int>::CopyFrom_( const Self& A )
{
    if ( !viewing_ )
        ResizeTo_( A.height_, A.width_, std::max(A.height_,1) );
    Int height = height_ * dsize_;
    Int ldim = ldim_  * dsize_;
    Int ldimOfA = A.ldim_ * dsize_;
    byte *dst = data_;
    const byte* src = A.data_;
#ifdef HAVE_OPENMP
    #pragma omp parallel for
#endif
    for( Int j = 0; j < width_ ; ++j ) {
        MemCopy( dst, src, height );
        dst += ldim;
        src += ldimOfA;
    }
}

template<typename Int>
void
AutoMatrix<Int>::CopyFrom( const Self& A )
{
    PushCallStack("AutoMatrix::CopyFrom");
    AssertDataTypes( A );
    AssertUnlocked();
	if ( viewing_ && ( height_ != A.height_ || width_ != A.width_ ) )
		throw std::logic_error
			("Cannot assign to a view of different dimensions");
	CopyFrom_( A );			
    PopCallStack();
}

//
// RESIZING
// Only change ldim when necessary. Simply 'shrink' our view if possible.
//

template <typename Int>
void 
AutoMatrix<Int>::Empty()
{
	if ( !viewing_ )
		delete [] data_;
	viewing_ = false;
	locked_ = false;
	height_ = 0;
	width_ = 0;
	ldim_ = 1;
	numel_ = 0;
	data_ = 0;
}

template<typename Int>
void
AutoMatrix<Int>::ResizeTo_( Int height, Int width, Int ldim )
{
    // Only change the ldim when necessary. Simply 'shrink' our view if possible.
	if ( !viewing_ ) {
		size_t nelem = ldim * width;
		if ( numel_ < nelem ) {
			delete [] data_;
			if ( nelem )
				data_ = new byte [ nelem * dsize_ ];
			numel_ = nelem;
		}
	}
    height_ = height;
	width_ = width;
	ldim_ = ldim;
}

template<typename Int>
void
AutoMatrix<Int>::ResizeTo_( Int height, Int width )
{
	Int ldim;
	if ( height > height_ || width > width_ )
		ldim = std::max( height, 1 );
	else
		ldim = ldim_;
	ResizeTo_( height, width, ldim );
}

template<typename Int>
void
AutoMatrix<Int>::ResizeTo( Int height, Int width )
{
    PushCallStack("AutoMatrix::ResizeTo(height,width)");
    AssertNonnegative( height, "height" );
    AssertNonnegative( width, "width" );
    if ( viewing_ && ( height > height_ || width > width_ ) )
        throw std::logic_error("Cannot increase the size of a view");
	ResizeTo_( height, width );
    PopCallStack();
}

template<typename Int>
void
AutoMatrix<Int>::ResizeTo( Int height, Int width, Int ldim )
{
    PushCallStack("AutoMatrix::ResizeTo(height,width,ldim)");
    AssertNonnegative( height, "height" );
    AssertNonnegative( width, "width" );
    AssertLDim( height, ldim );
    if( viewing_ && ( height > height_ || width > width_ ) )
        throw std::logic_error("Cannot increase the size of a view");
    if( viewing_ && ldim != ldim_ )
        throw std::logic_error("Cannot modify the ldim of a view");
    ResizeTo_( height, width, ldim );
    PopCallStack();
}

// ATTACH FUNCTIONS

template<typename Int>
void
AutoMatrix<Int>::Attach_
( Int height, Int width, const void* buffer, Int ldim, bool locked )
{
	if ( !viewing_ )
		delete [] data_;
    height_  = height;
    width_   = width;
    ldim_    = ldim;
    viewing_ = true;
    locked_  = locked;
	numel_   = 0;
	data_    = static_cast<byte*>(const_cast<void*>(buffer));
}

template<typename Int>
void
AutoMatrix<Int>::Attach
( ScalarTypes dtype, Int height, Int width, const void* buffer, Int ldim, bool lock )
{
    PushCallStack("AutoMatrix::Attach");
    AssertDataTypes( dtype );
    AssertDimensions( height, width, ldim );
    AssertBuffer( buffer );
	Attach_( height, width, buffer, ldim, lock );
    PopCallStack();
}

//
// Explicit instantiations
//

#define COMMA ,
#undef TEMPLATE_INST
#define TEMPLATE_INST(T) \
	template class Matrix<T COMMA int>; \
	template Matrix<T,int>& AutoMatrix<int>::Cast<T>(); \
	template const Matrix<T,int>& AutoMatrix<int>::Cast<T>() const;
	
template class AutoMatrix<int>;
TEMPLATE_INST(int)
#ifndef DISABLE_FLOAT
TEMPLATE_INST(float)
#endif // ifndef DISABLE_FLOAT
TEMPLATE_INST(double)
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
TEMPLATE_INST(scomplex)
#endif // ifndef DISABLE_FLOAT
TEMPLATE_INST(dcomplex)
#endif // ifndef DISABLE_COMPLEX

#undef TEMPLATE_INST
#define TEMPLATE_INST(T,M,I) \
	template void View T ( M&, M& ); \
	template void LockedView T ( M&, const M& ); \
	template void View T ( M&, M&, I, I, I, I ); \
	template void LockedView T ( M&, const M&, I, I, I, I ); \
	template void View1x2 T ( M&, M&, M& ); \
	template void LockedView1x2 T ( M&, const M&, const M& ); \
	template void View2x1 T ( M&, M&, M& ); \
	template void LockedView2x1 T ( M&, const M&, const M& ); \
	template void View2x2 T ( M&, M&, M&, M&, M& ); \
	template void LockedView2x2 T ( M&, const M&, const M&, const M&, const M& ); \
	template void PartitionUp T ( M&, M&, M&, I ); \
	template void LockedPartitionUp T ( const M&, M&, M&, I ); \
	template void PartitionDown T ( M&, M&, M&, I ); \
	template void LockedPartitionDown T ( const M&, M&, M&, I ); \
	template void PartitionLeft T ( M&, M&, M&, I ); \
	template void LockedPartitionLeft T ( const M&, M&, M&, I ); \
	template void PartitionRight T ( M&, M&, M&, I ); \
	template void LockedPartitionRight T ( const M&, M&, M&, I ); \
	template void PartitionUpDiagonal T ( M&, M&, M&, M&, M&, I ); \
	template void LockedPartitionUpDiagonal T ( const M&, M&, M&, M&, M&, I ); \
	template void PartitionUpLeftDiagonal T ( M&, M&, M&, M&, M&, I ); \
	template void LockedPartitionUpLeftDiagonal T ( const M&, M&, M&, M&, M&, I ); \
	template void PartitionUpRightDiagonal T ( M&, M&, M&, M&, M&, I ); \
	template void LockedPartitionUpRightDiagonal T ( const M&, M&, M&, M&, M&, I ); \
	template void PartitionDownDiagonal T ( M&, M&, M&, M&, M&, I ); \
	template void LockedPartitionDownDiagonal T ( const M&, M&, M&, M&, M&, I ); \
	template void PartitionDownLeftDiagonal T ( M&, M&, M&, M&, M&, I ); \
	template void LockedPartitionDownLeftDiagonal T ( const M&, M&, M&, M&, M&, I ); \
	template void PartitionDownRightDiagonal T ( M&, M&, M&, M&, M&, I ); \
	template void LockedPartitionDownRightDiagonal T ( const M&, M&, M&, M&, M&, I ); \
	template void RepartitionUp T ( M&, M&, M&, M&, M&, I ); \
	template void LockedRepartitionUp T ( const M&, M&, M&, const M&, M&, I ); \
	template void RepartitionDown T ( M&, M&, M&, M&, M&, I ); \
	template void LockedRepartitionDown T ( const M&, M&, M&, const M&, M&, I ); \
	template void RepartitionLeft T ( M&, M&, M&, M&, M&, I ); \
	template void LockedRepartitionLeft T ( const M&, const M&, M&, M&, M&, I ); \
	template void RepartitionRight T ( M&, M&, M&, M&, M&, I ); \
	template void LockedRepartitionRight T ( const M&, const M&, M&, M&, M&, I ); \
	template void RepartitionUpDiagonal T ( M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, I ); \
	template void LockedRepartitionUpDiagonal T ( const M&, const M&, M&, M&, M&, M&, M&, M&, const M&, const M&, M&, M&, M&, I ); \
	template void RepartitionDownDiagonal T ( M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, I ); \
	template void LockedRepartitionDownDiagonal T ( const M&, const M&, M&, M&, M&, M&, M&, M&, const M&, const M&, M&, M&, M&, I ); \
	template void SlidePartitionUp( M&, M&, M&, M&, M& ); \
	template void SlideLockedPartitionUp( M&, const M&, const M&, M&, const M& ); \
	template void SlidePartitionDown( M&, M&, M&, M&, M& ); \
	template void SlideLockedPartitionDown( M&, const M&, const M&, M&, const M& ); \
	template void SlidePartitionLeft( M&, M&, M&, M&, M& ); \
	template void SlideLockedPartitionLeft( M&, M&, const M&, const M&, const M& ); \
	template void SlidePartitionRight( M&, M&, M&, M&, M& ); \
	template void SlideLockedPartitionRight( M&, M&, const M&, const M&, const M& ); \
	template void SlidePartitionUpDiagonal( M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M& ); \
	template void SlideLockedPartitionUpDiagonal( M&, M&, const M&, const M&, const M&, const M&, const M&, const M&, M&, M&, const M&, const M&, const M& ); \
	template void SlidePartitionDownDiagonal( M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M&, M& ); \
	template void SlideLockedPartitionDownDiagonal( M&, M&, const M&, const M&, const M&, const M&, const M&, const M&, M&, M&, const M&, const M&, const M& );
	
TEMPLATE_INST( <int>, AutoMatrix <int>, int )
#ifndef DISABLE_FLOAT
TEMPLATE_INST( <float COMMA int>, Matrix <float COMMA int>, int )
#endif
TEMPLATE_INST( <double COMMA int>, Matrix <double COMMA int>, int )
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
TEMPLATE_INST( <scomplex COMMA int>, Matrix <scomplex COMMA int>, int )
#endif
TEMPLATE_INST( <dcomplex COMMA int>, Matrix <dcomplex COMMA int>, int )
#endif

} // namespace elem
