/*********************************************************************** */
/*     Benchmark #2 -- Double Precision Whetstone (A001)                 */
/*                                                                       */
/*     o        This is a REAL*8 version of                              */
/*      the Whetstone benchmark program.                                 */
/*                                                                       */
/*     o        DO-loop semantics are ANSI-66 compatible.                */
/*                                                                       */
/*     o        Final measurements are to be made with all               */
/*      WRITE statements and FORMAT statements removed.                  */
/*                                                                       */
/*********************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
/*      IMPLICIT REAL*8 (A-H,O-Z); */


#ifndef LOOPS
#define LOOPS (2000)
#endif

static double T,T1,T2,E1[5];
static int J,K,L;
static  double X1,X2,X3,X4;
static  volatile double ptime,time0;

static void PA(double* E);
static void P0(void);
static void P3(double *X, double *Y, double *Z);
static void POUT(int N, int J, int K, double X1, double X2, double X3, double X4);

static inline double dtime (void)
{
        return SYSTIMER_GetTickCount() / (1000.0);
}

void test_whetd (void) {
        int LOOP,I,II,JJ,N1,N2,N3,N4,N5,N6,N7,N8,N9,N10,N11;
        double X,Y,Z;
        volatile double time1;

        printf (" Benchmark #2 -- Double Precision Whetstone (A001)\n");
        printf (" %d LOOPS\n", LOOPS);

/*      Start benchmark timing at this point. */

        time0 = dtime();
        ptime = time0;

/*      The actual benchmark starts here. */

        T = .499975;
        T1 = 0.50025;
        T2 = 2.0;

/*      With loopcount LOOP=10, one million Whetstone instructions */
/*      will be executed in EACH MAJOR LOOP..A MAJOR LOOP IS EXECUTED */
/*      'II' TIMES TO INCREASE WALL-CLOCK TIMING ACCURACY. */

        LOOP = LOOPS;
        II   = 1;

        for (JJ=1;JJ<=II;JJ++) { /* DO 500 JJ=1,II; */

/*      Establish the relative loop counts of each module. */

        N1 = 0;
        N2 = 12 * LOOP;
        N3 = 14 * LOOP;
        N4 = 345 * LOOP;
        N5 = 0;
        N6 = 210 * LOOP;
#if defined(R3KEMU) || defined(TINYSIM)
        /* trig functions are just too slow with doubly emulated f.p */
        N7 = 0;
#else
        N7 = 32 * LOOP;
#endif
        N8 = 899 * LOOP;
        N9 = 616 * LOOP;
        N10 = 0;
        N11 = 93 * LOOP;

/*      Module 1: Simple identifiers */

        X1 = 1.0;
        X2 = -1.0;
        X3 = -1.0;
        X4 = -1.0;

        if (N1!=0) { /* GO TO 35; */
                for(I=1;I<=N1;I++) { /* DO 30 I=1,N1; */
                X1 = (X1 + X2 + X3 - X4)*T;
                X2 = (X1 + X2 - X3 + X4)*T;
                X3 = (X1 - X2 + X3 + X4)*T;
                X4 = (-X1 + X2 + X3 + X4)*T;
/* 30 */        };
/* 35 */        };

        if (JJ==II) POUT(N1,N1,N1,X1,X2,X3,X4);

/*      Module 2: Array elements */

        E1[1] = 1.0;
        E1[2] = -1.0;
        E1[3] = -1.0;
        E1[4] = -1.0;

        if (N2!=0) { /* GO TO 45; */
                for (I=1;I<=N2;I++) { /* DO 40 I=1,N2; */
                E1[1] = (E1[1] + E1[2] + E1[3] - E1[4])*T;
                E1[2] = (E1[1] + E1[2] - E1[3] + E1[4])*T;
                E1[3] = (E1[1] - E1[2] + E1[3] + E1[4])*T;
                E1[4] = (-E1[1] + E1[2] + E1[3] + E1[4])*T;
/* 40 */        };
/* 45 */        };

        if (JJ==II) POUT(N2,N3,N2,E1[1],E1[2],E1[3],E1[4]);

/*      Module 3: Array as parameter */

        if (N3!=0) { /* GO TO 59; */
                for (I=1;I<=N3;I++) { /* DO 50 I=1,N3; */
                PA(E1);
/* 50 */        };
/* 59 */        };

        if (JJ==II) POUT(N3,N2,N2,E1[1],E1[2],E1[3],E1[4]);

/*      Module 4: Conditional jumps */

        J = 1;
        if (N4!=0) { /* GO TO 65; */
                for (I=1;I<=N4;I++) { /* DO 60 I=1,N4; */
                if (J==1) goto L51;
                J = 3;
                goto L52;
L51:            J = 2;
L52:            if (J > 2) goto L53;
                J = 1;
                goto L54;
L53:            J = 0;
L54:            if (J < 1) goto L55;
                J = 0;
                goto L60;
L55:            J = 1;
L60:    ;       };
/* 65 */        };

        if (JJ==II) POUT(N4,J,J,X1,X2,X3,X4);

/*      Module 5: Omitted */
/*      Module 6: Integer arithmetic */

        J = 1;
        K = 2;
        L = 3;

        if (N6!=0) { /* GO TO 75; */
                for (I=1;I<=N6;I++) { /* DO 70 I=1,N6; */
                J = J * (K-J) * (L-K);
                K = L * K - (L-J) * K;
                L = (L - K) * (K + J);
                E1[L-1] = J + K + L;
                E1[K-1] = J * K * L;
/* 70 */        };
/* 75 */        };

        if (JJ==II) POUT(N6,J,K,E1[1],E1[2],E1[3],E1[4]);

/*      Module 7: Trigonometric functions */

        X = 0.5;
        Y = 0.5;

        if (N7!=0) { /* GO TO 85; */
                for (I=1;I<=N7;I++) { /* DO 80 I=1,N7; */
                X=T*atan(T2*sin(X)*cos(X)/(cos(X+Y)+cos(X-Y)-1.0));
                Y=T*atan(T2*sin(Y)*cos(Y)/(cos(X+Y)+cos(X-Y)-1.0));
/* 80 */        };
/* 85 */        };

        if (JJ==II) POUT(N7,J,K,X,X,Y,Y);

/*      Module 8: Procedure calls */

        X = 1.0;
        Y = 1.0;
        Z = 1.0;

        if (N8!=0) { /* GO TO 95; */
                for (I=1;I<=N8;I++) { /* DO 90 I=1,N8; */
                P3(&X,&Y,&Z); /* FORTRAN semantics */
/* 90 */        };
/* 95 */        };

        if (JJ==II) POUT(N8,J,K,X,Y,Z,Z);

/*      Module 9: Array references */

        J = 1;
        K = 2;
        L = 3;
        E1[1] = 1.0;
        E1[2] = 2.0;
        E1[3] = 3.0;

        if (N9!=0) { /* GO TO 105; */
                for (I=1;I<=N9;I++) { /* DO 100  I=1,N9; */
                P0();
/* 100 */       };
/* 105 */       };

        if (JJ==II) POUT(N9,J,K,E1[1],E1[2],E1[3],E1[4]);

/*      Module 10: Integer arithmetic */

        J = 2;
        K = 3;

        if (N10!=0) { /* GO TO 115; */
                for (I=1;I<=N10;I++) { /* DO 110 I=1,N10; */
                J = J + K;
                K = J + K;
                J = K - J;
                K = K - J - J;
/* 110 */       };
/* 115 */       };

        if (JJ==II) POUT(N10,J,K,X1,X2,X3,X4);

/*      Module 11: Standard functions */

        X = 0.75;

        if (N11!=0) { /* GO TO 125; */
                for (I=1;I<=N11;I++) { /* DO 120 I=1,N11; */
                X = sqrt(exp(log(X)/T1));
/* 120 */       };
/* 125 */       };

        if (JJ==II) POUT(N11,J,K,X,X,X,X);

/*      THIS IS THE END OF THE MAJOR LOOP. */

/* 500 */       };

/*      Stop benchmark timing at this point. */

        time1 = dtime();

/*---------------------------------------------------------------- */
/*      Performance in Whetstone KIP's per second is given by */

/*      (100*LOOP*II)/TIME */

/*      where TIME is in seconds. */
/*-------------------------------------------------------------------- */

        printf("time=%.6f secs,",time1-time0);
        if (time1 <= time0)
        {
                printf("error: %d\n", __LINE__);
        }
        else
        {
                printf (" Double Whetstone MIPS %.3f\n",((100*LOOP*II)/(time1-time0))/1000);
        }
}       /* END; */

