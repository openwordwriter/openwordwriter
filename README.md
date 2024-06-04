Open Word Writer
================

Open Word Writer is an open source word processor. It is a fork of AbiWord 3.0.5. The goal is to maintain and improve this fine
word processor, and make a good tool for people writing documents, books and articles. 

Open Word Writer will be just right for writing documents. Microsoft Word is good, but too expensive, LaTeX is too difficult and OpenOffice/LibreOffice is too big. 

Bug reports and Pull requests welcome.

[![CI Build](https://github.com/openwordwriter/openwordwriter/actions/workflows/ci.yml/badge.svg)](https://github.com/openwordwriter/openwordwriter/actions/workflows/ci.yml)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)

![Screenshot](
https://raw.githubusercontent.com/openwordwriter/openwordwriter/master/docs/oww-screenshot.png "screenshot")



----
### Changes from AbiWord:
- 
------
### TODO:
- Builds for Windows, Linux and Mac.
- Rename to Open Word Writer (openwordwriter executable)
- Automated code checking tools
- Modernize
- Remove old stuff



This is the source tree for the AbiSource desktop productivity tools,
namely the AbiWord word processor.
Everything contained here is copyrighted and is available for your use
and redistribution under certain license terms.  Please read
COPYRIGHT.TXT and TRADEMARK.TXT for more information.

Current information is available at https://www.abisource.com/.

This tree contains the following top-level subdirectories:

src
	All source code is under here, including applications,
	library modules, and third-party source code.

ac-helpers
	Autoconf macros used by the autoconfiscated build system (configure).

docs
	Documents related to the project.  Includes specifications,
	design documents, and the like.  You should also find instructions
	for how to build the tree in here, within the "build" subdirectory.

po
	Localized strings for each locale we support, in PO format, as well
	as scripts for conversion to and from Abi's native .strings files,
	which are the files actually used directly by the program(s).  PO
	files are maintained as a convenience for translators, many of whom
	are already accustomed to working with them (as opposed to .strings).

test
	Test files for AbiWord (not explicitly part of regression testing).

user
	Support files, other than code, which need to be installed
	on the user's machine, are stored in here.  Items which
	are too large to be practical do not necessarily go here,
	such as the fonts.

flatpak
        Files to build AbiWord as a flatpak using flatpak-builder. See
        instructions below.

Flatpak
=======

We support flatpak:

To build:

$ flatpak-builder --force-clean --repo=repo abiword flatpak/com.abisource.AbiWord.json

To run:

$ flatpak-builder --run abiword flatpak/com.abisource.AbiWord.json abiword
