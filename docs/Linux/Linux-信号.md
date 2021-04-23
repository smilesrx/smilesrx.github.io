---
typora-root-url: Image
---

# 信号介绍

## 1、信号的概念

信号是信息的载体，Linux/UNIX环境下主要的通信手段

软中断信号（signal，简称为信号）用来通知进程发生了**异步事件**。进程之间可以互相通过系统调用 kill 发送软中断信号。内核也可以因为内部事件而给进程发送信号，通知进程发生了某个事件。

信号的实现手段导致信号有很强的延时性，但对于用户来说，时间非常短，不易察觉。

注意，信号只是用来通知某进程发生了什么事件，并不给该进程传递任何数据。

## 2、信号的处理方式

收到信号的进程对各种信号有不同的处理方式：分三类

* 第一种：**捕捉信号**（调用用户自定义的函数）
* 第二种：**忽略信号**，对该信号丢弃不做任何处理，就像未发生过一样。
* 第三种：执行信号的**默认处理方式**

## 3、信号的机制

进程A给进程B发生信号，进程B收到信号之前执行自己的代码，收到信号后，不管执行到程序的什么位置，都要暂停运行，去处理信号，处理完后再继续执行。与硬件中断类似（异步模式）但信号是在软件层面上属于“软中断”。

每个进程收到的所有信号，都由内核负责发送。

![信号机制](/信号机制.png)

## 4、信号的状态

信号有三种状态：产生、未决、递达

* 信号的产生
  * 按键产生，如：Ctrl+C、Ctrl+Z、Ctrl+\
  * 系统调用产生：如：kill、raise、abort
  * 软件条件产生：如：定时器alarm
  * 硬件异常产生：如：非法访问内存（段错误）、除0（浮点数除外）、内存对齐出错
  * 命令产生：如：kill命令
* 未决：产生和递达之间的状态。主要由于阻塞（屏蔽）导致该状态。
* 递达：递达并且到达进程

## 5、阻塞信号集和未决信号集

Linux内核的进程控制块PCB是一个结构体，task_struct, 除了包含进程id，状态，工作目录，用户id，组id，文件描述符表，还包含了信号相关的信息，主要指**阻塞信号集和未决信号集**。

* 阻塞信号集中保存的都是被当前进程阻塞的信号，若当前进程收到的是阻塞信号集中的某个信号，这些信号需要暂时被阻塞，不予处理
* 信号产生后由于某些原因（主要是阻塞）不能递达，这类信号的集合称之为未决信号集。在屏蔽接触前，信号一直处于未决状态；若是信号从阻塞信号集中解除阻塞，则该信号会被处理，并从未决信号集中去除

## 6、信号四要素

通过 `man 7 signal` 可以查看信号相关信息

* 信号的编号：使用`kill -l`命令可以查看当前系统有哪些信号，不存在编号为0的信号。其中1-31号信号称之为常规信号（也叫普通信号或标准信号），34-64称之为实时信号，驱动编程与硬件相关。
* 信号的名称
* 产生信号的事件
* 信号的默认处理动作：
  * Term：终止进程
  * Ign：忽略信号
  * Core：终止进程，生成Core文件。（查验死亡原因，用于gdb调试）
  * Stop：停止（暂停）进程
  * Cont：继续运行进程

注意：The signals SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.

常用信号的默认处理方式：

| 信号编号 | 信号名称 |                      含义                      | 默认处理方式 |
| :------: | :------: | :--------------------------------------------: | :----------: |
|    2     |  SIGINT  |    该信号在用户键入INTR字符（Ctrl-C）时发出    |   终止进程   |
|    3     | SIGQUIT  |            由QUIT字符（Ctrl-\）发出            |   终止进程   |
|    9     | SIGKILL  |  用来结束进程的运行，并且不能阻塞、处理和忽略  |   终止进程   |
|    14    | SIGALRM  |           该信号当一个定时器到时发出           |   终止进程   |
|    19    | SIGSTOP  |     暂停一个进程，且不能被阻塞、处理或忽略     |   暂停进程   |
|    20    | SIGTSTP  | 暂停交互进程，用户可键入SUSP字符（Ctrl-Z）发出 |   暂停进程   |
|    17    | SIGCHLD  |     子进程改变状态时，父进程会收到这个信号     |     忽略     |
|    6     | SIGABRT  |               该信号用于结束进程               |     终止     |

