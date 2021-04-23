---
typora-root-url: Image
---

# C++异常

提供异常的基本目的就是为了让一个函数在发现了自己无法处理的错误时抛出（throw）一个异常，然后它的（直接或者间接）调用者能够捕获并处理这个问题。

异常处理就是处理程序中的错误。所谓错误是指在程序运行的过程中发生的一些异常事件（如：除0溢出，数组下标越界，所要读取的文件不存在,空指针，内存不足等等

在C语言的世界中，对错误的处理总是围绕着两种方法：一是使用整型的返回值标识错误；二是使用`errno`宏（可以简单的理解为一个全局整型变量）去记录错误。当然C++中仍然是可以用这两种方法的。

 这两种方法最大的缺陷就是会出现不一致问题。例如有些函数返回1表示成功，返回0表示出错；而有些函数返回0表示成功，返回非0表示出错。

 还有一个缺点就是函数的返回值只有一个，你通过函数的返回值表示错误代码，那么函数就不能返回其他的值。当然，你也可以通过指针或者C++的引用来返回另外的值，但是这样可能会令你的程序略微晦涩难懂。

# c++异常机制相比C语言异常处理的优势

1. 函数的返回值可以忽略，但异常不可忽略。如果程序出现异常，但是没有被捕获，程序就会终止，这多少会促使程序员开发出来的程序更健壮一点。而如果使用C语言的error宏或者函数返回值，调用者都有可能忘记检查，从而没有对错误进行处理，结果造成程序莫名其面的终止或出现错误的结果。
2. 整型返回值没有任何语义信息。而异常却包含语义信息，有时你从类名就能够体现出来。
3. 整型返回值缺乏相关的上下文信息。异常作为一个类，可以拥有自己的成员，这些成员就可以传递足够的信息。
4. 异常处理可以在调用跳级。这是一个代码编写时的问题：假设在有多个函数的调用栈中出现了某个错误，使用整型返回码要求你在每一级函数中都要进行处理。而使用异常处理的栈展开机制，只需要在一处进行处理就可以了，不需要每级函数都处理。

C语言异常处理尴尬例子

```c++
int Div(int &a,int &b){
	if(b==0)
		return -1;
	return a/b;
}

void test(){    //期望：异常
	int a=10;
	int b=0;
	int ret=Div(a,b);
	if(ret==-1)
		cout<<"异常"<<endl;   //结果：异常
	cout<<"ret="<<ret; 
}

void test01(){    //期望：ret=-1
	int a=10;
	int b=-10;
	int ret=Div(a,b);
	if(ret==-1)
		cout<<"异常"<<endl;    //结果：异常
	cout<<"ret="<<ret;
}
```

分析：

* 如果判断返回值，那么返回值是错误码还是结果？
* 如果不判断返回值，那么b==0时候，程序结果已经不正确

结论：

* 异常应该捕获，如果你捕获，可以，那么异常必须继续抛给上层函数,你不处理，不代表你的上层不处理
* 异常没有捕获的结果就是运行结果错的一塌糊涂，结果未知，未知的结果程序没有必要执行下去

# 异常基本语法

## 1、关键字：`try throw catch`

* 可以出现异常的代码 放到 try块
* 利用throw抛出异常
* 利用catch捕获异常

采用C++异常处理重写上述C语言异常处理的尴尬例子

```c++
void test(){    //期望：异常
	int a=10;
	int b=0;
	int ret;
	try{
		ret=Div(a,b);
	}
	catch(int){
		cout<<"异常"<<endl;   //结果：异常
	}
	cout<<"ret="<<ret; 
}

void test01(){   //期望：-1
	int a=10;
	int b=-10;
	int ret;
	try{
		ret=Div(a,b);
	}
	catch(int){
		cout<<"异常"<<endl;
	}
	cout<<"ret="<<ret;     //结果：-1
}
```

总结：

* 若有异常则通过throw操作创建一个异常对象并抛出。
* 将可能抛出异常的程序段放到try块之中。
* 如果在try段执行期间没有引起异常，那么跟在try后面的catch字句就不会执行。

## 2、异常严格类型匹配

