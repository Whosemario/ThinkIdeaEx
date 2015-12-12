#-*- coding:utf-8 -*-

import os
import sys

import NamePipe

pipe = NamePipe.NamePipe(1177, True)

while True:
	txt = raw_input(">>> ")
	pipe.write(txt)
	txt = pipe.read()
	if txt:
		sys.stdout.write("%s\n" % txt)
		sys.stdout.flush()