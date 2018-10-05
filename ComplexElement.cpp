// ComplexElement.cpp

#include "ComplexElement.h"

MatrixMath::ComplexElement::ComplexElement( double real /*= 0.0*/, double imag /*= 0.0*/ )
{
	this->real = real;
	this->imag = imag;
}

MatrixMath::ComplexElement::ComplexElement( const Element& element )
{
	real = ( ( const ComplexElement& )element ).real;
	imag = ( ( const ComplexElement& )element ).imag;
}

/*virtual*/ MatrixMath::ComplexElement::~ComplexElement( void )
{
}

/*virtual*/ MatrixMath::Element* MatrixMath::ComplexElement::GetCopy( void ) const
{
	return new ComplexElement( real, imag );
}

/*virtual*/ void MatrixMath::ComplexElement::SetCopy( const Element* element )
{
	real = ( ( const ComplexElement* )element )->real;
	imag = ( ( const ComplexElement* )element )->imag;
}

/*virtual*/ bool MatrixMath::ComplexElement::IsApproximately( const Element* element ) const
{
	if( fabs( real - ( ( const ComplexElement* )element )->real ) >= EPSILON )
		return false;

	if( fabs( imag - ( ( const ComplexElement* )element )->imag ) >= EPSILON )
		return false;

	return true;
}

/*virtual*/ void MatrixMath::ComplexElement::SetAdditiveIdentity( void )
{
	real = 0.0;
	imag = 0.0;
}

/*virtual*/ void MatrixMath::ComplexElement::SetMultiplicativeIdentity( void )
{
	real = 1.0;
	imag = 0.0;
}

/*virtual*/ bool MatrixMath::ComplexElement::SetSum( const Element* lElement, const Element* rElement )
{
	real = ( ( const ComplexElement* )lElement )->real + ( ( const ComplexElement* )rElement )->real;
	imag = ( ( const ComplexElement* )lElement )->imag + ( ( const ComplexElement* )rElement )->imag;
	return true;
}

/*virtual*/ bool MatrixMath::ComplexElement::SetProduct( const Element* lElement, const Element* rElement )
{
	double lReal = ( ( const ComplexElement* )lElement )->real;
	double rReal = ( ( const ComplexElement* )rElement )->real;

	double lImag = ( ( const ComplexElement* )lElement )->imag;
	double rImag = ( ( const ComplexElement* )rElement )->imag;

	real = lReal * rReal - lImag * rImag;
	imag = lReal * rImag + lImag * rReal;

	return true;
}

/*virtual*/ void MatrixMath::ComplexElement::Conjugate( void )
{
	imag = -imag;
}

/*virtual*/ void MatrixMath::ComplexElement::Negate( void )
{
	real = -real;
	imag = -imag;
}

/*virtual*/ bool MatrixMath::ComplexElement::Invert( void )
{
	double lambda = real * real + imag * imag;

	if( fabs( lambda ) < EPSILON )
		return false;

	real = real / lambda;
	imag = -imag / lambda;

	return true;
}

/*virtual*/ bool MatrixMath::ComplexElement::Accumulate( const Element* element )
{
	real += ( ( const ComplexElement* )element )->real;
	imag += ( ( const ComplexElement* )element )->imag;

	return true;
}

/*virtual*/ bool MatrixMath::ComplexElement::Scale( const Element* element )
{
	double productReal = real * ( ( const ComplexElement* )element )->real - imag * ( ( const ComplexElement* )element )->imag;
	double productImag = real * ( ( const ComplexElement* )element )->imag + imag * ( ( const ComplexElement* )element )->real;

	real = productReal;
	imag = productImag;

	return true;
}

// ComplexElement.cpp