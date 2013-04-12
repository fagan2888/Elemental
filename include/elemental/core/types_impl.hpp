/*
   Copyright (c) 2009-2013, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef CORE_TYPES_IMPL_HPP
#define CORE_TYPES_IMPL_HPP

namespace elem {

template<typename F,typename Int>
inline
SafeProduct<F,Int>::SafeProduct( Int numEntries )
: rho(1), kappa(0), n(numEntries)
{ }

namespace data_type_wrapper {

template <typename T,enum ScalarTypes S,typename Int>
const char* ScalarTypeBase<T,S,Int>::Name = "Unknown";

template <typename Int>
struct ScalarTypeBase<Int,INTEGRAL,Int> {
	typedef Int Self;
	typedef Int Real;
	static const ScalarTypes Enum = INTEGRAL;
	static const char* Name;
	static const bool isComplex = false;
	static const bool canBeComplex = false;
};

template <typename Int>
const char* ScalarTypeBase<Int,INTEGRAL,Int>::Name = "Integral";

template <typename Int>
struct ScalarType<Int,Int> : public ScalarTypeBase<Int,INTEGRAL,Int> {};

template <typename Int>
struct ScalarEnum<INTEGRAL,Int> : public ScalarTypeBase<Int,INTEGRAL,Int> {};

#ifndef DISABLE_FLOAT
template <typename Int>
struct ScalarTypeBase<float,SINGLE,Int> {
	typedef float Type;
	typedef float Real;
	static const ScalarTypes Enum = SINGLE;
	static const char* Name;
	static const bool isComplex = false;
	static const bool canBeComplex = true;
};

template <typename Int>
const char* ScalarTypeBase<float,SINGLE,Int>::Name = "Single";

template <typename Int>
struct ScalarType<float,Int> : public ScalarTypeBase<float,SINGLE,Int> {};

template <typename Int>
struct ScalarEnum<SINGLE,Int> : public ScalarTypeBase<float,SINGLE,Int> {};
#endif

template <typename Int>
struct ScalarTypeBase<double,DOUBLE,Int> {
	typedef double Type;
	typedef double Real;
	static const ScalarTypes Enum = DOUBLE;
	static const char* Name;
	static const bool isComplex = false;
	static const bool canBeComplex = true;
};

template <typename Int>
const char* ScalarTypeBase<double,DOUBLE,Int>::Name = "Double";

template <typename Int>
struct ScalarType<double,Int> : public ScalarTypeBase<double,DOUBLE,Int> {};

template <typename Int>
struct ScalarEnum<DOUBLE,Int> : public ScalarTypeBase<double,DOUBLE,Int> {};

#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
template <typename Int>
struct ScalarTypeBase<scomplex,SCOMPLEX,Int> {
	typedef scomplex Type;
	typedef float Real;
	static const ScalarTypes Enum = SCOMPLEX;
	static const char* Name;
	static const bool isComplex = true;
	static const bool canBeComplex = false;
};

template <typename Int>
const char* ScalarTypeBase<scomplex,SCOMPLEX,Int>::Name = "SComplex";

template <typename Int>
struct ScalarType<scomplex,Int> : public ScalarTypeBase<scomplex,SCOMPLEX,Int> {};

template <typename Int>
struct ScalarEnum<SCOMPLEX,Int> : public ScalarTypeBase<scomplex,SCOMPLEX,Int> {};
#endif

template <typename Int>
struct ScalarTypeBase<dcomplex,DCOMPLEX,Int> {
	typedef dcomplex Type;
	typedef double Real;
	static const ScalarTypes Enum = DCOMPLEX;
	static const char* Name;
	static const bool isComplex = true;
	static const bool canBeComplex = false;
};

template <typename Int>
const char* ScalarTypeBase<dcomplex,DCOMPLEX,Int>::Name = "SComplex";

template <typename Int>
struct ScalarType<dcomplex,Int> : public ScalarTypeBase<dcomplex,SCOMPLEX,Int> {};

template <typename Int>
struct ScalarEnum<DCOMPLEX,Int> : public ScalarTypeBase<dcomplex,SCOMPLEX,Int> {};
#endif

} // namespace data_type_wrapper

namespace distribution_wrapper {

inline std::string 
DistToString( Distribution distribution )
{
    std::string distString;
    switch( distribution )
    {
        case MC: distString = "MC"; break;
        case MD: distString = "MD"; break;
        case MR: distString = "MR"; break;
        case VC: distString = "VC"; break;
        case VR: distString = "VR"; break;
        default: distString = "* "; break;
    }
    return distString;
}

inline Distribution 
StringToDist( std::string s )
{
    Distribution distribution;
    if( s == "MC" )
        distribution = MC;
    else if( s == "MD" )
        distribution = MD;
    else if( s == "MR" )
        distribution = MR;
    else if( s == "VC" )
        distribution = VC;
    else if( s == "VR" )
        distribution = VR;
    else if( s == "* " || s == " *" || s == "*" )
        distribution = STAR;
    else
    {
        throw std::logic_error
        ("StringToDist expects string in "
         "{\"MC\",\"MD\",\"MR\",\"VC\",\"VR\",\"* \",\" *\",\"*\"}");
    }
    return distribution;
}

} // namespace distribution_wrapper

namespace left_or_right_wrapper {

inline char 
LeftOrRightToChar( LeftOrRight side )
{
    char sideChar;
    switch( side )
    {
        case LEFT:  sideChar = 'L'; break;
        default:    sideChar = 'R'; break;
    }
    return sideChar;
}
    
inline LeftOrRight 
CharToLeftOrRight( char c )
{
    LeftOrRight side;
    switch( c )
    {
        case 'L': side = LEFT;  break;
        case 'R': side = RIGHT; break;
        default:
            throw std::logic_error("CharToLeftOrRight expects char in {L,R}");
    }
    return side;
}

} // namespace left_or_right_wrapper

namespace orientation_wrapper {

inline char 
OrientationToChar( Orientation orientation )
{
    char orientationChar;
    switch( orientation )
    {
        case NORMAL:    orientationChar = 'N'; break;
        case TRANSPOSE: orientationChar = 'T'; break;
        default:        orientationChar = 'C'; break;
    }
    return orientationChar;
}

inline Orientation 
CharToOrientation( char c )
{
    Orientation orientation;
    switch( c )
    {
        case 'N': orientation = NORMAL;    break;
        case 'T': orientation = TRANSPOSE; break;
        case 'C': orientation = ADJOINT;   break;
        default:
            throw std::logic_error
            ("CharToOrientation expects char in {N,T,C}");
    }
    return orientation;
}

} // namespace orientation_wrapper

namespace unit_or_non_unit_wrapper {

inline char 
UnitOrNonUnitToChar( UnitOrNonUnit diag )
{
    char diagChar;
    switch( diag )
    {
        case NON_UNIT: diagChar = 'N'; break;
        default:       diagChar = 'U'; break;
    }
    return diagChar;
}

inline UnitOrNonUnit 
CharToUnitOrNonUnit( char c )
{
    UnitOrNonUnit diag;
    switch( c )
    {
        case 'N': diag = NON_UNIT; break;
        case 'U': diag = UNIT;     break;
        default:
            throw std::logic_error("CharToUnitOrNonUnit expects char in {N,U}");
    }
    return diag;
}

} // namespace unit_or_non_unit_wrapper

namespace upper_or_lower_wrapper {

inline char 
UpperOrLowerToChar( UpperOrLower uplo )
{
    char uploChar;
    switch( uplo )
    {
        case LOWER: uploChar = 'L'; break;
        default:    uploChar = 'U'; break;
    }
    return uploChar;
}

inline UpperOrLower 
CharToUpperOrLower( char c )
{
    UpperOrLower uplo;
    switch( c )
    {
        case 'L': uplo = LOWER; break;
        case 'U': uplo = UPPER; break;
        default:
            throw std::logic_error("CharToUpperOrLower expects char in {L,U}");
    }
    return uplo;
}

} // namespace upper_or_lower_wrapper

} // namespace elem

#endif // ifndef CORE_TYPES_IMPL_HPP
