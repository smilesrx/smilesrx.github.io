---
typora-root-url: Image

---

# 线程

## 1、什么是线程

* 轻量级的进程，在Linux环境下线程的本质任然是进程
* 进程：拥有独立的地址空间，拥有PCB，相当于**独居**
* 线程：有PCB，但没有独立的地址空间，多个线程共享进程空间，相当于**合租**

<img src="/线程.png" alt="线程" style="zoom: 67%;" />

在Linux操作系统下：

* 线程：最小的执行单位
* 进程：最小分配资源单位，可看成是只有一个线程的进程。

线程的特点：

* 线程是轻量级的进程，也有PCB，创建线程使用的是底层函数和进程一样，都是clone
* 从内核看进程和线程是一样发的，都有各自不同的PCB
* 进程可以蜕变成线程
* 在Linux下，线程最小是执行的单位；进程是最小的分配资源单位。
* 查看指定进程的LWP号：`ps -Lf pid`

实际上，无论是创建进进程的 fork，还是创建线程的 pthread_create,底层实现都是调用同一内核函数clone

* 如果复制对方的地址空间，那么久产生一个 "进程"；
* 如果共享对方的地址空间，就产生一个 "线程"。

**所以：Linux内核是不区分进程和线程的，只有在用户层面上进行区分。**

## 2、线程共享资源

* 文件描述符
* 每种信号的处理函数
* 当前的工作目录
* 用户ID和组ID
* 内存地址空间

## 3、线程非共享资源

* 线程ID
* 处理器现场和栈指针（内核栈）
* 独立的栈空间（用户空间）
* errno变量
* 信号屏蔽字
* 调度优先级

## 4、线程优、缺点

* 优点：
  * 提高程序的并发性
  * 开销小
  * 数据通行、共享数据方便
* 缺点：
  * 库函数，不稳定
  * gdb调试、编写困难
  * 对信号支持不好

优点相对突出，缺点均不是硬伤。Linux下由于事先方法导致进程、线程差别不大。

## 5、pthread_create 函数

* 函数作用：创建一个线程

* 函数原型：`int pthread_create(pthread_t *thread, const pthread_attr *attr, void *(start_routine)(void *)), void *arg;`

* 函数参数：

  * thread：传出参数，保存系统为我们分配好的线程ID
  * attr：通常传NULL，表示使用默认属性。若想使用具体的属性也可以修改该参数
  * start_routine：函数指针，指向线程主函数（线程体），该函数运行结束，则线程结束
  * arg：线程主函数执行期间所使用的参数

* 函数返回值：

  * 成功：返回0
  * 失败：返回错误号

* 注意点：

  * 由于pthread_create 的错误码不保存在errno中，因此不能直接使用perror() 打印错误信息。可以使用strerror()  把错误码转换成错误信息再打印。
  * 如果任意一个线程调用了 exit 或 exit_t ,则整个进程的所有线程终止，由于从main函数return也相当于调用exit，为了防止新创建的线程还没有得到执行就终止，我们在main函数return之前延时1秒。

* 练习1：

  * 创建一个线程，并给线程传递一个int参数

  * 创建一个线程，并给线程传递一个结构体参数

    ```c
    //创建子线程: 传递参数
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <pthread.h>
    
    struct Test{
    	int data;
    	char name[64];
    };
    
    //线程执行函数
    void *mythread(void *arg)
    {
    	//int n = *(int *)arg;
        //printf("n==[%d]\n", n);
        
    	struct Test *p = (struct Test *)arg;
    	//struct Test *p = arg;
    	printf("[%d][%s]\n", p->data, p->name);
    	printf("child thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
    }
    
    int main()
    {
    	int n = 99;
    	struct Test t;
    	memset(&t, 0x00, sizeof(struct Test));
    	t.data = 88;
    	strcpy(t.name, "xiaowen");	
    	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        //                      void *(*start_routine) (void *), void *arg);
    	//创建子线程
    	pthread_t thread;
        //int ret = pthread_create(&thread, NULL, mythread, NULL);
        //int ret = pthread_create(&thread, NULL, mythread, &n);
    	int ret = pthread_create(&thread, NULL, mythread, &t);
    	if(ret!=0)
    	{
    		printf("pthread_create error, [%s]\n", strerror(ret));
    		return -1;
    	}
    	printf("main thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
    
    	//目的是为了让子线程能够执行起来
    	sleep(1);
    	return 0;
    }
    ```

