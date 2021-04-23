---
typora-root-url: Image
---

# list容器基本概念

链表是一种物理存储单元上非连续、非顺序的存储结构，数据元素的逻辑顺序是通过链表中的指针链接次序实现的。链表由一系列结点（链表中每一个元素称为结点）组成，结点可以在运行时动态生成。每个结点包括两个部分：一个是存储数据元素的数据域，另一个是存储下一个结点地址的指针域。

相较于vector的连续线性空间，list就显得负责许多，它的好处是每次插入或者删除一个元素，就是配置或者释放一个元素的空间。因此，list对于空间的运用有绝对的精准，一点也不浪费。而且，对于任何位置的元素插入或元素的移除，list永远是常数时间。

List和vector是两个最常被使用的容器。

List容器是一个双向链表。

<img src="/list容器.png" alt="list容器" style="zoom: 50%;" />

* n采用动态存储分配，不会造成内存浪费和溢出
* 链表执行插入和删除操作十分方便，修改指针即可，不需要移动大量元素
* 链表灵活，但是空间和时间额外耗费较大

# list容器的迭代器

List容器不能像vector一样以普通指针作为迭代器，因为其节点不能保证在同一块连续的内存空间上。List迭代器必须有能力指向list的节点，并有能力进行正确的递增、递减、取值、成员存取操作。

所谓”list正确的递增，递减、取值、成员取用”是指，递增时指向下一个节点，递减时指向上一个节点，取值时取的是节点的数据值，成员取用时取的是节点的成员。

由于list是一个双向链表，迭代器必须能够具备前移、后移的能力，所以list容器提供的是Bidirectional Iterators.

List有一个重要的性质，插入操作和删除操作都不会造成原有list迭代器的失效。这在vector是不成立的，因为vector的插入操作可能造成记忆体重新配置，导致原有的迭代器全部失效，甚至List元素的删除，也只有被删除的那个元素的迭代器失效，其他迭代器不受任何影响。

# list容器的数据结构

## 1、list容器不仅是一个双向链表，而且还是一个循环的双向链表。

```c++
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<list>
using namespace std;

int main(){
	list<int> myList;
	for (int i = 0; i < 10; i ++){
		myList.push_back(i);
	}

	list<int>::_Nodeptr node =  myList._Myhead->_Next;

	for (int i = 0; i < myList._Mysize * 2;i++){
		cout << "Node:" << node->_Myval << endl;
		node = node->_Next;
		if (node == myList._Myhead){
			node = node->_Next;
		}
	}

	system("pause");
	return EXIT_SUCCESS;
}
```

分析：循环打印list里面的数据（两遍）

## 2、list迭代器不支持随机访问

```c++
void test(){
	list<int> L;
	for(int i=0;i<5;i++){
		L.push_back(rand()%10);
	}
	Print(L);
	
	list<int>::iterator itBegin = L.begin();
	//itBegin = itBegin + 1;
}
```

# list常用API

# 1、list容器遍历

1. 正序遍历

   ```c++
   //遍历list（正序） 
   void Print(list<int> &L){
   	for(list<int>::iterator it=L.begin();it!=L.end();it++){
   		cout<<*it<<" ";
   	}
   	cout<<endl;
   } 
   ```

2. 逆序遍历

   ```c++
   //遍历list（逆序） 
   void Print1(list<int> &L){
   	for(list<int>::reverse_iterator it=L.rbegin();it!=L.rend();it++){
   		cout<<*it<<" ";
   	}
   	cout<<endl;
   }
   ```

## 1、list构造函数

| 序号 | 原型                   | 注释                                         |
| ---- | ---------------------- | -------------------------------------------- |
| 1    | list<T> lstT;          | list采用采用模板类实现,对象的默认构造形式：  |
| 2    | list(beg,end);         | 构造函数将[beg, end)区间中的元素拷贝给本身。 |
| 3    | list(n,elem);          | 构造函数将n个elem拷贝给本身。                |
| 4    | list(const list &lst); | 拷贝构造函数。                               |

