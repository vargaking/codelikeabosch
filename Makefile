# determining OS
uname := $(shell uname)
ifeq ($(uname),Darwin)
	# mac os specific code
	PYTHON := python3
	PYTHON_CONFIG := python3-config
endif
ifeq ($(uname),Linux)
	# linux specific code
	PYTHON := python3
	PYTHON_CONFIG := python3-config
endif

CXXFLAGS := --std=c++17 -O1 -Wno-write-strings -I$(shell pwd)/include -I$(shell pwd)/include/pybind11/include $(shell $(PYTHON_CONFIG) --includes 2> /dev/null || ~$(PYTHON_CONFIG) --includes)

suffix = $(shell $(PYTHON_CONFIG) --extension-suffix 2> /dev/null || ~$(PYTHON_CONFIG) --extension-suffix)

.DEFAULT_GOAL := help

export CXX CXXFLAGS PYTHON uname suffix

.PHONE: all
all: install build venv

.PHONY: help
help:
	@echo "Usage: make ..."
	@echo "    help    : print this menu"
	@echo "    venv    : activate venv"
	@echo "    install : install pip dependencies in venv"
	@echo "    build   : build all binaries of the project"
	@echo "    clean   : clean build files"

.PHONY: venv
venv:
	@source $(shell pwd)/../bin/activate; \
	clear; \
	echo "Entering virtual environment."; \
	$$SHELL -i; \
	echo "Exiting virtual environment.";

.PHONY: install
install:
	source $(shell pwd)/../bin/activate && $(PYTHON) -m pip install -r requirements.txt && exit

.PHONY: build
build:
	@if ~$(PYTHON_CONFIG) --includes 1>/dev/null 2>/dev/null || $(PYTHON_CONFIG) --includes 1>/dev/null 2>/dev/null; then \
		cd lib && $(MAKE); \
	else \
		echo "Please install the python-dev or python3-dev packages before proceeding."; \
		echo "On MacOS installing Python 3 through Homebrew should work out of the box."; \
		echo "You should be able to run 'make' after installing it."; \
		exit 1; \
	fi

.PHONY: clean
clean:
	cd lib && $(MAKE) clean

.PHONY: run
run: build
	flask --app app run --debugger --reload