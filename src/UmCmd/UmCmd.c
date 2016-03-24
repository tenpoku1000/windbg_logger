
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include <windows.h>
#include <stdio.h>
#include "Public.h"
#include "utils.h"

static BOOL call_kmdf_driver(void);

int main()
{
    BOOL ret_val = call_kmdf_driver();

    printf("\n");
    printf("Exit by pressing enter key.\n");

    getchar();

    return ret_val ? EXIT_SUCCESS : EXIT_FAILURE;
}

static BOOL call_kmdf_driver(void)
{
    HANDLE handle = CreateFile(
        KMDF_DOS_NAME,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (INVALID_HANDLE_VALUE == handle){

        get_last_error(__func__, __LINE__);

        return FALSE;
    }

    KMDF_VALUE kmdf_value = { .size = sizeof(kmdf_value), .value = 1 };
    DWORD ret_val = 0;

    if (FALSE == DeviceIoControl(handle, KMDF_IOCTL_SET_VALUE,
        &kmdf_value, sizeof(KMDF_VALUE),
        NULL, 0, &ret_val, NULL)){

        get_last_error(__func__, __LINE__);

        printf("DeviceIoControl() failed. return value = %d.\n", ret_val);

        return FALSE;
    }

    printf("DeviceIoControl() successful. return value = %d.\n", ret_val);

    return CloseHandle(handle);
}

