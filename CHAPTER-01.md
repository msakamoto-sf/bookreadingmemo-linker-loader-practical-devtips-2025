# 第1賞 リンカとローダの役割

### リスト 1.1 hello.c

hello.c:

```
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Hello, World\n");
    exit(0);
}
```

変更点: 書籍のままだと `exit()` 呼び出しで `warning: implicit declaration of function exit` が発生し、stdlib.h を使ってね、と案内されたのでそちらの include を追加している。

### リスト 1.2 リンクまでの流れ

写経環境で実行:

```
$ gcc hello.c -Wall -o hello -v
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
COLLECT_GCC_OPTIONS='-Wall' '-o' 'hello' '-v' '-mtune=generic' '-march=x86-64-v2'
 /usr/libexec/gcc/x86_64-amazon-linux/11/cc1 -quiet -v hello.c -quiet -dumpbase hello.c -dumpbase-ext .c -mtune=generic -march=x86-64-v2 -Wall -version -o /tmp/ccYx4kIj.s
GNU C17 (GCC) version 11.5.0 20240719 (Red Hat 11.5.0-5) (x86_64-amazon-linux)
        compiled by GNU C version 11.5.0 20240719 (Red Hat 11.5.0-5), GMP version 6.2.1, MPFR version 4.1.0-p13, MPC version 1.2.1, isl version isl-0.18-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
ignoring nonexistent directory "/usr/lib/gcc/x86_64-amazon-linux/11/include-fixed"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-amazon-linux/11/../../../../x86_64-amazon-linux/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-amazon-linux/11/include
 /usr/local/include
 /usr/include
End of search list.
GNU C17 (GCC) version 11.5.0 20240719 (Red Hat 11.5.0-5) (x86_64-amazon-linux)
        compiled by GNU C version 11.5.0 20240719 (Red Hat 11.5.0-5), GMP version 6.2.1, MPFR version 4.1.0-p13, MPC version 1.2.1, isl version isl-0.18-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
Compiler executable checksum: 4cd4b384fe11ef2a0a8ebbcd6fd64403
COLLECT_GCC_OPTIONS='-Wall' '-o' 'hello' '-v' '-mtune=generic' '-march=x86-64-v2'
 as -v --64 -o /tmp/ccjOCocm.o /tmp/ccYx4kIj.s
GNU assembler version 2.39 (x86_64-amazon-linux) using BFD version version 2.39-6.amzn2023.0.11
COMPILER_PATH=/usr/libexec/gcc/x86_64-amazon-linux/11/:/usr/libexec/gcc/x86_64-amazon-linux/11/:/usr/libexec/gcc/x86_64-amazon-linux/:/usr/lib/gcc/x86_64-amazon-linux/11/:/usr/lib/gcc/x86_64-amazon-linux/
LIBRARY_PATH=/usr/lib/gcc/x86_64-amazon-linux/11/:/usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/:/lib/../lib64/:/usr/lib/../lib64/:/usr/lib/gcc/x86_64-amazon-linux/11/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-Wall' '-o' 'hello' '-v' '-mtune=generic' '-march=x86-64-v2' '-dumpdir' 'hello.'
 /usr/libexec/gcc/x86_64-amazon-linux/11/collect2 -plugin /usr/libexec/gcc/x86_64-amazon-linux/11/liblto_plugin.so -plugin-opt=/usr/libexec/gcc/x86_64-amazon-linux/11/lto-wrapper -plugin-opt=-fresolution=/tmp/ccjG3ZOh.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s --build-id --no-add-needed --eh-frame-hdr --hash-style=gnu -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o hello /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crt1.o /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crti.o /usr/lib/gcc/x86_64-amazon-linux/11/crtbegin.o -L/usr/lib/gcc/x86_64-amazon-linux/11 -L/usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 -L/usr/lib/gcc/x86_64-amazon-linux/11/../../.. /tmp/ccjOCocm.o -lgcc --push-state --as-needed -lgcc_s --pop-state -lc -lgcc --push-state --as-needed -lgcc_s --pop-state /usr/lib/gcc/x86_64-amazon-linux/11/crtend.o /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crtn.o
COLLECT_GCC_OPTIONS='-Wall' '-o' 'hello' '-v' '-mtune=generic' '-march=x86-64-v2' '-dumpdir' 'hello.'
```

ポイント:

- 書籍ではプリプロセッサで `/usr/libexec/cpp0` が呼ばれているが、Amazon Linux 2023 では `/usr/libexec/gcc/x86_64-amazon-linux/11/cc1` によりプリプロセスとアセンブラへの変換まで行われているように見える。
- 書籍ではリンカとして `/usr/libexec/elf/ld` が呼ばれているが、Amazon Linux 2023 では `/usr/libexec/gcc/x86_64-amazon-linux/11/collect2` がリンク処理を担当しているように見える。

### リスト 1.3 size hello の結果

```
$ gcc hello.c -o hello -Wall

$ ./hello
Hello, World

$ size hello
   text    data     bss     dec     hex filename
   1244     556       4    1804     70c hello
```

