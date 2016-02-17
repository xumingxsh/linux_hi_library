#coding=utf-8
# 如果不添加“coding=utf-8”，则会出现编译错误
# 该文件的功能是根据传入的参数，从config.csv中取出相应的命令并执行
# 在windows中，我曾经实现了一个相同的程序，那个程序是用MFC和批处理实现的
# 该文件和config.csv建议添加到"~/python"文件夹中
# config.csv的格式：
#	命令,执行方式,执行内容

import os
import sys
		
cpp_source_path = "~/linux_hi_library/linux_hi_library/"
local = os.path.dirname(os.path.abspath(__file__))
config_path = os.path.join(local,"config.csv")
cmds = open(config_path)

def cd_folder(path):
	os.system('echo cd ' + handle_content(path))

def print_error(err):
	os.system('echo ' + err + '')

def handle_content(content):
	return content.replace("[cpp_source_path]", cpp_source_path)
	
cmd_run="cd"
cmd_content=""
for line in cmds:
	arr = line.split(",")
	if len(arr) != 3:
		continue
	if str(sys.argv[1]) != arr[0]:
		continue
	cmd_run = arr[1].strip()
	cmd_content = arr[2]
	break

if cmd_content != "":
	cmd_content = cmd_content.replace("[cpp_source_path]", cpp_source_path)
	if cmd_run == "folder":
		cd_folder(cmd_content)
	elif cmd_run == "sh":
		os.system('sh ' + cmd_content)
	elif cmd_run == "py":
		os.system('python ' + cmd_content)
	else:
		print_error('this command not support:' + cmd_run)
else:
	print_error('this command not support:' + str(sys.argv[1]))
	