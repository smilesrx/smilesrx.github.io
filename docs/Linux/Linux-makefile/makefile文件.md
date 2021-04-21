---
typora-root-url: Image
---

 # makefile文件

makefile文件中定义了一系列的规则来指定, 哪些文件需要先编译, 哪些文件需要后编译, 哪些文件需要重新编译, 甚至于进行更复杂的功能操作, 因为makefile就像一个Shell脚本一样, 其中也可以执行操作系统的命令. makefile带来的好处就是——“自动化编译”, 一旦写好, 只需要一个make命令, 整个工程完全自动编译, 极大的提高了软件开发的效率.

make是一个命令工具, 是一个解释makefile中指令的命令工具, 一般来说, 大多数的IDE都有这个命令, 比如：Visual C++的nmake, Linux下GNU的make. 可见, makefile都成为了一种在工程方面的编译方法.

makefile文件中会使用gcc编译器对源代码进行编译, 最终生成可执行文件或者是库文件.

makefile文件的命名：makefile或者Makefile

# makefile的基本规则

## 1、规则如下：

```makefile
目标：依赖
（tab）命令
```

## 2、makefile基本规则三要素：

* 目标：要生成的目标文件
* 依赖：目标文件由哪些文件生成
* 命令：通过执行该命令由依赖生成目标

# makefile工作原理

## 1、基本原理

若想生成目标，检查规则中的所有依赖文件是否存在：

* 如果所有的依赖文件不存在，则向下搜索规则，看是否有生成该依赖的规则：

  如果有规则用来生成该依赖文件，则执行规则中的命令生成依赖文件；

  如果没有规则用来生成该依赖文件，则报错。

  <img src="/依赖文件不存在.png" alt="依赖文件不存在" style="zoom:67%;" />

* 如果所有的依赖都存在，检查规则中的目标是否需要更新，必须先检查它的所有依赖，依赖中任何一个被更新，则目标必须更新（检查的规则是哪个时间大哪个最新）

  若目标的时间 > 依赖的时间，不更新

  若目标的时间 < 依赖的时间，则更新

  <img src="/依赖文件都存在.png" alt="依赖文件都存在" style="zoom:67%;" />

## 2、总结

* 分析各个目标和依赖之间的关系
* 根据依赖关系自底向上指向命令
* 根据依赖文件的时间和目标文件的时间确定是否需要更新
* 如果目标不依赖任何文件，则执行对应的命令，以示更新（如：伪目标）

# makefile中的变量

在makefile中使用变量有点类似于C语言中宏定义，使用该变量相当于内容替换，使用变量可以使makefile文件易于维护，修改起来变得简单。

makefile中有三中变量：

* 普通变量
* 自带变量
* 自动变量

## 1、普通变量

* 变量定义直接用 =

* 使用变量值用 $(变量名)

  ```makefile
  foo=abc      //定义变量并赋值
  bar=$(foo)   //使用变量，%=$(变量名)
  ```

  定义了两个变量：foo、bar，其中bar的值是foo变量的值的引用。

## 2、自带变量

除了使用用户自定义的变量，makefile中也提供了一些变量（变量名大写）供用户直接使用，我们可以直接对其进行赋值：

```makefile
CC=gcc #arm-linux-gcc
CPPFLAGS:C预处理的选项 -I
CFLAGS:C编译器的选项 -Wall -g -c
LDFLAGS:链接器选项 -L -l
```

## 3、自动变量

* $@:表示规则中的目标
* $<:表示规则中的第一个条件
* $^:表示规则中的所有条件，组成一个列表，以空格隔开，如果这个列表中有重复的项则消除重复项。
* **特别注意：**自动变量只能在规则的命令中使用

# makefile的模式规则

至少在规则的目标定义中要包含’%’, ‘%’表示一个或多个, 在依赖条件中同样可以使用’%’, 依赖条件中的’%’的取值取决于其目标:

比如: `main.o:main.c fun1.o: fun1.c fun2.o:fun2.c`, 说的简单点就是: xxx.o:xxx.c

# makefile中的函数

## 1、wildcard-查找指定目录下的指定类型的文件

`src=$(wildcard *.c)` //找到当前目录下所有后缀为.c的文件,赋值给src

等价于：`src=main.c fun1.c fun2.c sum.c`

## 2、patsubst--匹配替换

`obj=$(patsubst %.c,%.o, $(src))` //把src变量里所有后缀为.c的文件替换成.o

等价于：`obj=mian.o fun1.o fun2.o sum.o`

**注意：在makefile中所有的函数都有返回值**

# makefile的清理操作

用途：清除编译生成的中间 .o文件和最终的目标文件

make clean 如果当前目录下有同名clean文件，则不执行clean对于的命令，解决方案：

伪目标什么：`.PHONY:clean`

申明目标为伪目标之后，makefile将不会检查该目标是否存在或者该目标是否需要更新

clean命令中的特殊符号：

* “ - ” 此条命令出错，make也会继续执行后续的命令。如：`rm -main.o``

  `rm -f` 强制执行，比如若要删除的文件不存在使用-f不会报错

* “ @ ”不显示命令的本身，只显示结果。如：`@echo clean done`

make 默认执行第一个出现的目标，可通过make dest指定要执行的目标

make -f ：-f执行一个makefile文件名称，使用make执行指定的makefile:`make -f mainmak`

# makefile 文件的版本优化

## 1、makefile版本一

```makefile
main:mian.c fun1.c fun2.c sum.c
	gcc main.c fun1.c fun2.c sum.c -o main -I./
```

缺点：效率低，修改一个文件，所有的文件会全部重新编译。

## 2、makefile版本二

```makefile
main:main.o fun1.o fun2.o sum.o
	gcc mian.o fun1.o fun2.o fun2.o sum.o -o main
main.o:main.c
	gcc -c main.c -o main.o
fun1.o:fun1.c
	gcc -c fun1.c -o fun1.o
fun2.o:fun2.c
	gcc -c fun2.c -o fun2.o
sum.o:sum.c
	gcc -c sum.c -o sum.o
```

缺点：冗余，若 .c 文件数量很多，编写起来比较麻烦

## 3、makefile版本三

```makefile
target=mian
object=mian.o fun1.o fun2.o sum.o      ## 普通变量
CC=gcc     ##自带变量
CPPFLAGS=-I./

$(target):$(object)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) -c $< -o $@
```

## 4、makefile版本四

```makefile
src=$(wildcard ./*.c)
object=$(patsubst %.c,%.o,$(src))
target=mian
CC=gcc
CPPFLAGS=-I./

$(target):$(object)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) -c $< -o $@
```

缺点：每次重新编译都需要手工清理中间的 .o 文件和最终的目标文件

## 5、makefile版本五

```makefile
src=$(wildcard ./*.c)
object=$(patsubst %.c,%.o,$(src))
target=main
CC=gcc
CPPFLAGS=-I./

$(target):$(object)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) -c $< -o $@ 
.PHONY:clean
clean:
	rm -f $(target) $(object)
```

在makefile的第五个版本中综合使用了变量、函数、模式规则和清理命令，是一个比较完整的版本



























