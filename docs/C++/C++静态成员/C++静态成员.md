# 什么是static

static 是 C/C++ 中很常用的修饰符，它被用来控制变量的存储方式和可见性。

## 1、引入static

我们知道在函数内部定义的变量，当程序执行到它的定义处时，编译器为它在栈上分配空间，函数在栈上分配的空间在此函数执行结束时会释放掉，这样就产生了一个问题: 如果想将函数中此变量的值保存至下一次调用时，如何实现？ 最容易想到的方法是定义为全局的变量，但定义一个全局变量有许多缺点，最明显的缺点是破坏了此变量的访问范围（使得在此函数中定义的变量，不仅仅只受此函数控制）。static 关键字则可以很好的解决这个问题。

另外，在 C++ 中，需要一个数据对象为整个类而非某个对象服务,同时又力求不破坏类的封装性,即要求此成员隐藏在类的内部，对外不可见时，可将其定义为静态数据。

## 2、静态数据存储

1. **全局（静态）存储区**：分为 DATA 段和 BSS 段。

   * DATA 段（全局初始化区）存放初始化的全局变量和静态变量；
   * BSS 段（全局未初始化区）存放未初始化的全局变量和静态变量。

   程序运行结束时自动释放。

   其中BBS段在程序执行之前会被系统自动清0，所以未初始化的全局变量和静态变量在程序执行之前已经为0。

   存储在静态数据区的变量会在程序刚开始运行时就完成初始化，也是唯一的一次初始化。

   

2. 在 C++ 中 static 的内部实现机制：静态数据成员要在程序一开始运行时就必须存在。因为函数在程序运行中被调用，所以静态数据成员不能在任何函数内分配空间和初始化。故静态数据成员一定要在**类内申明，类外定义**。

   注意：

   * 静态数据成员要实际地分配空间，故不能在类的声明中定义（只能声明数据成员）。类声明只声明一个类的"尺寸和规格"，并不进行实际的内存分配，所以在类声明中写成定义是错误的。
   * 它也不能在头文件中类声明的外部定义，因为那会造成在多个使用该类的源文件中，对其重复定义。

   static 被引入以告知编译器，将变量存储在程序的静态存储区而非栈上空间，静态数据成员按定义出现的先后顺序依次初始化，注意静态成员嵌套时，要保证所嵌套的成员已经初始化了。清除时的顺序是初始化的反顺序。

# 在C中static的作用

## 1、static修饰变量

1. 修饰局部变量

   * 普通局部变量：在任何一个函数内部定义的变量（不加static修饰符）

     * 作用域：局部作用域
     * 生命周期：函数结束时被清理
     * 编译器一般不对普通局部变量进行初始化，也就是说它的值在初始时是不确定的，除非对其显式赋值。
     * 普通局部变量存储于**进程栈空间**，使用完毕会立即释放。

     ```c++
     void func()
     {
         int variable;   //未初始化，结果不定
         int variable1=10;
         cout<<"variable="<<variable<<" variable1="<<variable1<<endl;
         variable1++;
     }
     
     void test()
     {
         func();   //variable=4254465 variable1=10 
         //cout<<"variable="<<variable<<" variable1="<<variable1<<endl;  //作用域：局部，func函数外部不可见
         //[Error] 'variable' was not declared in this scope
         //[Error] 'variable1' was not declared in this scope
         func();   //variable=4254465 variable1=10    //被再次初始化
     }
     ```

   * 静态局部变量：使用static修饰符定义在任何一个函数内部定义的变量

     * 作用域：局部作用域
     * 生命周期：程序结束时被清理

     * 即使在声明时未赋初值，编译器也会把它初始化为0。
     * 在首次执行到申明或定义处时被初始化，下次函数调用不会被再次初始化（只进行一次初始化操作）
     * 静态局部变量存储于进程的**全局数据区**，即使函数返回，它的值也会保持不变。

     ```c++
     void func()
     {
         static int variable;   //被系统初始化为0
         static int variable1=10;
         cout<<"variable="<<variable<<" variable1="<<variable1<<endl;
         variable1++;    //variable1=11
     }
     
     void test()
     {
         func();     //variable=0 variable1=10 
         //cout<<"variable="<<variable<<" variable1="<<variable1<<endl;  //作用域：局部，func函数外部不可见
         //[Error] 'variable' was not declared in this scope
         //[Error] 'variable1' was not declared in this scope
         func();    //variable=0 variable1=11    //只能进行一次初始化
     } 
     ```

