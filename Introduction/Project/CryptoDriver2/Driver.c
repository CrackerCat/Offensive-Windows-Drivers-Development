#include <ntddk.h>
#include <wdf.h>
#include "Device.h" 

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD CryptoDriverEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP CryptoDriverEvtDriverContextCleanup;

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
) {
    WDF_DRIVER_CONFIG config;
    NTSTATUS status;

    WDF_DRIVER_CONFIG_INIT(&config, CryptoDriverEvtDeviceAdd);
    config.EvtDriverUnload = CryptoDriverEvtDriverContextCleanup;

    status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Failed to create driver: 0x%08X\n", status));
    }

    return status;
}

VOID
CryptoDriverEvtDriverContextCleanup(
    _In_ WDFOBJECT DriverObject
) {
    UNREFERENCED_PARAMETER(DriverObject);
    KdPrint(("Driver context cleanup\n"));
}

NTSTATUS
CryptoDriverEvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
) {
    UNREFERENCED_PARAMETER(Driver);
    return CryptoDeviceCreate(DeviceInit); // Função definida em Device.c
}
