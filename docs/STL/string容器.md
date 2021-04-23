# string容器基本概念

C风格字符串(以空字符结尾的字符数组)太过复杂难于掌握，不适合大程序的开发，所以C++标准库定义了一种string类，定义在**头文件**`<string>`。

String和c风格字符串对比：

* char *是一个指针，String是一个类

  string封装了char * ，管理这个字符串，是一个char*型的容器。

* string封装了很多实用的成员方法

  查找find，拷贝copy，删除delete 替换replace，插入insert

* 不用考虑内存释放和越界

   string管理char*所分配的内存。每一次string的复制，取值都由string类负责维护，不用担心复制越界和取值越界

# string容器常用操作

## 1、 string 构造函数

| 原型                         | 作用                                     |
| ---------------------------- | ---------------------------------------- |
| `string();`                  | 创建一个空的字符串 例如: string str;     |
| `string(const string& str);` | 使用一个string对象初始化另一个string对象 |
| `string(const char* s);`     | 使用字符串s初始化                        |
| `string(int n, char c);`     | 使用n个字符c初始化                       |

```c++
void test(){
	string str;          // 1.string();
	cout<<str<<endl;    //输出：空字符串
	
	string str1("aaaa");  // 3.string(const char* s);
	cout<<str1<<endl;   //输出：aaaa
	
	string str2(5,'c');  // 4.string(int n, char c);
	cout<<str2<<endl;    //输出：ccccc
	
	string str3(str1);   // 2.string(const string& str);
	cout<<str3<<endl;   //输出：aaaa
}
```

## 2、string基本赋值操作

| 原型                                                 | 作用                                 |
| ---------------------------------------------------- | ------------------------------------ |
| `string& operator=(const char* s); `                 | char*类型字符串 赋值给当前的字符串   |
| `string& operator=(const string &s);`                | 把字符串s赋给当前的字符串            |
| `string& operator=(char c);`                         | 字符赋值给当前的字符串               |
| `string& assign(const char *s);`                     | 把字符串s赋给当前的字符串            |
| `string& assign(const char *s, int n);`              | 把字符串s的前n个字符赋给当前的字符串 |
| `string& assign(const string &s);`                   | 把字符串s赋给当前字符串              |
| `string& assign(int n, char c);`                     | 用n个字符c赋给当前字符串             |
| `string& assign(const string &s, int start, int n);` | 将s从start开始n个字符赋值给字符串    |

```c++
void test(){
	char *str="aaaa";  //string& operator=(const char* s); 
	string str1=str;
	cout<<str1<<endl;
	
	string str2=str1;   //string& operator=(const string &s);
	cout<<str2<<endl;
	
	string str4;
	str4.assign("bbbb");   //string& assign(const char *s);
	cout<<str4<<endl;
	
	string str5;
	str5.assign("hellowrold",5);   //string& assign(const char *s, int n);
	cout<<str5<<endl;
	
	string str6;
	str6.assign(str5);   //string& assign(const string &s);
	cout<<str6<<endl;
	
	string str7;
	str7.assign(5,'c');     //string& assign(int n, char c);
	cout<<str7<<endl;
	
	string str8;
	str8.assign(str5,0,5);   //string& assign(const string &s, int start, int n);
	cout<<str8<<endl;	
}
```

## 3、string存取字符操作

| 序号 | 原型                       | 作用               |
| ---- | -------------------------- | ------------------ |
| 1    | `char& operator[](int n);` | 通过[ ]方式取字符  |
| 2    | `char& at(int n);`         | 通过at方法获取字符 |

```c++
void test(){
	string str="hello";
	cout<<str[2]<<endl;        //char& operator[](int n);
	
	cout<<str.at(4)<<endl;      //char& at(int n);
}
```

注意：通过at方法获取字符时如越界操作系统会自动抛出异常

```c++
string str="hello";
cout<<str.at(5)<<endl;
//terminate called after throwing an instance of 'std::out_of_range'                             //what():  basic_string::at: __n (which is 5) >= this->size() (which is 5) 
```

注意： 为了修改string字符串的内容，下标操作符[]和at都会返回字符的引用。但当字符串的内存被重新分配之后，可能发生错误

