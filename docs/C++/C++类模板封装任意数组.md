# 类模板封装任意类型数组

# 1、任意类型数组类模板

```c++
template <class T>
class MyArray{
public:
    MyArray();                        //默认构造
    MyArray(int capaticy);            //有参构造
    MyArray(const MyArray &arr);      //拷贝构造
    MyArray& operator=(const MyArray &arr);    //赋值运算符重载
    T& operator[](int index);                  //下标运算符重载
    void PushBack(T val);                //尾插法
    void pushIndex(T val,int index);     //按位置插入 
	int GetSize();            //获取数组大小
	int GetCapaticy();        //获取数组容量
	void Print();  //打印数组  
    ~MyArray();    //析构函数
private:
    T *pAdress;       //数组地址
    int m_Capaticy;   //数组容量
    int m_Size;       //数组大小
};
```

# 2、有参构造函数实现

```c++
template <typename T>
MyArray<T>::MyArray(int capaticy){
	this->m_Capaticy=capaticy;
	this->m_Size=0;
	this->pAdress=new T[this->m_Capaticy];
}
```

# 3、拷贝构造函数实现

```c++
template <class T>
MyArray<T>::MyArray(const MyArray &arr){
	this->m_Capaticy=arr.m_Capaticy;
	this->m_Size=arr.m_Size;
	this->pAdress=new T[this->m_Capaticy];
	for(int i=0;i<this->m_Size;++i){
		this->pAdress[i]=arr[i];
	}
}
```

# 4、赋值运算符重载

```c++
template <class T>
MyArray<T>& MyArray<T>::operator=(const MyArray &arr){
	if(this->pAdress){
		delete[] this->pAdress;
		this->pAdress=NULL;
	}
	this->m_Capaticy=arr.m_Capaticy;
	this->m_Size=arr.m_Size;
	this->pAdress=new T[this->m_Capaticy];
	for(int i=0;i<this->m_Size;++i){
		this->pAdress[i]=arr[i];
	}
	return *this;
}
```

# 5、下标运算符重载

```cassandra
template <class T>
T& MyArray<T>::operator[](int index)          //下标运算符重载
{
	return this->pAdress[index];

```

# 6、数组尾插法

```c++
template <class T>
void MyArray<T>::PushBack(T val){
	if(this->m_Capaticy==this->m_Size)
		return;
	this->pAdress[this->m_Size]=val;
	this->m_Size++;
} 
```

# 7、数组按位置插入

```c++
template <class T>
void MyArray<T>::pushIndex(T val,int index){
	if(this->m_Capaticy==this->m_Size)
		return;
	for(int i=this->m_Size-1;i>=index-1;--i){
		this->pAdress[i+1]=this->pAdress[i];
	}
	this->pAdress[index-1]=val;
	this->m_Size++;
} 
```

# 8、获取数组大小

```c++
template <class T>
int MyArray<T>::GetSize(){
	return this->m_Size;
}
```

# 9、获取数组容量

```c++
template <class T>
int MyArray<T>::GetCapaticy(){
	return this->m_Capaticy;
}
```

# 10、打印数组（整型和字符型）

```c++
template <class T>
void MyArray<T>::Print(){
	for(int i=0;i<this->GetSize();++i){
		cout<<" "<<this->pAdress[i];
	}
	cout<<endl;
}
 
```

# 11、析构函数实现

```c++
template <class T>
MyArray<T>::~MyArray(){
	if(this->pAdress){
		delete[] this->pAdress;
		this->pAdress=NULL;
	}
}
```

# 12、整型测试

```c++
void test()
{
	MyArray<int> myarray(50);
	for(int i=0;i<10;++i){
		myarray.PushBack(i+100);
	}
	myarray.Print();
	
	myarray.pushIndex(50,7);
	myarray.Print();
	myarray.GetSize();
	myarray.GetCapaticy();	
	myarray.Print(); 
}
```

# 13、字符型测试

```c++
void test01()
{
	MyArray<char> myarray(50);
	for(int i=0;i<10;++i){
		myarray.PushBack(i+65);
	}
	myarray.Print();
	
	myarray.pushIndex(97,7);
	myarray.Print();
	myarray.GetSize();
	myarray.GetCapaticy();
}
```

# 14、自定义类型测试

```c++
class Person{
public:
	friend void myPrintPerson(MyArray<Person> &myPersonArr);
	Person(){}
	Person(string name,int age){
		this->m_Name=name;
		this->m_Age=age;
	}	
private:
	string m_Name;
	int m_Age;
};

void myPrintPerson(MyArray<Person> &myPersonArr)
{
	for (int i = 0; i < myPersonArr.GetSize();i++)
	{
		cout << "姓名： " << myPersonArr[i].m_Name << " 年龄：" << myPersonArr[i].m_Age << endl;
	}
}

void test03()
{
	MyArray<Person> myarray(50);
	
	Person p1("孙悟空", 1000);
	Person p2("猪八戒", 800);
	Person p3("唐僧", 500);
	Person p4("沙悟净", 300);
	Person p5("白龙马", 10000);
	
	myarray.PushBack(p1);
	myarray.PushBack(p2);
	myarray.PushBack(p3);
	myarray.PushBack(p4);
	myarray.PushBack(p5);
	
	myPrintPerson(myarray);
}
```

