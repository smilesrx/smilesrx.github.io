---
typora-root-url: Image
---

# 进程相关概念

## 1、程序和进程

* 程序，是指编译好的二进制文件，在磁盘上，占用磁盘空间，是一个静态概念。
* 进程，一个启动的程序，进程占用的是系统资源，如：物理内存，CPU，终端等，是一个动态的概念

## 2、并行和并发

* 并发：在一个时间段内，是在同一个cpu上同时运行多个程序。

  如：若将cpu的1s的时间分成1000个时间片，每个进程执行完一个时间片必须无条件让出cpu的使用权，这样1s中就可以执行1000个进程

  <img src="/并发.png" alt="并发" style="zoom:67%;" />

* 并行性指两个或两个以上的程序在同一时刻发生。

  <img src="/并行.png" alt="并行" style="zoom:67%;" />

## 3、PCB 进程控制块

每个进程在内核中都有一个进程控制块（PCB）来维护进程相关信息，Linux内核的进程控制块是 task_struct 结构体。

* 进程 id ：系统中的每个进程有唯一的id，在C语言用pid_t 类型表示，其实就是一个非负整数
* 进程状态：有就绪、运行、挂机、停止等状态。
* 进程切换时需要保存和恢复CPU寄存器
* 描述虚拟地址空间的信息
* 描述控制终端的信息
* 当前工作目录
* umask掩码
* 文件描述符表，包含文件指向 file结构体的指针
* 和信号相关的信息
* 用户id和组id
* 会话和进程组
* 进程可以使用的资源上限

## 4、进程状态

进程基本的状态有5种。分别为初始态，就绪态，运行态，挂起态与终止态。其中初始态为进程准备阶段，常与就绪态结合起来看 

<img src="/进程状态切换.png" alt="进程状态切换" style="zoom:67%;" />

# 创建进程

## 1、fork 函数

* 函数描述：创建子进程

* 函数原型：`pid_t fork(void);`

* 函数参数：无

* 函数返回值：

  * 成功：父进程返回子进程的PID，子进程返回0
  * 失败：返回-1，设置errno值。

* 调用fork函数的内核实现原理：

  <img src="/fork函数的内核实现原理.png" alt="fork函数的内核实现原理" style="zoom: 80%;" />

* 总结：

  * fork函数的返回值：

    父进程返回子进程的PID，是一个大于0的数、子进程返回0

    注意：不是fork函数在一个进程中返回2个值，而是在父子进程中各自返回一个值

  * 子进程创建成功后，代码的执行位置：

    父进程执行到什么位置，子进程就从哪里执行

  * 如何区分父子进程：

    通过fork 函数的返回值

  * 父子进程的执行顺序：

    不一定，那个进程先抢到CPU，那个进程先执行

* 练习：验证fork函数创建子进程

  ```c
  //fork函数测试
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  int main()
  {
  	printf("before fork, pid:[%d]\n", getpid());   //只有父进程执行此语句
  	//创建子进程
  	//pid_t fork(void);
  	pid_t pid = fork();           //子进程从此处开始执行
  	if(pid<0) //fork失败的情况
  	{
  		perror("fork error");
  		return -1;
  	}
  	else if(pid>0)//父进程
  	{
  		printf("father: [%d], pid==[%d], fpid==[%d]\n", pid, getpid(),getppid());
  		sleep(1);   //让父进程在子进程之后退出
  	}
  	else if(pid==0) //子进程
  	{
  		printf("child: pid==[%d], fpid==[%d]\n", getpid(), getppid());
  	}
  	
  	printf("after fork, pid:[%d]\n", getpid());     //父子进程都执行此语句
  
  	return 0;
  }
  ```

## 2、ps 命令和 kill 命令

* `ps aux | grep "xxx"`
* `ps ajx | grep "xxx"`
  * -a :(all)当前系统所有用户的进程
  * -u：查看进程所有者及其他一些信息
  * -x：显示没有控制端的进程  --- 不能与用户进行交互的进程【输入、输出】
  * -j：列出与作业控制相关的信息
* `kill -l`查看系统有哪些信号
* `kill -9 pid` 杀死某个线程

## 3、getpid 和 getppid函数

* getpid：获取当前进程的PID

  `pid_t getpid(void);`

