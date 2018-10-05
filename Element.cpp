// Element.cpp

#include "Element.h"
#include "RealElement.h"

namespace MatrixMath
{
	ElementFactory* theElementFactory = nullptr;
}

void MatrixMath::SetElementFactory( ElementFactory* elementFactory )
{
	theElementFactory = elementFactory;
}

MatrixMath::ElementFactory* MatrixMath::GetElementFactory( void )
{
	return theElementFactory;
}

MatrixMath::ElementFactory::ElementFactory( void )
{
}

/*virtual*/ MatrixMath::ElementFactory::~ElementFactory( void )
{
}

/*virtual*/ MatrixMath::Element* MatrixMath::ElementFactory::Create( void )
{
	return new RealElement();
}

/*virtual*/ void MatrixMath::ElementFactory::Destroy( Element* element )
{
	delete element;
}

MatrixMath::Element::Element( void )
{
}

MatrixMath::Element::Element( const Element& element )
{
}

/*virtual*/ MatrixMath::Element::~Element( void )
{
}

// Element.cpp