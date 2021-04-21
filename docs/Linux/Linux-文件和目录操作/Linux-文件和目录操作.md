# 文件IO函数

## 1、文件描述符

一个进程启动之后，默认打开三个文件描述符：

```c
#define STDIN_FILENO       0
#define STDOUT_FILENO      1
#define STDERR_FILENO      2
```

新打开文件返回文件描述符表中未使用的最小文件描述符，调用open函数可以打开或创建一个文件，得到一个文件描述符。

## 2、open函数

* 函数描述：打开或者新建一个文件

* 函数原型：

  `int open(const char *pathname,int flags);`

  `int open(const hcar *payhname,int flags,mode_t mode);`

* 函数参数：

  pathname：要打开或创建的文件名（包括路径）

  flags：有一系列常数值可供选择，可以同时选择多个常数用按位或运算符链接

  * 必选项：以下三个常数中必须指定一个，且仅允许指定一个。

    `O_RDONLY`:只读打开

    `O_WRONLY`:只写打开

    `O_RDWR`:可读可写打开

  * 可选项：可同时指定多个，和必选项按位或连接

    `O_APPEND`:追加

    `O_CREAT`:若文件不存在则创建，此时需要提供第三个参数mode，表示创建文件的访问权限

    `O_EXCL`:如果同时指定了`O_CREAT`,并且文件已存在,则出错返回。

    `O_TRUNC` 如果文件已存在, 将其长度截断为为0字节。

    `O_NONBLOCK `对于设备文件, 以`O_NONBLOCK`方式打开可以做非阻塞I/O

  mode：文件最终权限：mode & ~umask

* 函数返回值：

  * 成功：返回一个最小且未被占用的文件描述符
  * 失败：返回-1，并设置error值

## 3、close函数

* 函数描述：关闭文件
* 函数原型：`int close(int fd);`
* 函数参数：fd 文件描述符
* 函数返回值：
  * 成功：返回0
  * 失败：返回-1，并设置error值

需要说明的是，当一个进程终止时，内核对该进程所有尚未关闭的文件描述符调用clode关闭，所以即使用户程序不调用close，在终止时内核会自动关闭它打开的所有文件。但是对于一个常年累月运行的程序（比如网络服务器），打开的文件描述符一定要记得关闭，否则随着打开的文件越来越多，会占用大量的文件描述符和系统资源。

## 4、read函数

* 函数描述：从打开的设备或文件中读取数据
* 函数原型：`ssize_t read(int fd,void *buf,size_t count);`
* 函数参数：
  * fd:文件描述符
  * buf：读取的数据保存在缓冲区buf中
  * count：buf缓冲区存放的最大字节数
* 函数返回值：
  * 大于0：读取到的字节数
  * 等于0：文件读取完毕
  * -1：出错，并设置error值

## 5、write函数

* 函数描述：向打开的设备或文件中写数据
* 函数原型：`ssize_t write(int fd,const char*buf,size_t count);`
* 函数参数：
  * fd:文件描述符
  * buf：缓冲区，要写入文件或设备的数据
  * count：buf中数据的长度
* 函数返回值：
  * 成功：返回写入的字节数
  * 错误：返回-1，并设置error值

## 6、lseek函数

* 函数描述：移动文件指针

* 函数原型：`off_t lseek(int fd,off_t offset,int whence);`

* 函数参数：

  * fd:文件描述符

  * offset：偏移量（相对于参数whence）

  * whence：

    SEEK_SET:文件开头

    SEEK_CUR:文件当前位置

    SEEK_END:文件末尾

* 函数返回值：若lseek成功执行，则返回新的偏移量

lseek函数常用操作：

* 文件指针移动到开头：`lseek(fd,0,SEEK_SET);`

* 获取文件指针的当前位置：`int len=lseek(fd,0,SEEK_CUR);`

* 获取文件长度：`int len=lseek(fd,0,SEEK_END );`

* lseek实现文件拓展:

  * 从文件尾部开始向后拓展1000个字节

    `currpos=lseek(fd,1000,SEEK_END);`

  * 额外执行一次写操作，否则文件无法完成拓展

    `write(fd,"a",1);`

## 7、以上函数综合练习

