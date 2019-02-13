# Makefile for test the framework
.PHONY: test

test:
	rm -rf ../.shunit2
	rm -rf .testing
	mkdir -p .testing
	# TODO So far, the released tags do not contains all the macros and functions that
	# master branch does (assertContains, e.g.). Once a new version containing all 
	# the functionalyty is released we should clone using --branch
	git clone https://github.com/kward/shunit2 ../.shunit2 && cd ../.shunit2 && git checkout abb3ab2fef8c549933e378ae3d12127dfc748e73
	./workspace_hook_tests.sh
	./tests.sh
	./timeout-tests.sh
