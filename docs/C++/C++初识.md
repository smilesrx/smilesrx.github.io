# 简单的C++程序

```c++
#include <iostream>
using namespace std;
int main()
{
    cout<<"Hello World"<<endl;
    system("pause");
    return EXIT_SUCCESS;
}
```

分析：

* `#include <iostream>`; 预编译指令，引入头文件`iostream`。
* `using namespace std`; 使用标准命名空间。
* `cout << “hello world”<< endl;` 和`printf`功能一样，输出字符串`”hello wrold”`。

## 1、面向过程

面向过程是一种以过程为中心的编程思想。  

通过分析出解决问题所需要的步骤，然后用函数把这些步骤一步一步实现，使用的时候一个一个依次调用就可以了。  

面向过程编程思想的核心：功能分解，自顶向下，逐层细化（程序=数据结构+算法）。  

面向过程编程语言存在的主要缺点：不符合人的思维习惯，而是要用计算机的思维方式去处理问题，而且面向过程编程语言重用性低，维护困难。

## 2、面向对象

**面向对象编程**（Object-Oriented Programming）简称 OOP 技术，是开发计算机应用程序的一种新方法、新思想。

过去的面向过程编程常常会导致所有的代码都包含在几个模块中，使程序难以阅读和维护。在做一些修改时常常牵一动百，使以后的开发和维护难以为继  

使用 OOP 技术，常常要使用许多代码模块，每个模块都只提供特定的功能，它们是彼此独立的，这样就增大了代码重用的几率，更加有利于软件的开发、维护和升级。

## 3、面向对象的三大特性

* 封装

  把客观事物封装成抽象的类，并且类可以把自己的数据和方法只让可信的类或者对象操作，对不可信的进行信息隐藏。  

  类将成员变量和成员函数封装在类的内部，根据需要设置访问权限，通过成员函数管理内部状态。

* 继承

  继承所表达的是类之间相关的关系，这种关系使得对象可以继承另外一类对象的特征和能力。  

  继承的作用：避免公用代码的重复开发，减少代码和数据冗余。

* 多态

  多态性可以简单地概括为“一个接口，多种方法”，字面意思为多种形态。程序在运行时才决定调用的函数，它是面向对象编程领域的核心概念。  

# C++对C的扩展

## 1、::作用域运算符

如果前面什么都不添加 代表全局作用域

通常情况下，如果有两个同名变量，一个是全局变量，另一个是局部变量，那么局部变量在其作用域内具有较高的优先权，它将屏蔽全局变量。

```c++
int m=100;  //全局变量
void test()
{
    int m=50;   //局部变量
    cout<<"m="<<m<<endl;       //输出结果为:m=50
}
```

作用域运算符`::`可解决局部变量与全局变量的重名问题，即在局部变量的作用域内，可用`::`对被屏蔽的同名的全局变量进行访问。

```c++
int a=60;    //全局变量
void test()
{
    int a=40;    //局部变量
    cout<<"a="<<::a<<endl;     //::a 引用全局的变量a  输出结果：a=60
}
```

## 2、C++命名空间（namespace）

名称（name）可以是符号常量、变量、函数、结构、枚举、类和对象等等  

名称互相冲突性的可能性越大

可以更好地控制标识符的作用域

* 命名空间用途：解决名称冲突
* 命名空间下可以存放 ： 变量、函数、结构体、类…
* 命名空间必须要声明在全局作用域
* 命名空间可以嵌套命名空间
* 命名空间是开放的，可以随时将新成员添加到命名空间下
* 命名空间可以匿名的
* 命名空间可以起别名

### 1、命名空间的使用语法：

1. 创建一个命名空间

   ```c++
   namespace A
   {
       int a=15;
   }
   namespace B
   {
       int a=20;
   }
   void test()
   {
       cout<<"A::a="A::a<<endl;    //输出结果：A::a=15
       cout<<"B::a="B::a<<endl;    //输出结果：B::a=20
   }
   ```

