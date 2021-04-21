---
typora-root-url: Image
---

# queue容器

Queue是一种**先进先出**(First In First Out,FIFO)的数据结构，它有两个出口，queue容器允许从一端新增元素，从另一端移除元素。

<img src="/queue容器.png" alt="queue容器" style="zoom:67%;" />

# queue没有迭代器

Queue所有元素的进出都必须符合”先进先出”的条件，只有queue的顶端元素，才有机会被外界取用。**Queue不提供遍历功能，也不提供迭代器**。

# queue常用API

## 1、queue构造函数

| 序号 | 原型                     | 注释                                         |
| ---- | ------------------------ | -------------------------------------------- |
| 1    | queue<T> queT;           | queue采用模板类实现，queue对象的默认构造形式 |
| 2    | queue(const queue &que); | 拷贝构造函数                                 |

```c++
void test()
{
	queue<int> q;              //queue<T> queT;;----默认构造
	for(int i=0;i<10;i++){
		q.push(i);
	}
	
	queue<int> q1(q);         //queue(const queue &que);---拷贝构造函数
}
```

## 2、queue存取、插入和删除操作

| 序号 | 原型        | 注释                 |
| ---- | ----------- | -------------------- |
| 1    | push(elem); | 往队尾添加元素       |
| 2    | pop();      | 从队头移除第一个元素 |
| 3    | back();     | 返回最后一个元素     |
| 4    | front();    | 返回第一个元素       |

```c++
void test()
{
	queue<int> q;              //queue<T> queT;;----默认构造
	for(int i=0;i<10;i++){
		q.push(i);            //push(elem);----往队尾添加元素
	}
	cout<<"front="<<q.front()<<endl;    //front();----返回第一个元素
	cout<<"back="<<q.back()<<endl;       //back();-------返回最后一个元素
	q.pop();                          //pop();-------从队头移除第一个元素
	cout<<"front="<<q.front()<<endl;
	
}
```

## 3、queue赋值操作

`queue& operator=(const queue &que);`                         重载等号操作符

```c++
void test()
{
	queue<int> q;              //queue<T> queT;;----默认构造
	for(int i=0;i<10;i++){
		q.push(i);
	}
	
	queue<int> q1;
	q1=q;              //queue& operator=(const queue &que);----重载等号操作符
}
```

## 4、queue大小操作

| 序号 | 原型     | 注释             |
| ---- | -------- | ---------------- |
| 1    | empty(); | 判断队列是否为空 |
| 2    | size();  | 返回队列的大小   |

```c++
void test()
{
	queue<int> q;              //queue<T> queT;;----默认构造
	for(int i=0;i<10;i++){
		q.push(i);
	}
	if(!q.empty()){                  //empty();-------判断队列是否为空
		cout<<"queue非空"<<endl;
		cout<<"size="<<q.size()<<endl;     //size();----返回队列的大小
	}
	else
		cout<<"queue为空"<<endl; 
}
```

