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

#define SCALARSETUP(TYPE,RTYPE,ENUM,ISCPLX,CCPLX) \
	template <typename Int> \
	struct ScalarTypeBase<TYPE,ENUM,Int> { \
		typedef TYPE Type; \
		typedef TYPE RealType; \
		typedef Int IntegralType; \
		static const size_t SizeOf = sizeof(TYPE); \
		static const ScalarTypes Enum = ENUM; \
		static const bool isValid = true; \
		static const bool isComplex = ISCPLX; \
		static const bool canBeComplex = CCPLX; \
	};
	
SCALARSETUP(Int,Int,INTEGRAL,false,false)
#ifndef DISABLE_FLOAT
SCALARSETUP(float,float,SINGLE,false,true)
#endif
SCALARSETUP(double,double,DOUBLE,false,true)
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
SCALARSETUP(scomplex,float,SCOMPLEX,true,false)
template <typename Int>
struct ScalarType<std::complex<float>,Int> : public ScalarTypeBase<scomplex,SCOMPLEX,Int> {};
#endif
SCALARSETUP(dcomplex,double,DCOMPLEX,true,false)
template <typename Int>
struct ScalarType<std::complex<double>,Int> : public ScalarTypeBase<dcomplex,DCOMPLEX,Int> {};
#endif

#undef SCALARSETUP

inline std::string
ScalarTypeToString( ScalarTypes stype )
{
    switch( stype )
    {
    	case INTEGRAL:  return "Integral";
#ifndef DISABLE_FLOAT   	
    	case SINGLE:    return "Single";
#endif
    	case DOUBLE:    return "Double";
#ifndef DISABLE_COMPLEX
#ifndef DISABLE_FLOAT
    	case SCOMPLEX:  return "SComplex";
#endif
    	case DCOMPLEX:  return "DComplex";
#endif
    	default:        return "Unknown";
    }
}

} // namespace data_type_wrapper

namespace distribution_wrapper {

#define DISTSETUP(U,V,UN,VN) \
	const char* const T_ ## U ## _ ## V = #UN "_" #VN; \
	template <> \
	struct DistMapBase<U,V,U ## _ ## V> { \
		static const Distribution RowDist = U; \
		static const Distribution ColDist = V; \
		static const Distribution2D Dist2D = U ## _ ## V; \
		static const bool isValid = true; \
	};
DISTSETUP(MC,MR,MC,MR)
DISTSETUP(MC,STAR,MC,*)
DISTSETUP(MD,STAR,MD,*)
DISTSETUP(MR,MC,MR,MC)
DISTSETUP(MR,STAR,MR,*)
DISTSETUP(STAR,MC,*,MC)
DISTSETUP(STAR,MD,*,MD)
DISTSETUP(STAR,MR,*,MR)
DISTSETUP(STAR,STAR,*,*)
DISTSETUP(STAR,VC,*,VC)
DISTSETUP(STAR,VR,*,VR)
DISTSETUP(VC,STAR,VC,*)
DISTSETUP(VR,STAR,VR,*)
#undef DISTSETUP

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

inline std::string
Dist2DToString( Distribution2D dist2D )
{
	switch ( dist2D )
	{
		case MC_MR:      return "MC_MR";
		case MC_STAR:    return "MC_STAR";
		case MD_STAR:    return "MD_STAR";
		case MR_MC:      return "MR_MC";
		case MR_STAR:    return "MR_STAR";
		case STAR_MC:    return "STAR_MC";
		case STAR_MD:    return "STAR_MD";
		case STAR_MR:    return "STAR_MR";
		case STAR_STAR:  return "STAR_STAR";
		case STAR_VC:    return "STAR_VC";
		case STAR_VR:    return "STAR_VR";
		case VC_STAR:	 return "VC_STAR";
		case VR_STAR:    return "VR_STAR";
	}
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
