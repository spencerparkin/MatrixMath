// Defines.h

#pragma once

#if defined MATRIX_MATH_API_EXPORT
#	define MATRIX_MATH_API		__declspec( dllexport )
#elif defined MATRIX_MATH_API_IMPORT
#	define MATRIX_MATH_API		__declspec( dllimport )
#else
#	define MATRIX_MATH_API
#endif

#define EPSILON				1e-7

#include <list>

// Defines.h