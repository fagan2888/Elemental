/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_VIEW_DECL_HPP
#define CORE_VIEW_DECL_HPP

namespace elem {

// To make our life easier. Undef'd at the bottom of the header
#define M Matrix<T,Int>
#define AM AutoMatrix<Int>
#define DM DistMatrix<T,U,V,Int>

//
// Viewing a full matrix
//

template<typename Int>
void View
( AM& A, AM& B );
template<typename T,typename Int>
void View
( M& A, M& B );
template<typename T,Distribution U,Distribution V,typename Int>
void View
( DM& A, DM& B );

template<typename Int>
void LockedView
( AM& A, const AM& B );
template<typename T,typename Int>
void LockedView
( M& A, const M& B );
template<typename T,Distribution U,Distribution V,typename Int>
void LockedView
( DM& A, const DM& B );

//
// Viewing a submatrix
//

template<typename Int>
void View
( AM& A, AM& B,
  Int i, Int j, Int height, Int width );
template<typename T,typename Int>
void View
( M& A, M& B,
  Int i, Int j, Int height, Int width );
template<typename T,Distribution U,Distribution V,typename Int>
void View
( DM& A, DM& B,
  Int i, Int j, Int height, Int width );

template<typename Int>
void LockedView
( AM& A, const AM& B,
  Int i, Int j, Int height, Int width );
template<typename T,typename Int>
void LockedView
( M& A, const M& B,
  Int i, Int j, Int height, Int width );
template<typename T,Distribution U,Distribution V,typename Int>
void LockedView
( DM& A, const DM& B,
  Int i, Int j, Int height, Int width );
  
//
// View two horizontally connected matrices
//

template<typename Int>
void View1x2
( AM& A,
  AM& BL, AM& BR );
template<typename T,typename Int>
void View1x2
( M& A,
  M& BL, M& BR );
template<typename T,Distribution U,Distribution V,typename Int>
void View1x2
( DM& A,
  DM& BL, DM& BR );

template<typename Int>
void LockedView1x2
( AM& A,
  const AM& BL, 
  const AM& BR );
template<typename T,typename Int>
void LockedView1x2
(       M& A,
  const M& BL,
  const M& BR );
template<typename T,Distribution U,Distribution V,typename Int>
void LockedView1x2
(       DM& A,
  const DM& BL,
  const DM& BR );

//
// View two vertically connected matrices
//

template<typename Int>
void View2x1
( AM& A,
  AM& BT, 
  AM& BB );
template<typename T,typename Int>
void View2x1
( M& A,
  M& BT,
  M& BB );
template<typename T,Distribution U,Distribution V,typename Int>
void View2x1
( DM& A,
  DM& BT,
  DM& BB );

template<typename Int>
void LockedView2x1
( AM& A,
  const AM& BT, 
  const AM& BB );
template<typename T,typename Int>
void LockedView2x1
(       M& A,
  const M& BT,
  const M& BB );
template<typename T,Distribution U,Distribution V,typename Int>
void LockedView2x1
(       DM& A,
  const DM& BT,
  const DM& BB );

//
// View a two-by-two set of connected matrices
//

template<typename Int>
void View2x2
( AM& A,
  AM& BTL, AM& BTR,
  AM& BBL, AM& BBR );
template<typename T,typename Int>
void View2x2
( M& A,
  M& BTL, M& BTR,
  M& BBL, M& BBR );
template<typename T,Distribution U,Distribution V,typename Int>
void View2x2
( DM& A,
  DM& BTL, DM& BTR,
  DM& BBL, DM& BBR );

template<typename Int>
void LockedView2x2
( AM& A,
  const AM& BTL, const AM& BTR,
  const AM& BBL, const AM& BBR );
template<typename T,typename Int>
void LockedView2x2
(       M& A,
  const M& BTL,
  const M& BTR,
  const M& BBL,
  const M& BBR );
template<typename T,Distribution U,Distribution V,typename Int>
void LockedView2x2
(       DM& A,
  const DM& BTL,
  const DM& BTR,
  const DM& BBL,
  const DM& BBR );

// Utilities for handling the extra information needed for [MD,* ] and [* ,MD]
template<typename T,Distribution U,Distribution V,typename Int>
void HandleDiagPath
( DM& A, const DM& B );
template<typename T,typename Int>
void HandleDiagPath
( DistMatrix<T,MD,STAR,Int>& A, const DistMatrix<T,MD,STAR,Int>& B );
template<typename T,typename Int>
void HandleDiagPath
( DistMatrix<T,STAR,MD,Int>& A, const DistMatrix<T,STAR,MD,Int>& B );

#undef M
#undef AM
#undef DM

} // namespace elem

#endif // ifndef CORE_VIEW_DECL_HPP
