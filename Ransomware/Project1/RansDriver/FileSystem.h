#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <ntddk.h>

// Inicializa o sistema de arquivos
NTSTATUS FsInitialize(_In_ PDRIVER_OBJECT DriverObject);

// Finaliza o sistema de arquivos
VOID FsUnload();

// Intercepta arquivos específicos
NTSTATUS FsInterceptFile(_In_ PUNICODE_STRING FilePath);

// Renomeia arquivos para a extensão .joas
NTSTATUS RenameFileToJoas(_In_ PUNICODE_STRING FilePath);

// Funções de interceptação de leitura e escrita
NTSTATUS FsInterceptWrite(
    _In_ PDEVICE_OBJECT DeviceObject,
    _Inout_ PIRP Irp
);

NTSTATUS FsInterceptRead(
    _In_ PDEVICE_OBJECT DeviceObject,
    _Inout_ PIRP Irp
);

#endif // FILE_SYSTEM_H
