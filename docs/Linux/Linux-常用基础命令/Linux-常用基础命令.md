---
typora-root-url: Image
---

# Shell 相关

## 1、什么是Shell

shell就是命令解析器, shell将用户输入的命令解释成内核能够识别的指令;

现在我们使用的操作系统（Windows、Mac OS、Android、iOS等）都是带图形界面的，简单直观，容易上手                                    

计算机的早期并没有图形界面，我们只能通过一个一个地命令来控制计算机，这些命令有成百上千之多，记住这些命令非常困难

对于图形界面，用户点击某个图标就能启动某个程序；

对于命令行，用户输入某个程序的名字（可以看做一个命令）就能启动某个程序。

这两者的基本过程都是类似的，都需要查找程序在硬盘上的安装位置，然后将它们加载到内存运行

**然而，真正能够控制计算机硬件（CPU、内存、显示器等）的只有操作系统内核（Kernel），图形界面和命令行只是架设在用户和内核之间的一座桥梁。**

用户不能直接接触内核，需要另外再开发一个**程序**，让用户直接使用这个程序；该程序的作用就是接收用户的操作（点击图标、输入命令），并进行简单的处理，然后再传递给内核。如此一来，用户和内核之间就多了一层“代理”，这层“代理”既简化了用户的操作，也保护了内核。
**用户界面**和**命令行**就是这个另外开发的**程序**，就是这层“代理”。在Linux下，这个命令行程序叫做 Shell。

用户在终端输入命令, 由shell命令解释器对命令进行解析(按照$PATH环境变量搜索命令), 解析成内核能够识别的指令, 然后由内核执行命令, 最后由终端显示命令执行的结果给用户.

**注意**：shell在寻找命令的时候是按照$PATH环境变量去查找的，如果找到了就执行对应的命令，若找不到就报错.

 执行`echo $PATH`可以查看PATH环境变量的值.

## 2、Linux的Shell种类

* Bourne Shell（/usr/bin/sh或/bin/sh）
* Bourne Again Shell（/bin/bash）
* C Shell（/usr/bin/csh）
* K Shell（/usr/bin/ksh）
* Shell for Root（/sbin/sh）

## 3、查看当前系统使用的Shell

```shell
echo $SHELL
## /bin/bash
```

## 4、当前系统支持的Shell

```shell
cat /etc/shells
## /bin/sh
## /bin/bash
## /usr/bin/bash
## /bin/rbash
## /usr/bin/rbash
## /bin/dash
## /usr/bin/dash
```

# Linux 目录结构

Linux系统的目录结构是一个倒立的树状结构, 根目录用  /  表示

<img src="/目录结构.png" alt="目录结构" style="zoom:67%;" />

## 1、Linux目录详解

* **/bin**：

  bin 是 Binaries (二进制文件) 的缩写, 这个目录存放着最经常使用的命令。如: ls , rm , mv, cp等常用命令

* **/boot：**

  这里存放的是启动 Linux 时使用的一些核心文件，包括一些连接文件以及镜像文件。

* **/dev ：**

  dev 是 Device(设备) 的缩写, 该目录下存放的是 Linux 的外部设备，在 Linux 中访问设备的方式和访问文件的方式是相同的。

  字符设备文件、块设备文件，在linux下, 一切皆是文件

* **/etc：**

  etc 是 Etcetera(等等) 的缩写,这个目录用来存放所有的系统管理所需要的配置文件和子目录。

* **/home**：

  用户的主目录，在 Linux 中，每个用户都有一个自己的目录，一般该目录名是以用户的账号命名的，

* **/lib**：

  lib 是 Library(库) 的缩写这个目录里存放着系统最基本的动态连接共享库，其作用类似于 Windows 里的 DLL 文件。几乎所有的应用程序都需要用到这些共享库。

* **/lost+found**：

  这个目录一般情况下是空的，当系统非法关机后，这里就存放了一些文件。

* **/media**

  linux 系统会自动识别一些设备，例如U盘、光驱等等，当识别后，Linux 会把识别的设备挂载到这个目录下。

* **/mnt**：

  系统提供该目录是为了让用户临时挂载别的文件系统的，我们可以将光驱挂载在 /mnt/ 上，然后进入该目录就可以查看光驱里的内容了。

* **/opt**：

  opt 是 optional(可选) 的缩写，这是给主机额外安装软件所摆放的目录。比如你安装一个ORACLE数据库则就可以放到这个目录下。默认是空的。

* **/root**：

  该目录为系统管理员，也称作超级权限者的用户主目录。

