[![Build Status](https://travis-ci.org/sobomax/libelperiodic.svg?branch=master)](https://travis-ci.org/sobomax/libelperiodic)
[![Coverage Status](https://coveralls.io/repos/github/sobomax/libelperiodic/badge.svg?branch=master)](https://coveralls.io/github/sobomax/libelperiodic?branch=master)

# libElPeriodic
Library to run frequent periodic tasks.

## Principle of Operation
The libElPeriodic is designed to simplify writing control loops that are
expected to run at constant time intervals with smallest possible overhead
and little or no support from the underlying run time environment.

The library is optimized to align active periods of the control loop
to the both set frequency and phase by applying phase locked loop design
with a proportional phase detector and a low-pass filter as an error
amplifier.
