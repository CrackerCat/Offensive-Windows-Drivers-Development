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

DEFINE_GUID (GUID_DEVINTERFACE_CryptoDriver2,
    0x5f28c4f7,0xbb2d,0x4a5a,0x92,0x16,0x40,0xc9,0xf5,0xb8,0x78,0x02);
// {5f28c4f7-bb2d-4a5a-9216-40c9f5b87802}
