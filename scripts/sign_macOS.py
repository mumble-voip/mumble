#!/usr/bin/env python3
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# About the tool
# --------------
# sign_macOS.py is a tool that takes a Mumble .DMG file and digitally signs its content
# (executables, codecs, plugins, installers).
#
# The file(s) may already be signed. In this case, the tool will simply replace all the existing signatures.
#
# Using the tool
# --------------
# To sign Mumble-1.4.0.unsigned.dmg, one would do the following:
#
# $ ./sign_macOS.py -i Mumble-1.4.0.unsigned.dmg -o Mumble-1.4.0.dmg
#
# Configuration
# -------------
# The signing behavior of the tool can be configured via a configuration file.
# By default the tool will read its configuration from $HOME/.sign_macOS.cfg.
#
# The configuration file uses JSON. For example, to sign using a particular
# set of Developer ID certificates in your login keychain, you could use
# something like this:
#
#   --->8---
# 	{
# 		# The keychain needs the .keychain extension explicitly typed out.
# 		"keychain": "login.keychain",
# 		# Your Application certificate.
# 		"developer-id-app": "Developer ID Application: John Appleseed",
# 		# Your Installer certificate.
# 		"developer-id-installer": "Developer ID Installer: John Appleseed"
# 	}
#   --->8---

import argparse
import io
import json
import os
import pathlib
import plistlib
import shutil
import string
import subprocess
import sys
import tempfile

# Specify a set of codesign requirements for Mumble binaries.
#
# We require an Apple CA and a Developer ID leaf certificate (the one we're signing with).
# The 'designated' line is specifically tuned to work on older versions
# of macOS that aren't Developer ID-aware without breakage.
#
# We also explicitly require all shared libraries to be codesigned by Apple.
# We can reasonably do that because Mumble is statically built on macOS.
requirements = '''designated => anchor apple generic and identifier "${identifier}" and (certificate leaf[field.1.2.840.113635.100.6.1.9] /* exists */ or certificate 1[field.1.2.840.113635.100.6.2.6] /* exists */ and certificate leaf[field.1.2.840.113635.100.6.1.13] /* exists */ and certificate leaf[subject.OU] = "${subject_OU}")
library => anchor apple'''

