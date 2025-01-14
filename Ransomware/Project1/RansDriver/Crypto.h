#ifndef CRYPTO_H
#define CRYPTO_H

#include <ntddk.h>

// Função para criptografar um arquivo
NTSTATUS CryptoEncryptFile(
    _In_ PUNICODE_STRING FilePath
);

// Função para criptografar um buffer
NTSTATUS CryptoEncryptBuffer(
    _In_ const UCHAR* InputBuffer,    // Use `UCHAR` em vez de `BYTE`
    _In_ ULONG InputSize,
    _Out_ UCHAR* OutputBuffer,
    _In_ ULONG OutputSize
);

#endif // CRYPTO_H
