---
typora-root-url: Image
---

# stack容器基本概念

stack是一种**先进后出**(First In Last Out,FILO)的数据结构，它只有一个出口，形式如图所示。stack容器允许新增元素，移除元素，取得栈顶元素，但是除了最顶端外，没有任何其他方法可以存取stack的其他元素。换言之，**stack不允许有遍历行为**。

有元素推入栈的操作称为:push,将元素推出stack的操作称为pop.

<img src="/stack容器.png" alt="stack容器" style="zoom: 67%;" />

# stack没有迭代器

Stack所有元素的进出都必须符合”先进后出”的条件，只有stack顶端的元素，才有机会被外界取用。**Stack不提供遍历功能，也不提供迭代器。**

# stack常用API

## 1、stack构造函数

| 序号 | 原型                     | 注释                                            |
| ---- | ------------------------ | ----------------------------------------------- |
| 1    | stack<T> stkT;           | stack采用模板类实现， stack对象的默认构造形式： |
| 2    | stack(const stack &stk); | 拷贝构造函数                                    |

```c++
void test()
{
	stack<int> s;              //stack<T> stkT;----默认构造
	for(int i=0;i<10;i++){
		s.push(i);
	}
	
	stack<int> s1(s);         //stack(const stack &stk);---拷贝构造函数
}
```

## 2、stack赋值操作

`stack& operator=(const stack &stk);`                       重载等号操作符

```c++
void test()
{
	stack<int> s;
	for(int i=0;i<10;i++){
		s.push(i);
	}
	
	stack<int> s1;
	s1=s;              //`stack& operator=(const stack &stk);---重载等号操作符
}
```

## 3、stack数据存取操作

| 序号 | 原型        | 注释                 |
| ---- | ----------- | -------------------- |
| 1    | push(elem); | 向栈顶添加元素       |
| 2    | pop();      | 从栈顶移除第一个元素 |
| 3    | top();      | 返回栈顶元素         |

```c++
void test()
{
	stack<int> s;
	for(int i=0;i<10;i++){
		s.push(i);                 //push(elem);-----向栈顶添加元素
	}
	
	cout<<"栈顶："<<s.top()<<endl;          //top();----返回栈顶元素
	s.pop();                          //pop();----从栈顶移除第一个元素
	cout<<"栈顶："<<s.top()<<endl; 
}
```

## 4、 stack大小操作

| 序号 | 原型     | 注释             |
| ---- | -------- | ---------------- |
| 1    | empty(); | 判断堆栈是否为空 |
| 2    | size();  | 返回堆栈的大小   |

```c++
void test()
{
	stack<int> s;
	for(int i=0;i<10;i++){
		s.push(i);
	}
	if(!s.empty()){            //empty();------判断堆栈是否为空
		cout<<"stack非空"<<endl;
		cout<<"size="<<s.size()<<endl;       //size();----返回堆栈的大小
	}
	else
		cout<<"stack为空"<<endl;
}
```