```c++
int myDivision(int a , int b){
	if ( b == 0){
		//throw 1;       //抛出int类型的异常
		//throw 'a';     //抛出char类型的异常
		//throw 3.14;    //抛出double类型的异常
		//string str = "abc";
		//throw str;     //抛出string类型异常
	}
	return a / b;
}

void test01(){
	int a = 10;
	int b = 0;

	try{myDivision(a, b);}
	catch (int){cout << "int类型异常捕获" << endl;}
	catch (char){cout << "char类型异常捕获" << endl;}
	catch (double){
		//捕获到了异常，但是不想处理，继续向上抛出这个异常
		//异常必须有函数进行处理，如果没有任何处理，程序自动调用 terminate 函数，让程序中断
		throw;
		cout << "double类型异常捕获" << endl;
	}
	catch (...){
		//throw; 
		cout << "其他类型异常捕获" << endl;
	}	
}

int main(){
	try{test01();}
	catch (double){cout << "double函数中 double类型异常捕获" << endl;}
	catch (...){cout << "main函数中 其他类型异常捕获" << endl;}

	system("pause");
	return EXIT_SUCCESS;
}
```

总结：

* catch子句会根据出现的先后顺序被检查，匹配的catch语句捕获并处理异常(或继续抛出异常)
* 如果匹配的处理未找到，则运行函数terminate将自动被调用，其缺省功能调用abort终止程序。
* 处理不了的异常，可以在catch的最后一个分支，使用throw，向上抛。
* catch( 类型) 如果想捕获其他类型 catch(…)
* 异常可以是自定义数据类型

## 3、栈解旋

异常被抛出后，从进入try块起，到异常被抛掷前，这期间在栈上构造的所有对象，都会被自动析构。析构的顺序与构造的顺序相反，这一过程称为栈的解旋(unwinding).

```c++
class MyException{
public:
	MyException(){cout<<"MyException默认构造调用"<<endl;}
	MyException(const MyException &myexception){cout<<"MyException的默认拷贝构造调用"<<endl;}
	~MyException(){cout<<"MyException的析构调用"<<endl;} 
	void printError(){cout << "我自己的异常" << endl;}
};

class Person{
public:
	Person(){cout << "Person的默认构造函数调用" << endl;}
	~Person(){cout << "Person的析构函数调用" << endl;}
};

int myDivision(int a , int b){
	if ( b == 0){
		//从try代码块开始，到throw抛出异常之前，所有栈上的数据都会被释放掉，
		//释放的顺序和创建顺序相反的，这个过程我们称为栈解旋
		Person p1;
		Person p2;
		throw MyException();   //抛出 MyException的匿名对象 --- 自定义类型
	}
	return a / b;
}

void test01(){
	int a = 10;
	int b = 0;

	try{myDivision(a, b);}
	catch (MyException &e){e.printError();}  //注意此处的值传递（调用构造函数）和传引用（调用拷贝构造函数）的区别 
	catch (...){
        //throw;   //继续向上抛出
        cout << "其他类型异常捕获" << endl;
    }	
}

int main(){
	try{test01();}
	catch (double){cout << "double函数中 double类型异常捕获" << endl;}
	catch (...){cout << "main函数中 其他类型异常捕获" << endl;}

	system("pause");
	return EXIT_SUCCESS;
} 
```

![栈解旋](/栈解旋.png)



## 4、异常的接口申明

* 为了加强程序的可读性，可以在函数声明中列出可能抛出异常的所有类型，例如：`void func() throw(A,B,C);`这个函数`func`能够且只能抛出类型A,B,C及其子类型的异常。
* 如果在函数声明中没有包含异常接口声明，则此函数可以抛任何类型的异常，例如:`void func()`
* 一个不抛任何类型异常的函数可声明为:`void func() throw(空)`
* 如果一个函数抛出了它的异常接口声明所不允许抛出的异常,`unexcepted`函数会被调用，该函数默认行为调用`terminate`函数中断程序。

