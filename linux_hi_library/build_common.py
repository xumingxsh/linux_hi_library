#coding=utf-8
# 如果不添加“coding=utf-8”，则会出现编译错误
# 主要提供执行so和测试文件编译并运行的shell脚本，由build_run实现
# 该功能会被多个库使用，该文件的功能与run.sh（每个库目录下的一个shell文件）
# 的功能相似
# 存在问题：(2015-02-14)
# 	1） test.run经常不能运行，例如：
#		shell文件中的命令：
#export LD_LIBRARY_PATH=~/lib && echo $LD_LIBRARY_PATH && ls ~/lib  && ./test.run
#		结果：
#~/lib
#libhicommon.so  libhidb.so  libhinet.so
#./test.run: error while loading shared libraries: libhicommon.so: cannot open 
#shared object file: No such file or directory
#		我还不知道怎么解决
# (2015-02-14)
# 		1） 通过将so文件拷贝到运行文件的文件夹，并设置
#		export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
# 		解决了2015-02-14存在的问题1
# 		2） 添加自动生成.o的功能。主要是在makefile文件夹中添加cpp.mak并包含，
#		在cpp.mak中，声明CPP_SOURCE变量
import os
import re

link = '\n'

def build_run(path):	
	"执行编译os，测试文件test.run并运行test.run的shell脚本"
	#os.system("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/lib")
	#print os.environ
	os.system('cd ' + path + link + ' make')
	print "make so library completed"
	test_path = os.path.join(path,"test")
	os.system('cd ' + test_path + link + ' make')
	print "make test object completed"
	cmd = 'cd ' + test_path + "/run"
	cmd = cmd + link + ' export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH'
	cmd = cmd + link + ' ./test'
	#print cmd
	os.system(cmd)

# 根据文件夹及其子文件夹下的cpp文件获得编译o文件
def list_cpp(path, lc):
	full_path = os.path.join(lc, path)
	if path == "":
		full_path = lc
	ls = os.listdir(full_path)  
	for i in ls:
		child_path = os.path.join(full_path,i)
		if os.path.isfile(child_path) == False:
			if i == "test":
				continue
			if path == "":
				list_cpp(i, lc)
				continue
			if path.endswith('/'):
				list_cpp(path + i, lc)
			else:			
				list_cpp(path + "/" + i, lc)
		else:
			if i.upper().endswith('.CPP'):
				if path == "":
					cpps.append(i)
				else:
					cpps.append(path + "/" + i)

cpps = []
def create_cpp_mac(lc):
	global cpps
	cpps = []
	local = lc
	list_cpp("", lc)
	
	fpath = os.path.join(lc, 'cpp.mak')
	f = open(fpath, "w+")
	f.write("CPP_SOURCE = \\\n")
	index = 0
	for i in cpps:
		index = index + 1
		if index == len(cpps):
			f.write("\t" + i[0: len(i) - 4] + ".o\n")
		else:
			f.write("\t" + i[0: len(i) - 4] + ".o\\\n")
	f.close()
	del cpps

def make_build_run(path):	
	create_cpp_mac(path)
	print "create lib cpp.mak success"
	test_path = os.path.join(path, 'test')	
	create_cpp_mac(test_path)
	print "create test cpp.mak success"
	build_run(path)
	print "make and run finish"

	