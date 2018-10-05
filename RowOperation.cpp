// RowOperation.cpp

#include "RowOperation.h"
#include "Element.h"
#include "Matrix.h"

void MatrixMath::DeleteRowOperationList( RowOperationList& rowOperationList )
{
	while( rowOperationList.size() > 0 )
	{
		RowOperationList::iterator iter = rowOperationList.begin();
		RowOperation* rowOperation = *iter;
		delete rowOperation;
		rowOperationList.erase( iter );
	}
}

MatrixMath::RowOperation::RowOperation( void )
{
}

/*virtual*/ MatrixMath::RowOperation::~RowOperation( void )
{
}

MatrixMath::RowSwap::RowSwap( int rowA, int rowB )
{
	this->rowA = rowA;
	this->rowB = rowB;
}

/*virtual*/ MatrixMath::RowSwap::~RowSwap( void )
{
}

/*virtual*/ bool MatrixMath::RowSwap::Apply( Matrix& matrix ) const
{
	if( !matrix.HasRow( rowA ) || !matrix.HasRow( rowB ) )
		return false;

	for( int j = 0; j < matrix.GetCols(); j++ )
	{
		Element* element = matrix.elementArray[rowA][j];
		matrix.elementArray[rowA][j] = matrix.elementArray[rowB][j];
		matrix.elementArray[rowB][j] = element;
	}

	return true;
}

MatrixMath::RowScale::RowScale( int row, Element* scale )
{
	this->row = row;
	this->scale = scale;
}

/*virtual*/ MatrixMath::RowScale::~RowScale( void )
{
	GetElementFactory()->Destroy( scale );
}

/*virtual*/ bool MatrixMath::RowScale::Apply( Matrix& matrix ) const
{
	if( !matrix.HasRow( row ) )
		return false;

	for( int j = 0; j < matrix.GetCols(); j++ )
		matrix.GetElement( row, j )->Scale( scale );

	return true;
}

MatrixMath::RowMultipleAddToRow::RowMultipleAddToRow( int rowA, int rowB, Element* scale )
{
	this->rowA = rowA;
	this->rowB = rowB;
	this->scale = scale;
}

/*virtual*/ MatrixMath::RowMultipleAddToRow::~RowMultipleAddToRow( void )
{
	GetElementFactory()->Destroy( scale );
}

/*virtual*/ bool MatrixMath::RowMultipleAddToRow::Apply( Matrix& matrix ) const
{
	if( !matrix.HasRow( rowA ) || !matrix.HasRow( rowB ) )
		return false;

	Element* product = GetElementFactory()->Create();

	for( int j = 0; j < matrix.GetCols(); j++ )
	{
		product->SetProduct( matrix.GetElement( rowB, j ), scale );
		matrix.GetElement( rowA, j )->Accumulate( product );
	}

	GetElementFactory()->Destroy( product );

	return true;
};

// RowOperation.cpp