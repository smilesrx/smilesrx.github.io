# 类的封装

如何用程序语言来表示现实事物？现实世界的事物所具有的共性就是每个事物都具有自身的属性，一些自身具有的行为，所以如果我们能把事物的属性和行为表示出来，那么就可以抽象出来这个事物。

## 1、C语言封装的缺陷

将属性和行为分离（类型检测不强）

经典例子：人吃狗粮

```c
//人类--结构体
typedef struct Person
{
    //人的属性
    char name[60];   //姓名
    int age;         //年龄
}Person;
//人类--行为
void Person_Eat(Person *person)
{
    printf("%d的%s在吃人饭\n",person->age,person->name);
}

//狗类--结构体
typedef struct Dog
{
    char name[60];   //姓名
    int age;         //年龄
}Dog;
//狗类--行为
void Dog_Eat(Dog *dog)
{
    printf("%d的%s在吃狗粮\n",dog->age,dog->name);
}

void test()
{
    Person person;
    strcpy(person.name,"老王");
    person.age=18;
    
    Dog dog;
    strcpy(dog.name,"旺财");
    dog.age=5;
    
    Person_Eat(&dog);  //结果：5岁的旺财在吃人饭
    Dog_Eat(&person);  //结果：18岁的老王在吃狗粮
}
```

封装特性包含两个方面，一个是属性和变量合成一个整体，一个是给属性和函数增加访问权限。

## 2、C++封装

1. 封装

      * 把属性（变量）和行为（函数）合成一个整体，封装在一个类中

      * 对变量和函数进行访问权限控制

        ```c++
        //人类
        class Person{
        public:
            //属性
            string m_Name;
            int m_Age;
            //行为
            void Person_Eat(Person *person)
            {
                cout<<person->m_Age<<"岁的"<<person->m_Name<<"在吃人饭"<<endl;
            }
        };
        
        //狗类
        class Dog{
        public:
            //属性
            string m_Name;
            int m_Age;
            //行为
            void Dog_Eat(Dog *dog)
            {
                cout<<dog->m_Age<<"岁的"<<dog->m_Name<<"在吃狗粮"<<endl;
            }
        };
        
        void test()
        {
            Person person;
            person.m_Name="老王";
            person.m_Age=20;
            person.Person_Eat(&person);
            
            Dog dog;
            dog.m_Age=8;
            dog.m_Name="旺财"; 
        	dog.Dog_Eat(&dog); 
        }
        ```

2. 访问权限

      * 在类的内部(作用域范围内)，没有访问权限之分，所有成员可以相互访问

      * 在类的外部(作用域范围外)，访问权限才有意义：public，private，protected

      * 在类的外部，只有public修饰的成员才能被访问，在没有涉及继承与派生时，private和protected是同等级的，外部不允许访问

        | 访问属性  | 属性 | 对象内部 | 对象外部 |
        | :-------: | :--: | :------: | :------: |
        |  public   | 共有 |  可访问  |  可访问  |
        | protected | 保护 |  可访问  | 不可访问 |
        |  private  | 私有 |  可访问  | 不可访问 |

        ```c++
        class Person{
        //人具有的行为(函数)
        public:
        	void Dese(){ cout << "我有钱，年轻，个子又高，就爱嘚瑟!" << endl;}
        //人的属性(变量)
        public:
        	int m_Tall; //多高，可以让外人知道
        protected:
        	int m_Money; // 有多少钱,只能儿子孙子知道
        private:
        	int m_Age; //年龄，不想让外人知道
        };
        
        void test()
        {
            Person person;
            person.Dese();    //可访问
            person.m_Tall;    //可访问
            //person.m_money;   //不可访问
            //person.m_Age;     //不可访问
        }
        ```

        ## 3、struct 和class的区别

        class默认访问权限为private,struct默认访问权限为public。

        ```c++
        class A{
            int m_A;   //默认private权限
        };
        
        struct B{
            int m_B;   //默认public权限
        };
        
        void test()
        {
            A a;
            a.m_A=10;   //不可访问
            
            B b;
            b.m_B=20;   //可访问
        }
        ```

# 将成员变量设置为私有属性--提高安全性

## 1、可赋予客户端访问数据的一致性。

如果成员变量不是public，客户端唯一能够访问对象的方法就是通过成员函数。如果类中所有public权限的成员都是函数，客户在访问类成员时只会默认访问函数，不需要考虑访问的成员需不需要添加(),这就省下了许多搔首弄耳的时间。

## 2、自己可以控制读写权限

使用成员函数可使得我们对变量的控制处理更加精细。如果我们让所有的成员变量为public，每个人都可以读写它。如果我们设置为private，我们可以实现“不准访问”、“只读访问”、“读写访问”，甚至你可以写出“只写访问”。

```c++
class AccessLevels{
public:
	//对只读属性进行只读访问
	int getReadOnly(){ return readOnly; }
	//对读写属性进行读写访问
	void setReadWrite(int val){ readWrite = val; }
	int getReadWrite(){ return readWrite; }
	//对只写属性进行只写访问
	void setWriteOnly(int val){ writeOnly = val; }
private:
	int readOnly; //对外只读访问
	int noAccess; //外部不可访问
	int readWrite; //读写访问
	int writeOnly; //只写访问
};
```

示例：老王自己知道自己的情人是李婶，别人不知道

