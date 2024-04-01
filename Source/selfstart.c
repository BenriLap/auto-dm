/*
    selfstart.c
    自启动
*/

// 引入头文件
#include "../Include/selfstart.h"
#include "../Include/output.h"

// 函数定义

int InvokeSelfStart(enum INVOKEMODE invokeMode)
{
    HKEY hKey = {0};

    // 打开键
    if (RegOpenKeyEx(ROOT_KEY, RUN_SUB_KEY, 0, KEY_ALL_ACCESS, &hKey))
    {
        OInfo(ERROR, "打开键失败");
        return -1;
    }

    int result = 0; // 返回值

    // 获取应用程序路径
    char appPath[MAX_PATH] = {0};
    _getcwd(appPath, MAX_PATH);
    strcat(appPath, "\\AutoDM.exe");

    switch (invokeMode)
    {
        case ISEXIST:
            {
                char data[PATH_MAX] = {0};
                DWORD size = sizeof(data);

                if (RegQueryValueEx(hKey,RUN_VALUE,NULL,NULL,(LPBYTE)&data, &size)) goto close;
                else
                {
                    result = 1;
                    goto close;
                }
            }
        break;
        case CREATE:
            if (RegSetValueEx(hKey, RUN_VALUE, 0, REG_SZ, (LPBYTE)appPath, MAX_PATH)) goto close;
            else
            {
                result = 1;
                goto close;
            }
        break;
        case DELETE:
            if (!RegDeleteValue(hKey, RUN_VALUE)) result = 1;
            if (!RegOpenKeyEx(ROOT_KEY, RUN_NOTI_SUB_KEY, 0, KEY_ALL_ACCESS, &hKey)) RegDeleteValue(hKey, RUN_NOTI_VALUE);
            goto close;
        break;
    }

    close:
    // 关闭键
    RegCloseKey(hKey);

    return result;
}
