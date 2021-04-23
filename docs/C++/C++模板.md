# C++模板

c++提供了函数模板(function template.)

所谓函数模板：实际上是建立一个**通用函数**，其**函数类型和形参类型不具体制定，用一个虚拟的类型来代表**。这个通用函数就成为函数模板。凡是函数体相同的函数都可以用这个模板代替，不必定义多个函数，只需在模板中定义一次即可。在调用函数时系统会根据实参的类型来取代模板中的虚拟类型，从而实现不同函数的功能。

* c++提供两种模板机制：**函数模板**和**类模板**
* 类属 - 类型参数化，又称参数模板

总结：

* 模板把函数或类要处理的数据类型参数化，表现为参数的多态性，成为类属。
* 模板用于表达逻辑结构相同，但具体数据元素类型不同的数据对象的通用行为。

# 函数模板

**用模板是为了实现泛型，可以减轻编程的工作量，增强函数的重用性**

## 1、函数模板实现任意类型的数据的值交换

如果我要交换double类型数据，那么还需要些一个double类型数据交换的函数

繁琐，写的函数越多，当交换逻辑发生变化的时候，所有的函数都需要修改，无形当中增加了代码的维护难度

如果能把类型作为参数传递进来就好了，传递int就是Int类型交换，传递char就是char类型交换

我们有一种技术，可以实现类型的参数化---函数模板

* 交换函数模板

  ```c++
  template <class T>
  void Swap(T &a,T &b){
  	T tmp=a;
  	a=b;
  	b=tmp;
  }
  ```

* int---整型测试

  ```c++
  void test()
  {
  	int a=1;
  	int b=5;
  	Swap(a,b);
  	cout<<"a="<<a<<" b="<<b<<endl;
  }
  ```

* double类型测试

  ```c++
  void test01()
  {
  	double a=12.5d;
  	double b=5.7d;
  	Swap(a,b);
  	cout<<"a="<<a<<" b="<<b<<endl;
  }
  ```

* float类型测试

  ```c++
  void test02()
  {
  	float a=1.2f;
  	float b=3.4f;
  	Swap(a,b);
  	cout<<"a="<<a<<" b="<<b<<endl;
  }
  ```

* char---字符类型测试

  ```c++
  void test03()
  {
  	char a='8';
  	char b='4';
  	Swap(a,b);
  	cout<<"a="<<a<<" b="<<b<<endl;
  }
  ```

## 2、函数模板和普通函数的区别

* 函数模板不允许自动类型转换，必须严格匹配类型

* 普通函数能够自动进行类型转化

  ```c++
  template <class T>
  T Add(T &a,T &b){
  	cout<<"模板函数调用"<<endl;
  	return a+b;
  }
  
  int Add(int a,int b){
  	cout<<"普通函数调用"<<endl;
  	return a+b;
  }
  
  void test()
  {
  	int a=5;
  	char b='A';
  	cout<<Add(a,a)<<endl;      //普通函数
  	cout<<Add(b,b)<<endl;  //模板函数
  	cout<<Add(a,b)<<endl;      //普通函数
  	cout<<Add(b,a)<<endl;      //普通函数
  }
  ```

  

## 3、函数模板和普通函数在一起调用规则

* 如果函数模板和普通函数都可以调用，那么优先调用普通函数

* 如果想强制调用函数模板，可以使用空模板参数列表`myPrint<>(a, b);`

* 函数模板也可以发生函数重载

* 如果函数模板能产生更好的匹配，那么优先使用函数模板

  ```c++
  template <class T>
  T Add(T &a,T &b){
  	cout<<"模板函数调用"<<endl;
  	return a+b;
  }
  
  int Add(int a,int b){
  	cout<<"普通函数调用"<<endl;
  	return a+b;
  }
  
  void test()
  {
  	int a=5;
  	int b=10;
  	int c='A';
  	char d='B';
  	cout<<Add(a,b)<<endl;     //优先普通函数（模板和普通函数都可以匹配）
  	cout<<Add<>(a,b)<<endl;   //使用空模板参数强制匹配模板韩顺
  	cout<<Add(c,d)<<endl;     //模板函数有更好的匹配---优先使用模板函数
  }
  ```

  ```c++
  //函数模板重载
  template <class T>
  T Add(T &a,T &b,T &c){
  	cout<<"模板函数调用"<<endl;
  	return a+b+c;
  }
  
  void test()
  {
      int a=5;
  	int b=10;
  	int c=15;
      cout<<Add<>(a,b)<<endl;
      cout<<Add(a,b,c)<<endl;
  }
  ```

