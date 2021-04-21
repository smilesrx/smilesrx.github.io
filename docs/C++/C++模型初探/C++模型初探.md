# 成员变量和函数的存储

在c语言中，“数据”和“处理数据的操作（函数）”是分开来声明的，也就是说，语言本身并没有支持“数据和函数”之间的关联性。

在c++中，在类中定义数据和函数，来实现数据和函数直接的绑定。

在C++类中有两种成员数据：`static`、`nonstatic`；三种成员函数：`static`、`nonstatic`、`virtual`。

* c++中的**非静态数据成员**直接内含在类对象中，就像c `struct`一样。
* 成员函数(member function)虽然内含在class声明之内，却不出现在对象中。
* 每一个非内联成员函数(non-inline member function)只会诞生一份函数实例。
* 类中的成员变量 和 成员函数  是分开存储的。
* 空类的`sizeof`结果为 1。

```c++
class MyClass01{
public:
	int m_A;
};

class MyClass02{
public:
	int mA;
	static int m_B;
};

class MyClass03{
public:
	void printMyClass(){cout << "hello world!" << endl;}
public:
	int m_A;
	static int m_B;
};

class MyClass04{
public:
	void printMyClass(){cout << "hello world!" << endl;}
	static void ShowMyClass(){cout << "hello world！" << endl;}
public:
	int m_A;
	static int m_B;
};

class MyClass05{};    //空类

void test()
{
	MyClass01 mclass01;
	MyClass02 mclass02;
	MyClass03 mclass03;
	MyClass04 mclass04;
    MyClass05 mclass05;

	cout << "MyClass01:" << sizeof(mclass01) << endl; //4
	//静态数据成员并不保存在类对象中
	cout << "MyClass02:" << sizeof(mclass02) << endl; //4
	//非静态成员函数不保存在类对象中
	cout << "MyClass03:" << sizeof(mclass03) << endl; //4
	//静态成员函数也不保存在类对象中
	cout << "MyClass04:" << sizeof(mclass04) << endl; //4
    //空类
    cout << "MyClass05:" << sizeof(mclass05) << endl; //1

}
```

结论：**C++类对象中的变量和函数是分开存储**

# this指针

本质：`Person * const this`---指针常量（指针的指向不可以修改）

c++的数据和操作也是分开存储，并且每一个非内联成员函数(non-inline member function)只会诞生一份函数实例，也就是说多个同类型的对象会共用一块代码

那么问题是：这一块代码是如何区分那个对象调用自己的呢？

c++通过提供特殊的对象指针，this指针，解决上述问题。

## 1、this指针特点

1. this指针指向被调用的成员函数所属的对象

2. this指针是隐含在对象成员函数内的一种指针

   当一个对象被创建后，它的每一个成员函数都含有一个系统自动生成的隐含指针this，用以保存这个对象的地址，也就是说虽然我们没有写上this指针，编译器在编译的时候也是会加上的。

3. this也称为“指向本对象的指针”。`*this`就是本体

4. this指针并不是对象的一部分，不会影响`sizeof(对象)`的结果

5. this指针可以解决名称冲突

this指针是C++实现封装的一种机制，它将对象和该对象调用的成员函数连接在一起

**注意：**静态成员函数内部没有this指针，静态成员函数不能操作非静态成员变量

## 2、this 指针的使用

* 当形参和成员变量同名时，可用this指针来区分

