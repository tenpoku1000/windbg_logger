
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.
//
// The original version of this source code was generated by automatically the Microsoft Visual Studio.

/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

// @@@
//
////
//// Define an Interface Guid so that app can find the device and talk to it.
////
//
//DEFINE_GUID(GUID_DEVINTERFACE_KmdfDriver,
//    0x8f0757e3, 0x7d7f, 0x4e30, 0x8b, 0xc6, 0x70, 0x9c, 0xbc, 0xbb, 0xa7, 0x56);
//// {8f0757e3-7d7f-4e30-8bc6-709cbcbba756}

#define KMDF_DEVICE_NAME L"\\Device\\WdbgLog"
#define KMDF_SYMBOLIC_NAME L"\\DosDevices\\Global\\WdbgLog"
#define KMDF_DOS_NAME L"\\\\.\\WdbgLog"

#define KMDF_IOCTL_SET_VALUE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS| FILE_WRITE_ACCESS)

typedef struct KMDF_VALUE_{
    UINT64 size;
    UINT32 value;
}KMDF_VALUE;
// @@@