2. 修饰全局变量

   全局变量定义在函数体外部，在**全局数据区**分配存储空间，且编译器会自动对其初始化。

   * 普通全局变量：

     * 作用域：对整个工程可见，其他文件可以使用extern外部声明后直接使用
     * 其他文件不能再定义一个与其相同名字的变量了（否则编译器会认为它们是同一个变量）

     ```c++
     //main.c
     int variable;   //被系统初始化为0
     void test()
     {
         cout<<"variable="<<variable<<endl;    //variable=0 
         variable++;
         cout<<"variable="<<variable<<endl;    //variable=1
     }
     
     //other.c
     extern int varlable;  //其他文件外部使用
     
     int variable;   //重定义（错）
     ```

   * 静态全局变量：

     * 作用域：仅对当前文件可见，其他文件不可访问
     * 其他文件可以定义与其同名的变量，两者互不影响

     ```c++
     //main.c
     static int variable;   //被系统初始化为0
     void test()
     {
         cout<<"variable="<<variable<<endl;        //variable=0  
         variable=20;
         cout<<"variable="<<variable<<endl;        //variable=20 
     }
     
     //other.c
     extern int varlable;  //其他文件外部不可访问（错）
     
     int variable;   //外部文件自定义变量--不冲突
     ```

## 2、static修饰函数

函数的使用方式与全局变量类似，在函数的返回类型前加上static，就是静态函数，其特性如下：

* 静态函数只能在声明它的文件中可见，其他文件不能引用该函数

  ```c++
  //main.c
  void funcA()
  {
      cout<<"调用funcA函数"<<endl;
  }
  void test()
  {
      funcA();
      funcB();   //报错，在本文件（main.c）找不到funcB()这个函数
  }
  
  //other.c
  static funcB()
  {
      cout<<"调用funcB函数"<<endl;
  }
  ```

* 不同的文件可以使用相同名字的静态函数，互不影响

  ```c++
  //file1.c
  void func()
  {
      cout<<"flie1.c文件下调用func函数"<<endl;
  }
  void test1()
  {
      func();
  }
  
  //file2.c
  void func()
  {
       cout<<"flie2.c文件下调用func函数"<<endl;
  }
  void test2()
  {
      func();
  }
  
  //main.c
  void test()
  {
      test1();    //flie1.c文件下调用func函数
      test2();    //flie2.c文件下调用func函数
  }
  ```

# 在C++中static的作用

## 1、static修饰变量（类成员变量）

在类内的数据成员前加static关键字修饰，使之成为类内的静态成员

1. 特点：

   * 静态数据成员存储在全局数据区，静态数据成员在定义时分配存储空间，所以不能在类声明中定义（在类内申明，类外定义）

     类外定义语法：`数据类型 类名:: 静态数据成员名=变量值`

   * 属于类的静态静态资源（只有一个拷贝）

   * 不属于某个对象，在没有实例对象时可直接用类名访问

     `类名::类内的静态数据成员`

     在为对象分配空间中不包括静态成员所占空间。

   * 所有对象共享这个资源，可通过类对象访问

     `对象名.类内的静态数据成员`

   * 和普通数据成员一样，静态数据成员也遵从`public, protected, private`访问规则

2. 作用：

   * 解决与程序中其它全局名字冲突的问题
   * 实现信息隐藏。静态数据成员可以是private成员，而全局变量不能
   
3. 源码示例：

   ```c++
   class Student{
   public:
       //类的静态成员属性
       static int m_Number;
   private:
       static int m_Age;
   };
   //类外初始化，初始化时不加static
   int Student::m_Number=1001;
   int Student::m_Age=15;
   
   void test()
   {
       //1. 通过类名直接访问
       cout<<"Student::m_Number--学号："<<Student::m_Number<<endl;     //Student::m_Number--学号：1001 
       
       //2. 通过对象访问
       Student stu;
       cout<<"stu.m_Number--学号："<<stu.m_Number<<endl;        //stu.m_Number--学号：1001 
       
       //3. 静态成员也有访问权限，类外不能访问私有成员
       //cout<<"年龄："<<Student::m_Age<<endl;
       Student stu1;
       //cout<<"年龄："<<stu1.m_Age<<endl;
   }
   ```

## 2、static修饰函数（类成员函数）

在类定义中，前面有static说明的成员函数称为静态成员函数

1. 特点：

   * 静态成员函数属于整个类，而不属于某一个对象,只有一份拷贝
* 在对象没有创建前，即可通过类名调用
   * 静态成员函数没有this指针，它无法访问属于类对象的非静态数据成员，也无法访问非静态成员函数，它只能调用静态数据成员和静态成员函数
   * 非静态成员函数可以任意地访问静态成员函数和静态数据成员
   * 静态成员函数也有访问权限
   
