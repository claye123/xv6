#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
  int i, count = 0, k, m = 0;
  char* lineSplit[MAXARG], *p;
  char block[32], buf[32];
  p = buf;
  for (i = 1; i < argc; i++) {
    lineSplit[count++] = argv[i];
  }
  while ((k = read(0, block, sizeof(block))) > 0) {
    for (i = 0; i < k; i++) {
      if (block[i] == '\n') {
        buf[m] = 0;   //  数组中的字符末尾设置为字符串结束符号
        lineSplit[count++] = p;   //  将指针 p 存储到 lineSplit 数组中，并更新 count
        lineSplit[count] = 0;   // 将 lineSplit 数组的下一个位置设置为 NULL，表示字符串数组的结束
        m = 0;   //  重置为 0，准备处理下一行的单词。
        p = buf;
        count = argc - 1;
        if (fork() == 0) {
          exec(argv[1], lineSplit);
        }
        wait(0);
      } else if (block[i] == ' ') {
        buf[m++] = 0;              
        lineSplit[count++] = p;
        p = &buf[m];   //   将指针 p 指向 buf 数组中下一个单词的首地址
      } else {
        buf[m++] = block[i];   // 如果当前字符不是换行符也不是空格，则将当前字符存储到 buf 数组中
      }
    }
  }
  exit(0);
}