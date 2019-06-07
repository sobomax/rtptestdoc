# Copyright (c) 2006-2018 Sippy Software, Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

from ctypes import cdll, c_double, c_void_p, c_int, c_long, Structure, \
  pointer, POINTER
from math import modf
#from time import sleep
import sys

class timespec(Structure):
    _fields_ = [
        ('tv_sec', c_long),
        ('tv_nsec', c_long)
    ]

_elpl = cdll.LoadLibrary('libelperiodic.so')
_elpl.prdic_init.argtypes = [c_double, c_double]
_elpl.prdic_init.restype = c_void_p
_elpl.prdic_procrastinate.argtypes = [c_void_p,]
_elpl.prdic_free.argtypes = [c_void_p,]
_elpl.prdic_addband.argtypes = [c_void_p, c_double]
_elpl.prdic_addband.restype = c_int
_elpl.prdic_useband.argtypes = [c_void_p, c_int]
_elpl.prdic_set_epoch.argtypes = [c_void_p, POINTER(timespec)]

#h = _elpl.prdic_init(200.0, 0.0)
#sleep(20)

class ElPeriodic(object):
    _hndl = None
    _elpl = None

    def __init__(self, freq, offst = 0.0):
        self._elpl = _elpl
        self._hndl = self._elpl.prdic_init(freq, offst)
        if not bool(self._hndl):
            raise Exception('prdic_init() failed')

    def procrastinate(self):
        self._elpl.prdic_procrastinate(self._hndl)

    def addband(self, freq_hz):
        r = self._elpl.prdic_addband(self._hndl, freq_hz)
        return int(r)

    def useband(self, bandnum):
        self._elpl.prdic_useband(self._hndl, c_int(bandnum))

    def set_epoch(self, dtime):
        ts = timespec()
        tv_frac, tv_sec = modf(dtime)
        ts.tv_sec = int(tv_sec)
        ts.tv_nsec = int(tv_frac * 1e+09)
        self._elpl.prdic_set_epoch(self._hndl, pointer(ts))

    def __del__(self):
        if bool(self._hndl):
            self._elpl.prdic_free(self._hndl)

if __name__ == '__main__':
    i = 0
    elp = ElPeriodic(200.0)
    elp.set_epoch(0.0)
    while i < 20000:
        elp.procrastinate()
        i += 1
        sys.stdout.write('%d\r' % i)
        sys.stdout.flush()
    #print(h)
    del elp
