# 友元

类的主要特点之一是数据隐藏，即类的私有成员无法在类的外部(作用域之外)访问。但是，有时候需要在类的外部访问类的私有成员

解决方法：使用友元函数

友元函数：一种特权函数，c++允许这个特权函数访问私有成员

可以把一个全局函数、某个类中的成员函数、甚至整个类声明为友元

# 友元语法

* `friend`关键字只出现在声明处

* 其他类、类成员函数、全局函数都可声明为友元

* 友元函数不是类的成员，不带this指针

* 友元函数可访问对象任意成员属性，包括私有属性

  ```c++
  //房间类 
  class Home;
  
  //人类
  class Person{
  public:
  	Person(string name):m_Name(name){} 
  	void LookAtBedroom(Home &home);
  	void LookAtLivingroom(Home &home);
  public:
  	string m_Name;
  }; 
  
  class Home{
  #if 0
  //类成员函数 做友元 
  friend void Person::LookAtLivingroom(Home &home);
  friend void Person::LookAtBedroom(Home &home);
  #else
  //类 做友元 
  friend Person;
  #endif
  //全局函数 做友元 
  friend void CleanBedroom(string name); 
  
  public:
  	Home(){
  		m_Livingroom="客厅";
  		m_Bedroom="卧室";
  	}
  public:
  	string m_Livingroom;
  private:
  	string m_Bedroom;
  };
  
  
  void Person::LookAtBedroom(Home &home){
  	cout<<m_Name<<"正在我的"<<home.m_Bedroom<<endl;
  }
  void Person::LookAtLivingroom(Home &home){
  	cout<<m_Name<<"正在我家"<<home.m_Livingroom<<endl;
  }
  
  void CleanBedroom(string name)
  {
  	cout<<name<<"正在打扫我的卧室"<<endl;
  } 
  
  void test()
  {
  	Person myfriend("强子");
  	Home myhome;
  	myfriend.LookAtLivingroom(myhome);
  	myfriend.LookAtBedroom(myhome);
  	CleanBedroom("李阿姨");
  }
  ```

# 友元注意事项

* 友元关系不能被继承。
* 友元关系是单向的，类A是类B的朋友，但类B不一定是类A的朋友。
* 友元关系不具有传递性。类B是类A的朋友，类C是类B的朋友，但类C不一定是类A的朋友

# 电视机和遥控器的案例

电视机类，电视机有开机和关机状态，有音量，有频道，提供音量操作的方法，频道操作的方法。由于电视机只能逐一调整频道，不能指定频道，增加遥控类，遥控类除了拥有电视机已有的功能，再增加根据输入调台功能。

```c++
class Remote;

class Television{
friend Remote;
public:
	enum{ off,on }; //电视状态
	enum{ minVol,maxVol = 100 }; //音量从0到100
	enum{ minChannel = 1,maxChannel = 255 }; //频道从1到255
	
	Television(){  //电视机初始状态 
		m_State=off;
		m_Volume=minVol;
		m_Channel=minChannel;
	}
	
	//电视机开机-关机 
	void OnOrOff()
	{
		this->m_State=m_State? off:on;
	} 
	
	//调节音量 
	void VolumeUp()
	{
		if(!this->m_State||this->m_Volume>maxVol)
			return;
		this->m_Volume+=5;
	}
	void VolumeDown()
	{
		if(!this->m_State||this->m_Volume<minVol)
			return;
		this->m_Volume-=5; 
	}
	
	//更换电视频道
	void ChannelUp()
	{
		if(!this->m_State||this->m_Channel>maxChannel)
			return;
		this->m_Channel++;
	}
	void ChannelDown()
	{
		if(!this->m_State||this->m_Channel<minChannel)
			return;
		this->m_Channel--;
	}
	
	//展示当前电视状态信息
	void ShowTeleState()
	{
		cout<<"电视机状态："<<this->m_State<<endl;
		if(this->m_State)
		{
			cout<<"电视机音量："<<this->m_Volume<<endl;
			cout<<"电视机频道："<<this->m_Channel<<endl;
		} 
	}
private:
	int m_State;     //电视状态，开机，还是关机
	int m_Volume;    //电视机音量
	int m_Channel;   //电视频道
	
};

class Remote{
public:
	Remote(Television* television){
		this->pTelevision=television;
	}
	//开、关机 
	void OnOrOff()
	{
		this->pTelevision->OnOrOff();
	}
	//调节音量 
	void VolumeUp(){
		this->pTelevision->VolumeUp();
	}
	void VolumDown(){
		this->pTelevision->VolumeDown();
	} 
	//调节频道
	void ChannelUp(){
		this->pTelevision->ChannelUp();
	} 
	void ChannelDown(){
		this->pTelevision->ChannelDown();
	}
	void ShowTeleState(){
		this->pTelevision->ShowTeleState();
	} 
	//按输入调节频道
	void SetChannel(int channel){
		if(channel<this->pTelevision->minChannel||channel>this->pTelevision->maxChannel)
			return;
		this->pTelevision->m_Channel=channel; 
	}
	 	
private:
	Television* pTelevision;
	
};


void test01()
{
	Television television;
	television.ShowTeleState();
	television.OnOrOff(); //开机
	television.VolumeUp(); //增加音量+1
	television.VolumeUp(); //增加音量+1
	television.VolumeUp(); //增加音量+1
	television.VolumeUp(); //增加音量+1
	television.ChannelUp(); //频道+1
	television.ChannelUp(); //频道+1
	television.ShowTeleState();
}

void test02()
{
	//创建电视
	Television television;
	//创建遥控
	Remote remote(&television);
	remote.OnOrOff();      //开机 
	remote.ChannelUp();   //频道+1
	remote.ChannelUp();   //频道+1
	remote.VolumeUp();    //音量+1
	remote.VolumeUp();    //音量+1
	remote.VolumeUp();    //音量+1
	remote.ShowTeleState();
	//remote.OnOrOff();     //关机 
	remote.SetChannel(50);
	remote.ShowTeleState();
}
```

