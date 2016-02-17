# 编译程序库，编译成功后，将so文件链接到~/lib中
# 存在问题：
#	1) 曾经有个想法，写个程序，根据本文件夹及其子文件夹中的cpp文件，
# 	   自动生成一个makefile，一致还没有动手
include ../common.mak
include cpp.mak
 
COMM_PROPERTY := $(CMM_OPTION)  
	
# Add source files here
EXECUTABLE  :=  lib/lib$(HI_COMMON_NAME).so


# Cuda source files (compiled with cudacc) c++:-fno-inline del:-O2
OBJS= $(CPP_SOURCE)

.cpp.o:
	$(build_c)

all: clean createDir $(OBJS) $(EXECUTABLE) clean_mid link_so

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
