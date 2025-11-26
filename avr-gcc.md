# Building AVR Toolchain

## Prerequisites

### Tools

```sh
sudo apt update
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo wget xz-utils git gawk gnupg -y
```

### Folders

```sh
mkdir ~/avr-build && cd ~/avr-build
export PREFIX=/opt/avr-gcc-15
sudo mkdir -p $PREFIX
```

### Public Keys

```sh
wget -q https://ftp.gnu.org/gnu/gnu-keyring.gpg
gpg --quiet --import gnu-keyring.gpg
```

## Binutils

Latest version of `Binutils` can be found [here](https://ftp.gnu.org/gnu/binutils/?C=M;O=D).

### Download and Verify Binutils

```sh
wget https://ftp.gnu.org/gnu/binutils/binutils-2.45.tar.xz \
  https://ftp.gnu.org/gnu/binutils/binutils-2.45.tar.xz.sig
gpg --verify binutils-2.45.tar.xz.sig binutils-2.45.tar.xz
```

### Build Binutils

```sh
tar xJf binutils-2.45.tar.xz --no-same-owner
mkdir binutils-build && cd binutils-build
../binutils-2.45/configure --prefix=$PREFIX --target=avr --disable-nls --disable-werror
make -j$(nproc)
sudo make install
cd ..
```

## GCC

Latest version of `GCC` can be found [here](https://ftp.gnu.org/gnu/gcc/?C=M;O=D).

### Download and Verify GCC

```sh
wget https://ftp.gnu.org/gnu/gcc/gcc-15.2.0/gcc-15.2.0.tar.xz \
  https://ftp.gnu.org/gnu/gcc/gcc-15.2.0/gcc-15.2.0.tar.xz.sig
gpg --verify gcc-15.2.0.tar.xz.sig gcc-15.2.0.tar.xz
```

### Build GCC

```sh
tar xJf gcc-15.2.0.tar.xz --no-same-owner
cd gcc-15.2.0
./contrib/download_prerequisites
cd ..
mkdir gcc-build && cd gcc-build
../gcc-15.2.0/configure --prefix=$PREFIX --target=avr --enable-languages=c \
  --disable-nls --disable-libssp --with-dwarf2 --disable-libstdcxx-pch \
  --disable-bootstrap --disable-multilib
make -j$(nproc) all-gcc all-target-libgcc
sudo make install-gcc install-target-libgcc
cd ..
```

## AVR-LibC

Latest version of `AVR-LibC` can be found [here](https://github.com/avrdudes/avr-libc/releases).

### Download and Verify AVR-LibC

```sh
wget https://github.com/avrdudes/avr-libc/releases/download/avr-libc-2_2_1-release/avr-libc-2.2.1.tar.bz2 \
  https://github.com/avrdudes/avr-libc/releases/download/avr-libc-2_2_1-release/avr-libc-2.2.1.tar.bz2.sig
gpg --verify avr-libc-2.2.1.tar.bz2.sig avr-libc-2.2.1.tar.bz2
```

### Build AVR-LibC

You must install [`avr-binutils`](#build-binutils), [`avr-gcc`](#build-gcc) and make sure your [PATH](#add-to-path) is set properly before installing `AVR-LibC`.

```sh
tar xjf avr-libc-2.2.1.tar.bz2 --no-same-owner
mkdir libc-build && cd libc-build
../avr-libc-2.2.1/configure --prefix=$PREFIX --build=$(../avr-libc-2.2.1/config.guess) --host=avr
make -j$(nproc)
sudo make install
cd ..
```

## avrdude

```sh
git clone https://github.com/avrdudes/avrdude.git
cd avrdude
./configure --prefix=$PREFIX
make -j$(nproc)
sudo make install
cd ..
```

## Add to PATH

```sh
echo 'export PATH="'$PREFIX'/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```