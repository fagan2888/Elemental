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

enum MatrixTypes {
	Integral,
#ifndef DISABLE_FLOAT
	Float,
#endif
	Double,
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
	FComplex,
#endif
	DComplex,
#endif
	Unknown
};

template <typename Int>
class AutoMatrix
{
public:
	typedef AutoMatrix<Int> Self;
	typedef AutoMatrix<Int> Parent;
	
	//
	// Consistency checkers
	//
	
    void AssertBounds( Int i, Int j ) const;
    void AssertDataTypes( const Self& B, bool unknown_ok = false ) const;
    void AssertCRDataTypes( const Self& B, bool imag_bad = false ) const;
    void AssertView( Int i, Int j, Int height, Int width ) const;
    enum LockTypes { ViewLock, PartitionLock, MiscLock };
    void AssertUnlocked( LockTypes ltype = MiscLock ) const;
	void AssertContiguous2x1( const Self& B ) const;
	void AssertContiguous1x2( const Self& B ) const;
	void AssertContiguous2x2( const Self& B12, const Self& B21, const Self& B22 ) const;    
	void AssertContiguous3x3( const Self& A01, const Self& A02,
		const Self& A10, const Self& A11, const Self& A12,
		const Self& A20, const Self& A21, const Self& A22 ) const;
    
	//
	// Factories
	//

	 static Self* Create( MatrixTypes type );
	 static Self* Create( MatrixTypes type, Int height, Int width );
	 static Self* Create( MatrixTypes type, Int height, Int width, Int ldim );
	 static Self* Create( MatrixTypes type, Int height, Int width, void* buffer, Int ldim );
	 static Self* Create( MatrixTypes type, Int height, Int width, const void* buffer, Int ldim );
	virtual Self* CloneEmpty() const; // = 0;
	virtual Self* Clone() const; // = 0;
	
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
    Int DataSize() const;
    Int MemorySize() const;
    virtual MatrixTypes DataType() const;
    bool Viewing() const;
    bool Locked() const;

    void* Buffer();
    void* Buffer( Int i, Int j );

    const void* LockedBuffer() const;
    const void* LockedBuffer( Int i, Int j ) const;
    
    //
    // I/O
    //
    
    virtual void Print( std::ostream& os, std::string msg="" ) const;
    void Print( std::string msg="" );
    
    //
    // Entry manipulation
    //

	void Get( MatrixTypes type, Int i, Int j, void* dst ) const;
	void Set( MatrixTypes type, Int i, Int j, const void* src );
	void Update( MatrixTypes type, Int i, Int j, const void* src );
    void GetRealPart( MatrixTypes type, Int i, Int j, void* dst ) const;
    void GetImagPart( MatrixTypes type, Int i, Int j, void* st ) const;
    void SetRealPart( MatrixTypes type, Int i, Int j, const void* src );
    void SetImagPart( MatrixTypes type, Int i, Int j, const void* src );
    void UpdateRealPart( MatrixTypes type, Int i, Int j, const void* src );
    void UpdateImagPart( MatrixTypes type, Int i, Int j, const void* src );
    
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

	virtual void CopyFrom( const Self& A );
    Self& operator=( const Self& A );

	void Attach( MatrixTypes dtype, Int height, Int width, void* buffer, Int ldim );
	void LockedAttach( MatrixTypes dtype, Int height, Int width, const void* buffer, Int ldim );
	
    void Empty();
    void ResizeTo( Int height, Int width );
    void ResizeTo( Int height, Int width, Int ldim );
    
protected:
	AutoMatrix( size_t dsize );
	AutoMatrix( size_t dsize, Int height, Int width );
	AutoMatrix( size_t dsize, Int height, Int width, Int ldim );
	AutoMatrix( size_t dsize, Int height, Int width, void* data, Int ldim );
	AutoMatrix( size_t dsize, Int height, Int width, const void* data, Int ldim );
	
	void AssignHelper( Self& A );
	void ResizeHelper( Int height, Int width, Int ldim );
	
    //
    // These virtual functions do no consistency checking, but provide type-specific
    // implementations for AutoMatrix<Int> to call. AutoMatrix handles all of the
    // consistency checking before these are called. Of course, Matrix<T,Int> uses
    // these itself as well, for consistency.
    //
    
	virtual void Get_( Int i, Int j, void* dst ) const; // = 0;
	virtual void Set_( Int i, Int j, const void* src ); // = 0;
	virtual void Update_( Int i, Int j, const void* src ); // = 0;
    virtual void GetRealPart_( Int i, Int j, void* dst ) const; // = 0;
    virtual void SetRealPart_( Int i, Int j, const void* src ); // = 0;
    virtual void UpdateRealPart_( Int i, Int j, const void* src ); // = 0;
    virtual void GetImagPart_( Int i, Int j, void* st ) const; // = 0;
    virtual void SetImagPart_( Int i, Int j, const void* src ); // = 0;
    virtual void UpdateImagPart_( Int i, Int j, const void* src ); // = 0;
    
