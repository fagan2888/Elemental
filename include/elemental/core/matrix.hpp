/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_MATRIX_HPP
#define CORE_MATRIX_HPP

namespace elem {

template <typename Int>
class AutoMatrix
{
public:
	typedef AutoMatrix<Int> Self;
	typedef AutoMatrix<Int> Parent;
	typedef elem::Scalar<Int> Scalar;
    
	//
	// Factories
	//
	
	static Self* Create( ScalarTypes stype );
	static Self* Create( ScalarTypes stype, Int height, Int width );
	static Self* Create( ScalarTypes stype, Int height, Int width, Int ldim );
	template <class T> static Self* Create();
	static Self* Create( Int height, Int width, const elem::Buffer<Int>& buffer, Int ldim );
	static Self* Create( Int height, Int width, const elem::ConstBuffer<Int>& buffer, Int ldim );
	virtual Self* CloneEmpty() const = 0;
	virtual Self* Clone() const = 0;
	
	//
	// Casting. These return references to the underlying derived type. They throw an
	// exception if the type doesn't match.
	//
	
	template <typename T> Matrix<T,Int>& Cast();
	template <typename T> const Matrix<T,Int>& Cast() const;
	
    //
    // Destructor
    //

    virtual ~AutoMatrix();

    //
    // Basic information
    //

    Int Height() const;
    Int Width() const;
    Int DiagonalLength( Int offset=0 ) const;
    Int LDim() const;
    bool IsComplex() const;
    size_t DataSize() const;
    size_t MemorySize() const;
    virtual ScalarTypes DataType() const = 0;
    bool Viewing() const;
    bool Locked() const;
    
    void* Buffer();
    void* Buffer( Int i, Int j );

    const void* LockedBuffer() const;
    const void* LockedBuffer( Int i, Int j ) const;
    
    // No bounds checking
    
    const void* LockedBuffer_() const;
    const void* LockedBuffer_( Int i, Int j ) const;
    
    //
    // I/O
    //
    
    virtual void Print( std::ostream& os, std::string msg="" ) const;
    void Print( std::string msg="" ) const;
    void PrintShort( std::ostream& os ) const;
    
    //
    // Entry manipulation
    //
    
	Scalar Get( Int i, Int j ) const;
	void Set( Int i, Int j, const Scalar& s );
	void Update( Int i, Int j, const Scalar& s );
	
    Scalar GetRealPart( Int i, Int j ) const;
    void SetRealPart( Int i, Int j, const Scalar& s );
    void UpdateRealPart( Int i, Int j, const Scalar& s );
    
    Scalar GetImagPart( Int i, Int j ) const;
    void SetImagPart( Int i, Int j, const Scalar& s );
    void UpdateImagPart( Int i, Int j, const Scalar& s );
    
    void GetDiagonal( Self& d, Int offset = 0 ) const;
    void SetDiagonal( const Self& d, Int offset = 0 );
    void UpdateDiagonal( const Self& d, Int offset = 0 );
    
    void GetRealPartOfDiagonal( Self& d, Int offset = 0 ) const;
    void SetRealPartOfDiagonal( const Self& d, Int offset = 0 );
    void UpdateRealPartOfDiagonal( const Self& d, Int offset = 0 );
    
    void GetImagPartOfDiagonal( Self& d, Int offset = 0 ) const;
    void SetImagPartOfDiagonal( const Self& d, Int offset = 0 );
    void UpdateImagPartOfDiagonal( const Self& d, Int offset = 0 );

    //
    // Utilities
    //

	void CopyFrom( const Self& A );
    Self& operator=( const Self& A );

    void ResizeTo( Int height, Int width );
    void ResizeTo( Int height, Int width, Int ldim );
	void Attach( ScalarTypes dtype, Int height, Int width, void* buffer, Int ldim );
	void LockedAttach( ScalarTypes dtype, Int height, Int width, const void* buffer, Int ldim );
	void Empty();
    
protected:
	AutoMatrix( size_t dsize );
	AutoMatrix( size_t dsize, Int height, Int width, Int ldim );
	AutoMatrix( size_t dsize, Int height, Int width, void* data, Int ldim );
	AutoMatrix( size_t dsize, Int height, Int width, const void* data, Int ldim );
	AutoMatrix( const Self& A );
	
