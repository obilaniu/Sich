#!/usr/bin/env python
# -*- coding: utf-8 -*-


# Import sundry Python packages
import os, sys, re, shutil
import subprocess as sp


#
# Main
#

if __name__ == "__main__":
	if len(sys.argv) < 4:
		print("Please provide the source directory, two non-existant directory "
		      "paths as well as configuration flags for a reproducible build test.")
		sys.exit(1)
	
	sources = sys.argv[1]
	folderA = sys.argv[2]
	folderB = sys.argv[3]
	
	if os.path.exists(folderA):
		print("The folder \'"+folderA+"\' already exists!")
		sys.exit(1)
	if os.path.exists(folderB):
		print("The folder \'"+folderB+"\' already exists!")
		sys.exit(1)
	if folderA == folderB:
		print("The two folders cannot match!")
		sys.exit(1)
	
	sources = os.path.abspath(sources)
	folderA = os.path.abspath(folderA)
	folderB = os.path.abspath(folderB)
	folderABuild   = os.path.join(folderA, "build")
	folderAInstall = os.path.join(folderA, "install")
	folderBBuild   = os.path.join(folderB, "build")
	folderBInstall = os.path.join(folderB, "install")
	os.mkdir(folderA)
	os.mkdir(folderABuild)
	os.mkdir(folderAInstall)
	
	
	
	# Build A
	cmdLine    = ["meson", sources, "--prefix="+folderAInstall] + sys.argv[4:]
	procCfgA   = sp.Popen(cmdLine, cwd=folderABuild)
	if procCfgA.wait() != 0:
		print("Error during Meson configuration!")
		sys.exit(1)
	
	procBuildA = sp.Popen(["ninja"], cwd=folderABuild)
	if procBuildA.wait() != 0:
		print("Error during Ninja build!")
		sys.exit(1)
	
	procInstallA = sp.Popen(["ninja", "install"], cwd=folderABuild)
	if procInstallA.wait() != 0:
		print("Error during Ninja install!")
		sys.exit(1)
	
	# Move
	shutil.move(folderA, folderB)
	os.mkdir(folderA)
	os.mkdir(folderABuild)
	os.mkdir(folderAInstall)
	
	
	# Build B
	cmdLine    = ["meson", sources, "--prefix="+folderAInstall] + sys.argv[4:]
	procCfgA   = sp.Popen(cmdLine, cwd=folderABuild)
	if procCfgA.wait() != 0:
		print("Error during Meson configuration!")
		sys.exit(1)
	
	procBuildA = sp.Popen(["ninja"], cwd=folderABuild)
	if procBuildA.wait() != 0:
		print("Error during Ninja build!")
		sys.exit(1)
	
	procInstallA = sp.Popen(["ninja", "install"], cwd=folderABuild)
	if procInstallA.wait() != 0:
		print("Error during Ninja install!")
		sys.exit(1)
	
	print("\n\n\nChecking Differences...")
	
	
	# Difference taking
	diffCode = sp.Popen(["diff", "-qr", folderAInstall, folderBInstall]).wait()
	if diffCode != 0:
		print("Not reproducbile!")
	else:
		print("Build reproduced!")
	sys.exit(diffCode)