```c++
void test(){
	string s = "abcdefg";
	char& a = s[2];
	char& b = s[3];

	a = '1';
	b = '2';

	cout << s << endl;       //ab12efg 
	cout << (int*)s.c_str() << endl;   //地址：0xc71548 

	s = "pppppppppppppppppppppppp";

	a = '1';
	b = '2';

	cout << s << endl;     //pppppppppppppppppppppppp---修改没起作用（为什么？）
	cout << (int*)s.c_str() << endl;     //地址：0xc71a48 
}
```

分析：s字符串第二字重写分配内存导致地址不一样，故前面返回的引用已失效---修改失败

## 4、string拼接操作

| 序号 | 原型                                             | 作用                                              |
| ---- | ------------------------------------------------ | ------------------------------------------------- |
| 1    | string& operator+=(const string& str);           | 重载+=操作符                                      |
| 2    | string& operator+=(const char* str);             | 重载+=操作符                                      |
| 3    | string& operator+=(const char c);                | 重载+=操作符                                      |
| 4    | string& append(const char *s);                   | 把字符串s连接到当前字符串结尾                     |
| 5    | string& append(const char *s, int n);            | 把字符串s的前n个字符连接到当前字符串结尾          |
| 6    | string& append(const string &s);                 | 同operator+=()                                    |
| 7    | string& append(const string &s, int pos, int n); | 把字符串s中从pos开始的n个字符连接到当前字符串结尾 |
| 8    | string& append(int n, char c);                   | 在当前字符串结尾添加n个字符c                      |

```c++
//中文字符占两个字节（两个char）
string str1="hello";
string str2="world";
str1+=str2;            //string& operator+=(const string& str);
cout<<str1<<endl;
	
string str3="我";
str3+="爱北京";         //string& operator+=(const char* str);
cout<<str3<<endl;
	
string str4="下课了";
str4.append("去约会");     //string& append(const char *s);
cout<<str4<<endl;
	
string str5="今天请你";
str5.append("吃饭了吗",4);      //string& append(const char *s, int n);
cout<<str5<<endl;
	
string str6="我喜欢";
string str7="你妹妹";
str6.append(str7);             //string& append(const string &s);
cout<<str6<<endl;
	
string str8="我觉得你";
string str9="不是人";
str8.append(str9,2,4);        //string& append(const string &s, int pos, int n);
cout<<str8<<endl;
	
str1.append(5,'!');          //string& append(int n, char c);
cout<<str1<<endl;
```

## 5、string查找和替换

find()---从前往后找、rfind()---从后往前找，注意pos的值

| 序号 | 原型                                                | 作用                                |
| ---- | --------------------------------------------------- | ----------------------------------- |
| 1    | int find(const string& str, int pos = 0) const;     | 查找str第一次出现位置,从pos开始查找 |
| 2    | int find(const char* s, int pos = 0) const;         | 查找s第一次出现位置,从pos开始查找   |
| 3    | int find(const char* s, int pos, int n) const;      | 从pos位置查找s的前n个字符第一次位置 |
| 4    | int find(const char c, int pos = 0) const;          | 查找字符c第一次出现位置             |
| 5    | int rfind(const string& str, int pos = npos) const; | 查找str最后一次位置,从pos开始查找   |
| 6    | int rfind(const char* s, int pos = npos) const;     | 查找s最后一次出现位置,从pos开始查找 |
| 7    | int rfind(const char* s, int pos, int n) const;     | 从pos查找s的前n个字符最后一次位置   |
| 8    | int rfind(const char c, int pos = 0) const;         | 查找字符c最后一次出现位置           |
| 9    | string& replace(int pos, int n, const string& str); | 替换从pos开始n个字符为字符串str     |
| 10   | string& replace(int pos, int n, const char* s);     | 替换从pos开始的n个字符为字符串s     |