* getppid:获取当前进程的父进程的PID

  `pid_t getppid(vpid);`

## 4、练习

* 编写程序，循环创建多个子进程，要求如下：

  多个子进程是兄弟关系、判断子进程是第几个进程

  注意：若让多个子进程都是兄弟进程，必须不能让子进程再去创建新的子进程

  ```c
#include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <sys/types.h>
  
  int main(int argc,char *argv[])
  {
      int i=0;
      pid_t pid;
      for(i=0;i<3;i++)
      {
          pid=fork();  //创建子进程
          if(pid<0)    //fork失败
          {
              perror("fork error");
              return -1;
          }
          if(pid==0)
          {
              break;   //防止子进程再次创建孙子进程
          }
      }
      
      if(pid==0)  //等价于 if(i<3) 第i个子进程 --- if(i==0 || i==1 || i==3)
      {
          sleep(i);
          printf("child[%]: pid==[%d] fpid[%d]\n",i,getpid(),getppid())；
      }
      if(pid>0)  //等价于 if(i==3)
      {
          sleep(i);
          printf("father: pid==[%d]\n",getpid());
      }
      return 0；
  }
  ```
  
* 编写程序，测试父子进程**不能共享全局变量**

  **重点通过这个案例讲解读时共享，写时复制**
  
  ```c
  //fork函数测试
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  int g_var = 99;   //全局变量
  int main(int argv char *argv[])
  {
      pid_t pid=fork();   //创建子进程
      if(pid<0)  //fork失败
      {
          perror("fork error");
          return -1;
      }
      if(pid>0)   //父进程
      {
          printf("father: [%d] pid==[%d] fpid==[%d]\n",pid,getpid,getppid());
          g_var++;   //父进程修改全局变量
          printf("father: g_var==[%d]\n",g_var);
          printf("[%p]\n",&g_var);   //父进程打印全局变量的地址
          sleep(3);   //让父进程在子进程之后退出
      }
      if(pid==0)   //子进程
      {
          sleep(1);   //让父进程先执行
          printf("child: pid==[%d] fpid==[%d]\n",getpid(),getppid());
          printf("child: g_var==[%d]\n",g_var);
          printf("[%p]\n",&g_var);  //变量的地址父子进程都一样
      }
      return 0;
  }
  ```

# exec 函数族

有时候需要在一个进程里面执行其他的命令或者是用户自定义的应用程序，此时就用到了exec函数族当中的函数。

使用方法一般是在父进程里面调用fork函数创建子进程，然后在子进程里面调用exec函数。

## 1、execl 函数

execl函数一般执行自己写的可执行程序

* 函数原型：`int execl(const char *path, const char *arg, ... /* (char  *) NULL */);`
* 函数参数：
  * path:要执行的程序的绝对路径
  * arg：占位，通常写应用程序的名字
  * 变参：要执行的程序的需要的参数
  * 参数写完之后：NULL
* 函数返回值：
  * 成功：不返回，不会在执行execl函数后面的代码
  * 失败：会执行execl后面的代码，可用perror打印错误的原因。

## 2、execlp 函数

execlp函数一般是执行系统自带的程序或者是命令.

* 函数原型：`int execlp(const char *file, const char *arg, .../* (char  *) NULL */);`
* 函数参数：
  * file：执行命令的名字，根据PATH环境变量来搜索该命令
  * arg：占位
  * 变参：命令的参数
  * 参数写完之后：NULL
* 函数返回值：
  * 成功：不返回，不会在执行execlp函数后面的代码
  * 失败：会执行execlp后面的代码，可用perror打印错误的原因。

## 3、exec 函数族实现原理

如：`execlp("ls","ls","l",NULL);`

<img src="/exec函数族实现原理.png" alt="exec函数族实现原理" style="zoom:75%;" />

总结：exec函数是用一个新的程序替换了当前进程的代码段、数据段、栈和堆空间；原有的进程空间没有发生变换，并没有创建新的进程，进程PID没有发生变化。

## 4、练习

* 使用execl函数执行一个用户自定义的应用程序

