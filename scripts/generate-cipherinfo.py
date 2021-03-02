#!/usr/bin/env python
#
# Copyright 2015-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
#
# cipherinfo.py
# Generate static TLS cipher information for Mumble.

from __future__ import (unicode_literals, print_function, division)

import json
import re
import subprocess

try:
	from urllib2 import urlopen
except:
	from urllib.request import urlopen

from xml.dom import minidom

IETF_TLS_PARAMETERS_WWW = "https://www.ietf.org/assignments/tls-parameters/tls-parameters.xml"

def integerFromIdent(ident):
	'''
		Parses a TLS cipher suite identifier from the IETF TLS cipher suite registry.
		The cipher suite identifier is represented as a string of hex-encoded byte values
		separated by commas.

		This function consumes the byte sequence and converts it into an unsigned integer.
	'''
	hexVals = ident.split(',')
	nbytes = len(hexVals)
	if nbytes > 3:
		raise Exception("unexpected amount of bytes")
	outVal = 0
	for idx, val in enumerate(hexVals):
		intVal = int(val, base=16)
		outVal |= ((intVal & 0xff) << (((nbytes - 1) - idx) * 8))
	return outVal

def rfcNameLut():
	lut = {}

	# Auto-generate from IETF_TLS_PARAMETERS_WWW
	u = urlopen(IETF_TLS_PARAMETERS_WWW)
	s = u.read()
	s = s.decode('utf-8')
	u.close()

	dom = minidom.parseString(s)
	registries = dom.getElementsByTagName("registry")
	for registry in registries:
		ident = registry.getAttribute("id")
		if ident == "tls-parameters-4":
			records = registry.getElementsByTagName("record")
			for record in records:
				value = record.getElementsByTagName("value")[0].childNodes[0].nodeValue.strip()
				description = record.getElementsByTagName("description")[0].childNodes[0].nodeValue.strip()
				# Skip free-form informational entries that use ranges (-) and *
				# in their value.
				if re.match("^[0xA-Z1-9,]*$", value) is None:
					continue
				lut[value] = description

	##########################################################################
	# Obsolete SSLv2 cipher suites from RFC 4346, Appendix E:
	##########################################################################
	# TLS_RC4_128_WITH_MD5                  # 0x01, 0x00, 0x80
	lut["0x01,0x00,0x80"] = "TLS_RC4_128_WITH_MD5"
	# TLS_RC4_128_EXPORT40_WITH_MD5         # 0x02, 0x00, 0x80
	lut["0x02,0x00,0x80"] = "TLS_RC4_128_EXPORT40_WITH_MD5"
	# TLS_RC2_CBC_128_CBC_WITH_MD5          # 0x03, 0x00, 0x80
	lut["0x03,0x00,0x80"] = "TLS_RC2_CBC_128_CBC_WITH_MD5"
	# TLS_RC2_CBC_128_CBC_EXPORT40_WITH_MD5 # 0x04, 0x00, 0x80
	lut["0x04,0x00,0x80"] = "TLS_RC2_CBC_128_CBC_EXPORT40_WITH_MD5"
	# TLS_IDEA_128_CBC_WITH_MD5             # 0x05, 0x00, 0x80
	lut["0x05,0x00,0x80"] = "TLS_IDEA_128_CBC_WITH_MD5"
	# TLS_DES_64_CBC_WITH_MD5               # 0x06, 0x00, 0x40
	lut["0x06,0x00,0x40"] = "TLS_DES_64_CBC_WITH_MD5"
	# TLS_DES_192_EDE3_CBC_WITH_MD5         # 0x07, 0x00, 0xC0
	lut["0x07,0x00,0xC0"] = "TLS_DES_192_EDE3_CBC_WITH_MD5"

	return lut

def opensslCiphersOutput():
	p = subprocess.Popen(['openssl', 'ciphers', '-V', 'ALL'], stdout=subprocess.PIPE)
	stdout, stderr = p.communicate()
	if stdout is not None:
		stdout = stdout.decode('utf-8')
	if stderr is not None:
		stderr = stderr.decode('utf-8')
	if p.returncode != 0:
		raise Exception('"openssl ciphers" failed: %s', stderr)
	return stdout

def extract(splat):
	if len(splat) < 8:
		splat.extend([''])
	return splat

