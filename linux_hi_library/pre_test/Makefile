# 编译程序库，编译成功后，将so文件链接到~/lib中
# 存在问题：
#	1) 曾经有个想法，写个程序，根据本文件夹及其子文件夹中的cpp文件，
# 	   自动生成一个makefile，一致还没有动手
include common.mak
 
COMM_PROPERTY := $(CMM_OPTION)  -std=c++11
	
# Add source files here
EXECUTABLE  :=  run/test


# Cuda source files (compiled with cudacc) c++:-fno-inline del:-O2
OBJS= main.o

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