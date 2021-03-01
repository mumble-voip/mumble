#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2016-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Convert the Git log of the repository into a CHANGES file.
#
# This script uses the same formatting that
# 'git2cl.pl' has historically used.

from __future__ import (unicode_literals, print_function, division)

from  collections import defaultdict
import subprocess
import sys
import re

def textwidth(text):
	'''
		Calculate the width of 'text' in number
		of characters.

		This function counts tabs as 8 characters.
	'''
	count = 0
	for char in text:
		if char == '\t':
			count += 8
		else:
			count += 1
	return count


def perlCompatibleTextWrap(text, width, initial_indent, subsequent_indent):
	'''
		Format text like Perl's Text::Wrap#wrap function.

		We do this to keep compatibility with existing formatting in
		the CHANGES file. (Invoking this script overwrites the old file,
		so the output of this function must match the Perl function's
		output.)
	'''

	lines = []
	line = ""

	# Require at least one empty character
	# of free space at the end of the line.
	width -= 1

	# Create our word queue by splitting the commit
	# description by a regular space character (' ').
	#
	# This is a little simplistic, but it works in practice.
	# In an ideal world, we should probably split all
	# whitespace, so our queue guaranteed to only contain
	# words.
	queue = text.split(' ')

	# Iterate through each word and in the queue
	# and construct our formatted text.
	while len(queue) > 0:
		word = queue.pop(0)

		# Which indentation mode should we use for
		# this line?
		# If this is the first line, use 'initial_indent'.
		# If not, use 'subsequent_indent'.
		if len(lines) == 0:
			indent = initial_indent
		else:
			indent = subsequent_indent

		# If nothing has been added to our line yet,
		# add the indentation.
		if line == "":
			line = indent

		# Find the wordPrefix.
		#
		# The wordPrefix represents the string that
		# will come before 'word' when 'word' is added
		# to the line.
		#
		# If no other words have been added to the
		# line, the wordPrefix is the indentation.
		# (Either initial_indent or subsequent_indent).
		# If other words have been added to the line
		# already, the wordPrefix will be the existing
		# content of the line, plus a space.
		if line == indent:
			wordPrefix = line
		else:
			wordPrefix = line + " "

		# Calculate the width of line with the word added.
		widthWithWordAdded = textwidth(wordPrefix + word)

		# Check if the line would become too long if the word
		# was added to the line as-is (considering the wordPrefix).
		if widthWithWordAdded > width:
			# Handle words that need to be split.
			# As a rule, split words MUST start at the very beginning
			# of a line (considering indentation) -- that is, there
			# most be no other words in the line for a split word to
			# be output.
			#
			# We check that the line only contains indentation.
			# This is the same as saying that the current line has
			# no other words in it.
			#
			# Since the word could not fit onto the line, we enter the
			# 'else'-part of this if-statement, and we consider the
			# current line done -- the rest of the current line is
			# left unused.
			# Then, once we're on the next line, which is empty, we
			# enter the word-splitting part of this if-statement and
			# begin outputting the split word.
			if line == indent:
				# The preconditions have been met
				# for word splitting to occur.
				#
				# Cut the word into two parts.
				#
				# The first part is the one that goes
				# onto the empty line are are currently at,
				# and fills it.
				#
				# The other part is the remainder. That part
				# is put back into the word queue.
				size = width - textwidth(wordPrefix)
				keep = word[0:size]
				putBack = word[size:]
				queue.insert(0, putBack)
				word = keep
			else:
				# There was not enough space left in the line to be
				# able to contain this word.
				#
				# This is either because,
				# (a) There simply was not enough space, or...
				# (b) The word-splitting check demanded that
				#     we skip the rest of the line. This
				#     happens because we, as a rule, only
				#     output split words beginning on their
				#     own pristine line.
				#
				# Discard the word, and consider the current line complete.
				# This moves on to the next line.
				queue.insert(0, word)
				lines.append(line)
				line = ""
				continue

		# Add the word to the line
		line = wordPrefix + word

	# If we have a non-empty line, but no
	# words left in the queue, it is the last
	# line. Append it to the 'lines' list.
	if line != "":
		lines.append(line)

	# Format the lines for user consumption.
	return "\n".join(lines)

def matchRegexp(regexp, content):
	'''
		A simple function to make regular
		expression matching return a boolean
		value for easier readability.
	'''
	return re.match(regexp, content, re.UNICODE) is not None

