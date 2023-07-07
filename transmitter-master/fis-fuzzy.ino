/*
 *  fis-fuzzy.ino
 *
 *  fis fuzzy lib
 *  Created on: 2023. 4. 3
 */

FIS_TYPE fis_trapmf(FIS_TYPE x, FIS_TYPE* p) {
        FIS_TYPE a = p[0], b = p[1], c = p[2], d = p[3];
        FIS_TYPE t1 = ((x <= c) ? 1 : ((d < x) ? 0 : ((c != d) ? ((d - x) / (d - c)) : 0)));
        FIS_TYPE t2 = ((b <= x) ? 1 : ((x < a) ? 0 : ((a != b) ? ((x - a) / (b - a)) : 0)));
        return (FIS_TYPE)min(t1, t2);
}

// Triangular Member Function
FIS_TYPE fis_trimf(FIS_TYPE x, FIS_TYPE* p) {
        FIS_TYPE a = p[0], b = p[1], c = p[2];
        FIS_TYPE t1 = (x - a) / (b - a);
        FIS_TYPE t2 = (c - x) / (c - b);
        if ((a == b) && (b == c)) return (FIS_TYPE)(x == a);
        if (a == b) return (FIS_TYPE)(t2 * (b <= x) * (x <= c));
        if (b == c) return (FIS_TYPE)(t1 * (a <= x) * (x <= b));
        t1 = min(t1, t2);
        return (FIS_TYPE)max(t1, 0.0f);
}

FIS_TYPE fis_min(FIS_TYPE a, FIS_TYPE b) {
        return min(a, b);
}

FIS_TYPE fis_max(FIS_TYPE a, FIS_TYPE b) {
        return max(a, b);
}

FIS_TYPE fis_array_operation(FIS_TYPE* array, int size, _FIS_ARR_OP pfnOp) {
        int i;
        FIS_TYPE ret = 0;

        if (size == 0) return ret;
        if (size == 1) return array[0];

        ret = array[0];
        for (i = 1; i < size; i++) {
                ret = (*pfnOp)(ret, array[i]);
        }

        return ret;
}


//***********************************************************************
// Data for Fuzzy Inference System
//***********************************************************************
// Pointers to the implementations of member functions
_FIS_MF fis_gMF[] = {
        fis_trapmf, fis_trimf
};

// Count of member function for each Input
int fis_gIMFCount[] = { 3, 3, 3, 3, 3 };

// Count of member function for each Output
int fis_gOMFCount[] = { 2, 2, 2, 2, 2 };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFI0Coeff1[] = { 0, 0, 100, 105 };
FIS_TYPE fis_gMFI0Coeff2[] = { 100, 110, 120 };
FIS_TYPE fis_gMFI0Coeff3[] = { 115, 120, 200, 200 };
FIS_TYPE* fis_gMFI0Coeff[] = { fis_gMFI0Coeff1, fis_gMFI0Coeff2, fis_gMFI0Coeff3 };
FIS_TYPE fis_gMFI1Coeff1[] = { 0, 0, 120, 140 };
FIS_TYPE fis_gMFI1Coeff2[] = { 120, 150, 180 };
FIS_TYPE fis_gMFI1Coeff3[] = { 160, 180, 300, 300 };
FIS_TYPE* fis_gMFI1Coeff[] = { fis_gMFI1Coeff1, fis_gMFI1Coeff2, fis_gMFI1Coeff3 };
FIS_TYPE fis_gMFI2Coeff1[] = { 0, 0, 24, 26 };
FIS_TYPE fis_gMFI2Coeff2[] = { 24, 27, 30 };
FIS_TYPE fis_gMFI2Coeff3[] = { 28, 30, 40, 42 };
FIS_TYPE* fis_gMFI2Coeff[] = { fis_gMFI2Coeff1, fis_gMFI2Coeff2, fis_gMFI2Coeff3 };
FIS_TYPE fis_gMFI3Coeff1[] = { 0, 0, 6, 7 };
FIS_TYPE fis_gMFI3Coeff2[] = { 6, 7, 8 };
FIS_TYPE fis_gMFI3Coeff3[] = { 7, 8, 14, 14 };
FIS_TYPE* fis_gMFI3Coeff[] = { fis_gMFI3Coeff1, fis_gMFI3Coeff2, fis_gMFI3Coeff3 };
FIS_TYPE fis_gMFI4Coeff1[] = { 0, 0, 400, 600 };
FIS_TYPE fis_gMFI4Coeff2[] = { 400, 700, 1000 };
FIS_TYPE fis_gMFI4Coeff3[] = { 800, 1000, 1500, 1500 };
FIS_TYPE* fis_gMFI4Coeff[] = { fis_gMFI4Coeff1, fis_gMFI4Coeff2, fis_gMFI4Coeff3 };
FIS_TYPE** fis_gMFICoeff[] = { fis_gMFI0Coeff, fis_gMFI1Coeff, fis_gMFI2Coeff, fis_gMFI3Coeff, fis_gMFI4Coeff };

// Coefficients for the Output Member Functions
FIS_TYPE fis_gMFO0Coeff1[] = { -0.4, 0, 0.4 };
FIS_TYPE fis_gMFO0Coeff2[] = { 0.6, 1, 1.6 };
FIS_TYPE* fis_gMFO0Coeff[] = { fis_gMFO0Coeff1, fis_gMFO0Coeff2 };
FIS_TYPE fis_gMFO1Coeff1[] = { -0.4, 0, 0.4 };
FIS_TYPE fis_gMFO1Coeff2[] = { 0.6, 1, 1.6 };
FIS_TYPE* fis_gMFO1Coeff[] = { fis_gMFO1Coeff1, fis_gMFO1Coeff2 };
FIS_TYPE fis_gMFO2Coeff1[] = { -0.4, 0, 0.4 };
FIS_TYPE fis_gMFO2Coeff2[] = { 0.6, 1, 1.6 };
FIS_TYPE* fis_gMFO2Coeff[] = { fis_gMFO2Coeff1, fis_gMFO2Coeff2 };
FIS_TYPE fis_gMFO3Coeff1[] = { -0.4, 0, 0.4 };
FIS_TYPE fis_gMFO3Coeff2[] = { 0.6, 1, 1.6 };
FIS_TYPE* fis_gMFO3Coeff[] = { fis_gMFO3Coeff1, fis_gMFO3Coeff2 };
FIS_TYPE fis_gMFO4Coeff1[] = { -0.4, 0, 0.4 };
FIS_TYPE fis_gMFO4Coeff2[] = { 0.6, 1, 1.6 };
FIS_TYPE* fis_gMFO4Coeff[] = { fis_gMFO4Coeff1, fis_gMFO4Coeff2 };
FIS_TYPE** fis_gMFOCoeff[] = { fis_gMFO0Coeff, fis_gMFO1Coeff, fis_gMFO2Coeff, fis_gMFO3Coeff, fis_gMFO4Coeff };

// Input membership function set
int fis_gMFI0[] = { 0, 1, 0 };
int fis_gMFI1[] = { 0, 1, 0 };
int fis_gMFI2[] = { 0, 1, 0 };
int fis_gMFI3[] = { 0, 1, 0 };
int fis_gMFI4[] = { 0, 1, 0 };
int* fis_gMFI[] = { fis_gMFI0, fis_gMFI1, fis_gMFI2, fis_gMFI3, fis_gMFI4 };

