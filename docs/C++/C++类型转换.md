# C++类型转换

类型转换(cast)是将一种数据类型转换成另一种数据类型。例如，如果将一个整型值赋给一个浮点类型的变量，编译器会暗地里将其转换成浮点类型。

转换是非常有用的，但是它也会带来一些问题，比如在**转换指针时**，我们很可能将其转换成一个比它更大的类型，但这可能会破坏其他的数据。-----（应该小心类型转换）

标准c++提供了一个显示的转换的语法，来替代旧的C风格的类型转换。

使用C风格的强制转换可以把想要的任何东西转换成我们需要的类型。

新类型的强制转换可以提供更好的控制强制转换过程，允许控制各种不同种类的强制转换。C++风格的强制转换其他的好处是，它们能更清晰的表明它们要干什么。程序员只要扫一眼这样的代码，就能立即知道一个强制转换的目的。

# 静态类型转换（static_cast）

1. 用于类层次结构中基类（父类）和 派生类（子类）之间指针或引用的转换。
   * 进行上行转换（把派生类的指针或引用转换成基类表示）是安全的；
   * 进行下行转换（把基类指针或引用转换成派生类表示）时，由于没有动态类型检查，所以是不安全的。
2. 用于基本数据类型之间的转换，如把int转换成char，把char转换成int。这种转换的安全性也要开发人员来保证。
3. 语法：`static_cast<目标类型>(原变量/原对象)`

## 1、基本类型转换（内置类型）

```c++
//1.基本数据类型转换
void test01(){
	double a=15.05d;
	int b=static_cast<int>(a);
	cout<<"a="<<a<<"  b="<<b<<endl;        //a=15.05  b=15 
	
	char c='A';
	int d=static_cast<int>(c);
	cout<<"c="<<c<<"  d="<<d<<endl;       //c=A  d=65 
} 
```

## 2、继承关系父子指针相互转换

```c++
//2.继承关系父子指针相互转换
class Base{ };  //基类 
class Son:public Base{ };  //派生类 
class Other{ };  //其他类 

void test02(){
	Base *base=new Base;
	Son *son=new Son;
	
	//下行（父转子）---不安全 
	Son *son1=static_cast<Son *>(base);
	
	//上行（子转父）---安全
	Base *base1=static_cast<Base *>(son);
}
```

## 3、继承关系父子对象的引用相互转化

```c++
//3.继承关系父子对象的引用相互转换
void test03(){
	Base base;
	Son son;
	Base &base1=base;
	Son &son1=son;
	
	//下行（父转子）---不安全 
	Son &son2=static_cast<Son &>(base1); 
	
	//上行（子转父）---安全
	Base &base2=static_cast<Base &>(son1); 
}
```

## 4、无继承关系类指针和类对象的引用相互转换---失败

```c++
//4.无继承关系类型转换
void test04(){
	Son *son=new Son;
	//Other *other=static_cast<Other *>(son);   //失败 
	
	Base base;
	Base &base1=base;
	//Other &other=static_cast<Other &>(base1); //失败 	
}
```

# 动态类型转换（dynamic_cast）

1. 不许内置数据类型转换 
2. 允许父子之间指针或者引用的转换
   * 父转子  不安全的  转换失败
   * 子转父  安全  转换成功
   * 如果发生多态，总是安全，可以成功
3. 语法：`dynamic_cast<目标类型>(原变量\原对象)`

## 1、不允许内置数据类型转换

```c++
//1.基本数据类型转换 --- 失败
void test01(){
	double a=15.05d;
	//int b=dynamic_cast<int>(a);
	//cout<<"a="<<a<<"  b="<<b<<endl;       
}
```

## 2、父子关系指针、引用（下行）转换---不安全（失败）

```c++
//父转子指针、引用
class Base{ };  //基类 
class Son:public Base{ };  //派生类 
class Other{ };  //其他类 
void test02(){
	Base *base=new Base;
	//Son *son=dynamic_cast<Son *>(base);
	
	Base base1;
	Base &base2=base1;
	//Son &son1=dynamic_cast<Son &>(base2);	
}
```

## 3、父子关系指针、引用（上行）转换---安全（成功）

```c++
//子转父指针、引用
void test03(){
	Son *son=new Son;
	Base *base=dynamic_cast<Base *>(son);
	
	Son son1;
	Son &son2=son1;
	Base &base1=dynamic_cast<Base &>(son2); 
}
```

## 4、无继承关系指针和引用相互转换---失败

```c++
//4.无继承关系类型转换
void test04(){
	Son *son=new Son;
	Other *other=dynamic_cast<Other *>(son);   //失败 
	
	Base base;
	Base &base1=base;
	Other &other=dynamic_cast<Other &>(base1); //失败 	
}
```

# 常量转换（const_cast)

该运算符用来修改类型的`const`属性。

1. 只允许 指针或者引用 之间转换
   * 常量指针和非常量指针相互转换，并且仍然指向原来的对象；
   * 常量引用和非常量引用相互转换，并且仍然指向原来的对象；
2. 语法：`const_cast<目标类型>(原变量\原对象)`

## 1、不能对非指针或非引用进行转换

```c++
void test01(){
	int a=10;
	//double b=const_cast<double>(a);
}
```

## 2、常量指针和非常量指针相互转换

```c++
void test(){
	const int *p=NULL; //常量指针
	//int const *p=NULL;  //等价
	
	//常量指针转非常量指针 
	int *pp=const_cast<int *>(p); 
	
	//非常量指针转常量指针 
	int const *ppp=const_cast<int const *>(pp);
}
```

## 3、常量引用和非常量引用相互转化

```c++
void test(){
	int a=10; 
	int &b=a;   //非常量引用
	 
	//非常量引用转常量引用 
	const int &c=const_cast<const int &>(b);
	
	//常量引用转非常量引用
	int &d=const_cast<int &>(c);
}
```

# 重新解释转换（reinterpret_cast）

这是最不安全的一种转换机制，最有可能出问题。

主要用于将一种数据类型从一种类型转换为另一种类型。它可以将一个指针转换成一个整数，也可以将一个整数转换成一个指针.

## 1、整数和指针之间相互转化---成功（危险）

```c++
void test(){
	int a=10;
	char *p=reinterpret_cast<char *>(a);
	printf("%p\n",a);         //000000000000000a 
	printf("%p\n",p);         //000000000000000a 
}
```

## 2、无继承关系之间指针、引用相互转换---成功（危险）

```c++
class ClassA{ };
class ClassB{ }; 
void test(){
	//指针相互转换
	ClassA *A=new ClassA;
	ClassB *B=reinterpret_cast<ClassB *>(A); 
	
	//引用转换
	ClassA a;
	ClassA &b=a;
	ClassB &c=reinterpret_cast<ClassB &>(b); 
}
```









