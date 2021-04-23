# 构造函数

构造函数主要作用在于创建对象时为对象的成员属性赋值，构造函数由编译器自动调用，无须手动调用。

## 1、构造函数语法

* 构造函数没有返回值，不能用void

* 构造函数名和类名相同

* 构造可以有参数

* 构造可以重载

* `classname(){ }`

  ```c++
  class Person{
  public:
      Person()
      {
          cout<<"构造函数调用"<<endl;
          pName=(char *)malloc(20);
          strcpy(pName,"老王"); 
          m_Tall=180;
          m_Money=100000;
      }
      ~Person()
      {
          cout<<"析构函数调用"<<endl;
          if(pName)
          {
              free(pName);
              pName=NULL;
          }
      }
  public:
  	char* pName;
  	int m_Tall;
  	int m_Money;
  };
  
  void test()
  {
      Person person;   //结果：构造函数调用 
      cout<<"姓名："<<person.pName<<"  身高："<<person.m_Tall<<"  身家："<<person.m_Money<<endl;
                       //结果：姓名：老王  身高：180  身家：100000  
                       //结果：析构函数调用
  }
  ```

## 2、构造函数的分类及调用

### 1、分类

   * 按参数类型：分为无参构造函数和有参构造函数

     ```c++
     class Person{
     public:
     	Person()
     	{
     		cout<<"无参构造函数调用"<<endl; 
     	}
     	Person(string name,int tall,int money)
     	{
     		m_Name=name;
     		m_Tall=tall;
     		m_Money=money;
     		cout<<"有参构造函数调用"<<endl;
     	}
     public:
     	string m_Name;
     	int m_Tall;
     	int m_Money;
     };
     
     void test()
     {
         Person person;         //结果：构造函数调用 
         Person person1("老王",170,10000);   //结果：有参构造调用
         cout<<"姓名："<<person1.m_Name<<"  身高："<<person1.m_Tall<<"  身家：" <<person1.m_Money<<endl; 
     }
     
     ```

   * 按类型：分为普通构造函数和拷贝构造函数（复制构造函数)  

     ```c++
     class Person{
     public:
     	Person(string name,int tall,int money)
     	{
     		m_Name=name;
     		m_Tall=tall;
     		m_Money=money;
     		cout<<"有参构造函数调用"<<endl;
     	}
     	Person(const Person &person)
     	{
     		m_Name=person.m_Name;
     		m_Tall=person.m_Tall;
     		m_Money=person.m_Money;
     		cout<<"拷贝构造函数调用"<<endl;
     	} 
     public:
     	string m_Name;
     	int m_Tall;
     	int m_Money;
     };
     
     void test()
     {
         Person person1("老王",170,10000);   //结果：有参构造函数调用
         cout<<"姓名："<<person1.m_Name<<"  身高："<<person1.m_Tall<<"  身家："<<person1.m_Money<<endl; 
         
         Person person2(person1);         //结果：拷贝构造函数调用
         cout<<"姓名："<<person2.m_Name<<"  身高："<<person2.m_Tall<<"  身家："<<person2.m_Money<<endl; 
     }
     ```

### 2、调用

```c++
class Person{
public:
    //无参构造
    Person()
    {
        m_Name="无参构造";
        m_Tall=120;
        m_Money=5000; 
        cout<<"无参构造函数调用"<<endl;
    }
    //有参构造
    Person(string name)
    {
    	m_Name=name;
    	m_Tall=0;
    	m_Money=0;
	}
	Person(int tall)
    {
    	m_Name=" ";
    	m_Tall=tall;
    	m_Money=0;
	}
	Person(string name,int tall,int money)
	{
		m_Name=name;
		m_Tall=tall;
		m_Money=money;
		cout<<"有参构造函数调用"<<endl;
	}
    //拷贝构造
	Person(const Person &person)
	{
		m_Name=person.m_Name;
		m_Tall=person.m_Tall;
		m_Money=person.m_Money;
		cout<<"拷贝构造函数调用"<<endl;
	} 
public:
	string m_Name;
	int m_Tall;
	int m_Money;
};
```

