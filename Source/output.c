/*
    output.c
    输出信息
*/

// 引入头文件
#include "../Include/output.h"

// 函数定义

void OVerInfo()
{
    // 设置编码格式
    SetConsoleOutputCP(65001);

    // 输出版本信息
    printf("AutoDM\n版本 %s\n", THE_VER);
    printf("(c) 苯励\n");
}

void OHelpInfo()
{
    // 设置编码格式
    SetConsoleOutputCP(65001);

    // 输出帮助信息
    printf("描述：\n");
    printf("\tAutoDM 是一个在 Windows 平台上，根据事先设定好的开始结束时间或地理位置推算出的日出日落时间，自动更改“默认 Windows 模式”和“默认应用模式”的深色、浅色设置的应用程序。\n");
    printf("参数：\n");
    printf("\tversion\t\t输出版本信息\n");
    printf("\thelp\t\t输出帮助信息\n");
    printf("\tsetting\t\t查看并更改配置文件\n");
    printf("\tsetting [path]\t导入配置文件\n");
    printf("\tselfstart\t查看并更改自启动设置\n");
}

void OSettingInfo(struct Setting *setting)
{
    // 设置编码格式
    SetConsoleOutputCP(65001);

    // 输出设置项信息
    printf("选择的时间模式: ");
    if (setting->selectedMode)
    {
        printf("自动变更时间模式\n");

        printf("经度: %d°%u\'%u\"\n", setting->freeTimeModeSetting.lngDir * setting->freeTimeModeSetting.lngD, setting->freeTimeModeSetting.lngM, setting->freeTimeModeSetting.lngS);
        printf("纬度: %d°%u\'%u\"\n", setting->freeTimeModeSetting.latD, setting->freeTimeModeSetting.latM, setting->freeTimeModeSetting.latS);
    } else
    {
        printf("固定时间模式\n");

        printf("开始时间: %u:%u\n", setting->fixedTimeModeSetting.startH, setting->fixedTimeModeSetting.startM);
        printf("结束时间: %u:%u\n", setting->fixedTimeModeSetting.endH, setting->fixedTimeModeSetting.endM);
    }

    printf("是否更改Windows模式: ");
    if (setting->isChangeWindowsMode) printf("是\n");
    else printf("否\n");

    printf("是否更改应用模式: ");
    if (setting->isChangeAppMode) printf("是\n");
    else printf("否\n");

    printf("是否更改壁纸: ");
    if (setting->isChangeWallpaper) printf("是\n");
    else printf("否\n");
}

int OInfo(enum INFOMODE infoMode, char *info)
{
    // 设置编码格式
    SetConsoleOutputCP(65001);

    HANDLE hStdO = GetStdHandle(STD_OUTPUT_HANDLE); // 标准输出句柄
    
    switch (infoMode)
    {
        case ERROR:
            SetConsoleTextAttribute(hStdO, FOREGROUND_RED);
            printf("错误: %s\n", info);
            SetConsoleTextAttribute(hStdO, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case WARNING:
            SetConsoleTextAttribute(hStdO, FOREGROUND_RED | FOREGROUND_GREEN);
            printf("警告: %s\n", info);
            SetConsoleTextAttribute(hStdO, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case SUCCESS:
            SetConsoleTextAttribute(hStdO, FOREGROUND_GREEN);
            printf("成功: %s\n", info);
            SetConsoleTextAttribute(hStdO, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case ASK:
            SetConsoleTextAttribute(hStdO, FOREGROUND_BLUE | FOREGROUND_GREEN);
            printf("询问: %s? [Y/N]\n", info);
            SetConsoleTextAttribute(hStdO, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            char ans = '\0'; // 回答字符

            fflush(stdin);
            scanf("%c", &ans);
            if (ans == 'Y' || ans == 'y' || ans == '1') return 1;
            else if (ans == 'N' || ans == 'n' || ans == '0') return 0;
            else
            {
                OInfo(ERROR, "输入格式错误");
                return -1;
            }
        case CONFIG:
            SetConsoleTextAttribute(hStdO, FOREGROUND_BLUE | FOREGROUND_GREEN);
            printf("配置: %s\n", info);
            SetConsoleTextAttribute(hStdO, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
    }

    return 1;
}
