#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# This file is both the script to install Git repository hooks and the
# implementation of those hooks themselves. It does this by symlinking to
# itself from .git/hooks/*, and by examining the name it was called as to
# decide what to do.
#


# Import sundry Python packages
import os, sys, re
import subprocess as sp
from   gitutils import *



#
# Main
#

if __name__ == "__main__":
	# Useful in identifying how we were invoked.
	basename = os.path.basename(sys.argv[0])
	dirname  = os.path.dirname (sys.argv[0])
	repo     = ThisGitRepo()
	
	
	if   repo.wasInvokedAsHook(sys.argv[0]):
		sys.exit(repo.invokeHook(sys.argv[0]))
	elif basename    == "repository_hooks.py":
		if   len(sys.argv) == 1:
			print("Invoke me with argument \"install\" to install the Git "+
			      "repository hooks!")
			sys.exit(1)
		elif sys.argv[1] == "install": repo.installHooks()
		elif sys.argv[1] == "get":     print(repo.getConfig(args[1]))
		elif sys.argv[1] == "set":     repo.setConfig(args[1], args[2])
		elif sys.argv[1] == "unset":   repo.setConfig(args[1], None)
		elif sys.argv[1] == "root":    print(repo.getRootDir())
	
	# Exit
	sys.exit(0)