1. 无参构造调用

   ```c++
   void test()
   {
       Person person;        //结果：无参构造函数调用
       cout<<"姓名："<<person.m_Name<<"  身高："<<person.m_Tall<<"  身家："<<person.m_Money<<endl; 
                            //姓名：无参构造  身高：120  身家：5000  
   }
   ```

2. 有参构造调用

      * 括号法

        ```c++
        void test()
        {
            Person person1("老王",170,10000);   //结果：有参构造函数调用
            cout<<"姓名："<<person1.m_Name<<"  身高："<<person1.m_Tall<<"  身家："<<person1.m_Money<<endl; 
                                               //结果：姓名：老王  身高：170  身家：10000 
        }
        ```

      * 显示法

        ```c++
        void test()
        {
            Person person1=Person("老李",170,10000);   //结果：有参构造调用
            cout<<"姓名："<<person1.m_Name<<"  身高："<<person1.m_Tall<<"  身家："<<person1.m_Money<<endl; 
                                                       //结果：姓名：老李  身高：170  身家：10000 
        }
        ```

      * 隐式法

        ```c++
        void test()
        {
            Person person1=(160,170,175);
            //等价于----Person person1=Person(175)
            cout<<"姓名："<<person1.m_Name<<"  身高："<<person1.m_Tall<<"  身家："<<person1.m_Money<<endl; 
                         //结果：姓名：   身高：175  身家：0  
            //注意：这里去参数列表最后一个值然后匹配只有一个参数的构造函数
        }
        ```

3. 拷贝构造调用

   * 括号法

     ```c++
     void test()
     {
         Person person1("老王",170,10000);   //结果：有参构造函数调用
         Person person2(person1);            //结果：拷贝构造函数调用
         cout<<"姓名："<<person2.m_Name<<"  身高："<<person2.m_Tall<<"  身家："<<person2.m_Money<<endl;
                                             //结果：姓名：老王  身高：170  身家：10000  
     }
     ```

   * 隐式法

     ```c++
     void test()
     {
         Person person1("老王",170,10000);   //结果：有参构造函数调用
         Person person2=person1;            //结果：拷贝构造函数调用
         cout<<"姓名："<<person2.m_Name<<"  身高："<<person2.m_Tall<<"  身家："<<person2.m_Money<<endl;
                                             //结果：姓名：老王  身高：170  身家：10000  
     }
     ```

4. 匿名对象

   匿名对象，没有名字的对象--特点：当前行执行完后 立即释放

   ```c++
   void test()
   {
       Person("老王",165,5000);   //立即释放，无法访问对象的成员
   }
   
   ```

   匿名对象用来初始化一个对象

   ```c++
   void test()
   {
       Person person(Person(150));
       //等价于----Person person=Person(150)
       cout<<"姓名："<<person.m_Name<<"  身高："<<person.m_Tall<<"  身家："<<person.m_Money<<endl; 
                                       //结果：姓名：   身高：150  身家：0 
       
       Person person1(Person("朱老师",175,8000));
       //等价于-----Person person=Person("朱老师",175,8000)
       cout<<"姓名："<<person1.m_Name<<"  身高："<<person1.m_Tall<<"  身家："<<person1.m_Money<<endl;
                                       //结果：姓名：朱老师  身高：175  身家：8000 
   }
   ```

   注意: 使用匿名对象初始化判断调用哪一个构造函数，要看匿名对象的参数类型

### 3、注意事项

1. 不要用括号法调用无参构造函数------编译器会认为代码是函数的声明---Person();

2. 不要用拷贝构造函数 初始化 匿名对象 `Person(p3); `编译器认为 `Person p3`对象实例化  如果已经有`p3 ` `p3`就重定义

   ```c++
   class Teacher{
   public:
   	Teacher()
       {
   		cout << "默认构造函数!" << endl;
   	}
   	Teacher(const Teacher& teacher)
       {
   		cout << "拷贝构造函数!" << endl;
   	}
   public:
   	int mAge;
   };
   void test()
   {
   	Teacher t1;
   	//error C2086:“Teacher t1”: 重定义
   	Teacher(t1);  //此时等价于 Teacher t1;
   }
   ```

3. 匿名对象  特点： 当前行执行完后 立即释放

