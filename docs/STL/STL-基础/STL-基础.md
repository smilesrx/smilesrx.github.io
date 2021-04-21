# STL基本概念

STL 是“Standard Template Library”的缩写，中文译为“标准模板库”。STL 是 C++ 标准库的一部分，不用单独安装。

STL 从广义上分为: 容器(container) 算法(algorithm) 迭代器(iterator),容器和算法之间通过迭代器进行无缝连接。STL 几乎所有的代码都采用了**模板类**或者**模板函数**，这相比传统的由函数和类组成的库来说提供了更好的代码重用机会。

STL 就是借助**模板**把常用的数据结构及其算法都实现了一遍，并且做到了数据结构和算法的分离。例如，vector 的底层为顺序表（数组），list 的底层为双向链表，deque 的底层为循环队列，set 的底层为红黑树，hash_set 的底层为哈希表。

# STL六大组件简介

STL提供了六大组件，彼此之间可以组合套用，这六大组件分别是:容器、算法、迭代器、仿函数、适配器（配接器）、空间配置器。

1. **容器：**各种数据结构，如vector、list、deque、set、map等,用来存放数据，从实现角度来看，STL容器是一种class template。

2. **算法：**各种常用的算法，如sort、find、copy、for_each。从实现的角度来看，STL算法是一种function tempalte.

3. **迭代器：**扮演了容器与算法之间的胶合剂，共有五种类型，从实现角度来看，迭代器是一种将operator* , operator-> , operator++,operator--等指针相关操作予以重载的class template. 所有STL容器都附带有自己专属的迭代器，只有容器的设计者才知道如何遍历自己的元素。原生指针(native pointer)也是一种迭代器。

   ```c++
   void test(){
       int arr[]={1,2,3,4,5};
       int len=sizeof(arr)/sizeof(int);
       for(int *p=arr;p<arr+len;p++){
           cout<<*p<<" ";
       }
   }
   ```

4. **仿函数：**行为类似函数，可作为算法的某种策略。从实现角度来看，仿函数是一种重载了operator()的class 或者class template

5. **适配器：**一种用来修饰容器或者仿函数或迭代器接口的东西。

6. **空间配置器：**负责空间的配置与管理。从实现角度看，配置器是一个实现了动态空间配置、空间管理、空间释放的class tempalte.

STL六大组件的交互关系，容器通过空间配置器取得数据存储空间，算法通过迭代器存储容器中的内容，仿函数可以协助算法完成不同的策略的变化，适配器可以修饰仿函数。

# STL的优点

1. STL 是 C++的一部分，因此不用额外安装什么，它被内建在你的编译器之内。
2. STL 的一个重要特性是将数据和操作分离。数据由容器类别加以管理，操作则由可定制的算法定义。迭代器在两者之间充当“粘合剂”,以使算法可以和容器交互运作
3. 程序员可以不用思考 STL 具体的实现过程，只要能够熟练使用 STL 就 OK 了。这样他们就可以把精力放在程序开发的别的方面。
4. STL 具有高可重用性，高性能，高移植性，跨平台的优点。
   * 高可重用性：STL 中几乎所有的代码都采用了模板类和模版函数的方式实现，这相比于传统的由函数和类组成的库来说提供了更好的代码重用机会。关于模板的知
     识，已经给大家介绍了。
   *  高性能：如 map 可以高效地从十万条记录里面查找出指定的记录，因为 map 是采用红黑树的变体实现的。
   * 高移植性：如在项目 A 上用 STL 编写的模块，可以直接移植到项目 B 上。

# STL三大组件

## 1、容器

容器，置物之所也。

研究数据的特定排列方式，以利于搜索或排序或其他特殊目的，这一门学科我们称为数据结构。大学信息类相关专业里面，与编程最有直接关系的学科，首推数据结构与算法。几乎可以说，任何特定的数据结构都是为了实现某种特定的算法。STL容器就是将运用最广泛的一些数据结构实现出来。

常用的数据结构：数组(array),链表(list),tree(树)，栈(stack),队列(queue),集合(set),映射表(map),根据数据在容器中的排列特性，这些数据分为**序列式容器**和**关联式容器**两种。

* 序列式容器强调值的排序，序列式容器中的每个元素均有固定的位置，除非用删除或插入的操作改变这个位置。Vector容器、Deque容器、List容器等。
* 关联式容器是非线性的树结构，更准确的说是二叉树结构。各元素之间没有严格的物理上的顺序关系，也就是说元素在容器中并没有保存元素置入容器时的逻辑顺序。关联式容器另一个显著特点是：在值中选择一个值作为关键字key，这个关键字对值起到索引的作用，方便查找。Set/multiset容器 Map/multimap容器

## 2、算法

算法，问题之解法也。

以有限的步骤，解决逻辑或数学上的问题，这一门学科我们叫做算法(Algorithms).

