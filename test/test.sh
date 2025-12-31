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
@test "Good -L lower triangle" {
  run bats_pipe -0 $bin -qcbL good.aln \| $compare - good-qcbL.res
}
@test "Good CSV -c" {
  run bats_pipe -0 $exe -c good.aln \| $compare - good-c.res
}
@test "Good CSV -c MOLTEN -c" {
  run bats_pipe -0 $exe -c -m good.aln \| $compare - good-c-m.res
}
@test "MOLTEN with header -t" {
  run -0 $exe -m -t -c good.aln
  [[ "${lines[0]}" =~ ",distance" ]]
}
@test "MOLTEN -L unique pairs only" {
  run -0 $exe -mcL good.aln
  [[ ! "${output}" =~ "seq2,seq1" ]]
  [[ ! "${output}" =~ "seq3,seq2" ]]
  [[ ! "${output}" =~ "seq4,seq3" ]]
}
@test "Maxdist -x 1" {
  run -0 $exe -bcx 1 good.aln
  [[ "${lines[4]}" =~ "seq4,1,1,1,0" ]]
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

@test "Huge 50 seq x 5,000,000 sites" {
  run -0 $bin -c huge.afa.gz
  [[ "${lines[3]}" =~ "seq3," ]]
}
@test "Huge + threading" {
  run -0 $bin -c -j $cpus huge.afa.gz
  [[ "${lines[3]}" =~ "seq3," ]]
}
@test "Huge + threading + maxdiff 20 " {
  run -0 $bin -c -j $cpus -x 20 huge.afa.gz
  [[ "${lines[3]}" =~ "seq3," ]]
}
