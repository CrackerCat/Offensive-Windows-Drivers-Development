#include <ntddk.h>
#include <wdf.h>
#include "Device.h"
#include "Queue.h"

NTSTATUS
CryptoDeviceCreate(
    _Inout_ PWDFDEVICE_INIT DeviceInit
) {
    WDFDEVICE device;
    WDF_OBJECT_ATTRIBUTES attributes;
    NTSTATUS status;

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);

    status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Failed to create device: 0x%08X\n", status));
        return status;
    }

    status = CryptoQueueInitialize(device); // Função definida em Queue.c
    if (!NT_SUCCESS(status)) {
        KdPrint(("Failed to initialize queue: 0x%08X\n", status));
    }

    return status;
}
