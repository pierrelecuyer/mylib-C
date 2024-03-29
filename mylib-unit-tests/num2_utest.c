#include "gdef.h"
#include "util.h"
#include "mylib_utest.h"
#include "num.h"
#include "num2.h"
#include <stdio.h>
#include <string.h>


void num2_utest_LnFactorial() {
    printf("num2 : Executing LnFactorial unit tests.\n");
    assert_double(num2_LnFactorial(85),295.76660135076);
    assert_double(num2_LnFactorial(100),363.73937555556);
    assert_double(num2_LnFactorial(113),424.47819341826);
}

void num2_utest_LnGamma() {
    printf("num2 : Executing LnGamma unit tests.\n");
    assert_double(num2_LnGamma(0.00005),9.903458693809000468393);
    assert_double(num2_LnGamma(0.5),0.5723649429247000870717);
    assert_double(num2_LnGamma(18),33.50507345013688888401);
    assert_double(num2_LnGamma(50),144.5657439463448860089);
    assert_double(num2_LnGamma(113),419.7508055995447340991);
}

void num2_utest_Combination() {
    printf("num2 : Executing Combination unit tests.\n");
    assert_double(num2_Combination(0,0),1);
    assert_double(num2_Combination(10,0),1);
    assert_double(num2_Combination(24,12),2704156);
    assert_double(num2_Combination(74,60),456002537343216);
}

void num2_utest_log1p() {
    printf("num2 : Executing log1p unit tests.\n");
    assert_double(num2_log1p(0),0);
    assert_double(num2_log1p(-0.5),0.6931471805599453);
    assert_double(num2_log1p(0.5),0.4054651081081644);
    assert_double(num2_log1p(100),4.61512051684126);
}

void num2_utest_CalcMatStirling() {
    double **M;
    num2_CalcMatStirling(&M, 9, 9);
    double matStirlingSum[] = {1,1,2,5,15,52,203,877,4140,21147};
    double matStirlingSum_utest[10];
    int i,j;
    double k;

    for (i=0; i<=9; i++) {
        k=0;
        for (j=0; j<=i; j++) {
            k=k+M[j][i];
        }
        matStirlingSum_utest[i]=k;
    }
    printf("num2 : Executing CalcMatStirling unit tests.\n");
    assert_double(matStirlingSum[0],matStirlingSum_utest[0]);
    assert_double(matStirlingSum[1],matStirlingSum_utest[1]);
    assert_double(matStirlingSum[2],matStirlingSum_utest[2]);
    assert_double(matStirlingSum[3],matStirlingSum_utest[3]);
    assert_double(matStirlingSum[4],matStirlingSum_utest[4]);
    assert_double(matStirlingSum[5],matStirlingSum_utest[5]);
    assert_double(matStirlingSum[6],matStirlingSum_utest[6]);
    assert_double(matStirlingSum[7],matStirlingSum_utest[7]);
    assert_double(matStirlingSum[8],matStirlingSum_utest[8]);
    assert_double(matStirlingSum[9],matStirlingSum_utest[9]);
    num2_FreeMatStirling(&M,9);
}

void num2_utest_VolumeSphere() {
    printf("num2 : Executing VolumeSphere unit tests.\n");
    assert_double(num2_VolumeSphere(2,3),4.188790204786390984);
    assert_double(num2_VolumeSphere(3,3),5.696583541509835167);
    assert_double(num2_VolumeSphere(1000,3),7.99996059844262270);
    assert_double(num2_VolumeSphere(2,10),2.550164039877345443);
    assert_double(num2_VolumeSphere(3,10),35.651609488062913386);
    assert_double(num2_VolumeSphere(1000,10),1023.924607665882446014);
}

void num2_utest_BesselK025() {
    printf("num2 : Executing BesselK025 unit tests.\n");
    util_Assert(util_nearEqualRel(num2_BesselK025(1),0.4307397744485855246569,0.5e-6),"Failure in double comparison");
    util_Assert(util_nearEqualRel(num2_BesselK025(0.5),0.960316324931886022947,0.5e-6),"Failure in double comparison");
    util_Assert(util_nearEqualRel(num2_BesselK025(0.00005),25.463633449667459958,0.5e-6),"Failure in double comparison");
}

void num2_utest_Digamma() {
    printf("num2 : Executing Digamma unit tests.\n");
    assert_double(num2_Digamma(10),2.251752589066721107647);
    assert_double(num2_Digamma(2),0.4227843350984671393935);
    assert_double(num2_Digamma(0.3),-3.502524222200132988964);
    assert_double(num2_Digamma(0.0005),-2000.576393498247108909);
}

void num2_utest_all() {
    printf("num2 : Executing all unit tests.\n");
    num2_utest_LnFactorial();
    num2_utest_LnGamma();
    num2_utest_Combination();
    num2_utest_CalcMatStirling();
    num2_utest_VolumeSphere();
    num2_utest_BesselK025();
    num2_utest_Digamma();
    printf("num2 : all unit tests succeeded.\n");
}
