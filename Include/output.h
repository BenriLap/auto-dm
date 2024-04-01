/*
    output.h
    输出信息
*/

#ifndef _OUTPUT_H_
#define _OUTPUT_H_

// 引入头文件
#include "../Include/main.h"

// 宏定义
#define THE_VER "0.1.0.0" // 当前应用程序版本

// 枚举类型
enum INFOMODE // 输出信息类型
{
    ERROR, // 错误
    WARNING, // 警告
    SUCCESS, // 成功
    ASK, // 询问
    CONFIG // 配置
};

// 函数声明
/*
    输出版本信息
*/
void OVerInfo();
/*
    输出帮助信息
*/
void OHelpInfo();
/*
    输出设置项信息
    \param 设置项结构体指针
*/
void OSettingInfo(struct Setting *setting);
/*
    输出信息
    \param 输出信息类型
    \param 信息内容字符指针
    \return 1:输出成功、输入肯定回答 0:输入否定回答 -1:输入格式错误
*/
int OInfo(enum INFOMODE infoMode, char *info);

#endif
