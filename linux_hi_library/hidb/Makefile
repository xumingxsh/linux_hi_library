include ../common.mak
include cpp.mak

COMM_PROPERTY := $(CMM_OPTION) -l$(HI_COMMON_NAME) \
	-L./lib -I../hicommon/include -lmysqlclient -L/usr/lib/mysql 

EXECUTABLE  :=  lib/lib$(HI_DB_NAME).so

OBJS= $(CPP_SOURCE)
	
.cpp.o:
	$(build_c)

all: clean createDir prepear $(OBJS) $(EXECUTABLE) clean_mid link_so

$(EXECUTABLE): $(CUFILES)
	$(create_so)
	
clean:
	$(clear_file)	
createDir:
	$(create_lib)		
	
clean_mid:
	$(clear_mid_file)
	
link_so:
	$(link_so_2_lib)
	
prepear:
	ln -f ../hicommon/lib/lib$(HI_COMMON_NAME).so ~/lib$(HI_COMMON_NAME).so
	cp -f ../hicommon/lib/lib$(HI_COMMON_NAME).so ./lib/
