#include <ntddk.h>
#include <wdf.h>
#include "Queue.h"
#include "Crypto.h"

NTSTATUS
CryptoQueueInitialize(
    _In_ WDFDEVICE Device
) {
    WDF_IO_QUEUE_CONFIG ioQueueConfig;
    WDFQUEUE queue;
    NTSTATUS status;

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig, WdfIoQueueDispatchSequential);
    ioQueueConfig.EvtIoWrite = CryptoEvtIoWrite;

    status = WdfIoQueueCreate(Device, &ioQueueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Failed to create I/O queue: 0x%08X\n", status));
    }

    return status;
}

VOID
CryptoEvtIoWrite(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
) {
    NTSTATUS status;
    WDFMEMORY memory;
    BYTE* buffer;

    UNREFERENCED_PARAMETER(Queue);

    if (Length == 0) {
        WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
        return;
    }

    status = WdfRequestRetrieveInputMemory(Request, &memory);
    if (!NT_SUCCESS(status)) {
        WdfRequestComplete(Request, status);
        return;
    }

    buffer = (BYTE*)WdfMemoryGetBuffer(memory, NULL);

    // Exemplo de criptografia simples (reversão de bytes)
    for (size_t i = 0; i < Length; i++) {
        buffer[i] ^= 0xAA; // Operação XOR como exemplo de criptografia.
    }

    WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, Length);
}