# 信号相关函数

## 1、signal 函数

* 函数作用：注册信号捕捉函数

* 函数原型：

  `typedef void(*sighandler_t)(int);`  

  `sighandler_t signal(int signum,sighandler_t handler);`

* 函数参数：

  * signum：信号的编号
  * handler：信号的处理函数

* signal函数测试：注册信号处理函数

  ```c
  //signal函数测试---注册信号处理函数
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <signal.h>
  
  //信号处理函数
  void sighandler(int signo)
  {
  	printf("signo==[%d]\n", signo);
  }
  
  int main()
  {
  	//注册信号处理函数
  	signal(SIGINT, sighandler);
  	sleep(10);
  
  	return 0;
  }
  ```

## 2、kill 函数 / 命令

* 描述：给指定进程发生指定信号

* kill 命令：`kill -SIGKILL 进程PID`

* kill 函数原型：`int kill(pid_t pid int sig);`

* 函数参数：

  * pid：

    pid>0：发送信号给指定进程

    pid=0：发送信号给与调用kill函数进程属于同一进程组的所有进程。

    pid<-1：取|pid|发给对应进程组。

    pid=-1：发送给进程有权限发送的系统中所有进程。

  * sig信号参数：信号宏名（不推荐信号的编号）

* 函数返回值：

  * 成功：0
  * 失败：-1，并设置error值

* 进程组：每个进程都属于一个进程组，进程组是一个或多个进程集合，他们相互关联，共同完成一个实体任务，每个进程组都有一个进程组长，默认进程组ID与进程组长ID相同。

* kill 函数测试：杀死自己

  ```c
  //signal函数测试---注册信号处理函数
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <signal.h>
  
  //信号处理函数
  void sighandler(int signo)
  {
  	printf("signo==[%d]\n", signo);
  }
  
  int main()
  {
  	//注册信号处理函数
  	signal(SIGINT, sighandler);
  	sleep(3);
  	kill(getpid(), SIGINT);
  
  	return 0;
  }
  ```
  
* kill 函数测试: 父进程杀死子进程或者子进程杀死父进程

  ```c
  //kill函数测试: 父进程杀死子进程或者子进程杀死父进程
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <errno.h>
  #include <signal.h>
  
  int main(int argc, char *argv[])
  {
  	pid_t pid;
  	int i = 0;
      int n = 2;
  	for(i=0; i<n; i++)
  	{
  		pid = fork();
  		if(pid<0)    //fork失败
  		{
  			perror("fork error");
  			return -1;
  		}
  		else if(pid>0)   //父进程
          {	
  			//父进程杀死第一个子进程
  			/*if(i==0)
  			{
  				kill(pid, SIGKILL);
  			}*/
  		}
  		else if(pid==0)   //子进程
  		{
  			break;  //防止子进程创建孙子进程
  		}
  	}
  
      if(i==0)   //第一个子进程
      {
          sleep(i);
      	printf("the first child, pid==[%d]\n", getpid());
  		//子进程杀死父进程
  		//kill(getppid(), SIGKILL);
      }
      if(i==1)
      {
          sleep(i);
      	printf("the second child, pid==[%d]\n", getpid());
  		//杀死同一组的所有进程
  		kill(-1, SIGKILL);	
      }
      if(i==2)
      {
          sleep(i);
      	printf("the father, pid==[%d]\n", getpid());
      }
  	return 0;
  }
  ```

## 3、raise 函数

* 函数描述：给当前进程发送指定信号（自己给自己发）

* 函数原型：`int raise(int sig);`

* 函数参数：sig：信号的宏名

* 函数返回值：成功：0、失败非0

* 函数拓展：`raise(signo)==kill(getpid(),signo);`

