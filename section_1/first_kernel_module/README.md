# Linux Kernel Module Makefile

## Overview

This project demonstrates how to build a simple **Linux Kernel Module (LKM)** named `hello`.

### Project Structure

```text
hello/
├── hello.c
└── Makefile
```

After compilation, the kernel build system generates `hello.ko`, a loadable **Kernel Object**.

## Makefile

```makefile
obj-m += hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

## Makefile Explanation

### `obj-m += hello.o`

```makefile
obj-m += hello.o
```

This tells the Linux kernel build system (Kbuild) to build `hello.o` as a loadable kernel module.

```text
hello.c
   ↓
hello.o
   ↓
hello.ko
```

`hello.ko` is the final kernel module.

### `all` Target

```makefile
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
```

`all` is the default target. Running:

```bash
make
```

executes this target and builds the module.

### `$(shell uname -r)`

This runs:

```bash
uname -r
```

to obtain the currently running Linux kernel version.

For example:

```text
6.12.47+rpt-rpi-2712
```

The build directory therefore becomes:

```text
/lib/modules/6.12.47+rpt-rpi-2712/build
```

Verify that it exists:

```bash
ls -l /lib/modules/$(uname -r)/build
```

The kernel headers/build files must match the running kernel.

### `make -C`

The `-C` option tells `make` to change to the Linux kernel build directory:

```text
/lib/modules/$(uname -r)/build
```

The module is compiled through the Linux **Kbuild** system rather than by directly calling GCC. This provides the required kernel headers, configuration, architecture settings, compiler flags, and module build rules.

### `M=$(PWD)`

```makefile
M=$(PWD)
```

tells Kbuild that the external kernel module source is located in the current working directory.

For example:

```text
M=/home/pi/kernel_modules/hello
```

### `modules`

The `modules` target tells Kbuild to compile the external module specified by:

```makefile
obj-m += hello.o
```

The complete command:

```bash
make -C /lib/modules/$(uname -r)/build M=$(PWD) modules
```

means:

> Use the build system for the currently running kernel and build the external kernel module located in the current directory.

## Build the Module

```bash
make
```

A successful build generates files such as:

```text
hello.ko
hello.o
hello.mod
hello.mod.c
hello.mod.o
Module.symvers
modules.order
```

The main output is:

```text
hello.ko
```

## Load the Module

```bash
sudo insmod hello.ko
```

Verify:

```bash
lsmod | grep hello
```

## Check Kernel Messages

```bash
sudo dmesg | tail -20
```

Alternatively:

```bash
sudo journalctl -k -n 20
```

## Remove the Module

```bash
sudo rmmod hello
```

Then check the kernel log:

```bash
sudo dmesg | tail -20
```

## Clean the Build

```bash
make clean
```

The clean target calls:

```makefile
make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

and removes generated build files while retaining the source files and Makefile.

## Build Flow

```text
                    make
                      │
                      ↓
              Get Kernel Version
                      │
                      ↓
                  uname -r
                      │
                      ↓
          6.12.47+rpt-rpi-2712
                      │
                      ↓
 /lib/modules/6.12.47+rpt-rpi-2712/build
                      │
                      ↓
            Linux Kbuild System
                      │
                 M=$(PWD)
                      │
                      ↓
                  hello.c
                      │
                      ↓
                  hello.o
                      │
                      ↓
                  hello.ko
```

## Typical Workflow

```bash
# Build
make

# Load
sudo insmod hello.ko

# Verify
lsmod | grep hello

# Check messages
sudo dmesg | tail -20

# Unload
sudo rmmod hello

# Clean
make clean
```

## Raspberry Pi Kernel Header Requirement

The module must be compiled against headers compatible with the currently running kernel.

Check the running kernel:

```bash
uname -r
```

Example:

```text
6.12.47+rpt-rpi-2712
```

Check the build link:

```bash
ls -l /lib/modules/$(uname -r)/build
```

If this path does not exist, the matching kernel headers/build files must be installed before the module can be compiled.

## Quick Reference

| Command | Purpose |
|---|---|
| `uname -r` | Show running kernel version |
| `make` | Build the kernel module |
| `sudo insmod hello.ko` | Load the module |
| `lsmod \| grep hello` | Check whether the module is loaded |
| `sudo dmesg \| tail -20` | View recent kernel messages |
| `sudo rmmod hello` | Unload the module |
| `make clean` | Remove generated build files |

## Summary

The Makefile uses the standard Linux **Kbuild** infrastructure to compile `hello.c` into `hello.ko`.

```makefile
make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
```

- `-C` — use the Linux kernel build directory.
- `uname -r` — obtain the currently running kernel version.
- `M=$(PWD)` — specify the external module source directory.
- `modules` — build the external kernel module.

The resulting `hello.ko` can be loaded with `insmod` and removed with `rmmod`.