    //
    // These virtual functions do no consistency checking, but provide type-specific
    // implementations for AutoMatrix<Int> to call. AutoMatrix handles all of the
    // consistency checking before these are called. Of course, Matrix<T,Int> uses
    // these itself as well, for consistency.
    //
    
	virtual Scalar Get_S( Int i, Int j ) const = 0;
	virtual void Set_S( Int i, Int j, const Scalar& v ) = 0;
	virtual void Update_S( Int i, Int j, const Scalar& v ) = 0;
    virtual Scalar GetRealPart_S( Int i, Int j ) const = 0;
    virtual void SetRealPart_S( Int i, Int j, const Scalar& v ) = 0;
    virtual void UpdateRealPart_S( Int i, Int j, const Scalar& v ) = 0;
    virtual Scalar GetImagPart_S( Int i, Int j ) const = 0;
    virtual void SetImagPart_S( Int i, Int j, const Scalar& v ) = 0;
    virtual void UpdateImagPart_S( Int i, Int j, const Scalar& v ) = 0;
    
    virtual void GetDiagonal_( Self& d, Int offset ) const = 0;
    virtual void SetDiagonal_( const Self& d, Int offset ) = 0;
    virtual void UpdateDiagonal_( const Self& d, Int offset ) = 0;
    virtual void GetRealPartOfDiagonal_( Self& d, Int offset ) const = 0;
    virtual void SetRealPartOfDiagonal_( const Self& d, Int offset ) = 0;
    virtual void UpdateRealPartOfDiagonal_( const Self& d, Int offset ) = 0;
    virtual void GetImagPartOfDiagonal_( Self& d, Int offset ) const = 0;
    virtual void SetImagPartOfDiagonal_( const Self& d, Int offset ) = 0;
    virtual void UpdateImagPartOfDiagonal_( const Self& d, Int offset ) = 0;

    //
    // The protected, underscored version of these methods bypass all consistency 
    // checking. Thus they are good to use with the friend functions when RELEASE is
    // defined. When RELEASE is not defined, use the public, non-underscored versions.
    //
    
	void CopyFrom_( const Self& A );
	
	void* Buffer_();
    void* Buffer_( Int i, Int j );
    
    void ResizeTo_( Int height, Int width );
    void ResizeTo_( Int height, Int width, Int ldim );
    
	void Attach_( ScalarTypes dtype, Int height, Int width, const void* buffer, Int ldim, bool lock );
    void Attach( ScalarTypes dtype, Int height, Int width, const void* buffer, Int ldim, bool lock );
    
    //
    // By making these functions public that really should not be, we avoid having to
    // declare a lot of friend functions here. Fair trade, I think. So please don't
    // use these functions in your own code.
    //

public: // But not. Don't use these.
    void Attach__( const Self& A, Int i, Int j, Int height, Int width, bool lock );
	template <typename T> Matrix<T,Int>& Cast_();
	template <typename T> const Matrix<T,Int>& Cast_() const;
	
private:
    Int height_, width_, ldim_;
    bool viewing_, locked_;
	size_t numel_, dsize_; byte* data_; 
	void Attach_( Int height, Int width, const void* buffer, Int ldim, bool lock );
};

// Matrix base for arbitrary rings
template<typename T,typename Int>
class Matrix : public AutoMatrix<Int>
{
public:
	typedef typename Base<T>::type RT;
	typedef Matrix<T,Int> Self;
	typedef Matrix<RT,Int> RSelf;
	typedef AutoMatrix<Int> Parent;
	typedef elem::Scalar<Int> Scalar;
	
    //
    // Constructors
    // 

    Matrix(); 
    Matrix( Int height, Int width );
    Matrix( Int height, Int width, Int ldim );
    Matrix( Int height, Int width, T* buffer, Int ldim );
    Matrix( Int height, Int width, const T* buffer, Int ldim );
    Matrix( const Self& A );
    
    //
    // Factories
    //
    
    Self* CloneEmpty() const;
	Self* Clone() const;

    //
    // Basic information
    //

    ScalarTypes DataType() const;