```c++
//1.可抛出所有类型异常
void TestFunction01(){
	throw 10;
}

//2.只能抛出int char char*类型异常
void TestFunction02() throw(int,char,char*){
	string exception = "error!";   //string类型 
	throw exception;
}

//3.不能抛出任何类型异常
void TestFunction03() throw(){
	throw 10;
}

void test(){
	try{
		//TestFunction01();           //异常捕获 
		//TestFunction02();           //terminate called after throwing an instance of 'std::string'
		TestFunction03();           //terminate called after throwing an instance of 'int'
	}
	catch (...){
		cout << "捕获异常!" << endl;
	}
	
}
```

## 5、异常变量的声生命周期

```c++
class MyException
{
public:
	MyException(){cout << "MyException默认构造函数调用" << endl;}

	MyException(const MyException & e){cout << "MyException拷贝构造函数调用" << endl;}

	~MyException(){cout << "MyException析构函数调用" << endl;}
	
	void Print(){cout << "自定义异常捕获" << endl;} 

};

void doWork()
{
	throw  &MyException();
}

void test()
{
	try
	{
		doWork();
	}
	//抛出的是 throw MyException();  catch (MyException e)    调用默认构造函数和默认拷贝构造函数 效率低
	//抛出的是 throw MyException();  catch (MyException &e)   只调用默认构造函数 效率高 推荐
	//抛出的是 throw &MyException(); catch (MyException *e)   对象会提前释放掉，不能在非法操作
	//抛出的是 new MyException();   catch (MyException *e)    只调用默认构造函数 自己要管理释放
	catch (MyException *e)
	{
		e->Print();
		//cout << "自定义异常捕获" << endl;
		delete e;
	}
}
```

总结：

* 抛出的是 `throw MyException();`  ` catch (MyException e) `  调用默认构造函数和默认拷贝构造函数 效率低
* 抛出的是 `throw MyException(); `  `catch (MyException &e)`  只调用默认构造函数 效率高 推荐
* 抛出的是`throw new MyException();`   `catch (MyException *e) `  只调用默认构造函数 自己要管理释放
* 抛出的是 `throw &MyException(); `  `catch (MyException *e)  `对象会提前释放掉，不能在非法操作

## 6、异常的多态使用

* 异常基类

  ```c++
  class BaseException{
  public:
      //纯虚函数
      virtual void PrintError()=0;
  }
  ```

* 异常子类1---空指针异常

  ```c++
  class NullPointerException:public BaseException{
  public:
      //重写虚函数
      void PrintError(){
          cout<<"空指针异常捕获"<<endl;
      }
  }
  ```

* 异常子类2---越界异常

  ```c++
  class OutOfRangeException :public BaseException{
  public:
      //重写虚函数
      void PrintError(){
          cout<<"越界异常捕获"<<endl;
      }
  }
  ```

  测试：异常抛出、捕获、处理

  ```c++
  void Dowork(){
      //父类引用指向子类对象构成多态
      throw NullPointerException();   //抛出空指针异常
      throw OutOfRangeException();    //抛出越界异常
  }
  
  void test(){
      try{Dowork();}
      //捕获异常
      catch(BaseException &e){
          e.PrintError();
      }
  }
  ```

  好处：抛出任意子类对象的异常都可以用父类的引用来接受

# C++标准异常

## 1、标准库

标准库中也提供了很多的异常类，它们是通过类继承组织起来的。异常类继承层级结构图如下：

![标准异常库](/标准异常库.png)

## 2、标准异常类的成员

* 在上述继承体系中，每个类都有提供了构造函数、复制构造函数、和赋值操作符重载。
* logic_error类及其子类、runtime_error类及其子类，它们的构造函数是接受一个string类型的形式参数，用于异常信息的描述
* 所有的异常类都有一个what()方法，返回`const char*` 类型（C风格字符串）的值，描述异常信息。

1. 标准异常类的具体描述：

   | 异常名称          | 描述                                                         |
   | ----------------- | ------------------------------------------------------------ |
   | exception         | 所有标准异常类的父类                                         |
   | bad_alloc         | 当operator new and operator new[]，请求分配内存失败时        |
   | bad_exception     | 这是个特殊的异常，如果函数的异常抛出列表里声明了bad_exception异常，当函数内部抛出了异常抛出列表中没有的异常，这是调用的unexpected函数中若抛出异常，不论什么类型，都会被替换为bad_exception类型 |
   | bad_typeid        | 使用typeid操作符，操作一个NULL指针，而该指针是带有虚函数的类，这时抛出bad_typeid异常 |
   | bad_cast          | 使用dynamic_cast转换引用失败的时候                           |
   | ios_base::failure | io操作过程出现错误                                           |
   | logic_error       | 逻辑错误，可以在运行前检测的错误                             |
   | runtime_error     | 运行时错误，仅在运行时才可以检测的错误                       |

