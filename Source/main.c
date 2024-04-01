/*
    main.c
    主程序
*/

// 引入头文件
#include "../Include/main.h"

#include "../Include/configfile.h"
#include "../Include/output.h"
#include "../Include/copyfile.h"
#include "../Include/selfstart.h"

// 函数定义

int main(int argc, char *argv[])
{
    // 获取配置文件的路径
    char configFilePath[PATH_MAX] = {0};
    strcpy(configFilePath, argv[0]);
    configFilePath[strlen(configFilePath) - 11] = '\0';
    strcat(configFilePath, "\\config\\config.bin");
    // 参数非完整路径
    if (access(configFilePath, F_OK)) strcpy(configFilePath, CONFIG_FILE_PATH); // 使用相对路径

    // 获取深色模式壁纸的路径
    char dkWallpaperPath[PATH_MAX] = {0};
    strcpy(dkWallpaperPath, argv[0]);
    dkWallpaperPath[strlen(dkWallpaperPath) - 11] = '\0';
    strcat(dkWallpaperPath, "\\wallpaper\\dk.bin");
    // 参数非完整路径
    if (access(dkWallpaperPath, F_OK)) strcpy(dkWallpaperPath, DK_WALLPAPER_PATH); // 使用相对路径

    // 获取浅色模式壁纸的路径
    char ltWallpaperPath[PATH_MAX] = {0};
    strcpy(ltWallpaperPath, argv[0]);
    ltWallpaperPath[strlen(ltWallpaperPath) - 11] = '\0';
    strcat(ltWallpaperPath, "\\wallpaper\\lt.bin");
    // 参数非完整路径
    if (access(ltWallpaperPath, F_OK)) strcpy(ltWallpaperPath, LT_WALLPAPER_PATH); // 使用相对路径

    if (argc == 1)
    {
        struct Setting setting = {0};

        // 获取活动窗口句柄
        HWND hWnd = NULL;
        if (!(hWnd = GetForegroundWindow())) OInfo(WARNING, "获得活动窗口句柄失败");

        // 检查配置文件可读性
        if (access(configFilePath, R_OK))
        {
            OInfo(ERROR, "配置文件不可读");
            if (CreateConfigFile(PASS)) ROWConfigFile(READ, &setting, configFilePath);
            else return 0;
        }
        ShowWindow(hWnd, SW_HIDE);
        
        while (1)
        {
            // 检查配置文件可读性
            if (access(configFilePath, R_OK))
            {
                ShowWindow(hWnd, SW_SHOW);
                OInfo(ERROR, "配置文件不可读");
                if (!CreateConfigFile(PASS))
                {
                    OInfo(ERROR, "create configfile error");
                    break;
                }
                ShowWindow(hWnd, SW_HIDE);
            } else ROWConfigFile(READ, &setting, configFilePath);

            HKEY hKey = {0};
            if (RegOpenKeyEx(ROOT_KEY, THEMES_SUB_KEY, 0, KEY_ALL_ACCESS, &hKey))
            {
                OInfo(ERROR, "打开键失败");
                break;
            }

            DWORD size = sizeof(DWORD);

            DWORD SystemUsesLightTheme = 0;
            if (RegQueryValueEx(hKey,WINDOWS_MODE_VALUE,NULL,NULL,(LPBYTE)&SystemUsesLightTheme, &size))
            {
                OInfo(ERROR, "获取值失败");
                break;
            }

            DWORD AppsUseLightTheme = 0;
            if (RegQueryValueEx(hKey,APP_MODE_VALUE,NULL,NULL,(LPBYTE)&AppsUseLightTheme, &size))
            {
                OInfo(ERROR, "获取值失败");
                break;
            }

            float startTime = 0.0; // 开始时间
            float endTime = 0.0; // 结束时间

            SYSTEMTIME localTime = {0}; // 本地时间
            GetLocalTime(&localTime);

            if (setting.selectedMode)
            {
                char februaryDay = 28; // 二月份天数
                if (!(localTime.wYear % 4) && localTime.wYear % 100 || !(localTime.wYear % 400)) februaryDay = 29;

                enum DIR dir = NORTH; // 太阳直射南北方向
                if (localTime.wMonth == 3 && localTime.wDay >= 21 || localTime.wMonth == 9 && localTime.wDay < 23 || localTime.wMonth > 3 && localTime.wMonth < 9) dir = NORTH;
                else dir = SOUTH;

                float sunAngle = 0; // 太阳直射角度
                switch (localTime.wMonth)
                {
                    case 1:
                        // sunAngle = (21 + februaryDay + 31 - localTime.wDay) * SUN_MOVE_PER_DAY;
                        sunAngle = (52 + februaryDay - localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 2:
                        sunAngle = (21 + februaryDay - localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 3:
                        if (localTime.wDay < 21) sunAngle = (21 - localTime.wDay) * SUN_MOVE_PER_DAY;
                        else sunAngle = (localTime.wDay - 21) * SUN_MOVE_PER_DAY;
                    break;
                    case 4:
                        sunAngle = (10 + localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 5:
                        // sunAngle = (10 + 30 + localTime.wDay) * SUN_MOVE_PER_DAY;
                        sunAngle = (40 + localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 6:
                        // if (localTime.wDay < 22) sunAngle = (10 + 30 + 31 + localTime.wDay) * SUN_MOVE_PER_DAY;
                        if (localTime.wDay < 22) sunAngle = (71 + localTime.wDay) * SUN_MOVE_PER_DAY;
                        // else sunAngle = (23 + 31 + 31 + 30 - localTime.wDay) * SUN_MOVE_PER_DAY;
                        else sunAngle = (115 - localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 7:
                        // sunAngle = (23 + 31 + 31 - localTime.wDay) * SUN_MOVE_PER_DAY;
                        sunAngle = (85 - localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 8:
                        // sunAngle = (23 + 31 - localTime.wDay) * SUN_MOVE_PER_DAY;
                        sunAngle = (54 - localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 9:
                        if (localTime.wDay < 23) sunAngle = (23 - localTime.wDay) * SUN_MOVE_PER_DAY;
                        else sunAngle = (localTime.wDay - 23) * SUN_MOVE_PER_DAY;
                    break;
                    case 10:
                        sunAngle = (7 + localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 11:
                        // sunAngle = (7 + 31 + localTime.wDay) * SUN_MOVE_PER_DAY;
                        sunAngle = (38 + localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                    case 12:
                        // if (localTime.wDay < 22) sunAngle = (7 + 31 + 30 + localTime.wDay) * SUN_MOVE_PER_DAY;
                        if (localTime.wDay < 22) sunAngle = (68 + localTime.wDay) * SUN_MOVE_PER_DAY;
                        // else sunAngle = (21 + februaryDay + 31 + 31 - localTime.wDay) * SUN_MOVE_PER_DAY;
                        else sunAngle = (83 + februaryDay - localTime.wDay) * SUN_MOVE_PER_DAY;
                    break;
                }

                float lng = setting.freeTimeModeSetting.lngDir * setting.freeTimeModeSetting.lngD + setting.freeTimeModeSetting.lngM / 60.0f + setting.freeTimeModeSetting.lngS / 3600.0f; // 经度
                float lat = setting.freeTimeModeSetting.latD + setting.freeTimeModeSetting.latM / 60.0f + setting.freeTimeModeSetting.latS / 3600.0f; // 纬度
                
                float SunTimeOfs = 6 * sin(lat * PER_RADIAN) * tan(sunAngle * PER_RADIAN) / cos(lat * PER_RADIAN); // 因太阳直射点变动导致的昼长时间差

                float UTCStartTime = 0.0f; // UTC开始时间
                float UTCEndTime = 0.0f; // UTC结束时间

                if (dir)
                {
                    UTCStartTime = 6 - SunTimeOfs - lng * 4 / 60;
                    UTCEndTime = 18 + SunTimeOfs - lng * 4 / 60;
                } else
                {
                    UTCStartTime = 6 + SunTimeOfs - lng * 4 / 60;
                    UTCEndTime = 18 - SunTimeOfs - lng * 4 / 60;
                }

                SYSTEMTIME systemTime = {0}; // UTC时间
                GetSystemTime(&systemTime);

                float timeOfs = (localTime.wHour + localTime.wMinute / 60.0f) - (systemTime.wHour + systemTime.wMinute / 60.0f); // 本地时间和UTC时间差

                startTime = UTCStartTime + timeOfs;
                endTime = UTCEndTime + timeOfs;
            } else
            {
                startTime = setting.fixedTimeModeSetting.startH + setting.fixedTimeModeSetting.startM / 60.0f;
                endTime = setting.fixedTimeModeSetting.endH + setting.fixedTimeModeSetting.endM / 60.0f;
            }

            float fLocalTime = localTime.wHour + localTime.wMinute / 60.0f; // 小数化本地时间

            int isChange = 0; // 是否更改注册表数据

            DWORD value0 = 0x0; // 注册表写入数据0
            DWORD value1 = 0x1; // 注册表写入数据1

            if (fLocalTime < startTime && fLocalTime > endTime)
            {
                if (setting.isChangeWindowsMode && !SystemUsesLightTheme)
                {
                    RegSetValueEx(hKey, WINDOWS_MODE_VALUE, 0, REG_DWORD, (LPBYTE)&value1, sizeof(value1));
                    isChange = 1;
                }
                if (setting.isChangeAppMode && !AppsUseLightTheme)
                {
                    RegSetValueEx(hKey, APP_MODE_VALUE, 0, REG_DWORD, (LPBYTE)&value1, sizeof(value1));
                    isChange = 1;
                }
                if (setting.isChangeWallpaper)
                {
                    if (access(ltWallpaperPath, F_OK)) OInfo(ERROR, "壁纸文件不存在");
                    else SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, _fullpath(NULL, ltWallpaperPath, PATH_MAX), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
                }
            } else
            {
                if (setting.isChangeWindowsMode && SystemUsesLightTheme)
                {
                    RegSetValueEx(hKey, WINDOWS_MODE_VALUE, 0, REG_DWORD, (LPBYTE)&value0, sizeof(value0));
                    isChange = 1;
                }
                if (setting.isChangeAppMode && AppsUseLightTheme)
                {
                    RegSetValueEx(hKey, APP_MODE_VALUE, 0, REG_DWORD, (LPBYTE)&value0, sizeof(value0));
                    isChange = 1;
                }
                if (setting.isChangeWallpaper)
                {
                    if (access(dkWallpaperPath, F_OK)) OInfo(ERROR, "壁纸文件不存在");
                    else SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, _fullpath(NULL, dkWallpaperPath, PATH_MAX), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
                }
            }

            if (isChange)
            {
                // 重启explorer.exe
                system("taskkill /f /im explorer.exe");
                ShellExecute(NULL, "open", "explorer.exe", NULL, NULL, SW_HIDE);
            }

            RegCloseKey(hKey); // 关闭注册表
            
            Sleep(10 * 1000); // 循环间隔
        }
    } else
    {
        if (!strcmp(argv[1], "version"))
        {
            if (argc == 2) OVerInfo();
            else OInfo(ERROR, "命令格式错误");
        } else if (!strcmp(argv[1], "help"))
        {
            if (argc == 2) OHelpInfo();
            else OInfo(ERROR, "命令格式错误");
        } else if (!strcmp(argv[1], "setting"))
        {
            if (argc == 2)
            {
                if (CreateConfigFile(CHECK)) OInfo(SUCCESS, "配置成功");
            } else if (argc == 3)
            {
                struct Setting setting = {0};
                if (ROWConfigFile(READ, &setting, argv[2]))
                {
                    OSettingInfo(&setting);
                    if (OInfo(ASK, "是否应用配置文件") && CopyFile(argv[2], CONFIG_FILE_PATH)) OInfo(SUCCESS, "配置成功");
                }
            } else OInfo(ERROR, "命令格式错误");
        } else if (!strcmp(argv[1], "selfstart"))
        {
            if (argc == 2)
            {
                if (InvokeSelfStart(ISEXIST))
                {
                    int ans = 0; // 回答数据
                    OInfo(CONFIG, "删除自启动注册表(0) 新建自启动注册表(1)?");
                    fflush(stdin);
                    scanf("%d", &ans);
                    if (ans == 0) InvokeSelfStart(DELETE);
                    else if (ans == 1) InvokeSelfStart(CREATE);
                    else OInfo(ERROR, "输入格式错误");
                } else if (!InvokeSelfStart(ISEXIST) && OInfo(ASK, "新建自启动注册表")) InvokeSelfStart(CREATE);
            } else OInfo(ERROR, "命令格式错误");
        } else OInfo(ERROR, "命令格式错误");
    }

    return 0;
}
