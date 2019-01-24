# Makefile for test the framework
.PHONY: test

test:
	rm -rf .testing
	mkdir -p .testing
	# TODO So far, the released tags do not contains all the macros and functions that
	# master branch does (assertContains, e.g.). Once a new version containing all 
	# the functionalyty is released we should clone using --branch
	git clone --depth 1 https://github.com/kward/shunit2 .testing/shunit2 && cd .testing/shunit2 && git checkout abb3ab2fef8c549933e378ae3d12127dfc748e73
	./tests.sh
