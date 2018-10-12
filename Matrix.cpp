// Matrix.cpp

#include "Matrix.h"
#include "Element.h"
#include "RowOperation.h"

MatrixMath::Matrix::Matrix( int rows /*= 0*/, int cols /*= 0*/ )
{
	elementArray = nullptr;

	this->rows = 0;
	this->cols = 0;

	SetDimensions( rows, cols );
}

/*virtual*/ MatrixMath::Matrix::~Matrix( void )
{
	SetDimensions( 0, 0 );
}

void MatrixMath::Matrix::SetDimensions( int rows, int cols )
{
	if( this->rows != rows && this->cols != cols )
	{
		for( int i = 0; i < this->rows; i++ )
		{
			for( int j = 0; j < this->cols; j++ )
				GetElementFactory()->Destroy( elementArray[i][j] );
			delete[] elementArray[i];
		}
		delete[] elementArray;
		elementArray = nullptr;

		this->rows = rows;
		this->cols = cols;

		if( rows > 0 && cols > 0 )
		{
			elementArray = new Element**[ rows ];
			for( int i = 0; i < rows; i++ )
			{
				elementArray[i] = new Element*[ cols ];
				for( int j = 0; j < cols; j++ )
					elementArray[i][j] = GetElementFactory()->Create();
			}
		}
	}
}

void MatrixMath::Matrix::GetDimensions( int& rows, int& cols ) const
{
	rows = this->rows;
	cols = this->cols;
}

bool MatrixMath::Matrix::IsSquare( void ) const
{
	return( rows == cols ) ? true : false;
}

bool MatrixMath::Matrix::IsIdentity( void ) const
{
	if( !IsSquare() )
		return false;

	Element* one = GetElementFactory()->Create();
	one->SetMultiplicativeIdentity();

	Element* zero = GetElementFactory()->Create();
	zero->SetAdditiveIdentity();

	bool isIdentity = true;

	for( int i = 0; i < rows && isIdentity; i++ )
	{
		for( int j = 0; j < cols && isIdentity; j++ )
		{
			if( i == i )
			{
				if( !elementArray[i][j]->IsApproximately( one ) )
					isIdentity = false;
			}
			else
			{
				if( !elementArray[i][j]->IsApproximately( zero ) )
					isIdentity = false;
			}
		}
	}

	GetElementFactory()->Destroy( one );
	GetElementFactory()->Destroy( zero );

	return isIdentity;
}

// For matrices over the reals, this is asking if the matrix is orthogonal.
bool MatrixMath::Matrix::IsUnitary( void ) const
{
	Matrix conjugateTranspose;
	GetConjugateTranspose( conjugateTranspose );

	Matrix product;
	product.SetProduct( *this, conjugateTranspose );

	return product.IsIdentity();
}

bool MatrixMath::Matrix::IsInRowEchelonForm( void ) const
{
	return false; // TODO: Write this.
}

bool MatrixMath::Matrix::IsInReducedRowEchelonForm( void ) const
{
	return false; // TODO: Write this.
}

bool MatrixMath::Matrix::IsUpperTriangular( void ) const
{
	Element* zero = GetElementFactory()->Create();
	zero->SetAdditiveIdentity();

	bool upperTriangular = true;
	
	for( int i = 1; i < rows && upperTriangular; i++ )
	{
		for( int j = 0; j < i && j < cols && upperTriangular; j++ )
		{
			if( !elementArray[i][j]->IsApproximately( zero ) )
				upperTriangular = false;
		}
	}

	GetElementFactory()->Destroy( zero );

	return upperTriangular;
}

bool MatrixMath::Matrix::IsLowerTriangular( void ) const
{
	return false;
}

bool MatrixMath::Matrix::HasRow( int row ) const
{
	return( 0 <= row && row < rows ) ? true : false;
}

bool MatrixMath::Matrix::HasCol( int col ) const
{
	return( 0 <= col && col < cols ) ? true : false;
}