    virtual void GetDiagonal_( Self& d, Int offset ) const; // = 0;
    virtual void SetDiagonal_( const Self& d, Int offset ); // = 0;
    virtual void UpdateDiagonal_( const Self& d, Int offset ); // = 0;
    virtual void GetRealPartOfDiagonal_( Self& d, Int offset ) const; // = 0;
    virtual void SetRealPartOfDiagonal_( const Self& d, Int offset ); // = 0;
    virtual void UpdateRealPartOfDiagonal_( const Self& d, Int offset ); // = 0;
    virtual void GetImagPartOfDiagonal_( Self& d, Int offset ) const; // = 0;
    virtual void SetImagPartOfDiagonal_( const Self& d, Int offset ); // = 0;
    virtual void UpdateImagPartOfDiagonal_( const Self& d, Int offset ); // = 0;

    //
    // The protected, underscored version of these methods bypass all consistency 
    // checking. Thus they are good to use with the friend functions when RELEASE is
    // defined. When RELEASE is not defined, use the public, non-underscored versions.
    //
    
	virtual void CopyFrom_( const Self& A );
	
	void* Buffer_();
    void* Buffer_( Int i, Int j );
    const void* LockedBuffer_() const;
    const void* LockedBuffer_( Int i, Int j ) const;
    
    void ResizeTo_( Int height, Int width );
    void ResizeTo_( Int height, Int width, Int ldim );
    
	void Attach_( MatrixTypes dtype, Int height, Int width, const void* buffer, Int ldim, bool lock );
    void Attach( MatrixTypes dtype, Int height, Int width, const void* buffer, Int ldim, bool lock );
    
    //
    // By making this one function public that really should not be, we avoid having to
    // declare every view and partition function to be a friend. Fair trade, I think.
    // So please don't use this function.
    //

public: // but not.
    void Attach__( const Self& A, Int i, Int j, Int height, Int width, bool lock );
	
private:
    Int height_, width_, ldim_;
    void* data_; bool locked_;
    AutoMemory<Int> memory_;
	void Attach_( Int height, Int width, const void* buffer, Int ldim, bool lock, Int i, Int j );
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
	
    //
    // Constructors, Factories
    // 

    Matrix(); 
    Matrix( Int height, Int width );
    Matrix( Int height, Int width, Int ldim );
    Matrix( Int height, Int width, T* buffer, Int ldim );
    Matrix( Int height, Int width, const T* buffer, Int ldim );
    Matrix( const Self& A );
    Parent* CloneEmpty() const;
	Parent* Clone() const;

    //
    // Basic information
    //

    MatrixTypes DataType() const;

    T* Buffer();
    T* Buffer( Int i, Int j );
    const T* LockedBuffer() const;
    const T* LockedBuffer( Int i, Int j ) const;

    //
    // I/O
    //

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
    
    void GetRealPartOfDiagonal( Self& d, Int offset = 0 ) const;
    void SetRealPartOfDiagonal( Self& d, Int offset = 0 );
    void UpdateRealPartOfDiagonal( Self& d, Int offset = 0 );

    // Only valid for complex data
    void GetImagPartOfDiagonal( Self& d, Int offset = 0 ) const;
    void SetImagPartOfDiagonal( Self& d, Int offset = 0 );
    void UpdateImagPartOfDiagonal( Self& d, Int offset = 0 );

    //
    // Viewing other matrix instances (or buffers)
    //

    void Attach( Int height, Int width, T* buffer, Int ldim );
    void LockedAttach( Int height, Int width, const T* buffer, Int ldim );

    //
    // Utilities
    //

    Self& operator=( Self& A );
    
    // Not sure why I can't protect these. Don't use them.
    
	T Get_( Int i, Int j ) const;
	void Set_( Int i, Int j, T a );
	void Update_( Int i, Int j, T a );
    RT GetRealPart_( Int i, Int j ) const;
	void SetRealPart_( Int i, Int j, RT a );
	void UpdateRealPart_( Int i, Int j, RT a );
	
protected:

	friend class AutoMatrix<Int>;
	
    // Only valid for complex data
	RT GetImagPart_( Int i, Int j ) const;
	void SetImagPart_( Int i, Int j, RT a );
	void UpdateImagPart_( Int i, Int j, RT a );
	
	void Get_( Int i, Int j, void* dst ) const;
	void Set_( Int i, Int j, const void* src );
	void Update_( Int i, Int j, const void* src );
    void GetRealPart_( Int i, Int j, void* dst ) const;
    void SetRealPart_( Int i, Int j, const void* src );
    void UpdateRealPart_( Int i, Int j, const void* src );

    // Only valid for complex data
    void GetImagPart_( Int i, Int j, void* dst ) const;
    void SetImagPart_( Int i, Int j, const void* src );
    void UpdateImagPart_( Int i, Int j, const void* src );
    
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