* 测试：自己杀死自己

  ```c
  //raise和abort函数测-
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <signal.h>
  
  //信号处理函数
  void sighandler(int signo)
  {
  	printf("signo==[%d]\n", signo);
  }
  
  int main()
  {
  	//注册信号处理函数
  	signal(SIGINT, sighandler);
  
  	//给当前进程发送SIGINT信号
      sleep(3);
  	raise(SIGINT);
  	return 0;
  }
  ```

## 4、abort 函数

* 函数描述：给自己发送异常终止信号 （6）SIGABRT，并产生core文件

* 函数原型：`void abort(void);`

* 函数拓展：`abort()==kill(getpid(),SIGABRT);`

* 测试：自己给自己发SIGABRT信号

  ```c
  //raise和abort函数测-
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <signal.h>
  
  //信号处理函数
  void sighandler(int signo)
  {
  	printf("signo==[%d]\n", signo);
  }
  
  int main()
  {
  	//注册信号处理函数
  	signal(SIGABRT, sighandler);
  	//给当前进程发送SIGABRT
  	abort();
  
  	return 0;
  }
  ```

## 5、alarm 函数

alarm使用的是自然定时法，与进程状态无关。就绪、运行、挂起（阻塞、暂停）、终止、僵尸、何种状态alarm都计时。

* 函数描述：设置定时器（闹钟）。在指定seconds后，内核会给当前进程发送（14）SIGALRM信号。进程收到该信号，默认动作终止。每个进程都有且只有唯一的一个定时器。

* 函数原型：`unsigned int alarm(unsigned int seconds);`

* 函数返回值：返回0或剩余的秒数，无失败。

* 常用操作：取消定时器`alarm(0);`,返回旧闹钟余下的秒数。

* alarm 函数测试

  ```c
  //signal函数测试---注册信号处理函数
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <signal.h>
  
  //信号处理函数
  void sighandler(int signo)
  {
  	printf("signo==[%d]\n",signo);
  }
  
  int main(int argc,char *argv[])
  {
  	//注册信号处理函数
  	//signal(SIGINT,sighandler);
      signal(SIGALRM,sighandler);
  	
      int n=alarm(5);  //设置5s定时器
      printf("first:n==[%d]\n",n);
  
      sleep(3);
      n=alarm(3);   //覆盖
      printf("second:n==[%d]\n",n);
  
      //sleep(3);
      //n=alarm(0);  //取消定时器 
      //printf("third:n==[%d]\n",n);
  
      sleep(10);
  
  	return 0;
  }
  ```

* alarm 函数测试：1秒钟电脑执行i++的次数

  ```c
  //测试1秒钟可以数多少数字
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <signal.h>
  
  int main()
  {
  	alarm(1);
  	int i = 0;
  	while(1)
  	{
  		printf("[%d]", i++);
  	}
  	return 0;
  }
  ```

  实际执行时间 = 系统时间 + 用户时间 + 损耗时间

  损耗时间= 实际执行时间-(系统时间 + 用户时间 )

  * 每一个数字都直接打印:`time ./alarm_count`：94975次

    real	 0m1.001s
    user	0m0.029s
    sys	  0m0.210s

    损耗时间= 1.001-（0.210+0.029）=0.762

  * 输出重定向到文件后：`time ./alarm_count >test.log`：12856104次

    real	 0m1.001s
    user	0m0.846s
    sys	  0m0.152s

    损耗时间= 1.001-（0.152+0.846）=0.003

  原因是：调用printf函数打印数字遇到\n才会打印, 打印过程涉及到从用户区到内核区的切换, 切换次数越多消耗的时间越长, 效率越低;而使用文件重定向, 由于文件操作是带缓冲的, 所以涉及到用户区到内核区的切换次数大大减少,从而使损耗降低.

## 6、setitimer 函数

* 函数描述：设置定时器（闹钟），可代替alarm函数，精度微妙（us），可实现周期定时。

* 函数原型：`int setitimer(int which,const struct itimerval *new_value,struct itimerval *old_value);`

