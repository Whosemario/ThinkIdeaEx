# -*- coding:utf-8 -*-

from flask import Flask
app = Flask(__name__)

@app.route("/")
def hello():
	return "hello world."

class Master(object):

	def run(self):
		app.run()

if __name__ == "__main__":
	Master().run()

