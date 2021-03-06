---
typora-root-url: Image
---

# vim简介

vi是”visual interface”的简称, 它在Linux上的地位就仿佛Windows中的**记事本**一样. 它可以执行**编辑、删除、查找、替换、块操作**等众多文本操作, 而且用户可以根据自己的需要对其进行定制. vi是一个文本编辑程序, 没有菜单, 只有命令. 

vim更高级一些, 可以理解是vi的高级版本.

vim需要自行安装, 在shell中输入`vimtutor`命令可以查看相关的帮助文档.

# vim 的三种模式

Vi有三种基本工作模式: 命令模式、文本输入模式、末行模式。

三种工作模式的切换：

<img src="/模式切换.png" alt="模式切换" style="zoom:70%;" />

# vim 基本操作

## 1、命令行模式下的操作

用户按下esc键, 就可以使vi进入命令模式下; 当使用vi打开一个新文件开始也是进入命令模式下.

### 1、保存退出

| 快捷键 | 操作     |
| ------ | -------- |
| ZZ     | 保存退出 |

### 2、代码格式化

| 快捷键 | 操作         |
| ------ | ------------ |
| gg=G   | 代码个格式化 |

### 3、光标移动


| 快捷键 | 操作                      |
| ------ | ------------------------- |
| h      | 光标左移                  |
| j      | 光标下移                  |
| k      | 光标上移                  |
| l      | 光标右移                  |
| w      | 移动一个单词              |
| gg     | 光标移动到文件的开头      |
| G      | 光标移动到文件的结尾      |
| 0      | 光标移动到首行            |
| $      | 光标移动到末行            |
| nG     | 行跳转    12G，跳转至12行 |

### 4、删除命令

| 快捷键   | 操作                                                         |
| -------- | ------------------------------------------------------------ |
| x        | 删除光标后一个字符,相当于 Del                                |
| X        | 删除光标前一个字符,相当于 Backspace                          |
| dw       | 删除光标开始位置的字,包含光标所在字符                        |
| d0       | 删除光标前本行所有内容,不包含光标所在字符                    |
| D[d$]    | 删除光标后本行所有内容,包含光标所在字符                      |
| dd       | 删除光标所在行(本质其实是剪切)                               |
| ndd      | 从光标当前行向下删除指定的行数, 如15dd                       |
| v/ctrl+v | 使用h、j、k、l移动选择内容, 然后按d删除其中ctrl+v是列模式, v为非列模式 |

### 5、撤销和反撤销

| 快捷键 | 操作                                 |
| ------ | ------------------------------------ |
| u      | 一步一步撤销, 相当于word文档的ctrl+z |
| ctrl-r | 反撤销, 相当于word文档的ctrl+y       |

### 6、复制粘贴

| 快捷键   | 操作                                                         |
| -------- | ------------------------------------------------------------ |
| yy       | 复制当前行                                                   |
| nyy      | 复制n行, 如10yy                                              |
| p        | 在光标所在位置向下新开辟一行, 粘贴                           |
| P        | 在光标所在位置向下新开辟一行, 粘贴                           |
| 剪切操作 | 按dd或者ndd删除, 将删除的行保存到剪贴板中, 然后按p/P就可以粘贴了 |

### 7、可视化模式

| 快捷键   | 操作                                                         |
| -------- | ------------------------------------------------------------ |
| v/ctrl+v | 使用h、j、k、l移动选择内容;使用d删除使用y复制 使用p粘贴到光标的后面使用P粘贴到光标的前面 |

### 8、替换操作

| 快捷键 | 操作                   |
| ------ | ---------------------- |
| r      | 替换当前字符           |
| R      | 替换当前行光标后的字符 |

### 9、查找命令

