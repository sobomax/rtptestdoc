#include <sys/time.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//extern int optind;

#include "elperiodic.h"

static void
usage(void)
{

    fprintf(stderr, "usage: testskew [-vqm] freq ival\n");
    exit(1);
}

#define silence(msg) (!(qflag) ? (msg) : "")

int
main(int argc, char * const argv[])
{
    void *prd;
    int i, ch, vflag, qflag, mflag, Sflag;
    double freq, duration, skew, fval;
    time_t ncycles, mcycles;

    vflag = 0;
    qflag = 0;
    mflag = 0;
    Sflag = 0;
    fval = 0.0;
    while ((ch = getopt(argc, argv, "vqmSf:")) != -1) {
         switch (ch) {
         case 'v':
             vflag = 1;
             break;

         case 'q':
             qflag = 1;
             break;

         case 'm':
             mflag = 1;
             break;

         case 'S':
             Sflag = 1;
             break;

         case 'f':
             fval = atof(optarg);
             break;

         case '?':
         default:
             usage();
         }
    }
    argc -= optind;
    argv += optind;
    if (argc != 2) {
        usage();
    }
    freq = atof(argv[0]);
    duration = atof(argv[1]);
    prd = prdic_init(freq, 0.0);
    if (fval != 0.0) {
        prdic_set_fparams(prd, fval);
    }
    for (i = 0; i < (freq * duration); i++) {
        prdic_procrastinate(prd);
        ncycles = prdic_getncycles_ref(prd);
        if (vflag != 0) {
            printf("%lld\r", (long long)ncycles);
            fflush(stdout);
        }
    }
    if (vflag != 0) {
         printf("\n");
    }
    if (mflag == 0) {
        skew = 1.0 - ((double)ncycles / (freq * duration));
        if (Sflag == 0) {
            printf("%s%f%s\n", silence("skew: "), skew * 100.0, silence("%"));
        } else {
            printf("%s%d\n", silence("skew: "), (int)(skew * 100000.0));
        }
    } else {
        mcycles = ncycles - (freq * duration);
        printf("%s%jd\n", silence("missed cycles: "), (intmax_t)mcycles);
    }

    return (0);
}