2. 命名空间只能创建在全局范围（以下示例错误）

   ```c++
   void test()
   {
       namespace A
       {
           int m=16;
       }
       namespace B
       {
           int m=30;
       }
       cout<<"A::m="<<A::m<<endl;
       cout<<"B::m="<<B::m<<endl;
   }
   ```

3. 命名空间可以嵌套命名空间

   ```c++
   namespace A
   {
       int k=20;
       namespace B
       {
           int k=45;
       }
   }
   void test()
   {
       cout<<"A::k="<<A::k<<endl;      //运行结果：A::k=20
       cout<<"A::B::k="<<A::B::k<<endl;   //结果：A::B::k=45
   }
   ```

4. 命名空间是开放的，即可以随时把新的成员加入已有的命名空间中

   ```c++
   namespace A
   {
       int m=50;
   }
   namespace A
   {
       int n=75;
   }
   void test()
   {
       cout<<"A::m="<<A::m<<endl;    //结果：A::m=50
       cout<<"A::n="<<A::n<<endl;    //结果：A::n=75
   }
   ```

5. 声明和实现可分离

   ```c++
   namespace A
   {
       //申明
       int add(int a,int b);
       int sub(int a,int b);
   }
   //实现
   int A::add(int a,int b)
   {
       return a+b;
   }
   int A::sub(int a,int b)
   {
       return a-b;
   }
   void test()
   {
       //调用
       cout<<A::add(5,3)<<endl;    //结果：8
       cout<<A::sub(5,3)<<endl;    //结果：2
   }
   ```

6. 匿名命名空间

   意味着命名空间中的标识符只能在本文件内访问，相当于给这个标识符加上了`static`，使得其可以作为内部连接

   ```c++
   namespace 
   {
       int a=50;
       void swap(int &m,int &n)
       {
           int temp=m;
           m=n;
           n=temp；
       }
   }
   void test()
   {
       cout<<"a="a<<<<endl;     //结果：a=50
       int m=3;
       int n=5;
       cout<<"交换前m="<<m<<endl;   //结果：交换前m=3 
       cout<<"交换前n="<<n<<endl;   //结果：交换前n=5
       swap(m,n);
       cout<<"交换后m="<<m<<endl;   //结果：交换后m=3
       cout<<"交换后n="<<n<<endl;   //结果：交换后n=5
   }
   ```

7. 命名空间别名

   ```c++
   namespace Longnamespace
   {
       int m=15;
   }
   void test()
   {
       namespace shortnamespace=Longnamespace;
       cout<<"Longnamespace::m="<<Longnamespace::m<<endl;     //结果：Longnamespace::m=15 
       cout<<"shortnamespace::m="<<shortnamespace::m<<endl;   //结果：shortnamespace::m=15  
   }
   ```

### 2、using申明

1. using声明可使得指定的标识符可用。

   注意：当using声明与 就近原则同时出现，出错，尽量避免

   ```c++
   namespace A
   {
       int m=15;
       int n=20;
       void funcA(){cout<<"funcA函数调用"<<endl;}
       void funcB(){cout<<"funcB函数调用"<<endl;}
   }
   void test()
   {
       //1.通过命名空间作用域运算符访问
       cout<<"A::m="<<A::m<<endl;    //结果：A::m=15
       A::funcA();                   //结果：funcA函数调用
       //2.通using申明访问
       //int n=30;  //using声明与 就近原则同时出现，出错
       using A::n;
       using A::funcB;
       cout<<"n="<<n<<endl;          //结果：n=15
       funcB();                      //结果：funcB函数调用
   }
   ```

