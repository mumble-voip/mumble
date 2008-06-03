#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Simple Mac OS X Application Bundler for Mumble
#
# Loosely based on original bash-version by
# Sebastian Schlingmann (based, again, on a OSX
# application bundler by Thomas Keller).
#

import sys, os, string, re, shutil, plistlib, tempfile, exceptions, datetime
from subprocess import Popen, PIPE

class AppBundle(object):

	class UniversalBinaryException(exceptions.Exception):
		pass

	def is_universal_binary(self, bin):
		'''
			Determine whether a binary is universal or not.
		'''
		families = (('i386'), ('ppc', 'ppc7400'))
		out = Popen(['lipo', '-info', bin], stdout=PIPE, stderr=PIPE).communicate()[0]
		if out[:29] == 'Architectures in the fat file':
			archs = out.replace('\n', '').split('are: ')[1].strip().split(' ')
			nmatches = 0
			for arch in archs:
				for family in families:
					if arch in family:
						nmatches += 1
			return len(families) == nmatches
		return False

	def change_lib_reference(self, bin, old, new):
		'''
			Do some Mach-O voodoo.
		'''
		# Change identification
		if os.path.basename(bin) == new:
			p = Popen(['install_name_tool', '-id', '@executable_path/../Frameworks/'+new, bin])
		# Change dependancy
		else:
			p = Popen(['install_name_tool', '-change', old, '@executable_path/../Frameworks/'+new, bin])
		return p.wait()

	def is_system_lib(self, lib):
		'''
			Is the library a system library, meaning that we should not include it in our bundle?
		'''
		system_libs = (
			re.compile('^/System/Library/.*$'),
			re.compile('^/usr/lib/.*$'),
		)
		for l in system_libs:
			if l.match(lib) is not None:
				return True
		return False

	def get_binary_libs(self, path):
		'''
			Get a list of libraries that we depend on.
		'''
		m = re.compile('^\t(.*)\ \(.*$')
		libs = Popen(['otool', '-L', path], stdout=PIPE).communicate()[0]
		libs = string.split(libs, '\n')
		ret = []
		for line in libs:
			g = m.match(line)
			if g is not None:
				ret.append(g.groups()[0])
		return ret

	def handle_libs(self):
		'''
			Copy non-system libraries that we depend on into our bundle, and fix linker
			paths so they are relative to our bundle.
		'''
		print ' * Taking care of libraries'

		# Does our fwpath exist?
		fwpath = os.path.join(os.path.abspath(self.bundle), 'Contents', 'Frameworks')
		if not os.path.exists(fwpath):
			os.mkdir(fwpath)

		self.handle_binary_libs()
		self.handle_binary_libs(os.path.join(os.path.abspath(self.bundle), 'Contents', 'MacOS', 'murmurd'))

	def handle_binary_libs(self, macho=None):
		'''
			Fix up dylib depends for a specific binary.
		'''
		# Does our fwpath exist?
		fwpath = os.path.join(os.path.abspath(self.bundle), 'Contents', 'Frameworks')
		if not os.path.exists(fwpath):
			os.mkdir(fwpath)

		if macho is None:
			macho = os.path.abspath(self.binary)
		else:
			macho = os.path.abspath(macho)

		libs = self.get_binary_libs(macho)
		for lib in libs:

			if os.path.isfile(lib) and not self.is_universal_binary(lib):
				raise self.UniversalBinaryException("Library '%s' is not an Universal Binary. Aborting." % lib)

			if self.is_system_lib(lib):
				continue

			# At the moment, the only real framework Mumble uses, that isn't
			# already distributed with Mac OS X, is Qt. And simply copying the
			# Qt dylibs work just fine. This allows us to get rid of all of that
			# framework madness in this script (for now, at least... hopefully
			# forever! :-))
			else:
				baselib = os.path.basename(lib)
				dst = os.path.join(fwpath, baselib)
				self.change_lib_reference(macho, lib, baselib)

				# If we were run on an executable
				if not macho or macho.find(fwpath) == -1:
					if dst == lib:
						continue
					if not os.path.exists(dst):
						shutil.copy(lib, dst)
					self.handle_binary_libs(dst)

	def copy_murmur(self):
		'''
			Copy the murmurd binary into our Mumble app bundle
		'''
		print ' * Copying murmurd binary'
		src = os.path.join(self.bundle, '..', 'murmurd.app', 'Contents', 'MacOS', 'murmurd')
		dst = os.path.join(self.bundle, 'Contents', 'MacOS', 'murmurd')

		# Is it universal?
		if not self.is_universal_binary(src):
			raise self.UniversalBinaryException("Murmur executable is not an Universal Binary. Aborting.")

		shutil.copy(src, dst)

	def copy_resources(self, rsrcs):
		'''
			Copy needed resources into our bundle.
		'''
		print ' * Copying needed resources'
		rsrcpath = os.path.join(self.bundle, 'Contents', 'Resources')
		if not os.path.exists(rsrcpath):
			os.mkdir(rsrcpath)
		for rsrc in rsrcs:
			b = os.path.basename(rsrc)
			shutil.copy(rsrc, os.path.join(rsrcpath, b))

	def copy_plugins(self):
		'''
			Copy over any built Mumble plugins.
		'''
		print ' * Copying plugins'
		dst = os.path.join(self.bundle, 'Contents', 'Plugins')
		if os.path.exists(dst):
			shutil.rmtree(dst)
		shutil.copytree('release/plugins/', dst)

	def setup_plist(self):
		'''
			Modify our bundle's Info.plist to make it ready for release.
		'''
		p = self.infoplist
		p['CFBundleGetInfoString'] = 'An open source, low-latency, high quality voice chat software primarily intended for use while gaming.'
		p['CFBundleIdentifier'] = 'net.sourceforge.mumble'
		p['CFBundleVersion'] = self.version
		p['CFBundleURLTypes'] = [{
			# This breaks because OS X doesn't allow us to use the
			# app icon for a protocol:
			#'CFBundleURLIconFile': 'mumble.icns',
			'CFBundleURLName': 'Mumble Server URL',
			'CFBundleURLSchemes': ['mumble'],
		}]
		p['NSHumanReadableCopyright'] = 'Copyright (c) 2005-2008 Thorvald Natvig <slicer@users.sourceforge.net>'
		plistlib.writePlist(p, self.infopath)

	def done(self):
		print ' * Done!'
		print ''

	def __init__(self, bundle, version):
		self.bundle = bundle
		self.infopath = os.path.join(os.path.abspath(bundle), 'Contents', 'Info.plist')
		self.infoplist = plistlib.readPlist(self.infopath)
		self.binary = os.path.join(os.path.abspath(bundle), 'Contents', 'MacOS', self.infoplist['CFBundleExecutable'])
		if not self.is_universal_binary(self.binary):
			raise self.UniversalBinaryException("Main application executable is not an Universal Binary. Aborting.")
		self.version = version
		print ' * Perparing AppBundle'


