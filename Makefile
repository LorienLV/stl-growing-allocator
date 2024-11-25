# Set compiler.
# CC=gcc
CXX:=g++

# Set flags.
# CFLAGS=
CXXFLAGS:=-Wall -O3 -std=c++23
DEPFLAGS:=-MD -MP # Autogenerate dependencies.
CPPFLAGS:=-DDEBUG=1
# LDFLAGS:=
# INCLUDES:=

# Directories.
BUILDDIR:=build
SRCDIR:=src
OBJDIR:=$(BUILDDIR)/obj

# Files.
SRCS:=$(shell find $(SRCDIR) -name "*.cpp")
OBJS:=$(subst $(SRCDIR),$(OBJDIR),$(SRCS:%.cpp=%.o))
DEPS:=$(subst $(SRCDIR),$(OBJDIR),$(SRCS:%.cpp=%.d))

#
# Executables.
#
TEST:=$(BUILDDIR)/test

#
# Generate executables
#
$(TEST): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#
# Generate objects.
#
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(INCLUDES) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Include dependencies.
-include $(DEPS)

# The default goal is the next goal.
.DEFAULT_GOAL:=
.PHONY: all
all: main

main: $(TEST)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/*