```c++
void test(){
	list<int> myList;       //list<T> lstT;----默认构造
	
	list<int> myList1(5,1);       //list(n,elem);----构造函数将n个elem拷贝给本身。
	Print(myList1);
	
	list<int> myList2(myList1.begin(),myList1.end());  
           //list(beg,end);---构造函数将[beg, end)区间中的元素拷贝给本身
	Print(myList2);
	
	list<int> myList3(myList1);       //list(const list &lst);----拷贝构造函数。
	Print(myList3);
}
```

## 2、list数据元素插入和删除操作

| 序号 | 原型                 | 注释：参数pos是迭代器                           |
| ---- | -------------------- | ----------------------------------------------- |
| 1    | push_back(elem);     | 在容器尾部加入一个元素                          |
| 2    | pop_back();          | 删除容器中最后一个元素                          |
| 3    | push_front(elem);    | 在容器开头插入一个元素                          |
| 4    | pop_front();         | 从容器开头移除第一个元素                        |
| 5    | insert(pos,elem);    | 在pos位置插elem元素的拷贝，返回新数据的位置。   |
| 6    | insert(pos,n,elem);  | 在pos位置插入n个elem数据，无返回值。            |
| 7    | insert(pos,beg,end); | 在pos位置插入[beg,end)区间的数据，无返回值。    |
| 8    | clear();             | 移除容器的所有数据                              |
| 9    | erase(beg,end);      | 删除[beg,end)区间的数据，返回下一个数据的位置。 |
| 10   | erase(pos);          | 删除pos位置的数据，返回下一个数据的位置。       |
| 11   | remove(elem);        | 删除容器中所有与elem值匹配的元素。              |

```c++
void test(){
	list<int> L;
	for(int i=0;i<5;i++){
		L.push_back(i);       //push_back(elem);    ---在容器尾部加入一个元素
	}
	Print(L);
	
	L.push_front(8);    //push_front(elem);----在容器开头插入一个元素
	Print(L);
	
	L.pop_back();        //pop_back();----删除容器中最后一个元素
	L.pop_front();       //pop_front();----从容器开头移除第一个元素
	Print(L);
	
	L.insert(L.begin(),7);   //insert(pos,elem);----在pos位置插elem元素的拷贝，返回新数据的位置。
	L.insert(L.end(),4,1);   //insert(pos,n,elem);----在pos位置插入n个elem数据，无返回值。
	Print(L);
	
	list<int> L1;
	L1.insert(L1.begin(),L.begin(),L.end());  
              //insert(pos,beg,end);--在pos位置插入[beg,end)区间的数据，无返回值
	Print(L1);
	
	L.clear();      //clear();----移除容器的所有数据
	Print(L);
	
	L1.erase(L1.begin());    //erase(pos);----删除pos位置的数据，返回下一个数据的位置。
	Print(L1);
	
	L1.remove(3);     //remove(elem);---删除容器中所有与elem值匹配的元素。
	Print(L1);
	
	L1.erase(L1.begin(),L1.end());  //erase(beg,end);---删除[beg,end)区间的数据，返回下一个数据的位置。
	Print(L1);
}
```

## 3、list大小操作

| 序号 | 原型               | 注释                                                         |
| ---- | ------------------ | ------------------------------------------------------------ |
| 1    | size();            | 返回容器中元素的个数                                         |
| 2    | empty();           | 判断容器是否为空                                             |
| 3    | resize(num);       | 重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。 |
| 4    | resize(num, elem); | 重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。 |

```c++
void test(){
	list<int> L;
	for(int i=0;i<5;i++){
		L.push_back(i);
	}
	Print(L);
	
	if(!L.empty()){       //empty();----判断容器是否为空
		cout<<"list非空"<<endl;
		cout<<"size="<<L.size()<<endl;  //size();---返回容器中元素的个数
	}
	else
		cout<<"list为空"<<endl;
	
	L.resize(10);   //resize(num);
    //重新指定容器的长度为num，若容器变长，则以默认值填充新位置。若容器变短，则末尾超出容器长度的元素被删除。
	Print(L);
	cout<<"size="<<L.size()<<endl;
	
	L.resize(15,5);   //resize(num, elem);
    //重新指定容器的长度为num，若容器变长，则以elem值填充新位置。若容器变短，则末尾超出容器长度的元素被删除。
	Print(L);	
	cout<<"size="<<L.size()<<endl;
}
```

## 4、list赋值操作

