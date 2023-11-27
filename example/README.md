# Example

To use this example we will use a virtual can interface named vcan0.
The two can0.sh and vcan0.sh scripts are example setup commands for physical and virtual can interfaces.

## VCAN interface on WSL2

By default the kernel isn't built with the vcan module. To enable it we need to rebuild the kernel. The procedure is the following:

1. Install the required packages

    ```bash
    sudo apt-get update -y
    sudo apt-get install -y autoconf  bison build-essential flex libelf-dev libncurses-dev libssl-dev libtool libudev-dev dwarves wslu
    ```

2. Get the kernel

    ```bash
    KERNEL_VERSION=$(uname -r | grep -Po '^.+?(?=-)')
    cd
    wget https://github.com/microsoft/WSL2-Linux-Kernel/archive/refs/tags/linux-msft-wsl-$(KERNEL_VERSION).tar.gz
    tar -xf linux-msft-wsl-$(KERNEL_VERSION).tar.gz
    cd WSL2-Linux-Kernel-linux-msft-wsl-$(KERNEL_VERSION)
    cat /proc/config.gz | gunzip > .config
    make prepare modules_prepare -j $(expr $(nproc) - 1)
    ```

3. Configure kernel to add support to vcan. This can be done manually using menuconfig (Networking support/CAN bus subsystem support/Virtual Local CAN interface)

    ```bash
    make menuconfig -j $(expr $(nproc) - 1)
    ```
    or by adding the following lines to the .config file

    ```bash
    CONFIG_CC_CAN_LINK=y
    CONFIG_CAN=m
    CONFIG_CAN_RAW=m
    CONFIG_CAN_BCM=m
    CONFIG_CAN_GW=m
    CONFIG_CAN_VCAN=m
    CONFIG_CAN_DEV=m
    CONFIG_CAN_CALC_BITTIMING=y
    CONFIG_CAN_DEBUG_DEVICES=y
    CONFIG_CC_HAS_ZERO_CALL_USED_REGS=y
    CONFIG_HAVE_KCSAN_COMPILER=y
    ```

4. Build the kernel

    ```bash
    make modules -j $(expr $(nproc) - 1)
    sudo make modules_install
    make -j $(expr $(nproc) - 1)
    sudo make install
    ```

5. Copy the kernel to your Windows userprofile

    ```bash
    cp vmlinux $(wslpath "$(wslvar USERPROFILE)")
    cd $(wslpath "$(wslvar USERPROFILE)")
    touch .wslconfig
    cp .wslconfig .wslconfig.bak
    echo "[wsl2]" > .wslconfig
    echo "kernel=$(wslvar USERPROFILE | sed 's/\\/\\\\\\\\/g')\\\\vmlinux" >> .wslconfig
    ```
6. Reboot WSL2

    ```cmd
    wsl --shutdown
    ```

7. Check if the kernel is loaded

    ```bash
    sudo modprobe vcan
    sudo ip link add dev vcan0 type vcan
    sudo ip link set up vcan0
    sudo apt install can-utils
    sudo modprobe can_raw

    candump  -c -c -ta vcan0
    ```

    in another terminal

    ```bash
    cansend vcan0 123#1122334455667788
    ```