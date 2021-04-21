---
typora-root-url: Image
---

# deque容器基本概念

Vector容器是单向开口的连续内存空间，deque则是一种双向开口的连续线性空间。

所谓的双向开口，意思是可以在头尾两端分别做元素的插入和删除操作.

当然，vector容器也可以在头尾两端插入元素，但是在其头部操作效率奇差，无法被接受。

<img src="/deque容器.png" alt="deque容器" style="zoom:80%;" />

Deque容器和vector容器的差异：

* 一在于deque允许使用常数项时间对头端进行元素的插入和删除操作。
* 二在于deque没有容量的概念，因为它是动态的以分段连续空间组合而成，随时可以增加一段新的空间并链接起来，换句话说，像vector那样，”旧空间不足而重新配置一块更大空间，然后复制元素，再释放旧空间”这样的事情在deque身上是不会发生的。也因此，deque没有必须要提供所谓的空间保留(reserve)功能.

# deque容器实现原理

Deque容器是连续的空间，至少逻辑上看来如此，连续现行空间总是令我们联想到array和vector,array无法成长，vector虽可成长，却只能向尾端成长，而且**其成长其实是一个假象**，事实上(1) 申请更大空间 (2)原数据复制新空间 (3)释放原空间 三步骤，如果不是vector每次配置新的空间时都留有余裕，其成长假象所带来的代价是非常昂贵的。

Deque是由一段一段的定量的连续空间构成。一旦有必要在deque前端或者尾端增加新的空间，便配置一段连续定量的空间，串接在deque的头端或者尾端。Deque最大的工作就是维护这些**分段连续的内存空间的整体性的假象**，并提供随机存取的接口，避开了重新配置空间，复制，释放的轮回，代价就是复杂的迭代器架构。

既然deque是**分段连续内存空间**，那么就必须有**中央控制**，**维持整体连续的假象**，数据结构的设计及迭代器的前进后退操作颇为繁琐。Deque代码的实现远比vector或list都多得多。

Deque采取一块所谓的map(注意，不是STL的map容器)作为主控，这里所谓的map是一小块连续的内存空间，其中每一个元素(此处成为一个结点)都是一个指针，指向另一段连续性内存空间，称作缓冲区。缓冲区才是deque的存储空间的主体。

<img src="/deque容器实现原理.png" alt="deque容器实现原理" style="zoom: 67%;" />

# deque常用API

## 1、deque容器遍历

```c++
void Print(deque<int> &d){
	for(deque<int>::iterator it=d.begin();it!=d.end();it++){
		cout<<*it<<" ";
	}
	cout<<endl;
}
```

## 2、deque构造函数

| 序号 | 原型                     | 注释                                         |
| ---- | ------------------------ | -------------------------------------------- |
| 1    | deque<T> deqT;           | 默认构造形式                                 |
| 2    | deque(beg, end);         | 构造函数将[beg, end)区间中的元素拷贝给本身。 |
| 3    | deque(n, elem);          | 构造函数将n个elem拷贝给本身。                |
| 4    | deque(const deque &deq); | 拷贝构造函数。                               |

```c++
void test(){
	deque<int> d;            //deque<T> deqT;
	
	deque<int> d1(5,1);       //deque(n, elem);
	Print(d1); 
	
	deque<int> d2(d1.begin(),d1.end());     //deque(beg, end);
	Print(d2);
	
	deque<int> d3(d2);           //deque(const deque &deq);
	Print(d3);
}
```

## 3、赋值操作

| 序号 | 原型                                | 注释                                     |
| ---- | ----------------------------------- | ---------------------------------------- |
| 1    | assign(beg, end);                   | 将[beg, end)区间中的数据拷贝赋值给本身。 |
| 2    | assign(n, elem);                    | 将n个elem拷贝赋值给本身。                |
| 3    | deque& operator=(const deque &deq); | 重载等号操作符                           |
| 4    | swap(deq);                          | 将deq与本身的元素互换                    |

```c++
void test(){	
	deque<int> d1(5,1);
	deque<int> d2;
	d2.assign(d1.begin(),d1.end());       //assign(beg, end);
	Print(d2);
	
	deque<int> d3;
	d3.assign(5,2);                     //assign(n, elem);
	Print(d3);
	
	deque<int> d4=d3;                   //deque& operator=(const deque &deq);
	Print(d4);
	
	d1.swap(d4);                     //swap(deq);
	Print(d1);
	Print(d4);
}
```