| 快捷键  | 操作                                                      |
| ------- | --------------------------------------------------------- |
| /       | /xxxx, 从光标所在的位置开始搜索, 按n向下搜索, 按N向上搜索 |
| ?       | ?xxxx, 从光标所在的位置开始搜索, 按n向上搜索, 按N向下搜索 |
| #       | 将光标移动到待搜索的字符串上, 然后按n向上搜索,但N向下搜索 |
| shift+k | 在待搜索的字符串上按shift+k或者K, 可以查看相关的帮助文档  |

### 10、快速翻屏

| 快捷键   | 操作                           |
| -------- | ------------------------------ |
| ctrl + u | 向下翻半屏(up)--光标向上移动   |
| ctrl + d | 向上翻半屏(down)--光标向下移动 |
| ctrl + f | 向上翻一屏(front)              |
| ctrl + b | 向后翻一屏(back)               |

## 2、切换到文本输入模式

| 快捷键       | *操作                                                        |
| ------------ | ------------------------------------------------------------ |
| i            | 在光标前插入                                                 |
| a            | 在光标后插入                                                 |
| I            | 在光标所在行的行首插入                                       |
| A            | 在光标所在行的行尾插入                                       |
| o            | 在光标所在的行的下面新创建一行, 行首插入                     |
| O            | 在光标所在的行的上面新创建一行, 行首插                       |
| s            | 删除光标后边的字符, 从光标当前位置                           |
| S            | 删除光标所在当前行, 从行首插入                               |
| 按列模式插入 | 先按ctrl+v进入列模式, 按hjkl移动选定某列,按I或者shift+i向前插入, 然后插入字符, 最后按两次esc. |

## 3、末行模式下的操作

### 1、保存退出

| 快捷键 | 操作                     |
| ------ | ------------------------ |
| q      | 退出                     |
| q!     | 强制退出，不保存修改内容 |
| w      | 保存修改内容, 不退出     |
| wq     | 保存并退出               |
| x      | 相当于wq                 |

### 2、替换操作

| 快捷键                   | 操作                                 |
| ------------------------ | ------------------------------------ |
| :s/old/new/              | 光标所在行的第一个old替换为new       |
| :s/old/new/***\*g\****   | 光标所在行的所有old替换为new         |
| :m, ns/old/new/g         | 将第m行至第n行之间的old全部替换成new |
| :%s/old/new/g            | 当前文件的所有old替换为new           |
| :1,$s/old/new/g          | 当前文件的所有old替换为new           |
| :%s/old/new/g***\*c\**** | 同上，但是每次替换需要用户确认       |

### 3、执行shell命令

!shell命令

按下两次esc可以回到命令模式

### 4、分屏操作

* 在打开文件之后分屏:

  | 快捷键                     | 操作                                         |
  | -------------------------- | -------------------------------------------- |
  | sp                         | 当前文件水平分屏                             |
  | vsp                        | 当前文件垂直分屏                             |
  | sp 文件名                  | 当前文件和另一个文件水平分屏                 |
  | vsp 文件名                 | 当前文件和另一个文件垂直分屏                 |
  | ctrl-w-w                   | 在多个窗口切换光标                           |
  | wall/wqall/xall/qall/qall! | 保存/保存退出/保存退出/退出/强制退出分屏窗口 |

* 打开文件之前分屏：

  `vim -on file1 file2 …  `

  `垂直分屏: vim -On file1 file2… `

# vim 配置文件

## 1、用户级别配置

~/.vimrc, 修改用户级别的配置文件只会影响当前用户, 不会影响其他的用户.

例如: 在用户的家目录下的.vimrc文件中添加

set tabstop=4  ----设置缩进4个空格

set nu     ----设置行号

set shiftwidth=4  ---设置gg=G缩进4个空格, 默认是缩进8个空格

## 2、系统级别配置

/etc/vim/vimrc, 修改了系统级别的配置文件将影响系统下的所有用户.

说明: 由于linux是多用户操作系统, 建议只在用户级别的配置文件下进行修改, 不要影响其他用户.