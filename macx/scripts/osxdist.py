#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Simple Mac OS X Application Bundler for Mumble
#
# Loosely based on original bash-version by Sebastian Schlingmann (based, again, on a OSX application bundler
# by Thomas Keller).
#

import sys, os, string, re, shutil, plistlib, tempfile, exceptions, datetime, tarfile
from subprocess import Popen, PIPE
from optparse import OptionParser

options = None

def gitrev():
	'''Get git revision of the current Mumble build.'''
	return os.popen('git describe').read()[:-1]

def codesign(path):
	'''Call the codesign executable.'''

	if hasattr(path, 'isalpha'):
		path = (path,)
	for p in path:
		p = Popen(('codesign', '--keychain', options.codesign_keychain, '--signature-size', '6400', '-vvvv', '-s', options.codesign, p))
		retval = p.wait()
		if retval != 0:
			return retval
	return 0

def prodsign(inf, outf):
	'''Call the prodsign executable.'''

	p = Popen(('productsign', '--keychain', options.codesign_keychain, '--sign', options.codesign_installer, inf, outf))
	retval = p.wait()
	if retval != 0:
		return retval
	return 0

def create_overlay_package():
	print '* Creating overlay installer'

	bundle = os.path.join('release', 'MumbleOverlay.osax')
	overlaylib = os.path.join('release', 'libmumbleoverlay.dylib')
	if options.codesign:
		codesign(bundle)
		codesign(overlaylib)
	os.system('./macx/scripts/build-overlay-installer')
	if options.codesign:
		os.rename('release/MumbleOverlay.pkg', 'release/MumbleOverlayUnsigned.pkg')
		prodsign('release/MumbleOverlayUnsigned.pkg', 'release/MumbleOverlay.pkg')

class AppBundle(object):

	def copy_murmur(self):
		'''
			Copy the murmurd binary into our Mumble app bundle
		'''
		print ' * Copying murmurd binary'
		src = os.path.join(self.bundle, '..', 'murmurd')
		dst = os.path.join(self.bundle, 'Contents', 'MacOS', 'murmurd')
		shutil.copy(src, dst)

		print ' * Copying murmurd configuration'
		dst = os.path.join(self.bundle, 'Contents', 'MacOS', 'murmur.ini')
		shutil.copy('scripts/murmur.ini.osx', dst)

	def copy_g15helper(self):
		'''
			Copy the Mumble G15 helper daemon into our Mumble app bundle.
		'''
		if os.path.exists(os.path.join(self.bundle, '..', 'mumble-g15-helper')):
			print ' * Copying G15 helper'
			src = os.path.join(self.bundle, '..', 'mumble-g15-helper')
			dst = os.path.join(self.bundle, 'Contents', 'MacOS', 'mumble-g15-helper')
			shutil.copy(src, dst)
		else:
			print ' * No G15 helper found, skipping...'

	def copy_resources(self, rsrcs):
		'''
			Copy needed resources into our bundle.
		'''
		print ' * Copying needed resources'
		rsrcpath = os.path.join(self.bundle, 'Contents', 'Resources')
		if not os.path.exists(rsrcpath):
			os.mkdir(rsrcpath)

		# Copy resources already in the bundle
		for rsrc in rsrcs:
			b = os.path.basename(rsrc)
			if os.path.isdir(rsrc):
	                        shutil.copytree(rsrc, os.path.join(rsrcpath, b), symlinks=True)
			elif os.path.isfile(rsrc):
				shutil.copy(rsrc, os.path.join(rsrcpath, b))

		# Extras
		shutil.copy('release/MumbleOverlay.pkg', os.path.join(rsrcpath, 'MumbleOverlay.pkg'))

	def copy_codecs(self):
		'''
			Copy over dynamic CELT libraries.
		'''
		print ' * Copying CELT libraries.'
		dst = os.path.join(self.bundle, 'Contents', 'Codecs')
		os.makedirs(dst)
		shutil.copy('release/libcelt0.0.7.0.dylib', dst)
		shutil.copy('release/libcelt0.0.11.0.dylib', dst)

	def copy_plugins(self):
		'''
			Copy over any built Mumble plugins.
		'''
		print ' * Copying positional audio plugins'
		dst = os.path.join(self.bundle, 'Contents', 'Plugins')
		if os.path.exists(dst):
			shutil.rmtree(dst)
		shutil.copytree('release/plugins/', dst, symlinks=True)

	def update_plist(self):
		'''
			Modify our bundle's Info.plist to make it ready for release.
		'''
		if self.version is not None:
			print ' * Changing version in Info.plist'
			p = self.infoplist
			p['CFBundleVersion'] = self.version
			plistlib.writePlist(p, self.infopath)

	def add_compat_warning(self):
		'''
			Add compat binary for when our binary is run on i386 or ppc.
			The compat binary displays a warning dialog telling the user that they need to download a universal version of Mumble
		'''
		print ' * Splicing Mumble.compat into main bundle executable'
		os.system('lipo -create release/Mumble.compat -arch x86_64 %s -output %s' % (self.binary, self.binary))

	def set_min_macosx_version(self, version):
		'''
			Set the minimum version of Mac OS X version that this App will run on.
		'''
		print ' * Setting minimum Mac OS X version to: %s' % (version)
		self.infoplist['LSMinimumSystemVersion'] = version

	def done(self):
		plistlib.writePlist(self.infoplist, self.infopath)
		print ' * Done!'
		print ''

	def __init__(self, bundle, version=None):
		self.framework_path = ''
		self.handled_libs = {}
		self.bundle = bundle
		self.version = version
		self.infopath = os.path.join(os.path.abspath(bundle), 'Contents', 'Info.plist')
		self.infoplist = plistlib.readPlist(self.infopath)
		self.binary = os.path.join(os.path.abspath(bundle), 'Contents', 'MacOS', self.infoplist['CFBundleExecutable'])
		print ' * Preparing AppBundle'