class Signer:
	def __init__(self, cfgFile):
		with open(cfgFile) as file:
			content = file.read()
			self.cfg = json.loads(content)

	@staticmethod
	def cmd(args, cwd = None):
		'''
		Executes the requested program and throws an exception if the program returns a non-zero return code.
		'''
		ret = subprocess.Popen(args, cwd = cwd).wait()
		if ret != 0:
			raise Exception('command "%s" exited with exit status %i' % (args[0], ret))

	def certificateSubjectOU(self):
		'''
		Extracts the subject OU from the Application DeveloperID certificate that is specified in the script's configuration.
		'''
		findCert = subprocess.Popen(('/usr/bin/security', 'find-certificate',
									 '-c', self.cfg['developer-id-app'],
									 '-p', self.cfg['keychain']),
									 stdout = subprocess.PIPE, text = True)
		pem, _ = findCert.communicate()

		openssl = subprocess.Popen(('/usr/bin/openssl', 'x509', '-subject', '-noout'),
									stdout = subprocess.PIPE, stdin = subprocess.PIPE, text = True)
		subject, _ = openssl.communicate(pem)

		start = subject.split('/OU=', 1)[1]
		end = start.index('/')

		return start[:end]

	@staticmethod
	def lookupFileIdentifier(file):
		'''
		Looks up a bundle identifier suitable for use when signing the app bundle or binary.
		'''
		try:
			d = plistlib.load(os.path.join(file, 'Contents', 'Info.plist'))
			return d['CFBundleIdentifier']
		except:
			return os.path.basename(file)

	def codesign(self, files, entitlements = None):
		'''
		Calls the codesign executable on the signable object(s) in the specified directory.
		'''
		OU = self.certificateSubjectOU()

		if hasattr(files, 'isalpha'):
			files = (files,)

		for file in files:
			identifier = self.lookupFileIdentifier(file)
			reqs = string.Template(requirements).substitute({
				'identifier': identifier,
				'subject_OU': OU,
			})

			print("Identifier:", identifier)
			print("Subject_OU:", OU)
			print("Reqs:", reqs)

			args = ['-vvvv', '--force',
					'-r=' + reqs,
					'--identifier', identifier,
					'--keychain', self.cfg['keychain'],
					'--sign', self.cfg['developer-id-app']]

			if entitlements:
				args += ['--options', 'runtime',
						 '--entitlements', entitlements]

			self.cmd(['codesign'] + args + [file])

		return 0

	def prodsign(self, inFile, outFile):
		'''
		Calls the productsign executable to sign the specified product, outputting it signed.
		'''
		self.cmd(['productsign',
				  '--keychain', self.cfg['keychain'],
				  '--sign', self.cfg['developer-id-installer'],
				  inFile, outFile])

	@staticmethod
	def volNameForMountedDMG(mountPoint):
		diskutil = subprocess.Popen(['diskutil', 'info', '-plist', mountPoint], stdout = subprocess.PIPE)
		plist, _ = diskutil.communicate()
		fileLikePlist = io.BytesIO(plist)
		diskInfo = plistlib.load(fileLikePlist)
		return diskInfo['VolumeName']

	@staticmethod
	def extractDMG(file, workDir):
		'''
		Extracts the specified DMG into the "content" subdirectory of workDir.
		It returns the volume name of the extracted DMG.
		'''
		mountDir = os.path.join(workDir, 'mount')
		os.mkdir(mountDir)
		Signer.cmd(['hdiutil', 'mount', '-readonly', '-mountpoint', mountDir, file])
		volName = Signer.volNameForMountedDMG(mountDir)
		contentDir = os.path.join(workDir, 'content')
		os.mkdir(contentDir)
		for file in os.listdir(mountDir):
			if file == '.Trashes':
				continue

			src = os.path.join(mountDir, file)
			dst = os.path.join(contentDir, file)

			if os.path.islink(src):
				target = os.readlink(src)
				os.symlink(target, dst)
			elif os.path.isdir(src):
				shutil.copytree(src, dst, True)
			else:
				shutil.copy(src, dst)

		Signer.cmd(['umount', mountDir])

		return volName

	@staticmethod
	def extractPKG(file, workDir):
		'''
		Expands the specified PKG into workDir.
		'''
		Signer.cmd(['pkgutil', '--expand-full', file, workDir])

	def signApp(self, workDir, entitlements = None):
		'''
		Signs the app bundle in the "content" subdirectory of workDir.
		'''
		app = os.path.join(workDir, 'content', 'Mumble.app')

		binaries = []
		binariesDir = os.path.join(app, 'Contents', 'MacOS')
		for binary in os.listdir(binariesDir):
			binaries.append(os.path.join(binariesDir, binary))

		codecs = []
		codecsDir = os.path.join(app, 'Contents', 'Codecs')
		for codec in os.listdir(codecsDir):
			codecs.append(os.path.join(codecsDir, codec))

		plugins = []
		pluginsDir = os.path.join(app, 'Contents', 'Plugins')
		for plugin in os.listdir(pluginsDir):
			plugins.append(os.path.join(pluginsDir, plugin))

		self.codesign(binaries)
		self.codesign(codecs)
		self.codesign(plugins)

		overlayInst = os.path.join(app, 'Contents', 'Resources', 'MumbleOverlay.pkg')
		os.rename(overlayInst, overlayInst + '.intermediate')
		self.prodsign(overlayInst + '.intermediate', overlayInst)
		os.remove(overlayInst + '.intermediate')

		self.codesign(app, entitlements)

	def signOverlay(self, workDir):
		'''
		Extracts "MumbleOverlay.pkg" (which is extracted from the app bundle),
		signs the relevant binaries and then repacks it.
		'''
		app = os.path.join(workDir, 'content', 'Mumble.app')

		overlayPKG = os.path.join(app, 'Contents', 'Resources', 'MumbleOverlay.pkg')
		overlayDir = os.path.join(workDir, 'MumbleOverlay')

		self.extractPKG(overlayPKG, overlayDir)

		binaries = []
		binariesDir = os.path.join(overlayDir, 'net.sourceforge.mumble.OverlayScriptingAddition.pkg',
											   'Payload', 'MumbleOverlay.osax',
											   'Contents', 'MacOS')
		for binary in os.listdir(binariesDir):
			binaries.append(os.path.join(binariesDir, binary))

		self.codesign(binaries)

		self.makePKG(overlayDir, overlayPKG)

	@staticmethod
	def makeDMG(workDir, volName, outFile):
		'''
		Makes a new DMG for the Mumble app that resides in the workDir's content subdirectory.
		'''
		Signer.cmd(['hdiutil', 'create', '-srcfolder', os.path.join(workDir, 'content'),
					'-format', 'UDBZ', '-size', '1g', '-volname', volName, outFile])

	@staticmethod
	def makePKG(workDir, file):
		'''
		Flattens workDir's content into a PKG file.
		'''
		Signer.cmd(['pkgutil', '--flatten-full', workDir, file])

def main():
	p = argparse.ArgumentParser(usage='sign_macOS.py --input=<in.dmg> --output=<out.dmg> [--keep-tree]')
	p.add_argument('-c', '--config', help = 'Configuration file', default = os.path.join(pathlib.Path.home(), '.sign_macOS.cfg'))
	p.add_argument('-e', '--entitlements', help = 'Entitlements file')
	p.add_argument('-i', '--input', help = 'Input file')
	p.add_argument('-o', '--output', help = 'Output file')
	p.add_argument('-kt', '--keep-tree', action = 'store_true', dest = 'keepTree',
				   help = 'Keep the working tree after signing')
	args = p.parse_args()

	if not args.input:
		print('No input specified')
		sys.exit(1)

	if not args.output:
		print('No output specified')
		sys.exit(1)

	signer = Signer(args.config)

	inFile = os.path.abspath(args.input)
	outFile = os.path.abspath(args.output)
	workDir = tempfile.mkdtemp()

	print('Input: ' + inFile)
	print('Output: ' + outFile)
	print('Working dir: ' + workDir + '\n')

	if inFile.lower().endswith('.dmg'):
		volName = signer.extractDMG(inFile, workDir)

		signer.signOverlay(workDir)
		signer.signApp(workDir, args.entitlements)

		signer.makeDMG(workDir, volName, outFile)

		signer.codesign(outFile)
	elif inFile.lower().endswith('.pkg'):
		signer.extractPKG(inFile, workDir)

		files = []
		for file in os.listdir(workDir):
			files.append(os.path.join(workDir, file))

		signer.codesign(files)

		signer.makePKG(workDir, outFile)

		os.rename(outFile, outFile + '.intermediate')
		signer.prodsign(outFile + '.intermediate', outFile)
		os.remove(outFile + '.intermediate')
	else:
		shutil.copy(inFile, outFile)
		signer.codesign(outFile)

	print('')

	if not args.keepTree:
		shutil.rmtree(workDir, ignore_errors = True)
		print('Working tree removed\n')

	print('Signed file available at ' + args.output)

if __name__ == '__main__':
	main()
