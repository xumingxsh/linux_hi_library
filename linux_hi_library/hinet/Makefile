# 编译程序库，编译成功后，将so文件链接到~/lib中
# 存在问题：
#	1) 曾经有个想法，写个程序，根据本文件夹及其子文件夹中的cpp文件，
# 	   自动生成一个makefile，一致还没有动手

include ../common.mak
include cpp.mak

COMM_PROPERTY := $(CMM_OPTION) -l$(HI_COMMON_NAME) \
	-L./lib -I../hicommon/include -Iinclude
	
EXECUTABLE  :=  lib/lib$(HI_NET_NAME).so

OBJS= $(CPP_SOURCE)
	
.cpp.o:
	$(build_c)

all: clean createDir prepear $(OBJS) $(EXECUTABLE) clean_mid 

$(EXECUTABLE): $(CUFILES)
	$(create_so)
	
clean:
	$(clear_file)	
createDir:
	$(create_lib)	
	
clean_mid:
	$(clear_mid_file)
	
prepear:
	$(copy_common_so)
