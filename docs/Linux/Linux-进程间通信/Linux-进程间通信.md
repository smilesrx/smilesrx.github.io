---
typora-root-url: Image
---

# 进程间通信

Linux环境下，进程地址空间相互独立，每个进程各自有不同的用户空间。任何一个进程的全局变量在另一个进程中都看不到，所以进程和进程之间不能相互访问，要交换数据必须通过内核，在内核中开辟一块缓冲区，进程1把数据从用户空间拷贝到内核缓冲区，进程2再从内核缓冲区把数据读走，内核提供的这种机制称为进程间通信（IPC）

<img src="/进程间通信.png" alt="进程间通信" style="zoom: 67%;" />

# 进程间通信的方式

在进程间完成数据的传递需要借助操作系统提供的方法，如：文件、管道、信号、共享内存、消息队列、套接字、命名管道。现今常用的进程间通信方式：

* 管道（使用最简单）
* 信号（开销最小）
* 共享映射区（无血缘关系）
* 本地套接字（最稳定）

# 管道（匿名）- pipe

## 1、管道的概念

管道是一种最基本的IPC机制，也称匿名管道，应用于有血缘关系的进程间通信，完成数据的传递。调用 pipe 函数即可创建一个管道。

<img src="/pipe.png" alt="pipe" style="zoom:75%;" />

管道的特征：

* 管道的本质是一块内核缓冲区
* 管道由两个文件描述符引用，一个表示读端，一个表示写端
* 规定数据从管道的写端流入管道，从读端流出
* 管道的读端和写端默认设计阻塞的

## 2、管道的原理

* 管道的本质是内核缓冲区，内部使用环形队列实现
* 默认缓冲区大小 4k，可使用`ulimit -a`命令获取大小
* 实际操作过程中缓冲区会根据压力做适当调整

## 3、管道的局限性

* 数据一旦被读走，便不再管道中存在，不可反复读取
* 数据只能在一个方向上流动，若要实现双向流动，必须使用两个管道
* 只能在有血缘关系的进程间使用管道。

## 4、创建管道 - pipe 函数

* 函数描述：创建一个管道
* 函数原型：`int pipe(int fd[2]);`
* 函数参数：若函数调用成功，fd[0]存放管道的读端，fd[1]存放管道的写端
* 函数返回值：成功返回0、失败返回-1，并设置error值。

函数调用成功返回读端和写端的文件描述符，其中fd[0] 是读端， fd[1]是写端，向管道读写数据是通过使用这连个文件描述符进行的，读写管道的实质是操作内核缓冲区。

管道创建成功后，创建管道的进程（父进程）同时掌握着管道的读端和写端。

## 5、父子进程间使用匿名管道通信

一个进程在由 pipe 函数创建管道后，一般再 fork 一个子进程，然后通过管道实现父子进程间通信（因此不难推出，只要两个进程中存在血缘关系都可以采用管道方式来进行通信）。父子进程间具有相同的文件描述符，且指向同一个管道 pipe ,其他没有关系的进程不能获得 pipe 产生的两个文件描述符，也就无法利用管道进行通信。

* 第一步：父进程调用pipe函数创建管道
* 第二步：父进程调用fork函数创建子进程
* 第三步：父进程关闭 fd[0]，子进程关闭fd[1]。（父子进程各关闭不用的一端）

创建步骤总结：

* 父进程调用 pipe 函数创建管道，得到两个问价描述符 fd[0]、fd[1]，分别指向管道的读端和写端
* 父进程调用 fork 函数创建子进程，那么子进程也有两个文件描述符指向同一个管道
* 父进程关闭管道的读端，子进程关闭管道的写端。父进程可以向管道中写入数据，子进程将管道中的数据读走，这样就实现了父子进程间的通信。

## 6、管道的读写行为

* 读操作
  * 有数据：read正常读，返回读到的字节数
  * 无数据
    * 写端全部关闭：read解除阻塞，返回0，相当于读到了文件末尾
    * 写端没有全部关闭：read阻塞
* 写操作
  * 读端全部关闭：管道破裂，进程终止，内核给当前进程发SIGPIPE信号
  * 读端没有全部关闭
    * 缓冲区写满了：write阻塞
    * 缓冲区没有写满：write继续

## 7、设置管道为非阻塞

默认情况下，管道的读写两端都是阻塞的，若要设置读或者写端为非阻塞，可参照以下步骤：