class DiskImage(object):

	class Bargh(exceptions.Exception):
		pass

	def __init__(self, filename, volname):
		print ' * Preparing to create diskimage'
		self.filename = filename
		self.volname = volname
		self.tmp = tempfile.mkdtemp()

	def copy(self, src, dst='/'):
		'''
			Copy a file or directory into the disk image.
		'''
		asrc = os.path.abspath(src)

		if dst[0] != '/':
			raise self.Blargh

		# Determine destination
		if dst[-1] == '/':
			adst = os.path.abspath(self.tmp + '/' + dst + os.path.basename(src))
		else:
			adst = os.path.abspath(self.tmp + '/' + dst)

		if os.path.isdir(asrc):
			print ' * Copying directory: %s' % os.path.basename(asrc)
			shutil.copytree(asrc, adst)
		elif os.path.isfile(asrc):
			print ' * Copying file: %s' % os.path.basename(asrc)
			shutil.copy(asrc, adst)

	def symlink(self, src, dst):
		'''
			Create a symlink inside the disk image.
		'''
		asrc = os.path.abspath(src)
		adst = os.path.join(self.tmp, dst)
		print ' * Creating symlink %s' % os.path.basename(asrc)
		os.symlink(asrc, adst)

	def mkdir(self, name):
		'''
			Create a directory inside the disk image.
		'''
		print ' * Creating directory %s' % os.path.basename(name)
		adst = os.path.join(self.tmp, name)
		os.mkdir(adst)

	def create(self):
		'''
			Create the disk image itself.
		'''
		print ' * Creating diskimage. Please wait...'
		if os.path.exists(self.filename):
			os.remove(self.filename)
		p = Popen(['hdiutil', 'create',
		           '-srcfolder', self.tmp,
		           '-format', 'UDBZ',
		           '-volname', self.volname,
		           self.filename])
		retval = p.wait()
		print ' * Removing temporary directory.'
		shutil.rmtree(self.tmp)
		print ' * Done!'


if __name__ == '__main__':

	argc = len(sys.argv)

	# Release
	if argc > 1:
		ver = sys.argv[1]
		dmgfn = 'release/Mumble-%s.dmg' % ver
		dmgtitle = 'Mumble %s' % ver
	# Snapshot
	else:
		n = datetime.datetime.now()
		d = n.strftime('%F-%H%M')
		ver = 'Snapshot %s' % d
		dmgfn = 'release/Mumble-Snapshot-%s.dmg' % d 
		dmgtitle = 'Mumble Snapshot (%s)' %d

	# Do the finishing touches to our Application bundle before release
	a = AppBundle('release/Mumble.app', ver)
	a.copy_murmur()
	a.handle_libs()
	a.copy_resources(['icons/mumble.icns'])
	a.copy_plugins()
	a.setup_plist()
	a.done()

	# Prepare diskimage
	d = DiskImage(dmgfn, dmgtitle)
	d.copy('release/Mumble.app')
	d.copy('README', '/ReadMe.txt')
	d.copy('CHANGES', '/Changes.txt')
	d.copy('installer/DS_Store', '/.DS_Store')
	d.symlink('/Applications/', 'Applications')
	d.mkdir('Licenses')
	d.copy('installer/gpl.txt', '/Licenses/GPL.txt')
	d.copy('installer/qt.txt', '/Licenses/Qt.txt')
	d.copy('installer/speex.txt', '/Licenses/Speex.txt')
	d.mkdir('Murmur')
	d.copy('scripts/murmur.ini', '/Murmur/')
	d.copy('scripts/murmur.ini.system', '/Murmur/')
	d.copy('scripts/murmur.conf', '/Murmur/')
	d.copy('scripts/dbusauth.pl', '/Murmur/')
	d.copy('scripts/murmur.pl', '/Murmur/')
	d.copy('scripts/weblist.pl', '/Murmur/')
	d.create()
