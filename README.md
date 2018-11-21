# OSBoyzRaspi
Repo for CSE323 Operating System Project

GNU gcc-arm
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
Qemu VM
https://qemu.weilnetz.de/w64/

then run add the bin folder of gnu toolchain & the qemu folder to the system environment variable's user path list

command to run the kernel on qemu is
qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel spaceboyzos.elf
