# 多态

多态性改善了代码的可读性和组织性，同时也使创建的程序具有可扩展性，项目不仅在最初创建时期可以扩展，而且当项目在需要有新的功能时也能扩展。

面向对象程序设计一个基本原则:开闭原则(对修改关闭，对扩展开放)

# 静态多态、动态多态

c++支持编译时多态(静态多态)和运行时多态(动态多态)，运算符重载和函数重载就是编译时多态，派生类和虚函数实现运行时多态。

**静态多态和动态多态的区别就是函数地址是早绑定(静态联编)还是晚绑定(动态联编)：**

* 如果函数的调用，在编译阶段就可以确定函数的调用地址，并产生代码，就是静态多态(编译时多态)，就是说地址是早绑定的
* 如果函数的调用地址不能在编译期间确定，而需要在运行时才能决定，这这就属于晚绑定(动态多态,运行时多态)。

**动态多态产生的条件：**

* 先有继承关系
* 父类中有虚函数（函数名前加关键字virtual），子类重写父类中的虚函数
* 父类的指针或引用  指向子类的对象

# 多态原理

C++动态多态性是通过虚函数来实现的，虚函数允许子类（派生类）重新定义父类（基类）成员函数，而子类（派生类）重新定义父类（基类）虚函数的做法称为覆盖(override)，或者称为重写。

对于特定的函数进行动态绑定，c++要求在基类中声明这个函数的时候使用virtual关键字,动态绑定也就对virtual函数起作用.

* 为创建一个需要动态绑定的虚成员函数，可以简单在这个函数声明前面加上virtual关键字，定义时候不需要.
* 如果一个函数在基类中被声明为virtual，那么在所有派生类中它都是virtual的.
* 在派生类中virtual函数的重定义称为重写(override).
* Virtual关键字只能修饰成员函数.
* 构造函数不能为虚函数.

```c++
class Animal{
public:
	//虚函数
	virtual void speak(){cout << "动物在说话" << endl;}
	virtual void eat(int a ){cout << "动物在吃饭" << endl;}
};

class Cat :public Animal{
public:
	void speak(){cout << "小猫在说话" << endl;}
	void eat(int a){cout << "小猫在吃饭" << endl;}
};

class Dog :public Animal{
public:
	void speak(){cout << "小狗在说话" << endl;}
};

void DoSpeak(Animal &animal){
	animal.speak();
}

void test(){
	Cat cat;
	DoSpeak(cat);
	
	Dog dog;
	DoSpeak(dog);
}

void test01(){
	Animal *animal=new Cat;
	//调用猫说话
	animal->speak();
	// *(int *)animal 解引用到虚函数表中
	// *(int *)*(int *)animal 解引用到函数speak地址
	((void(*)()) (*(int *)*(int *)animal)) ();

	//C/C++默认调用惯例  __cdecl
	//用下列调用时候 真实调用惯例  是 __stdcall
	//调用猫吃饭
	typedef void( __stdcall *FUNPOINT)(int);
	(FUNPOINT (*((int*)*(int*)animal)))(10);
}
```

* 当父类写了虚函数后，类内部结构发生改变，多了一个`vfptr`---虚函数表指针

* 虚函数表内部记录着 虚函数的入口地址

* 当父类指针或引用指向子类对象，发生多态，调用是时候从虚函数中找函数入口地址

* 虚函数 关键字  virtual

* 纯虚函数 `virtual  int  func( )=0;`
  	如果一个类中包含了纯虚函数，那么这个类就无法实例化对象了,这个类通常我们称为 抽象类
  	抽象类的子类 必须要重写 父类中的纯虚函数，否则也属于抽象类

* 利用指针的偏移调用 函数

  ```c++
  ((void(*)()) (*(int *)*(int *)animal)) ();
  typedef void( __stdcall *FUNPOINT)(int);
  (FUNPOINT (*((int*)*(int*)animal)))(10);
  ```

# 多态案例---计算器实现

```c++
class AbstractCalculator{
public:
	//纯虚函数
	//如果一个类中包含了纯虚函数，那么这个类就无法实例化对象了,这个类通常我们称为 抽象类
	//抽象类的子类 必须要重写 父类中的纯虚函数，否则也属于抽象类
	virtual int getResult() = 0;

	int m_A;
	int m_B;
};
//加法计算器
class AddCalculator :public AbstractCalculator{
public:
	virtual int getResult(){return m_A + m_B;}
};

//减法计算器
class SubCalculator :public AbstractCalculator{
public:
	virtual int getResult(){return m_A - m_B;}
};

//乘法计算器
class MulCalculator :public AbstractCalculator{
public:
	virtual int getResult(){return m_A * m_B;}
};

void test01()
{
	AbstractCalculator * calculator = new AddCalculator;
	calculator->m_A = 100;
	calculator->m_B = 200;
	cout << calculator->getResult() << endl;
	delete calculator;
    
	calculator = new SubCalculator;
	calculator->m_A = 100;
	calculator->m_B = 200;
	cout << calculator->getResult() << endl;
    delete calculator;
}

```

