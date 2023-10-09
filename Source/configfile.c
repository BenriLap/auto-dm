/*
    configfile.c
    读写、新建配置文件
*/

// 引入头文件
#include "../Include/configfile.h"
#include "../Include/output.h"
#include "../Include/copyfile.h"

// 函数定义

int ROWConfigFile(enum ROWCONFIGFILEMODE configFileMode, struct Setting *setting, char *configFilePath)
{
    int result = 1; // 返回值
    FILE *configFile; // 配置文件指针

    if (!configFileMode)
    {
        // 检查原始文件可读性
        if (access(configFilePath, R_OK))
        {
            OInfo(ERROR, "配置文件不可读");
            return 0;
        }

        // 打开配置文件
        if (!(configFile = fopen(configFilePath, "rb")))
        {
            OInfo(ERROR, "打开配置文件失败");
            result = 0;
            goto close;
        }

        // 检查配置文件格式
        unsigned char tag[4] = "\0";
        fread(tag, 4, 1, configFile);
        if (tag[0] != 'B' || tag[1] != 'L' || tag[2] != 'A' || tag[3] != 'D')
        {
            OInfo(ERROR, "配置文件格式错误");
            result = 0;
            goto close;
        }

        // 检查配置文件版本
        unsigned char ver = 0;
        fread(&ver, 1, 1, configFile);
        if (ver > THE_CONFIG_FILE_VER) OInfo(WARNING, "配置文件版本过高");
        else if (ver < THE_CONFIG_FILE_VER) OInfo(WARNING, "配置文件版本过低");

        // 读取配置文件
        fread(&setting->selectedMode, 1, 1, configFile);
        fseek(configFile, 2, SEEK_CUR);

        while (!feof(configFile))
        {
            // 读取分区信息
            unsigned char head = 0; // 分区头
            fread(&head, 1, 1, configFile);
            unsigned char tag = head >> 4; // 分区标签
            unsigned char len = head & 0b1111; // 分区剩余长度

            switch (tag >> 2)
            {
                case 0b01:
                    switch (tag & 0b11)
                    {
                        case 0b00:
                            {
                                unsigned char fixedTime[3] = {0};
                                fread(&fixedTime, 3, 1, configFile);
                                setting->fixedTimeModeSetting.startH = fixedTime[0] >> 2;
                                setting->fixedTimeModeSetting.endH = (fixedTime[0] & 0b11) << 4 | fixedTime[1] >> 4;
                                setting->fixedTimeModeSetting.startM = (fixedTime[1] & 0b1111) << 2 | fixedTime[2] >> 6;
                                setting->fixedTimeModeSetting.endM = fixedTime[2] & 0b111111;
                            }
                        break;
                    }
                break;
                case 0b10:
                    switch (tag & 0b11)
                    {
                        case 0b00:
                            {
                                fread(&setting->freeTimeModeSetting.lngDir, 1, 1, configFile);
                                fread(&setting->freeTimeModeSetting.lngD, 1, 1, configFile);
                                fread(&setting->freeTimeModeSetting.latD, 1, 1, configFile);
                                
                                unsigned char lngLatMS[3] = {0};
                                fread(&lngLatMS, 3, 1, configFile);
                                setting->freeTimeModeSetting.lngM = lngLatMS[0] >> 2;
                                setting->freeTimeModeSetting.lngS = (lngLatMS[0] & 0b11) << 4 | lngLatMS[1] >> 4;
                                setting->freeTimeModeSetting.latM = (lngLatMS[1] & 0b1111) << 2 | lngLatMS[2] >> 6;
                                setting->freeTimeModeSetting.latS = lngLatMS[2] & 0b111111;

                                fseek(configFile, 1, SEEK_CUR);
                            }
                        break;
                    }
                break;
                case 0b11:
                    switch (tag & 0b11)
                    {
                        case 0b00:
                            {
                                unsigned char changeMode = 0;
                                fread(&changeMode, 1, 1, configFile);
                                setting->isChangeWindowsMode = changeMode >> 4;
                                setting->isChangeAppMode = changeMode & 0b1111;

                                fread(&setting->isChangeWallpaper, 1, 1, configFile);
                                fseek(configFile, 1, SEEK_CUR);
                            }
                        break;
                    }
                break;

                default:
                    if (!len) break;
                    fseek(configFile, len, SEEK_CUR);
                break;
            }
        }
    } else if (configFileMode)
    {
        // 打开配置文件
        if (!(configFile = fopen(configFilePath, "wb")))
        {
            OInfo(ERROR, "打开配置文件失败");
            result = 0;
            goto close;
        }

        {
            unsigned char tag[4] = "BLAD";
            fwrite(tag, 4, 1, configFile);

            unsigned char ver = THE_CONFIG_FILE_VER;
            fwrite(&ver, 1, 1, configFile);

            fwrite(&setting->selectedMode, 1, 1, configFile);

            unsigned char null = 0;
            fwrite(&null, 2, 1, configFile);
        }

        if (setting->selectedMode)
        {
            unsigned char head = 0;
            unsigned char tag = 0b1000;
            unsigned char len = 7;
            head = tag << 4;
            head |= len;
            fwrite(&head, 1, 1, configFile);

            fwrite(&setting->freeTimeModeSetting.lngDir, 1, 1, configFile);
            fwrite(&setting->freeTimeModeSetting.lngD, 1, 1, configFile);
            fwrite(&setting->freeTimeModeSetting.latD, 1, 1, configFile);

            unsigned char lngLatMS[3] = {0};
            lngLatMS[0] = (setting->freeTimeModeSetting.lngM << 2) | (setting->freeTimeModeSetting.lngS >> 4);
            lngLatMS[1] = (setting->freeTimeModeSetting.lngS << 4) | (setting->freeTimeModeSetting.latM >> 2);
            lngLatMS[2] = (setting->freeTimeModeSetting.latM << 6) | (setting->freeTimeModeSetting.latS);
            fwrite(&lngLatMS, 3, 1, configFile);

            unsigned char null = 0;
            fwrite(&null, 1, 1, configFile);
        } else
        {
            unsigned char head = 0;
            unsigned char tag = 0b0100;
            unsigned char len = 3;
            head = tag << 4;
            head |= len;
            fwrite(&head, 1, 1, configFile);

            unsigned char fixedTime[3] = {0};
            fixedTime[0] = (setting->fixedTimeModeSetting.startH << 2) | (setting->fixedTimeModeSetting.endH >> 4);
            fixedTime[1] = (setting->fixedTimeModeSetting.endH << 4) | (setting->fixedTimeModeSetting.startM >> 2);
            fixedTime[2] = (setting->fixedTimeModeSetting.startM << 6) | (setting->fixedTimeModeSetting.endM);
            fwrite(&fixedTime, 3, 1, configFile);
        }

        {
            unsigned char head = 0;
            unsigned char tag = 0b1100;
            unsigned char len = 3;
            head = tag << 4;
            head |= len;
            fwrite(&head, 1, 1, configFile);

            unsigned char changeMode = 0;
            changeMode = setting->isChangeWindowsMode << 4;
            changeMode |= setting->isChangeAppMode;
            fwrite(&changeMode, 1, 1, configFile);

            fwrite(&setting->isChangeWallpaper, 1, 1, configFile);

            unsigned char null = 0;
            fwrite(&null, 1, 1, configFile);
        }
    }

    close:
    // 关闭配置文件
    if (configFile && fclose(configFile))
    {
        OInfo(ERROR, "关闭文件失败");
        result = 0;
    }

    return result;
}

