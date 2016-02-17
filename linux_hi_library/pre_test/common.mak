# makefile的通用文件，主要定义需要的宏，需要的gcc选项，以及需要的命令包
# 存在问题（或疑问）：
# 1) 不清楚makefile是否支持自定义函数，所以没有使用函数
# 2) 好像标签不能再其他文件中，所以在此处没有声明标签（不知掉理解是否正确）
# 3) 命令包好像不允许传参数

DEBUG_ = _RELEASE
GCPP = g++

# 是否启用函数日志（进入，离开函数打印日志，并统计执行时间）
HI_USE_FUN_LOG = -DUSE_FUN_LOG
HI_COMMON_NAME = hicommon
HI_DB_NAME = hidb
HI_NET_NAME = hinet

################################################################################
# Source and Target
################################################################################

CMM_INCLPATH := -Iinclude  -I/usr/include -I/usr/include \
	-L/usr/lib/i386-linux-gnu

# ANSI/ISO C标准（兼容；警告；错误）;使用C++11标准；
# 对某些告警显示更详细的信息；显示所有的警告信息；
# 产生的代码不要包含对函数和变量具体内存位置的引用
# 启用编译器优化；
# 支持 long long类型;没有使用的参数不作警告
# （注意：std必须在ansi之前，否则编译会有问题，例如stl类找不到之类的）
CMM_GCC_OPTION := \
	-ansi -pedantic -pedantic-errors \
	-W -Wall \
	-fPIC -pthread \
	-fstrict-aliasing -Wstrict-aliasing=2 \
	-Wno-long-long -Wno-unused-parameter \
	-Wno-non-virtual-dtor -Wno-uninitialized  
CMM_MAC := -DLINUX_ -D$(DEBUG_) -DLINUX=2 -D_REENTRANT \
	-D_LARGEFILE64_SOURCE $(HI_USE_FUN_LOG)

CMM_OPTION := $(CMM_GCC_OPTION) $(CMM_MAC) $(CMM_INCLPATH)  
	
OBJS2= *.o 
TEST_EXE = run/test

# 定义命令包，makefile中，似乎不能自定义函数，只能用命令包
# 命令包好像还不能传参数（个人理解）
define create_lib
	mkdir -p lib
endef

define clear_mid_file
	rm -f $(OBJS2)
endef

define clear_file
	rm -f $(EXECUTABLE) $(OBJS2)
endef

# 编译汇编文件（不进行连接）
define build_c
	$(GCPP) -c $< $(COMM_PROPERTY)
endef

# 编译成so文件
define create_so
	$(GCPP) -shared -Wall -g -w -o $(EXECUTABLE) $(OBJS2) \
	$(COMM_PROPERTY) -D_GNU_SOURCE 
endef

# 编译成可执行文件
define create_exe
	$(GCPP) -Wall -g -w -o $(EXECUTABLE) $(OBJS2)\
	$(COMM_PROPERTY) -D_GNU_SOURCE -lm 
endef

# 准备so文件
define copy_common_so
	cp -f ../hicommon/lib/lib$(HI_COMMON_NAME).so ./lib
endef

# 为测试准备so文件
define prepare_test_so
	cp -f ../lib/$(TEST_LIB) run/
endef

# 为测试准备so文件
define copy_common_so_2_test
	cp -f ../../hicommon/lib/lib$(HI_COMMON_NAME).so run/
endef

