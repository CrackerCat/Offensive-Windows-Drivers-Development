#include <ntddk.h>
#include <bcrypt.h>
#include "Crypto.h"
#include <windef.h>

#define AES_KEY_SIZE 32
#define AES_BLOCK_SIZE 16


UCHAR AESKey[AES_KEY_SIZE] = "teste"; // Chave de exemplo

NTSTATUS CryptoEncryptFile(
    _In_ PUNICODE_STRING FilePath
) {
    KdPrint(("Iniciando criptografia para: %wZ\n", FilePath));

    // Abrir o arquivo
    HANDLE fileHandle;
    OBJECT_ATTRIBUTES objAttrs;
    IO_STATUS_BLOCK ioStatus;
    InitializeObjectAttributes(&objAttrs, FilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    NTSTATUS status = ZwCreateFile(&fileHandle, GENERIC_READ | GENERIC_WRITE, &objAttrs, &ioStatus, NULL,
        FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

    if (!NT_SUCCESS(status)) {
        KdPrint(("Falha ao abrir o arquivo: 0x%08X\n", status));
        return status;
    }

    // Ler conteúdo do arquivo
    BYTE buffer[1024] = { 0 };
    ULONG bytesRead = 0;

    status = ZwReadFile(fileHandle, NULL, NULL, NULL, &ioStatus, buffer, sizeof(buffer), NULL, NULL);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Falha ao ler o arquivo: 0x%08X\n", status));
        ZwClose(fileHandle);
        return status;
    }

    bytesRead = (ULONG)ioStatus.Information;

    // Criptografar conteúdo
    BYTE encryptedBuffer[1024] = { 0 };
    status = CryptoEncryptBuffer(buffer, bytesRead, encryptedBuffer, sizeof(encryptedBuffer));
    if (!NT_SUCCESS(status)) {
        ZwClose(fileHandle);
        return status;
    }

    // Escrever conteúdo criptografado de volta
    LARGE_INTEGER offset = { 0 };
    ZwSetInformationFile(fileHandle, &ioStatus, &offset, sizeof(offset), FilePositionInformation);

    status = ZwWriteFile(fileHandle, NULL, NULL, NULL, &ioStatus, encryptedBuffer, bytesRead, NULL, NULL);

    ZwClose(fileHandle);

    return status;
}

NTSTATUS CryptoEncryptBuffer(
    _In_ const BYTE* InputBuffer,
    _In_ ULONG InputSize,
    _Out_ BYTE* OutputBuffer,
    _In_ ULONG OutputSize
) {
    BCRYPT_ALG_HANDLE hAesAlg;
    BCRYPT_KEY_HANDLE hKey;
    NTSTATUS status;
    ULONG resultSize = 0;

    // Inicializa o algoritmo AES
    status = BCryptOpenAlgorithmProvider(&hAesAlg, BCRYPT_AES_ALGORITHM, NULL, 0);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Falha ao abrir AES: 0x%08X\n", status));
        return status;
    }

    status = BCryptSetProperty(hAesAlg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
    if (!NT_SUCCESS(status)) {
        BCryptCloseAlgorithmProvider(hAesAlg, 0);
        return status;
    }

    // Gera a chave
    status = BCryptGenerateSymmetricKey(hAesAlg, &hKey, NULL, 0, AESKey, AES_KEY_SIZE, 0);
    if (!NT_SUCCESS(status)) {
        BCryptCloseAlgorithmProvider(hAesAlg, 0);
        return status;
    }

    // Criptografa o buffer
    status = BCryptEncrypt(hKey, (PUCHAR)InputBuffer, InputSize, NULL, NULL, 0, OutputBuffer, OutputSize, &resultSize, 0);

    BCryptDestroyKey(hKey);
    BCryptCloseAlgorithmProvider(hAesAlg, 0);

    return status;
}
