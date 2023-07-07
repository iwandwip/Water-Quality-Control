/*
 *  fis-header.h
 *
 *  fis fuzzy lib
 *  Created on: 2023. 4. 3
 */

#define FIS_TYPE float
#define FIS_RESOLUSION 101
#define FIS_MIN -3.4028235E+38
#define FIS_MAX 3.4028235E+38
typedef FIS_TYPE (*_FIS_MF)(FIS_TYPE, FIS_TYPE*);
typedef FIS_TYPE (*_FIS_ARR_OP)(FIS_TYPE, FIS_TYPE);
typedef FIS_TYPE (*_FIS_ARR)(FIS_TYPE*, int, _FIS_ARR_OP);