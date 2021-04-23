# gdb介绍

GDB（GNU Debugger）是GCC的调试工具。其功能强大

GDB主要帮助完成以下四个方面的功能：

* 启动程序，可以按照自定义的要求随心所欲的运行程序
* 可以让被调试的程序在所指定的端点处停住。（断点可以是条件表达式）
* 当程序被停住时，可以检查此时程序中发生的事。
* 动态的改变程序的执行环境。

# 生成调试信息

一般来说GDB主要调试的是C/C++的程序。要调试C/C++的程序，首先在编译时，我们必须要把调试信息加到可执行文件中。使用编译器（cc/gcc/g++）的-g参数可以做到这一点。

`gcc -g hello.c -o hello`

如果没有 -g，将看不见程序的函数名，变量名，所代替的全部是运行时的内存地址。

# 启动dgb

* 启动gdb：`gdb program（可执行程序的名字+路径）`

  program 也就是执行文件，一般在当前目录下

  `gdb ./main`

* 设置运行参数

  `set args`可指定运行时的参数。（如：`set args 10 20 30 40 50`)

  `show args`命令可以查看设置好的运行参数。

* 启动程序

  `run`: 程序开始执行，如果有断点，停在第一处断点

  `start`:程序向下执行一行。（在第一条语句处停止）

# 显示源代码

GDB可以打印出所调试程序的源代码，当然，在程序编译时一定要加上 -g 的参数，吧源程序信息编译到可执行的文件中。不然看不到源程序。当程序停下来以后，GDB会报告程序停在了那个文件的第几行上。可以用 `list` 命令来打印程序的源代码，默认打印10行，list命令的用法如下所示：

* `list linenum`:打印linenum行的上下文内容
* `list function`:显示函数名为function的函数的源程序。
* `list`:显示当前行后面的源程序。
* `list -`:显示当前文件开始处的源程序。
* `list file:linenum`:显示file文件下第几行
* `list file:function`:显示file文件的函数名为function的函数的源程序

一般是打印当前行的上5行和下5行，如果显示函数是上2行下8行，默认是10行，当然，也可以指定显示的范围，使用下面的命令可以设置一次显示源程序的行数：

* `set listsize count`:设置一次显示源代码的行数。
* `show listsize`:查看当前listzise的设置值。

# 设置断点

## 1、简单断点---当期文件

break设置断点，可以简写为b：

* `b 10`设置断点，在源程序第10行
* `b func`设置断点，在func函数入口处

## 2、多文件设置断点---其他文件

在进入指定函数时停住：

* `b filename:linenum`:在源文件filename的linenum行处停住
* `b filename:function`:在源文件filename的function函数的入口处停住

## 3、查询所有断点

`info b==info break==i break== i b`

## 4、条件断点

一般来说，为断点设置一个条件，我们使用if关键词，后面跟其断点条件。设置一个条件断点：

`b test.c:8 if intValue==5`

## 5、维护断点

* `delete [range...]`删除指定的断点，其简写命令为d。

  如果不指定断点号，则表示删除所有的断点。range表示断点号的范围（如：3-7）。

  * 删除某个断点：`delete num`
  * 删除多个断点：`delete num1 num2 ...`
  * 删除 连续的多个断点：`delete m-n`
  * 删除所有的断点：`delete`

  比删除更好的一种方法是disable停止点（设置无效），disable了的停止点，GDB不会删除，当还需要时，enable（设置有效）即可。

* `disable[range..]`使指定断点无效，简写命令是dis。

  如果什么都不指定，表示disable所有的停止点。

  * 使一个断点无效：`disable num`
  * 使多个断点无效：`disable num1 num2 ...`
  * 使多个连续的断点无效：`disable m-n`
  * 是所有的断点无效：`disable` 

* enable[range...]是无效的断点生效，简写命令是ena。

  如果什么都不指定，表示enable所有的停止点。

  * 使一个断点有效：`enable num`
  * 使多个断点/有效：`enable num1 num2 ...`
  * 使多个连续的断点有效：`enable m-n`
  * 是所有的断点有效：`enable`

# 调试代码

* `run` 运行程序，可简写为r
* `next`单步跟踪，函数调用当作一条简单语句执行，可简写为n
* `step`单步跟踪，函数调用进入被调函数体内，可简写为s
* `finish`退出进入的函数，如果出不去，看一下函数体中的循环中是否有断点，如果有则删掉，或者设置无效
* `until`在一个循环体内单步跟踪时，这个命令可以使运行程序直接退出循环体，可简写为u，如果出不去，看一下函数体中的循环体中是否有断点，如果有删掉，或者设置无效
* `continue`继续运行程序，可简写为c（若有断点则跳到下一断点处）

# 查看变量的值

## 1、查看运行时变量的值

`print`打印变量、字符串、表达式等的值，可简写为p

`p count`:打印count的值

## 2、自动显示变量的值

可以设置一些自动显示的变量，当程序停住时，或是在单步跟踪时，这些变量会自动显示

* `dispaly 变量名`

* `info display`:查看display设置的自动显示的信息。

* `undisplay num(info display时显示的编号)`

* `delete display dnums...`:删除自动显示，dnums意为所设置好了的自动显示的编号。如果要同时删除几个，编号可以用空格分隔，如果要删除一个范围内的编号，可以用减号（-）表示（如2-5）

  删除某个自动显示：`undisplay num`或者`delete display num`

  删除多个：`delete display num1 num2...`

  删除一个范围：`delete display m-n`

* `disable display dnums...`

  使一个自动显示无效：`disable display num`

  使多个自动显示无效：`disable display num1 num2...`

  是使一个范围的自动显示无效：`disable display m-n`

* `enable display dnums`

  使一个自动显示有效：`enable display num`

  使多个自动显示有效：`enable display num1 num2...`

  使一个范围的自动显示有效：`enable display m-n`

## 3、查看修改变量的值

* `ptype width`:查看变量width的类型

  type=double

* `p width`:打印变量width的值

  $4=13

可以使用`set var`命令来告诉GDB，width不是GDB的参数，而是程序的变量名，如：

`set var width=47` :将变量的var值设置为47

在改变程序变量取值时，最好都使用`set var`格式的GDB命令。

# 退出gdb

`quit`或者`ctrl+d`