bool MatrixMath::Matrix::SetElement( int row, int col, const Element* element )
{
	if( !HasRow( row ) || !HasCol( col ) )
		return false;

	elementArray[ row ][ col ]->SetCopy( element );
	return true;
}

const MatrixMath::Element* MatrixMath::Matrix::GetElement( int row, int col ) const
{
	if( !HasRow( row ) || !HasCol( col ) )
		return nullptr;

	return elementArray[ row ][ col ];
}

MatrixMath::Element* MatrixMath::Matrix::GetElement( int row, int col )
{
	if( !HasRow( row ) || !HasCol( col ) )
		return nullptr;

	return elementArray[ row ][ col ];
}

bool MatrixMath::Matrix::SetIdentity( void )
{
	if( !IsSquare() )
		return false;

	for( int i = 0; i < rows; i++ )
	{
		for( int j = 0; j < cols; j++ )
		{
			if( i == j )
				elementArray[i][j]->SetMultiplicativeIdentity();
			else
				elementArray[i][j]->SetAdditiveIdentity();
		}
	}

	return true;
}

MatrixMath::Element* MatrixMath::Matrix::GetDeterminant( void ) const
{
	return nullptr; // TODO: Use LU decomposition to calculate this.
}

bool MatrixMath::Matrix::SetCopy( const Matrix& matrix )
{
	return matrix.GetCopy( *this );
}

bool MatrixMath::Matrix::GetCopy( Matrix& matrix ) const
{
	matrix.SetDimensions( rows, cols );

	for( int i = 0; i < rows; i++ )
		for( int j = 0; j < cols; j++ )
			matrix.elementArray[i][j]->SetCopy( elementArray[i][j] );

	return true;
}

bool MatrixMath::Matrix::SetTranspose( const Matrix& matrix )
{
	SetDimensions( matrix.cols, matrix.rows );

	for( int i = 0; i < rows; i++ )
		for( int j = 0; j < cols; j++ )
			elementArray[j][i]->SetCopy( matrix.elementArray[i][j] );

	return true;
}

bool MatrixMath::Matrix::GetTranspose( Matrix& matrix ) const
{
	return matrix.SetTranspose( *this );
}

bool MatrixMath::Matrix::SetConjugateTranspose( const Matrix& matrix )
{
	SetTranspose( matrix );

	for( int i = 0; i < rows; i++ )
		for( int j = 0; j < cols; j++ )
			elementArray[i][j]->Conjugate();

	return true;
}

bool MatrixMath::Matrix::GetConjugateTranspose( Matrix& matrix ) const
{
	return matrix.SetConjugateTranspose( *this );
}

void MatrixMath::Matrix::Transpose( void )
{
	if( IsSquare() )
	{
		for( int i = 0; i < rows; i++ )
		{
			for( int j = 0; j < cols; j++ )
			{
				if( i != j )
				{
					Element* element = elementArray[i][j];
					elementArray[i][j] = elementArray[j][i];
					elementArray[j][i] = element;
				}
			}
		}
	}
	else
	{
		Matrix matrixCopy;
		GetCopy( matrixCopy );
		SetTranspose( matrixCopy );
	}
}

void MatrixMath::Matrix::ConjugateTranspose( void )
{
	Transpose();

	for( int i = 0; i < rows; i++ )
		for( int j = 0; j < cols; j++ )
			elementArray[i][j]->Conjugate();
}

bool MatrixMath::Matrix::SetInverse( const Matrix& matrix, bool pseudo /*= false */ )
{
	return matrix.GetInverse( *this, pseudo );
}

