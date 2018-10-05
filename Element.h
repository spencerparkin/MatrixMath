// Element.h

#pragma once

#include "Defines.h"

namespace MatrixMath
{
	class Element;
	class ElementFactory;

	void SetElementFactory( ElementFactory* elementFactory );
	ElementFactory* GetElementFactory( void );
	
	// TODO: Push, pop element factory?
}

class MATRIX_MATH_API MatrixMath::ElementFactory
{
public:

	ElementFactory( void );
	virtual ~ElementFactory( void );

	virtual Element* Create( void );
	virtual void Destroy( Element* element );
};

// Our interface here assumes that we're a member of a field.
class MATRIX_MATH_API MatrixMath::Element
{
public:

	Element( void );
	Element( const Element& element );
	virtual ~Element( void );

	virtual Element* GetCopy( void ) const = 0;
	virtual void SetCopy( const Element* element ) = 0;

	virtual bool IsApproximately( const Element* element ) const = 0;

	virtual void SetAdditiveIdentity( void ) = 0;
	virtual void SetMultiplicativeIdentity( void ) = 0;

	virtual bool SetSum( const Element* lElement, const Element* rElement ) = 0;
	virtual bool SetProduct( const Element* lElement, const Element* rElement ) = 0;

	virtual void Conjugate( void ) = 0;
	virtual void Negate( void ) = 0;
	virtual bool Invert( void ) = 0;

	virtual bool Accumulate( const Element* element ) = 0;
	virtual bool Scale( const Element* element ) = 0;
};

// Element.h