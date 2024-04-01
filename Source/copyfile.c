/*
    copyfile.c
    复制文件
*/

// 引入头文件
#include "../Include/copyfile.h"
#include "../Include/output.h"

// 函数定义

int CopyFile(char *origPath, char *tgtPath)
{
    // 检查原始文件可读性
    if (access(origPath, R_OK))
    {
        OInfo(ERROR, "原始文件不可读");
        return 0;
    }

    int result = 1; // 函数返回值
    FILE *origFile = NULL, *tgtFile = NULL; // 文件指针

    // 打开文件
    if (!(origFile = fopen(origPath, "rb")) || !(tgtFile = fopen(tgtPath, "wb")))
    {
        OInfo(ERROR, "打开文件失败");
        result = 0;
        goto close;
    }

    // 复制文件
    void *buffer = (void *)malloc(COPY_BUFFER_SIZE); // 复制过程缓冲区
    int readCount; // 读取文件字节数
    while ((readCount = fread(buffer, 1, sizeof(*buffer), origFile)))
    {
        fwrite(buffer, readCount, 1, tgtFile);
    }
    free(buffer);

    close:
    {
        // 关闭文件
        int isError = 0; // 关闭文件是否错误
        if (origFile && fclose(origFile)) isError = 1;
        if (tgtFile && fclose(tgtFile)) isError = 1;
        if (isError)
        {
            OInfo(ERROR, "关闭文件失败");
            result = 0;
        }
    }

    return result;
}