int CreateConfigFile(enum CREATECONFIGFILEMODE createConfigMode)
{
    struct Setting setting = {0};

    if (!createConfigMode && ROWConfigFile(READ, &setting, CONFIG_FILE_PATH))
    {
        OSettingInfo(&setting);
        if (!OInfo(ASK, "是否要重新设置")) return 0;
    }

    // 新建设置项
    OInfo(WARNING, "新建配置文件");

    OInfo(CONFIG, "选择的时间模式: 固定时间模式(0) 自动变更时间模式(1)");
    int selectedMode = -1;
    fflush(stdin);
    scanf("%d", &selectedMode);

    if (selectedMode != 0 && selectedMode != 1)
    {
        OInfo(ERROR, "输入格式错误");
        return 0;
    }
    setting.selectedMode = (unsigned char)selectedMode;

    if (setting.selectedMode)
    {
        OInfo(CONFIG, "输入经度(度 分 秒)");
        int lngD = -180, lngM = -1, lngS = -1;
        fflush(stdin);
        scanf("%d %d %d", &lngD, &lngM, &lngS);

        if (lngD > 180 || lngD < -179)
        {
            OInfo(ERROR, "输入格式错误");
            return 0;
        }
        if (lngM > 59 || lngM < 0 || lngS > 59 || lngS < 0)
        {
            OInfo(ERROR, "输入格式错误");
            return 0;
        }
        if (lngD == 180 && (lngM || lngS))
        {
            OInfo(ERROR, "输入格式错误");
            return 0;
        }

        if (lngD > 0) setting.freeTimeModeSetting.lngDir = 1;
        else if (lngD < 0) setting.freeTimeModeSetting.lngDir = -1;
        else setting.freeTimeModeSetting.lngDir = 0;
        if (!lngD) setting.freeTimeModeSetting.lngD = 0;
        else setting.freeTimeModeSetting.lngD = (unsigned char)(lngD / setting.freeTimeModeSetting.lngDir);

        setting.freeTimeModeSetting.lngM = (unsigned char)lngM;
        setting.freeTimeModeSetting.lngS = (unsigned char)lngS;

        OInfo(CONFIG, "输入纬度(度 分 秒)");
        int latD = -91, latM = -1, latS = -1;
        fflush(stdin);
        scanf("%d %d %d", &latD, &latM, &latS);

        if (latD > 90 || latD < -90 || latM > 59 || latM < 0 || latS > 59 || latS < 0)
        {
            OInfo(ERROR, "输入格式错误");
            return 0;
        }
        if ((latD == 90 || latD == -90) && (latM || latS))
        {
            OInfo(ERROR, "输入格式错误");
            return 0;
        }
        setting.freeTimeModeSetting.latD = (unsigned char)latD;
        setting.freeTimeModeSetting.latM = (unsigned char)latM;
        setting.freeTimeModeSetting.latS = (unsigned char)latS;
    } else
    {
        OInfo(CONFIG, "输入开始时间(时 分)");
        int startH = -1, startM = -1;
        fflush(stdin);
        scanf("%d %d", &startH, &startM);

        if (startH < 0 || startH > 23 || startM < 0 || startM > 59)
        {
            OInfo(ERROR, "输入格式错误");
            return 0;
        }
        setting.fixedTimeModeSetting.startH = (unsigned char)startH;
        setting.fixedTimeModeSetting.startM = (unsigned char)startM;

        OInfo(CONFIG, "输入结束时间(时 分)");
        int endH = -1, endM = -1;
        fflush(stdin);
        scanf("%d %d", &endH, &endM);
        if (endH < 0 || endH > 23 || endM < 0 || endM > 59)
        {
            OInfo(ERROR, "输入格式错误");
            return 0;
        }
        setting.fixedTimeModeSetting.endH = (unsigned char)endH;
        setting.fixedTimeModeSetting.endM = (unsigned char)endM;
    }

    int isChangeWindowsMode = -1;
    isChangeWindowsMode = OInfo(ASK, "是否更改Windows模式");
    if (isChangeWindowsMode == -1) return 0;
    setting.isChangeWindowsMode = (unsigned char)isChangeWindowsMode;

    int isChangeAppMode = -1;
    isChangeAppMode = OInfo(ASK, "是否更改应用模式");
    if (isChangeAppMode == -1) return 0;
    setting.isChangeAppMode = (unsigned char)isChangeAppMode;

    int isChangeWallpaper = -1;
    isChangeWallpaper = OInfo(ASK, "是否更改壁纸");
    if (isChangeWallpaper == -1) return 0;
    setting.isChangeWallpaper = (unsigned char)isChangeWallpaper;

    if (setting.isChangeWallpaper == 1)
    {
        char path[PATH_MAX] = {0};
        OInfo(CONFIG, "输入深色模式壁纸路径");
        fflush(stdin);
        scanf("%s", &path);
        if (!CopyFile(path, DK_WALLPAPER_PATH)) return 0;
        OInfo(CONFIG, "输入浅色模式壁纸路径");
        fflush(stdin);
        scanf("%s", &path);
        if (!CopyFile(path, LT_WALLPAPER_PATH)) return 0;
    }

    // 写入配置文件
    if (ROWConfigFile(WRITE, &setting, CONFIG_FILE_PATH)) return 1;

    return 0;
}