## 3、拷贝构造函数的调用时机

### 1、用已经创建好的对象来初始化新的对象

```c++
void test()
{
    Person person("仓井老师",160,1000000);   //结果：有参构造函数调用
    Person person1=person;                  //结果：拷贝构造函数调用
    cout<<"姓名："<<person1.m_Name<<"  身高："<<person1.m_Tall<<"  身家："<<person1.m_Money<<endl;
                                            //结果：姓名：仓井老师  身高：160  身家：1000000 
}
```

### 2、值传递的方式 给函数参数传值

```c++
void func(Person person)      //等价于----Person person=person1
{
    cout<<"姓名："<<person.m_Name<<"  身高："<<person.m_Tall<<"  身家："<<person.m_Money<<endl;
                             //结果：姓名：朱辅导  身高：175  身家：10000 
}  
void test()
{
    Person person1("朱辅导",175,10000);    //结果：有参构造函数调用
    func(person1);                        //结果：拷贝构造函数调用  --函数结束会调用析构函数
}
```

### 3、以值返回方式 返回局部对象

```c++
Person func()
{
    Person person("鹅厂大佬",178,5000000);    // person是局部变量，在函数执行完将会进行析构
    return person;      //会存在Person tmp=person    在主调函数中如果有变量接受  tmp的话就不会立刻释放
}
void test()
{
    Person person1=func();   //这一过程没有调用拷贝构造函数，而是直接将tmp转正为person1
    //cout<<"姓名："<<person1.m_Name<<"  身高："<<person1.m_Tall<<"  身家："<<person1.m_Money<<endl;
}

void test01()
{
    func();   //函数结束后调用析构函数立即释放tmp对象  
    cout << "func执行完了" << endl; //tmp在此之前被析构，相当于匿名对象，使用完就会被析构
}
```

## 4、构造函数的调用规则

1. 编译器会给一个类 至少添加3个函数   

       * 默认构造函数(无参，函数体为空)
       * 默认析构函数(无参，函数体为空)
       * 默认拷贝构造函数，对类中非静态成员属性简单值拷贝

2. 如果我们自己提供了 有参构造函数，编译器就不会提供默认构造函数，但是依然会提供拷贝构造函数

   ```c++
   class Student{
   public:
       //有参构造
       Student(string name,int age)
       {
           m_Name=name;
           m_Age=age;
       }
   public:
       string m_Name;
       int m_Age;
   };
   
   void test()
   {
       //Student stu;      //报错---没有可调用的无参构造函数
       Student stu1("张总",22);
   }
   ```

3. 如果我们自己提供了 拷贝构造函数，编译器就不会提供其他构造函数

   ```c++
   class Student{
   public:
       //无参构造
       Student()
       {
           m_Name="抖音衣哥";
           m_Age=20;
       }
       //拷贝构造
       Student(const Student &stu)
       {
           m_Name=stu.m_Name;
           m_Age=stu.m_Age;
           cout<<"调用拷贝构造函数"<<endl;
       }
   public:
       string m_Name;
       int m_Age;
   };
   
   void test()
   {
       Student stu;      
       //Student stu1("张总",22);   //报错---没有可调用的有参构造函数
       Student stu1=Student(stu);
   }
   ```

## 5、深拷贝与浅拷贝的问题以及解决

### 1、浅拷贝

同一类型的对象之间可以赋值，使得两个对象的成员变量的值相同，两个对象仍然是独立的两个对象，这种情况被称为**浅拷贝**

```c++
class String{
public:
    //有参构造函数
    String(int age)
    {
        m_Age=age;
        cout<<"有参构造调用"<<endl;
    }
    //拷贝构造函数（浅拷贝）--编译器提供可省略
    String(const String &str)
    {
        m_Age=str.m_Age;
        cout<<"拷贝构造调用"<<endl;
    }
    //析构函数（无动态内存分配采样编译器自动添加的无需重写--省略）
    ~String(){cout<<"析构调用"<<endl;}
    int m_Age;
};

void test()
{
    String str(15);   //有参调用
    String str1(str);  //拷贝构造调用
}
//两次析构调用

```