广义而言，我们所编写的每个程序都是一个算法，其中的每个函数也都是一个算法，毕竟它们都是用来解决或大或小的逻辑问题或数学问题。STL收录的算法经过了数学上的效能分析与证明，是极具复用价值的，包括常用的排序，查找等等。特定的算法往往搭配特定的数据结构，算法与数据结构相辅相成。

算法分为:**质变算法**和**非质变算法**。

质变算法：是指运算过程中会更改区间内的元素的内容。例如拷贝，替换，删除等等

非质变算法：是指运算过程中不会更改区间内的元素内容，例如查找、计数、遍历、寻找极值等等

## 3、迭代器

迭代器(iterator)是一种抽象的设计概念，现实程序语言中并没有直接对应于这个概念的实物。在<<Design Patterns>>一书中提供了23中设计模式的完整描述，其中iterator模式定义如下：提供一种方法，使之能够依序寻访某个容器所含的各个元素，而又无需暴露该容器的内部表示方式。

迭代器的设计思维-STL的关键所在，STL的中心思想在于将容器(container)和算法(algorithms)分开，彼此独立设计，最后再一贴胶着剂将他们撮合在一起。从技术角度来看，容器和算法的泛型化并不困难，c++的class template和function template可分别达到目标，如果设计出两这个之间的良好的胶着剂，才是大难题。

**迭代器的种类:**

| 输入迭代器     | 提供对数据的只读访问                                         | 只读，支持++、==、！=                   |
| -------------- | ------------------------------------------------------------ | --------------------------------------- |
| 输出迭代器     | 提供对数据的只写访问                                         | 只写，支持++                            |
| 前向迭代器     | 提供读写操作，并能向前推进迭代器                             | 读写，支持++、==、！=                   |
| 双向迭代器     | 提供读写操作，并能向前和向后操作                             | 读写，支持++、--，                      |
| 随机访问迭代器 | 提供读写操作，并能以跳跃的方式访问容器的任意数据，是功能最强的迭代器 | 读写，支持++、--、[n]、-n、<、<=、>、>= |

## 4、案例

```c++
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string> 
using namespace std;

#include <vector>   //vector容器
#include <algorithm>    //算法 

//STL三大组件：容器、算法、迭代器 
//1.容器存储内置（基础）数据类型 
void Print(int val) {
	cout << val << " ";
}
void test()
{
	vector<int> v;      //创建一个vector容器，容器中存放元素类型是int类型
	for (int i = 0; i < 5; i++) {
		v.push_back(i);      //项容器添加数据<int> 
	}

	//迭代器
	vector<int>::iterator itStart = v.begin();   //起始迭代器   begin()方法 返回指向第一个元素的迭代器
	vector<int>::iterator itEnd = v.end();       //结束迭代器   end()方法 返回指向最后一个元素下一个位置的迭代器

	//通过迭代器遍历
	//1.while()方法
	cout << "while():" << endl;
	while (itStart != itEnd) {
		cout << *itStart << " ";
		itStart++;
	}
	cout << endl;

	//2.for方法 
	cout << "for():" << endl;
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	//for_each方法 
	cout << "for_each:" << endl;
	for_each(v.begin(), v.end(), Print);  //Print回调函数 
	cout << endl;
	cout << "for_each:" << endl;
	for_each(v.begin(), v.end(), [](int val){cout << val << " "; });
	cout<<endl;
}

//2.容器内存储自定义数据类型（类对象）
class Person {
public:
	Person(string name, int age) {
		this->m_Name = name;
		this->m_Age = age;
	}

	string m_Name;
	int m_Age;
};
void test01() {
	vector<Person> v;
	Person p1("aaaa", 10);
	Person p2("bbbb", 20);
	Person p3("cccc", 30);
	Person p4("dddd", 40);
	Person p5("eeee", 50);
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);

	for (vector<Person>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "姓名：" << (*it).m_Name << "年龄：" << (*it).m_Age << endl;
	}
}

//3.容器内存放自定义类型的指针
void test02() {
	vector<Person*> v;
	Person p1("aaaa", 10);
	Person p2("bbbb", 20);
	Person p3("cccc", 30);
	Person p4("dddd", 40);
	Person p5("eeee", 50);
	v.push_back(&p1);
	v.push_back(&p2);
	v.push_back(&p3);
	v.push_back(&p4);
	v.push_back(&p5);
	for (vector<Person*>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "姓名：" << (*it)->m_Name << " 年龄：" << (*it)->m_Age << endl;
	}
}

//4.容器嵌套容器
void test03() {
	vector<vector<int>> v;
	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	for (int i = 0; i < 5; i++) {
		v1.push_back(i);
		v2.push_back(i + 5);
		v3.push_back(i + 10);
	}
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);

	for (vector<vector<int>>::iterator it = v.begin(); it != v.end(); it++) {
		for (vector<int>::iterator it1 = (*it).begin(); it1 != (*it).end(); it1++) {
			cout << *it1 << " ";
		}
		cout << endl;
	}
}

int main() {
	//test();
	//test01();
	//test02();
	//test03();

	system("pause");
	return EXIT_SUCCESS;
}
```



