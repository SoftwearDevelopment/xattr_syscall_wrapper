cxx_targets = listxattr getxattr setxattr removexattr
all_targets = $(cxx_targets)

prefix ?= /usr/local/
bin    ?= $(prefix)/bin/

CXXFLAGS += -Wall -Werror -Wextra -Wpedantic -std=c++11

.PHONY: clean install uninstall all

all: $(cxx_targets)

%: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) "$<" -o "$@"

clean:
	rm -fv $(cxx_targets)

install: $(all_targets)
	cp -v $(all_targets) "$(bin)"

uninstall:
	cd "$(bin)"; rm -fv $(all_targets)
