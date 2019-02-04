# Main makefile for the testing framework. It should be executed by the actual project containing the tests
.PHONY: init

init:
	rm -rf .shunit2
	# avoid any cache of checks/tests
	rm -rf tests/.testing
	rm -rf checksyntax/.checksyntax
	# TODO So far, the released tags do not contains all the macros and functions that
	# master branch does (assertContains, e.g.). Once a new version containing all 
	# the functionalyty is released we should clone using --branch
	git clone --depth 1 https://github.com/kward/shunit2 .shunit2 && cd .shunit2 && git checkout abb3ab2fef8c549933e378ae3d12127dfc748e73

test:
	$(MAKE) -C tests

syntax:
	$(MAKE) -C checksyntax

verify: syntax test
