#!/usr/bin/bash

pushd ../build/debug

make -f ../../nlp/Makefile
make -f ../../nlp/Makefile_tests

popd

#../build/debug/variable_dump.exe > variable_dump.out
#../build/debug/test_value_value_list_list.exe > test_value_value_list_list.out
#../build/debug/test01.exe < data/test.inp > test01.out
#../build/debug/test02.exe < data/test.inp > test02.out
../build/debug/test03.exe
