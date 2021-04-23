# vector容器基本概念

vector的数据安排以及操作方式，与array非常相似，两者的唯一差别在于空间的运用的灵活性。Array是静态空间，一旦配置了就不能改变，要换大一点或者小一点的空间，可以，一切琐碎得由自己来，首先配置一块新的空间，然后将旧空间的数据搬往新空间，再释放原来的空间。**Vector是动态空间**，**随着元素的加入，它的内部机制会自动扩充空间以容纳新元素。**因此vector的运用对于内存的合理利用与运用的灵活性有很大的帮助，我们再也不必害怕空间不足而一开始就要求一个大块头的array了。

Vector的实现技术，关键在于其对大小的控制以及重新配置时的数据移动效率，一旦vector旧空间满了，如果客户每新增一个元素，vector内部只是扩充一个元素的空间，实为不智，因为所谓的扩充空间(不论多大)，一如刚所说，是”配置新空间-数据移动-释放旧空间”的大工程,时间成本很高，应该加入某种未雨绸缪的考虑

# vector迭代器

Vector维护一个线性空间，所以不论元素的型别如何，普通指针都可以作为vector的迭代器，因为vector迭代器所需要的操作行为，如`operaroe*`, `operator->`, `operator++`, `operator--`, `operator+`, `operator-`, `operator+=`, `operator-=`, 普通指针天生具备。**Vector支持随机存取**，而普通指针正有着这样的能力。所以vector提供的是随机访问迭代器(Random Access Iterators).

`Vector<int>::iterator it1;`

`Vector<Teacher>::iterator it2;`

`it1的型别其实就是Int*,it2的型别其实就是Teacher*.`	

# vector的数据结构

Vector所采用的数据结构非常简单，线性连续空间，它以两个迭代器`_Myfirst`和`_Mylast`分别指向配置得来的连续空间中目前已被使用的范围，并以迭代器`_Myend`指向整块连续内存空间的尾端。

为了降低空间配置时的速度成本，vector实际配置的大小可能比客户端需求大一些，以备将来可能的扩充，这边是***\*容量\****的概念。换句话说，**一个vector的容量永远大于或等于其大小，一旦容量等于大小，便是满载，下次再有新增元素，整个vector容器就得另觅居所。**

```c++
void test(){
	vector<int>v;
	for(int i=0;i<5;i++){
		v.push_back(i);
	}
	cout<<"大小："<<v.size()<<endl;            //大小：5
	cout<<"容量："<<v.capacity()<<endl;        //容量：8
}
```

注意：

 所谓动态增加大小，并不是在原空间之后续接新空间(因为无法保证原空间之后尚有可配置的空间)，而是一块更大的内存空间，然后将原数据拷贝新空间，并释放原空间。因此，对vector的任何操作，一旦引起空间的重新配置，指向原vector的所有迭代器就都失效了。这是程序员容易犯的一个错误，务必小心。

# vector常用 API 操作

## 1、vector构造函数

| 序号 | 原型                        | 注释                                        |
| ---- | --------------------------- | ------------------------------------------- |
| 1    | vector<T> v;                | 采用模板实现类实现，默认构造函数            |
| 2    | vector(v.begin(), v.end()); | 将v[begin(), end())区间中的元素拷贝给本身。 |
| 3    | vector(n, elem);            | 构造函数将n个elem拷贝给本身。               |
| 4    | vector(const vector &vec);  | 拷贝构造函数。                              |

```c++
void test(){
	vector<int>v;                 //vector<T> v;
	for(int i=0;i<5;i++){
		v.push_back(i);
	}
	Print(v);
	
	vector<int>v2(v.begin(),v.end());     //vector(v.begin(), v.end());
	Print(v2);
	
	vector<int>v3(5,1);            //vector(n, elem);
	Print(v3);
	
	vector<int>v4(v3);            //vector(const vector &vec);
	Print(v4);
}
```

## 2、vector常用赋值操作

| 序号 | 原型                                   | 注释                                     |
| ---- | -------------------------------------- | ---------------------------------------- |
| 1    | assign(beg, end);                      | 将[beg, end)区间中的数据拷贝赋值给本身。 |
| 2    | assign(n, elem);                       | 将n个elem拷贝赋值给本身。                |
| 3    | vector& operator=(const vector  &vec); | 重载等号操作符                           |
| 4    | swap(vec);                             | 将vec与本身的元素互换。                  |

```c++
void test(){
	vector<int> v(5,2);
	Print(v);
	
	vector<int> v1;
	v1.assign(v.begin(),v.end());    //assign(beg, end);
	Print(v);
	
	vector<int> v2;
	v2.assign(5,1);            //assign(n, elem);
	Print(v2);
	
	vector<int> v3=v2;        //vector& operator=(const vector  &vec);
	Print(v3);
	
	v.swap(v2);               //swap(vec);
	Print(v);
	Print(v2);	
}
```

## 3、vector大小操作



| 序号 | 原型                   | 注释                                                         |
| ---- | ---------------------- | ------------------------------------------------------------ |
| 1    | size();                | 返回容器中元素的个数                                         |
| 2    | empty();               | 判断容器是否为空                                             |
| 3    | resize(int num);       | 重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。 |
| 4    | resize(int num, elem); | 重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长>度的元素被删除。 |
| 5    | capacity();            | 容器的容量                                                   |
| 6    | reserve(int len);      | 容器预留len个元素长度，预留位置不初始化，元素不可访问。      |

