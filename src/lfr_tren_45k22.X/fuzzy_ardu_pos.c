#define FIS_TYPE float
//#define FIS_TYPE_INT8 unsigned int
//#define FIS_TYPE_INT16 unsigned int16

#define FIS_RESOLUSION 101
#define FIS_MIN_VALUE -3.4028235E+38
#define FIS_MAX_VALUE 3.4028235E+38

typedef FIS_TYPE(*_FIS_MF)(FIS_TYPE, FIS_TYPE*);
typedef FIS_TYPE(*_FIS_ARR_OP)(FIS_TYPE, FIS_TYPE);
typedef FIS_TYPE(*_FIS_ARR)(FIS_TYPE*, int, _FIS_ARR_OP);

#define max(x,y)  ((x > y) ? x : y) 
#define min(x,y)  ((x < y) ? x : y)

// Number of inputs to the fuzzy inference system
const int fis_gcI = 8;
// Number of outputs to the fuzzy inference system
const int fis_gcO = 1;
// Number of rules to the fuzzy inference system
const int fis_gcR = 17;

FIS_TYPE g_fisInput[fis_gcI]={100,100,100,100,100,100,100,100};
FIS_TYPE g_fisOutput[fis_gcO];

//#define fis_min(a,b) min(a,b)
//#define fis_max(a,b) max(a,b)

FIS_TYPE fis_min(FIS_TYPE a, FIS_TYPE b)
{
   
    //return   ((a1< b1) ? a1 : b1) ;
    if(a<b) return a;
    else return b;
}

FIS_TYPE fis_max(FIS_TYPE a, FIS_TYPE b)
{
    //return ((a> b) ? a : b);
    if(a>b) return a;
    else return b;
}

// Trapezoidal Member Function
FIS_TYPE fis_trapmf(FIS_TYPE x, FIS_TYPE* p)
{
    FIS_TYPE a = p[0], b = p[1], c = p[2], d = p[3];
    FIS_TYPE t1 = ((x <= c) ? 1 : ((d < x) ? 0 : ((c != d) ? ((d - x) / (d - c)) : 0)));
    FIS_TYPE t2 = ((b <= x) ? 1 : ((x < a) ? 0 : ((a != b) ? ((x - a) / (b - a)) : 0)));
    return (FIS_TYPE) min(t1, t2);
}

// Triangular Member Function
FIS_TYPE fis_trimf(FIS_TYPE x, FIS_TYPE* p)
{
    FIS_TYPE a = p[0], b = p[1], c = p[2];
    FIS_TYPE t1 = (x - a) / (b - a);
    FIS_TYPE t2 = (c - x) / (c - b);
    if ((a == b) && (b == c)) return (FIS_TYPE) (x == a);
    if (a == b) return (FIS_TYPE) (t2*(b <= x)*(x <= c));
    if (b == c) return (FIS_TYPE) (t1*(a <= x)*(x <= b));
    t1 = min(t1, t2);
    return (FIS_TYPE) max(t1, 0);
}

FIS_TYPE fis_array_operation(FIS_TYPE *array, int8 size, _FIS_ARR_OP pfnOp)
{
    int i;
    FIS_TYPE ret = 0;

    if (size == 0) return ret;
    if (size == 1) return array[0];

    ret = array[0];
    for (i = 1; i < size; i++)
    {
        ret = (*pfnOp)(ret, array[i]);
    }

    return ret;
}
//***********************************************************************
// Data for Fuzzy Inference System                                       
//***********************************************************************
// Pointers to the implementations of member functions
_FIS_MF fis_gMF[2] =
{
    fis_trapmf, fis_trimf
};

// Count of member function for each Input
int8 fis_gIMFCount[8] = { 2, 2, 2, 2, 2, 2, 2, 2 };

