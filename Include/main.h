/*
    main.h
    主程序和公共头文件
*/

#ifndef _MAIN_H_
#define _MAIN_H_

// 引入头文件
#include <io.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 解除宏定义冲突
#undef ERROR
#undef DELETE
#undef CopyFile

// 宏定义
// 路径
#define CONFIG_FILE_PATH ".\\config\\config.bin" // 配置文件的路径
#define DK_WALLPAPER_PATH ".\\wallpaper\\dk.bin" // 深色模式壁纸的路径
#define LT_WALLPAPER_PATH ".\\wallpaper\\lt.bin" // 浅色模式壁纸的路径
// 常数
#define TROPIC_LAT 23.4333f // 回归线纬度
#define SUN_MOVE_PER_DAY 0.2568f // 太阳直射点每日位移纬度
#define PER_RADIAN 0.0175f // 角度转弧度参数
// 注册表键值
#define ROOT_KEY HKEY_CURRENT_USER // 根键
#define THEMES_SUB_KEY "software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize" // 是否使用浅色模式键
#define WINDOWS_MODE_VALUE "SystemUsesLightTheme" // 是否使用浅色Windows模式值
#define APP_MODE_VALUE "AppsUseLightTheme" // 是否使用浅色应用模式值

// 枚举类型
enum SELECTEDMODE // 选择的时间模式
{
    FIXEDTIME, // 固定时间模式
    FREETIME // 自动变更时间模式
};
enum DIR // 南北方向
{
    SOUTH, // 南
    NORTH // 北
};

// 结构体类型
struct FixedTimeModeSetting // 固定时间模式设置项
{
    unsigned char startH; // 开始时间小时
    unsigned char startM; // 开始时间分钟
    unsigned char endH; // 结束时间小时
    unsigned char endM; // 结束时间分钟
};
struct FreeTimeModeSetting // 自动变更时间模式设置项
{
    char lngDir; // 经度方向
    unsigned char lngD; // 经度度
    unsigned char lngM; // 经度分
    unsigned char lngS; // 经度秒
    char latD; // 纬度度
    unsigned char latM; // 纬度分
    unsigned char latS; // 纬度秒
};
struct Setting // 设置项
{
    unsigned char selectedMode; // 选择的时间模式
    struct FixedTimeModeSetting fixedTimeModeSetting; // 固定时间模式设置项
    struct FreeTimeModeSetting freeTimeModeSetting; // 自动变更时间模式设置项
    unsigned char isChangeWindowsMode; // 是否更改Windows模式
    unsigned char isChangeAppMode; // 是否更改应用模式
    unsigned char isChangeWallpaper; // 是否更改壁纸
};

#endif
