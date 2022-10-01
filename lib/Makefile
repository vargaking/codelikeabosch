.DEFAULT_GOAL := lib$(suffix)

ifeq ($(uname),Darwin)
	add_flags := $(CXX) $(CXXFLAGS) -shared -fPIC -undefined dynamic_lookup -o 
else
	add_flags := $(CXX) $(CXXFLAGS) -shared -fPIC -o
endif


lib$(suffix): python.cpp
	$(add_flags)$@ $^

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.dll