* 编写简单的IO函数读写文件的代码

  ```c
  //IO函数测试--->open close read write lseek
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  int main(int argc, char *argv[])
  {
  	//打开文件
  	int fd = open(argv[1], O_RDWR | O_CREAT, 0777);
  	if(fd<0)
  	{
  		perror("open error");
  		return -1;
  	}
  
  	//写文件
  	//ssize_t write(int fd, const void *buf, size_t count);
  	write(fd, "hello world", strlen("hello world"));
  
  	//移动文件指针到文件开始处
  	//off_t lseek(int fd, off_t offset, int whence);
  	lseek(fd, 0, SEEK_SET);
  
  	//读文件
  	//ssize_t read(int fd, void *buf, size_t count);
  	char buf[1024];
  	memset(buf, 0x00, sizeof(buf));
  	int n = read(fd, buf, sizeof(buf));
  	printf("n==[%d], buf==[%s]\n", n, buf);
  
  	//关闭文件
  	close(fd);
  
  	return 0;
  }
  
  ```

* 使用lseek函数获取文件的大小

  ```c
  //lseek函数获取文件大小
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  int main(int argc, char *argv[])
  {
  	//打开文件
  	int fd = open(argv[1], O_RDWR);
  	if(fd<0)
  	{
  		perror("open error");
  		return -1;
  	}
  
  	//调用lseek函数获取文件大小
  	int len = lseek(fd, 0, SEEK_END);
  	printf("file size:[%d]\n", len);
  
  	//关闭文件
  	close(fd);
  
  	return 0;
  }
  ```

* 使用lseek函数实现文件的扩展

  ```c
  //lseek函数实现文件拓展
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  
  int main(int argc, char *argv[])
  {
  	//打开文件
  	int fd = open(argv[1], O_RDWR);
  	if(fd<0)
  	{
  		perror("open error");
  		return -1;
  	}
  
  	//移动文件指针到第100个字节处
  	lseek(fd, 100, SEEK_SET);
  
  	//进行一次写入操作
  	write(fd, "H", 1);
  
  	//关闭文件
  	close(fd);
  
  	return 0;
  }
  ```

## 8、perror 和 errno

errno是一个全局变量，当系统调用后若出错会将errno进行设置，perror可以将errno对应的描述信息打印出来。

如：`peeor("open");`如果报错的话打印：`open:(空格)错误信息`

# 阻塞和非阻塞

问题：阻塞和非阻塞是文件的属性还是read函数的属性？

* 通过都普通文件测试：read函数读完文件内容之后，若再次read，则read函数立刻返回，表面read函数读普通文件是非阻塞的。

* 设备文件：·/dev/tty·、`STDIN_FILENO`、管道、套接字

  通过读/dev/tty终端设备文件，表明read函数读设备文件是阻塞的。

  通过读标准输入文件，表明read函数读设备文件是阻塞的。

* **结论：阻塞和非阻塞属于文件本身的属性（不是read函数的属性）**

# 文件和目录相关的函数

## 1、stat 和 lstat 函数

* 函数描述：获取文件属性

* 函数原型：

  `int stat(const char *pathname,struct stat *buf);`

  `int lstat(const char *pathname,struct lstat *buf);`

  ```c
  struct stat {
  	dev_t      st_dev;       //文件的设备编号
  	ino_t      st_ino;       //节点
  	mode_t     st_mode;      //文件的类型和存取的权限
  	nlink_t    st_nlink;     //连到该文件的硬连接数目，刚建立的文件值为1
  	uid_t      st_uid;       //用户ID
  	gid_t      st_gid;       //组ID
  	dev_t      st_rdev;      //(设备类型)若此文件为设备文件，则为其设备编号
  	off_t      st_size;      //文件字节数(文件大小)
  	blksize_t      st_blksize;   //块大小(文件系统的I/O 缓冲区大小)
  	blkcnt_t       st_blocks;    //块数
  	time_t         st_atime;     //最后一次访问时间
  	time_t         st_mtime;     //最后一次修改时间
  	time_t         st_ctime;     //最后一次改变时间(指属性)
  };
  ```

  ```c
  //st_mode -- 16位整数(两个字节)
  0-2 bit -- 其他人权限
  			S_IROTH     00004  读权限
  			S_IWOTH     00002  写权限
  			S_IXOTH     00001  执行权限
  			S_IRWXO     00007  掩码, 过滤 st_mode中除其他人权限以外的信息
  3-5 bit -- 所属组权限
  			S_IRGRP     00040   读权限
  			S_IWGRP     00020   写权限
               S_IXGRP     00010   执行权限
  			S_IRWXG    00070  掩码, 过滤 st_mode中除所属组权限以外的信息
  6-8 bit -- 文件所有者权限
  			S_IRUSR    00400     读权限
  			S_IWUSR    00200     写权限
  			S_IXUSR    00100     执行权限
  			S_IRWXU   00700  掩码, 过滤st_mode中除文件所有者权限以外的信息
  			If (st_mode & S_IRUSR)   -----为真表明可读
               If (st_mode & S_IWUSR)  ------为真表明可写
               If (st_mode & S_IXUSR)   ------为真表明可执行
  12-15 bit -- 文件类型
  			S_IFSOCK   0140000 套接字
  			S_IFLNK    0120000 符号链接（软链接）
               S_IFREG    0100000 普通文件
  			S_IFBLK    0060000 块设备
  			S_IFDIR    0040000 目录
               S_IFCHR    0020000 字符设备
  			S_IFIFO    0010000 管道
  			S_IFMT     0170000 掩码,过滤 st_mode中除文件类型以外的信息
  			if ((st_mode & S_IFMT)==S_IFREG) ----为真普通文件
               if(S_ISREG(st_mode))   ------为真表示普通文件
               if(S_ISDIR(st.st_mode))  ------为真表示目录文件
  ```