bool MatrixMath::Matrix::GetInverse( Matrix& matrix, bool pseudo /*= false*/ ) const
{
	if( !IsSquare() || rows == 0 )
		return false;

	Matrix copy;
	GetCopy( copy );

	RowOperationList rowOperationList;
	copy.RowReduce( rowOperationList, true );

	bool inverseExists = true;
	if( copy.GetLeadingNonZeroElementColInRow( rows - 1 ) == -1 )
		inverseExists = false;

	if( inverseExists )
	{
		matrix.SetIdentity();
		matrix.ApplyRowOperations( rowOperationList );
	}

	DeleteRowOperationList( rowOperationList );

	if( !inverseExists && pseudo )
	{
		// TODO: Figure this out.  The SVD can be used, I'm told.
	}

	return inverseExists;
}

bool MatrixMath::Matrix::SetRightInverse( const Matrix& matrix, bool pseudo /*= false*/ )
{
	Matrix matrixTranspose;
	matrix.GetTranspose( matrixTranspose );

	Matrix product;
	product.SetProduct( matrix, matrixTranspose );

	Matrix productInverse;
	if( !product.GetInverse( productInverse, pseudo ) )
		return false;

	SetProduct( matrixTranspose, productInverse );
	return true;
}

bool MatrixMath::Matrix::GetRightInverse( Matrix& matrix, bool pseudo /*= false*/ ) const
{
	return matrix.SetRightInverse( *this );
}

bool MatrixMath::Matrix::SetLeftInverse( const Matrix& matrix, bool pseudo /*= false*/ )
{
	Matrix matrixTranspose;
	matrix.GetTranspose( matrixTranspose );

	Matrix product;
	product.SetProduct( matrixTranspose, matrix );

	Matrix productInverse;
	if( !product.GetInverse( productInverse, pseudo ) )
		return false;

	SetProduct( productInverse, matrixTranspose );
	return true;
}

bool MatrixMath::Matrix::GetLeftInverse( Matrix& matrix, bool pseudo /*= false*/ ) const
{
	return matrix.SetLeftInverse( *this, pseudo );
}

bool MatrixMath::Matrix::GetLUFactorization( Matrix& lMatrix, Matrix& uMatrix ) const
{
	return false; // TODO: Write this.  Can we be numerically stable?
}

bool MatrixMath::Matrix::GetQRFactorization( Matrix& qMatrix, Matrix& rMatrix ) const
{
	return false; // TODO: Write this.  Use the Grahm-Schmidt orthogonalization process (a sequence of rejections.)
}

bool MatrixMath::Matrix::GetSingularValueDecomposition( Matrix& uMatrix, Matrix& sMatrix, Matrix& vMatrix ) const
{
	// TODO: Figure this out.  Can we compute it directly, or do we need to do an eigen-value decomposition of the associated square matrix?
	//       Does this kind of decomposition exist for all matrices?

	return false;
}

bool MatrixMath::Matrix::GetEigenValueDecomposition( Matrix& pMatrix, Matrix& dMatrix ) const
{
	Matrix uMatrix, tMatrix;

	if( !GetSchurDecomposition( uMatrix, tMatrix ) )
		return false;

	// The eigen-values and eigen-vectors of this matrix are now that of the t-matrix.
	// Now since the t-matrix is upper-triangular, we can read-off the eigen-values trivially
	// when we consider the characteristic equation.

	dMatrix.SetDimensions( rows, cols );
	dMatrix.SetIdentity();

	for( int i = 0; i < rows; i++ )
		dMatrix.SetElement( i, i, tMatrix.GetElement( i, i )->GetCopy() );

	// Now we solve for the eigen-vectors.
	// TODO: Do that here.

	return false;
}