// Count of member function for each Output 
//int8 fis_gOMFCount[1] = { 17 };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFI0Coeff1[4] = { 0, 0, 130, 700 };
FIS_TYPE fis_gMFI0Coeff2[4] = { 130, 700, 800, 800 };
FIS_TYPE* fis_gMFI0Coeff[2] = { fis_gMFI0Coeff1, fis_gMFI0Coeff2 };
FIS_TYPE fis_gMFI1Coeff1[4] = { 0, 0, 130, 700 };
FIS_TYPE fis_gMFI1Coeff2[4] = { 130, 700, 800, 800 };
FIS_TYPE* fis_gMFI1Coeff[2] = { fis_gMFI1Coeff1, fis_gMFI1Coeff2 };
FIS_TYPE fis_gMFI2Coeff1[4] = { 0, 0, 130, 700 };
FIS_TYPE fis_gMFI2Coeff2[4] = { 130, 700, 800, 800 };
FIS_TYPE* fis_gMFI2Coeff[2] = { fis_gMFI2Coeff1, fis_gMFI2Coeff2 };
FIS_TYPE fis_gMFI3Coeff1[4] = { 0, 0, 130, 700 };
FIS_TYPE fis_gMFI3Coeff2[4] = { 130, 700, 800, 800 };
FIS_TYPE* fis_gMFI3Coeff[2] = { fis_gMFI3Coeff1, fis_gMFI3Coeff2 };
FIS_TYPE fis_gMFI4Coeff1[4] = { 0, 0, 130, 700 };
FIS_TYPE fis_gMFI4Coeff2[4] = { 130, 700, 800, 800 };
FIS_TYPE* fis_gMFI4Coeff[2] = { fis_gMFI4Coeff1, fis_gMFI4Coeff2 };
FIS_TYPE fis_gMFI5Coeff1[4] = { 0, 0, 130, 700 };
FIS_TYPE fis_gMFI5Coeff2[4] = { 130, 700, 800, 800 };
FIS_TYPE* fis_gMFI5Coeff[2] = { fis_gMFI5Coeff1, fis_gMFI5Coeff2 };
FIS_TYPE fis_gMFI6Coeff1[4] = { 0, 0, 130, 700 };
FIS_TYPE fis_gMFI6Coeff2[4] = { 130, 700, 800, 800 };
FIS_TYPE* fis_gMFI6Coeff[2] = { fis_gMFI6Coeff1, fis_gMFI6Coeff2 };
FIS_TYPE fis_gMFI7Coeff1[4] = { 0, 0, 130, 700 };
FIS_TYPE fis_gMFI7Coeff2[4] = { 130, 700, 800, 800 };
FIS_TYPE* fis_gMFI7Coeff[2] = { fis_gMFI7Coeff1, fis_gMFI7Coeff2 };
FIS_TYPE** fis_gMFICoeff[8] = { fis_gMFI0Coeff, fis_gMFI1Coeff, fis_gMFI2Coeff, fis_gMFI3Coeff, fis_gMFI4Coeff, fis_gMFI5Coeff, fis_gMFI6Coeff, fis_gMFI7Coeff };

// Coefficients for the Output Member Functions
FIS_TYPE fis_gMFO0Coeff1[3] = { 0, 0, 0 };
FIS_TYPE fis_gMFO0Coeff2[3] = { 1, 1, 1 };
FIS_TYPE fis_gMFO0Coeff3[3] = { 2, 2, 2 };
FIS_TYPE fis_gMFO0Coeff4[3] = { 3, 3, 3 };
FIS_TYPE fis_gMFO0Coeff5[3] = { 4, 4, 4 };
FIS_TYPE fis_gMFO0Coeff6[3] = { 5, 5, 5 };
FIS_TYPE fis_gMFO0Coeff7[3] = { 6, 6, 6 };
FIS_TYPE fis_gMFO0Coeff8[3] = { 7, 7, 7 };
FIS_TYPE fis_gMFO0Coeff9[3] = { 8, 8, 8 };
FIS_TYPE fis_gMFO0Coeff10[3] = { 9, 9, 9 };
FIS_TYPE fis_gMFO0Coeff11[3] = { 10, 10, 10 };
FIS_TYPE fis_gMFO0Coeff12[3] = { 11, 11, 11 };
FIS_TYPE fis_gMFO0Coeff13[3] = { 12, 12, 12 };
FIS_TYPE fis_gMFO0Coeff14[3] = { 13, 13, 13 };
FIS_TYPE fis_gMFO0Coeff15[3] = { 14, 14, 14 };
FIS_TYPE fis_gMFO0Coeff16[3] = { 15, 15, 15 };
FIS_TYPE fis_gMFO0Coeff17[3] = { 16, 16, 16 };
FIS_TYPE* fis_gMFO0Coeff[17] = { fis_gMFO0Coeff1, fis_gMFO0Coeff2, fis_gMFO0Coeff3, fis_gMFO0Coeff4, fis_gMFO0Coeff5, fis_gMFO0Coeff6, fis_gMFO0Coeff7, fis_gMFO0Coeff8, fis_gMFO0Coeff9, fis_gMFO0Coeff10, fis_gMFO0Coeff11, fis_gMFO0Coeff12, fis_gMFO0Coeff13, fis_gMFO0Coeff14, fis_gMFO0Coeff15, fis_gMFO0Coeff16, fis_gMFO0Coeff17 };
FIS_TYPE** fis_gMFOCoeff[] = { fis_gMFO0Coeff };