* **/sbin**：

  s 就是 Super User 的意思，是 Superuser Binaries (超级用户的二进制文件) 的缩写，这里存放的是系统管理员使用的系统管理程序。

* **/tmp**：

  tmp 是 temporary(临时) 的缩写这个目录是用来存放一些临时文件的。

* **/usr**：

   usr 是 unix shared resources(共享资源) 的缩写，这是一个非常重要的目录，用户的很多应用程序和文件都放在这个目录下，类似于 windows 下的 program files 目录。

# Linux下文件路径

## 1、绝对路径

从根目录开始表示的路径，也就是从/开始

路径的写法，由根目录 **/** 写起，例如：` /usr/share/doc` 

## 2、相对路径

从当前所处的目录开始表示的路径。

路径的写法，不是由 **/** 写起，例如由` /usr/share/doc` 要到 `/usr/share/man `底下时，可以写成：` cd ../man`这就是相对路径的写法。

 . 表示当前目录

.. 表示当前目录的上一级目录

# Linux 处理目录的常用命令

## 1、ls (列出目录)

ls（英文全拼：list files）: 列出目录及文件名

* 查看指定目录下的文件信息

* 使用方法：

  * ls  --显示当前目录下文件信息
  * ls 目录或文件名  --显示指定目录下文件信息

* 选项参数：

  * -a ：全部的文件，连同隐藏文件( 开头为 . 的文件) 一起列出来(常用)

  * -d ：仅列出目录本身，而不是列出目录内的文件数据(常用)

  * -R：递归方式列出所有目录中的内容

  * -l：列出文件的详细信息, 8部分内容

    ```shell
    -rwxr-xr-x 1 root root        4553 12月 13  2019  znew
    ```

    1. 文件的类型 -（第1个字符）（共7种）：

       -： 普通文件

       d ： 目录

       l ：  符号链接，相当于windows中的快捷方式

       s ： 套接字

       p ： 管道

       b ： 块设备

       c ： 字符设备

    2. 用户的操作权限 （2 – 10个字符）

       文件所有者对文件的操作权限 （2,3,4个字符）

       文件所属组用户对文件的操作权限（5,6,7个字符）

       其他人对文件的操作权限（8,9,10个字符）

    3. 硬链接计数:

       对于目录来说, 链接计数等于该目录下所有的目录总数(含. 和 ..), 但是不包含该目录的子目录下的目录文件数量, 执行ls -la命令可以进行查看.

       对于文件来说, 指的是该文件所有的硬链接文件数量

    4. 文件所有者：  root

    5. 文件所属组：  root

    6. 文件大小：  4553 

       如果是目录: 只表示目录大小, 不包含目录中的内容, 目录大小为4k

       如果是文件：表示文件大小

    7. 文件的创建日期或最后修改时间：12月 13  2019  

    8. 文件名：znew

* 参数之间可以结合使用：

  * ls -la : 列出当前目录下所有文件的相信信息, 包括隐藏文件
  * ls -ltr: 列出当前目录下的文件, 按照时间逆向排序

## 2、cd (切换目录)

cd是Change Directory的缩写，这是用来变换工作目录的命令。

使用方法：

cd + 路径（路径可以使用相对路径或者绝对路径）

* 切换到家目录（例如: /home/srx）

  ```shell
  cd
  
  cd ~
  
  cd /home/srx/
  
  cd $HOME
  ## srx@srx-virtual-machine:/home$ cd 
  ## srx@srx-virtual-machine:~$ 
  ```

* 临近两个目录直接切换

  ```shell
  cd -
  ## srx@srx-virtual-machine:~$ cd -
  ## /home
  ## srx@srx-virtual-machine:/home$ cd -
  ## /home/srx
  ## srx@srx-virtual-machine:~$ 
  ```

## 3、pwd命令

查看用户当前所处的工作目录, printf working directory

```shell
srx@srx-virtual-machine:~/test$ pwd
## /home/srx/test
```

## 4、which命令

显示命令所在的目录, 如which ls  which cp 

```shell
srx@srx-virtual-machine:~/test$ which ls
## /usr/bin/ls
srx@srx-virtual-machine:~/test$ which cp
## /usr/bin/cp
```

## 5、touch命令

如果文件不存在, 创建新文件, 如果文件存在, 更新文件的最后修改时间。

命令使用方式：touch 文件名

## 6、mkdir命令

创建新目录, make directory

创建方式：mkdir目录名

如果创建多级目录需要添加参数 -p

