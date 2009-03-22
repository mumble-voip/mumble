#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Simple Mac OS X Application Bundler for Mumble
#
# Loosely based on original bash-version by Sebastian Schlingmann (based, again, on a OSX application bundler
# by Thomas Keller).
#

import sys, os, string, re, shutil, plistlib, tempfile, exceptions, datetime
from subprocess import Popen, PIPE
from optparse import OptionParser

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

			if os.path.isfile(lib) and self.universal and not self.is_universal_binary(lib):
				raise self.UniversalBinaryException("Library '%s' is not an Universal Binary. Aborting." % lib)

			if self.is_system_lib(lib):
				continue

			# At the moment, the only real framework Mumble uses, that isn't already distributed with Mac OS
			# X, is Qt. And simply copying the Qt dylibs work just fine. This allows us to get rid of all of
			# that framework madness in this script (for now, at least... hopefully forever! :-))
			else:
				baselib = os.path.basename(lib)
				dst = os.path.join(fwpath, baselib)
				self.change_lib_reference(macho, lib, baselib)

				if os.path.basename(macho) == os.path.basename(lib):
					continue
				if not os.path.exists(dst):
					shutil.copy(lib, dst)
				self.handle_binary_libs(dst)

	def copy_murmur(self):
		'''
			Copy the murmurd binary into our Mumble app bundle
		'''
		print ' * Copying murmurd binary'
		src = os.path.join(self.bundle, '..', 'murmurd')
		dst = os.path.join(self.bundle, 'Contents', 'MacOS', 'murmurd')

		# Is it universal?
		if self.universal and not self.is_universal_binary(src):
			raise self.UniversalBinaryException("Murmur executable is not an Universal Binary. Aborting.")

		shutil.copy(src, dst)

		print ' * Copying murmurd configuration'
		dst = os.path.join(self.bundle, 'Contents', 'MacOS', 'murmur.ini')
		shutil.copy('scripts/murmur.ini.osx', dst)

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
		print ' * Copying positional audio plugins'
		dst = os.path.join(self.bundle, 'Contents', 'Plugins')
		if os.path.exists(dst):
			shutil.rmtree(dst)
		shutil.copytree('release/plugins/', dst, symlinks=True)

	def copy_qt_plugins(self):
		'''
			Copy over any needed Qt plugins.
		'''
		print ' * Copying Qt plugins'
		qtplugindir = os.path.join(self.bundle, 'Contents', 'QtPlugins')
		if not os.path.exists(qtplugindir):
			os.mkdir(qtplugindir)

	def update_plist(self):
		'''
			Modify our bundle's Info.plist to make it ready for release.
		'''
		if self.version is not None:
			print ' * Changing version in Info.plist'
			p = self.infoplist
			p['CFBundleVersion'] = self.version
			plistlib.writePlist(p, self.infopath)

	def done(self):
		print ' * Done!'
		print ''

	def __init__(self, bundle, version=None):
		self.bundle = bundle
		self.version = version
		self.infopath = os.path.join(os.path.abspath(bundle), 'Contents', 'Info.plist')
		self.infoplist = plistlib.readPlist(self.infopath)
		self.binary = os.path.join(os.path.abspath(bundle), 'Contents', 'MacOS', self.infoplist['CFBundleExecutable'])
		self.universal = self.is_universal_binary(self.binary)
		print ' * Preparing AppBundle'
		if not self.universal:
			print ' * Main executable not universal. Disabling universal binary checks.'


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
		os.mkdir(adst)


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


class PackageMaker(FolderObject):

	def __init__(self, filename, id, title, version):
		FolderObject.__init__(self)
		print 'Preparing to create package installer'
		self.filename = filename
		self.id = id
		self.title = title
		self.version = version

	def create(self):
		'''
			Create the .pkg installer
		'''
		print ' * Creating installer. Please wait...'
		if os.path.exists(self.filename):
			shutil.rmtree(self.filename)
		p = Popen(['/Developer/usr/bin/packagemaker',
		           '--root',     self.tmp,
		           '--id',       self.id,
		           '--title',    self.title,
		           '--version',  self.version,
		           '--target',   '10.4',
		           '--scripts',  'installer_macx/scripts/',
		           '--out',      self.filename])
		retval = p.wait()
		print ' * Removing temporary directory.'
		shutil.rmtree(self.tmp)
		print ' * Done!'


