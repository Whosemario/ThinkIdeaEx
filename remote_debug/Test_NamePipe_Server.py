#-*- coding:utf-8 -*-

import os
import sys
import signal

import NamePipe

print "pid", os.getpid()
pipe = NamePipe.NamePipe(1177, False)

def close(sig, frame):
	print "pipe close"

signal.signal(signal.SIGPIPE, close)

while True:
	sys.stdout.write(">>> ")
	sys.stdout.flush()

	txt = pipe.read()
	if txt:
		sys.stdout.write("%s" % txt)
		sys.stdout.write(">>> ")
		sys.stdout.flush()
		pipe.write(txt)
