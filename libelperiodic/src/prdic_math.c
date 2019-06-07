/*
 * Copyright (c) 2014 Sippy Software, Inc., http://www.sippysoft.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/time.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "prdic_math.h"
#include "prdic_timespecops.h"

void
_prdic_PFD_init(struct _prdic_PFD *pfd_p)
{

    memset(pfd_p, '\0', sizeof(struct _prdic_PFD));
}

double
_prdic_PFD_get_error(struct _prdic_PFD *pfd_p, const struct timespec *tclk)
{
    double err0r;
    struct timespec ttclk;

    if (timespeciszero(&pfd_p->last_tclk)) {
        pfd_p->last_tclk = *tclk;
        return (0.0);
    }
    timespecsub2(&ttclk, tclk, &pfd_p->last_tclk);
    err0r = timespec2dtime(&ttclk);
    pfd_p->last_tclk = *tclk;
    return (1.0 - err0r);
}

double
_prdic_sigmoid(double x)
{

    return (x / (1 + fabs(x)));
}

double
_prdic_recfilter_apply(struct _prdic_recfilter *f, double x)
{

    f->lastval = f->a * x + f->b * f->lastval;
    if (f->peak_detect != 0) {
        if (f->lastval > f->maxval) {
            f->maxval = f->lastval;
        } if (f->lastval < f->minval) {
            f->minval = f->maxval;
        }
    }
    return f->lastval;
}

void
_prdic_recfilter_init(struct _prdic_recfilter *f, double fcoef, double initval, int peak_detect)
{

    f->lastval = initval;
    _prdic_recfilter_adjust(f, fcoef);
    if (peak_detect != 0) {
        f->peak_detect = 1;
        f->maxval = initval;
        f->minval = initval;
    } else {
        f->peak_detect = 0;
        f->maxval = 0;
        f->minval = 0;
    }
}

void
_prdic_recfilter_adjust(struct _prdic_recfilter *f, double fcoef)
{

    assert(fcoef < 1.0 && fcoef > 0.0);
    f->a = 1.0 - fcoef;
    f->b = fcoef;
}

double
_prdic_freqoff_to_period(double freq_0, double foff_c, double foff_x)
{

    return (1.0 / freq_0 * (1 + foff_c * foff_x));
}