* 使用execlp函数执行一个linux系统命令

  ```c
  //fork函数测试
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  int main()
  {
  	//创建子进程
  	pid_t pid = fork();
  	if(pid<0) //fork失败的情况
  	{
  		perror("fork error");
  		return -1;
  	}
  	else if(pid>0)  //父进程
  	{
  		printf("father: [%d], pid==[%d]\n", pid, getpid());
  		sleep(1);  //让父进程在子进程之后退出
  	}
  	else if(pid==0)   //子进程
  	{
  		printf("child: pid==[%d], fpid==[%d]\n", getpid(), getppid());
  		//execl("/usr/bin/ls", "ls", "-l", NULL);
  		//execl("./test", "test", "hello", "world", "ni", "hao", NULL);
  		//execlp("ls", "ls", "-l", NULL);
  		execlp("./test", "TESTING", "hello", "world", "ni", "hao", NULL);
  		//execlp("./iitest", "test", "hello", "world", "ni", "hao", NULL);
  		perror("execl error");  //只有当exec函数族的函数执行失败才会执行此语句
  	}
  	return 0;
  }
  ```

# 进程回收

## 1、为什么要进行进程的回收

当一个进程退出后，进程能够回收自己的用户区的资源，但是不能回收内核空间的资源，必须由他的父进程调用wait函数或者waitpid函数完成对子进程的回收，避免造成系统资源的浪费。

## 2、孤儿进程

* 若子进程的父进程已经死掉，而子进程还活着，这个进程就成了孤儿进程

* 为了保证每个进程都有自己的一个父进程，孤儿进程会被init进程领域，init进程成为孤儿进程的养父进程，当孤儿进程退出后，有init进程完成对孤儿进程内核资源回收。

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  int main()
  {
  	//创建子进程
  	pid_t pid = fork();
  	if(pid<0) //fork失败的情况
  	{
  		perror("fork error");
  		return -1;
  	}
  	else if(pid>0)//父进程
  	{
  		sleep(5);
  		printf("father:pid==[%d], fpid==[%d]\n",getpid(),getppid());
  	}
  	else if(pid==0) //子进程
  	{
  		printf("child: pid==[%d], fpid==[%d]\n", getpid(), getppid());
  		sleep(20);   //让子进程先退出，产生孤儿进程
  		printf("child: pid==[%d], fpid==[%d]\n", getpid(), getppid());
  	}
  	return 0;
  }
  ```

  <img src="/孤儿进程 .png" alt="孤儿进程 " style="zoom: 80%;" />

## 3、僵尸进程

* 若子进程死了，父进程还活着，但是父进程没有调用wait或waitpid函数完成对子进程内核资源的回收，则该子进程就成了僵尸进程。

* 如何解决僵尸进程：

  * 由于僵尸进程是一个已经死亡的进程，所以不能使用kill命令将其杀死
  * 通过杀死其父进程后，这个僵尸进程就被init进程领养，由init进程完成对僵尸进程的回收。

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  int main()
  {
  	//创建子进程
  	pid_t pid = fork();
  	if(pid<0) //fork失败的情况
  	{
  		perror("fork error");
  		return -1;
  	}
  	else if(pid>0)//父进程
  	{
  		sleep(100);   //让父进程后退出，回收子进程的内核资源
  		printf("father: pid==[%d], fpid==[%d]\n",getpid(),getppid());
  	}
  	else if(pid==0) //子进程
  	{
  		
  		printf("child: pid==[%d], fpid==[%d]\n", getpid(), getppid());
  	}
  	return 0;
  }
  ```

  <img src="/僵尸进程.png" alt="僵尸进程" style="zoom:67%;" />

  <img src="/解决僵尸进程.png" alt="解决僵尸进程" style="zoom: 80%;" />

# 进程的回收函数

**注意：调用一次wait函数或者waitpid函数只能回收一个子进程的内核资源**

## 1、wait 函数

* 函数原型：`pid_t wait(int *status);`

* 函数作用：

  * 阻塞并等待子进程退出
  * 回收子进程的内核资源
  * 获得子进程的结束状态（退出原因）

* 函数参数 status：

  * `WIFEXITED(status)`:为非0，表示进程正常退出

    `WEXITSTATUS(status)`:获取进程的退出状态

  * `WIFSIGNALED(status)`:为非0，表示进程异常终止

    `WTERMSIG(status)`:取得进程终止的信号编码。

