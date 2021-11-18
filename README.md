# mkpac
Kernel Packet Processing for Manycore Systems

This work is aimed at improving the Linux kernel packet processing performance for High-Speed NICs (40G/100G) on Manycore systems. We focus on the kernel space applications (e.g. software based packet forwarding (OVS kernel mode), software based firewall, and other NFV use-cases) and aim to optimize the kernel network stack performance by mitigating high-impact overheads residing in network stack of Linux kernel and by leveraging the availability of manycores. With TX and RX path optimizations, considerable performance improvement can be achieved for small packet sizes.

## Key Optimizations
- TX Optimizations
  - Adaptive TX cleanup
  - TX spin lock optimizations
  - TX queue selection
- RX Optimizations 
  - Ring-descriptors pre-allocation

## Prerequisites
- Linux Kernel version 4.4.1
- Intel XL710 40G NIC

## Configuration and Testing
- TX and RX Optimizations
```{.sh}
$> git clone https://github.com/oslab-swrc/mkpac.git
```
```{.sh}
$> patch -p1 < mkpac-txopt.patch
```
```{.sh}
$> patch -p0 < mkpax-rxopt.patch
```
- Test environment set-up
  - OVS (Kernel Mode) Packet Forwarding 
  - 40G traffic generation using DPDK-Pktgen
  
## Other resources
- [Middleware 2018 paper](https://dl.acm.org/doi/abs/10.1145/3284014.3284022)
- [IEEE Access 2020 paper](https://ieeexplore.ieee.org/document/9134769)
