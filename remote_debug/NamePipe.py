#-*- coding:utf-8 -*-

"""\
	使用FIFO模拟一个双工管道用于两个进程之间的通信
"""

import os
import tempfile

__all__ = ["NamePipe"]

class NamePipe(object):

	def __init__(self, pid, is_client = True, mode = 0666):
		super(NamePipe, self).__init__()

		name = self._get_pipe_name(pid)
		self.in_name = name + ".in"
		self.out_name = name + ".out"

		try:
			os.mkfifo(self.in_name, mode)
			os.chmod(self.in_name, mode)
		except OSError: pass
		try : 
			os.mkfifo(self.out_name, mode)
			os.chmod(self.out_name, mode)
		except OSError: pass

		self.is_client = is_client

		if is_client:
			# client
			self.in_fd = open(self.in_name, "r")
			self.out_fd = open(self.out_name, "w")
		else:
			# server
			self.out_fd = open(self.in_name, "w")
			self.in_fd = open(self.out_name, "r")

	def write(self, msg):
		if self.is_open():
			self.out_fd.write("%d\n" % len(msg))
			self.out_fd.write(msg)
			self.out_fd.flush()
			return True
		else:
			return False

	def read(self):
		if self.is_open():
			sz = self.in_fd.readline()
			if sz:
				return self.in_fd.read(int(sz))
			else:
				return ""
		else:
			return None

	def flush(self):
		pass

	def readline(self):
		return self.read()

	def is_open(self):
		#return True
		return not (self.in_fd.closed or self.out_fd.closed)

	def close(self):
		"""  只尝试unlink掉读端
		"""
		if self.is_client:
			try: os.remove(self.in_name)
			except OSError: pass
		else:
			try: os.remove(self.out_name)
			except OSError: pass
		self.in_fd.close()
		self.out_fd.close()


	def _get_pipe_name(self, pid):
		return os.path.join(tempfile.gettempdir(), "pipe-%d" % pid)	