* 第一步：`int flag=fcntl(fd[0],F_GETFL,0);`
* 第二步：`flag | =O_NONBLOCK；`
* 第三步：`fcntl(fd[0],SETFL,flag);`

若是读端设置为非阻塞：

* 写端没有全部关闭：管道中没有数据可读，read返回-1
* 写端没有全部关闭：管道中有数据可读，read返回实际读到的字节数
* 写端全部关闭：管道中没有数据可读，read返回0
* 写端全部关闭：管道中有数据可读，read返回实际读到的字节数

## 8、如何查看缓冲区的大小

* 命令：`ulimit -a`

* 函数：

  ` long fpathconf(int fd,int name);`

  `printf("pipe size==[%ld]\n", fpathconf(fd[0], _PC_PIPE_BUF));`

  `printf("pipe size==[%ld]\n", fpathconf(fd[1], _PC_PIPE_BUF));`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{
	 //创建管道
	int fd[2]={0};
	int ret=pipe(fd);  
	//int pipe(int fd[2]);
	if(ret<0)   //管道创建失败
	{
		perror("pipe error");
		return -1;
	}
	
	//查看缓冲区的大小
	printf("pipe size==[%ld]\n",fpathconf(fd[0],_PC_PIPE_BUF));
	printf("pipe size==[%ld]\n",fpathconf(fd[1],_PC_PIPE_BUF));

	//write(fd[1],"hello world",strlen("hello world"));
	close(fd[1]);
	//write(fd[1],"hello world",strlen("hello world"));

	//设置管道非阻塞
	int flag=fcntl(fd[0],F_GETFL,0);
	flag |=O_NONBLOCK;
	fcntl(fd[0],F_SETFL,flag);
	
	char buf[64];
	memset(buf,0x00,sizeof(buf));
	int n=read(fd[0],buf,sizeof(buf));
	printf("read over: n==[%d] buf==[%s]\n",n,buf);
    
	return 0;
}
```

## 7、练习

* 使用管道实现父子进程间通信

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  
  int main(int argc char *argv[])
  {
      //调用pipe函数创建管道
      int fd[2]={0};
      int ret=pipe(fd);
      if(fd<0)   //创建管代失败
      {
          perror("pipe error");
          return -1;
      }
      
      //调用fork函数创建子进程
      pid_t pid=fork();
      if(pid<0)   //创建子进程失败
      {
          perrror("fork error");
          return -1;
      }
      if(fd>0)   //父进程
      {
          //关闭管道的读端
          close(fd[0]);
          //写数据到管道的写端
          write(fd[1],"hello world",strlen("hello world"));
          wait(NULL);
      }
      if(fd==0)  //子进程
      {
          //关闭管道的写端
          close(fd[1]);
          //从管道的读端读数据
          char buf[64];
          memset(buf,0x00,sizeof(buf));
          int n=read(fd[0],buf,sizeof(buf));
          printf("read over: n==[%d] buf==[%s]\n",n,buf);
      }
      
      return 0;
  }
  ```

* 父子进程间通信实现 ps aux | grep bash

  思路：

  1. pipe函数创建管道
  2. fork函数创建子进程
  3. 父进程关闭管道的读端
  4. 子进程关闭管道的写端
  5. 父进程调用 dup2 函数将标准输出重定向到管道的写端
  6. 子进程调用 dup2 函数将标准输入重定向到管道的读端
  7. 父进程调用 execlp 函数执行 ps aux命令
  8. 子进程调用 execlp 函数执行 grep bash 命令

  ```c
  #incldue <stdio.h>
  #incldue <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <sys/types.h>
  
  int main (int argc char *argv[])
  {
      //pipe函数创建子进程
      int fd[2];
      int ret=pipe(fd);
      if(ret<0)   //管道创建失败
      {
          perror("pipe error");
          return -1;
      }
      
      //fork创建子进程
      pid_t pid=fork();
      if(pid<0)  //子进程创建失败
      {
          perror("fork error");
          return -1;
      }
      if(pid>0)    //父进程
      {
          //关闭管道的读端
          close(fd[0]);
          //调用dup2函数将标准输出重定向到管道的写端
          dup2(fd[1],STDOUT_FILENO);
          //调用execlp函数执行 ps aux命令
          execlp("ps","ps","aux",NULL);
          perror("execlp error");
      }
      if(pid==0)  //子进程
      {
          //关闭管道的写端
          close(fd[1]);
          //调用dup2函数将标准输入重定向到管到读端
          dup2(fd[0],STDIN_FILENO);
          //调用execlp函数执行 grep bash命令
          execlp("grep","grep","--color=auto","bash",NULL);
          perror("execlp error");
      }
      
      return 0;
  }
  ```

