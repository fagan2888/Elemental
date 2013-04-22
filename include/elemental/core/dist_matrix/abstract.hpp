/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_DISTMATRIX_ABSTRACT_DECL_HPP
#define CORE_DISTMATRIX_ABSTRACT_DECL_HPP

namespace elem {

template<typename T,typename Int> 
class AbstractDistMatrix : public AutoDistMatrix<Int>
{
public:
	typedef typename Base<T>::type RT;
	typedef AbstractDistMatrix<T,Int> Self;
	typedef AutoDistMatrix<Int> Parent;
	typedef AutoDistMatrix<Int> Auto;
	
	ScalarTypes DataType() const;
    virtual Distribution RowDist() const = 0;
    virtual Distribution ColDist() const = 0;
    virtual Distribution2D Dist2D() const = 0;

          T* Buffer();
          T* Buffer( Int iLocal, Int jLocal );
          
    const T* LockedBuffer( Int iLocal, Int jLocal ) const;
    const T* LockedBuffer() const;

          elem::Matrix<T,Int>& Matrix();
    const elem::Matrix<T,Int>& LockedMatrix() const;

	template <Distribution U,Distribution V> DistMatrix<T,U,V,Int>& Cast();
	template <Distribution U,Distribution V> const DistMatrix<T,U,V,Int>& Cast() const;
	
    void Align( Int colAlignment, Int rowAlignment );
    void AlignCols( Int colAlignment );
    void AlignRows( Int rowAlignment );

    //
    // Entry manipulation: auto class implementation
    //

    void Get( Int i, Int j, Scalar<Int>& alpha ) const;
    void Set( Int i, Int j, const Scalar<Int>& alpha );
    void Update( Int i, Int j, const Scalar<Int>& alpha );

    void GetRealPart( Int i, Int j, Scalar<Int>& alpha ) const;
    void SetRealPart( Int i, Int j, const Scalar<Int>& alpha );
    void UpdateRealPart( Int i, Int j, const Scalar<Int>& alpha );

    void GetImagPart( Int i, Int j, Scalar<Int>& alpha ) const;
    void SetImagPart( Int i, Int j, const Scalar<Int>& alpha );
    void UpdateImagPart( Int i, Int j, const Scalar<Int>& alpha );

    //
    // Global entry manipulation
    //

    virtual T Get( Int i, Int j ) const = 0;
    virtual void Set( Int i, Int j, T alpha ) = 0;
    virtual void Update( Int i, Int j, T alpha ) = 0;

    virtual RT GetRealPart( Int i, Int j ) const = 0;
    virtual void SetRealPart( Int i, Int j, typename Base<T>::type u ) = 0;
    virtual void UpdateRealPart( Int i, Int j, typename Base<T>::type u ) = 0;

    // Only valid for complex data
    virtual RT GetImagPart( Int i, Int j ) const = 0;
    virtual void SetImagPart( Int i, Int j, typename Base<T>::type u ) = 0;
    virtual void UpdateImagPart( Int i, Int j, typename Base<T>::type u ) = 0;

    //
    // Local entry manipulation
    //
    
    T GetLocal( Int iLocal, Int jLocal ) const;
    void SetLocal( Int iLocal, Int jLocal, T alpha );
    void UpdateLocal( Int iLocal, Int jLocal, T alpha );

    typename Base<T>::type GetLocalRealPart( Int iLocal, Int jLocal ) const;
    void SetLocalRealPart( Int iLocal, Int jLocal, typename Base<T>::type  alpha );
    void UpdateLocalRealPart( Int iLocal, Int jLocal, typename Base<T>::type  alpha );

    typename Base<T>::type GetLocalImagPart( Int iLocal, Int jLocal ) const;
    void SetLocalImagPart( Int iLocal, Int jLocal, typename Base<T>::type  alpha );
    void UpdateLocalImagPart( Int iLocal, Int jLocal, typename Base<T>::type alpha );
    
    //
    // Copying
    //

	Self& operator=( const Self& );    
	Self& operator=( const Parent& );
	void CopyFrom( const Self& );
	void CopyFrom( const Parent& );
	
public:
	// Not for public use.
	template <Distribution U,Distribution V> DistMatrix<T,U,V,Int>& Cast_();
	template <Distribution U,Distribution V> const DistMatrix<T,U,V,Int>& Cast_() const;
    
protected:
    Memory<T> auxMemory_;
    elem::Matrix<T,Int> matrix_;
    
    friend class AutoDistMatrix<Int>;
    void CopyFrom_( const Auto& A ); // virtual implementation
    
    virtual const Self& operator=( const DistMatrix<T,MC,MR,Int>&     A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,MC,STAR,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,MD,STAR,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,MR,MC,Int>&     A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,MR,STAR,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,STAR,MC,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,STAR,MD,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,STAR,MR,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,STAR,STAR,Int>& A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,STAR,VC,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,STAR,VR,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,VC,STAR,Int>&   A ) = 0;
    virtual const Self& operator=( const DistMatrix<T,VR,STAR,Int>&   A ) = 0;
    
    // Initialize with particular local dimensions
    AbstractDistMatrix
    ( Int height, Int width,
      bool constrainedColAlignment, bool constrainedRowAlignment,
      Int colAlignment, Int rowAlignment,
      Int colShift, Int rowShift,
      Int localHeight, Int localWidth,
      const elem::Grid& g );

    // Initialize with particular local dimensions and local leading dimensions
    AbstractDistMatrix
    ( Int height, Int width,
      bool constrainedColAlignment, bool constrainedRowAlignment,
      Int colAlignment, Int rowAlignment,
      Int colShift, Int rowShift,
      Int localHeight, Int localWidth,
      Int ldim,
      const elem::Grid& g );

    // View a constant distributed matrix's buffer
    AbstractDistMatrix
    ( Int height, Int width,
      Int colAlignment, Int rowAlignment,
      Int colShift, Int rowShift,
      Int localHeight, Int localWidth,
      const T* buffer,
      Int ldim,
      const elem::Grid& g );

    // View a mutable distributed matrix's buffer
    AbstractDistMatrix
    ( Int height, Int width,
      Int colAlignment, Int rowAlignment,
      Int colShift, Int rowShift,
      Int localHeight, Int localWidth,
      T* buffer,
      Int ldim,
      const elem::Grid& g );
};

template <typename T,Distribution U,Distribution V,typename Int>
class DistMatrix : public AbstractDistMatrix<T,Int>
{
public:
	typedef DistMatrix<T,MC,MR,Int> Self;
	typedef DistMatrix<typename Base<T>::type,MC,MR,Int> RSelf;
	typedef AbstractDistMatrix<T,Int> Parent;
	typedef AutoDistMatrix<Int> Auto;
	Distribution RowDist() const { return U; }
	Distribution ColDist() const { return V; }
    Self* CloneEmpty() const;
	Self* Clone() const;    
};

} // namespace elem

#endif // ifndef CORE_DISTMATRIX_ABSTRACT_DECL_HPP
