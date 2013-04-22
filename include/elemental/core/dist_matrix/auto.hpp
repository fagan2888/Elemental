/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_DISTMATRIX_AUTO_DECL_HPP
#define CORE_DISTMATRIX_AUTO_DECL_HPP

namespace elem {

template<typename Int>
struct DistData
{
    Distribution colDist;
    Distribution rowDist;
    Int colAlignment; 
    Int rowAlignment;
    Int diagPath; // only relevant for [MD,* ]/[* ,MD] distributions
    const Grid* grid;
    void PrintShort( std::ostream& os ) const;
};

template<typename Int> 
class AutoDistMatrix
{
public:
	typedef AutoDistMatrix<Int> Self;
	typedef AutoDistMatrix<Int> Parent;
	
	//
	// Consistency checkers
	//
	
    void AssertNotLocked() const;
    void AssertNotStoringData() const;
    void AssertFreeColAlignment() const;
    void AssertFreeRowAlignment() const;
    void AssertSameGrid( const elem::Grid& grid ) const;
    
	//
	// Factories
	//
	
	// static Self* Create( ScalarTypes stype, Distribution U, Distribution V );
	// static Self* Create( ScalarTypes stype, Distribution U, Distribution V, Int height, Int width );
	// static Self* Create( ScalarTypes stype, Distribution U, Distribution V, Int height, Int width, Int ldim );
	// template <class T,Distribution U,Distribution V> static Self* Create();
	
	// virtual Self* CloneEmpty() const = 0;
	// virtual Self* Clone() const = 0;
	
	//
	// Casting. These return references to the underlying derived type. They throw an
	// exception if the type doesn't match.
	//
	
	// template <typename T,Distribution U,Distribution V> DistMatrix<T,U,V,Int>& Cast();
	// template <typename T,Distribution U,Distribution V> const DistMatrix<T,U,V,Int>& Cast() const;
	
	//
	// Destructor
	//
	
    virtual ~AutoDistMatrix();

    //
    // Basic information
    //

    Int Height() const;
    Int Width() const;
    Int DiagonalLength( Int offset=0 ) const;
    Int LocalHeight() const;
    Int LocalWidth() const;
    Int LDim() const;
    virtual Int DiagPath() const;
    bool IsComplex() const;
    size_t DataSize() const;
    size_t AllocatedMemory() const;
    
    const elem::Grid& Grid() const;
    
    	  void* Buffer();
    	  void* Buffer( Int iLocal, Int jLocal );
    	  
    const void* LockedBuffer() const;
    const void* LockedBuffer( Int iLocal, Int jLocal ) const;

    virtual elem::AutoMatrix<Int>& Matrix() = 0;
    virtual const elem::AutoMatrix<Int>& LockedMatrix() const = 0;
    
    //
    // Casting
    //

	template <typename T> AbstractDistMatrix<T,Int>& Cast();
	template <typename T> const AbstractDistMatrix<T,Int>& Cast() const;
	template <typename T,Distribution U,Distribution V> DistMatrix<T,U,V,Int>& Cast();
	template <typename T,Distribution U,Distribution V> const DistMatrix<T,U,V,Int>& Cast() const;
	
    //
    // I/O
    //

    void Print( const std::string msg="" ) const;
    void Print( std::ostream& os, const std::string msg="" ) const;
    void Write( const std::string filename, const std::string msg="" ) const;
    void PrintShort( std::ostream& os ) const;

    //
    // Alignments
    //

    void FreeAlignments();
    bool ConstrainedColAlignment() const;
    bool ConstrainedRowAlignment() const;
    Int ColAlignment() const;
    Int RowAlignment() const;
    Int ColShift() const;
    Int RowShift() const;

    void Align( Int colAlignment, Int rowAlignment );
    void AlignCols( Int colAlignment );
    void AlignRows( Int rowAlignment );

    //
    // Viewing 
    //

    bool Viewing() const;
    bool Locked() const;

    //
    // Utilities
    //

	void CopyFrom( const Self& A );
    Self& operator=( const Self& A );
    
    void Empty();
    void EmptyData();
    void SetGrid( const elem::Grid& grid );
    virtual void ResizeTo( Int height, Int width ) = 0;

    //------------------------------------------------------------------------//
    // Routines that can be overridden in derived classes                     //
    //------------------------------------------------------------------------//

    virtual bool Participating() const;
    virtual void AlignWith( const elem::DistData<Int>& data );
    virtual void AlignWith( const Self& A );
    virtual void AlignColsWith( const elem::DistData<Int>& data );
    virtual void AlignColsWith( const Self& A );
    virtual void AlignRowsWith( const elem::DistData<Int>& data );
    virtual void AlignRowsWith( const Self& A );
    virtual void AlignWithDiagonal( const elem::DistData<Int>& data, Int offset=0 );
    virtual void AlignWithDiagonal( const Self& A, Int offset=0 );
    virtual bool AlignedWithDiagonal( const elem::DistData<Int>& data, Int offset=0 ) const;
    virtual bool AlignedWithDiagonal( const Self& A, Int offset=0 ) const;

    //------------------------------------------------------------------------//
    // Routines that MUST be implemented in non-abstract derived classes      //
    //------------------------------------------------------------------------//

    //
    // Basic information
    //

    virtual elem::DistData<Int> DistData() const = 0;
    virtual ScalarTypes DataType() const;
    virtual Distribution RowDist() const = 0;
    virtual Distribution ColDist() const = 0;
    virtual Distribution2D Dist2D() const = 0;

