// Copyright (C) 2023 Hubert Figuière
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "tf_test.h"
#include "ut_units.h"
#include "ut_debugmsg.h"

#define TFSUITE "core.af.util.units"

TFTEST_MAIN("UT_convertInchesToDimensionString works")
{
  double res = 72.0f;

  // defaults
  auto dim = UT_convertInchesToDimensionString(DIM_IN, 0.0);
  TFPASSEQ(dim, "0.0000in");
  dim = UT_convertInchesToDimensionString(DIM_IN, 1.0);
  TFPASSEQ(dim, "1.0000in");
  dim = UT_convertInchesToDimensionString(DIM_CM, 0.0);
  TFPASSEQ(dim, "0.00cm");
  dim = UT_convertInchesToDimensionString(DIM_CM, 1.0);
  TFPASSEQ(dim, "2.54cm");
  dim = UT_convertInchesToDimensionString(DIM_MM, 0.0);
  TFPASSEQ(dim, "0.0mm");
  dim = UT_convertInchesToDimensionString(DIM_MM, 1.0);
  TFPASSEQ(dim, "25.4mm");
  dim = UT_convertInchesToDimensionString(DIM_PI, 0.0);
  TFPASSEQ(dim, "0pi");
  dim = UT_convertInchesToDimensionString(DIM_PI, 1.0);
  TFPASSEQ(dim, "6pi");
  dim = UT_convertInchesToDimensionString(DIM_PT, 0.0);
  TFPASSEQ(dim, "0pt");
  dim = UT_convertInchesToDimensionString(DIM_PT, 1.0);
  TFPASSEQ(dim, "72pt");
  dim = UT_convertInchesToDimensionString(DIM_PX, 0.0);
  TFPASSEQ(dim, "0px");
  dim = UT_convertInchesToDimensionString(DIM_PX, 1.0);
  TFPASSEQ(dim, "72px");
  dim = UT_convertInchesToDimensionString(DIM_PERCENT, 0.0);
  TFPASSEQ(dim, "0.000000%");
  dim = UT_convertInchesToDimensionString(DIM_PERCENT, 1.0);
  TFPASSEQ(dim, "1.000000%");
  dim = UT_convertInchesToDimensionString(DIM_none, 0.0);
  TFPASSEQ(dim, "0.000000");
  dim = UT_convertInchesToDimensionString(DIM_none, 1.0);
  TFPASSEQ(dim, "1.000000");

  dim = UT_convertInchesToDimensionString(DIM_IN, 162.4/res, "3.2");
  TFPASSEQ(dim, "2.26in");

  dim = UT_convertInchesToDimensionString(DIM_IN, 12.0, nullptr);
  TFPASSEQ(dim, "12.0000in");

  dim = UT_convertInchesToDimensionString(DIM_MM, 10, ".2");
  TFPASSEQ(dim, "254.00mm");
  dim = UT_convertInchesToDimensionString(DIM_PERCENT, 1.0, ".0");
  TFPASSEQ(dim, "1%");

}

TFTEST_MAIN("UT_formatDimensionString works")
{
  double res = 72.0f;

  // defaults
  auto dim = UT_formatDimensionString(DIM_IN, 0.0);
  TFPASSEQ(dim, "0.0000in");
  dim = UT_formatDimensionString(DIM_IN, 1.0);
  TFPASSEQ(dim, "1.0000in");
  dim = UT_formatDimensionString(DIM_CM, 0.0);
  TFPASSEQ(dim, "0.00cm");
  dim = UT_formatDimensionString(DIM_CM, 1.0);
  TFPASSEQ(dim, "1.00cm");
  dim = UT_formatDimensionString(DIM_MM, 0.0);
  TFPASSEQ(dim, "0.0mm");
  dim = UT_formatDimensionString(DIM_MM, 1.0);
  TFPASSEQ(dim, "1.0mm");
  dim = UT_formatDimensionString(DIM_PI, 0.0);
  TFPASSEQ(dim, "0pi");
  dim = UT_formatDimensionString(DIM_PI, 1.0);
  TFPASSEQ(dim, "1pi");
  dim = UT_formatDimensionString(DIM_PT, 0.0);
  TFPASSEQ(dim, "0pt");
  dim = UT_formatDimensionString(DIM_PT, 1.0);
  TFPASSEQ(dim, "1pt");
  dim = UT_formatDimensionString(DIM_PX, 0.0);
  TFPASSEQ(dim, "0px");
  dim = UT_formatDimensionString(DIM_PX, 1.0);
  TFPASSEQ(dim, "1px");
  dim = UT_formatDimensionString(DIM_PERCENT, 0.0);
  TFPASSEQ(dim, "0.000000%");
  dim = UT_formatDimensionString(DIM_PERCENT, 1.0);
  TFPASSEQ(dim, "1.000000%");
  dim = UT_formatDimensionString(DIM_none, 0.0);
  TFPASSEQ(dim, "0.000000");
  dim = UT_formatDimensionString(DIM_none, 1.0);
  TFPASSEQ(dim, "1.000000");

  dim = UT_formatDimensionString(DIM_IN, 162.4/res, "3.2");
  TFPASSEQ(dim, "2.26in");

  dim = UT_formatDimensionString(DIM_IN, 12.0, nullptr);
  TFPASSEQ(dim, "12.0000in");

  dim = UT_formatDimensionString(DIM_MM, 10, ".2");
  TFPASSEQ(dim, "10.00mm");
  dim = UT_formatDimensionString(DIM_PERCENT, 1.0, ".0");
  TFPASSEQ(dim, "1%");

}
