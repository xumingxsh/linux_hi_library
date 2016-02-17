程序使用：
1： 将“linux_hi_library”文件夹放置到用户根目录（“~”）下。
2： 将“~/linux_hi_library/bin”设置为“$PATH”
export PATH=$PATH:~/linux_hi_library/bin
3： 设置~/linux_hi_library/bin下“2”文件的权限
chmod 777 ~/linux_hi_library/bin/2

4： 编译并测试
hicommon库(在shell终端中执行命令： 2 qdtyk)
hinet库(在shell终端中执行命令： 2 qdhinet)
hidb库(在shell终端中执行命令： 2 qdhidb)

注：
1： 第三条的解释：
“2”文件的目的是为了能像普通的shell命令一样使用“2”文件。
例如：
a) 编译我的hicommon库，并运行测试程序，可以使用命令：2 qdtyk
b) 编译我的hinet库，并运行测试程序，可以使用命令：2 qdhinet
c) 编译我的hidb库，并运行测试程序，可以使用命令：2 qdhidb
2： 程序的以来环境：
1） gcc： 4.8.2及以上版本，需要支持C++11
2） python2.7
3） 编译hidb需要安装安装mysql的c语言开发接口
4） 程序在ubuntu14.06和centos6.6上编译运行通过
3： 在编译程序时，build_run.py会自动生成cpp.mak文件并进行编译

其他：
1： 对C++11新特性的态度
我的态度是能不用新特性就不用新特性，
因为用新特性会给第一次接触这些特性的人带来阅读困难。

但是本程序库用到了很多C++11的新特性，这都是由于需要才采用的，
而不是为了验证技术而采用新特性。
例如final，delete的引入。
目标：
	希望对象不要被拷贝和继承
解决思路
	最初想要添加个nocopy基类，但这样会带来复杂度，后来想到用宏，也不是很好，
	而且不被继承也没有太好的思路（有个friend的解决方式，更复杂）
	后来才发现可以用C++11的delete和final。
2： 对待技术的态度
我一直对业务更有兴趣。所谓工欲善其事，必先利其器。
提升技术是更快，更好的响应需求的一种方式，所以对技术也比较感兴趣。
这些程序库是对以前项目中服务程序反思，抽象和整理的结果。
如果没有业务的支撑，也许根本不知道哪些技术对自己是有用的，哪些是没用的。
3： 这个程序库是我工作之余，用一个半月完成的。目的是为了学习Linux编程而写的。
在此过程中，从零开始学习了Linux的shell，makefile。
hicommon中大部分是移植自我的windows程序，hidb移植自我的windows程序，
hinet为全新程序。


 