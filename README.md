# bookreadingmemo-linker-loader-practical-devtips-2025
「リンカ・ローダ実践開発テクニック」(坂井 弘亮, CQ出版社, 2010)を2025年に読み直してみた時のメモ

- [「リンカ・ローダ実践開発テクニック」(坂井 弘亮, CQ出版社, 2010)](https://shop.cqpub.co.jp/hanbai/books/38/38071.htm) を2025年のLinux環境で読み直してみた時のメモです。
- 写経環境: Amazon Linux 2023 (64bit) (2025年以降の新し目のEC2環境)
- 追加パッケージ
  - `$ sudo dnf install vim screen` (EC2起動時点で既にインストール済み)
  - `$ sudo dnf group install "Development Tools"`
  - `$ sudo dnf install gdb`
- 本リポジトリのライセンス
  - 書籍のサンプルコードのライセンス(上記出版社ページからDLできるサンプルコード参照)に準じます。

写経環境の各種バージョン:

```
$ uname -r
6.1.130-139.222.amzn2023.x86_64

$ gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/libexec/gcc/x86_64-amazon-linux/11/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-amazon-linux
Configured with: ../configure --enable-bootstrap --enable-host-pie --enable-host-bind-now --enable-languages=c,c++,fortran,lto --prefix=/usr --mandir=/usr/share/man --infodir=/usr/share/info --with-bugurl=https://github.com/amazonlinux/amazon-linux-2022 --enable-shared --enable-threads=posix --enable-checking=release --with-system-zlib --enable-__cxa_atexit --disable-libunwind-exceptions --enable-gnu-unique-object --enable-linker-build-id --with-gcc-major-version-only --enable-plugin --enable-initfini-array --with-isl=/builddir/build/BUILD/gcc-11.5.0-20240719/obj-x86_64-amazon-linux/isl-install --enable-multilib --with-linker-hash-style=gnu --enable-offload-targets=nvptx-none --without-cuda-driver --enable-gnu-indirect-function --enable-cet --with-tune=generic --with-arch_64=x86-64-v2 --with-arch_32=x86-64 --build=x86_64-amazon-linux --with-build-config=bootstrap-lto --enable-link-serialization=1
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 11.5.0 20240719 (Red Hat 11.5.0-5) (GCC)

$ gdb -v
GNU gdb (GDB) Amazon Linux 12.1-5.amzn2023.0.4
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

$ rpm -qa | grep binutils
binutils-2.39-6.amzn2023.0.11.x86_64

$ rpm -qa | grep elfutils | sort
elfutils-0.188-3.amzn2023.0.2.x86_64
elfutils-debuginfod-client-0.188-3.amzn2023.0.2.x86_64
elfutils-debuginfod-client-devel-0.188-3.amzn2023.0.2.x86_64
elfutils-default-yama-scope-0.188-3.amzn2023.0.2.noarch
elfutils-devel-0.188-3.amzn2023.0.2.x86_64
elfutils-libelf-0.188-3.amzn2023.0.2.x86_64
elfutils-libelf-devel-0.188-3.amzn2023.0.2.x86_64
elfutils-libs-0.188-3.amzn2023.0.2.x86_64
```