一般情况下，浅拷贝没有任何副作用，但是当类中有指针，并且指针指向动态分配的内存空间，析构函数做了动态内存释放的处理，会导致内存问题。

```c++
class myString{
public:
    //有参构造
    myString(char *name)
    {
        m_Name=(char *)malloc(strlen(name)+1);
        strcpy(m_Name,name);
        cout<<"有参构造调用"<<endl;
    }
    //不重写拷贝构造函数
    //析构函数
    ~myString()
    {
        if(m_Name)
        {
            free(m_Name);
            m_Name=NULL;
        }
        cout<<"析构调用"<<endl;
    }
    char *m_Name;
};

void test()
{
    myString str("小野马");
    myString str1(str);
}
//代码会编译过，但在运行过程中会报错，
//问题所在：(1)两个（或两个以上）指针指向同一块空间，这个内存就会被释放多次；
//(2)两个指针指向同一块空间时，一旦一个指针修改了这块空间的值，另一个指针指向的空间的值也会被修改。
```

### 2、深拷贝

重写拷贝构造函数，在函数内部给新对象的指针分配内存

```c++
myString(const myString &str)
{
    m_Name=(char *)malloc(strlen(str.m_Name)+1);
    strcpy(m_Name,str.m_Name);
    cout<<"深拷贝构造调用"<<endl;
}
```

# 析构函数

析构函数主要用于对象**销毁前**系统自动调用，执行一些清理工作。

## 1、析构函数语法

* 析构函数没有返回值，不能用void
* 析构函数名是在类名前加 ~ 
* 析构函数不能有参数
* 析构函数不能重载
* `~classname(){ }`

# explicit关键字

c++提供了关键字explicit，禁止通过构造函数进行的隐式转换。声明为explicit的构造函数不能在隐式转换中使用。

explicit注意：

* explicit用于修饰构造函数,防止隐式转化。
* 是针对单参数的构造函数(或者除了第一个参数外其余参数都有默认值的多参构造)而言。
* 作用是表明该构造函数是显示的, 而非隐式的, 跟它相对应的另一个关键字是implicit, 意思是隐藏的,类构造函数默认情况下即声明为implicit(隐式).

```c++
class CxString  
{  
public:  
    char *_pstr;  
    int _size;  
    CxString(int size)         // 没有使用explicit关键字的类声明, 即默认为隐式
    {  
        _size = size;                // string的预设大小  
        _pstr = malloc(size + 1);    // 分配string的内存  
        memset(_pstr, 0, size + 1);  
    }  
    CxString(const char *p)  
    {  
        int size = strlen(p);  
        _pstr = malloc(size + 1);    // 分配string的内存  
        strcpy(_pstr, p);            // 复制字符串  
        _size = strlen(_pstr);  
    }  
};  
  
    // 下面是调用:  
    CxString string1(24);     // 这样是OK的, 为CxString预分配24字节的大小的内存  
    CxString string2 = 10;    // 这样是OK的, 为CxString预分配10字节的大小的内存  
    CxString string3;         // 这样是不行的, 因为没有默认构造函数, 错误为: “CxString”: 没有合适的默认构造函数可用  
    CxString string4("aaaa"); // 这样是OK的  
    CxString string5 = "bbb"; // 这样也是OK的, 调用的是CxString(const char *p)  
    CxString string6 = 'c';   // 这样也是OK的, 其实调用的是CxString(int size), 且size等于'c'的ascii码  
    string1 = 2;              // 这样也是OK的, 为CxString预分配2字节的大小的内存  
    string2 = 3;              // 这样也是OK的, 为CxString预分配3字节的大小的内存  
    string3 = string1;        // 这样也是OK的, 至少编译是没问题的, 但是如果析构函数里用free释放_pstr内存指针的时候可能会报错, 完整的代码必须重载运算符"=", 并在其中处理内存释放  
```

 上面的代码中, `CxString string2 = 10;` 这句为什么是可以的呢? 在C++中, 如果的构造函数只有一个参数时, 那么在编译的时候就会有一个缺省的转换操作:将该构造函数对应数据类型的数据转换为该类对象. 也就是说`CxString string2 = 10; `这段代码, 编译器自动将整型转换为`CxString`类对象, 实际上等同于下面的操作:

