#-*- coding:utf-8 -*-

from distutils.core import setup, Extension
setup(name='listex', version='1.0',  \
      ext_modules=[Extension('listex', ['src/listexmodule.c'])])