class FolderObject(object):

	class Exception(exceptions.Exception):
		pass

	def __init__(self):
		self.tmp = tempfile.mkdtemp()

	def copy(self, src, dst='/'):
		'''
			Copy a file or directory into the folder.
		'''
		asrc = os.path.abspath(src)

		if dst[0] != '/':
			raise self.Exception

		# Determine destination
		if dst[-1] == '/':
			adst = os.path.abspath(self.tmp + '/' + dst + os.path.basename(src))
		else:
			adst = os.path.abspath(self.tmp + '/' + dst)

		if os.path.isdir(asrc):
			print ' * Copying directory: %s' % os.path.basename(asrc)
			shutil.copytree(asrc, adst, symlinks=True)
		elif os.path.isfile(asrc):
			print ' * Copying file: %s' % os.path.basename(asrc)
			shutil.copy(asrc, adst)

	def symlink(self, src, dst):
		'''
			Create a symlink inside the folder.
		'''
		asrc = os.path.abspath(src)
		adst = self.tmp + '/' + dst
		print ' * Creating symlink %s' % os.path.basename(asrc)
		os.symlink(asrc, adst)

	def mkdir(self, name):
		'''
			Create a directory inside the folder.
		'''
		print ' * Creating directory %s' % os.path.basename(name)
		adst = self.tmp + '/'  + name
		os.makedirs(adst)