* 在类的非静态成员函数中返回对象本身，可使用`return *this`

  ```c++
  class Person{
  public:
  	//1. 当形参名和成员变量名一样时，this指针可用来区分
  	Person(string name,int age)
      {
  		//name = name;
  		//age = age;    //输出错误
  		this->name = name;
  		this->age = age;
  	}
  	//2. 返回对象本身的引用
  	//重载赋值操作符
  	//其实也是两个参数，其中隐藏了一个this指针
  	Person PersonPlusPerson(Person& person)
      {
  		string newname = this->name + person.name;
  		int newage = this->age + person.age;
  		Person newperson(newname, newage);
  		return newperson;
  	}
  	void ShowPerson()
      {
  		cout << "Name:" << name << " Age:" << age << endl;
  	}
  public:
  	string name;
  	int age;
  };
  
  //3. 成员函数和全局函数(Perosn对象相加)
  Person PersonPlusPerson(Person& p1,Person& p2)
  {
  	string newname = p1.name + p2.name;
  	int newage = p1.age + p2.age;
  	Person newperson(newname,newage);
  	return newperson;
  }
  
  void test()
  {
  	Person person("John",100);
  	person.ShowPerson();                    //Name:John Age:100 
  
  	cout << "---------" << endl;
  	Person person1("John",20);
  	Person person2("001", 10);
  	//1.全局函数实现两个对象相加
  	Person person3 = PersonPlusPerson(person1, person2);
  	person1.ShowPerson();                 //Name:John Age:20  
  	person2.ShowPerson();                 //Name:001 Age:10 
  	person3.ShowPerson();                 //Name:John001 Age:30 
  	//2. 成员函数实现两个对象相加
  	Person person4 = person1.PersonPlusPerson(person2);
  	person4.ShowPerson();                 //Name:John001 Age:30 
  }
  ```

* 空指针调用类成员

  ```c++
  class Person{
  public:
      Person()
      {
          m_Age=50;
          m_ID=100;
      }
      void funcA()
      {
          cout<<"hello func!"<<endl;
      }
      void ShowPerson()
      {
          cout<<"姓名："<<this->m_Age<<"ID："<<this->m_ID<<endl;
      }
      
      int m_Age;
      int m_ID;
  };
  
  void test()
  {
      Person *person=NULL;   //空指针
      person->funcA();
      //person->ShowPerson();   //函数体内用到this指针，导致程序崩掉 
  }
  ```

# const修饰成员函数(常函数)

本质：`const Person * const this`----（this的指向和指向的值都不可以修改）

在函数括号后面加上`const`-------`类型 函数名（参数表）const {函数体}`----**成员变量不可修改,除了mutable变量**

`const`目的是为了修饰成员函数中的this指针，让指针指向的值不可以修改

* 用`const`修饰的成员函数时，`const`修饰**this指针指向的内存区域**，成员函数体内不可以修改本类中的任何普通成员变量

* 当成员变量类型符前用`mutable`修饰时例外。

  ```c++
  //const修饰成员函数
  class Person{
  public:
  	Person()
      {
  		this->m_Age = 0;
  		this->m_ID = 0;
  	}
  	//在函数括号后面加上const,修饰成员变量不可修改,除了mutable变量
  	void func() const
      {
  		//this->m_Age = 200; //m_Age不可修改 --- [Error] assignment of member 'Person::m_Age' in read-only 
  		this->m_ID = 10;
  	}
  	void ShowPerson()
      {
  		cout << "m_ID:" << m_ID << " m_Age:" << m_Age << endl;
  	}
  private:
  	int m_Age;
  	mutable int m_ID;
  };
  
  void test()
  {
  	Person person;
  	person.func();
  	person.ShowPerson();
  }
  ```

# const修饰对象(常对象)

* 常对象只能调用`const`修饰的成员函数

* 常对象可访问 `const` 或`非 const `数据成员，不能修改，除非成员用mutable修饰。

  ```c++
  class Person{
  public:
  	Person()
      {
  		this->m_Age = 0;
  		this->m_ID = 0;
  	}
  	void ChangePerson() const
      {
  		//m_Age = 50;   //[Error] assignment of member 'Person::m_Age' in read-only object
  		m_ID = 100;
  	}
  	void ShowPerson()
      {
          this->m_Age = 1000;
  		cout << "m_ID:" << this->m_ID << "  m_Age:" << this->m_Age << endl;
  	}
  public:
  	int m_Age;
  	mutable int m_ID;
  };
  
  void test()
  {	
  	const Person person;      //常对象
  	//1. 可访问数据成员
  	cout << "m_Age:" << person.m_Age << endl;
  	//person.m_Age = 300;  //不可修改
  	person.m_ID = 1001;     //但是可以修改mutable修饰的成员变量
  	cout<<"m_ID:"<<person.m_ID<<endl; 
  	//2. 只能访问const修饰的函数
  	//person.ShowPerson();
  	person.ChangePerson();
  	cout << "m_Age:" << person.m_Age <<"  m_ID:"<<person.m_ID<<endl; 
  }
  ```

  