## 4、函数模板的实现机制

* 编译器并不是把函数模板处理成能够处理任何类型的函数
* 函数模板通过具体类型产生不同的函数 ---  通过函数模板产生的函数 称为模板函数
* 编译器会对函数模板进行两次编译，在声明的地方对模板代码本身进行编译，在调用的地方对参数替换后的代码进行编译。

## 5、函数模板的局限性

模板并不是真实的通用，对于自定义数据类型，可以使用具体化技术，实现对自定义数据类型特殊使用

```c++
template <class T>
//普通交换模板函数 
void Swap(T &a,T &b){
	T tmp=a;
	a=b;
	b=tmp;
}

void test()
{
    int a=5;
	int b=10;
	Swap(a,b);
	cout<<"a="<<a<<" b="<<b<<endl; 
}

class Person{
public:
	Person(){}
	Person(string name,int age){
		this->m_Name=name;
		this->m_Age=age;
	}
	string m_Name;
	int m_Age;
};

//显示具体化的原型和定意思以template<>开头，并通过名称来指出类型
//具体化优先于常规模板
template<> void Swap<Person>(Person &a,Person &b){
	Person tmp;
	tmp.m_Name=a.m_Name;
	a.m_Name=b.m_Name;
	b.m_Name=tmp.m_Name;
	
	tmp.m_Age=a.m_Age;
	a.m_Age=b.m_Age;
	b.m_Age=tmp.m_Age;
}
void test01()
{
	Person p1("不知火舞",55);
	Person p2("娜可露露",66);
	Swap(p1,p2);
	cout<<"p1.name:"<<p1.m_Name<<" p1.age:"<<p1.m_Age<<endl;
	cout<<"p2.name:"<<p2.m_Name<<" p2.age:"<<p2.m_Age<<endl; 
}
```

# 类模板

有两个或多个类，其功能是相同的，仅仅是数据类型不同

## 1、类模板用于实现类所需数据的类型参数化

```c++
template <class NameType,class AgeType>
class Student{
public:
	Student(NameType name,AgeType age){
		this->m_Name=name;
		this->m_Age=age;
	}
	void Show(){
		cout<<"name:"<<this->m_Name<<" age:"<<this->m_Age<<endl;
	}
	
public:
	NameType m_Name;
	AgeType m_Age;
};

void test()
{
    //Student stu("夏侯惇"，888);       // 类模板不能进行类型自动推导 
	Student<string,int>stu("孙悟空",1000);
	stu.Show();
}
```

## 2、类模板和函数模板的区别

* 类模板不可以使用自动类型推导，只能用显示指定类型

  ```c++
  //Student stu("夏侯惇"，888);       // 类模板不能进行类型自动推导 
  Student<string,int>stu("孙悟空",1000);
  ```

* 类模板中 可以有默认参数

  ```c++
  template <class NameType,class AgeType=int>
  Student<string>stu("猪八戒"，654);
  ```

## 3、类模板中成员函数创建时机

类模板中的成员函数  并不是一开始创建的，而是在运行阶段确定出T的数据类型才去创建

```c++
class Person1{
public:
	void showPerson1(){cout << "Person1 show 调用" << endl;}
};

class Person2{
public:
	void showPerson2(){cout << "Person2 show 调用" << endl;}
};

//类模板中的成员函数  并不是一开始创建的，而是在运行阶段确定出T的数据类型才去创建
template<class T>
class MyClass{
public:
	void func1(){obj.showPerson1();}
	void func2(){obj.showPerson2();}
	T obj;
};

void test01()
{
	MyClass <Person2> p1;
	//p1.func1();
	p1.func2();
}
```

## 4、类模板做函数参数

* 指定传入类型

  `void doWork(Person <string, int>&p)`

* 参数模板化

  `template<class T1, class T2>`

  `void doWork2(Person <T1, T2>&p)`

* 整个类 模板化

  `template<class T>`

  `void doWork3( T &p)`