```c++
void test(){
	string str="llo";
	string str1="helloworld";
	cout<<"位置："<<str1.find(str,0)<<endl;   //int find(const string& str, int pos = 0) const;
	
	cout<<"位置："<<str1.find("wor",0)<<endl;   //int find(const char* s, int pos = 0) const;
	
	cout<<"位置："<<str1.find("world",0,2)<<endl;  //int find(const char* s, int pos, int n) const; 
	
	cout<<"位置："<<str1.find('w',0)<<endl;    //int find(const char c, int pos = 0) const;  
	
	string str2="helloworllod";
	cout<<"位置："<<str2.rfind(str,11)<<endl; //int rfind(const string& str, int pos = npos) const;
	
	cout<<"位置："<<str2.rfind("llo",11)<<endl;  //int rfind(const char* s, int pos = npos) const;
	
	cout<<"位置："<<str2.rfind("llo",11,2)<<endl; //int rfind(const char* s, int pos, int n) const;
	
	cout<<"位置："<<str2.rfind('w',11)<<endl;    //int rfind(const char c, int pos = 0) const; 
	
	string str3="world";
	str2.replace(5,7,str3);     //string& replace(int pos, int n, const string& str); 
	cout<<str2<<endl;
	
	cout<<str2.replace(5,7,"world")<<endl;   //string& replace(int pos, int n, const char* s); 
}
```

## 6、string比较操作

compare函数在>时返回 1，<时返回 -1，==时返回 0。

比较区分大小写，比较时参考字典顺序，排越前面的越小。

大写的A比小写的a小。

| 序号 | 原型                                | 作用          |
| ---- | ----------------------------------- | ------------- |
| 1    | int compare(const string &s) const; | 与字符串s比较 |
| 2    | int compare(const char *s) const;   | 与字符串s比较 |

```c++
void test(){
	string str1 = "abcde";
	string str2 = "abcdef";

	if (str1.compare(str2) == 0)
		cout << "str1 == str2" << endl;
	else if (str1.compare(str2) > 0)
		cout << "str1 > str2" << endl;
	else
		cout << "str1 < str2" << endl;
}
```

## 7、string子串

`string substr(int pos = 0, int n = npos) const;`                返回由pos开始的n个字符组成的字符串

```c++
string str="helloworld";
cout<<str.substr(5,5)<<endl;
```

## 8、string插入和删除操作

| 序号 | 原型                                        | 作用                   |
| ---- | ------------------------------------------- | ---------------------- |
| 1    | string& insert(int pos, const char* s);     | 插入字符串             |
| 2    | string& insert(int pos, const string& str); | 插入字符串             |
| 3    | string& insert(int pos, int n, char c);     | 在指定位置插入n个字符c |
| 4    | string& erase(int pos, int n = npos);       | 删除从Pos开始的n个字符 |

```c++
void test(){
	string str="helloworld";
	str.insert(5,"aaaaa");
	cout<<str<<endl;
	
	str="helloworld";
	string str1="bbbbb";
	str.insert(5,str1);
	cout<<str<<endl;
	
	str="helloworld";
	str.insert(5,5,'c');
	cout<<str<<endl;
	
	str.erase(5,5);
	cout<<str<<endl;
}
```

## 9、string和c-style字符串转换

在c++中存在一个从const char*到string的隐式类型转换，却不存在从一个string对象到C_string的自动类型转换。对于string类型的字符串，可以通过c_str()函数返回string对象对应的C_string.

```c++
//const char*转string
const char* str="hello";
string str1=str;     //隐式转换
cout<<str1<<endl;
  
//string 转 const char*
string str2("world");
const char* str3=str2.c_str();
cout<<str3<<endl;
```

# 练习

## 1、解析字符串

解析字符串："www.itcast.com.cn"----将 www  itcast  com  cn 单词截取到 vector容器中

```c++
void test(){
	string str="www.itcast.com.cn";
	vector<string>v;
	
	int pos;
	int start=0;
	string str1;
	while(1){
		pos=str.find('.',start);
		if(pos==-1){
			str1=str.substr(start,str.size()-start);
			v.push_back(str1);
			break;
		}
		str1=str.substr(start,pos-start);
		v.push_back(str1);
		start=pos+1;
	}
	
	for(vector<string>::iterator it=v.begin();it!=v.end();it++){
		cout<<*it<<endl;
	}
}
```

## 2、大小写转换

将string字符串中的所有小写字母都变为大写字母。--- 反

```c++
void test(){
	string str = "abcdEFg";
	
	for(int i=0;i<str.size();i++){
		str[i]=tolower(str[i]);    //转小写
		cout<<str.at(i);
	}
	cout<<endl;
	
	str = "abcdEFg";
	for(int i=0;i<str.size();i++){
		str[i]=toupper(str[i]);    //转大写
		cout<<str.at(i);
	}
	cout<<endl;
}
```