* 函数返回值：

  * 成功：清理掉子进程的ID
  * 失败：-1（没有子进程）

* 练习：

  ```c
  //父进程调用wait函数完成对子进程的回收
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/wait.h>
  int main()
  {
  	//创建子进程
  	pid_t pid = fork();
  	if(pid<0) //fork失败的情况
  	{
  		perror("fork error");
  		return -1;
  	}
  	else if(pid>0)//父进程
  	{
  		printf("father: [%d], pid==[%d], fpid==[%d]\n", pid, getpid(),getppid());
  		int status;
  		pid_t wpid = wait(&status);
  		printf("wpid==[%d]\n", wpid);
  		if(WIFEXITED(status)) //正常退出
  		{
  			printf("child normal exit, status==[%d]\n", WEXITSTATUS(status));
  		}
  		else if(WIFSIGNALED(status)) //被信号杀死
  		{
  			printf("child killed by signal, signo==[%d]\n", WTERMSIG(status));
  		}
  	}
  	else if(pid==0) //子进程
  	{
  		printf("child: pid==[%d], fpid==[%d]\n", getpid(), getppid());
  		sleep(20);
  		return 2;
  	}
  	return 0;
  }
  ```

  子进程正常退出

  ![子进程的正常退出状态](/子进程的正常退出状态.png)

  子进程被信号杀死

  ![子进程被信号杀死状态](/子进程被信号杀死状态.png)

## 2、waitpid 函数

* 函数原型：`pid_t waitpid(pid_t pid,int *status,int options);`

* 函数参数：

  * **`pid=-1`:等待任意子进程。与wait等效**

  * **`pid>0`:等待与pid相等的子进程**

  * `pid=0`:等待进程组ID与目前进程相同的任何子进程，也就是说任何和调用waitpid()函数的进程在同一个进程组的进程。

  * `pid<-1`:等待其组ID等于pid的绝对值的任一子进程。(适用于子进程在其他组的情况)

  * status:子进程的退出状态，同wait函数

  * options：设置为WNOHANG，表示函数非阻塞，设置为0，函数阻塞。

* 函数返回值：

  * 大于0：返回回收掉的子进程的ID
  * 等于-1：无子进程
  * 等于0：参3options为WNOHANG，且子进程正在运行

* 练习：

  ```c
  //父进程调用waitpid函数完成对子进程的回收
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/wait.h>
  int main()
  {
  	//创建子进程
  	pid_t pid = fork();
  	if(pid<0) //fork失败的情况
  	{
  		perror("fork error");
  		return -1;
  	}
  	else if(pid>0)//父进程
  	{
  		printf("father: [%d], pid==[%d], fpid==[%d]\n", pid, getpid(),getppid());
  		int status;
           sleep(15);
  		pid_t wpid = waitpid(-1,&status,WNOHANG); //-1表示等待任意子进程，WNOHANG表示不阻塞
  		printf("wpid==[%d]\n", wpid);
  		if(WIFEXITED(status)) //正常退出
  		{
  			printf("child normal exit, status==[%d]\n", WEXITSTATUS(status));
  		}
  		else if(WIFSIGNALED(status)) //被信号杀死
  		{
  			printf("child killed by signal, signo==[%d]\n", WTERMSIG(status));
  		}
  	}
  	else if(pid==0) //子进程
  	{
  		printf("child: pid==[%d], fpid==[%d]\n", getpid(), getppid());
  		sleep(20);
  		return 2;
  	}
  	return 0;
  }
  ```