if __name__ == '__main__':

	parser = OptionParser()
	parser.add_option('', '--release', dest='release', help='Build a release. This determines the version number of the release.')
	parser.add_option('', '--snapshot', dest='snapshot', help='Build a snapshot release. This determines the \'snapshot version\'.')
	parser.add_option('', '--only-appbundle', dest='only_appbundle', help='Only prepare the appbundle. Do not package.', action='store_true', default=False)

	options, args = parser.parse_args()

	# Release
	if options.release:
		ver = options.release
		fn = 'release/Mumble-%s.dmg' % ver
		title = 'Mumble %s' % ver
	# Snapshot
	elif options.snapshot:
		n = datetime.datetime.now()
		d = n.strftime('%F-%H%M')
		ver = options.snapshot
		fn = 'release/Mumble-Snapshot-%s-%s.dmg' % (d, ver)
		title = 'Mumble Snapshot %s (%s)' % (ver, d)
	else:
		print 'Neither snapshot or release selected. Bailing.'
		sys.exit(1)

	if not os.path.exists('release'):
		print 'This script needs to be run from the root of the Mumble source tree.'
		sys.exit(1)

	# Fix .ini files
	os.system('cd scripts && sh mkini.sh')

	# Do the finishing touches to our Application bundle before release
	a = AppBundle('release/Mumble.app', ver)
	a.copy_murmur()
	a.handle_libs()
	a.copy_resources(['icons/mumble.icns'])
	a.copy_plugins()
	a.copy_qt_plugins()
	a.update_plist()
	a.done()

	if options.only_appbundle:
		sys.exit(0)

	# Prepare the base installer .pkg
	f = PackageMaker('release/Mumble-Base.pkg', 'net.sourceforge.mumble.base', 'Mumble Base', ver)
	f.mkdir('/Applications/')
	f.copy('release/Mumble.app', '/Applications/Mumble.app')
	f.mkdir('/Library/')
	f.mkdir('/Library/MumbleOverlay/')
	f.mkdir('/Library/MumbleOverlay/Bundles/')
	f.copy('release/mumble-overlay-injector', '/Library/MumbleOverlay/')
	f.copy('release/mumble-overlay', '/Library/MumbleOverlay/')
	f.copy('release/Stub.framework', '/Library/MumbleOverlay/Bundles/')
	f.copy('release/Overlay.framework', '/Library/MumbleOverlay/Bundles/')
	f.copy('release/MumbleOverlayContextMenu.framework', '/Library/Contextual Menu Items/')
	f.create()

	# Combine the base installer with our pretty installer wrapper
	p = Popen(['/Developer/usr/bin/packagemaker',
	           '--doc',    'installer_macx/MumbleInstaller.pmdoc',
	           '--id',     'net.sourceforge.mumble',
	           '--out',    'release/Install Mumble.mpkg'])
	if p.wait() != 0:
		print 'Creating master installer failed.'
		sys.exit(1)
	# A bug in PackageMaker? It doesn't copy over the script itself.
	shutil.copy('installer_macx/scripts/postflight', 'release/Install Mumble.mpkg/Contents/Packages/Mumble-Base.pkg/Contents/Resources/postflight')

	# Create diskimage
	d = DiskImage(fn, title)
	d.copy('release/Install Mumble.mpkg')
	d.copy('README', '/ReadMe.txt')
	d.copy('CHANGES', '/Changes.txt')
	d.copy('installer_macx/DS_Store', '/.DS_Store')
	d.mkdir('Licenses')
	d.copy('LICENSE', '/Licenses/Mumble.txt')
	d.copy('installer/lgpl.txt', '/Licenses/Qt.txt')
	d.copy('installer/speex.txt', '/Licenses/Speex.txt')
	d.copy('installer/portaudio.txt', '/Licenses/PortAudio.txt')
	d.copy('installer/gpl.txt', '/Licenses/ZeroC-Ice.txt')
	d.mkdir('Murmur')
	d.copy('scripts/murmur.ini.osx', '/Murmur/murmur.ini')
	d.copy('scripts/murmur.conf', '/Murmur/')
	d.copy('scripts/dbusauth.pl', '/Murmur/')
	d.copy('scripts/murmur.pl', '/Murmur/')
	d.copy('scripts/weblist.pl', '/Murmur/')
	d.copy('scripts/weblist.php', '/Murmur/')
	d.copy('scripts/icedemo.php', '/Murmur/')
	d.copy('scripts/ListUsers.cs', '/Murmur/')
	d.copy('scripts/mumble-auth.py', '/Murmur/')
	d.copy('scripts/rubytest.rb', '/Murmur')
	d.copy('scripts/simpleregister.php', '/Murmur/')
	d.copy('scripts/testcallback.py', '/Murmur/')
	d.copy('scripts/testauth.py', '/Murmur/')
	d.copy('scripts/addban.php', '/Murmur/')
	d.copy('scripts/php.ini', '/Murmur/')
	d.copy('src/murmur/Murmur.ice', '/Murmur/')
	d.create()
