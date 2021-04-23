# 函数模板实现int、char类型数组排序

```c++
template <class T>
void Swap(T &a,T &b)   //交换
{
	T tmp=a;
	a=b;
	b=tmp;
}

template <typename T>  //选择排序
void Sort(T arr,int len)
{
	for(int i=0;i<len;++i)
	{
		int max=i;
		for(int j=i+1;j<len;++j)
		{
			if(arr[max]<arr[j])
				max=j;
		}
		if(max!=1)
			Swap(arr[i],arr[max]);
	}
}

template <class T>    //打印
void Print(T arr,int len)
{
	for(int i=0;i<len;++i)
	{
		cout<<" "<<arr[i]; 
	}
	cout<<endl;
}

void test()
{
	int IntArr[]={5,2,9,4,6,3,1,8,0,7};
	int len=sizeof(IntArr)/sizeof(int);
	Sort(IntArr,len);
	Print(IntArr,len);
	
	char CharArr[]="helloworld";
	len=strlen(CharArr);
	Sort(CharArr,len);
	Print(CharArr,len);		
}
```