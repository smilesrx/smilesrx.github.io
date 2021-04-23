---
typora-root-url: Image
---

# 文件流类和文件流对象

输入输出是以系统指定的标准设备（输入设备为键盘，输出设备为显示器）为对象的。在实际应用中，常以磁盘文件作为对象。即从磁盘文件读取数据，将数据输出到磁盘文件。

和文件有关系的输入输出类主要在`fstream.h`这个头文件中被定义，在这个头文件中主要被定义了三个类，由这三个类控制对文件的各种输入输出操作，他们分别是`ifstream`、`ofstream`、`fstream`，其中`fstream`类是由`iostream`类派生而来，他们之间的继承关系见下图所示：

![C++类库中的流类](/C++类库中的流类.png)

* `ifstream`：专用于从文件中读取数据；
* `ofstream`：专用于向文件中写入数据；
* `fstream`：既可用于从文件中读取数据，又可用于向文件中写入数据。
* 这 3 个文件流类都位于 `<fstream> `头文件中

可以看到，`ifstream` 类和 `iostream` 类是从 `istream` 类派生而来的，因此` ifstream `类拥有` istream `类的全部成员方法。同样地，`ofstream` 和 `iostream` 类也拥有 `ostream` 类的全部成员方法。这也就意味着，`istream `和 `ostream` 类提供的供 `cin` 和 `cout` 调用的成员方法，也同样适用于文件流。并且这 3 个类中有些成员方法是相同的，比如 `operator <<()`、`operator >>()`、`peek()`、`ignore()`、`getline()`、`get()` 等。

值得一提的是，和 `<iostream>` 头文件中定义有` ostream` 和 `istream` 类的对象 `cin `和 `cout `不同，`<fstream> `头文件中并没有定义可直接使用的` fstream`、`ifstream` 和 `ofstream` 类对象。因此，如果我们想使用该类操作文件，需要自己创建相应类的对象。

`fstream` 类拥有 `ifstream` 和` ofstream` 类中所有的成员方法，

下表罗列了 `fstream `类一些常用的成员方法:

| 成员方法名        | 适用类对象                                                   | 功  能                                     |
| ----------------- | ------------------------------------------------------------ | ------------------------------------------ |
| open()            | fstream ifstream ofstream                                    | 打开指定文件，使其与文件流对象相关联。     |
| is_open()         | 检查指定文件是否已打开。                                     |                                            |
| close()           | 关闭文件，切断和文件流对象的关联。                           |                                            |
| swap()            | 交换 2 个文件流对象。                                        |                                            |
| operator>>        | fstream ifstream                                             | 重载 >> 运算符，用于从指定文件中读取数据。 |
| gcount()          | 返回上次从文件流提取出的字符个数。该函数常和 get()、getline()、ignore()、peek()、read()、readsome()、putback() 和 unget() 联用。 |                                            |
| get()             | 从文件流中读取一个字符，同时该字符会从输入流中消失。         |                                            |
| getline(str,n,ch) | 从文件流中接收 n-1 个字符给 str 变量，当遇到指定 ch 字符时会停止读取，默认情况下 ch 为 '\0'。 |                                            |
| ignore(n,ch)      | 从文件流中逐个提取字符，但提取出的字符被忽略，不被使用，直至提取出 n 个字符，或者当前读取的字符为 ch。 |                                            |
| peek()            | 返回文件流中的第一个字符，但并不是提取该字符。               |                                            |
| putback(c)        | 将字符 c 置入文件流（缓冲区）。                              |                                            |
| operator<<        | fstream ofstream                                             | 重载 << 运算符，用于向文件中写入指定数据。 |
| put()             | 向指定文件流中写入单个字符。                                 |                                            |
| write()           | 向指定文件中写入字符串。                                     |                                            |
| tellp()           | 用于获取当前文件输出流指针的位置。                           |                                            |
| seekp()           | 设置输出文件输出流指针的位置。                               |                                            |
| flush()           | 刷新文件输出流缓冲区。                                       |                                            |
| good()            | fstream ofstream ifstream                                    | 操作成功，没有发生任何错误。               |
| eof()             | 到达输入末尾或文件尾。                                       |                                            |

举例：

