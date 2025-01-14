/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_RansDriver,
    0xfa46993a,0xc89d,0x46a6,0xa7,0xc6,0xa5,0xc6,0xb1,0x7e,0x7e,0x53);
// {fa46993a-c89d-46a6-a7c6-a5c6b17e7e53}
