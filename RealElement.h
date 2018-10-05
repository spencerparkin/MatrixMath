// RealElement.h

#pragma once

#include "Element.h"

namespace MatrixMath
{
	class RealElement;
}

class MATRIX_MATH_API MatrixMath::RealElement : public MatrixMath::Element
{
public:

	RealElement( double real = 0.0 );
	RealElement( const Element& element );
	virtual ~RealElement( void );

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

	double real;
};

// RealElement.h