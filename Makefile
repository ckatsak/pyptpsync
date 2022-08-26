#
# ckatsak, Fri 26 Aug 2022 05:12:38 AM EEST
#

DOCKER ?= docker

.PHONY: wheels gnu musl
wheels: gnu musl

gnu:
	$(DOCKER) run --rm -v $(CURDIR):/io quay.io/pypa/manylinux2014_x86_64 \
		bash /io/build-wheels.sh manylinux
	twine upload -r testpypi \
		musllinux_dist/ptpsync-0.0.2-cp3*-musllinux*
musl:
	$(DOCKER) run --rm -v $(CURDIR):/io quay.io/pypa/musllinux_1_1_x86_64 \
		bash /io/build-wheels.sh musllinux
	twine upload -r testpypi \
		manylinux_dist/ptpsync-0.0.2-cp3*-manylinux*