## 4、deque大小操作

| 序号 | 原型                     | 注释                                                         |
| ---- | ------------------------ | ------------------------------------------------------------ |
| 1    | deque.size();            | 返回容器中元素的个数                                         |
| 2    | deque.empty();           | 判断容器是否为空                                             |
| 3    | deque.resize(num);       | 重新指定容器的长度为num,若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。 |
| 4    | deque.resize(num, elem); | 重新指定容器的长度为num,若容器变长，则以elem值填充新位置,如果容器变短，则末尾超出容器长度的元素被删除 |

```c++
void test(){	
	deque<int> d1(5,1);
	cout<<"d1.size="<<d1.size()<<endl;        //deque.size();
	Print(d1);
	
	if(!d1.empty())                         //deque.empty();
		cout<<"d1非空"<<endl;
	else 
		cout<<"d1为空"<<endl;
	
	d1.resize(10);                           //deque.resize(num);
	cout<<"d1.size="<<d1.size()<<endl;
	Print(d1);
	
	d1.resize(15,2);                         //deque.resize(num, elem); 
	cout<<"d1.size="<<d1.size()<<endl;
	Print(d1);
}
```

## 5、deque双端插入和删除操作

| 序号 | 原型              | 注释                   |
| ---- | ----------------- | ---------------------- |
| 1    | push_back(elem);  | 在容器尾部添加一个数据 |
| 2    | push_front(elem); | 在容器头部插入一个数据 |
| 3    | pop_back();       | 删除容器最后一个数据   |
| 4    | pop_front();      | 删除容器第一个数据     |

```c++
void test(){	
	deque<int> d1(5,1);
	Print(d1);
	
	d1.push_back(2);             //push_back(elem);
	d1.push_front(0);            //push_front(elem);
	Print(d1);
	
	d1.pop_back();           //pop_back();
	d1.pop_front();          //pop_front();
	Print(d1); 
}
```

## 6、deque数据存取

| 序号 | 原型         | 注释                                                       |
| ---- | ------------ | ---------------------------------------------------------- |
| 1    | at(idx);     | 返回索引idx所指的数据，如果idx越界，抛出out_of_range。     |
| 2    | operator[ ]; | 返回索引idx所指的数据，如果idx越界，不抛出异常，直接出错。 |
| 3    | front();     | 返回第一个数据。                                           |
| 4    | back();      | 返回最后一个数据                                           |

```c++
void test(){	
	deque<int> d1;
	for(int i=0;i<5;i++){
		d1.push_back(i+1);
	}
	Print(d1);
	
	cout<<"d1.at(3)="<<d1.at(3)<<endl;       //at(idx);
	cout<<"d1[2]="<<d1[2]<<endl;             //operator[ ];
	
	cout<<"d1.front="<<d1.front()<<endl;      //front();
	cout<<"d1.back="<<d1.back()<<endl;        //back();
}
```

## 7、deque插入操作

| 序号 | 原型                 | 注释:参数pos为迭代器                                |
| ---- | -------------------- | --------------------------------------------------- |
| 1    | insert(pos,elem);    | 在pos位置插入一个elem元素的拷贝，返回新数据的位置。 |
| 2    | insert(pos,n,elem);  | 在pos位置插入n个elem数据，无返回值。                |
| 3    | insert(pos,beg,end); | 在pos位置插入[beg,end)区间的数据，无返回值。        |

```c++
void test(){	
	deque<int> d1;
	for(int i=0;i<5;i++){
		d1.push_back(i+1);
	}
	
	d1.insert(d1.begin()+3,8);               //insert(pos,elem);
	Print(d1);
	
	d1.insert(d1.begin()+4,3,10);            //insert(pos,n,elem);
	Print(d1);
	
	d1.insert(d1.begin()+2,d1.begin(),d1.end());    //insert(pos,beg,end);
	Print(d1);
}
```

## 8、deque删除操作

| 序号 | 原型            | 注释:参数pos是迭代器                            |
| ---- | --------------- | ----------------------------------------------- |
| 1    | clear();        | 移除容器的所有数据                              |
| 2    | erase(beg,end); | 删除[beg,end)区间的数据，返回下一个数据的位置。 |
| 3    | erase(pos);     | 删除pos位置的数据，返回下一个数据的位置。       |