* 函数参数：

  * which：指定定时方式

    * 自然定时：ITIMER_REAL   （14）SIGALRM计算自然时间
    * 虚拟空间计时（用户空间）：ITMER_VIRTUAL （26）SIGVTALRM只计算进程占用cpu的时间
    * 运行时计时（用户+内核）：ITIMER_PROT  （27）SIGPROT计算占用cpu及执行系统调用的时间

  * new_value：struct itimerval, 负责设定timeout时间

    * `itimerval.it_value`: 设定第一次执行function所延迟的秒数 

    * `itimerval.it_interval`: 设定以后每几秒执行function

    * ```c
      struct itimerval { 
          struct timerval it_interval; // 闹钟触发周期
          struct timerval it_value;    // 闹钟触发时间
        }; 
        struct timeval { 
          long tv_sec; 			// 秒
          long tv_usec; 			// 微秒
       }      
      ```

  * old_value：存放旧的timeout值，一般指定为NULL

* 函数返回值：

  * 成功：0
  * 失败：-1，并设置error值

* 练习：使用setitimer实现每个1秒打印一次 hello world。

  ```c
  //setitimer函数测试
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <signal.h>
  #include <sys/time.h>
  
  //SIGALRM信号处理函数
  void sighandler(int signo)
  {
  	//printf("signo==[%d]\n", signo);
      printf("hello world\n");
  }
  
  int main()
  {
  	//int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);
  
  	//注册信号SIGALRM信号处理函数
  	signal(SIGALRM, sighandler);
  
  	struct itimerval tm;
  	//周期性时间赋值
  	tm.it_interval.tv_sec = 1;
  	tm.it_interval.tv_usec = 0;
  	
  	//第一次触发的时间
  	tm.it_value.tv_sec = 3;
  	tm.it_value.tv_usec = 0;
  	
  	setitimer(ITIMER_REAL, &tm, NULL);
  
  	while(1)
  	{
  		sleep(1);
  	}
  
  	return 0;
  }
  ```

# 信号集

## 1、未决信号集合阻塞信号集的关系

阻塞信号集是当前进程要阻塞的信号的集合，未决信号集是当前进程中还处于未决状态的信号的集合，这两个集合存储在内核的PCB中。

下面以SIGINT为例说明信号未决信号集和阻塞信号集的关系：

当进程收到一个SIGINT信号（信号编号为2），首先这个信号会保存在未决信号集合中，此时对应的2号编号的这个位置上置为1，表示处于未决状态；在这个信号需要被处理之前首先要在阻塞信号集中的编号为2的位置上去检查该值是否为1：

* 如果为1，表示SIGNIT信号被当前进程阻塞了，这个信号暂时不被处理，所以未决信号集上该位置上的值保持为1，表示该信号处于未决状态；
* 如果为0，表示SIGINT信号没有被当前进程阻塞，这个信号需要被处理，内核会对SIGINT信号进行处理（执行默认动作，忽略或者执行用户自定义的信号处理函数），并将未决信号集中编号为2的位置上将1变为0，表示该信号已经处理了，这个时间非常短暂，用户感知不到。

当SIGINT信号从阻塞信号集中解除阻塞之后，该信号就会被处理。

<img src="/信号集.png" alt="信号集" style="zoom:75%;" />

## 2、信号集相关函数

由于信号集属于内核的一块区域，用户不能直接操作内核空间，为此，内核提供了一些信号集相关的接口函数，使用这些函数用户就可以完成对信号集的相关操作。

信号集是一个能表示多个信号的数据类型，sigset_t set，set即一个信号集。既然是一个集合，就需要对集进行添加、删除等操作。

* `int sigemptyset(sigset_t *set);`

  * 函数说明：将某个信号集清空
  * 函数返回值：成功 0；失败 -1，设置errno

* `int sigfillset(sigset_t *set);`

  * 函数说明：将某个信号集置1
  * 函数返回值：成功 0；失败 -1，设置errno

