#pragma once

#include <ntddk.h>
#include <wdf.h>

VOID
CryptoEvtIoWrite(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
);