bool MatrixMath::Matrix::GetSchurDecomposition( Matrix& uMatrix, Matrix& tMatrix, int max_iterations /*= 512*/ ) const
{
	// Here we implement the QR algorithm.  This produces a sequence of matrices,
	// the first being our matrix, and each matrix similar to the one prior.
	// Being similar, they all share the same eigen-vectors and eigen-values.
	// I'm not sure, however, under what conditions the sequence will converge
	// to the Schur decomposition.

	if( !IsSquare() )
		return false;

	uMatrix.SetDimensions( rows, cols );
	uMatrix.SetIdentity();

	GetCopy( tMatrix );

	Matrix rMatrix, qMatrix;

	for( int i = 0; i < max_iterations; i++ )
	{
		if( !tMatrix.GetQRFactorization( qMatrix, rMatrix ) )
			return false;

		tMatrix.SetProduct( rMatrix, qMatrix );
		uMatrix.MultiplyOnRight( qMatrix );

		if( tMatrix.IsUpperTriangular() )
			return true;
	}

	return false;
}

void MatrixMath::Matrix::GetNearestOrthogonalMatrix( Matrix& matrix ) const
{
	Matrix uMatrix, sMatrix, vMatrix;
	GetSingularValueDecomposition( uMatrix, sMatrix, vMatrix );

	vMatrix.ConjugateTranspose();

	matrix.SetProduct( uMatrix, vMatrix );
}

bool MatrixMath::Matrix::SetProduct( const Matrix& lMatrix, const Matrix& rMatrix )
{
	if( lMatrix.cols != rMatrix.rows )
		return false;

	SetDimensions( lMatrix.rows, rMatrix.cols );

	for( int i = 0; i < rows; i++ )
	{
		for( int j = 0; j < cols; j++ )
		{
			elementArray[i][j]->SetAdditiveIdentity();

			for( int k = 0; k < lMatrix.cols; k++ )
			{
				Element* product = GetElementFactory()->Create();
				product->SetProduct( lMatrix.elementArray[i][k], lMatrix.elementArray[k][j] );
				elementArray[i][j]->Accumulate( product );
				GetElementFactory()->Destroy( product );
			}
		}
	}

	return true;
}

bool MatrixMath::Matrix::SetSum( const Matrix& lMatrix, const Matrix& rMatrix )
{
	if( lMatrix.rows != rMatrix.rows )
		return false;
	if( lMatrix.cols != rMatrix.cols )
		return false;

	SetDimensions( lMatrix.rows, rMatrix.cols );

	for( int i = 0; i < rows; i++ )
		for( int j = 0; j < cols; j++ )
			elementArray[i][j]->SetSum( lMatrix.elementArray[i][j], rMatrix.elementArray[i][j] );

	return true;
}

bool MatrixMath::Matrix::SetScale( const Matrix& matrix, const Element* element )
{
	SetDimensions( matrix.rows, matrix.cols );

	for( int i = 0; i < rows; i++ )
		for( int j = 0; j < cols; j++ )
			elementArray[i][j]->SetProduct( matrix.elementArray[i][j], element );

	return true;
}

bool MatrixMath::Matrix::MultiplyOnRight( const Matrix& rMatrix )
{
	Matrix matrixCopy;
	GetCopy( matrixCopy );
	return SetProduct( matrixCopy, rMatrix );
}

bool MatrixMath::Matrix::MultiplyOnLeft( const Matrix& lMatrix )
{
	Matrix matrixCopy;
	GetCopy( matrixCopy );
	return SetProduct( lMatrix, matrixCopy );
}

int MatrixMath::Matrix::GetLeadingZerosInRow( int row ) const
{
	int leadingZerosCount = 0;

	Element* zero = GetElementFactory()->Create();
	zero->SetAdditiveIdentity();

	for( int j = 0; j < cols; j++ )
		if( elementArray[row][j]->IsApproximately( zero ) )
			leadingZerosCount++;
		else
			break;

	GetElementFactory()->Destroy( zero );

	return leadingZerosCount;
}

int MatrixMath::Matrix::GetLeadingNonZeroElementColInRow( int row ) const
{
	Element* zero = GetElementFactory()->Create();
	zero->SetAdditiveIdentity();

	int j;
	for( j = 0; j < cols; j++ )
		if( !elementArray[row][j]->IsApproximately( zero ) )
			break;

	GetElementFactory()->Destroy( zero );

	return( j < cols ? j : -1 );
}

