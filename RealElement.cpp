// RealElement.cpp

#include "RealElement.h"

MatrixMath::RealElement::RealElement( double real /*= 0.0*/ )
{
	this->real = real;
}

MatrixMath::RealElement::RealElement( const Element& element )
{
	real = ( ( const RealElement& )element ).real;
}

/*virtual*/ MatrixMath::RealElement::~RealElement( void )
{
}

/*virtual*/ MatrixMath::Element* MatrixMath::RealElement::GetCopy( void ) const
{
	return new RealElement( real );
}

/*virtual*/ void MatrixMath::RealElement::SetCopy( const Element* element )
{
	real = ( ( const RealElement* )element )->real;
}

/*virtual*/ bool MatrixMath::RealElement::IsApproximately( const Element* element ) const
{
	if( fabs( real - ( ( const RealElement* )element )->real ) < EPSILON )
		return true;

	return false;
}

/*virtual*/ void MatrixMath::RealElement::SetAdditiveIdentity( void )
{
	real = 0.0;
}

/*virtual*/ void MatrixMath::RealElement::SetMultiplicativeIdentity( void )
{
	real = 1.0;
}

/*virtual*/ bool MatrixMath::RealElement::SetSum( const Element* lElement, const Element* rElement )
{
	real = ( ( const RealElement* )lElement )->real + ( ( const RealElement* )rElement )->real;
	return true;
}

/*virtual*/ bool MatrixMath::RealElement::SetProduct( const Element* lElement, const Element* rElement )
{
	real = ( ( const RealElement* )lElement )->real * ( ( const RealElement* )rElement )->real;
	return true;
}

/*virtual*/ void MatrixMath::RealElement::Conjugate( void )
{
	// Do nothing.
}

/*virtual*/ void MatrixMath::RealElement::Negate( void )
{
	real = -real;
}

/*virtual*/ bool MatrixMath::RealElement::Invert( void )
{
	if( fabs( real ) < EPSILON )
		return false;

	real = 1.0 / real;
	return true;
}

/*virtual*/ bool MatrixMath::RealElement::Accumulate( const Element* element )
{
	real += ( ( const RealElement* )element )->real;
	return true;
}

/*virtual*/ bool MatrixMath::RealElement::Scale( const Element* element )
{
	real *= ( ( const RealElement* )element )->real;
	return true;
}

// RealElement.cpp