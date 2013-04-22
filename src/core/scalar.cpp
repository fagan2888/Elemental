/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

#include "elemental-lite.hpp"

namespace elem {

void AssertTypeMatch( ScalarTypes A, ScalarTypes B )
{
	if ( A != B ) {
		throw std::logic_error( "enum::Scalar: type mismatch" );
	}
}

void TypeCastError( ScalarTypes A, ScalarTypes B )
{
	throw std::logic_error( "enum::Scalar: cannot cast between types" );
}

template <typename Int>
void Scalar<Int>::Print( std::ostream& ostr ) const
{
	switch ( type_ ) {
	case INTEGRAL: ostr << "Scalar( Integer, " << Get_<Int>() << " )"; break;
#ifndef DISABLE_FLOAT	
	case SINGLE:   ostr << "Scalar( Float, " << Get_<float>() << " )"; break;
#endif
	case DOUBLE:   ostr << "Scalar( Double, " << Get_<double>() << " )"; break;
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT	
	case SCOMPLEX: ostr << "Scalar( SComplex, (" << Get_<scomplex>().real << "," << Get_<scomplex>().imag << ") )"; break;
#endif
	case DCOMPLEX: ostr << "Scalar( DComplex, (" << Get_<dcomplex>().real << "," << Get_<dcomplex>().imag << ") )"; break;
#endif
	default: ostr << "Scalar( Unknown, * )"; break;
	}
}

template struct ScalarType<int,int>;
template struct ScalarEnum<INTEGRAL,int>;
template class Scalar<int>;
#ifndef DISABLE_FLOAT
template struct ScalarType<float,int>;
template struct ScalarEnum<SINGLE,int>;
#endif
template struct ScalarType<double,int>;
template struct ScalarEnum<DOUBLE,int>;
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
template struct ScalarType<scomplex,int>;
template struct ScalarType<std::complex<float>,int>;
#endif
template struct ScalarType<dcomplex,int>;
template struct ScalarType<std::complex<double>,int>;
#endif

} // namespace elem