* `int sigaddset(sigset_t *set,int signum);`

  * 函数说明：将某个信号加入到信号集中
  * 函数返回值：成功 0；失败 -1，设置errno

* `int sigdelset(sigset_t *set,int signum);`

  * 函数说明：将某个信号从信号集中删除
  * 函数返回值：成功 0；失败 -1，设置errno

* `int sigismember(sigset_t *set,int signum);`

  * 函数说明：判断某个信号是否在信号集中
  * 函数返回值：在 1；不在 0；出错 -1，设置errno

* `int sigprocmask(int how,const sigset_t *set,sigset_t *oldset);`

  * 函数说明：用来屏蔽信号、解除屏蔽

  * 函数参数：

    * how参数取值：假设当前的信号屏蔽字为mask

      SIG_BLOCK:当how设置为此值，set表示需要屏蔽的信号。mask = mask|set

      SIG_UNBLOCK: set表示需要解除屏蔽的信号。相当于 mask = mask & ~set

      SIG_SETMASK: set表示用于替代原始屏蔽及的新屏蔽集。相当于mask = set若，调用sigprocmask解除了对当前若干个信号的阻塞，则在sigprocmask返回前，至少将其中一个信号递达。

    * set：传入参数，是一个自定义信号集合。由参数how来指示如何修改当前信号屏蔽字。

    * oldset：传出参数，保存旧的信号屏蔽字。

  * 函数返回值：成功 0；失败 -1，设置errno

* `int sigpending(sigset_t *set);`

  * 函数说明：读取当前进程的未决信号集
  * 函数参数：set 传出参数
  * 函数返回值：成功 0；失败 -1，设置errno

## 3、练习

设置阻塞信号集并把所有常规信号的未决状态打印至屏幕。

```c
//信号集相关函数测试
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

//信号处理函数
void sighandler(int signo)
{
	printf("signo==[%d]\n", signo);
}

int main()
{

	//注册信号处理函数
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);

	//定义信号集变量
	sigset_t set;
	sigset_t oldset;

	//初始化信号集
	sigemptyset(&set);
	sigemptyset(&oldset);

	//将SIGINT SIGQUIT加入到set集合中
	sigaddset(&set, SIGINT);	
	sigaddset(&set, SIGQUIT);	

	//将set集合中的SIGINT SIGQUIT信号加入到阻塞信号集中
	//sigprocmask(SIG_BLOCK, &set, NULL);
	sigprocmask(SIG_BLOCK, &set, &oldset);

	int i=0;
	int j=1;
	sigset_t pend;
	
	while(1)
	{
		//获取未决信号集
		sigemptyset(&pend);
		sigpending(&pend);

		for(i=1; i<32; i++)
		{
			if(sigismember(&pend, i)==1)
			{
				printf("1");
			}
			else
			{
				 printf("0");
			}
		}	
		printf("\n");

		//循环10次解除对SIGINT SIGQUIT信号的阻塞
		if(j++%10==0)
		{
			//sigprocmask(SIG_UNBLOCK, &set, NULL);	
			sigprocmask(SIG_SETMASK, &oldset, NULL);
		}
		else
		{
			sigprocmask(SIG_BLOCK, &set, NULL);
		}

		sleep(1);
	}		

	return 0;
}
```

# 信号捕捉函数

SIGKILL 和 SIGSTOP 信号不能被捕获。

## 1、signal 函数

* 函数作用：注册信号捕捉函数

* 函数原型：

  `typedef void(*sighandler_t)(int);`  

  `sighandler_t signal(int signum,sighandler_t handler);`

* 函数参数：

  * signum：信号的编号
  * handler：信号的处理函数

## 2、sigaction 函数

* 函数说明：注册一个信号处理函数

* 函数原型：`int sigactoin(int signum,const struct sigaction *act,struct sigaction *oldact);`

