---
typora-root-url: Image
---

# 1、会输出"hello-std-out"

```c
#include <stdio.h>
#include <unistd.h>
void test()
{
    fprintf(stdout,"hello-std-out");
    fprintf(stderr,"hello-std-err\n");
    sleep(5)
}
```

![块缓冲](/块缓冲.png)



分析：`stdout`和`stderr`是不同的设备描述符。`stdout`是块设备，`stderr`则不是。

对于块设备，只有当以下情况下才会被输入：

* 遇到回车

  ```c
  fprintf(stdout,"hello-std-out\n");
  fprintf(stderr,"hello-std-err");
  ```

  ![块缓冲1](/块缓冲1.png)

* 缓冲区满

* flush被调用

  ```c
  fprintf(stdout,"hello-std-out    ");
  flush();
  fprintf(stderr,"hello-std-err");
  ```

  ![块缓冲2](/块缓冲2.png)

# 2、Windows和Linux下的sleep函数

sleep函数功能： 执行挂起一段时间，也就是等待一段时间在继续执行

1. Windows下的Sleep函数，首字母为大写，声明在`windows.h`头文件中，其参数`usigned long`类型，为毫秒数，即`Sleep(1)`为睡眠1毫秒。

   ```c
   #include <windows.h>
   Sleep(1000);
   ```

2. Linux下的sleep函数，首字母为小写，声明在`unistd.h`头文件中，其参数为秒数，即`sleep(1)`为睡眠1秒。

   ```c
   #include <unistd.h>
   sleep(1);
   ```

# 3、逗号（,）运算符

* 使用逗号运算符是为了把几个表达式放在一起。

  `var = (count=19, incr=10, count+1);` 

  首先把 count 赋值为 19，把 incr 赋值为 10，然后把 count 加 1，最后，把最右边表达式 count+1 的计算结果 20 赋给 var。

* 整个逗号表达式的值为系列中最后一个表达式的值。

* 从本质上讲，逗号的作用是将一系列运算按顺序执行。

* 作为C语言中的**运算级别最低**的一员，逗号运算符 结合的方向是 **从左往右**的

实例1:

```c
void test()
{
    int a=5,10;       //编译语法出错
    printf("a=%d\n",a);
}
```

这个程序会得到编译出错（语法出错）。逗号表达式是没错，可是在初始化和变量申明时，逗号并不是逗号表达式的意义。

修改：

```c
void test()
{
    int a=(5,10);                 //表达式中的括号是必需的，因为逗号运算符的优先级低于赋值操作符。
    printf("a=%d\n",a);           //结果：a=10 
}
```

实例2：

```c
void test()
{
    int j=10;
    int i=(j++,j+100,j+999);
    printf("i=%d\n",j);        //i=1010 
    
    int m=10;
    int k=(m++;m+=100,m+999);
    printf("k=%d\n",k);        //k=1110 
}
```

分析：

* j 最初的值为 10，然后自增到 11，这时 j 的值就为 11，然后再计算第二个表达式 j+100，最后 j（j 为 11） 再加上 999，最后 i 为 最后一个表达式的值 999+j，及 999+ 11=1010。
* jm最初的值为 10，然后自增到 11，这时 k 的值就为 11，然后再计算第二个表达式 k+=100，最后 k（k 为 111） 再加上 999，最后 i 为 最后一个表达式的值 999+j，及 999+ 111=1110。

# 4、printf（）函数的返回值

如果成功，则返回写入的字符总数，否则返回一个负数。

```c
void test()
{
    int m=56;
    printf("%d",printf("%d",printf("%d",m)));   //结果：5621
}
```

分析：

* 执行第三个`printf()`：输出m的值56
* 执行第二个`printf()`：输出2 ------"56"的字符总数
* 执行第一个`printf()`：输出1 ------ "2"的字符总数

注意：返回值是是输出的字符数量，包括数字，字母，标点符号，空格等

```c
void test()
{
    char str[]="a,b 5,3";    //字符总数：7--两字母、两数字、两标点、一空格
    printf("% d",printf("%s",str));         //a,b 5,3 7
}
```





