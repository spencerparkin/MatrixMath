// Matrix.h

#pragma once

#include "Defines.h"

namespace MatrixMath
{
	class Element;
	class Matrix;
	class RowOperation;

	typedef std::list< RowOperation* > RowOperationList;
}

class MATRIX_MATH_API MatrixMath::Matrix
{
	friend class RowSwap;

public:
	Matrix( int rows = 0, int cols = 0 );
	virtual ~Matrix( void );

	void SetDimensions( int rows, int cols );
	void GetDimensions( int& rows, int& cols ) const;

	int GetRows( void ) const { return rows; }
	int GetCols( void ) const { return cols; }

	bool IsSquare( void ) const;
	bool IsIdentity( void ) const;
	bool IsUnitary( void ) const;

	bool IsInRowEchelonForm( void ) const;
	bool IsInReducedRowEchelonForm( void ) const;

	bool HasRow( int row ) const;
	bool HasCol( int col ) const;

	bool SetElement( int row, int col, const Element* element );
	const Element* GetElement( int row, int col ) const;
	Element* GetElement( int row, int col );

	bool SetIdentity( void );

	Element* GetDeterminant( void ) const;

	bool SetCopy( const Matrix& matrix );
	bool GetCopy( Matrix& matrix ) const;

	bool SetTranspose( const Matrix& matrix );
	bool GetTranspose( Matrix& matrix ) const;

	bool SetConjugateTranspose( const Matrix& matrix );
	bool GetConjugateTranspose( Matrix& matrix ) const;

	void Transpose( void );
	void ConjugateTranspose( void );

	bool SetInverse( const Matrix& matrix, bool pseudo = false );
	bool GetInverse( Matrix& matrix, bool pseudo = false ) const;

	bool SetRightInverse( const Matrix& matrix, bool pseudo = false );
	bool GetRightInverse( Matrix& matrix, bool pseudo = false ) const;

	bool SetLeftInverse( const Matrix& matrix, bool pseudo = false );
	bool GetLeftInverse( Matrix& matrix, bool pseudo = false ) const;

	bool GetLUFactorization( Matrix& lMatrix, Matrix& uMatrix ) const;
	bool GetQRFactorization( Matrix& qMatrix, Matrix& rMatrix ) const;

	void GetSingularValueDecomposition( Matrix& uMatrix, Matrix& sMatrix, Matrix& vMatrix ) const;
	bool GetEigenValueDecomposition( Matrix& qMatrix, Matrix& lMatrix ) const;

	void GetNearestOrthogonalMatrix( Matrix& matrix ) const;

	bool SetProduct( const Matrix& lMatrix, const Matrix& rMatrix );
	bool SetSum( const Matrix& lMatrix, const Matrix& rMatrix );
	bool SetScale( const Matrix& matrix, const Element* element );

	bool MultiplyOnRight( const Matrix& rMatrix );
	bool MultiplyOnLeft( const Matrix& lMatrix );

	void RowReduce( RowOperationList& rowOperationList, bool fullyReduce = true );
	bool ApplyRowOperations( const RowOperationList& rowOperationList );

	int GetLeadingZerosInRow( int row ) const;
	int GetLeadingNonZeroElementColInRow( int row ) const;

private:

	Element*** elementArray;
	int rows, cols;
};



// Matrix.h