### リスト 1.4 objdump -h hello の結果

```
$ objdump -h hello

hello:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .interp       0000001c  0000000000400318  0000000000400318  00000318  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .note.gnu.property 00000040  0000000000400338  0000000000400338  00000338  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .note.gnu.build-id 00000024  0000000000400378  0000000000400378  00000378  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .note.ABI-tag 00000020  000000000040039c  000000000040039c  0000039c  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .gnu.hash     0000001c  00000000004003c0  00000000004003c0  000003c0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .dynsym       000000a8  00000000004003e0  00000000004003e0  000003e0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .dynstr       00000083  0000000000400488  0000000000400488  00000488  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .gnu.version  0000000e  000000000040050c  000000000040050c  0000050c  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  8 .gnu.version_r 00000030  0000000000400520  0000000000400520  00000520  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  9 .rela.dyn     00000060  0000000000400550  0000000000400550  00000550  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 10 .rela.plt     00000030  00000000004005b0  00000000004005b0  000005b0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 11 .init         0000001b  0000000000401000  0000000000401000  00001000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 12 .plt          00000030  0000000000401020  0000000000401020  00001020  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 13 .text         000000fe  0000000000401050  0000000000401050  00001050  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 14 .fini         0000000d  0000000000401150  0000000000401150  00001150  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 15 .rodata       0000001d  0000000000402000  0000000000402000  00002000  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 16 .eh_frame_hdr 0000002c  0000000000402020  0000000000402020  00002020  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 17 .eh_frame     00000088  0000000000402050  0000000000402050  00002050  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 18 .init_array   00000008  0000000000403de8  0000000000403de8  00002de8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .fini_array   00000008  0000000000403df0  0000000000403df0  00002df0  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 20 .dynamic      000001d0  0000000000403df8  0000000000403df8  00002df8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 21 .got          00000020  0000000000403fc8  0000000000403fc8  00002fc8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 22 .got.plt      00000028  0000000000403fe8  0000000000403fe8  00002fe8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 23 .data         00000004  0000000000404010  0000000000404010  00003010  2**0
                  CONTENTS, ALLOC, LOAD, DATA
 24 .bss          00000004  0000000000404014  0000000000404014  00003014  2**0
                  ALLOC
 25 .comment      0000005c  0000000000000000  0000000000000000  00003014  2**0
                  CONTENTS, READONLY
 26 .gnu.build.attributes 00001d10  0000000000406018  0000000000406018  00003070  2**2
                  CONTENTS, READONLY, OCTETS
```

### リスト 1.5 (size コマンドのソースコード)

書籍では FreeBSD 上の `usr/src/usr.bin/size/size.c` を参照している。
今回は Amazon Linux 2023 上であるため、まず size コマンドのパッケージを特定した後、source RPMをダウンロードしてソースコードを確認してみる。

size コマンドのパッケージを特定:

```
$ which size
/usr/bin/size

$ rpm -qf /usr/bin/size
binutils-2.39-6.amzn2023.0.11.x86_64
```

source RPMの展開(+ ビルド)用に `~/rpmbuild/` をセットアップ:

```
$ sudo dnf install rpmdevtools

$ rpmdev-setuptree
```

binutils の source RPM をダウンロードしてインストール:

```
$ dnf download --source binutils
-> カレントディレクトリに binutils-2.39-6.amzn2023.0.11.src.rpm がDLされる。

$ mv binutils-2.39-6.amzn2023.0.11.src.rpm ~/rpmbuild/SRPMS/

$ rpm -i ~/rpmbuild/SRPMS/binutils-2.39-6.amzn2023.0.11.src.rpm

$ find ~/rpmbuild/
(...)
/home/ec2-user/rpmbuild/SOURCES/CVE-2022-38533.patch
(...)
/home/ec2-user/rpmbuild/SOURCES/CVE-2025-0840.patch
/home/ec2-user/rpmbuild/SOURCES/binutils-2.19.50.0.1-output-format.sed
/home/ec2-user/rpmbuild/SOURCES/binutils-2.27-aarch64-ifunc.patch
/home/ec2-user/rpmbuild/SOURCES/binutils-2.39.tar.xz
/home/ec2-user/rpmbuild/SOURCES/binutils-CVE-38128-dwarf-abbrev-parsing.patch
(...)
/home/ec2-user/rpmbuild/SPECS/binutils.spec
(...)
```

ソースコードの展開とパッチ適用まで実行し、`~/rpmbuild/BUILD/binutils-2.39/` 以下のソースコードを確認:

```
$ rpmbuild -bp ~/rpmbuild/SPECS/binutils.spec

$ find ~/rpmbuild/BUILD/binutils-2.39/ -name size.c
/home/ec2-user/rpmbuild/BUILD/binutils-2.39/binutils/size.c
```

書籍と同じ箇所の `size.c` の抜粋 ... しようと思ったが、そもそも書籍では「実はこの `size.c` は `a.out` 用の古いソースでした」という流れになっている。この後リスト 1.8 で、今回の環境と同じく binutils 側の `size.c` を紹介しているのでこちらを確認してみる。