| 序号 | 原型                              | 注释                                     |
| ---- | --------------------------------- | ---------------------------------------- |
| 1    | assign(beg, end);                 | 将[beg, end)区间中的数据拷贝赋值给本身。 |
| 2    | assign(n, elem);                  | 将n个elem拷贝赋值给本身。                |
| 3    | list& operator=(const list &lst); | 重载等号操作符                           |
| 4    | swap(lst);                        | 将lst与本身的元素互换。                  |

```c++
void test(){
	list<int> L;
	for(int i=0;i<5;i++){
		L.push_back(i);
	}
	Print(L);
	
	list<int> L1;
	L1.assign(7,2);   //assign(n, elem);---将n个elem拷贝赋值给本身。
	Print(L1);
	
	list<int> L2;
	L2.assign(L.begin(),L.end());   //assign(beg, end);---将[beg, end)区间中的数据拷贝赋值给本身。
	Print(L2);
	
	list<int> L3=L2;    //list& operator=(const list &lst);---重载等号操作符
	Print(L3);
	
	L.swap(L1);   //swap(lst);----将lst与本身的元素互换。
	Print(L);
	Print(L1);
}
```

## 5、 list数据的存取

| 序号 | 原型     | 注释               |
| ---- | -------- | ------------------ |
| 1    | front(); | 返回第一个元素。   |
| 2    | back();  | 返回最后一个元素。 |

```c++
void test(){
	list<int> L;
	for(int i=0;i<5;i++){
		L.push_back(i+1);
	}
	Print(L);
	
	cout<<"front="<<L.front()<<endl;      //front();-----返回第一个元素。
	cout<<"back="<<L.back()<<endl;        //back();-----返回最后一个元素。
}
```

## 6、list反转排序

| 序号 | 原型       | 注释：质变                                                   |
| ---- | ---------- | ------------------------------------------------------------ |
| 1    | reverse(); | 反转链表，比如lst包含1,3,5元素，运行此方法后，lst就包含5,3,1元素。 |
| 2    | sort();    | list排序：默认（从小到大）                                   |

```c++
//自定义排序规则--（从大到小） 
int MyCompare(int v1,int v2){
	return v1>v2;
}
void test(){
	list<int> L;
	for(int i=0;i<5;i++){
		L.push_back(rand()%10);
	}
	Print(L);
	
	L.reverse();   //reverse();-----反转链表，比如lst包含1,3,5元素，运行此方法后，lst就包含5,3,1元素。
	Print(L);
	
	L.sort();         //sort();     //list排序：默认（从小到大）
	Print(L);
	
	L.sort(MyCompare);
	//L.reverse();
	Print(L);
}
```

# 案例---学生按年龄排序

```c++
class Student{
public:
	Student(string name,int age,int height){
		this->m_Name=name;
		this->m_Age=age;
		this->m_Height=height;
	}
	string m_Name;
	int m_Age;
	int m_Height;
};

Creat_Student(list<Student> &stu){
	Student stu1("亚瑟", 40 ,  180);
	Student stu2("赵云", 20 , 160);
	Student stu3("妲己", 30 , 120);
	Student stu4("韩信", 50 , 200);
	Student stu5("关羽", 10 ,  170);
	Student stu6("刘备", 10  , 165);
	Student stu7("张飞", 10 , 185);
	stu.push_back(stu1);
	stu.push_back(stu2);
	stu.push_back(stu3);
	stu.push_back(stu4);
	stu.push_back(stu5);
	stu.push_back(stu6);
	stu.push_back(stu7);
}

void Print(list<Student> &stu){
	for(list<Student>::iterator it=stu.begin();it!=stu.end();it++){
		cout<<"姓名："<<(*it).m_Name<<" 年龄："<<(*it).m_Age<<" 身高："<<(*it).m_Height<<endl;
	}
	cout<<endl;
}

//自定义排序规则--（从大到小） 
int MyCompare(Student stu1,Student stu2){
	if(stu1.m_Age==stu2.m_Age)
		return stu1.m_Height>stu2.m_Height;     //年龄相等的按身高排序 
	return stu1.m_Age>stu2.m_Age;
}

void test(){
	list<Student> stu;
	Creat_Student(stu);
	Print(stu);
	
	stu.sort(MyCompare);
	Print(stu);
}
```

