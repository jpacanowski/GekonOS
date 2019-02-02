# GekonOS
a lightweight 32-bit protected mode microkernel operating system

### Setting up the programming environment

```sh
$ sudo apt update
$ sudo apt upgrade
$ sudo apt install nasm
$ sudo apt install build-essential
$ sudo apt install xorriso
$ sudo apt install grub-mkrescue
$ sudo apt install qemu
```

### Building the OS from source code

```sh
$ ./build.sh
```

### Running the OS on Qemu

```sh
$ ./run_qemu.sh
```

### TODO
- [ ] obsługa trybu tekstowego
- [ ] utworzenie własnej tablicy GDT i przełączenie pagingu na nią (ta którą utworzy GRUB prawdopodobnie nie bedzie wystarczająca)
- [ ] utworzenie struktur zarządzania pamięcią (tablice bitowe) oraz zaznaczenie w nich sektorów specjalnych i chronionych
- [ ] utworzenie pagingu
- [ ] przemapowanie przerwań
- [ ] utworzenie IDT i jej inicjacja
- [ ] zamontowanie podstawowych ISRów oraz ISRa pustego
- [ ] zainicjowanie PIT
- [ ] utworzenie i zainicjowanie TSSa systemowego (software task switching)
- [ ] obliczenie wartości początkowych dla multitaskingu
- [ ] uruchomienie pierwszego procesu
- [ ] loader / manager modułów
- [ ] sterownik klawiatury
- [ ] sterownik FDC
- [ ] system plików FAT12
- [ ] shell

### Changelog

**v0.1** (15.05.2016)
* initial release