```c++
CxString string2(10);  
或  
CxString temp(10);  
CxString string2 = temp; 
```

但是, 上面的代码中的_size代表的是字符串内存分配的大小, 那么调用的第二句 `CxString string2 = 10;` 和第六句 `CxString string6 = 'c';"`就显得不伦不类, 而且容易让人疑惑

解决办法：使用explicit关键字 修饰构造函数

````c++
class CxString  
{  
public:  
    char *_pstr;  
    int _size;  
    explicit CxString(int size)     // 使用关键字explicit的类声明, 显示转换  
    {  
        _size = size;                // string的预设大小  
        _pstr = malloc(size + 1);    // 分配string的内存  
        memset(_pstr, 0, size + 1);  
    }  
    CxString(const char *p)  
    {  
        int size = strlen(p);  
        _pstr = malloc(size + 1);    // 分配string的内存  
        strcpy(_pstr, p);            // 复制字符串  
        _size = strlen(_pstr);  
    } 
};  
  
    // 下面是调用:  
    CxString string1(24);     // 这样是OK的  
    CxString string2 = 10;    // 这样是不行的, 因为explicit关键字取消了隐式转换  
    CxString string3;         // 这样是不行的, 因为没有默认构造函数  
    CxString string4("aaaa"); // 这样是OK的  
    CxString string5 = "bbb"; // 这样也是OK的, 调用的是CxString(const char *p)  
    CxString string6 = 'c';   // 这样是不行的, 其实调用的是CxString(int size), 且size等于'c'的ascii码, 但explicit关键字取消了隐式转换  
    string1 = 2;              // 这样也是不行的, 因为取消了隐式转换  
    string2 = 3;              // 这样也是不行的, 因为取消了隐式转换  
    string3 = string1;        // 这样也是不行的, 因为取消了隐式转换, 除非类实现操作符"="的重载  
````

explicit关键字只对有一个参数的类构造函数有效, 如果类构造函数参数大于或等于两个时, 是不会产生隐式转换的, 所以explicit关键字也就无效了. 例如: 

```c++
class CxString 
{  
public:  
    char *_pstr;  
    int _age;  
    int _size;  
    explicit CxString(int age, int size)      // explicit关键字在类构造函数参数大于或等于两个时无效  
    {  
        _age = age;  
        _size = size;                 // string的预设大小 
        _pstr = malloc(size + 1);     // 分配string的内存  
        memset(_pstr, 0, size + 1);  
    }  
    CxString(const char *p)  
     {  
        int size = strlen(p);  
        _pstr = malloc(size + 1);    // 分配string的内存  
        strcpy(_pstr, p);            // 复制字符串  
        _size = strlen(_pstr);  
    } 
};  
    // 这个时候有没有explicit关键字都是一样的  
```

但是, 也有一个例外, 就是当除了第一个参数以外的其他参数都有默认值的时候, explicit关键字依然有效, 此时, 当调用构造函数时只传入一个参数, 等效于只有一个参数的类构造函数, 例子如下:

```c++
class CxString  // 使用关键字explicit声明  
{  
public:  
    int _age;  
    int _size;  
    explicit CxString(int age, int size = 0)  
    {  
        _age = age;  
        _size = size;  
        _pstr = malloc(size + 1);     // 分配string的内存  
        memset(_pstr, 0, size + 1); 
    }  
    CxString(const char *p)  
     {  
        int size = strlen(p);  
        _pstr = malloc(size + 1);    // 分配string的内存  
        strcpy(_pstr, p);            // 复制字符串  
        _size = strlen(_pstr);  
    }
};  
  
    // 下面是调用:  
    CxString string1(24);     // 这样是OK的  
    CxString string2 = 10;    // 这样是不行的, 因为explicit关键字取消了隐式转换  
    CxString string3;         // 这样是不行的, 因为没有默认构造函数  
    string1 = 2;              // 这样也是不行的, 因为取消了隐式转换  
    string2 = 3;              // 这样也是不行的, 因为取消了隐式转换  
    string3 = string1;        // 这样也是不行的, 因为取消了隐式转换, 除非类实现操作符"="的重载  
```



