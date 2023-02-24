#include <stdio.h>

#include "array.c"

// 打印数组的自定义函数
void print(const void *obj) {
  printf("%d ", *(int *) obj);
}

// 排序用的比较函数，返回 1 升序，返回 0 降序
int cmp(const void *obj1, const void *obj2) {
  return (*(int *) obj1) < (*(int *) obj2);
}

int main() {
  // 初始化，容量2
  Array *numbs = initArray(2);

  int *a = MALLOC(sizeof(int));
  int *b = MALLOC(sizeof(int));
  int *c = MALLOC(sizeof(int));
  int *d = MALLOC(sizeof(int));
  int *e = MALLOC(sizeof(int));

  *a = 2, *b = 3, *c = 5, *d = 1, *e = 4;

  addArray(numbs, a);
  addArray(numbs, b);
  addArray(numbs, c);
  addArray(numbs, d);
  addArray(numbs, e);

  int *f = MALLOC(sizeof(int));
  *f = 6;
  // 在下标 0处插入元素
  insertArray(numbs, 0, f);

  // 打印结果为： 6 2 3 5 1 4
  printArray(numbs, print);
  printf("\n");

  // 删除下标为 1的元素
  removeByIdx(numbs, 1);

  // 删除 b元素
  removeByObj(numbs, b);

  // 排序
  sortArray(numbs, cmp);

  // 打印结果为： 1 4 5 6
  printArray(numbs, print);
  printf("\n");

  // 将数组反向
  reverseArray(numbs);

  // 打印结果为：6 5 4 1
  printArray(numbs, print);
  printf("\n");

  // 清空数组
  cleanArray(numbs);

  // 释放数组
  freeArray(numbs);
}