// Input membership function set
int fis_gMFI0[2] = { 0, 0 };
int fis_gMFI1[2] = { 0, 0 };
int fis_gMFI2[2] = { 0, 0 };
int fis_gMFI3[2] = { 0, 0 };
int fis_gMFI4[2] = { 0, 0 };
int fis_gMFI5[2] = { 0, 0 };
int fis_gMFI6[2] = { 0, 0 };
int fis_gMFI7[2] = { 0, 0 };
int* fis_gMFI[8] = { fis_gMFI0, fis_gMFI1, fis_gMFI2, fis_gMFI3, fis_gMFI4, fis_gMFI5, fis_gMFI6, fis_gMFI7};

// Output membership function set
int fis_gMFO0[17] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int* fis_gMFO[] = { fis_gMFO0};

// Rule Weights
//FIS_TYPE fis_gRWeight[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Type
int fis_gRType[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Inputs
int fis_gRI0[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
int fis_gRI1[] = { 2, 2, 2, 2, 2, 2, 2, 2 };
int fis_gRI2[] = { 1, 2, 2, 2, 2, 2, 2, 2 };
int fis_gRI3[] = { 1, 1, 2, 2, 2, 2, 2, 2 };
int fis_gRI4[] = { 1, 1, 1, 2, 2, 2, 2, 2 };
int fis_gRI5[] = { 2, 1, 1, 2, 2, 2, 2, 2 };
int fis_gRI6[] = { 2, 1, 1, 1, 2, 2, 2, 2 };
int fis_gRI7[] = { 2, 2, 1, 1, 2, 2, 2, 2 };
int fis_gRI8[] = { 2, 2, 1, 1, 1, 2, 2, 2 };
int fis_gRI9[] = { 2, 2, 2, 1, 1, 2, 2, 2 };
int fis_gRI10[] = { 2, 2, 2, 1, 1, 1, 2, 2 };
int fis_gRI11[] = { 2, 2, 2, 2, 1, 1, 2, 2 };
int fis_gRI12[] = { 2, 2, 2, 2, 1, 1, 1, 2 };
int fis_gRI13[] = { 2, 2, 2, 2, 2, 1, 1, 2 };
int fis_gRI14[] = { 2, 2, 2, 2, 2, 1, 1, 1 };
int fis_gRI15[] = { 2, 2, 2, 2, 2, 2, 1, 1 };
int fis_gRI16[] = { 2, 2, 2, 2, 2, 2, 2, 1 };
int* fis_gRI[] = { fis_gRI0, fis_gRI1, fis_gRI2, fis_gRI3, fis_gRI4, fis_gRI5, fis_gRI6, fis_gRI7, fis_gRI8, fis_gRI9, fis_gRI10, fis_gRI11, fis_gRI12, fis_gRI13, fis_gRI14, fis_gRI15, fis_gRI16 };

// Rule Outputs
int fis_gRO0[] = { 16 };
int fis_gRO1[] = { 17 };
int fis_gRO2[] = { 1 };
int fis_gRO3[] = { 2 };
int fis_gRO4[] = { 3 };
int fis_gRO5[] = { 4 };
int fis_gRO6[] = { 5 };
int fis_gRO7[] = { 6 };
int fis_gRO8[] = { 7 };
int fis_gRO9[] = { 8 };
int fis_gRO10[] = { 9 };
int fis_gRO11[] = { 10 };
int fis_gRO12[] = { 11 };
int fis_gRO13[] = { 12 };
int fis_gRO14[] = { 13 };
int fis_gRO15[] = { 14 };
int fis_gRO16[] = { 15 };
int* fis_gRO[] = { fis_gRO0, fis_gRO1, fis_gRO2, fis_gRO3, fis_gRO4, fis_gRO5, fis_gRO6, fis_gRO7, fis_gRO8, fis_gRO9, fis_gRO10, fis_gRO11, fis_gRO12, fis_gRO13, fis_gRO14, fis_gRO15, fis_gRO16 };

// Input range Min
//FIS_TYPE fis_gIMin[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// Input range Max
//FIS_TYPE fis_gIMax[8] = { 800, 800, 800, 800, 800, 800, 800, 800 };

// Output range Min
FIS_TYPE fis_gOMin[] = { 0 };

// Output range Max
FIS_TYPE fis_gOMax[] = { 16 };

//***********************************************************************
// Data dependent support functions for Fuzzy Inference System           
//***********************************************************************
FIS_TYPE fis_MF_out(FIS_TYPE** fuzzyRuleSet, FIS_TYPE x, int o)
{
    FIS_TYPE mfOut;
    int r;

    for (r = 0; r < fis_gcR; ++r)
    {
        int index = fis_gRO[r][o];
        if (index > 0)
        {
            index = index - 1;
            mfOut = (fis_gMF[fis_gMFO[o][index]])(x, fis_gMFOCoeff[o][index]);
        }
        else if (index < 0)
        {
            index = -index - 1;
            mfOut = 1 - (fis_gMF[fis_gMFO[o][index]])(x, fis_gMFOCoeff[o][index]);
        }
        else
        {
            mfOut = 0;
        }

        fuzzyRuleSet[0][r] = fis_min(mfOut, fuzzyRuleSet[1][r]);
    }
    return fis_array_operation(fuzzyRuleSet[0], fis_gcR, fis_max);
}

FIS_TYPE fis_defuzz_centroid(FIS_TYPE** fuzzyRuleSet, int o)
{
    FIS_TYPE step = (fis_gOMax[o] - fis_gOMin[o]) / (FIS_RESOLUSION - 1);
    FIS_TYPE area = 0;
    FIS_TYPE momentum = 0;
    FIS_TYPE dist, slice;
    int i;

    // calculate the area under the curve formed by the MF outputs
    for (i = 0; i < FIS_RESOLUSION; ++i){
        dist = fis_gOMin[o] + (step * i);
        slice = step * fis_MF_out(fuzzyRuleSet, dist, o);
        area += slice;
        momentum += slice*dist;
    }

    return ((area == 0) ? ((fis_gOMax[o] + fis_gOMin[o]) / 2) : (momentum / area));
}

//***********************************************************************
// Fuzzy Inference System                                                
//***********************************************************************
void fis_evaluate()
{
    FIS_TYPE fuzzyInput0[] = { 0, 0 };
    FIS_TYPE fuzzyInput1[] = { 0, 0 };
    FIS_TYPE fuzzyInput2[] = { 0, 0 };
    FIS_TYPE fuzzyInput3[] = { 0, 0 };
    FIS_TYPE fuzzyInput4[] = { 0, 0 };
    FIS_TYPE fuzzyInput5[] = { 0, 0 };
    FIS_TYPE fuzzyInput6[] = { 0, 0 };
    FIS_TYPE fuzzyInput7[] = { 0, 0 };
    FIS_TYPE* fuzzyInput[fis_gcI] = { fuzzyInput0, fuzzyInput1, fuzzyInput2, fuzzyInput3, fuzzyInput4, fuzzyInput5, fuzzyInput6, fuzzyInput7, };
    FIS_TYPE fuzzyOutput0[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FIS_TYPE* fuzzyOutput[fis_gcO] = { fuzzyOutput0};
    FIS_TYPE fuzzyRules[fis_gcR] = { 0 };
    FIS_TYPE fuzzyFires[fis_gcR] = { 0 };
    FIS_TYPE* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
    FIS_TYPE sW = 0;

    // Transforming input to fuzzy Input
    int i, j, r, o;
    for (i = 0; i < fis_gcI; ++i)
    {
        for (j = 0; j < fis_gIMFCount[i]; ++j)
        {
            fuzzyInput[i][j] =
                (fis_gMF[fis_gMFI[i][j]])(g_fisInput[i], fis_gMFICoeff[i][j]);
        }
    }

    int index = 0;
    for (r = 0; r < fis_gcR; ++r)
    {
        if (fis_gRType[r] == 1)
        //if (1)
        {
            fuzzyFires[r] = FIS_MAX_VALUE;
            for (i = 0; i < fis_gcI; ++i)
            {
                index = fis_gRI[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_min(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_min(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_min(fuzzyFires[r], 1);
            }
        }
        else
        {
            fuzzyFires[r] = FIS_MIN_VALUE;
            for (i = 0; i < fis_gcI; ++i)
            {
                index = fis_gRI[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_max(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_max(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_max(fuzzyFires[r], 0);
            }
        }

        //fuzzyFires[r] = fis_gRWeight[r] * fuzzyFires[r];
        fuzzyFires[r] = 1 * fuzzyFires[r];
        sW += fuzzyFires[r];
    }

    if (sW == 0)
    {
        for (o = 0; o < fis_gcO; ++o)
        {
            g_fisOutput[o] = ((fis_gOMax[o] + fis_gOMin[o]) / 2);
        }
    }
    else
    {
        for (o = 0; o < fis_gcO; ++o)
        {
            g_fisOutput[o] = fis_defuzz_centroid(fuzzyRuleSet, o);
        }
    }
}

