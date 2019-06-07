#!/usr/bin/env python

from distutils.core import setup

kwargs = {'name':'ElPeriodic',
      'version':'1.0',
      'description':'Phase-locked userland scheduling library',
      'author':'Maksym Sobolyev',
      'author_email':'sobomax@gmail.com',
      'url':'https://github.com/sobomax/libelperiodic',
      'packages':['elperiodic',],
      'package_dir':{'elperiodic':'python'}
     }

import sys

if __name__ == '__main__':
    setup(**kwargs)

