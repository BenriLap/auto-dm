/*
    copyfile.h
    复制文件
*/

#ifndef _COPYFILE_H_
#define _COPYFILE_H_

// 引入头文件
#include "../Include/main.h"

// 宏定义
#define COPY_BUFFER_SIZE 4 // 复制过程缓冲区大小

// 函数声明
/*
    复制文件
    \param 原始文件路径字符指针
    \param 目标文件路径字符指针
    \return 1:复制成功 0:复制失败
*/
int CopyFile(char *origPath, char *tgtPath);

#endif
