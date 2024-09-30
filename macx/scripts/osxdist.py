#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

#
# Simple Mac OS X Application Bundler for Mumble
#
# Loosely based on original bash-version by Sebastian Schlingmann (based, again, on a OSX application bundler
# by Thomas Keller).
#

import sys, os, string, shutil, plistlib, tempfile, glob
from subprocess import Popen, PIPE
from optparse import OptionParser

options = None

def gitrev():
	'''Get git revision of the current Mumble build.'''
	return os.popen('git describe').read()[:-1]

def certificate_subject_OU(identity):
	'''Extract the subject OU from the certificate matching the identity parameter in the specified keychain.'''

	findCert = Popen(('/usr/bin/security', 'find-certificate', '-c', identity, '-p', options.keychain), stdout=PIPE)
	pem, _ = findCert.communicate()

	openssl = Popen(('/usr/bin/openssl', 'x509', '-subject', '-noout'), stdout=PIPE, stdin=PIPE)
	subject, _ = openssl.communicate(pem)

	attr = '/OU='
	begin = subject.index(attr) + len(attr)
	tmp = subject[begin:]
	end = tmp.index('/')

	return tmp[:end]

def lookup_file_identifier(path):
	try:
		d = plistlib.load(open(os.path.join(path, 'Contents', 'Info.plist'), "rb"))
		return d['CFBundleIdentifier']
	except:
		return os.path.basename(path)

def codesign(path):
	'''Call the codesign executable on the signable object at path.'''

	certname = 'Developer ID Application: %s' % options.developer_id
	OU = certificate_subject_OU(certname)

	if hasattr(path, 'isalpha'):
		path = (path,)
	for p in path:
		identifier = lookup_file_identifier(p)
		reqs = None
		with open(os.path.join(options.source_dir, 'macx/scripts/codesign-requirements.tmpl'), 'r') as f:
			tmplReqs = f.read()
			reqs = string.Template(tmplReqs).substitute({
				'identifier': identifier,
				'subject_OU': OU,
			})
		p = Popen(('codesign', '--keychain', options.keychain, '-vvvv', '-i', identifier, '-r='+reqs, '-s', certname, p))
		retval = p.wait()
		if retval != 0:
			return retval
	return 0

def prodsign(inf, outf):
	'''Call the prodsign executable.'''

	certname = 'Developer ID Installer: %s' % options.developer_id
	p = Popen(('productsign', '--keychain', options.keychain, '--sign', certname, inf, outf))
	retval = p.wait()
	if retval != 0:
		return retval
	return 0

def create_overlay_package():
	print('* Creating overlay installer')

	bundle = os.path.join(options.binary_dir, 'MumbleOverlay.osax')
	overlaylib = os.path.join(options.binary_dir, 'libmumbleoverlay.dylib')
	if options.developer_id:
		codesign(bundle)
		codesign(overlaylib)

	# Used as background in the installer
	shutil.copy(os.path.join(options.source_dir, 'icons/mumble.osx.installer.png'), os.path.join(options.binary_dir, 'bg.png'))

	p = Popen(os.path.join(os.path.abspath(options.source_dir), 'macx/scripts/build-overlay-installer'), cwd=options.binary_dir)
	retval = p.wait()
	if retval != 0:
		raise Exception('build-overlay-installer failed')
	if options.developer_id:
		os.rename(os.path.join(options.binary_dir, 'MumbleOverlay.pkg'), os.path.join(options.binary_dir, 'MumbleOverlayUnsigned.pkg'))
		prodsign(os.path.join(options.binary_dir, 'MumbleOverlayUnsigned.pkg'), os.path.join(options.binary_dir, 'MumbleOverlay.pkg'))

