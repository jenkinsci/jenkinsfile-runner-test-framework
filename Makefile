# Main makefile for the testing framework
.PHONY: all

all: clean

clean:
	rm -rf shunit2
	git clone --depth 1 https://github.com/kward/shunit2

