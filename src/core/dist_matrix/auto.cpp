/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "elemental-lite.hpp"

#define ADM AutoDistMatrix<Int>
#define AM AutoMatrix<Int>
#define SCA Scalar<Int>

namespace elem {

template<typename Int>
void DistData<Int>::PrintShort( std::ostream& os ) const
{
	os << "DistData(" << DistToString( colDist ) << "," << DistToString( rowDist ) << ","
	   << colAlignment << "," << rowAlignment << "," << diagPath << ",grid)";
}

template <typename Int>
template <typename T>
AbstractDistMatrix<T,Int>& AutoDistMatrix<Int>::Cast()
{
	AssertDataTypes( DataType(), ScalarType<T>::Enum );
	return *reinterpret_cast<elem::AbstractDistMatrix<T,Int>*>(this);
}

template <typename Int>
template <typename T>
const AbstractDistMatrix<T,Int>& AutoDistMatrix<Int>::Cast() const
{
	AssertDataTypes( DataType(), ScalarType<T>::Enum );
	return *reinterpret_cast<const elem::AbstractDistMatrix<T,Int>*>(this);
}

template <typename Int>
template <typename T,Distribution U,Distribution V>
DistMatrix<T,U,V,Int>& AutoDistMatrix<Int>::Cast()
{
	AssertDataTypes( DataType(), ScalarType<T>::Enum );
	AssertDistributions( RowDist(), ColDist(), U, V );
	return *reinterpret_cast<elem::DistMatrix<T,U,V,Int>*>(this);
}

template <typename Int>
template <typename T,Distribution U,Distribution V>
const DistMatrix<T,U,V,Int>& AutoDistMatrix<Int>::Cast() const
{
	AssertDataTypes( DataType(), ScalarType<T>::Enum );
	AssertDistributions( RowDist(), ColDist(), U, V );
	return *reinterpret_cast<const elem::DistMatrix<T,U,V,Int>*>(this);
}

template<typename Int>
ADM::AutoDistMatrix
( Int height, Int width, 
  bool constrainedColAlignment, bool constrainedRowAlignment,
  Int colAlignment, Int rowAlignment,
  Int colShift, Int rowShift, 
  bool viewing, bool locked,
  const elem::Grid& grid )
: viewing_(viewing), locked_(locked), 
  height_(height), width_(width), 
  constrainedColAlignment_(constrainedColAlignment), 
  constrainedRowAlignment_(constrainedRowAlignment),
  colAlignment_(colAlignment), rowAlignment_(rowAlignment),
  colShift_(colShift), rowShift_(rowShift),
  grid_(&grid)
{ } 

template<typename Int>
ADM::~AutoDistMatrix() 
{ }

template<typename Int>
void
ADM::AssertNotLocked() const
{
    if( viewing_ && locked_ )
        throw std::logic_error
        ("Assertion that matrix not be a locked view failed");
}

template<typename Int>
void
ADM::AssertNotStoringData() const
{
    if( LockedMatrix().MemorySize() > 0 )
        throw std::logic_error
        ("Assertion that matrix not be storing data failed");
}

template<typename Int>
void
ADM::AssertFreeColAlignment() const
{
    if( constrainedColAlignment_ )
        throw std::logic_error
        ("Assertion that column alignment be free failed");
}

template<typename Int>
void
ADM::AssertFreeRowAlignment() const
{
    if( constrainedRowAlignment_ )
        throw std::logic_error("Assertion that row alignment be free failed");
}

template<typename Int> 
void
ADM::AssertSameGrid( const elem::Grid& grid ) const
{
    if( Grid() != grid )
        throw std::logic_error("Assertion that grids match failed");
}

template<typename Int> inline
Int ADM::DiagonalLength( Int offset ) const
{ 
	return elem::DiagonalLength(height_,width_,offset); 
}

template<typename Int>
void
ADM::PrintShort( std::ostream& os ) const
{
	os << "DistMatrix<" << ScalarTypeToString( DataType() ) << ","
	   << DistToString( RowDist() ) << "," << DistToString( ColDist() ) << ">("
	   << Height() << "," << Width();
	if ( Viewing() )
		os << "," << ( Locked() ? "locked view" : "view" );
	os << ")";
}

template<typename Int>
void
ADM::Print( const std::string msg ) const
{ 
	PushCallStack("DistMatrix::Print");
	PrintBase( std::cout, msg ); 
	PopCallStack();
}

template<typename Int>
void
ADM::Print
( std::ostream& os, const std::string msg ) const
{ 
	PushCallStack("DistMatrix::Print");
	PrintBase( os, msg ); 
	PopCallStack();
}

template<typename Int>
void
ADM::Write
( const std::string filename, const std::string msg ) const
{
    PushCallStack("DistMatrix::Write");
    const elem::Grid& g = Grid();
    const int commRank = mpi::CommRank( g.VCComm() ); 
    if( commRank == 0 )
    {
        std::ofstream file( filename.c_str() );
        file.setf( std::ios::scientific );
        PrintBase( file, msg );
        file.close();
    }
    else
    {
        NullStream nullStream;
        PrintBase( nullStream, msg );
    }
    PopCallStack();
}

template <typename Int>
Int ADM::DiagPath() const
{ return 0; }

template <typename Int>
void ADM::HandleDiagPath( const Self& B )
{ }

template <typename Int>
void
ADM::Align_( Int colAlignment, Int rowAlignment )
{
    AssertFreeRowAlignment();
    Empty();
    colAlignment_ = colAlignment;
    rowAlignment_ = rowAlignment;
    constrainedColAlignment_ = true;
    constrainedRowAlignment_ = true;
    SetShifts();
}

template<typename Int>
void
ADM::Align( Int colAlignment, Int rowAlignment )
{ 
    PushCallStack("DistMatrix::Align");    
    AssertFreeColAlignment();
    Align_( colAlignment, rowAlignment );
    PopCallStack();
}

template<typename Int>
void
ADM::AlignCols_( Int colAlignment )
{ 
    EmptyData();
    colAlignment_ = colAlignment;
    constrainedColAlignment_ = true;
    SetShifts();
}

template<typename Int>
void
ADM::AlignCols( Int colAlignment )
{ 
    PushCallStack("DistMatrix::AlignCols"); 
    AssertFreeColAlignment();
    AlignCols_( colAlignment );
    PopCallStack();
}

template<typename Int>
void
ADM::AlignRows_( Int rowAlignment )
{ 
    EmptyData();
    rowAlignment_ = rowAlignment;
    constrainedRowAlignment_ = true;
    SetShifts();
}

template<typename Int>
void
ADM::AlignRows( Int rowAlignment )
{ 
    PushCallStack("DistMatrix::AlignRows"); 
    AssertFreeRowAlignment();
    AlignRows_( rowAlignment );
    PopCallStack();
}

template<typename Int>
void
ADM::Empty()
{
    PushCallStack("DistMatrix::Empty"); 
    Matrix().Empty();
    locked_ = false;
    viewing_ = false;
    height_ = 0;
    width_ = 0;
    colAlignment_ = 0;
    rowAlignment_ = 0;
    constrainedColAlignment_ = false;
    constrainedRowAlignment_ = false;
    PopCallStack();
}

template<typename Int>
void
ADM::EmptyData()
{
	PushCallStack("DistMatrix::EmptyData");
    Matrix().Empty();
    locked_ = false;
    viewing_ = false;
    height_ = 0;
    width_ = 0;
	PopCallStack();
}

template<typename Int>
void
ADM::SetGrid( const elem::Grid& grid )
{
    Empty();
    grid_ = &grid; 
    SetShifts();
}

template<typename Int>
void
ADM::CopyFrom_( const ADM& A )
{
	throw std::logic_error( "This copy has not been implemented" );
}

template<typename Int>
bool
ADM::Participating() const
{ return grid_->InGrid(); }

template<typename Int>
void
ADM::AlignWith( const elem::DistData<Int>& data )
{ SetGrid( *data.grid ); }

template<typename Int>
void
ADM::AlignWith( const Self& A )
{ AlignWith( A.DistData() ); }

template<typename Int>
void
ADM::AlignColsWith( const elem::DistData<Int>& data )
{ 
    EmptyData(); 
    colAlignment_ = 0; 
    constrainedColAlignment_ = false; 
    SetShifts(); 
}

template<typename Int>
void
ADM::AlignColsWith( const ADM& A )
{ AlignColsWith( A.DistData() ); }

template<typename Int>
void
ADM::AlignRowsWith( const elem::DistData<Int>& data )
{ 
    EmptyData(); 
    rowAlignment_ = 0; 
    constrainedRowAlignment_ = false;
    SetShifts(); 
}

template<typename Int>
void
ADM::AlignRowsWith( const ADM& A )
{ AlignRowsWith( A.DistData() ); }

template<typename Int>
void
ADM::GetLocal( Int i, Int j, SCA& ans ) const
{ ans = LockedMatrix().Get(i,j); }

template<typename Int>
void
ADM::SetLocal( Int iLocal, Int jLocal, const SCA& alpha )
{ Matrix().Set(iLocal,jLocal,alpha); }

template<typename Int>
void
ADM::UpdateLocal( Int iLocal, Int jLocal, const SCA& alpha )
{ Matrix().Update(iLocal,jLocal,alpha); }

template<typename Int>
void ADM::GetLocalRealPart( Int iLocal, Int jLocal, SCA& ans ) const
{ ans = LockedMatrix().GetRealPart(iLocal,jLocal); }

template<typename Int>
void ADM::GetLocalImagPart( Int iLocal, Int jLocal, SCA& ans ) const
{ ans = LockedMatrix().GetImagPart(iLocal,jLocal); }

template<typename Int>
void
ADM::SetLocalRealPart
( Int iLocal, Int jLocal, const SCA& alpha )
{ Matrix().SetRealPart(iLocal,jLocal,alpha); }

template<typename Int>
void
ADM::SetLocalImagPart
( Int iLocal, Int jLocal, const SCA& alpha )
{ Matrix().SetImagPart(iLocal,jLocal,alpha); }

template<typename Int>
void
ADM::UpdateLocalRealPart
( Int iLocal, Int jLocal, const SCA& alpha )
{ Matrix().UpdateRealPart(iLocal,jLocal,alpha); }

template<typename Int>
void
ADM::UpdateLocalImagPart
( Int iLocal, Int jLocal, const SCA& alpha )
{ Matrix().UpdateImagPart(iLocal,jLocal,alpha); }

template<typename Int>
void
ADM::SetShifts()
{
    if( Participating() )
    {
        colShift_ = Shift(ColRank(),colAlignment_,ColStride());
        rowShift_ = Shift(RowRank(),rowAlignment_,RowStride());
    }
    else
    {
        colShift_ = 0;
        rowShift_ = 0;
    }
}

template<typename Int>
void
ADM::SetColShift()
{
    if( Participating() )
        colShift_ = Shift(ColRank(),colAlignment_,ColStride());
    else
        colShift_ = 0;
}

template<typename Int>
void
ADM::SetRowShift()
{
    if( Participating() )
        rowShift_ = Shift(RowRank(),rowAlignment_,RowStride());
    else
        rowShift_ = 0;
}

template<typename Int>
void ADM::Attach__( const Self& B, Int i, Int j, Int height, Int width, bool lock )
{
    PushCallStack("DistMatrix::Attach__");
    const Int colStride = B.ColStride();
    const Int rowStride = B.RowStride();
    Empty();
    grid_ = B.grid_;
    height_ = height;
    width_ = width;
    colAlignment_ = ( B.colAlignment_ + i ) % colStride;
    rowAlignment_ = ( B.rowAlignment_ + j ) % rowStride;
    HandleDiagPath( B );
    viewing_ = true;
    locked_ = lock;
    if( Participating() )
    {
        colShift_ = Shift( B.ColRank(), colAlignment_, colStride );
        rowShift_ = Shift( B.RowRank(), rowAlignment_, rowStride );
        const Int localI = Length( i, B.colShift_, colStride );
        const Int localJ = Length( j, B.rowShift_, rowStride );
        const Int localH = Length( height, colShift_, colStride );
        const Int localW = Length( width,  rowShift_, rowStride );
        Matrix().Attach__( B.LockedMatrix(), localI, localJ, localH, localW, lock );
    }
    else
    {
        colShift_ = 0;
        rowShift_ = 0;
    }
    PopCallStack();
}

template<typename Int>
void
ADM::CopyFrom( const ADM& A )
{
	PushCallStack( "DistMatrix::CopyFrom" );
	AssertDataTypes( *this, A );
	AssertUnlocked( *this );
	if ( Viewing() )
		AssertSameSize( *this, A );
	CopyFrom_( A );
	PopCallStack();		
}

template<typename Int>
void ADM::GetDiagonal( Self& d, Int offset ) const
{ NotImplemented( true, "GetDiagonal", NIDistMatrix, "this", this, NIDistMatrix, "d", &d, NIInt, "offset", &offset, NIDone ); }

template<typename Int>
void ADM::SetDiagonal( const Self& d, Int offset )
{ NotImplemented( true, "SetDiagonal", NIDistMatrix, "this", this, NIDistMatrix, "d", &d, NIInt, "offset", &offset, NIDone ); }

template<typename Int>
void ADM::GetRealPartOfDiagonal( Self& d, Int offset ) const
{ NotImplemented( true, "GetRealPartOfDiagonal", NIDistMatrix, "this", this, NIDistMatrix, "d", &d, NIInt, "offset", &offset, NIDone ); }

template<typename Int>
void ADM::SetRealPartOfDiagonal( const Self& d, Int offset )
{ NotImplemented( true, "SetRealPartOfDiagonal", NIDistMatrix, "this", this, NIDistMatrix, "d", &d, NIInt, "offset", &offset, NIDone ); }

template<typename Int>
void ADM::GetImagPartOfDiagonal( Self& d, Int offset ) const
{ NotImplemented( true, "GetImagPartOfDiagonal", NIDistMatrix, "this", this, NIDistMatrix, "d", &d, NIInt, "offset", &offset, NIDone ); }

template<typename Int>
void ADM::SetImagPartOfDiagonal( const Self& d, Int offset )
{ NotImplemented( true, "SetImagPartOfDiagonal", NIDistMatrix, "this", this, NIDistMatrix, "d", &d, NIInt, "offset", &offset, NIDone ); }

template<typename Int>
void ADM::SumScatterFrom( const Self& A )
{ NotImplemented( true, "SumScatterFrom", NIDistMatrix, "this", this, NIDistMatrix, "A", &A, NIDone ); }

template<typename Int>
void ADM::SumScatterUpdate( const Scalar<Int>& alpha, const Self& A )
{ NotImplemented( true, "SumScatterUpdate", NIDistMatrix, "this", this, NIScalar, "alpha", &alpha, NIDistMatrix, "A", &A, NIDone ); }

template<typename Int>
void ADM::AdjointFrom( const Self& A )
{ TransposeFrom( A, true ); }

template<typename Int>
void ADM::AdjointSumScatterFrom( const Self& A )
{ TransposeSumScatterFrom( A, true ); }

template<typename Int>
void ADM::AdjointSumScatterUpdate( const Scalar<Int>& alpha, const Self& A )
{ TransposeSumScatterUpdate( alpha, A, true ); }

template<typename Int>
void ADM::TransposeFrom( const Self& A, bool conjugate )
{ NotImplemented( true, "TransposeFrom", NIDistMatrix, "this", this, NIDistMatrix, "A", &A, NIBool, "conjugate", &conjugate, NIDone ); }

template<typename Int>
void ADM::TransposeSumScatterFrom( const Self& A, bool conjugate )
{ NotImplemented( true, "TransposeSumScatterFrom", NIDistMatrix, "this", this, NIDistMatrix, "A", &A, NIBool, "conjugate", &conjugate, NIDone ); }

template<typename Int>
void ADM::TransposeSumScatterUpdate( const Scalar<Int>& alpha, const Self& A, bool conjugate )
{ NotImplemented( true, "TransposeSumScatterUpdate", NIDistMatrix, "this", this, NIScalar, "alpha", &alpha, NIDistMatrix, "A", &A, NIBool, "conjugate", &conjugate, NIDone ); }

template<typename Int>
void ADM::SumOverCol()
{ NotImplemented( true, "SumOverCol", NIDistMatrix, "this", this, NIDone ); }

template<typename Int>
void ADM::SumOverRow()
{ NotImplemented( true, "SumOverRow",NIDistMatrix, "this", this,  NIDone ); }

template<typename Int>
void ADM::AlignWithDiagonal( const elem::DistData<Int>& data, Int offset )
{ NotImplemented( true, "AlignWithDiagonal", NIDistMatrix, "this", NIDistData, &data, NIInt, &offset, NIDone ); }

template<typename Int>
void ADM::AlignWithDiagonal( const Self& A, Int offset )
{ NotImplemented( true, "AlignWithDiagonal", NIDistMatrix, "this", NIDistMatrix, &A, NIInt, &offset, NIDone ); }

template<typename Int>
bool ADM::AlignedWithDiagonal( const elem::DistData<Int>& data, Int offset ) const
{ NotImplemented( true, "AlignedWithDiagonal", NIDistMatrix, "this", NIDistData, &data, NIInt, &offset, NIDone ); return false; }

template<typename Int>
bool ADM::AlignedWithDiagonal( const Self& A, Int offset ) const
{ NotImplemented( true, "AlignedWithDiagonal", NIDistMatrix, "this", NIDistMatrix, &A, NIInt, &offset, NIDone ); return false; }

//
// Explicit instantiation
//

template class DistData<int>;
template class AutoDistMatrix<int>;
template class AbstractDistMatrix<int,int>& AutoDistMatrix<int>::Cast<int>();
#ifndef DISABLE_FLOAT
template class AbstractDistMatrix<float,int>& AutoDistMatrix<int>::Cast<float>();
#endif
template class AbstractDistMatrix<double,int>& AutoDistMatrix<int>::Cast<double>();
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
template class AbstractDistMatrix<scomplex,int>& AutoDistMatrix<int>::Cast<scomplex>();
#endif
template class AbstractDistMatrix<dcomplex,int>& AutoDistMatrix<int>::Cast<dcomplex>();
#endif

} // namespace elem