```shell
srx@srx-virtual-machine:~/test$ mkdir -p mydir/a/bb/ccc
srx@srx-virtual-machine:~/test$ tree mydir/
mydir/
└── a
    └── bb
        └── ccc

3 directories, 0 files
```

## 7、rmdir命令

删除空目录，只能删除空目录，使用方式：rmdir 目录名

**-p ：**连同上一级『空的』目录也一起删除

## 8、rm命令

* 删除文件： rm 文件名

* 删除目录： rm  -r 目录名

* 参数：

  * -r：递归删除目录，删除目录必须添加此参数
  * -i：提示用户是否删除文件或目录
  * -f：强制删除

* 注意事项：

  使用rm命令删除的文件或目录不会放入回收站中，数据不易恢复。

## 9、cp 命令

命令使用方式：cp 源目录或文件 目标目录或文件

若有目录的拷贝需要使用-r参数

* cp 要拷贝的文件（file1） file（不存在）

  创建file，将file1中的内容拷贝到file

* cp file1 file（存在）

  file1覆盖file

* cp file dir（存在）

  拷贝file到dir目录

* cp -r dir（存在） dir1（存在）

  将dir目录拷贝到dir1目录中

  包括dir目录

* cp -r dir（存在） dir1（不存在）

  创建dir1

  将dir中的内容拷贝到dir1中, 不包括dir目录

* cp 拷贝目录也可以用-a参数, 这样可以保留被拷贝的文件的一些属性信息

## 10、mv命令

改名或者移动文件 mv file1 file2

* 改名

  mv file（存在） file1（不存在）

  mv dir（存在） dir1（不存在）

  mv file（存在） file2（存在）

* 移动文件(第二个参数一定是目录文件)

  * mv file（文件） dir（存在目录）

    将file文件移动到dir中

  * mv dir（目录存在） dir1（目录存在）

    将dir移动到dir1中, dir就会作为dir1的子目录而存在

* 参数：

  * -f ：force 强制的意思，如果目标文件已经存在，不会询问而直接覆盖；
  * -i ：若目标文件 (destination) 已经存在时，就会询问是否覆盖！

# Linux文件内容查看命令

## 1、cat命令

将文件内容一次性输出到终端。

使用方式： cat 文件名

缺点：终端显示的内容有限，如果文件太长无法全部显示。

可用于文件重定向: cat file1>file2, 相当于cp file1 file2

选项与参数：

* -b ：列出行号，仅针对非空白行做行号显示，空白行不标行号！
* -n ：列印出行号，连同空白行也会有行号，与 -b 的选项不同；

## 2、more命令

文件内容分页显示到终端，但是只能一直向下浏览，不能回退。

使用方式：more + 文件名

相关操作：

* 显示下一行：回车
* 显示下一页：空格
* 退出：q（ctrl + c）

## 3、less命令

文件内容分页显示到终端，可以自由上下浏览。

使用方式：less 文件名

相关操作：

* 显示下一行：回车、ctrl + p、键盘向下键
* 显示上一行：ctrl + n、键盘向上键
* 显示下一页：空格、PageDown
* 显示上一页：PageUp
* 退出：q（ctrl + c）

## 4、head命令

从文件头部开始查看前n行的内容

使用方式：head -n[行数] 文件名

如果没有指定行数，默认显示前10行内容

## 5、tail命令

从文件尾部向上查看最后n行的内容

使用方式：tail -n[行数] 文件名

如果没有指定行数，默认显示最后10行内容

一个比较重要的应用：显示日志 : tail -f test.log

一个终端tail -f test.log , 另一个终端: echo “hello world” >>test.log

## 6、wc命令

显示文件行数, 字节数, 单词数

* wc -l file显示文件的总行数

* wc -c file显示文件的总字节数

* wc -w file显示文件的总单词数

* wc file 显示文件的总行数, 单词数和总字节数

  ```shell
  srx@srx-virtual-machine:~/test$ wc test.log
  ## 5 10 60 test.log
  ```

# 软链接

软连接类似于windows下的快捷方式

## 1、如何创建软连接

* ln -s 文件名 快捷方式的名字
* 目录也可以创建软连接

## 2、创建软链接应注意事项

* ln创建软连接要用绝对路径，因为如果不使用绝对路径，一旦这个连接文件发生位置变动，就不能找到那个文件了。
* 软连接文件的大小是: 路径+文件名的总字节数

```shell
ln -s /home/srx/test/aaa.txt aaa.txt.link
## 395906 -rw-rw-r-- 1 srx srx    0 11月 11 20:09 aaa.txt
## 395907 lrwxrwxrwx 1 srx srx   22 11月 11 20:11 aaa.txt.link -> /home/srx/test/aaa.txt
```

