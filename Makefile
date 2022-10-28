all:
	g++ bindings.cc -shared -o bindings.dylib -std=c++17
