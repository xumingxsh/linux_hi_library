INCLUDE_PATH := -I/usr/include -I../include 

# ANSI/ISO C标准（兼容；警告；错误）;使用C++11标准；
# # 对某些告警显示更详细的信息；显示所有的警告信息；
# # 产生的代码不要包含对函数和变量具体内存位置的引用
# # 启用编译器优化；
# # 支持 long long类型;没有使用的参数不作警告
# # （注意：std必须在ansi之前，否则编译会有问题，例如stl类找不到之类的）
# # 检错
# # -pedantic 帮助程序员发现不符合ansi/iso标准代码。
# # -Wall 使gcc产生尽可能多的警告信号
# # -Werror会在警告的地方停止编译，迫使程序员对自己代码进行修改
OPT := -ansi -pedantic -pedantic-errors -std=c++11 \
    -W -Wall -Wno-unused-parameter 

    
OPTION := $(OPT) $(INCLUDE_PATH)  $(LIB_PATH) -lgtest

.cpp.o:
	g++ -c $< $(OPTION)

all: clean ../impl/common/hiLog.o logtest.o 
	g++ -o test $(OPTION) -lgtest_main hiLog.o logtest.o
	export LD_LIBRARY_PATH=/usr/lib;./test

clean:
	rm -f *.o

