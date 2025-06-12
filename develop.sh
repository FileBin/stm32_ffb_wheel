#!/usr/bin/env bash
mkdir /tmp/code-ffb-wheel
nix-shell --run 'TMPDIR=/tmp/code-ffb-wheel code .'