# 纯虚函数函数和抽象类

语法：`virtual int func() = 0;`

如果一个类中包含了纯虚函数，那么这个类就无法实例化对象了,这个类通常我们称为 抽象类

抽象类的子类 必须要重写 父类中的纯虚函数，否则也属于抽象类

# 虚析构和纯虚析构

## 1、虚析构

作用：虚析构函数是为了解决基类的指针指向派生类对象，并用基类的指针删除派生类对象。

语法：`virtual ~Person(){ }`

如果子类中有指向堆区的属性，那么要利用虚析构技术 在delete的时候 调用子类的析构函数

```c++
class Animal{
public:
	Animal(){cout << "Animal的构造函数调用" << endl;}
	virtual void speak(){cout << "动物在说话" << endl;}
	//如果子类中有指向堆区的属性，那么要利用虚析构技术 在delete的时候 调用子类的析构函数
	virtual ~Animal(){cout << "Animal的析构函数调用" << endl;}
};

class Cat :public Animal{
public:
	Cat(char * name){
		cout << "Cat的构造函数调用" << endl;
		this->m_Name = new char[strlen(name) + 1];
		strcpy(this->m_Name, name);
	}

	virtual void speak(){cout << this->m_Name <<" 小猫在说话" << endl;}

	~Cat(){
		if (this->m_Name){
			cout << "Cat的析构函数调用" << endl;
			delete[] this->m_Name;
			this->m_Name = NULL;
		}
	}
	char * m_Name;
};

void test01()
{
	Animal * animal = new Cat("Tom");   //先：Animal构造    后：Cat构造 
	animal->speak();  //小猫在说话

	delete animal;    //先：Cat析构   后：Animal析构
}
```

## 2、纯虚析构

纯虚析构函数在c++中是合法的，但是在使用的时候有一个额外的限制：必须为纯虚析构函数提供一个函数体。

作用：纯虚析构函数和非纯析构函数之间唯一的不同之处在于纯虚析构函数使得基类是抽象类，不能创建基类的对象。

语法：`virtual ~Animal() = 0;`    `Animal::~Animal(){ .. }`

注意：

* 纯虚析构 需要有声明 也需要有实现
* 如果一个类中 有了 纯虚析构函数，那么这个类也属于抽象类，无法实例化对象了

```c++
class Animal{
public:
	Animal(){cout << "Animal的构造函数调用" << endl;}
	virtual void speak(){cout << "动物在说话" << endl;}
	//如果子类中有指向堆区的属性，那么要利用虚析构技术 在delete的时候 调用子类的析构函数
	//纯虚析构 需要有声明 也需要有实现
	//如果一个类中 有了 纯虚析构函数，那么这个类也属于抽象类，无法实例化对象了
	virtual ~Animal() = 0;
};

Animal::~Animal(){cout << "Animal的纯虚析构函数调用" << endl;}

class Cat :public Animal{
public:
	Cat(char * name){
		cout << "Cat的构造函数调用" << endl;
		this->m_Name = new char[strlen(name) + 1];
		strcpy(this->m_Name, name);
	}

	virtual void speak(){cout << this->m_Name <<" 小猫在说话" << endl;}

	~Cat(){
		if (this->m_Name){
			cout << "Cat的析构函数调用" << endl;
			delete[] this->m_Name;
			this->m_Name = NULL;
		}
	}
	char * m_Name;
};

void test01()
{
    //Animal animal;   //纯虚析构使Animal为抽象类无法实例化对象
	Animal * animal = new Cat("Tom");
	animal->speak();

	delete animal;   //调用子类的析构函数
}
```

# 向上、向下类型转换

* 父转子  向下类型转换  不安全
* 子转父  向上类型转换  安全
* 如果发生多态，那么转换永远都是安全的

# 重载- 重定义-重写

```c++
class A{
public:
	//同一作用域下，func1函数重载
	void func1(){}
	void func1(int a){}
	void func1(int a,int b){}
	void func2(){}
	virtual void func3(){}
};

class B : public A{
public:
	//重定义基类的func2,隐藏了基类的func2方法
	void func2(){}
	//重写基类的func3函数，也可以覆盖基类func3
	virtual void func3(){}
};
```

## 1、重载

* 同一个作用域
* 函数名相同
* 参数表不同（参数类型、个数、顺序）
* 加`const`和不加`const`的引用
* 返回值不做重载的条件

## 2、重定义（隐藏）

* 有继承
* 子类（派生类）重新定义父类（基类）的同名成员（非virtual函数）

## 3、重写（覆盖）---  多态

*  有继承
* 子类（派生类）重写父类（基类）的virtual函数
* 函数返回值，函数名字，函数参数，必须和基类中的虚函数一致