# 硬链接

## 1、如何创建硬链接

ln 文件名 硬链接的名字

ln test.log test.log.hard

## 2、使用硬链接的注意事项

* 硬链接不能建在目录上

* 硬连接对绝对路径没有要求

* 硬连接不能跨文件系统

  **硬链接文件和源文件的inode是相同的**，文件系统的inode要求唯一，跨文件系统可能会使inode不同, 所以硬链接不能跨文件系统

* 创建一个硬链接文件, 硬链接计数+1, 删除一个则-1;

  ```shell
  ln /home/srx/test.log test.log.a
  ## 395891 -rw-rw-r-- 2 srx srx   60 11月 11 19:47 test.log         inode相同
  ## 395891 -rw-rw-r-- 2 srx srx   60 11月 11 19:47 test.log.a
  ```

## 3、硬链接的本质

* 硬连接的本质是不同的文件名所在的inode节点是相同的，相同的inode节点指向了相同的数据块，所以他们的文件内容是一样的，**文件内容会同步**。
* ls -i 文件名----------- 可以查看文件的inode
* stat 文件名 -------- 可以查看文件的inode

## 4、硬链接应用场合

* 可以起到同步文件的作用

  修改file的内容, 会在其余三个硬链接文件上同步.

* 可以起到保护文件的作用

  删除文件的时候, 只要硬链接计数不为0, 不会真正删除, 起到保护文件的作用.

# 用户权限、用户、用户组

## 1、whoami

显示当前登陆的用户名

```shell
srx@srx-virtual-machine:~/test$ whoami
## srx
```

## 2、修改文件权限chmod

linux是通过权限对文件进行控制的, 通过使用chmod命令可以修改文件相关的权限.

1. 文字设定法

   * 命令：chmod [who] [+|-|=] [mode] 文件名

   * 操作对象： [who]

     u----用户（usr）

     g----同组用户（group）

     o----其他用户（other）

     a----所有用户（all）默认

   * 操作符: [+|-|=]

     +：添加权限

     -：取消权限

     =：赋予指定权限，并取消其他权限

   * 权限：[mode] 

     r：只读权限

     w：只写权限

     x：可执行权限

   * 例：`chmod ug+wx file.log`

2. 数字设定法

   * 命令：chmod [+|-|=][mode] 文件名

   * 操作符 : [+-=]

     +：添加权限

     -：取消权限

     =：赋予指定权限，并取消其他权限

   * 数字含义：

     4：可读权限（r）

     2：可写权限（w）

     1：可执行权限（x）

     0：无权限

   * 例：`chmod 777  file.log`

## 3、修改文件所有者和所属组

1. 修改文件所有者（和所属组）chown

   * 用法：chown 文件所有者 文件名

     `sudo chown mytest file.txt`

   * 用法：chown 文件所有者:文件所属组 文件名

     `sudo chown mytest:mytest file.txt`

     `sudo chown mytest.mytest file.txt`

     `sudo chown :mytest file.txt`

     `sudo chown .mytest file.txt`

   * 注意：

     普通用户需要使用管理员用户权限执行该命令

     若系统没有其他用户, 可以使用`sudo adduser 用户名 `创建一个新用户.

2. 修改文件所属组chgrp

   * 使用方法：chgrp 用户组 文件或目录名

     `sudo chgrp mytest file.txt`

   * 普通用户需要使用管理员权限执行该命令

   

# find 命令

## 1、按文件名查询

使用参数 -name

* 命令：find  路径  -name  "文件名"
* 示例：`find /home -name "*.c"`

## 2、按类型类型查询

使用参数 -type

* 命令：find 路径 -type 类型

  type（d：目录、f：普通文件、l：链接文件、s；套接字文件、p：管道文件、b：块设备文件、c：字符设备文件）

* 示例：`srx@srx-virtual-machine:~/test$ find -type f`  查找当前目录下的普通文件

## 3、按文件大小查询

使用参数 -size

* 命令：find  路径  -size  范围

* 范围：

  大于：+ 表示---- +100k

  小于：-  表示---- -100k

  等于：无需添加符号--- 100k

  区间：-size +100k -size -500k

* 大小：

  M：必须大写

  k：必须小写

  c：表示字节数

* 示例：家目录下大于50k, 小于100k的文件:  `find ~/ -size +50k -size -100k`

## 4、按文件日期查询

* 创建日期：-ctime -n/+ n

   -n: n天以内

  +n: n天以外