* 练习2

  主线程循环创建5个子线程，并让子线程判断自己是第几个子线程。

  结果：最后每个子线程打印出来的值并不是想象中的值，比如都是5

  原因：在创建子线程的时候使用循环因子作为参数传递给子线程，这样主线程和多个子线程就会共享变量i（变量i在main函数中定义，在整个进程都一直有效）所以在子线程看来变量i是合法的栈内存空间。那么为什么最后每个子线程打印出来的值都是5呢? 是由于主线程可能会在一个cpu时间片内连续创建了5个子线程，此时变量i的值变成了5，当主线程失去cpu的时间片后，子线程得到cpu的时间片，子线程访问的是变量i的内存空间的值，所以打印出来值为5。

  <img src="/主线程和子线程共享同一块内存空间.png" alt="主线程和子线程共享同一块内存空间" style="zoom:75%;" />

  <img src="/主线程和子线程分时使用CPU资源.png" alt="主线程和子线程分时使用CPU资源" style="zoom:75%;" />

  解救办法：不能使多个子线程都共享同一块内存空间，应该使每个子线程访问不同的内存空间，可以在主线程定义一个数组：int arr[5];，然后创建线程的时候分别传递不同的数组元素，这样每个子线程访问的就是互不相同的内存空间，这样就可以打印正确的值。

  <img src="/多个子线程各自访问不同的内存空间.png" alt="多个子线程各自访问不同的内存空间" style="zoom:75%;" />

  ```c
  //循环创建子线程,并且打印是第几个子线程
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  
  //线程执行函数
  void *mythread(void *arg)
  {
  	int i = *(int *)arg;
  	printf("[%d]:child thread, pid==[%d], id==[%ld]\n", i, getpid(), pthread_self());
  	sleep(100);
  }
  
  int main()
  {
  	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
  	//                      void *(*start_routine) (void *), void *arg);
  	//创建子线程
  	int ret;
  	int i = 0;
  	int n = 5;
  	int arr[5];
  	pthread_t thread[5];
  	for(i=0; i<n; i++)
  	{
  		arr[i] = i;
  		ret = pthread_create(&thread[i], NULL, mythread, &arr[i]);
  		if(ret!=0)
  		{
  			printf("pthread_create error, [%s]\n", strerror(ret));
  			return -1;
  		}
  	}
  	printf("main thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  
  	//目的是为了让子线程能够执行起来
  	sleep(100);
  	return 0;
  }
  ```

## 6、pthread_exit 函数

在线程中禁止调用 exit 函数，否则会导致整个进程退出，取而代之的是调用 pthread_exit 函数，这个函数是使一个线程退出，如果主线程调用 pthread_exit 函数也不会使整个进程退出，不影响其他线程的执行。

* 函数描述：将单个线程退出

* 函数原型：`void pthread_exit(void *retval);`

* 函数参数：retval 表示线程退出状态，通常传入NULL

* 注意：pthread_exit 或者 return 返回的指针所指向的内存单元必须是全局或者是用malloc 分配的，不能在线程函数的栈上分配，因为当其他线程得到这个返回的指针时线程函数已经退出了，栈空间就会被回收。