2. logic_error的子类：

   | 异常名称         | 描述                                                         |
   | ---------------- | ------------------------------------------------------------ |
   | length_error     | 试图生成一个超出该类型最大长度的对象时，例如vector的resize操作 |
   | domain_error     | 参数的值域错误，主要用在数学函数中。例如使用一个负值调用只能操作非负数的函数 |
   | out_of_range     | 超出有效范围                                                 |
   | invalid_argument | 参数不合适。在标准库中，当利用string对象构造bitset时，而string中的字符不是’0’或’1’的时候，抛出该异常 |

3. runtime_error的子类：

   | 异常名称         | 描述                                                         |
   | ---------------- | ------------------------------------------------------------ |
   | range_error      | 计算结果超出了有意义的值域范围                               |
   | overflow_error   | 算术计算上溢                                                 |
   | underflow_error  | 算术计算下溢                                                 |
   | invalid_argument | 参数不合适。在标准库中，当利用string对象构造bitset时，而string中的字符不是’0’或’1’的时候，抛出该异常 |

## 3、举例

* 引入头文件  `#include <stdexcept>`
* 抛出越界异常 `throw out_of_range(“…”)`
* 获取错误信息  `catch( exception & e )   e.what();`

```c++
#include <stdexcept> //  std 标准  except 异常
class Person{
public:
	Person(int age){
		if (age < 0 || age > 150){
			throw out_of_range("年龄必须在 0 ~ 150之间");
			//throw length_error("年龄必须在 0 ~ 150之间");
		}
		else{
            this->m_Age = age;
        }
	}
	int m_Age;
};

void test(){
	try{
        Person p(151);
    }
	//catch ( out_of_range &e)
	catch ( exception &e){
        cout << e.what() << endl;
    }
}
```

# 编写自己的异常

* 标准库中的异常是有限的；
* 在自己的异常类中，可以添加自己的信息。（标准库中的异常类值允许设置一个用来描述异常的字符串）。

实现方法：

* 建议自己的异常类要继承标准异常类。因为C++中可以抛出任何类型的异常，所以我们的异常类可以不继承自标准异常，但是这样可能会导致程序混乱，尤其是当我们多人协同开发时。
* 当继承标准异常类时，应该重载父类的what函数和虚析构函数。
* 因为栈展开的过程中，要复制异常类型，那么要根据你在类中添加的成员考虑是否提供自己的复制构造函数。

```c++
#include <stdexcept> //  std 标准  except 异常
//自定义异常类
class MyOutOfRange:public exception{   //继承标准异常
public:
	MyOutOfRange(const string  errorInfo){
		this->m_Error = errorInfo;
	}

	MyOutOfRange(const char * errorInfo){
		this->m_Error = string( errorInfo);
	}

	virtual ~exception(){ }    //重写父类的虚析构
	virtual const char* what() const{   //重写父类的what()方法
		return this->m_Error.c_str() ;
	}
	
	string m_Error;
};

class Person{
public:
	Person(int age){
		if (age <= 0 || age > 150){
			//抛出异常 越界
			//cout << "越界" << endl;
			//throw  out_of_range("年龄必须在0~150之间");
			//throw length_error("长度异常");
			throw MyOutOfRange(("我的异常 年龄必须在0~150之间"));
		}
		else{
			this->m_Age = age;
		}	
	}

	int m_Age;
};

void test01(){
	try{
		Person p(151);
	}
	catch ( out_of_range & e ){   //标准异常捕获
		cout << e.what() << endl;
	}
	catch (length_error & e){   //标准异常捕获
		cout << e.what() << endl;
	}
	catch (MyOutOfRange e){   //自定义的异常类捕获异常
		cout << e.what() << endl;
	}
}
```



 





