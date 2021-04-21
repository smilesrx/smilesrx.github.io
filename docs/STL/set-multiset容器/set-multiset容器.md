---

---

# set容器基本概念

Set的特性是。所有元素都会根据元素的键值**自动被排序**。Set的元素不像map那样可以同时拥有实值和键值，set的元素即是键值又是实值。**Set不允许两个元素有相同的键值**。

我们可以通过set的迭代器改变set元素的值吗？不行，因为set元素值就是其键值，关系到set元素的排序规则。如果任意改变set元素值，会严重破坏set组织。换句话说，**set的iterator是一种const_iterator.**

set拥有和list某些相同的性质，当对容器中的元素进行插入操作或者删除操作的时候，操作之前所有的迭代器，在操作完成之后依然有效，被删除的那个元素的迭代器必然是一个例外。

# multiset容器基本概念

multiset特性及用法和set完全相同，唯一的差别在于它**允许键值重复**。set和multiset的底层实现是红黑树，红黑树为平衡二叉树的一种。

# 树的简单知识

* 二叉树就是任何节点最多只允许有两个字节点。分别是左子结点和右子节点。

  <img src="E:\Users\Rong MSI\Desktop\C++\笔记\STL\set-multiset容器\Image\二叉树.png" alt="二叉树" style="zoom: 67%;" />

* 二叉搜索树:

  是指二叉树中的节点按照一定的规则进行排序，使得对二叉树中元素访问更加高效。

  二叉搜索树的放置规则是：任何节点的元素值一定大于其左子树中的每一个节点的元素值，并且小于其右子树的值。因此从根节点一直向左走，一直到无路可走，即得到最小值，一直向右走，直至无路可走，可得到最大值。那么在二叉搜索树中找到最大元素和最小元素是非常简单的事情。

  <img src="E:\Users\Rong MSI\Desktop\C++\笔记\STL\set-multiset容器\Image\二叉搜索树.png" alt="二叉搜索树" style="zoom: 80%;" />

# set/multiset常用API

## 1、set构造函数

| 序号 | 原型                | 注释                 |
| ---- | ------------------- | -------------------- |
| 1    | set<T> st;          | set默认构造函数：    |
| 2    | mulitset<T> mst;    | multiset默认构造函数 |
| 3    | set(const set &st); | 拷贝构造函数         |

```c++
void test(){
	set<int> s;            //set<T> st;----set默认构造函数：
	for(int i=1;i<=5;i++){
		s.insert(i*10);
	} 
	Print(s);
	
	set<int> s2(s);          //set(const set &st);----拷贝构造函数
	Print(s2);
	 
	multiset<int> s1;         //mulitset<T> mst;----multiset默认构造函数
	for(int i=0;i<10;i++){
		s1.insert(rand()%10);
	}
	Print1(s1);
}
```

## 2、set赋值操作

| 序号 | 原型                           | 注释             |
| ---- | ------------------------------ | ---------------- |
| 1    | set& operator=(const set &st); | 重载等号操作符   |
| 2    | swap(st);                      | 交换两个集合容器 |

```c++
void test(){
	set<int> s;
	for(int i=1;i<=5;i++){
		s.insert(i*10);
	} 
	Print(s);
	
	set<int> s1=s;      //set& operator=(const set &st);---重载等号操作符
	Print(s1);
	
	set<int> s2;
	for(int i=1;i<=5;i++){
		s2.insert(i*100);
	} 
	Print(s2);
	
	s1.swap(s2);    //swap(st);----交换两个集合容器
	Print(s1);
	Print(s2);

```

## 3、set大小操作

| 序号 | 原型     | 注释                 |
| ---- | -------- | -------------------- |
| 1    | size();  | 返回容器中元素的数目 |
| 2    | empty(); | 判断容器是否为空     |

```c++
void test(){
	set<int> s;
	for(int i=1;i<=5;i++){
		s.insert(i*10);
	} 
	Print(s);
	
	if(!s.empty()){                       //empty();-----判断容器是否为空
		cout<<"set非空"<<endl;
		cout<<"size="<<s.size()<<endl;    //size();----返回容器中元素的数目
	}
	else
		cout<<"set为空"<<endl;
}
```

## 4、插入和删除操作

| 序号 | 原型             | 注释                                                   |
| ---- | ---------------- | ------------------------------------------------------ |
| 1    | insert(elem);    | 在容器中插入元素。返回值：pair<迭代器，bool>---对组    |
| 2    | clear();         | //清除所有元素                                         |
| 3    | erase(pos);      | 删除pos迭代器所指的元素，返回下一个元素的迭代器。      |
| 4    | erase(beg, end); | 删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。 |
| 5    | erase(elem);     | 删除容器中值为elem的元素。                             |

```c++
void test(){
	set<int> s;
	for(int i=1;i<=5;i++){
		s.insert(i*10);    //insert(elem);----在容器中插入元素。
	} 
	Print(s);
	
	set<int> s1(s);
	Print(s);
	
	s.clear();   //clear();---清除所有元素
	Print(s);
	
	s1.erase(s1.begin());   //erase(pos);---删除pos迭代器所指的元素，返回下一个元素的迭代器。
	Print(s1);
	
	s1.erase(30);    //erase(elem);----删除容器中值为elem的元素。
	Print(s1);
	
	s1.erase(s1.begin(),s1.end());//erase(beg, end);删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	Print(s1);
}
```

## 5、set查找操作

