#!/usr/bin/env python
# -*- coding: utf-8 -*-


#
# Repository Object
#

class Repo(object):
	def __init__(self, rootDir=None):
		self.rootDir        = rootDir
		self.supportedHooks = {}
	def getAddedFiles(self):    pass
	def getModifiedFiles(self): pass
	def getRenamedFiles(self):  pass
	def getDeletedFiles(self):  pass
	def getCopiedFiles(self):   pass
	def getRootDir(self):       return self.rootDir
	def wasInvokedAsHook(self, name):
		return name in self.supportedHooks.keys()
	def invokeHook(self, name, *args, **kwargs):
		return self.supportedHooks[name](*args, **kwargs)


#
# Interpret size suffixes
#

def interpretSizeSuffix(val):
	val = str(val)
	if   re.match("[0-9]+B$",   val): return int(val[:-1])*(1024**0)
	elif re.match("[0-9]+KiB$", val): return int(val[:-3])*(1024**1)
	elif re.match("[0-9]+MiB$", val): return int(val[:-3])*(1024**2)
	elif re.match("[0-9]+GiB$", val): return int(val[:-3])*(1024**3)
	elif re.match("[0-9]+TiB$", val): return int(val[:-3])*(1024**4)
	elif re.match("[0-9]+KB$",  val): return int(val[:-2])*(1000**1)
	elif re.match("[0-9]+MB$",  val): return int(val[:-2])*(1000**2)
	elif re.match("[0-9]+GB$",  val): return int(val[:-2])*(1000**3)
	elif re.match("[0-9]+TB$",  val): return int(val[:-2])*(1000**4)
	else:                             return int(val)

