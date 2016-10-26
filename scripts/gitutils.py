#!/usr/bin/env python
# -*- coding: utf-8 -*-



# Import sundry Python packages
import os, sys, re
import subprocess as sp
from   vcsutils import *



#
# Get absolute path to Git root directory given directory name
#

def getGitRoot(dirname):
	dirname = os.path.dirname(sys.argv[0]) if dirname==None else dirname
	dirname = os.path.abspath(str(dirname))
	
	
	while True:
		if os.path.isdir(os.path.join(dirname, ".git")):
			return dirname
		
		parDirname = os.path.dirname(dirname)
		if    dirname == parDirname: break
		else: dirname  = parDirname
	
	return None


#
# A basic implementation of operations on a Git repository
#

class GitRepo(Repo):
	def __init__(self, rootDir=None):
		super(GitRepo, self).__init__(getGitRoot(rootDir))
	def getAddedFiles(self):    return self.getFiles("A")
	def getModifiedFiles(self): return self.getFiles("M")
	def getRenamedFiles(self):  return self.getFiles("R")
	def getDeletedFiles(self):  return self.getFiles("D")
	def getCopiedFiles(self):   return self.getFiles("C")
	def getFiles(self, mode):
		mode    = str(mode)
		cmdLine = "git diff --cached --name-only --diff-filter="+mode+" "+self.getHEAD()
		p       = sp.Popen(cmdLine, stdout=sp.PIPE, stderr=sp.PIPE, shell=True, cwd=self.rootDir)
		return str(p.stdout.read().decode()).splitlines()
	
	def getHEAD(self):
		cmdLine = "git rev-parse --verify HEAD"
		p       = sp.Popen(cmdLine, stdout=sp.PIPE, stderr=sp.PIPE, shell=True, cwd=self.rootDir)
		if p.wait() == 0:
			return str(p.stdout.read().decode()).strip()
		else:
			return "4b825dc642cb6eb9a060e54bf8d69288fbee4904" # Empty object
	def getConfig(self, cfg, defaultVal=None):
		cfg     = str(cfg)
		cmdLine = "git config "+cfg
		p       = sp.Popen(cmdLine, shell=True, stdout=sp.PIPE, stderr=sp.PIPE, cwd=self.rootDir)
		if p.wait() == 0:
			return str(p.stdout.read().decode())
		else:
			return defaultVal
	def setConfig(self, cfg, val):
		cfg     = str(cfg)
		if val is None:
			cmdLine = "git config --unset "+cfg
		else:
			cmdLine = "git config "+cfg+" "+str(val)
		p = sp.Popen(cmdLine, shell=True, stdout=sp.PIPE, stderr=sp.PIPE, cwd=self.rootDir)
		p.wait()
	def isClean(self):
		cmdLine = "git diff --cached --exit-code --quiet"
		pc = sp.Popen(cmdLine, shell=True, stdout=sp.PIPE, stderr=sp.PIPE, cwd=self.rootDir)
		cleanCached    = pc.wait() == 0
		cmdLine = "git diff          --exit-code --quiet"
		pn = sp.Popen(cmdLine, shell=True, stdout=sp.PIPE, stderr=sp.PIPE, cwd=self.rootDir)
		cleanNonCached = pn.wait() == 0
		return cleanCached and cleanNonCached


#
# Subclass GitRepo with our custom functionality.
#

class ThisGitRepo(GitRepo):
	def __init__(self, rootDir=None):
		super(ThisGitRepo, self).__init__(rootDir)
		self.supportedHooks = {"pre-commit": self.hookPreCommit}
	
	def wasInvokedAsHook(self, name):
		return os.path.basename(name) in self.supportedHooks.keys()
	
	def invokeHook(self, name, *args, **kwargs):
		return self.supportedHooks[os.path.basename(name)](*args, **kwargs)
	
	def installHooks(self):
		for name in self.supportedHooks:
			self.installHook(name)
	
	def installHook(self, name):
		hookBase = os.path.join(self.rootDir, ".git", "hooks")
		hookPath = os.path.join(hookBase,     name)
		
		if os.path.exists(hookPath):
			os.unlink(hookPath)
		os.symlink(os.path.relpath(sys.argv[0], hookBase), hookPath)
	
	def setConfig(self, cfg, val):
		if cfg == "hook.maxfilesize":
			val = interpretSizeSuffix(val)
		super(GitRepo, self).setConfig(cfg, val)
	
	def hookPreCommit(self):
		if    self.hookPreCommitSaneNames()       > 0 or \
		      self.hookPreCommitCheckOversize()   > 0   : sys.exit(1)
		else                                            : sys.exit(0)
	
	def hookPreCommitSaneNames(self):
		err = 0
		for fName in self.getAddedFiles()   + self.getModifiedFiles() + \
		             self.getRenamedFiles() + self.getCopiedFiles():
			if re.match("\s", fName):
				err += 1
				print("File path  {:60s}  contains whitespace!".format("'{:s}'".format(fName)))
		
		return err
	
	def hookPreCommitCheckOversize(self):
		sizeThreshold = int(self.getConfig("hook.maxfilesize", 1048576))
		err = 0
		for fName in self.getAddedFiles() + self.getModifiedFiles():
			if os.path.isdir(fName):
				continue
			size = os.stat(fName).st_size
			if size > sizeThreshold:
				err += 1
				print("File  {:60s} is {:7d} bytes > {:d}!".format("'{:s}'".format(fName), size, sizeThreshold))
		
		return err

