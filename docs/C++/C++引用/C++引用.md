# 引用基本用法---给变量取别名

* 变量名实际上是一块连续内存单元的别名，是一个标号（门牌号）
* 程序中通过变量来申请并命名内存空间
* 通过变量的名字也可以使用内存空间

## 1、引用的基本语法

类型名 引用(&)  别名=变量名

## 2、引用注意事项

1. &在此不是取地址运算，而是起标识作用

2. 必须在申明引用变量时进行初始化

   ```c++
   int &m;   //报错--必须初始化
   ```

3. 引用初始化后不能修改（引用本质---常量指针-->指向不可以修改）

   ```c++
   int a=15;
   int b=20;
   int &c=a;
   c=b;    //出错
   ```

4. 不能有NULL引用，必须保证引用和一块合法的内存相关联

5. 可以建立对数组的引用

   * 直接建立引用

     ```c++
     int arr[10]={0};
     int(&ARR)[10]=arr;
     for(int i=0;i<10;i++)
     {
         ARR[i]=i+10;
     }
     for(int j=0;j<10;j++)
     {
         cout<<arr[j]<<endl;
     }
     ```

   * 先定义出数组类型，在通过类型建立引用

     ```c++
     typedef int (PArr)[10];
     int arr[10]={0};
     PArr &ARR=arr;
     for(int i=0;i<10;i++)
     {
         arr[i]=i+10;
     }
     for(int j=0;j<10;j++)
     {
         cout<<ARR[j]<<endl;
     }
     ```

## 3、认识引用

1. 给变量取别名

   ```c++
   int a=10;
   int &b=a;
   cout<<"a="<<a<<endl;
   cout<<"b="<<b<<endl;
   ```

2. 操作同一块内存

   ```c++
   int m=15;
   int &n=m;
   n=50;
   cout<<"m="<<m<<endl;
   cout<<"n="<<n<<endl;
   ```

3. 一个变量可以有多个别名

   ```c++
   int a=15;
   int &b=a;
   int &c=a;
   cout<<"a="<<a<<endl;
   cout<<"b="<<b<<endl;
   cout<<"c="<<c<<endl;
   ```

4. 引用变量地址都相同

   ```c++
   int m=20;
   int &n=m;
   int &k=m;
   cout<<"m的地址："<<&m<<endl;
   cout<<"n的地址："<<&n<<endl;
   cout<<"k的地址："<<&k<<endl;
   ```

# 函数中的引用

最常见看见引用的地方是在函数参数和返回值中

## 1、函数传参中的引用

当引用被用作函数参数的时，在函数内对任何引用的修改，将对还函数外的参数产生改变。当然，可以通过传递一个指针来做相同的事情，但引用具有更清晰的语法。

引出：函数传参的三种方法---经典例子（两数交换值的函数）swap()函数

* 值传递   --形参的改变不会影响实参

  ```c++
  void swap(int a,int b)
  {
      int tmp=a;
      a=b;
      b=tmp;
      cout<<"形参a="<<a<<endl;    //结果：形参a=5 
      cout<<"形参b="<<b<<endl;    //结果：形参b=10 
  }
  void test()
  {
      int a=10;
      int b=5;
      //传值函数调用
      swap(a,b);
      //验证
      cout<<"实参a="<<a<<endl;   //结果：实参a=10  
      cout<<"实参b="<<b<<endl;   //结果：实参b=5 
  }
  ```

* 址传递

  ```c++
  void swap(int *a,int *b)
  {
      int tmp=*a;
      *a=*b;
      *b=tmp;
  }
  void test()
  {
      int a=10;
      int b=5;
      //传址调用
      swap(&a,&b);
      //验证
      cout<<"实参a="<<a<<endl;   //结果：实参a=5  
      cout<<"实参b="<<b<<endl;   //结果：实参b=10
  }
  ```

* 传引用

  ```c++
  void swap(int &a,int &b)
  {
      int tmp=a;
      a=b;
      b=tmp;
  }
  void test()
  {
      int a=10;
      int b=5;
      //传引用调用
      swap(a,b);
      //验证
      cout<<"实参a="<<a<<endl;   //结果：实参a=5  
      cout<<"实参b="<<b<<endl;   //结果：实参b=10
  }
  ```

## 2、函数返回值中的引用

如果从函数中返回一个引用，必须像从函数中返回一个指针一样对待。当函数返回值时，引用关联的内存一定要存在  