```c++
template <class NameType,class AgeType>
class Person{
public:
    Person(NameType name,AgeType age){
        this->m_Name=name;
        this->m_Age=age;
    }
    void Show(){
    	cout<<"name:"<<this->m_Name<<" age:"<<this->m_Age<<endl; 
	}
    
public:
    NameType m_Name;
    AgeType m_Age;
};

//指定传入类型 
void DoWork(Person<string,int> &person){
	person.Show();
}

//参数模板化
template <class T1,class T2>
void DoWork1(Person<T1,T2> &person){
	person.Show();
} 

//整个类 模板化 
template <class T>
void DoWork2(T &person){
	person.Show();
} 

void test()
{
	Person<string,int> person("不知好歹",555);
	DoWork(person); 
	Person<string,int> person1("宫本武藏",666);
	DoWork1(person1);
	Person<string,int> person2("上官婉儿",777);
	DoWork2(person2);
}
```

## 5、类模板碰到继承

* 类模板派生普通类

  必须要指定出父类中的T数据类型，才能给子类分配内存

  ```c++
  template <class T>
  class Base{
  public:
  	T m_A;
  }; 
  
  class Son:public Base<int>{
  	
  };
  ```

* 类模板派生类模板

  ```c++
  template<class T>
  class Base2{
  public:
  	T m_A;
  };
  
  template<class T1 ,class T2>
  class Son2 :public Base2<T2>{
  public:
  	Son2(){
  		cout << typeid(T1).name() << endl;
  		cout << typeid(T2).name() << endl;
  	}
  	T1 m_B;
  };
  void test()
  {
  	Son2 <int, double> s;
  }
  ```

## 6、类模板类函数的类内、类外实现

* 类内实现

  ```c++
  template<class NameType, class AgeType>
  class Person
  {
  public:
  	Person(NameType name, AgeType age)
  	{
  		this->mName = name;
  		this->mAge = age;
  	}
  	void showPerson()
  	{
  		cout << "name: " << this->mName << " age: " << this->mAge << endl;
  	}
  public:
  	NameType mName;
  	AgeType mAge;
  };
  
  void test01()
  {
  	//Person P1("德玛西亚",18); // 类模板不能进行类型自动推导 
  	Person<string, int>P1("德玛西亚", 18);
  	P1.showPerson();
  }
  ```

* 类外实现   `Person<T1, T2>::Person(T1 name, T2 age){ }`

  ```c++
  template<class T1, class T2>
  class Person{
  public:
  	Person(T1 name, T2 age);
  	void showPerson();
  
  public:
  	T1 mName;
  	T2 mAge;
  };
  
  //类外实现
  template<class T1, class T2>
  Person<T1, T2>::Person(T1 name, T2 age){
  	this->mName = name;
  	this->mAge = age;
  }
  
  template<class T1, class T2>
  void Person<T1, T2>::showPerson(){
  	cout << "Name:" << this->mName << " Age:" << this->mAge << endl;
  }
  
  void test()
  {
  	Person<string, int> p("Obama", 20);
  	p.showPerson();
  }
  ```

## 7、类模板碰到友元

* 友元类内实现

  ```c++
  template <class T1,class T2>
  class Person{
  	friend void Show(Person<T1,T2> &person){
  		cout<<"name:"<<person.m_Name<<"  age:"<<person.m_Age<<endl; 
  	}
  public:
  	Person(T1 name,T2 age){
  		this->m_Name=name;
  		this->m_Age=age;
  	}
  private:
  	T1 m_Name;
  	T2 m_Age;
  };
  
  void test()
  {
  	Person<string,int> person("西施",18);
  	Show1(person);
  }
  ```

* 友元类外实现

  声明：

  ```c++
  friend void printPerson2<>(Person<T1, T2> &p);
  ```

  实现：

  ```c++
  template<class T1,class T2>
  void printPerson2(Person<T1, T2> &p){ 。。。}
  ```

  ```c++
  template<class T1,class T2>
  class Person;
  
  template<class T1,class T2>
  void Show(Person<T1, T2> &p);
  
  template <class T1,class T2>
  class Person{
  	friend void Show<>(Person<string,int> &person);
  public:
  	Person(T1 name,T2 age){
  		this->m_Name=name;
  		this->m_Age=age;
  	}
  private:
  	T1 m_Name;
  	T2 m_Age;
  };
  
  template<class T1,class T2>
  void Show(Person<T1, T2>&p){
  	cout << "性名： " << p.m_Name << "  年龄： " << p.m_Age << endl;
  }
  
  void test()
  {
  	Person<string,int> person("西施",18);
  	Show(person);
  }
  ```

  









