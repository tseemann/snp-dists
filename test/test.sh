#!/usr/bin/env bats

setup() {
  name="snp-dists"
  bats_require_minimum_version 1.5.0
  dir=$(dirname "$BATS_TEST_FILENAME")
  cd "$dir"
  cpus=$(nproc)
  bin="$dir/../$name"
  exe="$bin -q -b"
  compare="diff -bB"
}

@test "Binary exists" {
  [[ -x "$bin" ]]
}
@test "Version" {
  run -0 $bin -v
  [[ "${lines[0]}" =~ "$name" ]]
}
@test "Help" {
  run -0 $bin -h
  [[ "$output" =~ "USAGE" ]]
}
@test "No parameters" {
  run ! $bin
}
@test "Bad option" {
  run ! $bin -Y
  [[ "$output" =~ "invalid option" ]]
}
@test "Passing a folder" {
  run ! $bin $dir
  [[ "$output" =~ "ERROR" ]]
}
@test "Empty input" {
  run ! $bin /dev/null
  [[ "$output" =~ "ERROR" ]]
}

@test "Singleton" {
  run bats_pipe -0 $exe singleton.aln \| $compare - singleton.res
}
@test "Good" {
  run bats_pipe -0 $exe good.aln \| $compare - good.res
}
@test "Good -c" {
  run bats_pipe -0 $exe -c good.aln \| $compare - good-c.res
}
@test "Good -c -m" {
  run bats_pipe -0 $exe -c -m good.aln \| $compare - good-c-m.res
}
@test "Gzipped" {
  run bats_pipe -0 $exe gzip.aln.gz \| $compare - gzip.res
}
@test "Lowercase -k" {
  run bats_pipe -0 $exe -k lowercase.aln \| $compare - lowercase-k.res
}
@test "Lowercase" {
  run bats_pipe -0 $exe lowercase.aln \| $compare - lowercase.res
}
@test "Anbiguous" {
  run bats_pipe -0 $exe ambig.aln \| $compare - ambig.res
}
@test "Anbiguous -a" {
  run bats_pipe -0 $exe -a ambig.aln \| $compare - ambig-a.res
}
@test "Anbiguous -c -m" {
  run bats_pipe -0 $exe -c -m ambig.aln \| $compare - ambig-c-m.res
}

@test "Huge" {
  run -0 $bin -c huge.afa.gz
  [[ "${lines[3]}" =~ "seq3," ]]
}
@test "Multi-threading" {
  run -0 $bin -c -j $cpus huge.afa.gz
  [[ "${lines[3]}" =~ "seq3," ]]
}