| 序号 | 原型                  | 注释                                                         |
| ---- | --------------------- | ------------------------------------------------------------ |
| 1    | find(key);            | 查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end(); |
| 2    | count(key);           | 查找键key的元素个数                                          |
| 3    | lower_bound(keyElem); | 返回第一个key>=keyElem元素的迭代器。                         |
| 4    | upper_bound(keyElem); | 返回第一个key>keyElem元素的迭代器。                          |
| 5    | equal_range(keyElem); | 返回容器中key与keyElem相等的上下限的两个迭代器。             |

```c++
void test(){
	set<int> s;
	for(int i=1;i<=5;i++){
		s.insert(i*10);
	} 
	Print(s);
	
	set<int>::iterator pos=s.find(30);   
                    //find(key);--查找键key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
	if(pos!=s.end())
		cout<<"找到元素30"<<endl;
	else
		cout<<"未找到元素30"<<endl;
	
	//对于set而言，count的结果  要么是0  要么是1
	cout<<"count-20="<<s.count(20)<<endl;        //count(key);---查找键key的元素个数
	
	pos=s.lower_bound(20);    //lower_bound(keyElem);--返回第一个key>=keyElem元素的迭代器。
	if(pos!=s.end())
		cout<<"第一个大于等于20的元素值："<<*pos<<endl;
	else
		cout<<"为找到"<<endl;
		
	pos=s.upper_bound(20);   //upper_bound(keyElem);---返回第一个key>keyElem元素的迭代器。
	if(pos!=s.end())
		cout<<"第一个大于20的元素值："<<*pos<<endl;
	else
		cout<<"为找到"<<endl;
		
	pair<set<int>::iterator,set<int>::iterator> ret=s.equal_range(20);   //pair< , >---对组
      //equal_range(keyElem);----返回容器中key与keyElem相等的上下限的两个迭代器。
    
	if (ret.first != s.end())
	{
		cout << "equal_range中的lower_bound的值为：" << *ret.first << endl;
	}
	else
	{
		cout << "未找到" << endl;
	}


	if (ret.second != s.end())
	{
		cout << "equal_range中的upper_bound的值为：" << *ret.second << endl;
	}
	else
	{
		cout << "未找到" << endl;
	}
}
```

## 6、set和multiset插入相同值元素

* set不允许插入健值相同的元素

  ```c++
  void test(){
  	set<int> s;
  	pair<set<int>::iterator,bool> ret=s.insert(5);
  	if(ret.second) 
  		cout<<"第一次插入成功"<<endl;
  	else
  		cout<<"第一次插入失败"<<endl;
  	ret=s.insert(5);
  	if(ret.second) 
  		cout<<"第二次插入成功"<<endl;
  	else
  		cout<<"第二次插入失败"<<endl;
  }
  ```

  结果：第一次插入成功，第二次插入失败

* multiset允许插入健值相同的元素

  ```c++
  void test(){
  	multiset<int> s;
  	s.insert(5);
  	s.insert(5);
  	for(multiset<int>::iterator it=s.begin();it!=s.end();it++){
  		cout<<*it<<" "; 
  	}
  	cout<<endl;
  }
  ```

## 7、set、multiset容器的排序规则要在插入之前指定

```c++
class MyCompare{
public:
	bool operator()(int v1,int v2){
		return v1>v2;                  //从大到小
	}
};
void Print(set<int,MyCompare> &s){
	for(set<int,MyCompare>::iterator it=s.begin();it!=s.end();it++){
		cout<<*it<<" ";
	}
	cout<<endl;
}

void test(){
	set<int,MyCompare> s;
	for(int i=0;i<10;i++){
		s.insert(rand()%50);
	}
	Print(s);
}
```

## 8、利用仿函数  指定出自定义数据类型的排序规则

```c++
class Person{
public:
	Person(string name,int age){
		this->m_Name=name;
		this->m_Age=age;
	}
	string m_Name;
	int m_Age;
};
class MyCompare{
public:
	bool operator()(const Person &p1,const Person &p2){
		return p1.m_Age>p2.m_Age;
	}
};

void Creat_Person(set<Person,MyCompare> &s){
	Person p1("妲己",20);
	Person p2("亚瑟",40);
	Person p3("赵云",20);
	Person p4("韩信",50);
	Person p5("关羽",10);
	
	s.insert(p1);
	s.insert(p2);
	s.insert(p3);
	s.insert(p4);
	s.insert(p5);
}

void Print(set<Person,MyCompare> &s){
	for(set<Person,MyCompare>::iterator it=s.begin();it!=s.end();it++){
		cout<<"姓名："<<(*it).m_Name<<" 年龄："<<(*it).m_Age<<endl;
	}
	cout<<endl;
}

void test(){
	set<Person,MyCompare> s;
	Creat_Person(s);
	Print(s);	
}
```



# 对组(pair)

对组(pair)将一对值组合成一个值，这一对值可以具有不同的数据类型，两个值可以分别用pair的两个公有属性first和second访问。

类模板：`template <class T1, class T2> struct pair.`

创建对组的三种方式：

```c++
//第一种方法创建一个对组
pair<string, int> pair1(string("name"), 20);
cout << pair1.first << endl; //访问pair第一个值
cout << pair1.second << endl;//访问pair第二个值
//第二种
pair<string, int> pair2 = make_pair("name", 30);
cout << pair2.first << endl;
cout << pair2.second << endl;
//pair=赋值
pair<string, int> pair3 = pair2;
cout << pair3.first << endl;
cout << pair3.second << endl;
```

