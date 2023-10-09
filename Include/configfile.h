/*
    configfile.h
    读写、新建配置文件
*/

#ifndef _CONFIGFILE_H_
#define _CONFIGFILE_H_

// 引入头文件
#include "../Include/main.h"

// 宏定义
#define THE_CONFIG_FILE_VER 0 // 当前应用程序对应配置文件版本

// 枚举类型
enum ROWCONFIGFILEMODE // 读写配置文件模式
{
    READ, // 读
    WRITE // 写
};
enum CREATECONFIGFILEMODE // 新建配置文件模式
{
    CHECK, // 检查已有配置文件
    PASS, // 跳过检查已有配置文件
};

// 声明函数
/*
    读写配置文件
    \param 读写配置文件模式
    \param 设置项结构体指针
    \param 配置文件路径字符指针
    \return 1:读写成功 0:读写失败
*/
int ROWConfigFile(enum ROWCONFIGFILEMODE configFileMode, struct Setting *setting, char *configFilePath);
/*
    新建配置文件
    \param 新建配置文件模式
    \return 1:新建成功 0:新建失败
*/
int CreateConfigFile(enum CREATECONFIGFILEMODE createConfigMode);

#endif