* 修改日期：-mtime -n/+n

* 访问日期：-atime -n/+n

## 5、按深度

* -maxdepth n(层数）

  递归搜索目录, 搜索的层数不超过n层

* -mindepth n(层数)

  递归搜索目录,搜索的层数不能小于n层

## 6、高级查询

* 查找指定目录下所有目录，并列出目录中文件详细信息

  * `find ./ -type d -exec shell命令 {} \;`

    ` find ./ -type d -exec ls -l {} \;`

  * `find ./ -type d -ok shell命令 {} \;`

    `find ./ -type d -ok ls -l {} \;`

    注意: {}中间不能有空格

    ok比较安全, 特别是在执行rm删除文件的时候.

  * `find ./ -type d | xargs shell命令`

    `find ./ -type d | xargs ls -l`

# grep 命令

* grep -r（有目录） “查找的内容” 搜索的路径

  * -r 参数, 若是目录, 则可以递归搜索
  * -n 参数可以显示该查找内容所在的行号
  * -i 参数可以忽略大小写进行查找
  * -v 参数不显示含有某字符串

* 搜索当前目录下包含hello world字符串的文件

  * grep -r -n "hello world" ./   ------显示行号
  * grep -r -i -n "HELLO world" ./  -------忽略大小写查找

* find 和 grep命令配合使用

  先使用find命令查找文件, 然后使用grep命令查找哪些文件包含某个字符串

  `find . -name "*.log" | xargs grep -n "hello" ./`

# Linux中常用的压缩工具

## 1、gzip 和 bzip2工具

不能压缩目录，只能一个一个文件进行压缩，压缩之后会使原文件消失

* gzip *   压缩当前目录下所有的文件, 但是目录不能压缩
* gunzip *  解压当前目录下所有的.gz文件
* bzip2 *  压缩当前目录下所有的文件, 但是目录不能压缩
* bunzip2 * 解压当前目录下所有的. bz2文件

## 2、tar工具

* 相关参数说明：

  * z：用gzip来压缩/解压缩文件
  * j：用bzip2来压缩/解压缩文件
  * c：create, 创建新的压缩文件, 与x互斥使用
  * x：从压缩文件中释放文件, 与c互斥使用
  * v：详细报告tar处理的文件信息
  * f：指定压缩文件的名字
  * t:  查看压缩包中有哪些文件

* 压缩：

  * tar cvf 压缩包名字.tar 原材料[要打包压缩的文件或目录]

  * tar zcvf 压缩包名字.tar.gz 原材料[要打包压缩的文件或目录]
  * tar jcvf 压缩包名字.tar.bz2 原材料[要打包压缩的文件或目录]

* 解压缩：

  * tar  xvf  已有的压缩包（test.tar.gz）

  * tar  zxvf  已有的压缩包（test.tar.gz）

  * tar  jxvf  已有的压缩包（test.tar.bz2）

  * 解压到指定目录：添加参数 -C（大写）

    `tar zxvf test.tar.gz -C 解压目录`

* 查看压缩包里有哪些文件

  `tar -tvf test.tar`

## 3、rar工具

* 使用前需要安装 rar 工具

  `sudo apt-get install rar`

* 压缩：

  命令：rar a -r 要压缩的文件（含文件或者目录）

  `rar a -r my aa bb dir`   将aa bb dir 压缩袋my.rar

  打包生成的新文件无需指定后缀

* 解压缩：

  命令：rar x xxx.rar 解压目录（必须存在）

  `rar a -r my.rar`   将my.rar 解压到当前目录下

  `rar a -r my.rar mydir`  将my.rar解压到mydir目录下

## 4、zip工具

* 压缩：zip -r 压缩包名 要压缩的文件（含文件或目录）

  `zip -r xxx file dir`   将文件file和目录dir压缩到xxx.zip文件中

  无需指定压缩包后缀名

* 解压缩：unzip 压缩包名

  解压缩到指定目录：添加参数 -d

  `unzip xxx.zip -d /home/srx/test`

  注意：解压目录若不存在则会创建

# Linux软件安装和卸载

## 1、在线安装

* 软件安装

  `sudo apt-get install 软件名`

* 软卸载

  `sudo apt-get remove 软件名`

* 更新软件列表

  `sudo apt-get updatae

* 清理安装包

  `sudo apt-get clean`

## 2、软件包安装

在Ubuntu系统下必须有deb格式的安装包

* 软件安装

  `sudo dpkg -i xxx.deb`

* 软件卸载

  `sudo dpkg -r 软件名`
