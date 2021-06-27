#include <windows.h>
#include <iostream>

std::string WChar2Ansi(LPCWSTR pwszSrc)
{
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

    if (nLen<= 0) return std::string("");

    char* pszDst = new char[nLen];
    if (NULL == pszDst) return std::string("");

    WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
    pszDst[nLen -1] = 0;

    std::string strTemp(pszDst);
    delete [] pszDst;

    return strTemp;
}

int main()
{
    HANDLE hDirectory = CreateFile("D://Mine",
                                   GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY ,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                   NULL,
                                   OPEN_EXISTING,
                                   FILE_FLAG_BACKUP_SEMANTICS  | FILE_FLAG_OVERLAPPED,
                                   NULL);

    if(hDirectory == INVALID_HANDLE_VALUE){
        DWORD dwErr = GetLastError();
        return -1;
    }

    char notify[1024] = {0};
    FILE_NOTIFY_INFORMATION *pNotification = (FILE_NOTIFY_INFORMATION *)notify;
    DWORD BytesReturned = 0;

    DWORD filters = 0;
    filters = FILE_NOTIFY_CHANGE_FILE_NAME
            | FILE_NOTIFY_CHANGE_DIR_NAME
            | FILE_NOTIFY_CHANGE_FILE_NAME
            | FILE_NOTIFY_CHANGE_DIR_NAME
            | FILE_NOTIFY_CHANGE_LAST_WRITE
            | FILE_NOTIFY_CHANGE_LAST_ACCESS
            | FILE_NOTIFY_CHANGE_SIZE
            | FILE_NOTIFY_CHANGE_ATTRIBUTES
            | FILE_NOTIFY_CHANGE_LAST_WRITE
            | FILE_NOTIFY_CHANGE_LAST_ACCESS
            | FILE_NOTIFY_CHANGE_CREATION
            | FILE_NOTIFY_CHANGE_SECURITY;

    while (1)
    {
        ZeroMemory(pNotification, sizeof(notify));
        BOOL watch_state = ReadDirectoryChangesW(hDirectory,
                                                 &notify,
                                                 sizeof(notify),
                                                 TRUE,   //监控子目录
                                                 filters , //FILE_NOTIFY_CHANGE_DIR_NAME FILE_NOTIFY_CHANGE_CREATION FILE_NOTIFY_CHANGE_SIZE
                                                 (LPDWORD)&BytesReturned,
                                                 NULL,
                                                 NULL);

        if (GetLastError() == ERROR_INVALID_FUNCTION){
            // 文件监控，系统不支持!
            std::cout << "File monitoring, the system does not support" << std::endl;
            break;
        }else if(watch_state == FALSE){
            DWORD dwErr = GetLastError();
            // 文件监控，监控失败!
            std::cout << "File monitoring, monitoring failed" << std::endl;
            break;
        }else if (GetLastError() == ERROR_NOTIFY_ENUM_DIR){
            // 文件监控，内存溢出!
            std::cout << "File monitoring, memory overflow!" << std::endl;
            continue;
        }else{
            //这里主要就是检测返回的信息，(FILE_NOTIFY_INFORMATION)
            std::string szFileName = WChar2Ansi(pNotification->FileName);

            switch (pNotification->Action) {
            case FILE_ACTION_ADDED:
                // 文件监控，新增文件
                std::cout << "File monitoring, new files: " << szFileName << std::endl;
                break;
            case FILE_ACTION_REMOVED:
                // 文件监控，删除文件
                std::cout << "File monitoring, delete files: " << szFileName << std::endl;
                break;
            case FILE_ACTION_MODIFIED:
                // 文件监控，修改文件
                std::cout << "File monitoring, modify files: " << szFileName << std::endl;
                break;
            case FILE_ACTION_RENAMED_OLD_NAME:{
                // 文件监控，重命名文件 旧
                std::cout << "File monitoring, rename files old: " << szFileName << std::endl;
                // 文件监控，重命名文件 新
                std::string str = WChar2Ansi(pNotification->FileName + pNotification->NextEntryOffset / sizeof(wchar_t));
                std::cout << "File monitoring, rename files New: " << str << std::endl;
            } break;
            case FILE_ACTION_RENAMED_NEW_NAME: //还没出现过这种情况
                // 文件监控，重命名文件2
                std::cout << "File monitoring, rename file 2: " << szFileName << std::endl;
                break;
            }
        }
    }
    CloseHandle(hDirectory);

    return 0;
}
