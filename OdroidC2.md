# <span style="color: orange">WiKi</span>

[Link](https://wiki.odroid.com/odroid-c2/odroid-c2)

## <span style="color: orange">Download OS image</span>
[Link](https://wiki.odroid.com/odroid-c2/os_images/ubuntu/v4.1)
## Установка WiFi модуля
```shell
sudo apt install net-tools git make cmake clang gcc gdb libboost1.71-all-dev htop libcrypt-dev
```

## <span style="color: orange">Установка WiFi модуля</span>
```shell
sudo apt-get update && sudo apt-get upgrade
sudo apt-get install build-essential git dkms linux-headers-$(uname -r)
git clone https://github.com/kelebek333/rtl8188fu
sudo dkms add ./rtl8188fu
sudo dkms build rtl8188fu/1.0
sudo dkms install rtl8188fu/1.0
sudo modprobe rtl8188fu
```

## <span style="color: orange">Установка WiringPI</span>
```shell
sudo apt install software-properties-common
sudo add-apt-repository ppa:hardkernel/ppa
sudo apt update
sudo apt install odroid-wiringpi
sudo apt install libwiringpi-dev libwiringpi2
```

## <span style="color: orange">Настройка i2c</span>
[Link](https://wiki.odroid.com/odroid-c2/application_note/gpio/i2c)
```shell
sudo apt-get install i2c-tools
modprobe aml_i2c
sudo nano /etc/modules
"
  i2c-dev
  aml_i2c
"  
sudo adduser odroid i2c
sudo reboot
```

## <span style="color: orange">Настройка swapfile</span>
```shell
sudo fallocate -l 2G /swapfile
sudo dd if=/dev/zero of=/swapfile bs=1024 count=2097152
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
sudo nano /etc/fstab
"
  /swapfile swap swap defaults 0 0
"
```

## Monitoring

* CPU
```asm
cat /sys/devices/virtual/thermal/thermal_zone0/temp
```