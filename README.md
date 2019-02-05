# GekonOS
GekonOS is a hobbyist, educational, lightweight,completely independent, Unix-like operating system built entirely from scratch.

### Setting up the programming environment

Several packages are necessary to build GekonOS from source.

```sh
$ sudo apt update
$ sudo apt upgrade
```
(first, update the package lists)

```sh
$ sudo apt install nasm
```
(Install this to compile assembly source files)

```sh
$ sudo apt install build-essential
```
(Install this to build the cross-compiler)

```sh
$ sudo apt install xorriso
```
(Install this to create CD images)

```sh
$ sudo apt install grub-mkrescue
```
(Install this for GRUB bootloader)

```sh
$ sudo apt install qemu
```
(Install QEMU to run the system)

The build process has two parts: building a cross-compiler, and building the operating system.

### Building the OS from source code

```sh
$ ./build.sh
```

### Running GekonOS
It is highly recommended to run GekonOS on virtual machine. Some testing on real hardware are done, but driver support is still limited and virtual machines provide easily tested environments. QEMU, Bochs and VirtualBox are recommended and provide the most functionality.

### Running the OS on Qemu

```sh
$ ./run_qemu.sh
```

### Running the OS on Bochs

```sh
$ ./run_bochs.sh
```

### TODO
- [x] obsługa trybu tekstowego
- [x] utworzenie własnej tablicy GDT i przełączenie pagingu na nią (ta którą utworzy GRUB prawdopodobnie nie bedzie wystarczająca)
- [ ] utworzenie struktur zarządzania pamięcią (tablice bitowe) oraz zaznaczenie w nich sektorów specjalnych i chronionych
- [ ] utworzenie pagingu
- [x] przemapowanie przerwań
- [x] utworzenie IDT i jej inicjacja
- [x] zamontowanie podstawowych ISRów oraz ISRa pustego
- [x] zainicjowanie PIT
- [ ] utworzenie i zainicjowanie TSSa systemowego (software task switching)
- [ ] obliczenie wartości początkowych dla multitaskingu
- [ ] uruchomienie pierwszego procesu
- [ ] loader / manager modułów
- [x] sterownik klawiatury
- [ ] sterownik FDC
- [ ] system plików FAT12
- [ ] shell

### Changelog

**v0.1** (15.05.2016)
* initial release
