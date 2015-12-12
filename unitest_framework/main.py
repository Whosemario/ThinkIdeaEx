#-*- coding:utf-8 -*-

import os
import unittest

g_test_cases = []

def scanTestCaseInsts():
	
	rootfile = os.path.dirname(__file__)

	def func(arg, dirname, filenames):
		global g_test_cases

		module_dir = dirname[len(rootfile):]
		module_dir = module_dir.replace("/", ".")
		
		for file_name in filenames:
			if file_name not in (".", "..") \
				and file_name.endswith(".pyc") is False \
				and file_name.startswith("__init__") is False:
				file_name = file_name.split(".")[0]
				module_name = module_dir + "." + file_name
				m = __import__(module_name, globals(), locals(), [""])
				g_test_cases.extend(getattr(m, "test_cases", []))

	os.path.walk(os.path.join(rootfile, "test_cases"), func, None)


scanTestCaseInsts()

test_suite = unittest.TestSuite()
for test_inst in g_test_cases:
	test_suite.addTest(test_inst)

runner = unittest.TextTestRunner(verbosity = 2)
runner.run(test_suite)
