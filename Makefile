.PHONY: clear build build-tests run-tests run-prog

build:
	@cmake -B build -S .
	@cmake --build build

build-tests:
	@cmake -DPACKAGE_TESTS=ON -B build -S .
	@cmake --build build

run-tests: build-tests
	@ctest --test-dir build

run-prog: build
	@cd build && ./channel

clear:
	rm -rf build
