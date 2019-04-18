## Building a SAM toolchain from the command line
This project walks you through the process of setting your toolchain to compile/deploy C code for SAM microcontrollers. Today there are many IDEs that magically automate this process for you. These are convenient but obscure much of what is going on behind the scenes from the developer. The key to becoming a better embedded engineer is understanding the toolchain compilation/deployment workflow. When the time comes to take your product to production these scripts will come in handy.

### Prerequisites:

### Installing avr-gcc

Installing arm-none-eabi-gcc on MacOS w/Brew
```console
brew tap osx-cross/arm
brew install arm-gcc-bin
```
verify installation
```console
arm-none-eabi-gcc --version
```


### Installing edbg
Before you start ensure automake is installed on your machine.
```console
brew install automake
```

Clone edbg from repository navigate to edbg folder and build and test
```console
https://github.com/ataradov/edbg.git
cd edbg
make all
./edbg --help
```


### load firmware using edbg
```console
/Users/johncobb/dev/edbg/edbg -bpv -t atmel_cm4 -f ~/dev/sam4s_usart/sam4s_usart.bin
```
