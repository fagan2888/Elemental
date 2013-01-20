/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#ifndef ELEMENTAL_LITE_HPP
#define ELEMENTAL_LITE_HPP 1

#include "elemental/config.h"
#ifdef HAVE_F90_INTERFACE
# include "elemental/FCMangle.h"
#endif

#include "elemental/core.hpp"
#include "elemental/blas-like.hpp"

#include "elemental/lapack-like_decl.hpp"

#endif // ifndef ELEMENTAL_LITE_HPP