* 函数参数：

  * signum：捕捉的信号
  * act：传入参数，新的处理方式
  * oldact：传出参数，旧的处理方式

  ```c
  struct sigaction {
         void  (*sa_handler)(int);	// 信号处理函数
         void  (*sa_sigaction)(int, siginfo_t *, void *); //信号处理函数
         sigset_t  sa_mask; //信号处理函数执行期间需要阻塞的信号
         int      sa_flags; //通常为0，表示使用默认标识
         void     (*sa_restorer)(void);
  };
  ```

* 总结：

  * `sa_handler`：指定信号捕捉后的处理函数名(即注册函数)。也可赋值为SIG_IGN表忽略 或 SIG_DFL表执行默认动作
  * `sa_mask`: 用来指定在信号处理函数执行期间需要被屏蔽的信号，特别是当某个信号被处理时，它自身会被自动放入进程的信号掩码，因此在信号处理函数执行期间这个信号不会再度发生。注意：仅在处理函数被调用期间屏蔽生效，是临时性设置。
  * `sa_flags`：通常设置为0，使用默认属性。
  * `sa_restorer`：已不再使用
  
* 练习：使用sigaction 函数注册信号捕捉函数，并验证信号是否支持排队

  ```c
  //sigaction函数测试---注册信号处理函数
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <signal.h>
  
  //信号处理函数
  void sighandler(int signo)
  {
  	printf("signo==[%d]\n", signo);
  	sleep(5);     //在信号处理函数期间产生多个信号，验证信号不支持排队
  }
  
  int main()
  {
  	//注册信号处理函数
  	struct sigaction act;
  	act.sa_handler = sighandler;
  	sigemptyset(&act.sa_mask);  //在信号处理函数执行期间, 不阻塞任何信号
  	sigaddset(&act.sa_mask, SIGQUIT);   //在信号处理期间，阻塞SIGQUIT信号
  	act.sa_flags = 0;
  	sigaction(SIGINT, &act, NULL);
  	signal(SIGQUIT, sighandler);	
  	while(1)
  	{
  		sleep(10);
  	}
  
  	return 0;
  }
  ```

  结论：信号处理不支持排队

  * 在XXX信号处理函数执行期间, XXX信号是被阻塞的, 如果该信号产生了多次, 在XXX信号处理函数结束之后,  该XXX信号只被处理一次。
  * 在XXX信号处理函数执行期间,如果阻塞了YYY信号, 若YYY信号产生了多次, 当XXX信号处理函数结束后, YYY信号只会被处理一次。

## 3、内核实现信号捕捉的过程

如果信号的处理动作是用户自定义函数，在信号递达时就调用这个函数，这称为捕捉信号。由于信号处理函数的代码是在用户空间的，处理过程比较复杂，举例如下：

* 用户程序注册了SIGQUIT信号的处理函数sighandler。
* 当前正在执行main函数，这时发生中断或异常切换到内核态。
* 在中断处理完毕后要返回用户态的main函数之前检查到有信号SIGQUIT递达。
* 内核决定返回用户态后不是恢复main函数的上下文继续执行，而是执行sighandler函数，sighandler和main函数使用不同的堆栈空间，它们之间不存在调用和被调用的关系，是两个独立的控制流程。
* sighandler函数返回后自动执行特殊的系统调用sigreturn再次进入内核态。
* 如果没有新的信号要递达，这次再返回用户态就是恢复main函数的上下文继续执行了。

<img src="/内核实现信号捕捉和处理的过程.png" alt="内核实现信号捕捉和处理的过程" style="zoom:75%;" />

# SIGCHLD 信号

## 1、SIGCHLD 信号产生的条件

* 子进程退出
* 子进程收到 SIGSTOP 信号
* 当子进程停止时，收到 SIGCONT 信号

## 2、SIGCHLD 信号的作用

子进程退出后，内核会给其父进程发送 SIGCHLD 信号，父进程收到这个信号后可以对子进程进行回收。

使用 SIGCHLD 信号完成对子进程的回收可避免父进程阻塞等待而不执行其他操作，只有当父进程收到SIGCHLD信号后才去调用信号捕捉函数完成对子进程的回收，为收到信号之前处理其他操作。

## 3、使用 SIGCHLD 信号完成对子进程的回收

