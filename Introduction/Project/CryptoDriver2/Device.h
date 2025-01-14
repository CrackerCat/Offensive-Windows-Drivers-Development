#pragma once

#include <ntddk.h>
#include <wdf.h>

NTSTATUS
CryptoDeviceCreate(
    _Inout_ PWDFDEVICE_INIT DeviceInit
);
