#-*- coding:utf-8 -*-

"""\
	远程调试工具
	目标:
		通过pid链接到本服务器上的某个运行进程(一般是一个Game)
"""

__all__ = ["remote_debug", "reg_listener"]

import NamePipe
import os

# decorate
def remote_debug(func):
	def wrapper(*args, **kwargs):
		import pdb
		if _pipe:
			try:
				pdb.Pdb(stdin = _pipe, stdout = _pipe).runcall(func, *args, **kwargs)
			except IOError:
				_pipe.close()
				_pipe = None
		else:
			return func(*args, **kwargs)
	return wrapper

_pipe = None

def remote_connect(sig, frame):
	if _pipe:
		_pipe.close()
	_pipe = NamePipe.NamePipe(os.getpid(), False)

def reg_listener():
	import signal
	signal.signal(signal.SIGUSR1, remote_connect)