父进程创建三个子进程，然后让父进程捕获 SIGCHLD 信号完成对子进程的回收：

```c
//父进程使用SICCHLD信号完成对子进程的回收
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void waitchild(int signo)
{
	pid_t wpid;
	//回收子进程
	while(1)
	{
		wpid = waitpid(-1, NULL, WNOHANG);
		if(wpid>0)
		{
			printf("child is quit, wpid==[%d]\n", wpid);
		}
		else if(wpid==0)
		{
			printf("child is living, wpid==[%d]\n", wpid);
			break;
		}
		else if(wpid==-1)
		{
			printf("no child is living, wpid==[%d]\n", wpid);
			break;
		}
	}
}

int main()
{
	//将SIGCHLD信号阻塞
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &mask, NULL);
	int i = 0;
	int n = 3;
	for(i=0; i<n; i++)	
	{
		//fork子进程
		pid_t pid = fork();
		if(pid<0) //fork失败的情况
		{
			perror("fork error");
			return -1;
		}
		else if(pid>0) //父进程
		{
			printf("father: fpid==[%d], cpid==[%d]\n", getpid(), pid);
			sleep(1);
		}
		else if(pid==0) //子进程
		{
			printf("child: fpid==[%d], cpid==[%d]\n", getppid(), getpid());
			break;
		}
	}

	//父进程
	if(i==3)
	{
		printf("[%d]:father: fpid==[%d]\n", i, getpid());
		//signal(SIGCHLD, waitchild);
		//注册信号处理函数
		struct sigaction act;
		act.sa_handler = waitchild;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sleep(5);
		sigaction(SIGCHLD, &act, NULL);

		//解除对SIGCHLD信号的阻塞
		sigprocmask(SIG_UNBLOCK, &mask, NULL);

		while(1)
		{
			sleep(1);
		}
	}

	//第1个子进程
	if(i==0)
	{
		printf("[%d]:child: cpid==[%d]\n", i, getpid());
		//sleep(1);
	}

	//第2个子进程
	if(i==1)
	{
		printf("[%d]:child: cpid==[%d]\n", i, getpid());
		sleep(1);
	}

	//第3个子进程
	if(i==2)
	{
		printf("[%d]:child: cpid==[%d]\n", i, getpid());
		sleep(1);
	}

	return 0;
}
```

注意：

* 有可能还未完成信号处理函数的注册三个子进程都退出了：

  解决办法：可以在fork之前先将SIGCHLD信号阻塞，当完成信号处理函数的注册后在解除阻塞。

* 当SIGCHLD信号函数处理期间, SIGCHLD信号若再次产生是被阻塞的,而且若产生了多次, 则该信号只会被处理一次, 这样可能会产生僵尸进程：

  解决办法: 可以在信号处理函数里面使用while(1)循环回收, 这样就有可能出现捕获一次SIGCHLD信号但是回收了多个子进程的情况，从而可以避免产生僵尸进程。

# 信号 SIGUSR 实现父子进程间通信

```c
//使用SIGUSR1和SIGUSR2在父子进程间交替数数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int num = 0;
int flag;

void func1(int signo)
{
	printf("F:[%d]\n", num);
	num += 2;
	flag = 0;
	sleep(1);
}

void func2(int signo)
{
	printf("C:[%d]\n", num);
	num += 2;
	flag = 0;
	sleep(1);
}

int main(int argc, char *argv[])
{
	int ret;
	pid_t pid;
	
	pid = fork();
	if(pid<0)
	{
		perror("fork error");
		return -1;
	}
	else if(pid>0)
	{
		num=0;
		flag  = 1;
		signal(SIGUSR1, func1);	

		while(1)
		{
			if(flag==0)
			{
				kill(pid, SIGUSR2);
				flag = 1;
			}
		}
	}
	else if(pid==0)
	{
		num=1;
		flag = 0;
		signal(SIGUSR2, func2);

		while(1)
		{
			if(flag==0)
			{
				kill(getppid(), SIGUSR1);
				flag = 1;
			}
		}
	}
}
```