static void PA(double* E) { /* SUBROUTINE PA(E); */
/*      IMPLICIT REAL*8 (A-H,O-Z); */
/*      DIMENSION E(4); */
/*      COMMON T,T1,T2,E1(4),J,K,L; */
        int J1;
        J1 = 0;
L10:    E[1] = (E[1] + E[2] + E[3] - E[4]) * T;
        E[2] = (E[1] + E[2] - E[3] + E[4]) * T;
        E[3] = (E[1] - E[2] + E[3] + E[4]) * T;
        E[4] = (-E[1] + E[2] + E[3] + E[4]) / T2;
        J1 = J1 + 1;
        if ((J1 - 6) < 0) goto L10; /* 10,20,20; */

/* 20 */        return;
}       /* END; */

static void P0(void) { /* SUOUTINE P0; */
/*      IMPLICIT REAL*8 (A-H,O-Z); */
/*      COMMON T,T1,T2,E1(4),J,K,L; */
        E1[J] = E1[K];
        E1[K] = E1[L];
        E1[L] = E1[J];
        return;
}       /* END; */

static void P3(double *X, double *Y, double *Z)  { /* SUBROUTINE P3(X,Y,Z); */
/*      IMPLICIT REAL*8 (A-H,O-Z); */
/*      COMMON T,T1,T2,E1(4),J,K,L; */
        double Y1;
        X1 = *X;
        Y1 = *Y;
        X1 = T * (X1 + Y1);
        Y1 = T * (X1 + Y1);
        *Z = (X1 + Y1) / T2;
        return;
}       /* END; */

static void POUT(int N, int J, int K, double X1, double X2, double X3, double X4) { /* SUBROUTINE POUT(N,J,K,X1,X2,X3,X4); */
/*      IMPLICIT REAL*8 (A-H,O-Z); */
/*      common/ptime/ptime,time0; */
        double time1,dtime();
        time1 = dtime();
#ifdef TRACE
        printf ("%3d%3d %7d%7d%7d%12.4g%12.4g%12.4g%12.4g\n", (int)(time1-time0),(int)(time1-ptime),N,J,K,X1,X2,X3,X4);
#endif
        ptime = time1;
        return;
}       /* END; */

