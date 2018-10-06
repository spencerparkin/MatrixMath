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
		//...
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
		Matrix uMatrix, sMatrix, vMatrix;
		GetSingularValueDecomposition( uMatrix, sMatrix, vMatrix );

		uMatrix.ConjugateTranspose();
		vMatrix.ConjugateTranspose();

		Element* zero = GetElementFactory()->Create();
		zero->SetAdditiveIdentity();

		int j = sMatrix.rows < sMatrix.cols ? sMatrix.rows : sMatrix.cols;

		for( int i = 0; i < j; i++ )
		{
			Element* element = sMatrix.elementArray[i][i];
			if( !element->IsApproximately( zero ) )
				element->Invert();
		}

		GetElementFactory()->Destroy( zero );

		sMatrix.Transpose();

		matrix.SetProduct( vMatrix, sMatrix );
		matrix.MultiplyOnRight( uMatrix );
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
	return false; // TODO: Write this.  Use the Grahm-Schmidt process.
}

void MatrixMath::Matrix::GetSingularValueDecomposition( Matrix& uMatrix, Matrix& sMatrix, Matrix& vMatrix ) const
{
	// TODO: Write this.  This can be used to find the pseudo-inverse of a matrix.

	/*
	The naive and surely numerically unstable approach is as follows.

	M is our m x n matrix and we want to find U, S and V such that...

	M = U*S*V^{*},

	...where V^{*} is the conjugate transpose, and U (m x s) and V (n x s) are unitary matrices,
	and S (s x s) is a diagonal matrix with non-negative values on the diagonal.
	To that end, observe that...

	M*M^{*} = U*S*V^{*}*(V*S^{*}*U^{*}) = U*S*S^{*}*U^{*}.

	Thus, to find U and S, we first find the eigen-value decomposition of M*M^{*} as...

	M*M^{*} = Q*L*Q^{-1}.

	...where the columns of Q contain the eigen-vectors, and the diagonals of L are
	the corresponding eigen-values.  Now, in some cases (which ones?), the columns of
	Q can be chosen so that they form an orthonormal basis, at which point, it follows
	that U=Q since we would have Q^{-1}=Q^{*}, and then the diagonals of S would simply
	be the square roots of the diagonals of L.  The diagonals of L could all be made
	positive by negating the corresponding eigen-vectors if necessary.

	All that remains now is to find V.  But this is trivial since we have our original
	equation M = U*S*V^{*}, which implies that...

	V^{*} = S^{*}*U^{*}*M ==> V = M^{*}*U*S.

	The rows of U and V and the diagonals of S could then be sorted (in unison) so that
	the diagonals appears in descending order, as is convention.  It has been shown that
	the SVD of any matrix M exists, and under this sorting condition, is unique.

	Question: Will M*M^{*} always have an EVD for any M?  I've read that the SVD can always
	be found, but if the EVD doesn't always exist, then what's stated above is not a general
	solution to finding the SVD.
	*/
}

bool MatrixMath::Matrix::GetEigenValueDecomposition( Matrix& qMatrix, Matrix& lMatrix ) const
{
	// This would be very hard to write in general as it would involve finding
	// all of the roots and their multiplicities in the characteristic equation.
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
	return false; // TODO: Write this.
}

bool MatrixMath::Matrix::MultiplyOnLeft( const Matrix& lMatrix )
{
	return false; // TODO: Write this;
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
		size = rowOperationList.size();
	
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