def Cstr(val):
	if val is None:
		return 'NULL'
	return '"{0}"'.format(val)

def CPPbool(val):
	if val is True:
		return 'true'
	return 'false'

def main():
	added_ids = []
	output = []
	lut = rfcNameLut()
	ciphers = opensslCiphersOutput()
	for line in ciphers.split('\n'):
		if len(line) == 0:
			continue

		line = line.replace(' - ', ' ')

		line = line.replace('Kx=', '')
		line = line.replace('Au=', '')
		line = line.replace('Enc=', '')
		line = line.replace('Mac=', '')
		line = line.replace('(', '_')
		line = line.replace(')', '')
		line = line.replace('/', '_')

		tabline = re.sub('\ +', '', line, 1)
		tabline = re.sub('\ +', '\t', tabline)
		splat = tabline.split('\t')

		ident, osslname, minproto, kx, au, enc, mac, exp = extract(splat)

		# Look up the RFC name of this cipher suite.
		if ident in lut:
			rfcname = lut[ident]
		else:
			raise Exception('missing rfc_name in lut for %s' % ident)

		# Normalize kx, au, enc and mac.
		if 'None' in au:
			au = 'Anonymous'

		enc = enc.upper()

		if 'AESGCM' in enc:
			enc = enc.replace('GCM', '')
			enc = enc + '_GCM'

		if 'GCM' in rfcname and not 'GCM' in enc:
			enc = enc + '_GCM'
		elif 'EDE_CBC' in rfcname:
			enc = enc + '_EDE_CBC'
		elif 'CBC' in rfcname:
			enc = enc + '_CBC'
		elif 'CCM_8' in rfcname:
			enc = enc + '_CCM_8'
		elif 'CCM' in rfcname:
			enc = enc + '_CCM'

		if 'ECDHE' in osslname and not 'ECDHE' in kx:
			kx = kx.replace('ECDH', 'ECDHE')
		if 'ECDHE' in osslname and not 'ECDHE' in au:
			au = au.replace('ECDH', 'ECDHE')

		if 'DHE' in osslname and not 'DHE' in kx:
			kx = kx.replace('DH', 'DHE')
		if 'DHE' in osslname and not 'DHE' in au:
			au = au.replace('DH', 'DHE')
		if 'EDH' in osslname and not 'EDH' in kx:
			kx = kx.replace('DH', 'DHE')
		if 'EDH' in osslname and not 'EDH' in au:
			au = au.replace('DH', 'DHE')

		if mac != 'AEAD':
			mac = 'HMAC-' + mac

		# Validate macs
		valid_macs = ['AEAD', 'HMAC-MD5', 'HMAC-SHA1', 'HMAC-SHA256', 'HMAC-SHA384']
		if mac not in valid_macs:
			raise Exception('invalid mac found: %s' % mac)

		# Use key exchange names from the RFCs, but also create
		# verbose key exchange names for export ciphers.
		match = re.match('^(TLS_|SSL_)(.*)_WITH.*$', rfcname)
		valid_rfc_kex = [
			"ECDHE_RSA",
			"ECDHE_ECDSA",
			"SRP_SHA_DSS",
			"SRP_SHA_RSA",
			"SRP_SHA",
			"DHE_DSS",
			"DHE_RSA",
			"ECDH_anon",
			"DH_anon",
			"DH_DSS",
			"DH_RSA",
			"ECDH_RSA",
			"ECDH_ECDSA",
			"RSA",
			"PSK",
		]
		valid_export_rfc_kex = [
			"DHE_RSA_EXPORT",
			"DHE_DSS_EXPORT",
			"DHE_DSS_EXPORT",
			"DH_anon_EXPORT",
			"RSA_EXPORT",
		]
		skip_rfc_kex = [
			"IDEA_128_CBC",
			"RC2_CBC_128_CBC",
			"RC4_128",
			"DES_192_EDE3_CBC",
			"DES_64_CBC",
			"IDEA_128_CBC",
			"RC2_CBC_128_CBC_EXPORT40",
			"RC4_128_EXPORT40"
		]
		if match is not None:
			rfc_kex = match.groups()[1]
			rfc_verbose_kex = rfc_kex

			if rfc_kex in skip_rfc_kex:
				rfc_kex = kx
				rfc_verbose_kex = kx
			elif rfc_kex in valid_rfc_kex:
				pass
			elif rfc_kex in valid_export_rfc_kex:
				if rfc_kex == 'DHE_RSA_EXPORT':
					rfc_verbose_kex = 'DHE_512_RSA_EXPORT'
				elif rfc_kex == 'DHE_DSS_EXPORT':
					rfc_verbose_kex = 'DHE_512_DSS_EXPORT'
				elif rfc_kex == 'DH_anon_EXPORT':
					rfc_verbose_kex = 'DH_anon_512_EXPORT'
				elif rfc_kex == 'RSA_EXPORT':
					rfc_verbose_kex = 'RSA_512_EXPORT'
				else:
					raise Exception('missing check for rfc_kex?')
			else:
				raise Exception('bad rfc_kex found: %s' % rfc_kex)

		pfs = False
		if rfc_verbose_kex == 'ECDHE_RSA':
			pfs = True
		elif rfc_verbose_kex == 'ECDHE_ECDSA':
			pfs = True
		elif rfc_verbose_kex == 'DHE_RSA':
			pfs = True
		elif rfc_verbose_kex == 'DHE_DSS':
			pfs = True
		elif rfc_verbose_kex == 'DHE_512_RSA_EXPORT':
			pfs = True
		elif rfc_verbose_kex == 'DHE_512_DSS_EXPORT':
			pfs = True
		# XXX: should SRP be marked as forward_secret?

		output.append({
			'sort_order': integerFromIdent(ident),
			'identifier': ident,
			'openssl_name': osslname,
			'rfc_name': rfcname,
			'minimum_protocol': minproto,
			'key_exchange': rfc_kex,
			'key_exchange_verbose': rfc_verbose_kex,
			'openssl_key_exchange': kx,
			'openssl_authentication': au,
			'encryption': enc,
			'message_authentication': mac,
			'export': exp == 'export',
			'forward_secret': pfs
		})
		added_ids.append(ident)

	# Add everything we missed from OpenSSL...
	include_extras = False
	if include_extras:
		for key in lut.keys():
			if not key in added_ids:
				output.append({
					'sort_order': integerFromIdent(key),
					'identifier': key,
					'openssl_name': None,
					'rfc_name': lut[key],
					'minimum_protocol': None,
					'key_exchange': None,
					'key_exchange_verbose': None,
					'openssl_key_exchange': None,
					'openssl_authentication': None,
					'encryption': None,
					'message_authentication': None,
					'export': None,
					'forward_secret': None
				})
	sortedOutput = sorted(output, key=lambda meta: meta['sort_order'])

	output_c = True
	if output_c:
		print('// Automatically generated by "generate-cipherinfo.py". DO NOT EDIT BY HAND.')
		print('//')
		print('// I also agree to have manually vetted this file for correctness.')
		print('//')
		print('// If I do not agree, I will not have removed the line above saying')
		print('// otherwise. Nor will I have removed the line below this one which')
		print('// will cause a preprocessor error. Oops!')
		print('#error Please verify this file is correct')
		print('static const SSLCipherInfo cipher_info_lookup_table[] = {')
		for entry in sortedOutput:
			print('\t{')
			print('\t\t// openssl_name')
			print('\t\t{0},'.format(Cstr(entry["openssl_name"])))
			print('\t\t// rfc_name')
			print('\t\t{0},'.format(Cstr(entry["rfc_name"])))
			print('\t\t// encryption')
			print('\t\t{0},'.format(Cstr(entry["encryption"])))
			print('\t\t// key_exchange_verbose. kx = {0}, au = {1}'.format(entry["openssl_key_exchange"], entry["openssl_authentication"]))
			print('\t\t{0},'.format(Cstr(entry["key_exchange_verbose"])))
			print('\t\t// forward secret')
			print('\t\t{0},'.format(CPPbool(entry["forward_secret"])))
			print('\t\t// message authentication')
			print('\t\t{0},'.format(Cstr(entry["message_authentication"])))
			print('\t},')
		print('};')
	else:
		print(json.dumps(sortedOutput, sort_keys=True, indent=4, separators=(',', ': ')))

if __name__ == '__main__':
	main()