    // So that the local row indices are given by
    //   A.ColShift():A.ColStride():A.Height()
    virtual Int ColStride() const = 0; 
    // So that the local column indices are given by
    //   A.RowShift():A.RowStride():A.Width()
    virtual Int RowStride() const = 0;
    virtual Int ColRank() const = 0;
    virtual Int RowRank() const = 0;
    
    //
    // Local entry manipulation
    //
    
    void GetLocal( Int iLocal, Int jLocal, Scalar<Int>& alpha ) const;
    void SetLocal( Int iLocal, Int jLocal, const Scalar<Int>& alpha );
    void UpdateLocal( Int iLocal, Int jLocal, const Scalar<Int>& alpha );
    void GetLocalRealPart( Int iLocal, Int jLocal, Scalar<Int>& alpha ) const;
    void SetLocalRealPart( Int iLocal, Int jLocal, const Scalar<Int>& alpha );
    void UpdateLocalRealPart( Int iLocal, Int jLocal, const Scalar<Int>& alpha );
    void GetLocalImagPart( Int iLocal, Int jLocal, Scalar<Int>& alpha ) const;
    void SetLocalImagPart( Int iLocal, Int jLocal, const Scalar<Int>& alpha );
    void UpdateLocalImagPart( Int iLocal, Int jLocal, const Scalar<Int>& alpha );

    //
    // Global entry manipulation
    //

    virtual void Get( Int i, Int j, Scalar<Int>& alpha ) const = 0;
    virtual void Set( Int i, Int j, const Scalar<Int>& alpha ) = 0;
    virtual void Update( Int i, Int j, const Scalar<Int>& alpha ) = 0;
    virtual void GetRealPart( Int i, Int j, Scalar<Int>& alpha ) const = 0;
    virtual void SetRealPart( Int i, Int j, const Scalar<Int>& alpha ) = 0;
    virtual void UpdateRealPart( Int i, Int j, const Scalar<Int>& alpha ) = 0;
    virtual void GetImagPart( Int i, Int j, Scalar<Int>& alpha ) const = 0;
    virtual void SetImagPart( Int i, Int j, const Scalar<Int>& alpha ) = 0;
    virtual void UpdateImagPart( Int i, Int j, const Scalar<Int>& alpha ) = 0;

	//
	// Advanced routines. Many of these are implemented only for limited combinations
	// of distributions. The default implementation of each of these throws an exception
	//    
    
    virtual void GetDiagonal( Self& d, Int offset = 0 ) const;
    virtual void SetDiagonal( const Self& d, Int offset = 0 );
    virtual void GetRealPartOfDiagonal( Self& d, Int offset = 0 ) const;
    virtual void SetRealPartOfDiagonal( const Self& d, Int offset = 0 );
    virtual void GetImagPartOfDiagonal( Self& d, Int offset = 0 ) const;
    virtual void SetImagPartOfDiagonal( const Self& d, Int offset = 0 );
    virtual void SumScatterFrom( const Self& A );
    virtual void SumScatterUpdate( const Scalar<Int>& alpha, const Self& A );
    virtual void AdjointFrom( const Self& A );
    virtual void AdjointSumScatterFrom( const Self& A );
    virtual void AdjointSumScatterUpdate( const Scalar<Int>& alpha, const Self& A );
    virtual void TransposeFrom( const Self& A, bool conjugate=false );
    virtual void TransposeSumScatterFrom( const Self& A, bool conjugate=false );
    virtual void TransposeSumScatterUpdate( const Scalar<Int>& alpha, const Self& A, bool conjugate=false );
    virtual void SumOverCol();
    virtual void SumOverRow();

public: // but not.
	void Attach__( const Self& A, Int i, Int j, Int height, Int width, bool lock );
    virtual void HandleDiagPath( const Self& B );
	template <typename T> AbstractDistMatrix<T,Int>& Cast_();
	template <typename T> const AbstractDistMatrix<T,Int>& Cast_() const;
	template <typename T,Distribution U,Distribution V> DistMatrix<T,U,V,Int>& Cast_();
	template <typename T,Distribution U,Distribution V> const DistMatrix<T,U,V,Int>& Cast_() const;

protected:
    bool viewing_, locked_;
    Int height_, width_;
    bool constrainedColAlignment_, constrainedRowAlignment_;
    Int colAlignment_, rowAlignment_;
    Int colShift_, rowShift_, diagPath_;
    const elem::Grid* grid_;

    AutoDistMatrix
    ( Int height, Int width,
      bool constrainedColAlignment, bool constrainedRowAlignment,
      Int colAlignment, Int rowAlignment,
	  Int colShift, Int rowShift, 
	  bool viewing, bool locked,
      const elem::Grid& g );

    void Align_( Int colAlignment, Int rowAlignment );
    void AlignCols_( Int colAlignment );
    void AlignRows_( Int rowAlignment );
    virtual void CopyFrom_( const Self& A ) = 0;
    
protected:
    void SetShifts();
    void SetColShift();
    void SetRowShift();

    virtual void PrintBase( std::ostream& os, const std::string msg ) const = 0;

	template<typename T,typename Ord>
	friend class AbstractDistMatrix;
    template<typename S,Distribution U,Distribution V,typename Ord>
    friend class DistMatrix;
};

} // namespace elem

#endif // ifndef CORE_DISTMATRIX_AUTO_HPP
