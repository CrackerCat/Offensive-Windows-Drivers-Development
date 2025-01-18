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

DEFINE_GUID (GUID_DEVINTERFACE_VulnerableDriver,
    0x57e649d9,0xca9a,0x416a,0x8c,0xc8,0xee,0x35,0xc8,0x99,0x86,0xce);
// {57e649d9-ca9a-416a-8cc8-ee35c89986ce}
