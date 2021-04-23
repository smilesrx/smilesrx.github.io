# const修饰指针的三种效果

## 1、const	int	*p

​		const向右修饰

​		const 修饰 *p -------->  *p不可被修改

​											  p可被修改

## 2、int	*  const 	p



​		const 修饰 p -------->  p不可被修改

​										  *p可被修改

## 3、const	int	*  const	p

​		const 修饰 *p和p -------->  p不可被修改

​										         *p不可被修改