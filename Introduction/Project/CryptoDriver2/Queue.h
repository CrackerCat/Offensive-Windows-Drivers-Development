#pragma once

#include <ntddk.h>
#include <wdf.h>

NTSTATUS
CryptoQueueInitialize(
    _In_ WDFDEVICE Device
);
