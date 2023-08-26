#!/usr/bin/env python3

# Will check the Emacs modelines of the file passed as argument
# and run clang-format on it.
#

import os
import sys
import re

if sys.argv[0] is None:
    sys.exit(1)

filename = sys.argv[1]

basic_offset = 2
tab_width = basic_offset
use_tab = False

with open(filename) as f:
    line = f.readline()
    if line:
        results = re.findall(r"-\*-\s*Mode:\s*C\+\+;.*(tab-width):\s*([0-9]+);?\s*|(c-basic-offset):\s*([0-9]+);?\s*|(indent-tabs-mode):\s*([t]);?\s*", line, re.M)

        if len(results):
            for result in results:
                if result[0]:
                    tab_width = result[1]
                if result[2]:
                    basic_offset = result[3]
                if result[4]:
                    use_tab = result[5] == 't'
        else:
            print("No modelines found.")
            sys.exit(2)

if use_tab:
    use_tab = "Always"
else:
    use_tab = "Never"

clang_style = "{{BasedOnStyle: WebKit, BreakBeforeBraces: Custom, BraceWrapping: {{AfterCaseLabel: true, AfterClass: true, AfterFunction: true }}, TabWidth: {}, UseTab: {}}}".format(basic_offset, use_tab)

command = ["clang-format", "'--style={}'".format(clang_style), "-i", filename]

#print(" ".join(command))
os.system(" ".join(command))
