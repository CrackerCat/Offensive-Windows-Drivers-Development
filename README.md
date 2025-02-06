# Offensive-Windows-Drivers-Development

## Overview

**Offensive-Windows-Drivers-Development** is a research project designed to explore the development of Windows kernel-mode and user-mode drivers for offensive security purposes. The project focuses on techniques for low-level interaction with the Windows operating system, including file system interception, process manipulation, and advanced memory operations.

The goal is to provide insights into Windows internals and practical implementations that can aid red teamers, penetration testers and researchers in understanding how kernel-mode and user-mode drivers can be used in offensive scenarios, while also emphasizing the importance of defensive mechanisms to counter such techniques.


## Features

- **File System Interception**: Monitor and modify file I/O operations.
- **File Encryption**: Implement AES-based encryption at the kernel level.
- **Process Injection**: Advanced techniques for process manipulation from kernel space.
- **EDR Evasion**: Techniques for bypassing endpoint detection and response (EDR) solutions.
- **Memory Operations**: Direct manipulation of memory at the kernel level.
- **Proof-of-Concept (PoC) Drivers**: Examples for educational purposes.

## Prerequisites

- **Operating System**: Windows 10/11 (x64) with a kernel debugger (e.g., WinDbg).
- **Development Environment**: Visual Studio with Windows Driver Kit (WDK).
- **Tools**:
  - [WinDbg](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/)
  - [Process Hacker](https://processhacker.sourceforge.io/)
  - [Sysinternals Suite](https://learn.microsoft.com/en-us/sysinternals/)
 
## References
https://www.blackhat.com/docs/eu-17/materials/eu-17-Corina-Difuzzing-Android-Kernel-Drivers.pdf


https://voidsec.com/windows-drivers-reverse-engineering-methodology/

https://github.com/koutto/ioctlbf

https://github.com/otavioarj/SIOCTLBF

https://v1k1ngfr.github.io/winkernel-reverse-ida-ghidra/

https://infosecwriteups.com/understanding-ioctls-for-windows-vulnerability-research-exploit-development-c49229b38d8d

https://guidedhacking.com/threads/how-to-find-vulnerable-drivers-with-ioctlance.20824/

https://exploitreversing.com/2024/01/03/exploiting-reversing-er-series-article-02/

https://www.cyberark.com/resources/threat-research-blog/inglourious-drivers-a-journey-of-finding-vulnerabilities-in-drivers

https://www.cyberark.com/resources/threat-research-blog/finding-bugs-in-windows-drivers-part-1-wdm

https://research.checkpoint.com/2024/breaking-boundaries-investigating-vulnerable-drivers-and-mitigating-risks/

https://blogs.vmware.com/security/2023/10/hunting-vulnerable-kernel-drivers.html

https://www.unknowncheats.me/forum/general-programming-and-reversing/461976-methodology-static-reverse-engineering-windows-kernel-drivers.html