```c++
void test(){	
	deque<int> d1;
	for(int i=0;i<10;i++){
		d1.push_back(i+1);
	}
	Print(d1);
	
	d1.erase(d1.begin()+3,d1.end()-4);        //erase(beg,end);
	Print(d1);
	
	d1.erase(d1.begin()+2);                   //erase(pos);
	Print(d1);
	
	d1.clear();         //clear();
	Print(d1);
}
```

## 9、排序

```c++
#include <deque>
#include <algorithm>	
#include <ctime>
#include <Windows.h>

void Print(deque<int> &d){
	for(deque<int>::iterator it=d.begin();it!=d.end();it++){
		cout<<*it<<" ";
	}
	cout<<endl;
}

int MyCompare(int v1,int v2){   //自定义排序规则--从大到小
	return v1>v2;
}

void test()
{
	srand((unsigned)time(NULL));
	deque<int> d;
	for(int i=0;i<10;i++){
		d.push_back(rand()%10);
	}
	Print(d);
	
	sort(d.begin(),d.end());    //默认排序--从小到大
	Print(d);
	
	sort(d.begin(),d.end(),MyCompare);
	Print(d);
}
```

# 案例---评委打分

有5名选手：选手ABCDE，10个评委分别对每一名选手打分，去除最高分，去除评委中最低分，取平均分。

* 创建五名选手，放到vector中
* 遍历vector容器，取出来每一个选手，执行for循环，可以把10个评分打分存到deque容器中
* sort算法对deque容器中分数排序，pop_back pop_front去除最高和最低分
* deque容器遍历一遍，累加分数，累加分数/d.size()
* person.score = 平均分

```c++
#include <string>
#include <deque>
#include <vector>
#include <ctime>
#include <Windows.h>
#include <algorithm>

class Player{
public:
	Player(string name,double score=0){
		this->m_Name=name;
		this->m_Score=score;
	}
	string m_Name;    //姓名 
	double m_Score;   //平均分 
};

void CreatPlayers(vector<Player> &v){   //创建5名选手放进vector容器 
	string NameBeed="ABCDE";
	for(int i=0;i<5;i++){
		string name="选手";
		name+=NameBeed[i];
		Player p(name);
		v.push_back(p);
	}
}

void Set_Score(vector<Player> &v){    //10位评委分别给5名选手打分 
	cout<<"评委打分情况："<<endl;
	for(vector<Player>::iterator it=v.begin();it!=v.end();it++){   //遍历5名选手 
		deque<int> d;   
		double sum=0;   //总成绩 
		for(int i=0;i<10;i++){
			int score=rand()%40+60;
			d.push_back(score);     //评委打分放进deque容器 
		}
		
		cout<<(*it).m_Name<<":";
		for(deque<int>::iterator it1=d.begin();it1!=d.end();it1++){
			cout<<*it1<<" ";    //输出选手的得分情况 
		}
		cout<<endl;
		
		sort(d.begin(),d.end());       //对成绩进行排序（默认：从小到大） 
		d.pop_back();    //去除最高分 
		d.pop_front();   //去除最低分 
		cout<<"处理后:";
		for(deque<int>::iterator it2=d.begin();it2!=d.end();it2++){
			cout<<*it2<<" ";     //输出处理后选手的得分情况 
			sum+=(*it2);       //计算选手的总成绩 
		}
		cout<<endl<<endl;
		
		(*it).m_Score=sum/d.size();    //计算选手的平均成绩 
	}
}

void ShowResult(vector<Player> &v){    //输出最终结果 
	cout<<endl<<"结果如下："<<endl;
	for(vector<Player>::iterator it=v.begin();it!=v.end();it++){
		cout<<"姓名："<<(*it).m_Name<<"  平均成绩："<<(*it).m_Score<<endl;
	}
	cout<<endl;
}

void test(){![评委打分](/评委打分.png)
    //设置随机数种子
	srand((unsigned int)time(NULL));
	vector<Player> v;   
	CreatPlayers(v);	//创建选手	
	Set_Score(v);       //打分
	ShowResult(v);      //结果
}
```

<img src="/评委打分.png" alt="评委打分" style="zoom:67%;" />