class AppBundle(object):
	def copy_helper(self, fn):
		'''
			Copy a helper binary into the Mumble app bundle.
		'''
		if os.path.exists(os.path.join(self.bundle, '..', fn)):
			print(' * Copying helper binary: %s' % fn)
			src = os.path.join(self.bundle, '..', fn)
			dst = os.path.join(self.bundle, 'Contents', 'MacOS', fn)
			shutil.copy(src, dst)

	def copy_resources(self, rsrcs):
		'''
			Copy needed resources into our bundle.
		'''
		print(' * Copying needed resources')
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
		shutil.copy(os.path.join(options.binary_dir, 'MumbleOverlay.pkg'), os.path.join(rsrcpath, 'MumbleOverlay.pkg'))

	def copy_codecs(self):
		'''
			Try to copy the dynamic audio codec libraries into the App Bundle.
		'''
		print(' * Attempting to copy audio codec libraries into App Bundle')
		dst = os.path.join(self.bundle, 'Contents', 'Codecs')
		if not os.path.exists(dst):
			os.makedirs(dst)
		codecs = (os.path.join(options.binary_dir, 'libcelt0.0.7.0.dylib'), os.path.join(options.binary_dir, 'libopus.dylib'))
		for codec in codecs:
			if os.path.exists(codec):
				shutil.copy(codec, dst)

	def copy_plugins(self):
		'''
			Copy over any built Mumble plugins.
		'''
		print(' * Copying (positional audio) plugins')
		dst = os.path.join(self.bundle, 'Contents', 'Plugins')
		if not os.path.exists(dst):
			os.makedirs(dst)
		for plugin in glob.glob(os.path.join(options.binary_dir, 'plugins') + '/*.dylib'):
			shutil.copy(plugin, dst)

	def update_plist(self):
		'''
			Modify our bundle's Info.plist to make it ready for release.
		'''
		if self.version is not None:
			print(' * Changing version in Info.plist')
			p = self.infoplist
			p['CFBundleVersion'] = self.version
			plistlib.dump(p, open(self.infopath, "wb"))

	def set_min_macosx_version(self, version):
		'''
			Set the minimum version of Mac OS X version that this App will run on.
		'''
		print(' * Setting minimum Mac OS X version to: %s' % (version))
		self.infoplist['LSMinimumSystemVersion'] = version

	def done(self):
		plistlib.dump(self.infoplist, open(self.infopath, "wb"))
		print(' * Done!')
		print()

	def __init__(self, bundle, version=None):
		self.framework_path = ''
		self.handled_libs = {}
		self.bundle = bundle
		self.version = version
		self.infopath = os.path.join(os.path.abspath(bundle), 'Contents', 'Info.plist')
		self.infoplist = plistlib.load(open(self.infopath, "rb"))
		self.binary = os.path.join(os.path.abspath(bundle), 'Contents', 'MacOS', self.infoplist['CFBundleExecutable'])
		print(' * Preparing AppBundle')


class FolderObject(object):

	class Exception(Exception):
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
			print(' * Copying directory: %s' % os.path.basename(asrc))
			shutil.copytree(asrc, adst, symlinks=True)
		elif os.path.isfile(asrc):
			print(' * Copying file: %s' % os.path.basename(asrc))
			shutil.copy(asrc, adst)

	def symlink(self, src, dst):
		'''
			Create a symlink inside the folder.
		'''
		asrc = os.path.abspath(src)
		adst = self.tmp + '/' + dst
		print(' * Creating symlink %s' % os.path.basename(asrc))
		os.symlink(asrc, adst)

	def mkdir(self, name):
		'''
			Create a directory inside the folder.
		'''
		print(' * Creating directory %s' % os.path.basename(name))
		adst = self.tmp + '/'  + name
		os.makedirs(adst)


class DiskImage(FolderObject):

	def __init__(self, filename, volname):
		FolderObject.__init__(self)
		print(' * Preparing to create diskimage')
		self.filename = filename
		self.volname = volname

	def create(self):
		'''
			Create the disk image itself.
		'''
		print(' * Creating diskimage. Please wait...')
		if os.path.exists(self.filename):
			os.remove(self.filename)
		p = Popen(['hdiutil', 'create',
		           '-srcfolder', self.tmp,
		           '-format', 'UDBZ',
		           '-volname', self.volname,
		           '-megabytes', '130',
		           self.filename])
		retval = p.wait()
		assert retval == 0
		print(' * Removing temporary directory.')
		shutil.rmtree(self.tmp)
		print(' * Done!')

def package_client():
	if options.version is not None:
		ver = options.version
	else:
		ver = gitrev()
	if options.universal:
		fn = os.path.join(options.binary_dir, 'Mumble-Universal-%s.dmg') % ver
		title = 'Mumble %s (Universal)' % ver
	else:
		fn = os.path.join(options.binary_dir, 'Mumble-%s.dmg') % ver
		title = 'Mumble %s' % ver

	# Fix overlay installer package
	create_overlay_package()
	if options.only_overlay:
		sys.exit(0)

	# Do the finishing touches to our Application bundle before release
	a = AppBundle(os.path.join(options.binary_dir, 'Mumble.app'), ver)
	a.copy_helper('mumble-g15-helper')
	a.copy_codecs()
	a.copy_plugins()
	a.copy_resources([os.path.join(options.source_dir, 'icons/mumble.icns')])
	a.update_plist()
	if not options.universal:
		a.set_min_macosx_version('10.9.0')
	else:
		a.set_min_macosx_version('10.4.8')
	a.done()

	# Sign our binaries, etc.
	if options.developer_id:
		print(' * Signing binaries with Developer ID `%s\'' % options.developer_id)
		binaries = (
			os.path.join(options.binary_dir, 'Mumble.app'),
			os.path.join(options.binary_dir, 'Mumble.app/Contents/Plugins/liblink.dylib'),
			os.path.join(options.binary_dir, 'Mumble.app/Contents/Plugins/libmanual.dylib'),
			os.path.join(options.binary_dir, 'Mumble.app/Contents/Codecs/libcelt0.0.7.0.dylib'),
			os.path.join(options.binary_dir, 'Mumble.app/Contents/Codecs/libopus.dylib'),
			os.path.join(options.binary_dir, 'Mumble.app/Contents/MacOS/mumble-g15-helper'),
		)
		availableBinaries = [bin for bin in binaries if os.path.exists(bin)]
		codesign(availableBinaries)
		print()

	if options.only_appbundle:
		sys.exit(0)

	# Create diskimage
	d = DiskImage(fn, title)
	d.copy(os.path.join(options.source_dir, 'macx/scripts/DS_Store'), '/.DS_Store')
	d.symlink('/Applications', '/Applications')
	d.copy(os.path.join(options.binary_dir, 'Mumble.app'))
	d.create()

