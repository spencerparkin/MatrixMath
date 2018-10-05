// ComplexElement.h

#pragma once

#include "Element.h"

namespace MatrixMath
{
	class ComplexElement;
}

class MATRIX_MATH_API MatrixMath::ComplexElement : public MatrixMath::Element
{
public:

	ComplexElement( double real = 0.0, double imag = 0.0 );
	ComplexElement( const Element& element );
	virtual ~ComplexElement( void );

	virtual Element* GetCopy( void ) const override;
	virtual void SetCopy( const Element* element ) override;

	virtual bool IsApproximately( const Element* element ) const override;

	virtual void SetAdditiveIdentity( void ) override;
	virtual void SetMultiplicativeIdentity( void ) override;

	virtual bool SetSum( const Element* lElement, const Element* rElement ) override;
	virtual bool SetProduct( const Element* lElement, const Element* rElement ) override;

	virtual void Conjugate( void ) override;
	virtual void Negate( void ) override;
	virtual bool Invert( void ) override;

	virtual bool Accumulate( const Element* element ) override;
	virtual bool Scale( const Element* element ) override;

	double real, imag;
};

// ComplexElement.h