# 数组类封装

**MyArray.h**

```c++
#ifndef MYARRAY_H
#define MYARRAY_H

class MyArray{
public:
	//无参构造函数，用户没有指定容量，则初始化为100
	MyArray();
	//有参构造函数，用户指定容量初始化
	MyArray(int capacity); 
	//用户操作接口
	//根据位置添加元素
	void SetData(int pos, int val);
	//获得指定位置数据
	int GetData(int pos);
	//尾插法
	void PushBack(int val);
	//获得长度
	int GetLength();
	//打印数组
	void Print(); 
	//析构函数，释放数组空间
	~MyArray();
private:
	int m_Capacity; //数组一共可容纳多少个元素
	int m_Size; //当前有多少个元素
	int* pAdress; //指向存储数据的空间
};

#endif
```

**MyArray.cpp**

```c++
MyArray::MyArray(){
	this->m_Capacity=100;
	this->pAdress=new int[this->m_Capacity];	
	this->m_Size=0;
}

MyArray::MyArray(int capacity){
	this->m_Capacity=capacity;
	this->pAdress=new int[this->m_Capacity];
	this->m_Size=0;
}

void MyArray::SetData(int pos,int val){
	if(pos<0||pos>this->m_Capacity-1||this->m_Size>this->m_Capacity)
		return;
	for(int i=this->m_Size-1;i>=pos;i--)
	{
		pAdress[i+1]=pAdress[i];
	}
	pAdress[pos]=val;
	this->m_Size++;
}

int MyArray::GetData(int pos){
	if(pos<0||pos>this->m_Capacity-1)
		return 0;
	return pAdress[pos];
}


void MyArray::PushBack(int val){
	if(this->m_Size>this->m_Capacity)
		return;
	pAdress[this->m_Size]=val;
	this->m_Size++;
}

int MyArray::GetLength(){
	return this->m_Size;
} 

void MyArray::Print(){
	for(int i=0;i<this->m_Size;i++)
	{
		cout<<pAdress[i]<<" ";
	}
	cout<<endl;
}

MyArray::~MyArray(){
	if(this->pAdress)
	{
		delete[] pAdress;
		pAdress=NULL;
	}	
}
```

**TestMyArray.cpp**

```c++
void test()
{
	MyArray myarray(50);
	//数组中插入元素
	for (int i = 0; i < 10; i++)
    {
		//尾插法
		myarray.PushBack(i);
		//myarray.SetData(i, i);
	}
	//打印数组中元素
	myarray.Print();
	
	//按位置插入 
	myarray.SetData(5,40);
	myarray.Print();
	
	//按位置查找
	cout<<"第8个元素为："<<myarray.GetData(8)<<endl; 
	
	//获取长度
	cout<<"数组长度为："<<myarray.GetLength()<<endl; 
}
```



