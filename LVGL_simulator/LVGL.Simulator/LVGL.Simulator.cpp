#include <Windows.h>
#include "resource.h"

#if _MSC_VER >= 1200
 // Disable compilation warnings.
#pragma warning(push)
// nonstandard extension used : bit field types other than int
#pragma warning(disable:4214)
// 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
#endif


//#include "lvgl/examples/lv_examples.h"
#include "lv_drivers/win32drv/win32drv.h"
#include "lv_lib_freetype/lv_freetype.h"
#include "gui.h"

#include <stdio.h>

#if _MSC_VER >= 1200
// Restore compilation warnings.
#pragma warning(pop)
#endif

lv_fs_res_t lv_win32_filesystem_driver_error_from_win32(
    DWORD Error)
{
    lv_fs_res_t res;

    switch (Error)
    {
    case ERROR_SUCCESS:
        res = LV_FS_RES_OK;
        break;
    case ERROR_BAD_UNIT:
    case ERROR_NOT_READY:
    case ERROR_CRC:
    case ERROR_SEEK:
    case ERROR_NOT_DOS_DISK:
    case ERROR_WRITE_FAULT:
    case ERROR_READ_FAULT:
    case ERROR_GEN_FAILURE:
    case ERROR_WRONG_DISK:
        res = LV_FS_RES_HW_ERR;
        break;
    case ERROR_INVALID_HANDLE:
    case ERROR_INVALID_TARGET_HANDLE:
        res = LV_FS_RES_FS_ERR;
        break;
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_DRIVE:
    case ERROR_NO_MORE_FILES:
    case ERROR_SECTOR_NOT_FOUND:
    case ERROR_BAD_NETPATH:
    case ERROR_BAD_NET_NAME:
    case ERROR_BAD_PATHNAME:
    case ERROR_FILENAME_EXCED_RANGE:
        res = LV_FS_RES_NOT_EX;
        break;
    case ERROR_DISK_FULL:
        res = LV_FS_RES_FULL;
        break;
    case ERROR_SHARING_VIOLATION:
    case ERROR_LOCK_VIOLATION:
    case ERROR_DRIVE_LOCKED:
        res = LV_FS_RES_LOCKED;
        break;
    case ERROR_ACCESS_DENIED:
    case ERROR_CURRENT_DIRECTORY:
    case ERROR_WRITE_PROTECT:
    case ERROR_NETWORK_ACCESS_DENIED:
    case ERROR_CANNOT_MAKE:
    case ERROR_FAIL_I24:
    case ERROR_SEEK_ON_DEVICE:
    case ERROR_NOT_LOCKED:
    case ERROR_LOCK_FAILED:
        res = LV_FS_RES_DENIED;
        break;
    case ERROR_BUSY:
        res = LV_FS_RES_BUSY;
        break;
    case ERROR_TIMEOUT:
        res = LV_FS_RES_TOUT;
        break;
    case ERROR_NOT_SAME_DEVICE:
    case ERROR_DIRECT_ACCESS_HANDLE:
        res = LV_FS_RES_NOT_IMP;
        break;
    case ERROR_TOO_MANY_OPEN_FILES:
    case ERROR_ARENA_TRASHED:
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_INVALID_BLOCK:
    case ERROR_OUT_OF_PAPER:
    case ERROR_SHARING_BUFFER_EXCEEDED:
    case ERROR_NOT_ENOUGH_QUOTA:
        res = LV_FS_RES_OUT_OF_MEM;
        break;
    case ERROR_INVALID_FUNCTION:
    case ERROR_INVALID_ACCESS:
    case ERROR_INVALID_DATA:
    case ERROR_BAD_COMMAND:
    case ERROR_BAD_LENGTH:
    case ERROR_INVALID_PARAMETER:
    case ERROR_NEGATIVE_SEEK:
        res = LV_FS_RES_INV_PARAM;
        break;
    default:
        res = LV_FS_RES_UNKNOWN;
        break;
    }

    return res;
}

int main()
{
    lv_init();

    if (!lv_win32_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        480,
        320,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))
    {
        return -1;
    }

    lv_win32_add_all_input_devices_to_group(NULL);

    reset_app();

    while (!lv_win32_quit_signal)
    {
        lv_task_handler();
        Sleep(1);
    }

    return 0;
}