```c++
#include <iostream>
#include <fstream>
using namespace std;
int main() {
    const char *url ="http://www.baidu.com";
    //创建一个 fstream 类对象
    fstream fs;
    //将 test.txt 文件和 fs 文件流关联
    fs.open("test.txt", ios::out);
    //向test.txt文件中写入 url 字符串
    fs.write(url, 30);
    fs.close();
    return 0;
}
```

# 文件打开

在对文件进行读写操作之前，先要打开文件。

## 1、打开文件有以下两个目的：

* 通过指定文件名，建立起文件和文件流对象的关联，以后要对文件进行操作时，就可以通过与之关联的流对象来进行。
* 指明文件的使用方式。使用方式有只读、只写、既读又写、在文件末尾添加数据、以文本方式使用、以二进制方式使用等多种。

## 2、打开文件可以通过以下两种方式进行：

- 调用流对象的 open 成员函数打开文件。

  `ofs.open("./test.txt", ios::out | ios::trunc); `

- 定义文件流对象时，通过构造函数打开文件。

  `ofstream  ofs("./test.txt", ios::out | ios::trunc);`

## 3、使用open函数打开文件

1. 函数原型：

   `void open(const char* szFileName, int mode)`

2. 参数：

   第一个参数是指向文件名的指针，第二个参数是文件的打开模式标记。

3. 文件的打开模式

   文件的打开模式标记代表了文件的使用方式，这些标记可以单独使用，也可以组合使用

   | 模式标记                          | 适用对象                  | 作用                                                         |
   | --------------------------------- | ------------------------- | ------------------------------------------------------------ |
   | ios::in                           | ifstream fstream          | 打开文件用于读取数据。如果文件不存在，则打开出错。           |
   | ios::out                          | ofstream fstream          | 打开文件用于写入数据。如果文件不存在，则新建该文件；如果文件原来就存在，则打开时清除原来的内容。 |
   | ios::app                          | ofstream fstream          | 打开文件，用于在其尾部添加数据。如果文件不存在，则新建该文件。 |
   | ios::ate                          | ifstream                  | 打开一个已有的文件，并将文件读指针指向文件末尾（读写指 的概念后面解释）。如果文件不存在，则打开出错。 |
   | ios:: trunc                       | ofstream                  | 打开文件时会清空内部存储的所有数据，单独使用时与 ios::out 相同。 |
   | ios::binary                       | ifstream ofstream fstream | 以二进制方式打开文件。若不指定此模式，则以文本模式打开。     |
   | ios::in \| ios::out               | fstream                   | 打开已存在的文件，既可读取其内容，也可向其写入数据。文件刚打开时，原有内容保持不变。如果文件不存在，则打开出错。 |
   | ios::in \| ios::out               | ofstream                  | 打开已存在的文件，可以向其写入数据。文件刚打开时，原有内容保持不变。如果文件不存在，则打开出错。 |
   | ios::in \| ios::out \| ios::trunc | fstream                   | 打开文件，既可读取其内容，也可向其写入数据。如果文件本来就存在，则打开时清除原来的内容；如果文件不存在，则新建该文件。 |

   在流对象上执行 open 成员函数，给出文件名和打开模式，就可以打开文件。判断文件打开是否成功，可以看“对象名”这个表达式的值是否为 true，如果为 true，则表示文件打开成功。

   ```c++
   #include <iostream>
   #include <fstream>
   using namespace std;
   int main(){
       ifstream ifs;
       ifs.open("./test.txt", ios::in);
       if (ifs)  //条件成立，则说明文件打开成功
           inFile.close();
       else
           cout << "test.txt doesn't exist" << endl;
       
       ofstream ofs;
       ofs.open("test1.txt", ios::out);
       if (!ofs)  //条件成立，则说明文件打开出错
           cout << "error 1" << endl;
       else
           ofs.close();
       ofs.open("./test2.txt", ios::out | ios::in);
       if (ofs)  //条件成立，则说明文件打开成功
           ofs.close();
       else
           cout << "error 2" << endl;
       
       fstream fs;
       fs.open("./test3.txt", ios::out | ios::in | ios::trunc);
       if (!fs)
           cout << "error 3" << endl;
       else
           fs.close();
       return 0;
   }
   ```

## 4、使用流类的构造函数打开文件

定义流对象时，在构造函数中给出文件名和打开模式也可以打开文件。以 `ifstream` 类为例，

