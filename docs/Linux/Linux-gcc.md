---
typora-root-url: Image
---

# gcc的工作流程

gcc编译器将c源文件到生成一个可执行程序，中间一共经历了四个步骤：

<img src="/gcc.png" alt="gcc" style="zoom:100%;" />

![编译工具](/编译工具.png)

下面以test.c为例介绍gcc的四个步骤:

* `gcc -E test.c -o test.i`
* `gcc -S test.i -o test.s`
* `gcc -c test.s -o test.o`
* `gcc test.o -o test`

一步生成最终的可执行程序: 

`gcc test.c -o test`

# gcc常用参数

* -v  查看gcc版本号, --version也可以

* -E  生成预处理文件

* -S  生成汇编文件

* -c 只编译,生成.o文件, 通常称为目标文件

* -I （大写i）指定头文件所在的路径

* -L 指定库文件所在的路径

* -l（小写L） 指定库的名字

* -o 指定生成文件的名字

* -g 包含调试信息，使用gdb调试需要添加-g参数

* -Wall 提示更多警告信息

  