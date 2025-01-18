#include <ntddk.h>

#define IOCTL_VULNERABLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

UNICODE_STRING DEVICE_NAME = RTL_CONSTANT_STRING(L"\\Device\\VulnerableDriver");
UNICODE_STRING DEVICE_SYMBOLIC_NAME = RTL_CONSTANT_STRING(L"\\??\\VulnerableDriverLink");

void DriverUnload(PDRIVER_OBJECT DriverObject) {
    IoDeleteSymbolicLink(&DEVICE_SYMBOLIC_NAME);
    IoDeleteDevice(DriverObject->DeviceObject);
    DbgPrint("Driver unloaded\n");
}

NTSTATUS GetSystemProcess(PEPROCESS* SystemProcess) {
    NTSTATUS status;
    HANDLE hSystemProcess;
    OBJECT_ATTRIBUTES objAttr;
    CLIENT_ID clientId;

    InitializeObjectAttributes(&objAttr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
    clientId.UniqueProcess = (HANDLE)4; // PID SYSTEM
    clientId.UniqueThread = NULL;

    // Open Process SYSTEM
    status = ZwOpenProcess(&hSystemProcess, PROCESS_ALL_ACCESS, &objAttr, &clientId);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = ObReferenceObjectByHandle(hSystemProcess, PROCESS_ALL_ACCESS, *PsProcessType, KernelMode, (PVOID*)SystemProcess, NULL);
    ZwClose(hSystemProcess);

    return status;
}

//  IOCTL Manipulation
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

    DbgPrint("IRP_MJ_CREATE receive.\n");

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    PDEVICE_OBJECT deviceObject;
    NTSTATUS status;

    // Create device
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
        DbgPrint("Error create device %08x\n", status);
        return status;
    }


    status = IoCreateSymbolicLink(&DEVICE_SYMBOLIC_NAME, &DEVICE_NAME);
    if (!NT_SUCCESS(status)) {
        DbgPrint("Error create link simbólico: %08x\n", status);
        IoDeleteDevice(deviceObject);
        return status;
    }

    DbgPrint("Device and Symbolic Links created.\n");

    // Routines IRP
    DriverObject->MajorFunction[IRP_MJ_CREATE] = MajorFunctions;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = MajorFunctions;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControlHandler;
    DriverObject->DriverUnload = DriverUnload;

    return STATUS_SUCCESS;
}