* 练习：编写 pthread_exit 函数使一个线程退出。

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  
  //线程执行函数
  void *mythread(void *arg)
  {
      printf("child thread, pid==[%d], id==[%d]\n", getpid(), pthread_self());
      //pthread_exit(NULL);
      sleep(100);
  }
  
  int main(int argc,char *argv[])
  {
  	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
      //                     void * (void *start_rountine) (void *), void *arg);
      //创建子线程
      pthread_t thread;
      int ret = pthread_create(&thread, NULL, mythread, NULL);
      if(ret!=0)
      {
          printf("pthread_create error, [%s]\n", strerror(ret));
          return -1;
      }
      printf("main thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  
      //让子线程执行起来
      //pthread_exit(NULL);
      sleep(10);
  
      return 0;
  }
  ```

  通过测试得知，pthread_exit 函数只能使一个线程退出，假如子线程里面调用了 pthread_exit 函数，会使子线程终止；如果主线程调用了pthread_exit函数，并不影响子线程，只是使主线程退出。

## 7、pthread_join 函数

* 函数 描述：阻塞等待线程的退出，获取线程退出的状态，对应进程中的 waitpid()函数。

* 函数原型：`int pthread_join(pthread_t thread, void **retval);`

* 函数参数：

  * thread：线程ID
  * retval：存储线程结束状态，整个指针和 pthread_exit 的参数是同一块内存地址。

* 函数返回值：

  * 成功：0
  * 失败：错误号

* 练习：主线程获取子线程的退出状态

  传参方式：`pthread_t thread; void *ptr;`  `pthread_join(thread, &ptr);`

  ```c
  //线程退出函数测试
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  
  //全局变量 --- 线程的退出状态（结构体类型或int类型等）
  struct Test{
  	int data;
  	char name[64];
  }t;
  int g_var = 9;
  
  //线程执行函数
  void *mythread(void *arg)
  {
      printf("child thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
      //printf("[%p]\n", &g_var);
      //pthread_exit(&g_var);
      printf("[%p]\n",&t);
      memset(&t, 0x00, sizeof(t));
      t.data = 99;
      strcpy(t.name, "maomao");
      pthread_exit(&t);
  }
  
  int main()
  {
  	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
      //                      void *(*start_routine) (void *), void *arg);
  	//创建子线程
  	pthread_t thread;
  	int ret = pthread_create(&thread, NULL, mythread, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}
  	printf("main thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  
  	//回收子线程
  	void *ptr = NULL;
  	pthread_join(thread, &ptr);	
  	//int n = *(int *)ptr;
      //printf("child thread exit status:[%d], [%p]\n", n, ptr);
  	struct Test *p = (struct Test *)ptr;
  	printf("child exit status:[%d],[%s],[%p]\n",  p->data, p->name, ptr);
  
  	return 0;
  }
  ```

## 8、pthread_detach 函数

线程分离状态：指定该状态，线程主动与主控线程断开关系。线程结束后，其退出状态不由其他线程获取，而直接自己自动释放。网络、多线程服务器常用。

进程若有该机制，将不会产生僵尸进程。僵尸进程的产生主要由于进程死后，大部分资源被释放，一点残留资源仍存于系统中，导致内核认为该进程仍存在。

一般情况下，线程终止后，其终止状态一直保留到其它线程调用`pthread_join`获取它的状态为止。但是线程也可以被置为detach状态，这样的线程一旦终止就立刻回收它占用的所有资源，而不保留终止状态。**不能对一个已经处于detach状态的线程调用`pthread_join`**，这样的调用将返回EINVAL错误。也就是说，如果已经对一个线程调用了`pthread_detach`就不能再调用`pthread_join`了。

也可使用 `pthread_create`函数参2(线程属性)来设置线程分离。`pthread_detach`函数是在创建线程之后调用的。

* 函数描述：实现线程分离

* 函数原型：`int pthread_detach(pthread_t thread);`

* 函数返回值：

  * 成功：0
  * 失败：错误号

* 练习：在创建线程之后设置线程的分离状态

  如果线程已经设置了分离状态，则再调用 `pthread_join` 就会失败，可用这个方法验证是否已经成功设置分离状态。

  ```c
  //设置子线程为分离属性
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  
  //线程执行函数
  void *mythread(void *arg)
  {
  	printf("child thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  	sleep(10);
  }
  
  int main()
  {
  	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
      //                      void *(*start_routine) (void *), void *arg);
  	//创建子线程
  	pthread_t thread;
  	int ret = pthread_create(&thread, NULL, mythread, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}
  	printf("main thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  
  	//设置线程为分离属性
  	pthread_detach(thread);
  
  	//子线程设置分离属性,则pthread_join不再阻塞,立刻返回
  	ret = pthread_join(thread, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_join error:[%s]\n", strerror(ret));
  	}
  
  	//目的是为了让子线程能够执行起来
  	sleep(1);
  	return 0;
  }
  ```

## 9、pthread_cancel 函数

* 函数描述：杀死（取消）线程。其作用，对应进程中的kill() 函数。

* 函数原型：`int pthread_cancel(pthread_t thread);`

* 函数返回值：

  * 成功：0
  * 失败：错误号

* 注意：线程的取消并不是实时的，而有一定的延时。需要等待线程到达某个取消点(检查点)。

* 取消点：是线程检查是否被取消，并按请求进行动作的一个位置。通常是一些系统调用`creat`，`open`，`pause`，`close`，`read`，`write.....` 执行命令`man 7 pthreads`可以查看具备这些取消点的系统调用列表。可粗略认为一个系统调用(进入内核)即为一个取消点。还以通过调用`pthread_testcancel`函数设置一个取消点。

  * 函数原型：`void pthread_testcancel(void);`

* 练习：让主线程取消子线程的执行。

  先测试一下没有取消点看看能否使线程取消；然后调用`pthread_testcancel`设置一个取消点，看看能够使线程取消。

  ```c
  //创建子线程
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  
  //线程执行函数
  void *mythread(void *arg)
  {
  	while(1)
  	{
          int a;
          int b;
          sleep(10);
          
          //系统调用进入内核，相当于取消点
          printf("-----\n");
          
          //设置取消点
          //pthread_testcancel();
  	}
  }
  
  int main()
  {
  	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
      //                      void *(*start_routine) (void *), void *arg);
  	//创建子线程
  	pthread_t thread;
  	int ret = pthread_create(&thread, NULL, mythread, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}
  	printf("main thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  
  	//取消子线程
  	pthread_cancel(thread);
  
  	pthread_join(thread, NULL);
  	return 0;
  }
  ```

## 10、pthread_equal 函数

* 函数描述：比较两个线程是否ID是否相等

* 函数原型：`int pthread(pthread_t t1, pthread_t t2);`

* 练习：

  ```c
  //比较线程ID是否相等
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  
  //线程执行函数
  void *mythread(void *arg)
  {
  	printf("child thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  }
  
  int main()
  {
  	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
      //                      void *(*start_routine) (void *), void *arg);
  	//创建子线程
  	pthread_t thread;
  	int ret = pthread_create(&thread, NULL, mythread, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}
  	printf("main thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  
  	//比较线程ID
  	//if(pthread_equal(thread, pthread_self())!=0)
  	if(pthread_equal(pthread_self(), pthread_self())!=0)
  	{
  		printf("two thread id is same\n");
  	}
  	else
  	{
  		printf("two thread id is not same\n");
  	}
  
  	//目的是为了让子线程能够执行起来
  	sleep(1);
  	return 0;
  }
  ```

## 11、进程和线程的比较

|     进程     |      线程      |
| :----------: | :------------: |
|     fork     | pthread_create |
|     exit     |  pthread_exit  |
| wait/waitpid |  pthread_join  |
|     kill     | pthread_cancel |
|    getpid    |  pthread_self  |
|   perrror    |    strerror    |

# 线程属性

linux下线程的属性是可以根据实际项目需要，进行设置，之前讨论的线程都是采用线程的默认属性，默认属性已经可以解决绝大多数开发时遇到的问题，如果对程序的性能提出更高的要求，则需要设置线程属性。

* 线程的分离状态决定一个线程以什么样的方式来终止自己，有两种状态：

  * 非分离状态：线程的默认属性是非分离状态，这种情况下，原有的线程等待创建的线程结束。只有当`pthread_join()`函数返回时，创建的线程才算终止，才能释放自己占用的系统资源。
  * 分离状态：分离线程没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放系统资源。应该根据自己的需要，选择适当的分离状态。

* 设置线程属性分为以下步骤：

  * 第1步：定义线程属性类型类型的变量

    `pthread_attr_t attr;`

  * 第2步：对线程属性变量进行初始化

    `int pthread_attr_init(pthread_attr_t *attr);`

  * 第3步：设置线程为分离属性

    `int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);`

    * 参数 attr：线程属性
    * 参数 detachstate：
      * `PTHREAD_CREATE_DETACHED`（分离）
      * `PTHREAD_CREATE_JOINABLE` （非分离）

    注意：这一步完成之后调用`pthread_create`函数创建线程，则创建出来的线程就是分离线程；其实上述三步就是`pthread_create`的第二个参数做准备工作。

  * 第4步：释放线程属性资源

    `int pthread_attr_destroy(pthread_attr_t *attr);`

* 练习：创建一个分离属性的线程。

  验证：设置为分离属性的线程是不能够被`pthread_join`函数回收的，

  可以通过调用`pthread_join`函数测试该线程是否已经是分离属性的线程。

  ```c
  //在创建子线程的时候设置分离属性
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  
  //线程执行函数
  void *mythread(void *arg)
  {
  	printf("child thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  	sleep(2);
  }
  
  int main()
  {
  	//定义pthread_attr_t类型的变量
  	pthread_attr_t attr;
  	
  	//初始化attr变量
  	pthread_attr_init(&attr);
  
  	//设置attr为分离属性
  	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  
  	//创建子线程
  	pthread_t thread;
  	int ret = pthread_create(&thread, &attr, mythread, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}
  	printf("main thread, pid==[%d], id==[%ld]\n", getpid(), pthread_self());
  
  	//释放线程属性
  	pthread_attr_destroy(&attr);
  
  	//验证子线程是否为分离属性
  	ret = pthread_join(thread, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_join error:[%s]\n", strerror(ret));
  	}
  
  	return 0;
  }
  ```

# 线程同步

## 1、线程同步的概念

线程同步，指一个线程发出某一功能调用时，在没有得到结果之前，该调用不返回。同时其它线程为保证数据一致性，不能调用该功能。

## 2、线程同步的例子

创建两个线程，让两个线程共享一个全局变量int number， 然后让每个线程数5000次数，看最后打印出这个number值是多少？

* 线程A代码片段

  ```c
  for(i=0; i<5000; i++)
  {
      tmp = number;
      tmp++;
      number = tmp;
      printf("[A]:[%d]\n",number);
  }
  ```

* 线程B代码片段

  ```c
  for(i=0; i<5000; i++)
  {
      tmp = number;
      tmp++;
      number = tmp;
      printf("[B]:[%d]\n",number);
  }
  ```

* 代码片段说明：

  * 对number执行++操作，使用了中间变量tmp是为了尽可能的模拟cpu时间片用完而让出cpu的情况。

* 测试结果：经过多次测试最后的结果显示，有可能会出现number值少于5000*2=10000的情况。

* 分析原因：假如子线程A执行完了tmp++操作，还没有将tmp的值赋值给number失去了cpu的执行权，子线程B得到了cpu执行权，而子线程B最后执行完了number=tmp，而后失去了cpu的执行权；此时子线程A又重新得到cpu的执行权，并执行number=ptr操作，这样会把线程B刚刚写回number的值被覆盖了，造成number值不符合预期的值。

  <img src="/两个线程数数问题分析.png" alt="两个线程数数问题分析" style="zoom: 67%;" />

* 数据混乱的原因：

  * 资源共享（独享资源则不会）--- 全局变量
  * 调度随机（线程操作共享资源的先后顺序不确定）
  * 线程间缺乏必要的同步机制。

  以上3点中，前两点不能改变，欲提高效率，传递数据，资源必须共享。只要共享资源，就一定会出现竞争。只要存在竞争关系，数据就很容易出现混乱。所以只能从第三点着手解决。使多个线程在访问共享资源的时候，出现互斥。

* 解决：

  * 原子操作：指的是该操作要么不做，要么就完成。
  * 使用互斥锁解决同步问题：其实是模拟原子操作

* 互斥锁：

  <img src="/互斥锁.png" alt="互斥锁" style="zoom:75%;" />

  * Linux中提供一把互斥锁mutex（也称之为互斥量）。每个线程在对资源操作前都尝试先加锁，成功加锁才能操作，操作结束解锁。
  * 资源还是共享的，线程间也还是竞争的，但通过“锁”就将资源的访问变成互斥操作，而后与时间有关的错误也不会再产生了。
  * 线程1访问共享资源的时候要先判断锁是否锁着，如果锁着就阻塞等待；若锁是解开的就将这把锁加锁，此时可以访问共享资源，访问完成后释放锁，这样其他线程就有机会获得锁。
  * 应该注意：图中同一时刻，只能有一个线程持有该锁，只要该线程未完成操作就不释放锁。
  * 使用互斥锁之后，两个线程由并行操作变成了串行操作，效率降低了，但是数据不一致的问题得到解决了。

## 3、互斥锁相关函数

* `pthread_mutex_t` 类型

  * 其本质是一个结构体，简单当成整数看待
  * `pthread_mutex_t mutex;` 变量mutex 只有两种取值 1、0

* `pthread_mutex_init` 函数

  * 函数描述：初始化一个互斥锁  ----> 初值可看作 1

  * 函数原型：`int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);`

  * 函数参数：

    * mutex：传出参数，调用时应传入 &mutex

    * attr：互斥锁属性。是一个传入参数，通常NULL，选用默认属性（线程间共享）

    * restrict 关键字：只用于限制指针，告诉编译器，所有修改该指针指向内存中内容的操作，只能通过本指针完成。不能通过除本指针以外的其他变量或指针修改互斥量mutex的两种初始化方式：

      * 静态初始化：如果互斥锁 mutex 是静态分配的（定义在全局，或加了static关键字修饰），可以直接使用宏进行初始化。

        `pthead_mutex_t muetx = PTHREAD_MUTEX_INITIALIZER;`

      * 动态初始化：局部变量应采用动态初始化。

        `pthread_mutex_init(&mutex, NULL);`

* `pthread_mutex_destroy` 函数

  * 函数描述：销毁一个互斥锁
  * 函数原型：`int pthread_mutex_destroy(pthread_mutex_t *mutex);`
  * 函数参数：mutex --- 互斥锁变量

* `pthread_mutex_lock` 函数

  * 函数描述：对互斥锁加锁
  * 函数原型：`int pthread_mutex_lock(pthread_mutex_t *mutex);`
  * 函数参数：mutex --- 互斥锁变量

* `pthread_mutex_unlock` 函数

  * 函数描述：对互斥锁解锁
  * 函数原型：`int pthread_nutex_unlock(pthread_mutex_t *mutex);`
  * 函数参数：mutex --- 互斥锁变量

* `pthread_mutex_trylock` 函数

  * 函数描述：尝试加锁
  * 函数原型：`int pthread_mutex_trylock(pthread_mutex_t *mutex);`
  * 函数参数：mutex --- 互斥锁变量

## 4、加锁和解锁

互斥锁的使用步骤：

* 第一步：创建一把锁`pthread_mutex_t mutex;`

* 第二步：在 main 函数里初始化互斥锁`pthread_mutex_init(&mutex, NULL);

* 第三步：在共享资源出现的位置上下加锁、解锁

  `pthread_mutex_lock(&mutex);`  `pthread_mutex_unlock(&mutex);`

* 第四步：在 main 函数中释放互斥锁 `pthread_mutex_destroy(&mutex);`

注意：

* lock尝试加锁，如果加锁不成功，线程阻塞，阻塞到持有该互斥量的其他线程解锁为止。
* unlock主动解锁函数，同时将阻塞在该锁上的所有线程全部唤醒，至于哪个线程先被唤醒，取决于优先级、调度。默认：先阻塞、先唤醒。

练习：使用互斥锁解决两个线程数数不一致的问题。

* 代码片段：在访问共享资源前加锁，访问结束后立即解锁。

* 线程A代码段

  ```c
  for(i=0; i<5000; i++)
  {
      //加锁
      pthread_mutex_lock(&mutex);
      
      tmp = number;
      tmp++;
      number = tmp;
      printf("[A]:[%d]\n", number);
      
      //解锁
      pthread_mutex_unlock(&mutex);
  }
  ```

* 线程B代码段

  ```c
  for(i=0; i<5000; i++)
  {
      //加锁
      pthread_mutex_lock(&mutex);
      
      tmp = number;
      tmp++;
      number = tmp;
      printf("[B]:[%d]\n", number);
      
      //解锁
      pthread_mutex_unlock(&mutex);
  }
  ```

* 总结：使用互斥锁之后，两个线程由并行变为了串行，效率降低了，但是可以使两个线程同步操作共享资源，从而解决了数据不一致的问题。

* 注意：必须在所有操作共享资源的线程上都加上锁否则不能起到同步的效果。

## 5、死锁

死锁并不是 linux 提供给用户的一种使用方法，而是由于**用户使用互斥锁不当引起的一种现象**。

常见的死锁有两种：

* 第一种：自己锁自己，如下代码段

  ```c
  void *mythread1(void *args)
  {
      while(1)
      {
          //加锁
          pthread_mutex_lock(&mutex);
          pthread_mutex_lock(&mutex);
          
          printf("hello");
          sleep(rand()%3);
          printf("world\n");
          
          //解锁
          pthread_mutex_unlock(&mutex);
          sleep(rand()%3);
      }
      pthread_exit(NULL);
  }
  ```

* 第二种：线程A拥有A锁，请求获得B锁；线程B拥有B锁，请求获得A锁，这样造成线程A和线程B都不释放自己的锁，而且还想得到对方的锁，从而产生死锁

  <img src="/死锁.png" alt="死锁" style="zoom:75%;" />

如何解决死锁：

* 让线程按照一定的顺序去访问共享资源
* 在访问其他锁的时候，需要先将自己的锁解开
* 调用`pthread_mutex_trylock`，如果加锁不成功会立刻返回

## 6、读写锁

* 什么是读写锁：

  读写锁也叫共享-独占锁。当读写锁以读模式锁住时，它是以共享模式锁住的；当它以写模式锁住时，它是以独占模式锁住的。**写独占、读共享**

* 读写锁使用场合：

  读写锁非常适合于对数据结构读的次数远大于写的情况。

* 读写锁特征：

  * 读写锁是“写模式加锁”时，解锁前，所有对该锁加锁的线程都会被阻塞。
  * 读写锁是“读模式加锁”时，如果线程以读模式对其加锁会成功；如果线程以写模式加锁会阻塞。
  * 读写锁是“读模式加锁”时， 既有试图以写模式加锁的线程，也有试图以读模式加锁的线程。那么读写锁会阻塞随后的读模式锁请求。优先满足写模式锁。**读锁、写锁并行阻塞，写锁优先级高**

* 读写锁场景练习：

  * 线程 A 加写锁成功, 线程B请求读锁
    * 线程 B 阻塞
    * 当线程 A 解锁后，线程 B 加锁成功
  * 线程 A 持有读锁，线程 B 请求写锁
    * 线程 B 阻塞
    * 当线程 A 解锁后，线程 B 加锁成功
  * 线程 A 拥有读锁，线程 B 请求读锁
    * 线程 B 加锁成功
  * 线程 A 持有读锁，然后线程 B 请求写锁，然后线程 C 请求读锁
    * 线程 B 不阻塞，线程 C 阻塞  --- **写的优先级高**
    * 线程 A 解锁，线程 B 加写锁成功，线程 C 继续阻塞
    * 线程 B 解锁，线程 C加锁成功
  * 线程 A 持有写锁, 然后线程 B 请求读锁, 然后线程 C 请求写锁
    * 线程 B 阻塞，线程 C 阻塞
    * 线程 A 解锁，线程 C 加写锁成功，线程 B 继续阻塞 ---  **写的优先级高**
    * 线程 C 解锁， 线程 B 加锁成功

* 读写锁主要操作函数：

  * 定义一把读写锁：`pthread_rwlock_t rwlock;`
  * 初始化读写锁：`int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlock_t *restrict attr);`
    * 函数参数：
      * rwlock：读写锁
      * attr：读写锁属性，传NULL为默认属性
  * 销毁读写锁：`int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);`
  * 加读锁：`int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);`
  * 尝试加读锁：`int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);`
  * 加写锁：`int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);`
  * 尝试加写锁：`int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);`
  * 解锁：`int pthread_rwlock_unlock(&pthread_rwlock_t *rwlock);`

* 练习：3个线程不定时写同一全局资源，5个线程不定时读同一全局资源。

  ```c
  //读写锁测试程序
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  
  //全局变量
  int number = 0;
  
  //定义一把读写锁
  pthread_rwlock_t rwlock;
  
  //写线程回调函数
  void *thread_write(void *arg)
  {
  	int i = *(int *)arg;
  	int cur;
  	while(1)
  	{
  		//加写锁
  		pthread_rwlock_wrlock(&rwlock);
  
  		cur = number;
  		cur++;
  		number = cur;	
  		printf("[%d]-W:[%d]\n", i, cur);
  
  		//解锁
  		pthread_rwlock_unlock(&rwlock);
  		sleep(rand()%3);
  	}
      pthread_exit(NULL);
  }
  
  //读线程回调函数
  void *thread_read(void *arg)
  {
  	int i = *(int *)arg;
  	int cur;
  	while(1)
  	{
  		//加读锁
  		pthread_rwlock_rdlock(&rwlock);
  
  		cur = number;
  		printf("[%d]-R:[%d]\n", i, cur);
  
  		//解锁
  		pthread_rwlock_unlock(&rwlock);
  		sleep(rand()%3);
  	}
      pthread_exit(NULL);
  }
  
  int main()
  {
  	int i = 0;
  	int arr[8];
  	pthread_t thread[8];
  
  	//读写锁初始化
  	pthread_rwlock_init(&rwlock, NULL);
  
  	//创建3个写子线程
  	for(i=0; i<3; i++)
  	{
  		arr[i] = i;
  		pthread_create(&thread[i], NULL, thread_write, &arr[i]);
  	}
  
  	//创建5个读子线程
  	for(i=3; i<n; i++)
  	{
  		arr[i] = i;
  		pthread_create(&thread[i], NULL, thread_read, &arr[i]);
  	}
  
  	//回收子线程
  	int j = 0;
  	for(j=0;j<n; j++)
  	{
  		pthread_join(thread[j], NULL);
  	}
  
  	//释放锁
  	pthread_rwlock_destroy(&rwlock);
  
  	return 0;
  }
  ```

## 7、条件变量

* 条件本身不是锁，但它也可以造成线程阻塞。通常与互斥锁配合使用。给多线程提供一个会合的场所。

  * 使用互斥量保护共享数据;
  * 使用条件变量可以使线程阻塞, 等待某个条件的发生, 当条件满足的时候解除阻塞.

* 条件变量的两个动作:

  * 条件不满足, 阻塞线程
  * 条件满足, 通知阻塞的线程解除阻塞, 开始工作.

* 条件变量相关函数：

  * 定义一个条件变量：`pthread_cond_t cond;`

  * 初始化条件变量：`int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_cond_t *restrict attr);`

    * 参数 cond：条件变量
    * 参数 attr：条件变量的属性，通常传NULL
    * 成功返回0、失败返回错误号

  * 销毁条件变量：`int pthread_cond_destroy(pthread_cond_t *cond);`

  * `int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);`

    * 函数描述：

      条件不满足，引起线程阻塞并解锁；

      条件满足，解除阻塞，并加锁

    * 函数参数：

      cond：条件变量

      mutex：互斥锁变量

  * `int pthread_cond_signal(pthread_cond_t *cond);`

    * 函数描述：唤醒至少一个阻塞在该条件变量上的线程
    * 函数参数：条件变量
    * 返回值：成功0、失败错误号

* 使用条件变量的代码段

  <img src="/使用条件变量实现生产者和消费者.png" alt="使用条件变量实现生产者和消费者" style="zoom:67%;" />
  
  上述代码中，生产者线程调用`pthread_cond_signal`函数会使消费者线程在`pthread_cond_wait`处解除阻塞。
  
* 使用条件变量实现生产者和消费者

  ```c
  //使用条件变量实现生产者和消费者模型
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  typedef struct node
  {
  	int data;
  	struct node *next;
  }NODE;
  NODE *head = NULL;
  
  //定义一把锁
  pthread_mutex_t mutex;
  
  //定义条件变量
  pthread_cond_t cond;
  
  //生产者线程
  void *producer(void *arg)
  {
  	NODE *pNode = NULL;
  	while(1)
  	{
  		//生产一个节点
		pNode = (NODE *)malloc(sizeof(NODE));
  		if(pNode==NULL)
  		{
  			perror("malloc error");
  			exit(-1);
  		}
  		pNode->data = rand()%1000;
  		printf("P:[%d]\n", pNode->data);
  
  		//加锁
  		pthread_mutex_lock(&mutex);
  
  		pNode->next = head;
  		head = pNode;
  
  		//解锁
  		pthread_mutex_unlock(&mutex);
  
  		//通知消费者线程解除阻塞
  		pthread_cond_signal(&cond);
  		
  		sleep(rand()%3);
  	}
  }
  
  
  ```

//消费者线程
  void *consumer(void *arg)
  {
  	NODE *pNode = NULL;
  	while(1)
  	{
  		//加锁
          pthread_mutex_lock(&mutex);
  		
  		if(head==NULL)
  		{
  			//若条件不满足,需要阻塞等待
  			//若条件不满足,则阻塞等待并解锁;
  			//若条件满足(被生成者线程调用pthread_cond_signal函数通知),解除阻塞并加锁 
  			pthread_cond_wait(&cond, &mutex);
  		}

  		printf("C:[%d]\n", head->data);	
  		pNode = head;
  		head = head->next;

  		//解锁
  		pthread_mutex_unlock(&mutex);

  		free(pNode);
  		pNode = NULL;

  		sleep(rand()%3);
  	}
  }

  int main()
  {
  	int ret;
  	pthread_t thread1;
  	pthread_t thread2;

  	//初始化互斥锁
  	pthread_mutex_init(&mutex, NULL);

  	//条件变量初始化
  	pthread_cond_init(&cond, NULL);

  	//创建生产者线程
  	ret = pthread_create(&thread1, NULL, producer, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}

  	//创建消费者线程
  	ret = pthread_create(&thread2, NULL, consumer, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}

  	//等待线程结束
  	pthread_join(thread1, NULL);
  	pthread_join(thread2, NULL);

  	//释放互斥锁
  	pthread_mutex_destroy(&mutex);

  	//释放条件变量
  	pthread_cond_destroy(&cond);

  	return 0;
  }

## 8、信号量

* 信号量介绍：信号量相当于多把锁, 可以理解为是加强版的互斥锁

* 信号量相关函数：

  * 定义信号量：`sem_t sem;`
  * `int sem_init(sem_t *sem, int pshared, unsigned int value);`
    * 函数描述：初始化信号量
    * 函数参数：
      * sem：信号量变量
      * pshared：0表示线程同步，1表示进程同步
      * value：最多有几个线程操作共享数据
    * 函数返回值：成功0、失败-1，并设置errno值
  * `int sem_wait(sem_t *sem);`
    * 函数描述：调用该函数一次, 相当于sem--, 当sem为0的时候, 引起阻塞
    * 函数参数：信号变量
    * 函数返回值：成功0、失败-1，并设置errno值
  * `int sem_post(sem_t *sem);`
    * 函数描述：调用一次, 相当于sem++
    * 函数参数：信号变量
    * 函数返回值：成功0、失败-1，并设置errno值
  * `int sem_trywait(sem_t *sem);`
    * 函数描述：尝试加锁, 若失败直接返回, 不阻塞
    * 函数参数：信号变量
    * 函数返回值：成功0、失败-1，并设置errno值
  * `int sem_destroy(sem_t *sem);`
    * 函数描述：销毁信号量
    * 函数参数：信号变量
    * 函数返回值：成功0、失败-1，并设置errno值

* 信号量代码片段：

  <img src="/使用信号量实现生产者和消费者模型.png" alt="使用信号量实现生产者和消费者模型" style="zoom:67%;" />
  
* 使用信号量实现生产者和消费者模型

  ```c
  //使用信号量实现生产者和消费者模型
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <pthread.h>
  #include <semaphore.h>
  typedef struct node
  {
  	int data;
  	struct node *next;
  }NODE;
  
  NODE *head = NULL;
  
  //定义信号量
  sem_t sem_producer;
  sem_t sem_consumer;
  
  //生产者线程
  void *producer(void *arg)
  {
  	NODE *pNode = NULL;
  	while(1)
  	{
  		//生产一个节点
  		pNode = (NODE *)malloc(sizeof(NODE));
  		if(pNode==NULL)
  		{
  			perror("malloc error");
  			exit(-1);
  		}
  		pNode->data = rand()%1000;
  		printf("P:[%d]\n", pNode->data);
  
  		//加锁
  		sem_wait(&sem_producer); //--
  
  		pNode->next = head;
  		head = pNode;
  
  		//解锁
  		sem_post(&sem_consumer);  //相当于++
  
  		sleep(rand()%3);
  	}
  }
  
  //消费者线程
  void *consumer(void *arg)
  {
  	NODE *pNode = NULL;
  	while(1)
  	{
  		//加锁
  		sem_wait(&sem_consumer); //相当于--
  		
  		printf("C:[%d]\n", head->data);	
  		pNode = head;
  		head = head->next;
  
  		//解锁
  		sem_post(&sem_producer); //相当于++
  
  		free(pNode);
  		pNode = NULL;
  
  		sleep(rand()%3);
  	}
  }
  
  int main()
  {
  	int ret;
  	pthread_t thread1;
  	pthread_t thread2;
  
  	//初始化信号量
  	sem_init(&sem_producer, 0, 5);
  	sem_init(&sem_consumer, 0, 0);
  
  	//创建生产者线程
  	ret = pthread_create(&thread1, NULL, producer, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}
  
  	//创建消费者线程
  	ret = pthread_create(&thread2, NULL, consumer, NULL);
  	if(ret!=0)
  	{
  		printf("pthread_create error, [%s]\n", strerror(ret));
  		return -1;
  	}
  
  	//等待线程结束
  	pthread_join(thread1, NULL);
  	pthread_join(thread2, NULL);
  
  	//释放信号量资源
  	sem_destroy(&sem_producer);
  	sem_destroy(&sem_consumer);
  
  	return 0;
  }
  ```



  

