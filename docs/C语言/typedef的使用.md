# typedef的使用

## 1、为基本数据类型定义新的类型名

```
typedef double MYDBL;
```



## 2、为自定义类型（结构体、共用体和枚举）起别名 -  简化类型名关键字

```c
typedef struct Person
{
	char name[64];
	int age;
}myPerson;

void test01()
{
	struct Person p = { "aaa", 10 };

	myPerson p2 = { "bbb", 20 };   //起别名，简化

}
```

## 3、区分数据类型

```c
void test02()
{
	typedef char * PCHAR;    // char *起别名为 PCHAR
	PCHAR p1, p2;            //p1和p2都是 char *类型
    
	//char *p1, p2;          //  p1 char *类型    p2 char类型

	char *p3, *p4;           //p1和p2都是 char *类型
}
```

## 4、提高移植性

```c
typedef long long  MYLONG;      //long long取别名为 MYLONG  
void test03()
{
	MYLONG a = 10;

	MYLONG b = 10;

}
```

注意：typedef 是用来定义一种类型的新别名的，它不同于宏，不是简单的字符串替换

```c
typedef char* PCHAR;
int strcmp(const PCHAR,const PCHAR);
//不等于 const PCHAR！= const char*
int strcmp(const char*， PCHAR);
//实际上
const PCHAR=char* const;
//修正
typedef (const char*) PCHAR;
int strcmp(PCHAR， PCHAR);
```