* 函数返回值：

  成功返回  0

  失败返回 -1

* stat 和 lstat 的区别：

  对于普通文件，这两个函数没有区别，是一样的。

  对于软链接文件，调用lstat函数获取的是链接文件本身的属性信息，而stat函数获取的是链接文件指向的文件属性的信息

* 练习：

  stat函数获取文件的大小

  ```c
//stat函数测试: 获取文件大小, 文件属主和组
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  
  int main(int argc, char *argv[])
  {
  	//int stat(const char *pathname, struct stat *buf);
  	struct stat st;
  	stat(argv[1], &st);
  	printf("size:[%d], uid:[%d], gid:[%d]\n", st.st_size, st.st_uid, st.st_gid);
  
  	return 0;
  }
  ```
  
  stat函数获取文件的类型和文件的权限
  
  ```c
  //stat函数测试: 获取文件类型和权限
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  
  int main(int argc, char *argv[])
  {
  	//int stat(const char *pathname, struct stat *buf);
  	//获取文件属性
  	struct stat sb;
  	stat(argv[1], &sb);
  
  	//获取文件类型---方法1
  	if ((sb.st_mode & S_IFMT) == S_IFREG) 
   	{
  		printf("普通文件\n");
  	}	
  	else if((sb.st_mode & S_IFMT) ==S_IFDIR)
  	{
  		printf("目录文件\n");
  	}
  	else if((sb.st_mode & S_IFMT) ==S_IFLNK)
  	{
  		printf("连接文件\n");
  	}
  	//获取文件类型---方法2
  	if (S_ISREG(sb.st_mode)) 
  	{
  	 	printf("普通文件\n");
  	}
  	else if(S_ISDIR(sb.st_mode))
  	{
  		printf("目录文件\n");
  	}
  	else if(S_ISLNK(sb.st_mode))
  	{
  		printf("连接文件\n");
  	}
  
  	//判断文件权限
  	if(sb.st_mode & S_IROTH)
  	{
  		printf("---R----");
  	}
  
  	if(sb.st_mode & S_IWOTH)
  	{
  		printf("---W----");
  	}
  	
  	if(sb.st_mode & S_IXOTH)
  	{
  		printf("---X----");
  	}
  
  	printf("\n");
  
  	return 0;
  }
  ```
  
  lstat函数获取链接文件的属性（文件大小）
  
  ```c
  //stat函数测试: 获取文件大小, 文件属主和组
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  
  int main(int argc, char *argv[])
  {
  	//int stat(const char *pathname, struct stat *buf);
  	struct stat st;
  	lstat(argv[1], &st);
  	printf("size:[%d], uid:[%d], gid:[%d]\n", st.st_size, st.st_uid, st.st_gid);
  
  	return 0;
  }
  ```

## 2、opendir 函数

* 函数描述：打开一个目录
* 函数原型：`DIR *opendir(const char *name);`
* 函数返回值：指向目录的指针
* 函数参数：要遍历的目录（相对路径或绝对路径）

## 3、readdir函数

