#!/usr/bin/env bash

ninja -v -C out -j 10 &&
out/unittest
