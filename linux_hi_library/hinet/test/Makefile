include ../../common.mak
include cpp.mak

COMM_PROPERTY := $(CMM_OPTION)  -l$(HI_COMMON_NAME) -l$(HI_NET_NAME) \
	-I../include -I../../test -L../lib 
	
EXECUTABLE  =  $(TEST_EXE)
TEST_LIB = lib$(HI_NET_NAME).so

OBJS= ../../test/test.o $(CPP_SOURCE)

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
	$(copy_common_so_2_test)
	$(prepare_test_so)
