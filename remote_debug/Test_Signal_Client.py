#-*- coding:utf-8 -*-

import signal
import os
import sys
import NamePipe
import pdb

Prefix = pdb.Pdb().prompt

pid = int(sys.argv[1])

os.kill(pid, signal.SIGUSR1)

pipe = NamePipe.NamePipe(pid, True)

while True:

	while True:
		txt = pipe.read()
		if txt:
			sys.stdout.write("%s" % txt)
			sys.stdout.flush()
			if txt.startswith(Prefix):
				break


	txt = raw_input("")
	pipe.write(txt)