// Output membership function set
int fis_gMFO0[] = { 1, 1 };
int fis_gMFO1[] = { 1, 1 };
int fis_gMFO2[] = { 1, 1 };
int fis_gMFO3[] = { 1, 1 };
int fis_gMFO4[] = { 1, 1 };
int* fis_gMFO[] = { fis_gMFO0, fis_gMFO1, fis_gMFO2, fis_gMFO3, fis_gMFO4 };

// Rule Weights
FIS_TYPE fis_gRWeight[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Type
int fis_gRType[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Inputs
int fis_gRI0[] = { 1, 1, 1, 1, 1 };
int fis_gRI1[] = { 1, 1, 1, 1, 2 };
int fis_gRI2[] = { 1, 1, 1, 1, 3 };
int fis_gRI3[] = { 1, 1, 1, 2, 1 };
int fis_gRI4[] = { 1, 1, 1, 2, 2 };
int fis_gRI5[] = { 1, 1, 1, 2, 3 };
int fis_gRI6[] = { 1, 1, 1, 3, 1 };
int fis_gRI7[] = { 1, 1, 1, 3, 2 };
int fis_gRI8[] = { 1, 1, 1, 3, 3 };
int fis_gRI9[] = { 1, 1, 2, 1, 1 };
int fis_gRI10[] = { 1, 1, 2, 1, 2 };
int fis_gRI11[] = { 1, 1, 2, 1, 3 };
int fis_gRI12[] = { 1, 1, 2, 2, 1 };
int fis_gRI13[] = { 1, 1, 2, 2, 2 };
int fis_gRI14[] = { 1, 1, 2, 2, 3 };
int fis_gRI15[] = { 1, 1, 2, 3, 1 };
int fis_gRI16[] = { 1, 1, 2, 3, 2 };
int fis_gRI17[] = { 1, 1, 2, 3, 3 };
int fis_gRI18[] = { 1, 1, 3, 1, 1 };
int fis_gRI19[] = { 1, 1, 3, 1, 2 };
int fis_gRI20[] = { 1, 1, 3, 1, 3 };
int fis_gRI21[] = { 1, 1, 3, 2, 1 };
int fis_gRI22[] = { 1, 1, 3, 2, 2 };
int fis_gRI23[] = { 1, 1, 3, 2, 3 };
int fis_gRI24[] = { 1, 1, 3, 3, 1 };
int fis_gRI25[] = { 1, 1, 3, 3, 2 };
int fis_gRI26[] = { 1, 1, 3, 3, 3 };
int fis_gRI27[] = { 1, 2, 1, 1, 1 };
int fis_gRI28[] = { 1, 2, 1, 1, 2 };
int fis_gRI29[] = { 1, 2, 1, 1, 3 };
int fis_gRI30[] = { 1, 2, 1, 2, 1 };
int fis_gRI31[] = { 1, 2, 1, 2, 2 };
int fis_gRI32[] = { 1, 2, 1, 2, 3 };
int fis_gRI33[] = { 1, 2, 1, 3, 1 };
int fis_gRI34[] = { 1, 2, 1, 3, 2 };
int fis_gRI35[] = { 1, 2, 1, 3, 3 };
int fis_gRI36[] = { 1, 2, 2, 1, 1 };
int fis_gRI37[] = { 1, 2, 2, 1, 2 };
int fis_gRI38[] = { 1, 2, 2, 1, 3 };
int fis_gRI39[] = { 1, 2, 2, 2, 1 };
int fis_gRI40[] = { 1, 2, 2, 2, 2 };
int fis_gRI41[] = { 1, 2, 2, 2, 3 };
int fis_gRI42[] = { 1, 2, 2, 3, 1 };
int fis_gRI43[] = { 1, 2, 2, 3, 2 };
int fis_gRI44[] = { 1, 2, 2, 3, 3 };
int fis_gRI45[] = { 1, 2, 3, 1, 1 };
int fis_gRI46[] = { 1, 2, 3, 1, 2 };
int fis_gRI47[] = { 1, 2, 3, 1, 3 };
int fis_gRI48[] = { 1, 2, 3, 2, 1 };
int fis_gRI49[] = { 1, 2, 3, 2, 2 };
int fis_gRI50[] = { 1, 2, 3, 2, 3 };
int fis_gRI51[] = { 1, 2, 3, 3, 1 };
int fis_gRI52[] = { 1, 2, 3, 3, 2 };
int fis_gRI53[] = { 1, 2, 3, 3, 3 };
int fis_gRI54[] = { 1, 3, 1, 1, 1 };
int fis_gRI55[] = { 1, 3, 1, 1, 2 };
int fis_gRI56[] = { 1, 3, 1, 1, 3 };
int fis_gRI57[] = { 1, 3, 1, 2, 1 };
int fis_gRI58[] = { 1, 3, 1, 2, 2 };
int fis_gRI59[] = { 1, 3, 1, 2, 3 };
int fis_gRI60[] = { 1, 3, 1, 3, 1 };
int fis_gRI61[] = { 1, 3, 1, 3, 2 };
int fis_gRI62[] = { 1, 3, 1, 3, 3 };
int fis_gRI63[] = { 1, 3, 2, 1, 1 };
int fis_gRI64[] = { 1, 3, 2, 1, 2 };
int fis_gRI65[] = { 1, 3, 2, 1, 3 };
int fis_gRI66[] = { 1, 3, 2, 2, 1 };
int fis_gRI67[] = { 1, 3, 2, 2, 2 };
int fis_gRI68[] = { 1, 3, 2, 2, 3 };
int fis_gRI69[] = { 1, 3, 2, 3, 1 };
int fis_gRI70[] = { 1, 3, 2, 3, 2 };
int fis_gRI71[] = { 1, 3, 2, 3, 3 };
int fis_gRI72[] = { 1, 3, 3, 1, 1 };
int fis_gRI73[] = { 1, 3, 3, 1, 2 };
int fis_gRI74[] = { 1, 3, 3, 1, 3 };
int fis_gRI75[] = { 1, 3, 3, 2, 1 };
int fis_gRI76[] = { 1, 3, 3, 2, 2 };
int fis_gRI77[] = { 1, 3, 3, 2, 3 };
int fis_gRI78[] = { 1, 3, 3, 3, 1 };
int fis_gRI79[] = { 1, 3, 3, 3, 2 };
int fis_gRI80[] = { 1, 3, 3, 3, 3 };
int fis_gRI81[] = { 2, 1, 1, 1, 1 };
int fis_gRI82[] = { 2, 1, 1, 1, 2 };
int fis_gRI83[] = { 2, 1, 1, 1, 3 };
int fis_gRI84[] = { 2, 1, 1, 2, 1 };
int fis_gRI85[] = { 2, 1, 1, 2, 2 };
int fis_gRI86[] = { 2, 1, 1, 2, 3 };
int fis_gRI87[] = { 2, 1, 1, 3, 1 };
int fis_gRI88[] = { 2, 1, 1, 3, 2 };
int fis_gRI89[] = { 2, 1, 1, 3, 3 };
int fis_gRI90[] = { 2, 1, 2, 1, 1 };
int fis_gRI91[] = { 2, 1, 2, 1, 2 };
int fis_gRI92[] = { 2, 1, 2, 1, 3 };
int fis_gRI93[] = { 2, 1, 2, 2, 1 };
int fis_gRI94[] = { 2, 1, 2, 2, 2 };
int fis_gRI95[] = { 2, 1, 2, 2, 3 };
int fis_gRI96[] = { 2, 1, 2, 3, 1 };
int fis_gRI97[] = { 2, 1, 2, 3, 2 };
int fis_gRI98[] = { 2, 1, 2, 3, 3 };
int fis_gRI99[] = { 2, 1, 3, 1, 1 };
int fis_gRI100[] = { 2, 1, 3, 1, 2 };
int fis_gRI101[] = { 2, 1, 3, 1, 3 };
int fis_gRI102[] = { 2, 1, 3, 2, 1 };
int fis_gRI103[] = { 2, 1, 3, 2, 2 };
int fis_gRI104[] = { 2, 1, 3, 2, 3 };
int fis_gRI105[] = { 2, 1, 3, 3, 1 };
int fis_gRI106[] = { 2, 1, 3, 3, 2 };
int fis_gRI107[] = { 2, 1, 3, 3, 3 };
int fis_gRI108[] = { 2, 2, 1, 1, 1 };
int fis_gRI109[] = { 2, 2, 1, 1, 2 };
int fis_gRI110[] = { 2, 2, 1, 1, 3 };
int fis_gRI111[] = { 2, 2, 1, 2, 1 };
int fis_gRI112[] = { 2, 2, 1, 2, 2 };
int fis_gRI113[] = { 2, 2, 1, 2, 3 };
int fis_gRI114[] = { 2, 2, 1, 3, 1 };
int fis_gRI115[] = { 2, 2, 1, 3, 2 };
int fis_gRI116[] = { 2, 2, 1, 3, 3 };
int fis_gRI117[] = { 2, 2, 2, 1, 1 };
int fis_gRI118[] = { 2, 2, 2, 1, 2 };
int fis_gRI119[] = { 2, 2, 2, 1, 3 };
int fis_gRI120[] = { 2, 2, 2, 2, 1 };
int fis_gRI121[] = { 2, 2, 2, 2, 2 };
int fis_gRI122[] = { 2, 2, 2, 2, 3 };
int fis_gRI123[] = { 2, 2, 2, 3, 1 };
int fis_gRI124[] = { 2, 2, 2, 3, 2 };
int fis_gRI125[] = { 2, 2, 2, 3, 3 };
int fis_gRI126[] = { 2, 2, 3, 1, 1 };
int fis_gRI127[] = { 2, 2, 3, 1, 2 };
int fis_gRI128[] = { 2, 2, 3, 1, 3 };
int fis_gRI129[] = { 2, 2, 3, 2, 1 };
int fis_gRI130[] = { 2, 2, 3, 2, 2 };
int fis_gRI131[] = { 2, 2, 3, 2, 3 };
int fis_gRI132[] = { 2, 2, 3, 3, 1 };
int fis_gRI133[] = { 2, 2, 3, 3, 2 };
int fis_gRI134[] = { 2, 2, 3, 3, 3 };
int fis_gRI135[] = { 2, 3, 1, 1, 1 };
int fis_gRI136[] = { 2, 3, 1, 1, 2 };
int fis_gRI137[] = { 2, 3, 1, 1, 3 };
int fis_gRI138[] = { 2, 3, 1, 2, 1 };
int fis_gRI139[] = { 2, 3, 1, 2, 2 };
int fis_gRI140[] = { 2, 3, 1, 2, 3 };
int fis_gRI141[] = { 2, 3, 1, 3, 1 };
int fis_gRI142[] = { 2, 3, 1, 3, 2 };
int fis_gRI143[] = { 2, 3, 1, 3, 3 };
int fis_gRI144[] = { 2, 3, 2, 1, 1 };
int fis_gRI145[] = { 2, 3, 2, 1, 2 };
int fis_gRI146[] = { 2, 3, 2, 1, 3 };
int fis_gRI147[] = { 2, 3, 2, 2, 1 };
int fis_gRI148[] = { 2, 3, 2, 2, 2 };
int fis_gRI149[] = { 2, 3, 2, 2, 3 };
int fis_gRI150[] = { 2, 3, 2, 3, 1 };
int fis_gRI151[] = { 2, 3, 2, 3, 2 };
int fis_gRI152[] = { 2, 3, 2, 3, 3 };
int fis_gRI153[] = { 2, 3, 3, 1, 1 };
int fis_gRI154[] = { 2, 3, 3, 1, 2 };
int fis_gRI155[] = { 2, 3, 3, 1, 3 };
int fis_gRI156[] = { 2, 3, 3, 2, 1 };
int fis_gRI157[] = { 2, 3, 3, 2, 2 };
int fis_gRI158[] = { 2, 3, 3, 2, 3 };
int fis_gRI159[] = { 2, 3, 3, 3, 1 };
int fis_gRI160[] = { 2, 3, 3, 3, 2 };
int fis_gRI161[] = { 2, 3, 3, 3, 3 };
int fis_gRI162[] = { 3, 1, 1, 1, 1 };
int fis_gRI163[] = { 3, 1, 1, 1, 2 };
int fis_gRI164[] = { 3, 1, 1, 1, 3 };
int fis_gRI165[] = { 3, 1, 1, 2, 1 };
int fis_gRI166[] = { 3, 1, 1, 2, 2 };
int fis_gRI167[] = { 3, 1, 1, 2, 3 };
int fis_gRI168[] = { 3, 1, 1, 3, 1 };
int fis_gRI169[] = { 3, 1, 1, 3, 2 };
int fis_gRI170[] = { 3, 1, 1, 3, 3 };
int fis_gRI171[] = { 3, 1, 2, 1, 1 };
int fis_gRI172[] = { 3, 1, 2, 1, 2 };
int fis_gRI173[] = { 3, 1, 2, 1, 3 };
int fis_gRI174[] = { 3, 1, 2, 2, 1 };
int fis_gRI175[] = { 3, 1, 2, 2, 2 };
int fis_gRI176[] = { 3, 1, 2, 2, 3 };
int fis_gRI177[] = { 3, 1, 2, 3, 1 };
int fis_gRI178[] = { 3, 1, 2, 3, 2 };
int fis_gRI179[] = { 3, 1, 2, 3, 3 };
int fis_gRI180[] = { 3, 1, 3, 1, 1 };
int fis_gRI181[] = { 3, 1, 3, 1, 2 };
int fis_gRI182[] = { 3, 1, 3, 1, 3 };
int fis_gRI183[] = { 3, 1, 3, 2, 1 };
int fis_gRI184[] = { 3, 1, 3, 2, 2 };
int fis_gRI185[] = { 3, 1, 3, 2, 3 };
int fis_gRI186[] = { 3, 1, 3, 3, 1 };
int fis_gRI187[] = { 3, 1, 3, 3, 2 };
int fis_gRI188[] = { 3, 1, 3, 3, 3 };
int fis_gRI189[] = { 3, 2, 1, 1, 1 };
int fis_gRI190[] = { 3, 2, 1, 1, 2 };
int fis_gRI191[] = { 3, 2, 1, 1, 3 };
int fis_gRI192[] = { 3, 2, 1, 2, 1 };
int fis_gRI193[] = { 3, 2, 1, 2, 2 };
int fis_gRI194[] = { 3, 2, 1, 2, 3 };
int fis_gRI195[] = { 3, 2, 1, 3, 1 };
int fis_gRI196[] = { 3, 2, 1, 3, 2 };
int fis_gRI197[] = { 3, 2, 1, 3, 3 };
int fis_gRI198[] = { 3, 2, 2, 1, 1 };
int fis_gRI199[] = { 3, 2, 2, 1, 2 };
int fis_gRI200[] = { 3, 2, 2, 1, 3 };
int fis_gRI201[] = { 3, 2, 2, 2, 1 };
int fis_gRI202[] = { 3, 2, 2, 2, 2 };
int fis_gRI203[] = { 3, 2, 2, 2, 3 };
int fis_gRI204[] = { 3, 2, 2, 3, 1 };
int fis_gRI205[] = { 3, 2, 2, 3, 2 };
int fis_gRI206[] = { 3, 2, 2, 3, 3 };
int fis_gRI207[] = { 3, 2, 3, 1, 1 };
int fis_gRI208[] = { 3, 2, 3, 1, 2 };
int fis_gRI209[] = { 3, 2, 3, 1, 3 };
int fis_gRI210[] = { 3, 2, 3, 2, 1 };
int fis_gRI211[] = { 3, 2, 3, 2, 2 };
int fis_gRI212[] = { 3, 2, 3, 2, 3 };
int fis_gRI213[] = { 3, 2, 3, 3, 1 };
int fis_gRI214[] = { 3, 2, 3, 3, 2 };
int fis_gRI215[] = { 3, 2, 3, 3, 3 };
int fis_gRI216[] = { 3, 3, 1, 1, 1 };
int fis_gRI217[] = { 3, 3, 1, 1, 2 };
int fis_gRI218[] = { 3, 3, 1, 1, 3 };
int fis_gRI219[] = { 3, 3, 1, 2, 1 };
int fis_gRI220[] = { 3, 3, 1, 2, 2 };
int fis_gRI221[] = { 3, 3, 1, 2, 3 };
int fis_gRI222[] = { 3, 3, 1, 3, 1 };
int fis_gRI223[] = { 3, 3, 1, 3, 2 };
int fis_gRI224[] = { 3, 3, 1, 3, 3 };
int fis_gRI225[] = { 3, 3, 2, 1, 1 };
int fis_gRI226[] = { 3, 3, 2, 1, 2 };
int fis_gRI227[] = { 3, 3, 2, 1, 3 };
int fis_gRI228[] = { 3, 3, 2, 2, 1 };
int fis_gRI229[] = { 3, 3, 2, 2, 2 };
int fis_gRI230[] = { 3, 3, 2, 2, 3 };
int fis_gRI231[] = { 3, 3, 2, 3, 1 };
int fis_gRI232[] = { 3, 3, 2, 3, 2 };
int fis_gRI233[] = { 3, 3, 2, 3, 3 };
int fis_gRI234[] = { 3, 3, 3, 1, 1 };
int fis_gRI235[] = { 3, 3, 3, 1, 2 };
int fis_gRI236[] = { 3, 3, 3, 1, 3 };
int fis_gRI237[] = { 3, 3, 3, 2, 1 };
int fis_gRI238[] = { 3, 3, 3, 2, 2 };
int fis_gRI239[] = { 3, 3, 3, 2, 3 };
int fis_gRI240[] = { 3, 3, 3, 3, 1 };
int fis_gRI241[] = { 3, 3, 3, 3, 2 };
int fis_gRI242[] = { 3, 3, 3, 3, 3 };
int* fis_gRI[] = { fis_gRI0, fis_gRI1, fis_gRI2, fis_gRI3, fis_gRI4, fis_gRI5, fis_gRI6, fis_gRI7, fis_gRI8, fis_gRI9, fis_gRI10, fis_gRI11, fis_gRI12, fis_gRI13, fis_gRI14, fis_gRI15, fis_gRI16, fis_gRI17, fis_gRI18, fis_gRI19, fis_gRI20, fis_gRI21, fis_gRI22, fis_gRI23, fis_gRI24, fis_gRI25, fis_gRI26, fis_gRI27, fis_gRI28, fis_gRI29, fis_gRI30, fis_gRI31, fis_gRI32, fis_gRI33, fis_gRI34, fis_gRI35, fis_gRI36, fis_gRI37, fis_gRI38, fis_gRI39, fis_gRI40, fis_gRI41, fis_gRI42, fis_gRI43, fis_gRI44, fis_gRI45, fis_gRI46, fis_gRI47, fis_gRI48, fis_gRI49, fis_gRI50, fis_gRI51, fis_gRI52, fis_gRI53, fis_gRI54, fis_gRI55, fis_gRI56, fis_gRI57, fis_gRI58, fis_gRI59, fis_gRI60, fis_gRI61, fis_gRI62, fis_gRI63, fis_gRI64, fis_gRI65, fis_gRI66, fis_gRI67, fis_gRI68, fis_gRI69, fis_gRI70, fis_gRI71, fis_gRI72, fis_gRI73, fis_gRI74, fis_gRI75, fis_gRI76, fis_gRI77, fis_gRI78, fis_gRI79, fis_gRI80, fis_gRI81, fis_gRI82, fis_gRI83, fis_gRI84, fis_gRI85, fis_gRI86, fis_gRI87, fis_gRI88, fis_gRI89, fis_gRI90, fis_gRI91, fis_gRI92, fis_gRI93, fis_gRI94, fis_gRI95, fis_gRI96, fis_gRI97, fis_gRI98, fis_gRI99, fis_gRI100, fis_gRI101, fis_gRI102, fis_gRI103, fis_gRI104, fis_gRI105, fis_gRI106, fis_gRI107, fis_gRI108, fis_gRI109, fis_gRI110, fis_gRI111, fis_gRI112, fis_gRI113, fis_gRI114, fis_gRI115, fis_gRI116, fis_gRI117, fis_gRI118, fis_gRI119, fis_gRI120, fis_gRI121, fis_gRI122, fis_gRI123, fis_gRI124, fis_gRI125, fis_gRI126, fis_gRI127, fis_gRI128, fis_gRI129, fis_gRI130, fis_gRI131, fis_gRI132, fis_gRI133, fis_gRI134, fis_gRI135, fis_gRI136, fis_gRI137, fis_gRI138, fis_gRI139, fis_gRI140, fis_gRI141, fis_gRI142, fis_gRI143, fis_gRI144, fis_gRI145, fis_gRI146, fis_gRI147, fis_gRI148, fis_gRI149, fis_gRI150, fis_gRI151, fis_gRI152, fis_gRI153, fis_gRI154, fis_gRI155, fis_gRI156, fis_gRI157, fis_gRI158, fis_gRI159, fis_gRI160, fis_gRI161, fis_gRI162, fis_gRI163, fis_gRI164, fis_gRI165, fis_gRI166, fis_gRI167, fis_gRI168, fis_gRI169, fis_gRI170, fis_gRI171, fis_gRI172, fis_gRI173, fis_gRI174, fis_gRI175, fis_gRI176, fis_gRI177, fis_gRI178, fis_gRI179, fis_gRI180, fis_gRI181, fis_gRI182, fis_gRI183, fis_gRI184, fis_gRI185, fis_gRI186, fis_gRI187, fis_gRI188, fis_gRI189, fis_gRI190, fis_gRI191, fis_gRI192, fis_gRI193, fis_gRI194, fis_gRI195, fis_gRI196, fis_gRI197, fis_gRI198, fis_gRI199, fis_gRI200, fis_gRI201, fis_gRI202, fis_gRI203, fis_gRI204, fis_gRI205, fis_gRI206, fis_gRI207, fis_gRI208, fis_gRI209, fis_gRI210, fis_gRI211, fis_gRI212, fis_gRI213, fis_gRI214, fis_gRI215, fis_gRI216, fis_gRI217, fis_gRI218, fis_gRI219, fis_gRI220, fis_gRI221, fis_gRI222, fis_gRI223, fis_gRI224, fis_gRI225, fis_gRI226, fis_gRI227, fis_gRI228, fis_gRI229, fis_gRI230, fis_gRI231, fis_gRI232, fis_gRI233, fis_gRI234, fis_gRI235, fis_gRI236, fis_gRI237, fis_gRI238, fis_gRI239, fis_gRI240, fis_gRI241, fis_gRI242 };

// Rule Outputs
int fis_gRO0[] = { 2, 1, 2, 1, 2 };
int fis_gRO1[] = { 2, 1, 2, 1, 2 };
int fis_gRO2[] = { 2, 2, 2, 1, 2 };
int fis_gRO3[] = { 2, 1, 2, 1, 1 };
int fis_gRO4[] = { 2, 1, 2, 1, 1 };
int fis_gRO5[] = { 2, 2, 2, 1, 1 };
int fis_gRO6[] = { 2, 1, 2, 2, 1 };
int fis_gRO7[] = { 2, 1, 2, 2, 1 };
int fis_gRO8[] = { 2, 2, 2, 2, 1 };
int fis_gRO9[] = { 2, 1, 1, 1, 2 };
int fis_gRO10[] = { 2, 1, 1, 1, 2 };
int fis_gRO11[] = { 2, 2, 1, 1, 2 };
int fis_gRO12[] = { 2, 1, 1, 1, 1 };
int fis_gRO13[] = { 2, 1, 1, 1, 1 };
int fis_gRO14[] = { 2, 2, 1, 1, 1 };
int fis_gRO15[] = { 2, 1, 1, 2, 1 };
int fis_gRO16[] = { 2, 1, 1, 2, 1 };
int fis_gRO17[] = { 2, 2, 1, 2, 1 };
int fis_gRO18[] = { 2, 2, 1, 1, 2 };
int fis_gRO19[] = { 2, 2, 1, 1, 2 };
int fis_gRO20[] = { 2, 2, 1, 1, 2 };
int fis_gRO21[] = { 2, 2, 1, 1, 1 };
int fis_gRO22[] = { 2, 2, 1, 1, 1 };
int fis_gRO23[] = { 2, 2, 1, 1, 1 };
int fis_gRO24[] = { 2, 2, 1, 2, 1 };
int fis_gRO25[] = { 2, 2, 1, 2, 1 };
int fis_gRO26[] = { 2, 2, 1, 2, 1 };
int fis_gRO27[] = { 2, 1, 2, 1, 2 };
int fis_gRO28[] = { 2, 1, 2, 1, 2 };
int fis_gRO29[] = { 2, 2, 2, 1, 2 };
int fis_gRO30[] = { 2, 1, 2, 1, 1 };
int fis_gRO31[] = { 2, 1, 2, 1, 1 };
int fis_gRO32[] = { 2, 2, 2, 1, 1 };
int fis_gRO33[] = { 2, 1, 2, 2, 1 };
int fis_gRO34[] = { 2, 1, 2, 2, 1 };
int fis_gRO35[] = { 2, 2, 2, 2, 1 };
int fis_gRO36[] = { 2, 1, 1, 1, 2 };
int fis_gRO37[] = { 2, 1, 1, 1, 2 };
int fis_gRO38[] = { 2, 2, 1, 1, 2 };
int fis_gRO39[] = { 2, 1, 1, 1, 1 };
int fis_gRO40[] = { 1, 2, 1, 1, 1 };
int fis_gRO41[] = { 2, 2, 1, 1, 1 };
int fis_gRO42[] = { 2, 1, 1, 2, 1 };
int fis_gRO43[] = { 2, 1, 1, 2, 1 };
int fis_gRO44[] = { 2, 2, 1, 2, 1 };
int fis_gRO45[] = { 2, 2, 1, 1, 2 };
int fis_gRO46[] = { 2, 2, 1, 1, 2 };
int fis_gRO47[] = { 2, 2, 1, 1, 2 };
int fis_gRO48[] = { 2, 2, 1, 1, 1 };
int fis_gRO49[] = { 2, 2, 1, 1, 1 };
int fis_gRO50[] = { 2, 2, 1, 1, 1 };
int fis_gRO51[] = { 2, 2, 1, 2, 1 };
int fis_gRO52[] = { 2, 2, 1, 2, 1 };
int fis_gRO53[] = { 2, 2, 1, 2, 1 };
int fis_gRO54[] = { 2, 2, 2, 1, 2 };
int fis_gRO55[] = { 2, 2, 2, 1, 2 };
int fis_gRO56[] = { 2, 2, 2, 1, 2 };
int fis_gRO57[] = { 2, 2, 2, 1, 1 };
int fis_gRO58[] = { 2, 2, 2, 1, 1 };
int fis_gRO59[] = { 2, 2, 2, 1, 1 };
int fis_gRO60[] = { 2, 2, 2, 2, 1 };
int fis_gRO61[] = { 2, 2, 2, 2, 1 };
int fis_gRO62[] = { 2, 2, 2, 2, 1 };
int fis_gRO63[] = { 2, 2, 1, 1, 2 };
int fis_gRO64[] = { 2, 2, 1, 1, 2 };
int fis_gRO65[] = { 2, 2, 1, 1, 2 };
int fis_gRO66[] = { 2, 2, 1, 1, 1 };
int fis_gRO67[] = { 2, 2, 1, 1, 1 };
int fis_gRO68[] = { 2, 2, 1, 1, 1 };
int fis_gRO69[] = { 2, 2, 1, 2, 1 };
int fis_gRO70[] = { 2, 2, 1, 2, 1 };
int fis_gRO71[] = { 2, 2, 1, 2, 1 };
int fis_gRO72[] = { 2, 2, 1, 1, 2 };
int fis_gRO73[] = { 2, 2, 1, 1, 2 };
int fis_gRO74[] = { 2, 2, 1, 1, 2 };
int fis_gRO75[] = { 2, 2, 1, 1, 1 };
int fis_gRO76[] = { 2, 2, 1, 1, 1 };
int fis_gRO77[] = { 2, 2, 1, 1, 1 };
int fis_gRO78[] = { 2, 2, 1, 2, 1 };
int fis_gRO79[] = { 2, 2, 1, 2, 1 };
int fis_gRO80[] = { 2, 2, 1, 2, 1 };
int fis_gRO81[] = { 1, 1, 2, 1, 2 };
int fis_gRO82[] = { 1, 1, 2, 1, 2 };
int fis_gRO83[] = { 2, 2, 2, 1, 2 };
int fis_gRO84[] = { 1, 1, 2, 1, 1 };
int fis_gRO85[] = { 1, 1, 2, 1, 1 };
int fis_gRO86[] = { 2, 2, 2, 2, 1 };
int fis_gRO87[] = { 1, 1, 2, 2, 1 };
int fis_gRO88[] = { 1, 1, 2, 2, 1 };
int fis_gRO89[] = { 2, 2, 2, 2, 1 };
int fis_gRO90[] = { 1, 1, 1, 1, 2 };
int fis_gRO91[] = { 1, 1, 1, 1, 2 };
int fis_gRO92[] = { 2, 2, 1, 1, 2 };
int fis_gRO93[] = { 1, 1, 1, 1, 1 };
int fis_gRO94[] = { 1, 1, 1, 1, 1 };
int fis_gRO95[] = { 2, 2, 1, 1, 1 };
int fis_gRO96[] = { 1, 1, 1, 2, 1 };
int fis_gRO97[] = { 1, 1, 1, 2, 1 };
int fis_gRO98[] = { 2, 2, 1, 2, 1 };
int fis_gRO99[] = { 2, 2, 1, 1, 2 };
int fis_gRO100[] = { 2, 2, 1, 1, 2 };
int fis_gRO101[] = { 2, 2, 1, 1, 2 };
int fis_gRO102[] = { 2, 2, 1, 1, 1 };
int fis_gRO103[] = { 2, 2, 1, 1, 1 };
int fis_gRO104[] = { 2, 2, 1, 1, 1 };
int fis_gRO105[] = { 2, 2, 1, 2, 1 };
int fis_gRO106[] = { 2, 2, 1, 2, 1 };
int fis_gRO107[] = { 2, 2, 1, 2, 1 };
int fis_gRO108[] = { 1, 1, 2, 1, 2 };
int fis_gRO109[] = { 1, 1, 2, 1, 2 };
int fis_gRO110[] = { 2, 2, 2, 1, 2 };
int fis_gRO111[] = { 1, 1, 2, 1, 1 };
int fis_gRO112[] = { 1, 1, 2, 1, 1 };
int fis_gRO113[] = { 2, 2, 2, 1, 1 };
int fis_gRO114[] = { 1, 1, 2, 2, 1 };
int fis_gRO115[] = { 1, 1, 2, 2, 1 };
int fis_gRO116[] = { 2, 2, 2, 2, 1 };
int fis_gRO117[] = { 1, 1, 1, 1, 2 };
int fis_gRO118[] = { 1, 1, 1, 1, 2 };
int fis_gRO119[] = { 2, 2, 1, 1, 2 };
int fis_gRO120[] = { 1, 1, 1, 1, 1 };
int fis_gRO121[] = { 1, 1, 1, 1, 1 };
int fis_gRO122[] = { 2, 2, 1, 1, 1 };
int fis_gRO123[] = { 1, 1, 1, 2, 1 };
int fis_gRO124[] = { 1, 1, 1, 2, 1 };
int fis_gRO125[] = { 2, 2, 1, 2, 1 };
int fis_gRO126[] = { 2, 2, 1, 1, 2 };
int fis_gRO127[] = { 2, 2, 1, 1, 2 };
int fis_gRO128[] = { 2, 2, 1, 1, 2 };
int fis_gRO129[] = { 2, 2, 1, 1, 1 };
int fis_gRO130[] = { 2, 2, 1, 1, 1 };
int fis_gRO131[] = { 2, 2, 1, 1, 1 };
int fis_gRO132[] = { 2, 2, 1, 2, 1 };
int fis_gRO133[] = { 2, 2, 1, 2, 1 };
int fis_gRO134[] = { 2, 2, 1, 2, 1 };
int fis_gRO135[] = { 2, 2, 2, 1, 2 };
int fis_gRO136[] = { 2, 2, 2, 1, 2 };
int fis_gRO137[] = { 2, 2, 2, 1, 2 };
int fis_gRO138[] = { 2, 2, 2, 1, 1 };
int fis_gRO139[] = { 2, 2, 2, 1, 1 };
int fis_gRO140[] = { 2, 2, 2, 1, 1 };
int fis_gRO141[] = { 2, 2, 2, 2, 1 };
int fis_gRO142[] = { 2, 2, 2, 2, 1 };
int fis_gRO143[] = { 2, 2, 2, 2, 1 };
int fis_gRO144[] = { 2, 2, 1, 1, 2 };
int fis_gRO145[] = { 2, 2, 1, 1, 2 };
int fis_gRO146[] = { 2, 1, 1, 1, 2 };
int fis_gRO147[] = { 2, 2, 1, 1, 1 };
int fis_gRO148[] = { 2, 2, 1, 1, 1 };
int fis_gRO149[] = { 2, 2, 1, 1, 1 };
int fis_gRO150[] = { 2, 2, 1, 2, 1 };
int fis_gRO151[] = { 2, 2, 1, 2, 1 };
int fis_gRO152[] = { 2, 2, 1, 2, 1 };
int fis_gRO153[] = { 2, 2, 1, 1, 2 };
int fis_gRO154[] = { 2, 2, 1, 1, 2 };
int fis_gRO155[] = { 2, 1, 1, 1, 2 };
int fis_gRO156[] = { 2, 2, 1, 1, 1 };
int fis_gRO157[] = { 2, 2, 1, 1, 1 };
int fis_gRO158[] = { 2, 2, 1, 1, 1 };
int fis_gRO159[] = { 2, 2, 1, 2, 1 };
int fis_gRO160[] = { 2, 2, 1, 2, 1 };
int fis_gRO161[] = { 2, 2, 1, 2, 1 };
int fis_gRO162[] = { 1, 2, 2, 1, 2 };
int fis_gRO163[] = { 1, 2, 2, 1, 2 };
int fis_gRO164[] = { 2, 2, 2, 1, 2 };
int fis_gRO165[] = { 1, 2, 2, 1, 1 };
int fis_gRO166[] = { 1, 2, 2, 1, 1 };
int fis_gRO167[] = { 2, 2, 2, 1, 1 };
int fis_gRO168[] = { 1, 2, 2, 2, 1 };
int fis_gRO169[] = { 1, 2, 2, 2, 1 };
int fis_gRO170[] = { 2, 2, 2, 2, 1 };
int fis_gRO171[] = { 1, 2, 1, 1, 2 };
int fis_gRO172[] = { 1, 2, 1, 1, 2 };
int fis_gRO173[] = { 2, 2, 1, 1, 2 };
int fis_gRO174[] = { 1, 2, 1, 1, 1 };
int fis_gRO175[] = { 1, 2, 1, 1, 1 };
int fis_gRO176[] = { 2, 2, 1, 1, 1 };
int fis_gRO177[] = { 1, 2, 1, 2, 1 };
int fis_gRO178[] = { 1, 2, 1, 2, 1 };
int fis_gRO179[] = { 2, 2, 1, 2, 1 };
int fis_gRO180[] = { 2, 2, 1, 1, 2 };
int fis_gRO181[] = { 2, 2, 1, 1, 2 };
int fis_gRO182[] = { 2, 2, 1, 1, 2 };
int fis_gRO183[] = { 2, 2, 1, 1, 1 };
int fis_gRO184[] = { 2, 2, 1, 1, 1 };
int fis_gRO185[] = { 2, 2, 1, 1, 1 };
int fis_gRO186[] = { 2, 2, 1, 2, 1 };
int fis_gRO187[] = { 2, 2, 1, 2, 1 };
int fis_gRO188[] = { 2, 2, 1, 2, 1 };
int fis_gRO189[] = { 2, 2, 2, 1, 2 };
int fis_gRO190[] = { 2, 2, 2, 1, 2 };
int fis_gRO191[] = { 2, 2, 2, 1, 2 };
int fis_gRO192[] = { 1, 1, 2, 1, 1 };
int fis_gRO193[] = { 1, 1, 2, 1, 1 };
int fis_gRO194[] = { 2, 2, 2, 1, 1 };
int fis_gRO195[] = { 1, 1, 2, 2, 1 };
int fis_gRO196[] = { 1, 1, 2, 2, 1 };
int fis_gRO197[] = { 1, 1, 2, 2, 1 };
int fis_gRO198[] = { 1, 1, 1, 1, 2 };
int fis_gRO199[] = { 1, 1, 1, 1, 2 };
int fis_gRO200[] = { 2, 2, 1, 1, 2 };
int fis_gRO201[] = { 1, 1, 1, 1, 1 };
int fis_gRO202[] = { 1, 1, 1, 1, 1 };
int fis_gRO203[] = { 2, 2, 1, 1, 1 };
int fis_gRO204[] = { 1, 1, 1, 2, 1 };
int fis_gRO205[] = { 1, 1, 1, 2, 1 };
int fis_gRO206[] = { 2, 2, 1, 2, 1 };
int fis_gRO207[] = { 2, 2, 1, 1, 2 };
int fis_gRO208[] = { 2, 2, 1, 1, 2 };
int fis_gRO209[] = { 2, 2, 1, 1, 2 };
int fis_gRO210[] = { 2, 2, 1, 1, 1 };
int fis_gRO211[] = { 2, 2, 1, 1, 1 };
int fis_gRO212[] = { 2, 2, 1, 1, 1 };
int fis_gRO213[] = { 2, 2, 1, 2, 1 };
int fis_gRO214[] = { 2, 2, 1, 2, 1 };
int fis_gRO215[] = { 2, 2, 1, 2, 1 };
int fis_gRO216[] = { 2, 2, 2, 1, 2 };
int fis_gRO217[] = { 2, 2, 2, 1, 2 };
int fis_gRO218[] = { 2, 2, 2, 1, 2 };
int fis_gRO219[] = { 2, 2, 2, 1, 1 };
int fis_gRO220[] = { 2, 2, 2, 1, 1 };
int fis_gRO221[] = { 2, 2, 2, 1, 1 };
int fis_gRO222[] = { 2, 2, 2, 2, 1 };
int fis_gRO223[] = { 2, 2, 2, 2, 1 };
int fis_gRO224[] = { 2, 2, 2, 2, 1 };
int fis_gRO225[] = { 2, 2, 1, 1, 2 };
int fis_gRO226[] = { 2, 2, 1, 1, 2 };
int fis_gRO227[] = { 2, 2, 1, 1, 2 };
int fis_gRO228[] = { 2, 2, 1, 1, 1 };
int fis_gRO229[] = { 2, 2, 1, 1, 1 };
int fis_gRO230[] = { 2, 2, 1, 1, 1 };
int fis_gRO231[] = { 2, 2, 1, 2, 1 };
int fis_gRO232[] = { 2, 2, 1, 2, 1 };
int fis_gRO233[] = { 2, 2, 1, 2, 1 };
int fis_gRO234[] = { 2, 2, 1, 1, 2 };
int fis_gRO235[] = { 2, 2, 1, 1, 2 };
int fis_gRO236[] = { 2, 2, 1, 1, 2 };
int fis_gRO237[] = { 2, 2, 1, 1, 1 };
int fis_gRO238[] = { 2, 2, 1, 1, 1 };
int fis_gRO239[] = { 2, 2, 1, 1, 1 };
int fis_gRO240[] = { 2, 2, 1, 2, 1 };
int fis_gRO241[] = { 2, 2, 1, 2, 1 };
int fis_gRO242[] = { 2, 2, 1, 2, 1 };
int* fis_gRO[] = { fis_gRO0, fis_gRO1, fis_gRO2, fis_gRO3, fis_gRO4, fis_gRO5, fis_gRO6, fis_gRO7, fis_gRO8, fis_gRO9, fis_gRO10, fis_gRO11, fis_gRO12, fis_gRO13, fis_gRO14, fis_gRO15, fis_gRO16, fis_gRO17, fis_gRO18, fis_gRO19, fis_gRO20, fis_gRO21, fis_gRO22, fis_gRO23, fis_gRO24, fis_gRO25, fis_gRO26, fis_gRO27, fis_gRO28, fis_gRO29, fis_gRO30, fis_gRO31, fis_gRO32, fis_gRO33, fis_gRO34, fis_gRO35, fis_gRO36, fis_gRO37, fis_gRO38, fis_gRO39, fis_gRO40, fis_gRO41, fis_gRO42, fis_gRO43, fis_gRO44, fis_gRO45, fis_gRO46, fis_gRO47, fis_gRO48, fis_gRO49, fis_gRO50, fis_gRO51, fis_gRO52, fis_gRO53, fis_gRO54, fis_gRO55, fis_gRO56, fis_gRO57, fis_gRO58, fis_gRO59, fis_gRO60, fis_gRO61, fis_gRO62, fis_gRO63, fis_gRO64, fis_gRO65, fis_gRO66, fis_gRO67, fis_gRO68, fis_gRO69, fis_gRO70, fis_gRO71, fis_gRO72, fis_gRO73, fis_gRO74, fis_gRO75, fis_gRO76, fis_gRO77, fis_gRO78, fis_gRO79, fis_gRO80, fis_gRO81, fis_gRO82, fis_gRO83, fis_gRO84, fis_gRO85, fis_gRO86, fis_gRO87, fis_gRO88, fis_gRO89, fis_gRO90, fis_gRO91, fis_gRO92, fis_gRO93, fis_gRO94, fis_gRO95, fis_gRO96, fis_gRO97, fis_gRO98, fis_gRO99, fis_gRO100, fis_gRO101, fis_gRO102, fis_gRO103, fis_gRO104, fis_gRO105, fis_gRO106, fis_gRO107, fis_gRO108, fis_gRO109, fis_gRO110, fis_gRO111, fis_gRO112, fis_gRO113, fis_gRO114, fis_gRO115, fis_gRO116, fis_gRO117, fis_gRO118, fis_gRO119, fis_gRO120, fis_gRO121, fis_gRO122, fis_gRO123, fis_gRO124, fis_gRO125, fis_gRO126, fis_gRO127, fis_gRO128, fis_gRO129, fis_gRO130, fis_gRO131, fis_gRO132, fis_gRO133, fis_gRO134, fis_gRO135, fis_gRO136, fis_gRO137, fis_gRO138, fis_gRO139, fis_gRO140, fis_gRO141, fis_gRO142, fis_gRO143, fis_gRO144, fis_gRO145, fis_gRO146, fis_gRO147, fis_gRO148, fis_gRO149, fis_gRO150, fis_gRO151, fis_gRO152, fis_gRO153, fis_gRO154, fis_gRO155, fis_gRO156, fis_gRO157, fis_gRO158, fis_gRO159, fis_gRO160, fis_gRO161, fis_gRO162, fis_gRO163, fis_gRO164, fis_gRO165, fis_gRO166, fis_gRO167, fis_gRO168, fis_gRO169, fis_gRO170, fis_gRO171, fis_gRO172, fis_gRO173, fis_gRO174, fis_gRO175, fis_gRO176, fis_gRO177, fis_gRO178, fis_gRO179, fis_gRO180, fis_gRO181, fis_gRO182, fis_gRO183, fis_gRO184, fis_gRO185, fis_gRO186, fis_gRO187, fis_gRO188, fis_gRO189, fis_gRO190, fis_gRO191, fis_gRO192, fis_gRO193, fis_gRO194, fis_gRO195, fis_gRO196, fis_gRO197, fis_gRO198, fis_gRO199, fis_gRO200, fis_gRO201, fis_gRO202, fis_gRO203, fis_gRO204, fis_gRO205, fis_gRO206, fis_gRO207, fis_gRO208, fis_gRO209, fis_gRO210, fis_gRO211, fis_gRO212, fis_gRO213, fis_gRO214, fis_gRO215, fis_gRO216, fis_gRO217, fis_gRO218, fis_gRO219, fis_gRO220, fis_gRO221, fis_gRO222, fis_gRO223, fis_gRO224, fis_gRO225, fis_gRO226, fis_gRO227, fis_gRO228, fis_gRO229, fis_gRO230, fis_gRO231, fis_gRO232, fis_gRO233, fis_gRO234, fis_gRO235, fis_gRO236, fis_gRO237, fis_gRO238, fis_gRO239, fis_gRO240, fis_gRO241, fis_gRO242 };

// Input range Min
FIS_TYPE fis_gIMin[] = { 0, 0, 0, 0, 0 };

// Input range Max
FIS_TYPE fis_gIMax[] = { 200, 300, 40, 14, 1500 };

// Output range Min
FIS_TYPE fis_gOMin[] = { 0, 0, 0, 0, 0 };

// Output range Max
FIS_TYPE fis_gOMax[] = { 1, 1, 1, 1, 1 };

//***********************************************************************
// Data dependent support functions for Fuzzy Inference System
//***********************************************************************
FIS_TYPE fis_MF_out(FIS_TYPE** fuzzyRuleSet, FIS_TYPE x, int o) {
        FIS_TYPE mfOut;
        int r;

        for (r = 0; r < fis_gcR; ++r) {
                int index = fis_gRO[r][o];
                if (index > 0) {
                        index = index - 1;
                        mfOut = (fis_gMF[fis_gMFO[o][index]])(x, fis_gMFOCoeff[o][index]);
                } else if (index < 0) {
                        index = -index - 1;
                        mfOut = 1 - (fis_gMF[fis_gMFO[o][index]])(x, fis_gMFOCoeff[o][index]);
                } else {
                        mfOut = 0;
                }

                fuzzyRuleSet[0][r] = fis_min(mfOut, fuzzyRuleSet[1][r]);
        }
        return fis_array_operation(fuzzyRuleSet[0], fis_gcR, fis_max);
}

FIS_TYPE fis_defuzz_centroid(FIS_TYPE** fuzzyRuleSet, int o) {
        FIS_TYPE step = (fis_gOMax[o] - fis_gOMin[o]) / (FIS_RESOLUSION - 1);
        FIS_TYPE area = 0;
        FIS_TYPE momentum = 0;
        FIS_TYPE dist, slice;
        int i;

        // calculate the area under the curve formed by the MF outputs
        for (i = 0; i < FIS_RESOLUSION; ++i) {
                dist = fis_gOMin[o] + (step * i);
                slice = step * fis_MF_out(fuzzyRuleSet, dist, o);
                area += slice;
                momentum += slice * dist;
        }

        return ((area == 0) ? ((fis_gOMax[o] + fis_gOMin[o]) / 2) : (momentum / area));
}

//***********************************************************************
// Fuzzy Inference System
//***********************************************************************
void fis_evaluate() {
        FIS_TYPE fuzzyInput0[] = { 0, 0, 0 };
        FIS_TYPE fuzzyInput1[] = { 0, 0, 0 };
        FIS_TYPE fuzzyInput2[] = { 0, 0, 0 };
        FIS_TYPE fuzzyInput3[] = { 0, 0, 0 };
        FIS_TYPE fuzzyInput4[] = { 0, 0, 0 };
        FIS_TYPE* fuzzyInput[fis_gcI] = {
                fuzzyInput0,
                fuzzyInput1,
                fuzzyInput2,
                fuzzyInput3,
                fuzzyInput4,
        };
        FIS_TYPE fuzzyOutput0[] = { 0, 0 };
        FIS_TYPE fuzzyOutput1[] = { 0, 0 };
        FIS_TYPE fuzzyOutput2[] = { 0, 0 };
        FIS_TYPE fuzzyOutput3[] = { 0, 0 };
        FIS_TYPE fuzzyOutput4[] = { 0, 0 };
        FIS_TYPE* fuzzyOutput[fis_gcO] = {
                fuzzyOutput0,
                fuzzyOutput1,
                fuzzyOutput2,
                fuzzyOutput3,
                fuzzyOutput4,
        };
        FIS_TYPE fuzzyRules[fis_gcR] = { 0 };
        FIS_TYPE fuzzyFires[fis_gcR] = { 0 };
        FIS_TYPE* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
        FIS_TYPE sW = 0;

        // Transforming input to fuzzy Input
        int i, j, r, o;
        for (i = 0; i < fis_gcI; ++i) {
                for (j = 0; j < fis_gIMFCount[i]; ++j) {
                        fuzzyInput[i][j] =
                          (fis_gMF[fis_gMFI[i][j]])(g_fisInput[i], fis_gMFICoeff[i][j]);
                }
        }

        int index = 0;
        for (r = 0; r < fis_gcR; ++r) {
                if (fis_gRType[r] == 1) {
                        fuzzyFires[r] = FIS_MAX;
                        for (i = 0; i < fis_gcI; ++i) {
                                index = fis_gRI[r][i];
                                if (index > 0)
                                        fuzzyFires[r] = fis_min(fuzzyFires[r], fuzzyInput[i][index - 1]);
                                else if (index < 0)
                                        fuzzyFires[r] = fis_min(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                                else
                                        fuzzyFires[r] = fis_min(fuzzyFires[r], 1);
                        }
                } else {
                        fuzzyFires[r] = FIS_MIN;
                        for (i = 0; i < fis_gcI; ++i) {
                                index = fis_gRI[r][i];
                                if (index > 0)
                                        fuzzyFires[r] = fis_max(fuzzyFires[r], fuzzyInput[i][index - 1]);
                                else if (index < 0)
                                        fuzzyFires[r] = fis_max(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                                else
                                        fuzzyFires[r] = fis_max(fuzzyFires[r], 0);
                        }
                }

                fuzzyFires[r] = fis_gRWeight[r] * fuzzyFires[r];
                sW += fuzzyFires[r];
        }

        if (sW == 0) {
                for (o = 0; o < fis_gcO; ++o) {
                        g_fisOutput[o] = ((fis_gOMax[o] + fis_gOMin[o]) / 2);
                }
        } else {
                for (o = 0; o < fis_gcO; ++o) {
                        g_fisOutput[o] = fis_defuzz_centroid(fuzzyRuleSet, o);
                }
        }
}