* 函数描述：读取目录内容---目录项

* 函数原型：`struct dirent *readdir(DIR *dirp);`

* 函数返回值：读取的目录项指针

* 函数参数：opendir函数的返回值

  ```c
  struct dirent
  {
    ino_t d_ino;              // 此目录进入点的inode
    off_t d_off;              // 目录文件开头至此目录进入点的位移
    signed short int d_reclen;   // d_name 的长度, 不包含NULL 字符
    unsigned char d_type;     // d_name 所指的文件类型 
    char d_name[256];	        // 文件名
  };
  ```

  d_type的取值: 

  * `DT_BLK` - 块设备
  * `DT_CHR` - 字符设
  * `DT_DIR` - 目录
  * `DT_LNK` - 软连接
  * `DT_FIFO` - 管道
  * `DT_REG` - 普通文件
  * `DT_SOCK` - 套接字
  * `DT_UNKNOWN `- 未知

## 4、closedir函数

* 函数描述：关闭目录
* 函数原型：`int closedir(DIR *dirp);`
* 函数返回值：成功返回0，失败返回-1
* 函数参数：opendir函数的返回值

## 5、读取目录的一般步骤

* `DIR *pDir = opendir("dir");`   //打开目录
* `while((p=readdir(pDir))!=NULL){ }`    //循环读取文件
* `closedir(pDir);`    //关闭目录

## 6、练习

* 遍历指定目录下的所有文件，判断文件类型

* 递归遍历目录下的所有文件，并判断文件类型

  注意：递归遍历指定目录下的所有文件的时候，要过滤掉  . 和 .. 文件，否则会进入死循环

  ```c
  //目录操作测试: opendir readdir closedir
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <dirent.h>
  
  int main(int argc, char *argv[])
  {
  	//打开目录
  	//DIR *opendir(const char *name);
  	DIR *pDir = opendir(argv[1]);
  	if(pDir==NULL)
  	{
  		perror("opendir error");
  		return -1;
  	}
  
  	//循环读取目录项
  	//struct dirent *readdir(DIR *dirp);
  	struct dirent *pDent = NULL;
  	while((pDent=readdir(pDir))!=NULL)
  	{
  		//过滤掉.和..文件
  		if(strcmp(pDent->d_name, ".")==0 || strcmp(pDent->d_name, "..")==0)
  		{
  			continue;
  		}
  
  		printf("[%s]---->", pDent->d_name);
  
  		//判断文件类型
  		switch(pDent->d_type)
  		{
  			case DT_REG:
  				printf("普通文件");
  				break;
  
  			case DT_DIR:
  				printf("目录文件");
  				break;
  
  			case DT_LNK:
  				printf("链接文件");
  				break;
  
  			default:
  				printf("未知文件");
  		}
  
  		printf("\n");
  	}
  
  	//关闭目录
  	closedir(pDir);
  
  	return 0;
  }
  ```

## 7、dup 函数

* 函数描述：复制文件描述符

* 函数原型：`int dup(int oldfd);`

* 函数参数：oldfd---要复制的文件描述符

* 函数返回值：

  * 成功：返回最小且没被占用的文件描述符
  * 失败：返回-1，设置errno值

* 练习：编写程序，测试dup函数

  ```c
  //测试dup函数复制文件描述符
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  
  int main(int argc, char *argv[])
  {
  	//打开文件
  	int fd = open(argv[1], O_RDWR);
  	if(fd<0)
  	{
  		perror("open error");
  		return -1;
  	}
  
  	//调用dup函数复制fd
  	int newfd = dup(fd);
  	printf("newfd:[%d], fd:[%d]\n", newfd, fd);
  
  	//使用fd对文件进行写操作
  	write(fd, "hello world", strlen("hello world"));
  
  	//调用lseek函数移动文件指针到开始处
  	lseek(fd, 0, SEEK_SET);
  
  	//使用newfd读文件
  	char buf[64];
  	memset(buf, 0x00, sizeof(buf));
  	int n = read(newfd, buf, sizeof(buf));
  	printf("read over: n==[%d], buf==[%s]\n", n, buf);
  
  	//关闭文件
  	close(fd);
  	close(newfd);
  
  	return 0;
  }
  ```

## 8、dup2 函数

* 函数描述：复制文件描述符

* 函数原型：`int dup2(int oldfd,int newfd);`

