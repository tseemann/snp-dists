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

START_TEST (check_distance_calculation)
{
  // If its the same there are no differences.
  fail_unless(distance("AA","AA", 2, 0) == 0);
  
  // 1 difference
  fail_unless(distance("AA","AG", 2, 0) == 1);
  
  // all different
  fail_unless(distance("AA","GG", 2, 0) == 2);
  
  // 1 different in the middle
  fail_unless(distance("AAA","AGA", 3, 0) == 1); 
}
END_TEST
  
START_TEST (check_distance_only_acgt)
{
  // If its the same there are no differences.
  fail_unless(distance("AA","AA", 2, 1) == 0);
  
  // 1 difference
  fail_unless(distance("AA","AG", 2, 1) == 1);

  // all contain a non-ACGT character
  fail_unless(distance("ZZZ","AAA", 3, 1) == 0); 
  
  // Mix of ACGT and non-ACGT
  fail_unless(distance("ZZZGA","AAAAA", 5, 1) == 1); 
}
END_TEST

Suite * snpdists_suite (void)
{
  Suite *s = suite_create ("Creating_snp-dists");

  TCase *tc_snpdists = tcase_create ("snpdists");
  tcase_add_test (tc_snpdists, check_distance_calculation);
  tcase_add_test (tc_snpdists, check_distance_only_acgt);
  suite_add_tcase (s, tc_snpdists);
  return s;
}

