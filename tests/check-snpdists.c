/*
 *  Wellcome Trust Sanger Institute
 *  Copyright (C) 2016  Wellcome Trust Sanger Institute
 *  
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 *  of the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "check-snpdists.h"
#include "snpdists.h"

START_TEST (check_small_valid_file)
{
  // insert your test here.
  fail_unless(1==1);
}
END_TEST

Suite * snpdists_suite (void)
{
  Suite *s = suite_create ("Creating_snp-dists");

  TCase *tc_snpdists = tcase_create ("snpdists");
  tcase_add_test (tc_snpdists, check_small_valid_file);
  suite_add_tcase (s, tc_snpdists);
  return s;
}