    T* Buffer();
    T* Buffer( Int i, Int j );
    
    const T* LockedBuffer() const;
    const T* LockedBuffer( Int i, Int j ) const;

    //
    // I/O
    //

    void Print( std::string msg="" ) const;
    void Print( std::ostream& os, std::string msg="" ) const;

    //
    // Entry manipulation
    //

    T Get( Int i, Int j ) const;
    void Set( Int i, Int j, T a );
    void Update( Int i, Int j, T a );

    RT GetRealPart( Int i, Int j ) const;
    void SetRealPart( Int i, Int j, RT a );
    void UpdateRealPart( Int i, Int j, RT a );

    // Only valid for complex data
    RT GetImagPart( Int i, Int j ) const;
    void SetImagPart( Int i, Int j, RT a );
    void UpdateImagPart( Int i, Int j, RT a );
    
    //
    // Diagonal manipulation
    //

    void GetDiagonal( Self& d, Int offset=0 ) const;
    void SetDiagonal( Self& d, Int offset=0 );
    void UpdateDiagonal( Self& d, Int offset=0 );
    
    void GetRealPartOfDiagonal( RSelf& d, Int offset = 0 ) const;
    void SetRealPartOfDiagonal( RSelf& d, Int offset = 0 );
    void UpdateRealPartOfDiagonal( RSelf& d, Int offset = 0 );

    // Only valid for complex data
    void GetImagPartOfDiagonal( RSelf& d, Int offset = 0 ) const;
    void SetImagPartOfDiagonal( RSelf& d, Int offset = 0 );
    void UpdateImagPartOfDiagonal( RSelf& d, Int offset = 0 );

    //
    // Utilities
    //

	void CopyFrom( const Self& A );
    Self& operator=( const Self& A );
    void ResizeTo( Int height, Int width );
    void ResizeTo( Int height, Int width, Int ldim );
    void Attach( Int height, Int width, T* buffer, Int ldim );
    void LockedAttach( Int height, Int width, const T* buffer, Int ldim );
	
private:
	friend class Matrix<Complex<T>,Int>;
	friend class AutoMatrix<Int>;
	
    T* Buffer_();
    T* Buffer_( Int i, Int j );
    
    const T* LockedBuffer_() const;
    const T* LockedBuffer_( Int i, Int j ) const;
    
	Scalar Get_S( Int i, Int j ) const;
	void Set_S( Int i, Int j, const Scalar& v );
	void Update_S( Int i, Int j, const Scalar& v );
    Scalar GetRealPart_S( Int i, Int j ) const;
    void SetRealPart_S( Int i, Int j, const Scalar& v );
    void UpdateRealPart_S( Int i, Int j, const Scalar& v );
    Scalar GetImagPart_S( Int i, Int j ) const;
    void SetImagPart_S( Int i, Int j, const Scalar& v );
    void UpdateImagPart_S( Int i, Int j, const Scalar& v );
	
	T Get_( Int i, Int j ) const;
	void Set_( Int i, Int j, T a );
	void Update_( Int i, Int j, T a );
	
    RT GetRealPart_( Int i, Int j ) const;
	void SetRealPart_( Int i, Int j, RT a );
	void UpdateRealPart_( Int i, Int j, RT a );
	
    // Only valid for complex data
	RT GetImagPart_( Int i, Int j ) const;
	void SetImagPart_( Int i, Int j, RT a );
	void UpdateImagPart_( Int i, Int j, RT a );

    void GetDiagonal_( Parent& d, Int offset ) const;
    void SetDiagonal_( const Parent& d, Int offset );
    void UpdateDiagonal_( const Parent& d, Int offset );
    
    void GetRealPartOfDiagonal_( Parent& d, Int offset ) const;
    void SetRealPartOfDiagonal_( const Parent& d, Int offset );
    void UpdateRealPartOfDiagonal_( const Parent& d, Int offset );

    // Only valid for complex data
    void GetImagPartOfDiagonal_( Parent& d, Int offset ) const;
    void SetImagPartOfDiagonal_( const Parent& d, Int offset );
    void UpdateImagPartOfDiagonal_( const Parent& d, Int offset );
};

} // namespace elem

#endif // ifndef CORE_MATRIX_HPP
