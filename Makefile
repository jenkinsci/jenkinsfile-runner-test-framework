# Main makefile for the testing framework
.PHONY: init

init:
	rm -rf shunit2
	git clone --depth 1 https://github.com/kward/shunit2