```c++
class Person{
public:
	
	//设置姓名
	void Set_Name(string name){m_Name=name;} 
	//获取姓名
	string Get_Name(){return m_Name;} 
	
	//设置年龄
	void Set_Age(int age){m_Age=age;}
	//获取年龄
	int Get_Age(){return m_Age;}
	
	//设置情人 
	void Set_Lover(string lover){m_Lover=lover;}

private:
	string m_Name;      //姓名 
	int m_Age;         //年龄 
	string m_Lover;    //情人 
}; 

void test()
{
	Person person;
	//设置姓名
	person.Set_Name("老王"); 
	//获取姓名
	cout<<"姓名："<<person.Get_Name();
	
	//设置年龄
	person.Set_Age(50);
	//获取年龄
	cout<<"  年龄："<<person.Get_Age()<<endl;
	
	//设置情人 
	person.Set_Lover("李婶");
}
```

# 面向对象设计案例

## 1、立方体

设计立方体类(Cube)，求出立方体的面积( 2*a*b + 2*a*c + 2*b*c )和体积( a * b * c)，分别用全局函数和成员函数判断两个立方体是否相等。

```c++
class Cube{
public:
	//设置长、宽、高
	void Set_L(int l){m_L=l;}  
	void Set_W(int w){m_W=w;}  
	void Set_H(int h){m_H=h;}
	
	//获取长、宽、高
	int Get_L(){return m_L;}
	int Get_W(){return m_W;}
	int Get_H(){return m_H;}
	
	//求表面积
	int caculateS(){return 2*(m_L*m_W+m_L*m_H+m_L*m_H);} 
	
	//求体积 
	int caculateV(){return m_L*m_W*m_H;} 
	
private:
	int m_L;   //长 
	int m_W;   //宽 
	int m_H;   //高 
};

void test()
{
	Cube cube;
	//设置长、宽、高
	cube.Set_L(10); 
	cube.Set_W(10); 
	cube.Set_H(10);
	
	//获取长、宽、高
	cout<<"长:"<<cube.Get_L()<<endl;
	cout<<"宽:"<<cube.Get_W()<<endl;
	cout<<"高:"<<cube.Get_H()<<endl;
	
	//计算表面积
	cout<<"面积："<<cube.caculateS()<<endl;
	
	//计算体积
	cout<<"体积："<<cube.caculateV()<<endl; 
}
```

## 2、点和圆的关系

设计一个圆形类（Circle），和一个点类（Point），计算点和圆的关系。

假如圆心坐标为x0, y0, 半径为r，点的坐标为x1, y1：

* 点在圆上：(x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) == r*r
* 点在圆内：(x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) < r*r
* 点在圆外：(x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) > r*r

```c++
class Point{
public:
	//设置坐标
	void Set_XY(int x,int y){m_X=x;m_Y=y;} 
	
	//获取坐标 
	int Get_X(){return m_X;}
	int Get_Y(){return m_Y;}
	
private:
	int m_X;   //x坐标 
	int m_Y;   //y坐标 
};
class Cicle{
public:
	//设置圆心
	void Set_CenetrPoint(Point point){m_Point=point;} 
	//获取圆心 
	int Get_CenterPointX(){return m_Point.Get_X();}
	int Get_centerPointY(){return m_Point.Get_Y();}
	
	//设置半径
	void Set_R(int r){m_R=r;}
	//获取半径 
	int Get_R(){return m_R;}
	
	//判断点和圆的关系
	void IsPointInCircle(Point point)
	{
		/*int distance=(point.Get_X()-Get_CenterPointX())*(point.Get_X()-Get_CenterPointX())
		+(point.Get_Y()-Get_centerPointY())*(point.Get_Y()-Get_centerPointY());*/
		int distance=(point.Get_X()-m_Point.Get_X())*(point.Get_X()-m_Point.Get_X())
		+(point.Get_Y()-m_Point.Get_Y())*(point.Get_Y()-m_Point.Get_Y());
		if(distance==(m_R*m_R))
			cout<<"点在圆上"<<endl;
		else if(distance>(m_R*m_R))
			cout<<"点在圆外"<<endl;
		else
			cout<<"点在圆内"<<endl; 
	}
	
private:
	Point m_Point;   //圆心
	int m_R;        //半径 
};

//全局函数判断点和圆的关系
void IsPointInCircle1(Point point,Cicle cicle) 
{
	int distance=(point.Get_X()-cicle.Get_CenterPointX())*(point.Get_X()-cicle.Get_CenterPointX())
	+(point.Get_Y()-cicle.Get_centerPointY())*(point.Get_Y()-cicle.Get_centerPointY());
	if(distance==(cicle.Get_R()*cicle.Get_R()))
		cout<<"点在圆上"<<endl;
	else if(distance>(cicle.Get_R()*cicle.Get_R()))
		cout<<"点在圆外"<<endl;
	else
		cout<<"点在圆内"<<endl; 
}

void test()
{
	Point point;  //圆心
	Cicle cicle;  //圆 
	//设置圆心
	point.Set_XY(10,10);
	cicle.Set_CenetrPoint(point);
	
	//获取圆心 
	cout<<"圆心:("<<cicle.Get_CenterPointX()<<","<<cicle.Get_centerPointY()<<")"<<endl;

	//设置半径
	cicle.Set_R(5);
	//获取半径 
	cout<<"半径:"<<cicle.Get_R()<<endl;
	
	//点
	Point point1;
	point1.Set_XY(10,14);
	//成员函数判断点和圆的关系
	cout<<"成员函数函数判断下:" ;
	cicle.IsPointInCircle(point1);	
	
	//全局函数判断点和圆的关系
	cout<<"全局函数函数判断下:" ;
	IsPointInCircle1(point1,cicle);
}
```