* 不能返回局部变量的引用

  ```c++
  int& funcA()
  {
      int a=10;   //局部变量 创建在栈上--寿命有限
      return a;
  }
  void test()
  {
      int &b=funcA();
  	cout << "ref = " << ref << endl;   //结果：ref=10
  	cout << "ref = " << ref << endl;   //结果：ref=0
  }
  ```

* 如果函数做左值，必须返回引用

  ```c++
  //返回静态变量的引用
  int& funcA()
  {
      static int a=10;   //局部静态变量：寿命（文件结束）、作用域（定义函数内）
      cout<<"a="<<a<<endl;
      return a;
  }
  void test()
  {
      funcA()=50;   //函数做左值    //结果：a=10
      funcA();                     //结果：a=50
  }
  ```

# 引用的本质

引用本质在C++内部实现是一个**常量指针**---指针的指向不可以修改，值可以修改

常量指针：`Type * const point`

```c++
Type& ref = val;     // Type* const ref = &val;
```

因此引用所占用的空间大小与指针相同

```c++
//发现是引用，转换为 int* const ref = &a;
void testFunc(int& ref)
{
	ref = 100; // ref是引用，转换为*ref = 100
}
int main()
{
	int a = 10;
	int& aRef = a;  //自动转换为 int* const aRef = &a;这也能说明引用为什么必须初始化
	aRef = 20;      //内部发现aRef是引用，自动帮我们转换为: *aRef = 20;
	cout << "a:" << a << endl;
	cout << "aRef:" << aRef << endl;
	testFunc(a);
	return EXIT_SUCCESS;
}
```

# 指针引用

在c语言中如果想改变一个指针的指向而不是它所指向的内容，就必须要用更高级的指针去接收低级指针

函数申明可以是这样的：`int funcA(int **);`

给指针变量取一个别名

```c++
Type * pointer=NULL:
Type* &=pointer;
```

## 1、指针间接修改

主调函数未分配内存，被调函数分配内存，传参时要用高级指针接收

```c++
struct Teacher
{
    int m_Age;
};
//指针间接修改teacher的年龄
void AllocateAndInitByPointer(Teacher**teacher)   //被调函数用二级指针接收
{
    *teacher=(Teacher *)malloc(sizeof(Teacher));
    (*teacher)->m_Age=35;
}
void test()
{
    Teacher *teacher=NULL;   //主调函数未分配内存
    AllocateAndInitByPointer(&teacher);
    cout<<teacher->m_Age<<endl;
}
```

## 2、引用间接修改

```c++
struct Student
{
    int m_Age;
};
void AllocateAndInitByReference(Student* &stu)
{
    stu=(Student *)malloc(sizeof(Student));
    stu->m_Age=45;
}
void test()
{
    Student *student=NULL;
    AllocateAndInitByReference(student);
    cout<<student->m_Age<<endl;
    free(student);
    student=NULL;
}
```

# 常引用

## 1、常引用的定义格式

`const Type &ref=val;` 

```c++
const int &ref = 10; // 加了const之后，  相当于写成   int temp = 10;  const int &ref = temp;
int *p = (int *)&ref;
*p = 10000;
cout << ref << endl;
```

## 2、常引用注意事项

* 字面量不能赋值给引用，但可以赋值给常引用  

  ```c++
  int &ref=100;   //字面量给引用赋值（错误）
  const int &ref=10;   //字面量可以给常引用值
  ```

* 常引用不能修改

  ```c++
  int m=100;
  int &ret=m;
  ret=50;
  cout<<"m="<<m<<endl;
  
  const int &ref=m;
  ref=80;   //修改常引用（错误）
  cout<<"m="<<m<<endl;
  ```

## 3、常引用的使用场景

函数传参，防止实参被修改（类的拷贝，复制构造函数）

```c++
//1、一般引用
int funcA(int &val) 
{
    val=1000;      
    return val;
}
//2、常引用
int funcB(const int &val)
{
    val=100;        //直接报错--不允许修改
    return val;
}
void test()
{
    int m=15;
    cout<<"m="<<funcB(m)<<endl;   //无法修改---结果：m=15
    cout<<"m="<<funcA(m)<<endl;   //被修改-----结果：m=1000
}
```

将函数的形参定义为常量引用的好处:

* 引用不产生新的变量，减少形参与实参传递时的开销。
* 由于引用可能导致实参随形参改变而改变，将其定义为常量引用可以消除这种副作用。