2. using申明碰到函数重载

   如果命名空间包含一组用相同名字重载的函数，using声明就声明了这个重载函数的所有集合。

   ```c++
   namespace A
   {
       void funcA(){cout<<"无参调用"<<endl;}
       void funcA(int a){cout<<"传值有参调用"<<endl;}
       void funcA(int *a){cout<<"传址有参调用"<<endl;}
       void funcA(int &a,int){cout<<"传引用有参调用"<<endl;}
   }
   void test()
   {
       //using申明
       using A::funcA;
       int a=5;
       funcA();       //结果：无参调用
       funcA(a);      //结果：传值有参调用
       funcA(&a);     //结果：传址有参调用
       funcA(a,5);    //结果：传引用有参调用
   }
   ```

### 3、using编译指令

using编译指令使整个命名空间标识符可用。

```c++
namespace A{
	int a = 20;
	int b = 30;
	void funcA(){ cout << "hello funcA" << endl; }
	void funcB(){ cout << "hello funcB" << endl; }
}

void test01()
{
    //using 编译指令
	using namespace A;
	cout << "a="<<a << endl;
	cout << "b="<<b << endl;
	funcA();
	funcB();

	//不会产生二义性
	int paramA = 30;
	cout << paramA << endl;     //就近原则
}

namespace B{
	int a = 20;
	int b = 30;
	void funcA(){ cout << "hello funcA" << endl; }
	void funcB(){ cout << "hello funcB" << endl; }
}

void test02(){
	using namespace A;
	using namespace B;
	//二义性产生，不知道调用A还是B的paramA
	//cout << "a="<<a << endl;
    cout<<"A::a="<<A::a<<endl;
}
```

注意：

* 当using编译指令  与  就近原则同时出现，优先使用就近
* 当using编译指令有多个，需要加作用域 区分

## 3、严格的类型转换

在C++，不同类型的变量一般是不能直接赋值的，需要相应的强转。

```c++
//c代码
int *p=malloc(sizeof(int)*4);

//c++代码
int *p=(int *)malloc(sizeof(int)*4)；
```

## 4、struct类型增强

* c中定义结构体变量需要加上`struct`关键字，c++不需要。
* c中的结构体只能定义成员变量，不能定义成员函数。c++即可以定义成员变量，也可以定义成员函数。

```c++
//1. 结构体中即可以定义成员变量，也可以定义成员函数
struct Student{
	string mName;
	int mAge;
	void setName(string name){ mName = name; }
	void setAge(int age){ mAge = age; }
	void showStudent()
    {
		cout << "Name:" << mName << " Age:" << mAge << endl;
	}
};

//2. c++中定义结构体变量不需要加struct关键字
void test01(){
	Student student;
	student.setName("John");
	student.setAge(20);
	student.showStudent();
}
```

## 5、三木运算符增强

1. c语言三目运算表达式返回值为数据值，为右值，不能赋值。

   ```c
   int a=5;
   int b=10;
   printf("%d\n",a>b?a:b);     //结果：10
   //a>b?a:b   返回的是b的值（10）---右值（不可当做左值来被赋值）
   (a>b?a:b)=50;   //错误
   ```

2. c++语言三目运算表达式返回值为变量本身(引用)，为左值，可以赋值。

   ```c++
   int a=5;
   int b=10;
   printf("%d\n",a>b?a:b);    //结果：10
   int a=5;
   //a>b?a:b   返回的是b变量---左值（可以被赋值）
   (a>b?a:b)=50;
   cout<<"b="<<b<<endl;
   ```

   

## 6、bool类型

* `bool`类型只有两个值，true(1值)，false(0值)
* `bool`类型占1个字节大小
* 给`bool`类型赋值时，非0值会自动转换为true(1),0值会自动转换false(0)

## 7、const增强

`const`单词字面意思为常数，不变的

### 1、C语言下`const`

认为`const`应该是一个只读变量，既然是变量那么就会给`const`分配内存，并且在c中`const`是一个全局只读变量，c语言中`const`修饰的只读变量是外部连接的。

