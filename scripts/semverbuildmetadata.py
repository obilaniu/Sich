#!/usr/bin/env python
# -*- coding: utf-8 -*-


# Import sundry Python packages
import os, sys, re, shutil, time
import subprocess as sp
from   gitutils import ThisGitRepo



#
# This script receives as arguments [1] the path to the source tree root,
# [2] a semantic version specifier that it then validates, exiting with a
# non-zero code the version specifier has a problem, and [3]
# SOURCE_DATE_EPOCH, which is either a non-negative representing the nominal
# build time in seconds since the Epoch (Thu Jan  1 00:00:00 1970), or the
# string "now". If provided as an integer, SOURCE_DATE_EPOCH should preferably
# be equal to the time of last modification to any file in the repository.
# 
# In particular, the version specifier must be a valid Semantic Version, and
# must not contain build metadata (computing the build metadata is this
# script's responsibility).
# 
# If the version specifiers has no problems, the script prints to stdout, line
# by line, the following:
# 
# - Line  0: Major  version
# - Line  1: Minor  version
# - Line  2: Patch  version
# - Line  3: Short  version "M.m.p"
# - Line  4: Normal version (Argument [1] to this script)
# - Line  5: Full   version (The argument to this script + build metadata)
# - Line  6: "1" if release version, "0" otherwise
# - Line  7: VCS Id (name of the Version Control System in use). Currently, one
#            of ["none", "git"]. In the future, perhaps "svn" will be added.
# - Line  8: VCS Commit. For VCS Id
#            - "none", the empty string.
#            - "git", the currently checked-out commit, as the output of
#              git rev-parse HEAD, or 4b825dc642cb6eb9a060e54bf8d69288fbee4904
#              (empty object) if no commits yet.
#            - "svn", the current revision, as given by svn info.
# - Line  9: VCS working directory is clean. "1" if clean, "0" otherwise.
# - Line 10: An integer SOURCE_DATE_EPOCH ( == Argument [2] to this script when
#            it is not "now")
# - Line 11: A string representing the nominal build time in the local
#            timezone. Equal to
#                asctime(localtime(SOURCE_DATE_EPOCH))
# - Line 12: A string representing the nominal build time in UTC. Equal to
#                asctime(gmtime(SOURCE_DATE_EPOCH))
# - Line 13: A string representing the nominal build time in UTC, in a format
#            suitable for use in a semantic version. This format is
#                YYYYMMDD.HHMMSS

if __name__ == "__main__":
	if len(sys.argv) != 4:
		print("This script is meant to be invoked with three arguments:\n"+
			  "\t[1] The path to the source tree root\n"+
			  "\t[2] The project semantic version specifier,\n"+
			  "\t\tM.m.p-prerelease.ids.if.any\n"+
			  "\t[3] SOURCE_DATE_EPOCH, the decimal integer number of seconds since the Epoch.")
		sys.exit(-1)
	
	# Sanitize time.
	sourceRoot        = sys.argv[1]
	SOURCE_DATE_EPOCH = sys.argv[3]
	if SOURCE_DATE_EPOCH == "now":
		SOURCE_DATE_EPOCH = int(time.time())
	else:
		SOURCE_DATE_EPOCH = int(SOURCE_DATE_EPOCH)
		if SOURCE_DATE_EPOCH < 0 or str(SOURCE_DATE_EPOCH) != sys.argv[3]:
			sys.exit(1)
	semVerTime = time.strftime("%Y%m%d.%H%M%S", time.gmtime(SOURCE_DATE_EPOCH))
	
	# Sanitize version.
	normVer           = sys.argv[2]
	if len(normVer.split("+")) != 1:
		sys.exit(2)
	splitHyphenVer    = normVer.split("-")
	shortVer          = splitHyphenVer[0]
	splitDotVer       = shortVer.split(".")
	if len(splitDotVer) != 3:
		sys.exit(2)
	majorVer          = str(int(splitDotVer[0]))
	minorVer          = str(int(splitDotVer[1]))
	patchVer          = str(int(splitDotVer[2]))
	shortVer          = majorVer+"."+minorVer+"."+patchVer
	
	# Identify repository.
	vcsId             = "none"
	vcsCommit         = ""
	vcsClean          = True
	vcsVer            = ""
	if os.path.isdir(os.path.join(sourceRoot, ".git")):
		repo          = ThisGitRepo(sourceRoot)
		vcsId         = "git"
		vcsCommit     = repo.getHEAD()
		vcsClean      = repo.isClean()
		
		vcsVer        = ".git-"+vcsCommit
	elif os.path.isdir(os.path.join(sourceRoot, ".svn")):
		#repo          = ThisSVNRepo(sourceRoot)
		#vcsId         = "svn"
		#vcsCommit     = ...
		#vcsClean      = repo.isClean()
		#
		#vcsVer        = ".svn."+vcsCommit
		pass
	
	# Normal version name.
	normVer           = shortVer
	if   len(splitHyphenVer)  > 1 and not vcsClean:
		normVer      += "-"+"-".join(splitHyphenVer[1:])+".UNCLEAN"
	elif len(splitHyphenVer)  > 1 and     vcsClean:
		normVer      += "-"+"-".join(splitHyphenVer[1:])
	elif not vcsClean:
		normVer      += "-UNCLEAN"
	
	# Is a release? (Release must be clean)
	isRelease         = normVer == shortVer and vcsClean
	fullVer           = normVer+"+"+semVerTime+vcsVer
	
	# Print everything.
	print(majorVer)
	print(minorVer)
	print(patchVer)
	print(shortVer)
	print(normVer)
	print(fullVer)
	print("1" if isRelease else "0")
	print(vcsId)
	print(vcsCommit)
	print("1" if vcsClean  else "0")
	print(SOURCE_DATE_EPOCH)
	print(time.asctime(time.localtime(SOURCE_DATE_EPOCH)).strip())
	print(time.asctime(time.gmtime(SOURCE_DATE_EPOCH)).strip())
	print(semVerTime)
	
	
	# The string is valid and the output has been printed.
	sys.exit(0)
