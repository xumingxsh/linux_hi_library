# coding=utf-8
# 如果不添加“coding=utf-8”，则会出现编译错误
# 获得当前文件的路径，并作为参数，调用build_common中的build_run函数
# 存在问题（疑问）：
#	1) 有什么方法能把获得当前文件路径的及设置系统路径的代码也放置到
#		build_common中，好像没办法

import os
import sys

local = os.path.dirname(os.path.abspath(__file__))
parent = os.path.dirname(local)
sys.path.append(parent)
import build_common
		
build_common.make_build_run(local)