# 父子进程之间共享文件

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	int fd = open("./test.log", O_RDWR | O_CREAT, 0777);
	if(fd<0)
	{
        perror("open error");
        return -1;
	}
	pid_t pid = fork();   //创建子进程
	if(pid<0)
	{
        perror("fork error");
        return -1;
	}
	else if(pid>0)    //父进程
	{
        printf("father: fpid==[%d], cpid==[%d]\n", getpid(), pid);
        write(fd, "hello world", strlen("hello world"));
        close(fd);
        wait(NULL);
	}
	else if(pid==0) //子进程
	{
        sleep(1);  //让父进程先执行
        printf("child: fpid==[%d], cpid==[%d]\n", getppid(), getpid());
        char buf[255];
        memset(buf, 0x00, sizeof(buf));
        lseek(fd, 0, SEEK_SET);
        int n = read(fd, buf, sizeof(buf));
        printf("read over, n==[%d], buf==[%s]\n", n, buf);
        close(fd);
	}
	return 0;
}
```

# 练习

父进程fork三个子进程：

* 其中一个调用 ps 命令
* 一个调用自定义的应用程序
* 一个调用会出现段错误的程序

父进程回收三个子进程（waitpid），并且打印三个子进程的退出状态

段错误：

* 访问了非法内存
* 访问了不可写的区域进行操作
* 占空间溢出

```c
//调用fork函数创建子进程，并完成对子进程的回收
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
	int i=0;
	int n=3;
	pid_t pid;
	for(i=0;i<n;i++)
	{
		pid=fork();   //创建子进程
		if(pid<0)   //fork失败
		{
			perror("fork error");
			return -1;
		}
		if(pid==0)  //子进程
		{
			break;   //防止子进程创建孙子进程
		}
	}
	
	//父进程
	if(i=n)
	{
		printf("father: [%d] pid==[%d]\n",pid,getpid());
		
		pid_t wpid;
		int status;
		while(1)
		{
			wpid=waitpid(-1,&status,WNOHANG);
			if(wpid==0)  //有子进程还在运行
			{
				continue;
			}
			else if(wpid==-1)  //已经没有子进程退出
			{
				printf("no child is living ,wpid==[%d]\n",wpid);
				exit(0);
			}
			else if(wpid>0)  //有子进程退出
			{
				if(WIFEXITED(status))  //子进程正常退出
				{
					printf("child:[%d] normal exit,status==[%d]\n",wpid,WEXITSTATUS(status));
				}
				else if(WIFSIGNALED(status))   //被信号杀死
				{
					printf("child:[%d] killed by signo==[%d]\n",wpid,WTERMSIG(status));
				}
			}
		}
	}

	//第一个子进程
	if(i==0)
	{
		printf("child:[%d] pid==[%d]  fpid==[%d]\n",i,getpid(),getppid());
		execlp("ls","ls","-l",NULL);
		perror("execlp error");
		exit(1);
	}

	//第二个子进程
	if(i==1)
	{
		printf("child:[%d] pid==[%d]  fpid==[%d]\n",i,getpid(),getppid());
		execl("./hello","hello","ni","hao",NULL);
		perror("execl error");
		return -1;
	}

	//第三个子进程
	if(i==2)
	{
		printf("child:[%d] pid==[%d]  fpid==[%d]\n",i,getpid(),getppid());
		execl("./test","test",NULL);
		perror("execl error");
		return -1;
	}
 
	return 0;
}
```

# 守护进程

## 1、守护进程介绍

Daemon（精灵）进程，是Linux中的后台服务程序，通常独立于控制端并且周期性的执行某任务或等待处理某些发生的事件。一般采用以d结尾的名字，如vsftpd

Linux后台的一些系统服务进程，没有控制终端，不能直接和用户交换。不受用户登录、注销的影响，一直在运行，他们都是守护进程。如：预读入缓输出机制的实现；ftp服务器；nfs服务器。

总结守护进程的特点：

* Linux后台服务进程
* 独立于控制端
* 周期性的执行某种任务
* 不受用户登录和注销的影响
* 一般采用以d结尾的名字

## 2、进程组和会话组

* 进程组

  * 进程组是一个或多个进程的集合，每个进程都属于一个进程组，引入进程组是为了简化对进程的管理。当父进程创建子进程的时候，默认子进程与父进程属于同一进程组。

    进程组 ID==第一个进程ID（组长进程）。如父进程创建了多个子进程，父进程和多个子进程同属于一个组，而由于父进程是进程组里的第一个进程，所以父进程就是这个组的组长，组长ID==父进程ID。

  * 可以使用 `kill -SIGKILL -进程组ID（负的）`来将整个进程组内的进程全部杀死。

  * 只要进程组中有一个进程存在，进程组就存在，与组长进程是否终止无关。

  * 进程组生存期：从进程组创建后到最后一个进程退出。

* 会话组

  * 一个会话是一个或多个进程组的集合
  * **创建会话的进程不能是进程组组长**
  * 创建会话的进程成为一个进程组的组长进程，同时也成为会话的会长
  * 需要有 root 权限
  * 新创建的会话丢弃原有的控制终端
  * 建立新会话时，先调用 fork 函数，父进程终止，子进程调用 setsid 函数

* 可以使用 `ps -ajx`来查看进程组 ID 和会话组 ID

  * 可以 fork 出几个子进程，然后查看进程组的 ID 和会话组 ID

* 进程组合会话组的关系图：

  <img src="/进程组合会话组的关系.png" alt="进程组合会话组的关系" style="zoom:67%;" />

## 3、创建守护进程的模型

* 第一步：fork 子进程，父进程退出

  子进程继承了父进程的进程组 ID ，但具有一个新的进程ID，这样就保证了进程不是一个进程组的组长 ID，这对于下面要做的 setsid 函数的调用时必要的前提条件。

* 第二步：子进程调用 setsid 函数创建会话

  * 该进程成为新会话的首进程，是会话的会长
  * 成为一个新进程组的组长进程，是进程组组长
  * 不受控制终端的影响

* 第三步：改变当前工作目录 chdir

  如：a.out 在 U 盘上，启动这个程序，这个程序的当前的工作目录就是这个 U盘，如果 U盘拔掉后进程的当前工作目录将消失，a.out 将不能正常工作。

* 第四步：重设文件掩码 `mode & ~umask`

  * 子进程会继承父进程的掩码
  * 增加了子进程程序操作的灵活性
  * `umask(0000)`;

* 第五步：关闭文件描述符

  * 守护进程不受控制终端的影响所以可以关闭，以释放资源
  * `close(STDIN_FILENO);` `close(STDOUT_FILENO);` `close(STDERR_FILENO);`

* 第六步：执行核心工作

  守护进程的核心代码逻辑

## 4、练习

编写一个守护进程，每隔2S钟获取一次系统时间，并将这个时间写入磁盘文件。

分析：

* 每隔两秒：使用setitimer 函数设置定时器，发送SIGALRM信号
* 信号操作：注册信号（signal 或者 sigaction），还有一个信号处理函数
* 获取系统时间：time 函数、ctime函数
* 写入磁盘文件：文件操作 open、write、close

优化：

* 不频繁打开关闭文件
* 控制 log文件的大小

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syt/time.h>

//全局变量
int falg = 0;  //文件打开标志
int num = 1;   //文件名下标
char *File = "mytemon.log";
char *File_Name[64];   //文件名
int fd;   //文件描述符
int size;   //文件大小

//信号处理函数
void myfunc(int signo)
{
    sprintf(File_name, %s%d, File, num);
    if(flag==0)
    {
        fd = open(File_Name, O_RDWR | O_CREAT | O_APPEND, 0775);
        if(fd<0)   //文件打开失败
            return;
        else    //文件打开成功
            falg = 1;  //关闭文件打开标志
    }
    
    //获取系统时间
    time_t t;
    time(&t);
    char *p = ctime(&t);
    
    //写入文件
    write(fd, p, strlen(p));
    
    //获取文件大小
    size = lseek(fd, 0, SEEK_END);
    if(size>2048)   //限制文件的大小
    {
        close(fd);   //关闭文件
        num++;     //文件名下标加1
        flag = 0;    //打开文件标志
    }
}

int main(int argc, char *argv[])
{
    //创建子进程
    pid_t pid = fork();
    if(pid<0 || pid>0)
    {
        exit(1);   //父进程退出
    }
    
    //子进程
    setsid();
    
    //改变当前工作目录
    chdir("~/srx/log");
    
    //修改文件掩码
    umask(0000);
    
    //关闭标准输入、标准输出、标准错误文件
    close(STDIN_FILENO);
    clsoe(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    //核心操作
    //注册信号处理函数
    struct sigaction act;
    act.sa_handler = myfun;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigation(SIGALRM, &act, NULL);
    
    //设置定时器
    struct itimerval tm;
    tm.it_interval.tv_sec = 2;
    tm.it_interval.tv_usec = 0;
    tm.it_value.tv_sec = 3;
    tm.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &tm, NULL);
    
    while(1)
    {
        sleep(1);
    }
    
    return 0；
}
```











