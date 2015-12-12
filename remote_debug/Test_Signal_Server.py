#-*- coding:utf-8 -*-

import signal
import os
import NamePipe
import pdb

def func(a, b):
	print "start func"
	c = a+b
	print "c is", c
	print "end func"

def remote_connect(sig, frame):
	pipe = NamePipe.NamePipe(os.getpid(), False)
	pdb.Pdb(stdin = pipe, stdout = pipe).runcall(func, 1, 2)

signal.signal(signal.SIGUSR1, remote_connect)

print "pid", os.getpid()

while True:
	pass