class DiskImage(FolderObject):

	def __init__(self, filename, volname):
		FolderObject.__init__(self)
		print ' * Preparing to create diskimage'
		self.filename = filename
		self.volname = volname

	def create(self):
		'''
			Create the disk image itself.
		'''
		print ' * Creating diskimage. Please wait...'
		if os.path.exists(self.filename):
			shutil.rmtree(self.filename)
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
	parser = OptionParser()
	parser.add_option('', '--release', dest='release', help='Build a release. This determines the version number of the release.')
	parser.add_option('', '--snapshot', dest='snapshot', help='Build a snapshot release. This determines the \'snapshot version\'.')
	parser.add_option('', '--git', dest='git', help='Build a snapshot release. Use the git revision number as the \'snapshot version\'.', action='store_true', default=False)
	parser.add_option('', '--universal', dest='universal', help='Build an universal snapshot.', action='store_true', default=False)
	parser.add_option('', '--only-appbundle', dest='only_appbundle', help='Only prepare the appbundle. Do not package.', action='store_true', default=False)
	parser.add_option('', '--only-overlay', dest='only_overlay', help='Only create the overlay installer.', action='store_true', default=False)
	parser.add_option('', '--codesign', dest='codesign', help='Identity to use for code signing. (If not set, no code signing will occur)')
	parser.add_option('', '--codesign-installer', dest='codesign_installer', help='Identity to use for code signing installer packages. (Implies --codesign)')
	parser.add_option('', '--codesign-keychain', dest='codesign_keychain', help='The keychain to use when invoking the codesign utility. (Defaults to login.keychain', default='login.keychain')
	parser.add_option('', '--no-server', dest='no_server', help='Exclude Murmur-related files from disk image.', action='store_true', default=False)

	options, args = parser.parse_args()

	# Release
	if options.release:
		ver = options.release
		if options.universal:
			fn = 'release/Mumble-Universal-%s.dmg' % ver
			title = 'Mumble %s (Universal) ' %ver
		else:
			fn = 'release/Mumble-%s.dmg' % ver
			title = 'Mumble %s ' % ver
	# Snapshot
	elif options.snapshot or options.git:
		if not options.git:
			ver = options.snapshot
		else:
			ver = gitrev()	
		if options.universal:
			fn = 'release/Mumble-Universal-Snapshot-%s.dmg' % ver
			title = 'Mumble Snapshot %s (Universal)' % ver
		else:
			fn = 'release/Mumble-Snapshot-%s.dmg' % ver
			title = 'Mumble Snapshot %s' % ver
	else:
		print 'Neither snapshot or release selected. Bailing.'
		sys.exit(1)

	if not os.path.exists('release'):
		print 'This script needs to be run from the root of the Mumble source tree.'
		sys.exit(1)

	# Fix overlay installer package
	create_overlay_package()
	if options.only_overlay:
		sys.exit(0)

	# Fix .ini files
	os.system('cd scripts && sh mkini.sh')

	# Do the finishing touches to our Application bundle before release
	a = AppBundle('release/Mumble.app', ver)
	if not options.no_server:
		a.copy_murmur()
	a.copy_g15helper()
	a.copy_codecs()
	a.copy_plugins()
	a.copy_resources(['icons/mumble.icns'])
	a.update_plist()
	if not options.universal:
		a.add_compat_warning()
		a.set_min_macosx_version('10.6.0')
	else:
		a.set_min_macosx_version('10.4.8')
	a.done()

	# Sign our binaries, etc.
	if options.codesign:
		print ' * Signing binaries with identity `%s\'' % options.codesign
		binaries = [
			# 1.2.x
			'release/Mumble.app',
			'release/Mumble.app/Contents/MacOS/mumble-g15-helper',
			'release/Mumble.app/Contents/Plugins/liblink.dylib',
			'release/Mumble.app/Contents/Plugins/libmanual.dylib',
			'release/Mumble.app/Contents/Codecs/libcelt0.0.7.0.dylib',
			'release/Mumble.app/Contents/Codecs/libcelt0.0.11.0.dylib',
		]
		if not options.no_server:
			binaries.append('release/Mumble.app/Contents/MacOS/murmurd')

		codesign(binaries)
		print ''

	if options.only_appbundle:
		sys.exit(0)

	# Create diskimage
	d = DiskImage(fn, title)
	d.copy('macx/scripts/DS_Store', '/.DS_Store')
	d.symlink('/Applications', '/Applications')
	d.copy('release/Mumble.app')
	d.create()
