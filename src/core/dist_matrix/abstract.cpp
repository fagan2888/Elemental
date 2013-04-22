/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "elemental-lite.hpp"

#ifdef RELEASE
#define PARENT(x) Parent :: x ## _
#else
#define PARENT(x) Parent :: x
#endif

namespace elem {

template <typename T,typename Int>
template <Distribution U,Distribution V>
DistMatrix<T,U,V,Int>& AbstractDistMatrix<T,Int>::Cast()
{
	AssertDistributions( RowDist(), ColDist(), U, V );
	return *reinterpret_cast<elem::DistMatrix<T,U,V,Int>*>(this);
}

template <typename T,typename Int>
template <Distribution U,Distribution V>
const DistMatrix<T,U,V,Int>& AbstractDistMatrix<T,Int>::Cast() const
{
	AssertDistributions( RowDist(), ColDist(), U, V );
	return *reinterpret_cast<const elem::DistMatrix<T,U,V,Int>*>(this);
}

template<typename T,typename Int>
AbstractDistMatrix<T,Int>::AbstractDistMatrix
( Int height, Int width, 
  bool constrainedColAlignment, bool constrainedRowAlignment,
  Int colAlignment, Int rowAlignment,
  Int colShift, Int rowShift, 
  Int localHeight, Int localWidth,
  const elem::Grid& grid )
: AutoDistMatrix<Int>(
	height, width, constrainedColAlignment, constrainedRowAlignment,
	colAlignment, rowAlignment, colShift, rowShift, false, false, grid ),
  auxMemory_(), 
  matrix_(localHeight,localWidth)
{ } 

template<typename T,typename Int>
AbstractDistMatrix<T,Int>::AbstractDistMatrix
( Int height, Int width, 
  bool constrainedColAlignment, bool constrainedRowAlignment,
  Int colAlignment, Int rowAlignment,
  Int colShift, Int rowShift, 
  Int localHeight, Int localWidth, Int ldim,
  const elem::Grid& grid )
: AutoDistMatrix<Int>(
	height, width, constrainedColAlignment, constrainedRowAlignment,
	colAlignment, rowAlignment, colShift, rowShift, false, false, grid ),
  auxMemory_(), 
  matrix_(localHeight,localWidth,ldim)
{ } 

template<typename T,typename Int>
AbstractDistMatrix<T,Int>::AbstractDistMatrix
( Int height, Int width, 
  Int colAlignment, Int rowAlignment,
  Int colShift, Int rowShift, 
  Int localHeight, Int localWidth, const T* buffer, Int ldim,
  const elem::Grid& grid )
: AutoDistMatrix<Int>(
	height, width, true, true,
	colAlignment, rowAlignment, colShift, rowShift, true, true, grid ),
  auxMemory_(), 
  matrix_(localHeight,localWidth,buffer,ldim)
{ } 

template<typename T,typename Int>
AbstractDistMatrix<T,Int>::AbstractDistMatrix
( Int height, Int width, 
  Int colAlignment, Int rowAlignment,
  Int colShift, Int rowShift, 
  Int localHeight, Int localWidth, T* buffer, Int ldim,
  const elem::Grid& grid )
: AutoDistMatrix<Int>(
	height, width, true, true,
	colAlignment, rowAlignment, colShift, rowShift, true, true, grid ),
  auxMemory_(), 
  matrix_(localHeight,localWidth,buffer,ldim)
{ } 

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::Get( Int iLocal, Int jLocal, Scalar<Int>& ans ) const
{ ans = Get( iLocal, jLocal ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::Set( Int iLocal, Int jLocal, const Scalar<Int>& ans )
{ Set( iLocal, jLocal, ans.template Cast<T>() ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::Update( Int iLocal, Int jLocal, const Scalar<Int>& ans )
{ Update( iLocal, jLocal, ans.template Cast<T>() ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::GetRealPart( Int iLocal, Int jLocal, Scalar<Int>& ans ) const
{ ans = GetRealPart( iLocal, jLocal ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::SetRealPart( Int iLocal, Int jLocal, const Scalar<Int>& ans )
{ SetRealPart( iLocal, jLocal, ans.template Cast<typename Base<T>::type>() ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::UpdateRealPart( Int iLocal, Int jLocal, const Scalar<Int>& ans )
{ UpdateRealPart( iLocal, jLocal, ans.template Cast<typename Base<T>::type>() ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::GetImagPart( Int iLocal, Int jLocal, Scalar<Int>& ans ) const
{ ans = GetImagPart( iLocal, jLocal ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::SetImagPart( Int iLocal, Int jLocal, const Scalar<Int>& ans )
{ SetImagPart( iLocal, jLocal, ans.template Cast<typename Base<T>::type>() ); }

template <typename T,typename Int>
void AbstractDistMatrix<T,Int>::UpdateImagPart( Int iLocal, Int jLocal, const Scalar<Int>& ans )
{ UpdateImagPart( iLocal, jLocal, ans.template Cast<typename Base<T>::type>() ); }

template<typename T,typename Int>
void
AbstractDistMatrix<T,Int>::CopyFrom_( const Parent& AA )
{
	const Self& A = AA.template Cast_<T>();
	switch ( A.Dist2D() ) {
	case MC_MR:		operator=( A.template Cast_<MC,MR>()     ); break;
	case MC_STAR:	operator=( A.template Cast_<MC,STAR>()   ); break;
	case MD_STAR:	operator=( A.template Cast_<MD,STAR>()   ); break;
	case MR_MC:		operator=( A.template Cast_<MR,MC>()     ); break;
	case MR_STAR:	operator=( A.template Cast_<MR,STAR>()   ); break;
	case STAR_MC:	operator=( A.template Cast_<STAR,MC>()   ); break;
	case STAR_MD:	operator=( A.template Cast_<STAR,MD>()   ); break;
	case STAR_MR:	operator=( A.template Cast_<STAR,MR>()   ); break;
	case STAR_STAR:	operator=( A.template Cast_<STAR,STAR>() ); break;
	case STAR_VC:	operator=( A.template Cast_<STAR,VC>()   ); break;
	case STAR_VR:	operator=( A.template Cast_<STAR,VR>()   ); break;
	case VC_STAR:	operator=( A.template Cast_<VC,STAR>()   ); break;
	case VR_STAR:	operator=( A.template Cast_<VR,STAR>()   ); break;
	}
}

#undef TEMPLATE_INST
#define TEMPLATE_INST(T,Int) \
	template class AbstractDistMatrix<T,Int>; \
	template       DistMatrix<T,MC,MR,Int>&     AbstractDistMatrix<T,Int>::Cast<MC,MR>(); \
	template       DistMatrix<T,MC,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<MC,STAR>(); \
	template       DistMatrix<T,MD,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<MD,STAR>(); \
	template       DistMatrix<T,MR,MC,Int>&     AbstractDistMatrix<T,Int>::Cast<MR,MC>(); \
	template       DistMatrix<T,MR,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<MR,STAR>(); \
	template       DistMatrix<T,STAR,MC,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,MC>(); \
	template       DistMatrix<T,STAR,MD,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,MD>(); \
	template       DistMatrix<T,STAR,MR,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,MR>(); \
	template       DistMatrix<T,STAR,STAR,Int>& AbstractDistMatrix<T,Int>::Cast<STAR,STAR>(); \
	template       DistMatrix<T,STAR,VC,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,VC>(); \
	template       DistMatrix<T,STAR,VR,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,VR>(); \
	template       DistMatrix<T,VC,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<VC,STAR>(); \
	template       DistMatrix<T,VR,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<VR,STAR>(); \
	template const DistMatrix<T,MC,MR,Int>&     AbstractDistMatrix<T,Int>::Cast<MC,MR>() const; \
	template const DistMatrix<T,MC,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<MC,STAR>() const; \
	template const DistMatrix<T,MD,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<MD,STAR>() const; \
	template const DistMatrix<T,MR,MC,Int>&     AbstractDistMatrix<T,Int>::Cast<MR,MC>() const; \
	template const DistMatrix<T,MR,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<MR,STAR>() const; \
	template const DistMatrix<T,STAR,MC,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,MC>() const; \
	template const DistMatrix<T,STAR,MD,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,MD>() const; \
	template const DistMatrix<T,STAR,MR,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,MR>() const; \
	template const DistMatrix<T,STAR,STAR,Int>& AbstractDistMatrix<T,Int>::Cast<STAR,STAR>() const; \
	template const DistMatrix<T,STAR,VC,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,VC>() const; \
	template const DistMatrix<T,STAR,VR,Int>&   AbstractDistMatrix<T,Int>::Cast<STAR,VR>() const; \
	template const DistMatrix<T,VC,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<VC,STAR>() const; \
	template const DistMatrix<T,VR,STAR,Int>&   AbstractDistMatrix<T,Int>::Cast<VR,STAR>() const;

TEMPLATE_INST(int,int)
#ifndef DISABLE_FLOAT
TEMPLATE_INST(float,int)
#endif // ifndef DISABLE_FLOAT
TEMPLATE_INST(double,int)
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
TEMPLATE_INST(scomplex,int)
#endif // ifndef DISABLE_FLOAT
TEMPLATE_INST(dcomplex,int)
#endif // ifndef DISABLE_COMPLEX

} // namespace elem
