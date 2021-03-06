
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.
//
// The original version of this source code was generated by automatically the Microsoft Visual Studio.

/*++

Module Name:

    device.c - Device handling events for example driver.

Abstract:

   This file contains the device entry points and callbacks.
    
Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "device.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, KmdfDriverCreateDevice)
#endif


NTSTATUS
KmdfDriverCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    WDF_OBJECT_ATTRIBUTES   deviceAttributes;
    PDEVICE_CONTEXT deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

    PAGED_CODE();

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);
// @@@
    DECLARE_CONST_UNICODE_STRING(nt_device_name, KMDF_DEVICE_NAME);
    DECLARE_CONST_UNICODE_STRING(symbolic_name, KMDF_SYMBOLIC_NAME);

    WdfDeviceInitSetDeviceType(DeviceInit, FILE_DEVICE_UNKNOWN);

    status = WdfDeviceInitAssignName(DeviceInit, &nt_device_name);

    if (!NT_SUCCESS(status)){

        return status;
    }

    status = WdfDeviceInitAssignSDDLString(DeviceInit, &SDDL_DEVOBJ_SYS_ALL_ADM_ALL);

    if (!NT_SUCCESS(status)){

        return status;
    }
// @@@
    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    if (NT_SUCCESS(status)) {
        //
        // Get a pointer to the device context structure that we just associated
        // with the device object. We define this structure in the device.h
        // header file. DeviceGetContext is an inline function generated by
        // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
        // This function will do the type checking and return the device context.
        // If you pass a wrong object handle it will return NULL and assert if
        // run under framework verifier mode.
        //
        deviceContext = DeviceGetContext(device);

        //
        // Initialize the context.
        //
        deviceContext->PrivateDeviceData = 0;

// @@@
        ////
        //// Create a device interface so that applications can find and talk
        //// to us.
        ////
        //status = WdfDeviceCreateDeviceInterface(
        //    device,
        //    &GUID_DEVINTERFACE_KmdfDriver,
        //    NULL // ReferenceString
        //    );
        status = WdfDeviceCreateSymbolicLink(device, &symbolic_name);
// @@@

        if (NT_SUCCESS(status)) {
            //
            // Initialize the I/O Package and any Queues
            //
            status = KmdfDriverQueueInitialize(device);
        }
    }

    return status;
}


