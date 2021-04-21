# map/multimap基本概念

Map的特性是，所有元素都会根据元素的键值自动排序。Map所有的元素都是pair,同时拥有实值和键值，pair的第一元素被视为键值，第二元素被视为实值，map不允许两个元素有相同的键值。

我们可以通过map的迭代器改变map的键值吗？答案是不行，因为map的键值关系到map元素的排列规则，任意改变map键值将会严重破坏map组织。如果想要修改元素的实值，那么是可以的。

Map和list拥有相同的某些性质，当对它的容器元素进行新增操作或者删除操作时，操作之前的所有迭代器，在操作完成之后依然有效，当然被删除的那个元素的迭代器必然是个例外。

Multimap和map的操作类似，唯一区别multimap键值可重复。

Map和multimap都是以红黑树为底层实现机制。

# map/multimap常用API

## 1、map遍历

```c++
void Print(map<int,int> &m){
	for(map<int,int>::iterator it=m.begin();it!=m.end();it++){
		cout<<"key:"<<(*it).first<<" val:"<<(*it).second<<endl;
	}
	cout<<endl;
}
```

## 2、map构造函数

| 序号 | 原型                | 注释             |
| ---- | ------------------- | ---------------- |
| 1    | map<T1, T2> mapTT   | map默认构造函数: |
| 2    | map(const map &mp); | 拷贝构造函数     |

```c++
void test(){
	map<int,int> m;                  //map<T1, T2> mapTT----map默认构造函数:
	m.insert(pair<int,int>(1,5));
	m.insert(make_pair(3,8));
    m.insert(map<int,int>::value_type(2,9));
    m[4]=7;
    m[6]=4;
	Print(m);
	
	map<int,int> m1(m);            //map(const map &mp);----拷贝构造函数
	Print(m);
}
```

## 3、map赋值操作

| 序号 | 原型                           | 注释             |
| ---- | ------------------------------ | ---------------- |
| 1    | map& operator=(const map &mp); | 重载等号操作符   |
| 2    | swap(mp);                      | 交换两个集合容器 |

```c++
void test(){
	map<int,int> m;                  //map<T1, T2> mapTT----map默认构造函数:
	m.insert(pair<int,int>(1,5));
	m.insert(make_pair(3,8));
    m.insert(map<int,int>::value_type(2,9));
    m[4]=7;
    m[6]=4;
	Print(m);
	
	map<int,int> m1=m;            //map& operator=(const map &mp);----重载等号操作符
	Print(m);
	
	map<int,int> m2;
	for(int i=0;i<5;i++){
		m2[rand()%10]=rand()%50;
	}
	Print(m2);
	
	m1.swap(m2);         //swap(mp);-----交换两个集合容器
	Print(m1);
	Print(m2);
}
```

## 4、map大小操作

| 序号 | 原型     | 注释                 |
| ---- | -------- | -------------------- |
| 1    | size();  | 返回容器中元素的数目 |
| 2    | empty(); | 判断容器是否为空     |

```c++
void test(){
	map<int,int> m;                  //map<T1, T2> mapTT----map默认构造函数:
	m.insert(pair<int,int>(1,5));
	m.insert(make_pair(3,8));
    m.insert(map<int,int>::value_type(2,9));
    m[4]=7;
    m[6]=4;
    m[6]=5;   //不允许相同健值元素插入  --替换 
	Print(m);
	
	if(!m.empty()){                      //empty();----判断容器是否为空
		cout<<"map非空"<<endl;
		cout<<"size="<<m.size()<<endl;     //size();----返回容器中元素的数目
	}
	else
		cout<<"map为空"<<endl;
}
```

## 5、map插入数据元素操作