2. 代码示例

   ```c++
   class Person{
   public:
       //普通成员函数
       void funcA()
       {
           this->m_Name="夏侯惇";
           this->m_Age=28;
   		this->m_Number="1002"; 
           cout<<"姓名："<<m_Name<<" 年龄："<<m_Age<<" 学号："<<m_Number<<endl;
       }
       //静态成员函数
       static void funcB()
       {
           //this->m_Name="老夫子";  //不能访问非静态成员(没有this指针)
           //this->m_Age=26;
           m_Number="1003";
       	cout<<"姓名："<<"无法访问"<<" 年龄："<<m_Age<<" 学号："<<m_Number<<endl;   
       }
       //类的静态成员属性
       string m_Name;
       static int m_Age;
   private:
       static void funcC()
       {
           //this->m_Name="凯爹";  //不能访问非静态成员(没有this指针)
           //this->m_Age=26;
       	m_Number="1004";
       	cout<<"姓名："<<"无法访问"<<" 年龄："<<m_Age<<" 学号："<<m_Number<<endl;  
       }
       static string m_Number;
   };
   //静态成员属性类外初始化
   int Person::m_Age=15;
   string Person::m_Number="1001";
   
   void test()
   {
       //1. 类名直接调用--静态成员函数
       Person::funcB();    //姓名：无法访问 年龄：15 学号：1003 
       
       //2. 通过对象调用
       Person person;
       person.funcA();       //姓名：夏侯惇 年龄：28 学号：1002 
       person.funcB();       //姓名：无法访问 年龄：28 学号：1003 
       
       //3. 静态成员函数也有访问权限
       //Person::funcC();
       Person person1;
       //person1.funcC();
   }
   ```

## 3、const静态成员属性

如果一个类的成员，既要实现共享，又要实现不可改变，那就用 static const 修饰。

**定义静态`const`数据成员时，最好在类内部初始化**。

```c++
class Person{
public:
    string m_Name;
    Person(string name):m_Name(name){};
    const static int m_Age=30;
    //
    static const int m_Money=1000000;
};

void test()
{
    Person person("貂蝉");            //姓名：貂蝉 年龄：30 身家：1000000 
    cout<<"姓名："<<person.m_Name<<" 年龄："<<person.m_Age<<" 身家："<<person.m_Money<<endl;
    
    //不可修改 
    //person.m_Age=45;  
}
```

# 静态成员实现单例模式

## 1、单例模式

单例模式是一种常用的软件设计模式。在它的核心结构中只包含一个被称为单例的特殊类。

通过单例模式可以保证系统中一个类只有一个实例而且该实例易于外界访问，从而方便对实例个数的控制并节约系统资源。

如果希望在系统中某个类的对象只能存在一个，单例模式是最好的解决方案。

Singleton（单例）：在单例类的内部实现只生成一个实例，同时它提供一个静态的`getInstance()`工厂方法，让客户可以访问它的唯一实例；为了防止在外部对其实例化，**将其默认构造函数和拷贝构造函数设计为私有**；在单例类内部定义了一个Singleton类型的静态对象，作为外部共享的唯一实例。

## 2、单例模式实现一台打印机供多人使用

```c++
//打印机类
class Printer{
public:
	static Printer *getInstance(){return Pprinter;}
	int m_Print_Times;   //打印次数
    string m_Model;     //打印机型号

private:
	Printer()
	{
		m_Model="喵喵";
		m_Print_Times=0;
	}
	Printer(const Printer& printer){}
	
private:
    static Printer *Pprinter;
};

Printer* Printer::Pprinter = new Printer;
//Printer* printer=Printer::getInstance();

//员工类
class Employee{
public:
	Employee(string name)
	{
		m_Name=name;
		printer=Printer::getInstance();
	}
	void Print(string text)
	{
		cout<<"员工："<<m_Name<<"正在使用："<<printer->m_Model<<"打印机打印："<<text<<endl;
		cout<<printer->m_Model<<"打印机已被使用："<<++printer->m_Print_Times<<"次"<<endl;
	}
	
    string m_Name;
    Printer* printer;
};

void test()
{
	Employee employee("财务小姐姐");
	employee.Print("你的工资已被本仙女没收了");
	
	cout<<"---------------------------"<<endl;
	
	Employee employee1("前台小姐姐");
	employee1.Print("今晚有空有我家吗"); 
	
	if(employee.printer==employee1.printer)
	{
		cout<<endl<<"公司只有一台名为："<<employee.printer->m_Model<<"的打印机"<<endl; 
	} 
}
```