def package_server():
	if options.version is not None:
		ver = options.version
	else:
		ver = gitrev()

	name = 'Murmur-OSX-Static-%s' % ver

	# Fix .ini files
	p = Popen(('bash', 'mkini.sh'), cwd=os.path.join(options.source_dir, 'scripts'))
	retval = p.wait()
	if retval != 0:
		raise Exception('mkini.sh failed')

	destdir = os.path.join(options.binary_dir, name)
	if os.path.exists(destdir):
		shutil.rmtree(destdir)
	os.mkdir(destdir)

	shutil.copy(os.path.join(options.binary_dir, 'installer/gpl.txt'), os.path.join(destdir, 'LICENSE'))
	shutil.copy(os.path.join(options.binary_dir, 'docs/additional-readmes/README.static.osx'), os.path.join(destdir, 'README'))
	shutil.copy(os.path.join(options.binary_dir, 'CHANGES'), os.path.join(destdir, 'CHANGES'))
	shutil.copy(os.path.join(options.binary_dir, 'scripts/murmur.pl'), os.path.join(destdir, 'murmur.pl'))
	shutil.copy(os.path.join(options.binary_dir, 'scripts/weblist.pl'), os.path.join(destdir, 'weblist.pl'))
	shutil.copy(os.path.join(options.binary_dir, 'scripts/icedemo.php'), os.path.join(destdir, 'icedemo.php'))
	shutil.copy(os.path.join(options.binary_dir, 'scripts/weblist.php'), os.path.join(destdir, 'weblist.php'))
	shutil.copy(os.path.join(options.binary_dir, 'scripts/murmur.ini.osx'), os.path.join(destdir, 'murmur.ini'))
	shutil.copy(os.path.join(options.binary_dir, 'src/murmur/Murmur.ice'), os.path.join(destdir, 'Murmur.ice'))

	shutil.copy(os.path.join(options.binary_dir, 'murmurd'), os.path.join(destdir, 'murmurd'))
	codesign(os.path.join(destdir, 'murmurd'))

	certname = 'Developer ID Installer: %s' % options.developer_id
	p = Popen(('xip', '--keychain', options.keychain, '-s', certname, '--timestamp', destdir, os.path.join(options.binary_dir, name+'.xip')))
	retval = p.wait()
	if retval != 0:
		print('Failed to build Murmur XIP package')
		sys.exit(1)

	absrelease = os.path.join(os.getcwd(), 'options.binary_dir')

	p = Popen(('tar', '-cjpf', name+'.tar.bz2', name), cwd=absrelease)
	retval = p.wait()
	if retval != 0:
		print('Failed to build Murmur tar.bz2 package')
		sys.exit(1)

	p = Popen(('gpg', '--detach-sign', '--armor', '-u', options.developer_id, '-o', name+'.tar.bz2.sig', name+'.tar.bz2'), cwd=absrelease)
	retval = p.wait()
	if retval != 0:
		print('Failed to sign Murmur tar.bz2 package')
		sys.exit(1)

if __name__ == '__main__':
	parser = OptionParser()
	parser.add_option('', '--source-dir', dest='source_dir', help='This sets the path to the repository. (Defaults to ".")', default='.')
	parser.add_option('', '--binary-dir', dest='binary_dir', help='This sets the path to the folder containing binaries. It will also be used as output directory. (Defaults to "build")', default='build')
	parser.add_option('', '--version', dest='version', help='This overrides the version number of the build.')
	parser.add_option('', '--universal', dest='universal', help='Build an universal snapshot.', action='store_true', default=False)
	parser.add_option('', '--only-appbundle', dest='only_appbundle', help='Only prepare the appbundle. Do not package.', action='store_true', default=False)
	parser.add_option('', '--only-overlay', dest='only_overlay', help='Only create the overlay installer.', action='store_true', default=False)
	parser.add_option('', '--developer-id', dest='developer_id', help='Identity (Developer ID) to use for code signing. The name is also used for GPG signing. (If not set, no code signing will occur)')
	parser.add_option('', '--keychain', dest='keychain', help='The keychain to use when invoking code signing utilities. (Defaults to "login.keychain")', default='login.keychain')
	parser.add_option('', '--server', dest='server', help='Build a Murmur package.', action='store_true', default=False)

	options, args = parser.parse_args()

	if not os.path.exists(options.source_dir):
		print('Please specify a source directory that exists!')
		sys.exit(1)

	if not os.path.exists(options.binary_dir):
		print('Please specify a binary directory that exists!')
		sys.exit(1)

	if not options.server:
		package_client()
	else:
		package_server()