* 全局`const`   //受常量区的保护

  ```c
  const int a=15;   //全局
  //1、直接修改
  a=20;    //错误
  //2、间接修改
  int *p=&a;
  *p=20;   //编译通过，运行失败
  ```

* 局部`const`

  ```c
  void tset()
  {
      const int a=15;   //局部  分配到栈上
      //1、直接修改
      a=20;   //失败
      //2、间接修改
      int *p=&a;
      *p=20;  //成功
      printf("a=%d\n",a);
  }
  ```

  ​	`int arr[a]`; 在C语言下 `const`是伪常量，不可以初始化数组

* c语言下`const`修饰全局变量默认为外部链接属性

  ```c
  //外部文件下
  const int g_a = 1000;    //默认是外部链接属性
  
  void test()
  {
      extern const int g_a; 
  	printf("g_a = %d\n", g_a);
  }
  
  ```

  

### 2、c++下的`const`

在c++中，是否为`const`常量分配内存空间依赖于如何使用

取一个`const`地址, 或者把它定义为`extern`,则会为该`const`创建内存空间

* 全局`const`   //受常量区的保护

  ```c++
  const int a=15;   //全局
  //1、直接修改
  a=20;    //错误
  //2、间接修改
  int *p=&a;
  *p=20;   //编译通过，运行失败
  ```

* 局部`const`

  ```c++
  const int b=100;
  //1、直接修改
  b=50;   //失败
  //2、间接修改
  int *p=(int *)&b;
  *p=50;    //失败
  cout<<"b="<<b<<endl;   //结果：b=100
  cout<<"*p="<<*p<<endl;  //结果：*p=50
  ```

  `int arr[b;`   //C++下`const`修饰的变量 称为常量 ，可以初始化数组  

  

* 在c++中，出现在所有函数之外的`const`作用于整个文件(也就是说它在该文件外不可见)，默认为内部连接，c++中其他的标识符一般默认为外部连接。

  ```c++
  //外部文件下
  extern const int g_a = 1000;  //默认是内部链接属性 可以加关键字 extern 提高作用域
  
  void test()
  {
      extern const int g_a; 
  	printf("g_a = %d\n", g_a);
  }
  ```

# C++下 const分配内存的情况

## 1、对const变量 取地址 ，会分配临时内存

```c++
void test01()
{
	const int a = 10;
	int * p = (int *)&a;
}
```

## 2、使用普通变量  初始化 const变量

```c++
void test02()
{
	int a = 10;
	const int b = a;

	int *p = (int *)&b;
	*p = 1000;
	cout << "b = " << b << endl;
    cout << "*p = " << *p << endl;
}
```

## 3、对于自定义数据类型 ,比如类对象，那么也会分配内存

```c++
struct Person
{
	string m_Name;
	int m_Age;
};
void test03()
{
	const Person p;
	//p.m_Age = 10;    //不可修改

    //指针间接修改
	Person * pp = (Person *)&p;
	(*pp).m_Name = "Tom";
	pp->m_Age = 10;

	cout << "姓名： " << p.m_Name << " 年龄： " << p.m_Age << endl;
}
```

# 用const替换宏#define常量

## 1、const和#define区别总结

1. `const`有类型，可进行编译器类型安全检查。`#define`无类型，不可进行类型检查。
2. `const`有作用域，而`#define`不重视作用域.

## 2、宏`#define`缺陷

1. 无类型

2. 宏常量不重视作用域.

   ```c++
   void func1()
   {
   	const int a = 10;
   	#define A 20 
       //#undef A  //卸载宏常量A
   }
   void func2()
   {
   	//cout << "a:" << a << endl; //不可访问，超出了const int a作用域
   	cout << "A:" << A << endl; //#define作用域从定义到文件结束或者到#undef，可访问
   }
   ```

3. 宏常量不可以有命名空间

   ```c++
   namespace MySpace
   {
   	#define num 1024
   }
   void test()
   {
   	//cout << MySpace::num << endl; //错误
   }
   ```

   

