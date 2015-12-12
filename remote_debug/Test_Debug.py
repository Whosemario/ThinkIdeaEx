#-*- coding:utf-8 -*-

"""\
	测试bdb
"""

import bdb
import pdb

def func(a, b):
	print "start func"
	c = a+b
	print "c is", c
	print "end func"
	return 1

if __name__ == "__main__":

	print pdb.runcall(func, 1, 2)