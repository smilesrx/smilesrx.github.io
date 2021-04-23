# 函数的默认参数

c++在声明函数原型的时可为一个或者多个参数指定默认(缺省)的参数值，当函数调用的时候如果没有指定这个值，编译器会自动用默认值代替。

注意：

* 函数的默认参数是从左至右的，如果某一位置的参数设置了默认参数，那么该参数之后的参数都必须设置默认参数
* 如果函数申明和定义是分开的，那么申明和定义不能同时设置默认参数

```c++
void funcA(int a,int b,int c)
{
    cout<<"funcA函数调用"<<endl;
    cout<<"a="<<a<<"  b="<<b<<"  c="<<c<<endl;
}

void funcB(int a,int b=5,int c=10)
{
    cout<<"funcB函数调用"<<endl;
    cout<<"a="<<a<<"  b="<<b<<"  c="<<c<<endl;
}

void funcC(int a=5,int b=15,int c=20)
{
    cout<<"funC函数调用"<<endl;
    cout<<"a="<<a<<"  b="<<b<<"  c="<<c<<endl;
}

void test()
{
    int a=1;
    int b=2;
    int c=3;
    funcA(a,b,c);   //结果：a=1  b=2  c=3 
    
    funcB(a,b,c);   //结果：a=1  b=2  c=3 
    funcB(a,b);     //结果：a=1  b=2  c=10 
    funcB(a);       //结果：a=1  b=5  c=10 
    
    funcC();        //结果：a=5  b=15  c=20   
    funcC(a);       //结果：a=1  b=15  c=20   
    funcC(a,b);     //结果：a=1  b=2  c=20  
    funcC(a,b,c);   //结果：a=1  b=2  c=3 
}
```

# 函数的占位参数

* c++在声明和定义函数时，可以设置占位参数。

  ```c++
  //申明
  void func(int a,int b,int);
  //实现
  void func(int a,int b,int)
  {
      cout<<"a+b="<<a+b<<endl;
  }
  ```

* 占位参数只有参数类型声明，而没有参数名声明。

* 一般情况下，在函数体内部无法使用占位参数。

* 占位参数也可以设置默认值

  ```c++
  void func(int a=5,int b=10,int=15)
  {
     cout<<"a+b="<<a+b<<endl;
  }
  ```

* 占位参数也是参数调用时如未设置默认参数必须传占位参数的值

  ```c++
  void func(int a,int b,int)
  {
      cout<<"a+b="<<a+b<<endl;
  }
  void test()
  {
      func(5,10,15);
  }
  ```

# 函数重载

条件：

1. 作用域相同

2. 函数名相同

3. 参数表不同（参数类型、参数个数、参数顺序）

4. 参数加`const`和不加`const`的引用也能做重载条件

   ```c++
   void func(const int &a)
   {
       cout<<"常引用下a="<<a<<endl;
   }
   
   void func(int &a)
   {
       cout<<"一般引用下a="<<a<<endl;
   }
   
   void test()
   {
       int a=10;
       func(a);    //结果：一般引用下a=10
       func(5);    //结果：常引用下a=5
   }
   ```

返回值类型不做重载条件

```c++
void func(){cout<<"func无参函数调用"<<endl;}
//int func(){cout<<"func无参函数调用"<<endl;rerurn 0;}   //返回值类型不做重载条件
void func(int a){cout<<"a="<<a<<endl;}
void func(string str){cout<<"str="<<str<<endl;}
void func(int a,string str){cout<<"a="<<a<<"  str="<<str<<endl;}
void func(string str,int a){cout<<"str="<<str<<"  a="<<a<<endl;}

void test()
{
    func();           //结果：func无参函数调用  
    func(5);           //结果：a=5  
    func("hello");     //结果：str=hello  
    func(5,"hello");   //结果：a=5  str=hello 
    func("hello",5);   //结果：str=hello  a=5 
}
```

# 函数重载碰到默认参数

注意：切记避免二义性

```c++
void func(int a=5)
{
    cout<<"a="<<a<<endl;
}

void func()
{
    cout<<"我就是个坑，而你非要跳进来"<<endl;
}

void test()
{
    func();    //调用时两个函数都能匹配不知道调用哪一个（产生二义性）
}
```

# extern "C"

由于c++中需要支持函数重载，所以c和c++中对同一个函数经过编译后生成的函数名是不相同的，这就导致了一个问题，如果在c++中调用一个使用c语言编写模块中的某个函数，那么c++是根据c++的名称修饰方式来查找并链接这个函数，那么就会发生链接错误(找不到这个函数)

用途：在C++中调用C语言文件

解决方法：

* 在C++文件中加`extern "C" void show();`告诉编译器  show函数按c语言的方式进行**编译**和**链接**，而不是按c++的方式

* 在C语言头文件中加

  ```c
  #if __cplusplus
  extern "C"{
  #endif
      
  #if __cplusplus
  }
  #endif
  ```

  

