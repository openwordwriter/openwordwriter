Testing AbiWord
===============

## Built-in test suite

AbiWord has a built-in testsuite.

It is build as the `AbiTest` plugin. Its code is in
`plugins/testharness/xp`

The test harness (testing framework) source code is in `src/af/tf/xp`
It gets linked to the plugin. The plugin links to libabiword (what it
tests).

It all gets build when you build AbiWord. As of now, it needs to be
installed like any other plugins.

To run it:

```shell
$ abiword --plugin=AbiTest
```

In case of test failure, `abiword` will return a non-zero code.

### Test data files

Ensure either `ABI_TEST_SRC_DIR` or `top_srcdir` is set to
the top of the source dir.

### Individual tests

Test are *.t.cpp files. Usually in a `t` subdirectory of what they are
testing. They are built and linked directly into the plugin.

To add such a file make sure that:
- the 't' directory is in `DIST_SUBDIR`
- the test source file is in `EXTRA_DIST`
- the test source file is in `plugins/testharness/xp/Makefile.am`

When they are run from the AbiTest harness all the basics are setup.

Each test source must define `TFSUITE` to a string that will
name the test. Example:

```C++
#define TFSUITE "core.wp.impexp.table"
```

This is the id that can be passed at runtime to select test to run.
Like:

```shell
$ abiword --plugin=AbiTest -E core.wp.impexp.table
```

This will only the test(s) in that suite.

## Debugging

You can set these env variable to assist in test and debugging:

* `ABI_TEST_SRC_DIR`: set the top srcdir for the test harness. (see
  above)
* `ABI_DEBUG_SILENT`: set to 1 to silence `UT_DEBUGMSG()` and ignore
  `UT_ASSERT()` in a debug build.
