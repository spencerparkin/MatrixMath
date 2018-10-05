// RowOperation.h

#pragma once

#include "Defines.h"

namespace MatrixMath
{
	class RowOperation;
	class RowSwap;
	class RowScale;
	class RowMultipleAddToRow;
	class Matrix;
	class Element;

	typedef std::list< RowOperation* > RowOperationList;

	void DeleteRowOperationList( RowOperationList& rowOperationList );
}

class MATRIX_MATH_API MatrixMath::RowOperation
{
public:
	RowOperation( void );
	virtual ~RowOperation( void );

	virtual bool Apply( Matrix& matrix ) const = 0;
};

class MATRIX_MATH_API MatrixMath::RowSwap : public MatrixMath::RowOperation
{
public:
	RowSwap( int rowA, int rowB );
	virtual ~RowSwap( void );

	virtual bool Apply( Matrix& matrix ) const override;

	int rowA, rowB;
};

class MATRIX_MATH_API MatrixMath::RowScale : public MatrixMath::RowOperation
{
public:
	RowScale( int row, Element* scale );
	virtual ~RowScale( void );

	virtual bool Apply( Matrix& matrix ) const override;

	int row;
	Element* scale;
};

class MATRIX_MATH_API MatrixMath::RowMultipleAddToRow : public MatrixMath::RowOperation
{
public:
	RowMultipleAddToRow( int rowA, int rowB, Element* scale );
	virtual ~RowMultipleAddToRow( void );

	virtual bool Apply( Matrix& matrix ) const override;

	int rowA, rowB;
	Element* scale;
};

// RowOperation.h