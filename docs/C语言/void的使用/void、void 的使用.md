# void的使用

## 1、对函数返回值的限定---函数无返回值

```c
void test（int a)
{
    int num+=a；
}
```

## 2、void * 限定函数的返回值为任意类型的指针

```c
void * test()
{
	int a=5；
	int *p=&a；
	return p；
}
```



## 3、对函数参数的限定---函数无参数

```c
int test(void)
{
	return 5+3;
}
```

## 4、void * 限定函数参数为任意类型的指针

```c
void test(void * p)
{
    int *a=p;
}
```

## 5、void

void为“不确定类型”，不可以用void来声明变量

```c
void a=20；   //错误
```



## 6、void  * 

void  * 为万能指针或泛型指针

**注意：**

### 1、void * 类型的指针不可以解引用  

### 2、void * 可以接受任何类型的赋值：

任何类型的指针都可以直接赋值给`void *`型指针，无需进行强制类型转换，相当于`void *`包含了其他类型的指针。

```c
void * p= int *a；
```

void * 可以赋值给任何类型的变量
但是需要进行强制转换，应为`void *`的范围较大，所以强制转换，使其进行范围缩小。

```c
int a =5;
void *p=&a；
int *q=(int *)p;
```