def gitMailmapLookup(author, email):
	'''
		Look up the canonical forms of the
		combination of 'author' and 'email'
		in the .mailmap file.
	'''
	escapedAuthor = author.replace('<', '?').replace('>', '?')
	escapedEmail = email.replace('<', '?').replace('<', '?')
	contact = "{0} <{1}>".format(escapedAuthor, escapedEmail)

	p = subprocess.Popen(["git", "check-mailmap", contact], stdout=subprocess.PIPE)
	stdout, stderr = p.communicate()
	if stdout is not None:
		stdout = stdout.decode('utf-8')
	if stderr is not None:
		stderr = stderr.decode('utf-8')
	if p.returncode != 0:
		raise Exception('"git check-mailmap" failed: %s', stderr)

	match = re.match('^(.*)\ \<(.*)\>$', stdout)
	if match is None:
		raise Exception("unable to parse contact output from 'git check-mailmap'")

	groups = match.groups()
	author = groups[0]
	email = groups[1]

	return (author, email)

def gitLogOutput():
	'''
		Invoke git to get NUL-separated log lines.
		The rows are as follows:
		 - Commit hash
		 - Author
		 - Email
		 - Date (YYYY-MM-DD)
		 - Subject
	'''
	p = subprocess.Popen(["git", "log", "--use-mailmap", "--date=short", "--pretty=format:%h%x00%aN%x00%aE%x00%cd%x00%s"], stdout=subprocess.PIPE)
	stdout, stderr = p.communicate()
	if stdout is not None:
		stdout = stdout.decode('utf-8')
	if stderr is not None:
		stderr = stderr.decode('utf-8')
	if p.returncode != 0:
		raise Exception('"git log" failed: %s', stderr)
	return stdout

def main():
	log = gitLogOutput()

	# The authors dict maps between email addresses
	# and author names.
	authors = {}

	# The dates dictionary maps from
	# dates, to another dictionary
	# that maps from email addresses
	# to a sorted list of commit subject
	# lines.
	#
	# In pseudo code, the type of 'dates' is:
	#
	#     dict<string(YYMMDD), dict<string(email), list(subject)>>
	#
	dates = defaultdict(lambda: defaultdict(list))

	# lastSubject contains the last subject line
	# processed. It is used to detect duplicate
	# subject lines and discard them.
	lastSubject = ""

	for line in log.split('\n'):
		commit, author, email, date, subject = line.split('\0')

		if author == "Thorvald Natvig":
			# Because someone forgets to set their git username on every platform...
			email = "slicer@users.sourceforge.net"

		# Back in the old days, we appended [$SourceForgeUserName]
		# in our commit messages when we applied patches by contributors
		# on SourceForge.
		#
		# For example:
		#
		#     Half-Life 2 Deathmatch Positional Audio [Snares]
		#
		# The code below extracts author names from the subject line
		# of the commit message, and updates the 'author' and
		# 'email' variables to point to the SourceForge user
		# specified in the square brackets. The email address
		# is simply the lowercase username @users.sourceforge.net.
		patchAuthorRe = "\s*\[([\w-]+)\]\s*$"
		match = re.search(patchAuthorRe, subject, flags=re.UNICODE)
		if match is not None:
			author = match.groups()[0]
			email = "{0}@users.sourceforge.net".format(author.lower())

			# Translate the author/email via .mailmap
			author, email = gitMailmapLookup(author, email)

			# Remove the [$SourceForgeUserName] from the subject line.
			subject = re.sub(patchAuthorRe, "", subject)

		# Record the first occurrence of an author's
		# email address and name in the author <-> email map.
		if email not in authors:
			authors[email] = author

		if matchRegexp("^Merge branch 'master", subject):
			continue
		if matchRegexp("^Indent and submodule update", subject):
			continue
		if matchRegexp("^Indent, changelog and submodule update", subject):
			continue
		if matchRegexp("^Indent, changelog, submodule and language update", subject):
			continue
		if matchRegexp("^Indent run", subject):
			continue
		if matchRegexp("^\*\*\* empty log message", subject):
			continue
		if matchRegexp("^git-svn-id: http", subject):
			continue
		if matchRegexp("^TEST", subject):
			continue
		if matchRegexp("^Merge pull request", subject):
			continue
		if matchRegexp("^Transifex translation update", subject):
			continue

		# Skip duplicate subject lines.
		if subject == lastSubject:
			continue
		lastSubject = subject

		# Format the commit hash and subject line.
		entry = perlCompatibleTextWrap(subject, 76, initial_indent="    {0}  ".format(commit), subsequent_indent="\t     ")

		# Add the entry to the dictionary structure.
		dates[date][email].append(entry)

	# Output all collected entries
	# to the CHANGES file.
	with open('CHANGES', 'wb') as f:
		dateKeys = sorted(dates.keys(), reverse=True)
		for date in dateKeys:
			f.write("{0}\n".format(date).encode('utf-8'))
			entries = dates[date]
			emails = sorted(authors.keys())
			for email in emails:
				author = authors[email]
				entry = entries.get(email)
				if entry is None:
					continue
				f.write("  {0} <{1}>\n".format(author, email).encode('utf-8'))
				f.write("\n".join(entries[email]).encode('utf-8'))
				f.write("\n\n".encode('utf-8'))

if __name__ == '__main__':
	main()