```c++
void test(){
	vector<int> v(5,2);
	Print(v);
	
	cout<<"v.size="<<v.size()<<endl;   //size();
	
	if(v.empty())                 //empty();
		cout<<"容器v空"<<endl;
	cout<<"容器v不空"<<endl;
	
	v.resize(10);              //resize(int num);
	Print(v);
	cout<<"v.size="<<v.size()<<endl;
	
	v.resize(15,1);          //resize(int num, elem);   
	Print(v);
	cout<<"v.size="<<v.size()<<endl;
	
	cout<<"v.capacity"<<v.capacity();     //capacity();
	
	vector<int> v2;
	v2.reserve(10);              //reserve(int len);
	//Print(v2);  //不可访问 
}
```

## 4、vector数据存取操作

| 序号 | 原型         | 注释                                                       |
| ---- | ------------ | ---------------------------------------------------------- |
| 1    | at(int idx); | 返回索引idx所指的数据，如果idx越界，抛出out_of_range异常。 |
| 2    | operator[ ]; | 返回索引idx所指的数据，越界时，运行直接报错                |
| 3    | front();     | 返回容器中第一个数据元素                                   |
| 4    | back();      | 返回容器中最后一个数据元素                                 |

```c++
void test(){
	vector<int> v;
	for(int i=0;i<5;i++){
		v.push_back(i);
	} 
	Print(v);
	
	cout<<"v.at(3)="<<v.at(3)<<endl;    //at(int idx);
	
	cout<<"v[2]="<<v[2]<<endl;         //operator[ ];
	
	cout<<"v.front="<<v.front()<<endl;   //front();
	
	cout<<"v.back="<<v.back()<<endl;    //back();
}
```

## 5、vector插入和删除操作

| 序号 | 原型                                             | 注释                                 |
| ---- | ------------------------------------------------ | ------------------------------------ |
| 1    | insert(const_iterator pos, int count,ele);       | 迭代器指向位置pos插入count个元素ele. |
| 2    | push_back(ele);                                  | 尾部插入元素ele                      |
| 3    | pop_back();                                      | 删除最后一个元素                     |
| 4    | erase(const_iterator start, const_iterator end); | 删除迭代器从start到end之间的元素     |
| 5    | erase(const_iterator pos);                       | 删除迭代器指向的元素                 |
| 6    | clear();                                         | 删除容器中所有元素                   |

```c++
void test(){
	vector<int> v;
	for(int i=0;i<5;i++){
		v.push_back(i);         //push_back(ele);
	} 
	Print(v);
	
	v.insert(v.begin()+2,8);   //insert(const_iterator pos, int count,ele);
	Print(v);
	
	v.pop_back();      //pop_back();
	Print(v);
	
	v.erase(v.begin()+3,v.end());       //erase(const_iterator start, const_iterator end);
	Print(v);
	
	v.erase(v.begin()+1);         //erase(const_iterator pos);
	Print(v);
	
	v.clear();         //clear();
	Print(v);
}
```

## 6、巧用swap，收缩内存空间

```c++
void test(){
	vector<int> v;
	for (int i = 0; i < 100000;i ++){
		v.push_back(i);
	}

	cout << "capacity:" << v.capacity() << endl;      //capacity:131072  
	cout << "size:" << v.size() << endl;              //size:100000 

	//此时 通过resize改变容器大小
	v.resize(10);

	cout << "capacity:" << v.capacity() << endl;     //capacity:131072  
	cout << "size:" << v.size() << endl;             //size:10 

	//容量没有改变
	vector<int>(v).swap(v);
	//vector<int>(v)---匿名对象调用拷贝构造（v）初始化 
	//交换了值和size，但内存（capacity）被匿名对象释放 

	cout << "capacity:" << v.capacity() << endl;      //capacity:10 
	cout << "size:" << v.size() << endl;              //size:10 
}
```

## 7、reserve预留空间

```c++
void test(){
	vector<int> v;
	int* pStart = NULL;
	int count = 0;
	for (int i = 0; i < 100000;i ++){
		v.push_back(i);
		if (pStart != &v[0]){
			pStart = &v[0];
			count++;
		}
	}
	cout << "count:" << count << endl;          //count:18   //进行18次的内存重写分配
}
```

```c++
void test(){
	vector<int> v;
	//预先开辟空间
	v.reserve(100000);
	int* pStart = NULL;
	int count = 0;
	for (int i = 0; i < 100000;i ++){
		v.push_back(i);
		if (pStart != &v[0]){
			pStart = &v[0];
			count++;
		}
	}
	cout << "count:" << count << endl;    //count：1  与开辟内存后只分配一次内存
}
```

## 8、逆序遍历

`vector<int>::reverse_iterator it=v.rbegin();it!=v.rend();it++`

```c++
void test(){
	vector<int> v;
	for(int i=0;i<5;i++){
		v.push_back(i+1);
	} 
	
	for(vector<int>::reverse_iterator it=v.rbegin();it!=v.rend();it++){
		cout<<*it<<" ";
	}
}
```