* 它有如下构造函数：

  `ifstream::ifstream (const char* szFileName, int mode = ios::in, int);`

* 参数：

  第一个参数是指向文件名的指针；

  第二个参数是打开文件的模式标记，默认值为`ios::in`;

  第三个参数是整型的，也有默认值，一般极少使用。

```c++
#include <iostream>
#include <fstream>
using namespace std;
int main(){
    ifstream ifs("./test.txt", ios::in);
    if (ifs)
        ifs.close();
    else
        cout << "test.txt doesn't exist" << endl;
    
    ofstream ofs("test1.txt", ios::out);
    if (!ofs)
        cout << "error 1";
    else
        ofs.close();
    
    fstream fs("tmp\\test2.txt", ios::out | ios::in);
    if (!fs)
        cout << "error 2";
    else
        fs.close();
    return 0;
}
```

## 5、注意

1. 新版本的I/O类库中不提供`ios::nocreate`和`ios::noreplace`。

2. 每一个打开的文件都有一个文件指针，该指针的初始位置由I/O方式指定，每次读写都从文件指针的当前位置开始。每读入一个字节，指针就后移一个字节。当文件指针移到最后，就会遇到文件结束`EOF`（文件结束符也占一个字节，其值为-1)，此时流对象的成员函数`eof`的值为非0值(一般设为1)，表示文件结束 了。

3. 可以用“位或”运算符“|”对输入输出方式进行组合:

   * `ios::in | ios:: noreplace `    打开一个输入文件，若文件不存在则返回打开失败的信息
   * ` ios::app | ios::nocreate`      打开一个输出文件，在文件尾接着写数据，若文件不存在，则返回打开失败的信息
   * `ios::out l ios::noreplace`  打开一个新文件作为输出文件，如果文件已存在则返回打开失败的信息
   * `ios::in l ios::out I ios::binary`    打开一个二进制文件，可读可写
   * 但不能组合互相排斥的方式，如 `ios::nocreate l ios::noreplace`。

4. 如果打开操作失败，open函数的返回值为0(假)，如果是用调用构造函数的方式打开文件的，则流对象的值为0。可以据此测试打开是否成功。如:

   ```c++
   if(outfile.open("f1.bat", ios::app) ==0)
           cout <<"open error";
   //或
       if( !outfile.open("f1.bat", ios::app) )
           cout <<"open error";
   ```

   ## 6、举例

   ```c++
   void test01(){
   	//写文件  o --  输出    
   	ofstream  ofs("./test.txt", ios::out | ios::trunc);
   	//ofs.open("./test.txt", ios::out | ios::trunc);  设置打开方式 以及路径
   	if (!ofs.is_open()){
   		cout << "文件打开失败" << endl;
   		return;
   	}
   	ofs << "姓名：孙悟空" << endl;
   	ofs << "年龄：999" << endl;
   	ofs << "性别：男" << endl;
   	//关闭文件
   	ofs.close();
   }
   ```

# 利用4种方式 对文件进行读取

* 方式一

  ```c++
  char buf[1024] = { 0 };
  while (ifs >> buf){
  	cout << buf << endl;
  }
  ```

* 方式二

  ```c++
  char buf[1024] = { 0 };
  while (ifs.getline(buf,sizeof(buf))){
  	cout << buf << endl;
  }
  ```

* 方式三

  ```c++
  第三种方式
  string buf;
  while ( getline(ifs,buf) ){
  	cout << buf << endl;
  }
  ```

* 方式四

  ```c++
  char  c;
  while ( (c = ifs.get()) != EOF ){
  	cout << c;
  }
  ```

# 文件关闭

在对已打开的磁盘文件的读写操作完成后，应关闭该文件。关闭文件用成员函数close。如：`outfile.close( );`  将输出文件流所关联的磁盘文件关闭

所谓关闭，实际上是解除该磁盘文件与文件流的关联，原来设置的工作方式也失效，这样，就不能再通过文件流对该文件进行输入或输出

```c++
void test02()
{
	//读文件   i  -- 输入
	ifstream  ifs;
	ifs.open("./test.txt", ios::in);
	if (!ifs.is_open()){
		cout << "文件打开失败" << endl;
		return;
	}
}
```

