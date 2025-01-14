#include <ntddk.h>
#include "FileSystem.h"

// Prototypes
DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD DriverUnload;

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
) {
    UNREFERENCED_PARAMETER(RegistryPath);

    KdPrint(("Driver de criptografia inicializado.\n"));

    // Configuração das funções principais
    DriverObject->DriverUnload = DriverUnload;

    // Inicializar interceptação
    NTSTATUS status = FsInitialize(DriverObject);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Falha na inicialização do sistema de arquivos: 0x%08X\n", status));
        return status;
    }

    return STATUS_SUCCESS;
}

VOID DriverUnload(
    _In_ PDRIVER_OBJECT DriverObject
) {
    UNREFERENCED_PARAMETER(DriverObject);

    FsUnload();
    KdPrint(("Driver descarregado.\n"));
}
