COURSE = 	cs572
COURSE_DIR = 	$(HOME)/$(COURSE)

CXX =		clang++
#CXX =		g++11
CPPFLAGS =	-I$(COURSE_DIR)/include
CXXFLAGS = 	-g -Wall -Wall -pthread -std=c++11
LDFLAGS =	-L . -L $(COURSE_DIR)/lib
LDLIBS =	-lpthread -lm

#main program being built
TARGET =       tl0

#header files
H_FILES = \
  eval.h \
  lexer.h

#c++ files needed to build TARGET
CPP_FILES = \
  eval.cpp \
  lexer.cpp \
  tl0.cpp

#c++ files used for unit tests
UNIT_TEST_CPP_FILES = \
  eval_test.cpp \
  lexer_test.cpp

#interactive tests built using a conditionally-included main().
MAIN_TESTS = \
  lexer_main

#all source files
SRC_FILES = \
  Makefile \
  README \
  $(H_FILES) \
  $(CPP_FILES) \
  $(UNIT_TEST_CPP_FILES)


OBJS = $(CPP_FILES:.cpp=.o)
UNIT_TESTS = $(UNIT_TEST_CPP_FILES:.cpp=)
DEPENDS = $(CPP_FILES:.cpp=.depends) $(UNIT_TEST_CPP_FILES:.cpp=.depends)

.phony:		clean test

#build all executables and run unit tests
all:		$(TARGET) $(MAIN_TESTS) test

#build main target
$(TARGET):	$(OBJS)
		$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@


#build and run all unit tests
test:		$(addsuffix .test, $(UNIT_TESTS))

%.test:		%
		./$<

#interactive lexer test
lexer_main:	lexer.cpp lexer.h
		$(CXX) $(CPPFLAGS)  $(CXXFLAGS) -DTEST_LEXER $< \
		  $(LDFLAGS) $(LDLIBS) -o $@

.SECONDEXPANSION:

#build unit tests
$(UNIT_TESTS):	$$@.o $(filter-out $(TARGET).o, $(OBJS)) | gtest
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) \
		  -lgtest -lgtest_main -o $@

clean:
		rm -f *~ *.o $(DEPENDS) $(TARGET) \
			$(UNIT_TESTS) $(MAIN_TESTS) $(GTEST_LIBS)


# Google test section

#google test libraries
GTEST_LIBS =	libgtest.a libgtest_main.a

.phony:		gtest

gtest:		$(GTEST_LIBS)


GTEST_DIR = 	$(COURSE_DIR)/src/googletest/googletest


include $(COURSE_DIR)/include/gtest-makefile


#This rule creates a .depends file for .cpp prerequisites.
%.depends:	%.cpp
		@$(CPP) $(CPPFLAGS) -MM $< > $@


-include $(DEPENDS)