| 序号 | 原型                                                    | 注释                                    |
| ---- | ------------------------------------------------------- | --------------------------------------- |
| 1    | map.insert(...);     map<int, string> mapStu;           | 往容器插入元素，返回pair<iterator,bool> |
| 2    | mapStu.insert(pair<int, string>(3, "小张"));            | 通过pair的方式插入对象                  |
| 3    | mapStu.inset(make_pair(-1, "校长"));                    | 通过pair的方式插入对象                  |
| 4    | mapStu.insert(map<int, string>::value_type(1, "小李")); | 通过value_type的方式插入对象            |
| 5    | mapStu[3] = "小刘";   mapStu[5] = "小王";               | 通过数组的方式插入值                    |

```c++
void test(){
	map<int,int> m;                  //map<T1, T2> mapTT----map默认构造函数:
	m.insert(pair<int,int>(1,5));    //mapStu.insert(pair<int, string>(3, "小张"));----通过pair的方式插入对象
	m.insert(make_pair(3,8));        //mapStu.inset(make_pair(-1, "校长"));------通过pair的方式插入对象
    m.insert(map<int,int>::value_type(2,9)); //mapStu.insert(map<int, string>::value_type(1, "小李"));---通过value_type的方式插入对象
    m[4]=7;       //mapStu[3] = "小刘";--------通过数组的方式插入值
    m[6]=4;      
    m[6]=5;   //不允许相同健值元素插入  --替换 
	Print(m);
}
```

## 6、map删除操作

| 序号 | 原型            | 注释                                                   |
| ---- | --------------- | ------------------------------------------------------ |
| 1    | clear();        | 删除所有元素                                           |
| 2    | erase(pos);     | 删除pos迭代器所指的元素，返回下一个元素的迭代器。      |
| 3    | erase(beg,end); | 删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。 |
| 4    | erase(keyElem); | 删除容器中key为keyElem的对组                           |

```c++
void test(){
	map<int,int> m;                  //map<T1, T2> mapTT----map默认构造函数:
	m.insert(pair<int,int>(1,5));
	m.insert(make_pair(3,8));
    m.insert(map<int,int>::value_type(2,9));
    m[4]=7;
    m[6]=4;
    m[6]=5;   //不允许相同健值元素插入  --替换 
	Print(m);
	
	map<int,int> m1=m;
	Print(m1);
	m1.clear();       //clear();---------删除所有元素
	Print(m1);
	
	//m.erase(m.begin()+3);  //不能随机访问
	m.erase(++m.begin()); 
	Print(m);
	m.erase(--m.end());         //erase(pos);----删除pos迭代器所指的元素，返回下一个元素的迭代器。
	Print(m);
	
	m.erase(3);           //erase(keyElem);-----删除容器中key为keyElem的对组
	Print(m);
	
	m.erase(m.begin(),m.end());   //erase(beg,end);---删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
	Print(m);
}
```

## 7、map查找操作

| 序号 | 原型                  | 注释                                                         |
| ---- | --------------------- | ------------------------------------------------------------ |
| 1    | find(key);            | 查找键key是否存在,若存在，返回该键的元素的迭代器；/若不存在，返回map.end(); |
| 2    | count(keyElem);       | 返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。 |
| 3    | lower_bound(keyElem); | 返回第一个key>=keyElem元素的迭代器。                         |
| 4    | upper_bound(keyElem); | 返回第一个key>keyElem元素的迭代器。                          |
| 5    | equal_range(keyElem); | 返回容器中key与keyElem相等的上下限的两个迭代器。             |

