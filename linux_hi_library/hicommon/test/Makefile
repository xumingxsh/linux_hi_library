include ../../common.mak
include cpp.mak

COMM_PROPERTY := $(CMM_OPTION) -l$(HI_COMMON_NAME) \
	-I../include -I../../test -L../lib -I~/lib \
	-I../../hicommon/include -L../lib -std=c++11

EXECUTABLE  =  $(TEST_EXE)
TEST_LIB = lib$(HI_COMMON_NAME).so

OBJS= ../../test/test.o  $(CPP_SOURCE)

.cpp.o:
	$(build_c)

all: clean prepear $(OBJS) $(EXECUTABLE) clean_mid

$(EXECUTABLE): $(CUFILES)
	$(create_exe)
	
clean:
	$(clear_file)
clean_mid:
	$(clear_mid_file)
	
prepear:
	mkdir -p run
	$(prepare_test_so)