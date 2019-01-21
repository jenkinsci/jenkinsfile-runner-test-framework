# Main makefile for the testing framework. It should be executed by the actual project containing the tests
.PHONY: init

init:
	rm -rf shunit2
	git clone --depth 1 https://github.com/kward/shunit2