```c++
void test(){
	map<int, int> m;
	m.insert(pair<int, int>(1, 10));
	m.insert(make_pair(2, 20));
	m.insert(map<int, int>::value_type(3, 30));
	m[4] = 40;
	m[5];       //val=默认0 
	Print(m);

	map<int,int>::iterator ret = m.find(3);
	if (ret != m.end())
		cout << "找到了 key为 " << ret->first << " value = " << ret->second << endl;
	else
		cout << "未找到" << endl;

	//统计  count 
	int num =  m.count(4);
	cout << "key为4的对组个数为： " << num << endl;

	//lower_bound(keyElem);//返回第一个key>=keyElem元素的迭代器。
	map<int,int>::iterator pos = m.lower_bound(3);

	if (pos != m.end())
		cout << "找到了lower_bound key: " << pos->first << " value: " << pos->second << endl;
	else
		cout << "未找到" << endl;

	//upper_bound(keyElem);//返回第一个key>keyElem元素的迭代器。
	pos = m.upper_bound(3);
	if (pos != m.end())
		cout << "找到了upper_bound key: " << pos->first << " value: " << pos->second << endl;
	else
		cout << "未找到" << endl;


	//equal_range(keyElem);//返回容器中key与keyElem相等的上下限的两个迭代器。
	pair<map<int, int>::iterator, map<int, int>::iterator>  ret2 = m.equal_range(3);

	if ( ret2.first != m.end() )
		cout << "找到了equal_range中的 lower_bound的 key =  " << ret2.first->first << " value = " << ret2.first->second << endl;
	else
		cout << "未找到" << endl;

	if (ret2.second != m.end())
		cout << "找到了equal_range中的 upper_bound的 key =  " << ret2.second->first << " value = " << ret2.second->second << endl;
	else
		cout << "未找到" << endl;
}
```

## 8、仿函数自定义健值排序规则

```c++
class MyCompare{
public:
	bool operator()(int v1,int v2){
		return v1>v2;                   //从大到小
	}
}; 

void Print(map<int,int,MyCompare> &m){
	for(map<int,int,MyCompare>::iterator it=m.begin();it!=m.end();it++){
		cout<<"key:"<<(*it).first<<" val:"<<(*it).second<<endl;
	}
	cout<<endl;
}

void test(){
	map<int,int,MyCompare> m;
	for(int i=0;i<5;i++){
		m[rand()%10]=rand()%50;
	}
	Print(m);
}
```

# multimap 案例

* 公司今天招聘了 5 个员工，5 名员工进入公司之后，需要指派员工在那个部门工作
* 人员信息有: 姓名 年龄 电话 工资等组成
* 通过 Multimap 进行信息的插入 保存 显示
* 分部门显示员工信息 显示全部员工信息

```c++
enum{CAIWU,RENLI,YANFA};
class Worker{
public:
	string m_Name;
	int m_Money;
};

void Creat_Worker(vector<Worker> &v){
	string nameSeed="ABCDE";
	for(int i=0;i<5;i++){
		Worker worker;
		worker.m_Name="员工";
		worker.m_Name+=nameSeed[i];
		worker.m_Money=rand()%10000+20000;
		v.push_back(worker); 
	} 
}

void SetGroup(vector<Worker> &v,multimap<int,Worker> &m){
	for(vector<Worker>::iterator it=v.begin();it!=v.end();it++){
		int ID=rand()%3;
		m.insert(make_pair(ID,*it));
	}
} 

void Print(multimap<int,Worker>::iterator pos,int count,multimap<int,Worker> &m){
	int num=0;
	for(;pos!=m.end(),num<count;pos++,num++){
		cout << "姓名： " << pos->second.m_Name << " 工资： " << pos->second.m_Money << endl;
	}
}

void ShowWorker(multimap<int,Worker> &m){
	cout<<"各部门员工信息如下："<<endl;
	
	cout << "财务部门员工信息如下：" << endl;
	Print(m.find(CAIWU),m.count(CAIWU),m);
	
	cout << "人事部门员工信息如下：" << endl;
	Print(m.find(RENLI),m.count(RENLI),m);
	
	cout << "研发部门员工信息如下：" << endl;
	Print(m.find(YANFA),m.count(YANFA),m);
}

void test(){
	vector<Worker> v;
	multimap<int,Worker> m;
	Creat_Worker(v);
	SetGroup(v,m);
	ShowWorker(m);
}
```

