#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 平台指针的大小
static const int P_S = sizeof(void *);

// 打印错误信息并退出
#define ERROR_EXIT(...) {              \
  printf("__%s()__ : ", __FUNCTION__); \
  printf(__VA_ARGS__); exit(-1);       \
}

// 申请or扩展内存
#define MALLOC(size) reallocate(NULL, size)
#define RELLOC(ptr, size) reallocate(ptr, size)

// 内存扩展
static void *reallocate(void *ptr, size_t size) {
  void *re = (ptr == NULL) ? malloc(size) : realloc(ptr, size);
  if (re == NULL) ERROR_EXIT("内存分配错误")
  return re;
}

// 动态数组结构
typedef struct {
  void **items;     // 维护在堆区创建的指针数组
  int capacity;     // 动态数组容量
  int size;         // 动态数组元素个数
} Array;

static void ensure_capacity(Array *array) {
  if (array->size == array->capacity) {       // array数组已满，需要申请更大空间
    array->capacity = array->capacity * 2;    // 动态数组新容量
    array->items = RELLOC(array->items, P_S * array->capacity);
  }
}

// 初始化
Array *initArray(int capacity) {
  if (capacity < 0 || capacity > INT_MAX) ERROR_EXIT("初始化大小错误")
  capacity = capacity < 2 ? 2 : capacity; // 最小容量为2
  Array *array = MALLOC(sizeof(Array));
  array->capacity = capacity;             // 将动态数组容量初始化为函数入参
  array->size = 0;                        // 将动态数组实际大小初始化为0
  array->items = MALLOC(P_S * capacity);  // 在堆区申请二级指针p的内存
  return array;
}

// 添加元素
void addArray(Array *array, void *obj) {
  if (array == NULL || obj == NULL) ERROR_EXIT("空指针错误")
  ensure_capacity(array); // 确保数组容量足够
  array->items[array->size++] = obj;
}

// 在指定位置插入元素
void insertArray(Array *array, int index, void *obj) {
  if (array == NULL || obj == NULL) ERROR_EXIT("空指针错误")
  if (index < 0 || index >= array->size) ERROR_EXIT("索引错误")
  ensure_capacity(array); // 确保数组容量足够
  // 因为 malloc realloc申请的内存是连续的，所以可以直接操作连续的内存空间
  memmove(array->items + index + 1, array->items + index, P_S * (array->size - index));
  array->items[index] = obj;
  array->size++;
}

// 判断元素是否在数组中, 在 返回元素下标，不在返回 -1
int objInArray(Array *array, void *obj) {
  if (array == NULL || obj == NULL) ERROR_EXIT("空指针错误")
  for (int i = 0; i < array->size; ++i) {
    if (array->items[i] == obj) return i;  // 因为items储存的是地址指针，所以比较指针就可以
  }
  return -1;
}

// 通过索引删除元素
void removeByIdx(Array *array, int index) {
  if (array == NULL) ERROR_EXIT("空指针错误")
  if (array->size == 0) ERROR_EXIT("数组为空")
  if (index >= array->size || index < 0) ERROR_EXIT("下标错误")
  free(array->items[index]);   // 删除元素
  array->items[index] = NULL;
  // 因为 malloc realloc申请的内存是连续的，所以可以直接操作连续的内存空间
  memmove(array->items + index, array->items + index + 1, P_S * (array->size - index));
  array->size--;                      // 原实际数组大小减1
  int hc = array->capacity / 2;       // 调整动态数组空间
  if (array->size == hc && hc != 0) { // 元素数量为容量一半,新容量不能为0
    array->capacity = hc;
    array->items = RELLOC(array->items, P_S * hc);
  }
}

// 通过元素删除 ，相同obj只删除第一个
void removeByObj(Array *array, void *obj) {
  int idx = objInArray(array, obj);
  if (idx == -1) ERROR_EXIT("元素不存在")
  removeByIdx(array, idx);
}

// 反向数组中元素
void reverseArray(Array *array) {
  if (array == NULL || array->items == NULL) ERROR_EXIT("空指针错误")
  void *temp;
  for (int min = 0, max = array->size - 1; min < max; min++, max--) {
    temp = array->items[min];
    array->items[min] = array->items[max];
    array->items[max] = temp;
  }
}

// (快速排序算法)
static void quickSort(void **data, int left, int right, int (*cmp)(const void *, const void *)) {
  int leftIdx = left, rightIdx = right;
  void *pivot = data[(left + right) / 2], *temp; // temp是两个变量交换时的临时存放点
  for (; leftIdx <= rightIdx;) {
    for (; cmp(data[leftIdx], pivot); leftIdx++);
    for (; cmp(pivot, data[rightIdx]); rightIdx--);
    if (leftIdx <= rightIdx) {    // 交换两个变量
      temp = data[leftIdx];
      data[leftIdx++] = data[rightIdx];
      data[rightIdx--] = temp;
    }
  }
  if (rightIdx > left) quickSort(data, left, rightIdx, cmp);
  if (leftIdx < right) quickSort(data, leftIdx, right, cmp);
}

// 动态数组排序 compare为比较函数，返回 1 升序，返回 0 降序
void sortArray(Array *array, int (*compare)(const void *, const void *)) {
  if (array == NULL || compare == NULL) ERROR_EXIT("空指针错误")
  quickSort(array->items, 0, array->size - 1, compare);
}

// 清空元素，但不释放数组，数组可以再用
void cleanArray(Array *array) {
  if (array == NULL) ERROR_EXIT("空指针错误")
  if (array->items == NULL) ERROR_EXIT("空指针错误")
  for (int i = 0; i < array->size; ++i) {
    free(array->items[i]);
    array->items[i] = NULL;
  }
  array->capacity = 2;
  array->size = 0;
}

// 释放array
void freeArray(Array *array) { // 释放占用的内存即可
  if (array == NULL || array->items == NULL) ERROR_EXIT("空指针错误")
  free(array->items);  // 释放所有元素
  array->items = NULL;
  free(array);         // 释放数组
  array = NULL;
}