* 兄弟进程间通信实现 ps aux | grep bash，父进程调用waitpid函数完成对子进程的回收

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  
  int main(int argc,char *argv[])
  {
      //pipe创建管道
      int fd[2];
      int ret=pipe(fd);
      if(ret<0)  //管道创建失败
      {
          perror("pipe error");
          return -1;
      }
      
      //fork创建子进程
      int i=0;
      int n=2;
      pid_t pid;
      for(i=0;i<n;i++)
      {
          pid=fork();
          if(pid<0)   //管道创建失败
          {
              perror("fork error");
              return -1;
          }
          if(pid==0)  //子进程
          {
              break;  //防止子进程创建孙子进程
          }
      }
      
      if(i==n)  //父进程
      {
          //关闭管道的读端和写端
          close(fd[0]);
          close(fd[1]);
          //waitpid函数完成对子进程的资源回收
          int status;
          pid_t wpid;
          while(1)
          {
              wpid=waitpid(-1,&status,WNOHANG);
              if(wpid==0)  //有子进程还在运行
              {
                  sleep(1);
                  continue;
              }
              if(wpid==-1)  //已经没有子进程可退出
              {
                  printf("no child is living ,wpid==[%d]\n",wpid);
                  exit(0);
              }
              if(wpid>0)  //子进程退出
              {
                  if(WIFEXITED(status))  //子进程正常退出
                  {
                      printf("child:[%d] normal exited ,status==[%d]\n",wpid,WEXITSTATUS(status));
                  }
                  if(WIFSIGNALED(status))  //子进程被信号杀死
                  {
                      printf("child killed by signo==[%d]\n",WTERMSIG(status));
                  }
              }
          }
      }
      
      if(i==0)  //第一个子进程
      {
          //关闭管道的读端
          close(fd[0]);
          //调用dup2函数将标准输出重定向到管道的写端
          dup2(fd[1],STDOUT_FILENO);
          //调用execlp函数执行 ps aux 命令
          execlp("ps","ps","aux",NULL);
          perror("execlp error");
          close(fd[1]);
      }
      if(i==1)  //第二个子进程
      {
          //关闭管道的写端
          close(fd[1]);
          //调用dup2函数将标准输入重定向到管道的读端
          dup2(fd[0],STDIN_FILENO);
          //调用execlp函数执行 grep bash 命令
          execlp("grep","grep","--color=auto","bash",NULL);
          perror("execlp error");
          close(fd[0]);
      }
      
      return 0;
  }
  ```

# 命名管道- FIFO

## 1、FIFO 概念

FIFO常被称为命名管道，以区分管道（pipe)。管道（pipe)只能用于有血缘关系的进程间通信，但通过FIFO，可实现不相关的进程间通信。

FIFO是Linux基础问价类型中的一种，（文件类型为p，可通过 ls -l 查看文件的类型）。但FIFO文件在磁盘上没有数据块，文件大小为0，仅仅用来标识内核中的一条通道。进程可以打开这个文件进行read/write，实际上是在读写内核缓冲区，这样就实现了进程间的通信。

## 2、创建管道

* 方式一：使用命令 `mkfifo 管道名`
* 方式二：使用函数`int mkfifo(const char* pathname,mode_t mode);`

当创建了一个FIFO，就可以使用open 函数打开它，常见的文件I/O函数都可以用于FIFO。

FIFO严格遵循先进先出，对FIFO的读总是从开始处返回数据，对他们的写则把数据加到末尾，**不支持lseek函数等文件定位操作。**

## 3、使用FIFO完成两个进程通信

<img src="/FIFO.png" alt="FIFO" style="zoom:75%;" />

思路：

* 进程A：
  * 创建一个FIFO文件：myfifo
  * 调用open函数打myfifo文件
  * 调用write函数写入一个字符串，其实是将数据写到了内核缓冲区。
  * 调用close函数关闭myfifo文件
* 进程B：
  * 调用open函数打开myfifo文件
  * 调用read函数读取文件的内容，其实就是从内核缓冲区读取数据
  * 打印读取到的数据
  * 调用close函数关闭myfifo文件

注意：myfifo文件是在进程A中创建的，如果先启动进程B会报错

解决：调用access函数检查myfifo文件是否存在。

```c
//fifo完成两个进程间通信的测试---write.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	//创建fifo文件
	//int mkfifo(const char *pathname, mode_t mode);
	int ret = access("./myfifo", F_OK);
	if(ret!=0)
	{		
		ret = mkfifo("./myfifo", 0777);
		if(ret<0)
		{
			perror("mkfifo error");
			return -1;
		}
	}

	//打开文件
	int fd = open("./myfifo", O_RDWR);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}

	//写fifo文件
	int i = 0;
	char buf[64];
	while(1)
	{
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%d:%s", i, "hello world");
		write(fd, buf, strlen(buf));
		sleep(1);

		i++;
	}

	//关闭文件
	close(fd);

	//getchar();

	return 0;
}
```

```c
//fifo完成两个进程间通信的测试---read.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	//创建fifo文件
	//int mkfifo(const char *pathname, mode_t mode);
	//判断myfofo文件是否存在,若不存在则创建
	int ret = access("./myfifo", F_OK);
	if(ret!=0)
	{
		ret = mkfifo("./myfifo", 0777);
		if(ret<0)
		{
			perror("mkfifo error");
			return -1;
		}
	}

	//打开文件
	int fd = open("./myfifo", O_RDWR);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}

	//读fifo文件
	int n;
	char buf[64];
	while(1)
	{
		memset(buf, 0x00, sizeof(buf));
		n = read(fd, buf, sizeof(buf));
		printf("n==[%d], buf==[%s]\n", n, buf);
	}

	//关闭文件
	close(fd);

	return 0;
}
```

# 内存映射区

## 1、共享内存

存储映射I/O使一个磁盘文件与存储空间中的一个缓冲区苍映射。从缓冲区中读取数据，就相当于读文件中的相应字节；将数据写入缓冲区，则会将数据写入文件。这样，就可在不使用read和write函数的情况下，使用地址（指针）完成I/O操作。

使用共享内存的方式，首先应通知内核，将一个指定的文件映射到存储区。这个映射工作可以通过mmap函数来实现。

<img src="/共享内存.png" alt="共享内存" style="zoom:67%;" />

## 2、mmap 函数

* 函数作用：建立存储映射区
* 函数原型：`void *mmap(void *addr,size_t length,int prot,int flags,int fd,off_t offset);`
* 函数参数：
  * addr：指定映射区的起始地址，通常设为NULL，由系统指定
  * length：映射到内存的文件大小
  * port：映射区的保护方式，最常用点的：PROT_READ、PROT_WRITE、PROT_READ | PROT_WRITE
  * flags：映射区的特性：
    * MAP_SHARED：写入映射区的数据会写回文件，且允许其他映射该文件的进程共享
    * MAP_PRIVATE:对映射区的写入操作会产生一个映射区的复制，对此映射区域所做的修改不会写回原文件。
  * fd：由open返回的文件描述符，代表映射的文件
  * offset：以文件开始处的偏移量。必须是 4k 的整数倍，通常为0，表示从文件的开头开始映射。
* 函数返回值：
  * 成功：返回创建映射区的首地址
  * 失败：MAP_FAILED宏

## 3、munmap 函数

* 函数作用：释放有mmap函数建立的存储映射区
* 函数原型：`int munmap(void *addr,size_t length);`
* 函数返回值：
  * 成功：返回0
  * 失败：返回-1，并设置errno值
* 函数参数：
  * addr：调用mmap函数成功返回的映射区的首地址
  * length：映射区大小（mmap函数的第二个参数）

## 4、mmap 注意事项

* 创建映射区的过程中，隐含着一次对映射文件的读操作，将文件内容读取到映射区
* 当 MAP_SHARED时，要求：映射区的权限位应<=文件的打开权限（出于对映射区的保护）。而 MAP_PRIVATE则无所谓，因为mmap中的权限是对内存的限制。
* 映射区的释放与文件的关闭无关，只要映射区建立成功，文件可以立刻关闭
* 特别注意：当映射文件大小为0时，不能创建映射区，所以，用于映射的文件必须要有实际的大小；mmap使用时常常会出现总线的错误，通常是由于共享文件存储空间大小引起的
* munmap 传入的地址一定是mmap的返回地址，坚决杜绝指针++操作。
* 文件便宜必须为0或者 4k 的整数倍
* mmap创建映射区出错概率高，一定要检查返回值，确保映射区建立成功再进行后续操作。

## 5、有关mmap函数的使用总结

* 第一个参数传NULL
* 第二个参数是要映射的文件大小，必须大于0
* 第三个参数：PORT_WRITE | PORT_READ
* 第四个参数：MAP_SHARED或MAP_PRIVATE
* 第五个参数：打开文件的文件描述符
* 第六个参数：0或 4k的整数倍

## 6、练习

* 使用mmap函数完成父子进程间通信

  ![mmap](/mmap.png)

  思路：

  * 调用mmap函数创建映射区，返回映射区的地址
  * 调用fork函数创建子进程，子进程也拥有了映射区首地址
  * 父子进程可以通过映射区首地址指针完成通信
  * 调用munmap函数释放存储映射区

  ```c
  //使用mmap函数完成父子进程间通信
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  #include <sys/stat.h>
  #include <sys/mman.h>
  
  int main()
  {
  	//使用mmap函数建立共享映射区
  	//void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
  	int fd = open("./test.log", O_RDWR);  //打开映射文件
  	if(fd<0)   //打开文件失败
  	{
  		perror("open error");
  		return -1;
  	}
  
  	int len = lseek(fd, 0, SEEK_END);
  
  	void * addr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  	//void * addr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
  	if(addr==MAP_FAILED)  //建立映射区失败
  	{
  		perror("mmap error");
  		return -1;
  	}
  	close(fd);
  
  	//创建子进程
  	pid_t pid = fork();
  	if(pid<0) 
  	{
  		perror("fork error");
  		return -1;
  	}
  	else if(pid>0)  //父进程
  	{
  		memcpy(addr, "hello world", strlen("hello world"));	
  		wait(NULL);
  	}
  	else if(pid==0)  //子进程
  	{
  		sleep(1);  //让父进程先执行写操作
  		char *p = (char *)addr;
  		printf("[%s]", p);
  	}
  
  	return 0;
  }
  ```

* 使用mmap函数完成没有血缘关系的进程间通信

  ```c
  //使用mmap函数完成两个不相干进程间通信  ---write.c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  #include <sys/stat.h>
  #include <sys/mman.h>
  
  int main()
  {
  	//使用mmap函数建立共享映射区
  	//void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
  	int fd = open("./test.log", O_RDWR);  //打开映射文件
  	if(fd<0)   //打开文件失败
  	{
  		perror("open error");
  		return -1;
  	}
  
  	int len = lseek(fd, 0, SEEK_END);
  
  	//建立共享映射区
  	void * addr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  	if(addr==MAP_FAILED)  //映射区建立失败
  	{
  		perror("mmap error");
  		return -1;
  	}
  	
  	memcpy(addr, "0123456789", 10);  //写数据到映射区地址
  
  	return 0;
  }
  ```

  ```c
  //使用mmap函数完成两个不相干进程间通信
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  #include <sys/stat.h>
  #include <sys/mman.h>
  
  int main()
  {
  	//使用mmap函数建立共享映射区
  	//void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
  	int fd = open("./test.log", O_RDWR);  //打开映射文件
  	if(fd<0)  //打开文件失败
  	{
  		perror("open error");
  		return -1;
  	}
  
  	int len = lseek(fd, 0, SEEK_END);
  
  	//建立共享映射区
  	void * addr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  	if(addr==MAP_FAILED)  //映射区建立失败
  	{
  		perror("mmap error");
  		return -1;
  	}
  
  	char buf[64];
  	memset(buf, 0x00, sizeof(buf));
  	memcpy(buf, addr, 10);  //从映射区地址读取数据
  	printf("buf==[%s]\n", buf);
  
  	return 0;
  }
  ```

* 使用mmap函数建立匿名映射完成父子进程间通信

  ```c
  //使用mmap匿名映射完成父子进程间通信
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  #include <sys/stat.h>
  #include <sys/mman.h>
  
  int main()
  {
  	//使用mmap函数建立共享映射区
  	//void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
  	void * addr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  	if(addr==MAP_FAILED)   //映射区建立失败
  	{
  		perror("mmap error");
  		return -1;
  	}
  
  	//创建子进程
  	pid_t pid = fork();
  	if(pid<0) 
  	{
  		perror("fork error");
  		return -1;
  	}
  	else if(pid>0)   //父进程
  	{
  		memcpy(addr, "hello world", strlen("hello world"));	 //写数据到映射区
  		wait(NULL);
  	}
  	else if(pid==0)  //子进程
  	{
  		sleep(1);
  		char *p = (char *)addr;
  		printf("[%s]", p);  //从映射区都数据
  	}
  
  	return 0;
  }
  ```

  