### リスト 1.8 セクションのサイズをカウントしている部分

写経環境で、リスト 1.5 でダウンロード + 展開してきた binutils の `~/rpmbuild/BUILD/binutils-2.39/binutils/size.c` から抜粋:

```
    469 berkeley_or_gnu_sum (bfd *abfd ATTRIBUTE_UNUSED, sec_ptr sec,
    470                      void *ignore ATTRIBUTE_UNUSED)
    471 {
    472   flagword flags;
    473   bfd_size_type size;
    474
    475   flags = bfd_section_flags (sec);
    476   if ((flags & SEC_ALLOC) == 0)
    477     return;
    478
    479   size = bfd_section_size (sec);
    480   if ((flags & SEC_CODE) != 0
    481       || (selected_output_format == FORMAT_BERKLEY
    482           && (flags & SEC_READONLY) != 0))
    483     textsize += size;
    484   else if ((flags & SEC_HAS_CONTENTS) != 0)
    485     datasize += size;
    486   else
    487     bsssize += size;
    488 }
```

ほぼ書籍通りの内容となっていることを確認できた。

### リスト 1.9 objdump -p hello の結果

```
$ objdump -p hello

hello:     file format elf64-x86-64

Program Header:
    PHDR off    0x0000000000000040 vaddr 0x0000000000400040 paddr 0x0000000000400040 align 2**3
         filesz 0x00000000000002d8 memsz 0x00000000000002d8 flags r--
  INTERP off    0x0000000000000318 vaddr 0x0000000000400318 paddr 0x0000000000400318 align 2**0
         filesz 0x000000000000001c memsz 0x000000000000001c flags r--
    LOAD off    0x0000000000000000 vaddr 0x0000000000400000 paddr 0x0000000000400000 align 2**12
         filesz 0x00000000000005e0 memsz 0x00000000000005e0 flags r--
    LOAD off    0x0000000000001000 vaddr 0x0000000000401000 paddr 0x0000000000401000 align 2**12
         filesz 0x000000000000015d memsz 0x000000000000015d flags r-x
    LOAD off    0x0000000000002000 vaddr 0x0000000000402000 paddr 0x0000000000402000 align 2**12
         filesz 0x00000000000000d8 memsz 0x00000000000000d8 flags r--
    LOAD off    0x0000000000002de8 vaddr 0x0000000000403de8 paddr 0x0000000000403de8 align 2**12
         filesz 0x000000000000022c memsz 0x0000000000000230 flags rw-
 DYNAMIC off    0x0000000000002df8 vaddr 0x0000000000403df8 paddr 0x0000000000403df8 align 2**3
         filesz 0x00000000000001d0 memsz 0x00000000000001d0 flags rw-
    NOTE off    0x0000000000000338 vaddr 0x0000000000400338 paddr 0x0000000000400338 align 2**3
         filesz 0x0000000000000040 memsz 0x0000000000000040 flags r--
    NOTE off    0x0000000000000378 vaddr 0x0000000000400378 paddr 0x0000000000400378 align 2**2
         filesz 0x0000000000000044 memsz 0x0000000000000044 flags r--
0x6474e553 off    0x0000000000000338 vaddr 0x0000000000400338 paddr 0x0000000000400338 align 2**3
         filesz 0x0000000000000040 memsz 0x0000000000000040 flags r--
EH_FRAME off    0x0000000000002020 vaddr 0x0000000000402020 paddr 0x0000000000402020 align 2**2
         filesz 0x000000000000002c memsz 0x000000000000002c flags r--
   STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
   RELRO off    0x0000000000002de8 vaddr 0x0000000000403de8 paddr 0x0000000000403de8 align 2**0
         filesz 0x0000000000000218 memsz 0x0000000000000218 flags r--

Dynamic Section:
  NEEDED               libc.so.6
  INIT                 0x0000000000401000
  FINI                 0x0000000000401150
  INIT_ARRAY           0x0000000000403de8
  INIT_ARRAYSZ         0x0000000000000008
  FINI_ARRAY           0x0000000000403df0
  FINI_ARRAYSZ         0x0000000000000008
  GNU_HASH             0x00000000004003c0
  STRTAB               0x0000000000400488
  SYMTAB               0x00000000004003e0
  STRSZ                0x0000000000000083
  SYMENT               0x0000000000000018
  DEBUG                0x0000000000000000
  PLTGOT               0x0000000000403fe8
  PLTRELSZ             0x0000000000000030
  PLTREL               0x0000000000000007
  JMPREL               0x00000000004005b0
  RELA                 0x0000000000400550
  RELASZ               0x0000000000000060
  RELAENT              0x0000000000000018
  VERNEED              0x0000000000400520
  VERNEEDNUM           0x0000000000000001
  VERSYM               0x000000000040050c

Version References:
  required from libc.so.6:
    0x09691a75 0x00 03 GLIBC_2.2.5
    0x069691b4 0x00 02 GLIBC_2.34
```

プログラムヘッダの数が書籍より多いが、表示内容は大きく変わっていないと思われる。


