# mkpac
Kernel Packet Processing for Manycore Systems

This work is aimed at improving the Linux kernel packet processing performance for High-Speed NICs (40G/100G) on Manycore systems. We focus on the kernel space applications (e.g. software based packet forwarding (OVS kernel mode), software based firewall, and other NFV use-cases) and aim to optimize the kernel network stack performance by mitigating high-impact overheads residing in network stack of Linux kernel and by leveraging the availability of manycores. With TX and RX path optimizations, considerable performance improvement can be achieved for small packet sizes.

mkpac is provided under the terms of the GNU General Public License v2.0.

## Key Optimizations
- TX Optimizations
  - Adaptive TX cleanup
  - TX spin lock optimizations
  - TX queue selection
- RX Optimizations 
  - Ring-descriptors pre-allocation

## Prerequisites
- Linux Kernel version 4.4.178
- Intel XL710 40G NIC

## Build and install the kernel
- TX and RX Optimizations
```
    - mkpac was developed on Linux 4.4.178
    - The Linux kernel source with mkpac patches is available in ./linux-4.4.178
    - Build and install the kernel.
```
- Test environment set-up
  - OVS (Kernel Mode) Packet Forwarding is used to forward packet from RX to TX port.
  - 40G traffic is generated using DPDK-Pktgen for testing the perf.
  
## Other resources
- [Middleware 2018 paper](https://dl.acm.org/doi/abs/10.1145/3284014.3284022)
- [IEEE Access 2020 paper](https://ieeexplore.ieee.org/document/9134769)