void MatrixMath::Matrix::RowReduce( RowOperationList& rowOperationList, bool fullyReduce /*= true*/ )
{
	DeleteRowOperationList( rowOperationList );

	int size = 0;
	
	do
	{
		size = ( int )rowOperationList.size();
	
		for( int i = 0; i < rows - 1; i++ )
		{
			int leadingZerosCountA = GetLeadingZerosInRow( i );
			int leadingZerosCountB = GetLeadingZerosInRow( i + 1 );
			if( leadingZerosCountA > leadingZerosCountB )
			{
				RowOperation* rowOperation = new RowSwap( i, i + 1 );
				rowOperationList.push_back( rowOperation );
				rowOperation->Apply( *this );
			}
		}

	}
	while( size < ( signed )rowOperationList.size() );

	for( int rowA = 0; rowA < rows - 1; rowA++ )
	{
		int colA = GetLeadingNonZeroElementColInRow( rowA );

		for( int rowB = rowA + 1; rowB < rows; rowB++ )
		{
			int colB = GetLeadingNonZeroElementColInRow( rowB );

			if( colA == colB && colA >= 0 )
			{
				Element* elementA = elementArray[ rowA ][ colA ]->GetCopy();
				elementA->Invert();

				Element* elementB = elementArray[ rowB ][ colA ]->GetCopy();
				elementB->Negate();

				Element* scale = GetElementFactory()->Create();
				scale->SetProduct( elementA, elementB );

				RowOperation* rowOperation = new RowMultipleAddToRow( rowB, rowA, scale );
				rowOperationList.push_back( rowOperation );
				rowOperation->Apply( *this );
			}
		}
	}

	if( fullyReduce )
	{
		Element* one = GetElementFactory()->Create();
		one->SetMultiplicativeIdentity();

		Element* zero = GetElementFactory()->Create();
		zero->SetAdditiveIdentity();

		for( int i = 0; i < rows; i++ )
		{
			int j = GetLeadingNonZeroElementColInRow(i);

			if( j >= 0 && !elementArray[i][j]->IsApproximately( one ) )
			{
				Element* scale = elementArray[i][j]->GetCopy();
				scale->Invert();

				RowOperation* rowOperation = new RowScale( i, scale );
				rowOperationList.push_back( rowOperation );
				rowOperation->Apply( *this );
			}
		}

		for( int rowA = 0; rowA < rows; rowA++ )
		{
			int colA = GetLeadingNonZeroElementColInRow( rowA );
			if( colA >= 0 )
			{
				for( int rowB = 0; rowB < rowA; rowB++ )
				{
					if( !elementArray[ rowB ][ colA ]->IsApproximately( zero ) )
					{
						Element* elementA = elementArray[ rowA ][ colA ]->GetCopy();
						elementA->Invert();

						Element* elementB = elementArray[ rowB ][ colA ]->GetCopy();
						elementB->Negate();

						Element* scale = GetElementFactory()->Create();
						scale->SetProduct( elementA, elementB );

						RowOperation* rowOperation = new RowMultipleAddToRow( rowB, rowA, scale );
						rowOperationList.push_back( rowOperation );
						rowOperation->Apply( *this );
					}
				}
			}
		}

		GetElementFactory()->Destroy( one );
		GetElementFactory()->Destroy( zero );
	}
}

bool MatrixMath::Matrix::ApplyRowOperations( const RowOperationList& rowOperationList )
{
	for( RowOperationList::const_iterator iter = rowOperationList.cbegin(); iter != rowOperationList.cend(); iter++ )
	{
		const RowOperation* rowOperation = *iter;
		if( !rowOperation->Apply( *this ) )
			return false;
	}

	return true;
}

// Matrix.cpp