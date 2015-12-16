#-*- coding:utf-8 -*-

from greenlet import greenlet
from greenlet import getcurrent

def coroutine(func):
	return greenlet(func).switch

def await(*args, **kwargs):
	getcurrent().parent.switch()