* 函数参数：

  oldfd---原来的文件描述符

  newfd---复制成的新的文件描述符

* 函数返回值：

  * 成功：将oldfd复制给newfd，两个文件描述符指向同一个文件
  * 失败：返回-1，设置errno值

* 假设newfd已经指向了一个文件，首先close原来打开的文件，然后newfd指向oldfd的文件，若newfd没有被占用，newfd指向oldfd指向的文件。

* 练习：

  编写程序，测试dup2函数实现文件描述符的复制。

  ```c
  //测试dup2函数复制文件描述符
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  
  int main(int argc, char *argv[])
  {
  	//打开文件
  	int oldfd = open(argv[1], O_RDWR | O_CREAT, 0755);
  	if(oldfd<0)
  	{
  		perror("open error");
  		return -1;
  	}
  
  	int newfd = open(argv[2], O_RDWR | O_CREAT, 0755);
  	if(newfd<0)
  	{
  		perror("open error");
  		return -1;
  	}
  	//调用dup2函数复制fd
  	dup2(oldfd, newfd);
  	printf("newfd:[%d], oldfd:[%d]\n", newfd, oldfd);
  
  	//使用fd对文件进行写操作
  	write(newfd, "hello world", strlen("hello world"));
  
  	//调用lseek函数移动文件指针到开始处
  	lseek(newfd, 0, SEEK_SET);
  
  	//使用newfd读文件
  	char buf[64];
  	memset(buf, 0x00, sizeof(buf));
  	int n = read(oldfd, buf, sizeof(buf));
  	printf("read over: n==[%d], buf==[%s]\n", n, buf);
  
  	//关闭文件
  	close(oldfd);
  	close(newfd);
  
  	return 0;
  }
  ```

  编写程序，完成终端标准输出重定向到文件中

  ```c
  //使用dup2函数实现标准输出重定向操作
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  
  int main(int argc, char *argv[])
  {
  	//打开文件
  	int fd = open(argv[1], O_RDWR | O_CREAT, 0777);
  	if(fd<0)
  	{
  		perror("open error");
  		return -1;
  	}
  
   	//调用dup2函数实现文件重定向操作
  	dup2(fd, STDOUT_FILENO);
  		
  	printf("ni hao hello world");
  
  	close(fd);
  	close(STDOUT_FILENO);
  
  	return 0;
  }
  ```

## 9、fcntl 函数

* 函数描述：改变已经打开的文件的属性

* 函数原型：`int fcntl(int fd,int cnmd,.../* arg */);`

* 函数参数：

  * 若 cmd 为 F_DUPFD，复制文件描述符，与 dup 相同
  * 若 cmd 为 F_GETFL，获取文件描述符的 flag 属性值
  * 若 cmd 为 F_SETFL，设置文件描述符的 flag 属性

* 函数返回值：

  * 成功：

    若 cmd 为 F_DUPFD，返回一个新的文件描述符

    若 cmd 为 F_GETFL，返回文件描述符的flags值

    若 cmd 为 F_SETFL，返回 0

  * 失败：返回-1，设置errno值

* fcntl 函数常用的操作：

  * 复制一个新的文件描述符：`int newfd=fcntl(fd,F_DUPFD,0);`

  * 获取文件的属性标志：`int flag=fcntl(fd,F_GETFL,0);`

  * 设置文件状态标志：

    `flag=flag | O_APPEND);`

    `fcntl(fd,F_SETFL,flag);`

  * 常用的属性标志：

    `O_APPEND`:设置文件打开为末尾追加

    `O_NONBLOCK`:设置打开的文件描述符为非阻塞

* 练习：

  使用 fcntl 函数实现复制文件描述符

  使用 fcntl 函数设置在打开的文件末尾追加内容

  ```c
  //修改文件描述符的flag属性
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  
  int main(int argc, char *argv[])
  {
  	//打开文件
  	int fd = open(argv[1], O_RDWR);
  	if(fd<0)
  	{
  		perror("open error");
  		return -1;
  	}
  
  	//获得和设置fd的flags属性
  	int flags = fcntl(fd, F_GETFL, 0);
  	flags = flags | O_APPEND;
  	fcntl(fd, F_SETFL, flags);
  
  	//写文件
  	write(fd, "hello world", strlen("hello world"));
  
  	//关闭文件
  	close(fd);
  
  	return 0;
  }
  ```

  



