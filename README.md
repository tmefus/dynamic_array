# C语言的动态数组简单实现

### 注意

_1、动态数组只能存放堆区的元素_

```c
int a = 1;            // a 元素在栈区
addArray(array, &a);  // 将栈区的元素放入数组
removeArray(array, 0);// 删除时会报错，不能free栈区变量
```

* 所有删除、释放的函数都会因为这个原因报错

---
_2、循环中的错误_

```c
for(int i = 0; i < array.size; i++) {
   removeByIdx(array, i);
   // removeByObj(array, element); 按元素删除也不行
}
```

* 因为每一次循环都会判断 array.size，而删除元素的行为会改变 array.size的值，所以会导致循环出错。

---

### 全部函数

```c
// 初始化, 传入初始容量
Array *initArray(int capacity);

// 添加元素
void addArray(Array *arr, void *data);

// 在指定位置插入元素
void insertArray(Array *arr, int index, void *data)

// 判断元素是否在数组中, 在 返回元素下标，不在返回 -1
int objInArray(Array *arr, void *obj);

// 通过索引删除元素
void removeByIdx(Array *arr, int index);

// 通过元素删除 ，相同obj只删除第一个
void removeByObj(Array *arr, void *obj);

// 反向数组中元素
void reverseArray(Array *array);

// 动态数组排序
void sortArray(Array *array, int (*compare)(const void *, const void *));

// 打印数组
void printArray(Array *array, void (*print)(const void *));

// 清空元素，但不释放数组，数组可以再用
void cleanArray(Array *arr);

// 释放array
void deleteArray(Array *arr);
```

另外，还提供了两个内存相关的宏函数，都对错误进行过处理，可以代替库函数的相关函数使用

```c
// 申请堆内存，可以替代 malloc函数
void *MALLOC(size);

// 重新分配堆内存，可以替代 realloc函数
void *RELLOC(ptr, size);
```

### 以上功能在 main 文件中有具体使用方法

