#!/bin/bash
#
# ckatsak, Fri 26 Aug 2022 05:10:05 AM EEST
#
# Arguments:
#  $1: Expecting either 'manylinux' or 'musllinux'

set -eux

# Compile wheels
for PYBIN in /opt/python/cp{36,37,38,39,310,311}*/bin; do
    rm -rf /io/build/
    "${PYBIN}/pip" install -U setuptools wheel
    "${PYBIN}/pip" wheel /io/ -w "/io/${1}_dist/" --no-deps
done

# Bundle external shared libraries into the wheels
for whl in "/io/${1}_dist"/*{cp36,cp37,cp38,cp39,cp310,cp311}*.whl; do
    auditwheel repair "$whl" -w "/io/${1}_dist/"
done

# Install packages and test
for PYBIN in /opt/python/cp{36,37,38,39,310,311}*/bin; do
    "${PYBIN}/pip" install ptpsync -f "/io/${1}_dist/"
done

