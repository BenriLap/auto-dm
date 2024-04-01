/*
    selfstart.h
    自启动
*/

#ifndef _SELFSTART_H_
#define _SELFSTART_H_

// 引入头文件
#include "../Include/main.h"

// 宏定义
#define RUN_SUB_KEY "software\\Microsoft\\Windows\\CurrentVersion\\Run" // 运行项目键
#define RUN_NOTI_SUB_KEY "software\\Microsoft\\Windows\\CurrentVersion\\RunNotification" // 运行通知项目键
#define RUN_VALUE "AutoDM" // 运行项目值
#define RUN_NOTI_VALUE "StartupTNotiAutoDM" // 运行通知项目值 

// 枚举类型
enum INVOKEMODE // 调用自启动注册表模式
{
    ISEXIST, // 是否存在
    CREATE, // 新建
    DELETE // 删除
};

// 函数声明
/*
    调用自启动注册表
    \param 调用自启动模式
    \return 1:调用成功、注册表值存在 0:调用失败、注册表值不存在
*/
int InvokeSelfStart(enum INVOKEMODE invokeMode);

#endif
