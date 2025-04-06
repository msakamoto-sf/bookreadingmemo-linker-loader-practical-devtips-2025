# 第1章 リンカとローダの役割

### リスト 1.1 hello.c

- [hello.c](CHAPTER-01/hello.c)
- 変更点: 書籍のままだと `exit()` 呼び出しで `warning: implicit declaration of function exit` が発生し、stdlib.h を使ってね、と案内されたのでそちらの include を追加している。

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

### リスト 1.10, 1.11 さまざまな変数の定義

- [values_sub.c](CHAPTER-01/values_sub.c)
- [values.c](CHAPTER-01/values.c)

コンパイルと実行:

```
$ gcc -c values_sub.c
$ gcc -c values.c
$ gcc -o values values.o values_sub.o

$ ./values
   c = a
  i0 = 0,   i1 = 0,   i2 = 1
 ci0 = 0,  ci1 = 0,  ci2 = 1
  p1 = sample0,  str = sample1
  &c = 0x00404018
 &i0 = 0x00404040,  &i1 = 0x00404044,  &i2 = 0x0040401c
&ci0 = 0x0040210c, &ci1 = 0x00402110, &ci2 = 0x00402114
 &p1 = 0x00404028, &str = 0x00404030
```

### リスト 1.12 nm values_sub.o の結果

対応するCソースのコメントつき:

```
$ nm values_sub.o
0000000000000000 D c     // char c = 'a';
0000000000000000 R ci0   // const int ci0;
0000000000000004 R ci1   // const int ci1 = 0;
0000000000000008 R ci2   // const int ci2 = 1;
0000000000000000 T func  // int func() { ... }
0000000000000000 B i0    // int i0;
0000000000000004 B i1    // int i1 = 0;
0000000000000004 D i2    // int i2 = 1;
0000000000000008 B p0    // char * p0;
0000000000000010 D p1    // char * p1 = "sample0";
0000000000000010 b si0   // static int si0;
0000000000000014 b si1   // static int si1 = 0;
0000000000000008 d si2   // static int si2 = 1;
0000000000000018 D str   // char str[] = "sample1";
```

シンボルのタイプ(`nm(1)` より抜粋):

- B/b : 未初期化またはゼロ初期化されたBSS領域 (大文字がglobal, 小文字がlocal)
- D/d : 初期化済みのデータ領域 (大文字がglobal, 小文字がlocal)
- R/r : 読み込み専用領域 (大文字がglobal, 小文字がlocal)
- T/t : text(code)領域 (= 実行コード)
- U : このELFファイル内では未定義のシンボル

→ `values_sub.o` のシンボルタイプについて、 `values_sub.c` の変数定義と一致していることを確認できた。

参考: `nm values.o` の結果

```
$ nm values.o
                 U c
                 U ci0
                 U ci1
                 U ci2
                 U exit
                 U i0
                 U i1
                 U i2
0000000000000000 T main
                 U p1
                 U printf
                 U str
```

### リスト 1.13 nm values の結果

```
$ nm values
(...)
0000000000401050 T _start
0000000000404018 D c
000000000040210c R ci0
0000000000402110 R ci1
0000000000402114 R ci2
0000000000404038 b completed.0
0000000000404010 W data_start
0000000000401090 t deregister_tm_clones
                 U exit@GLIBC_2.2.5
0000000000401130 t frame_dummy
000000000040122c T func
0000000000404040 B i0
0000000000404044 B i1
000000000040401c D i2
0000000000401136 T main
0000000000404048 B p0
0000000000404028 D p1
                 U printf@GLIBC_2.2.5
00000000004010c0 t register_tm_clones
0000000000404050 b si0
0000000000404054 b si1
0000000000404020 d si2
0000000000404030 D str
```

### リスト 1.16 strip values の実行結果

```
$ ls -l values
-rwxr-xr-x. 1 ec2-user ec2-user 25616 Apr  2 15:42 values*

$ file values
values: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, \
interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=f691308c805e0c81fd3ba87831d73b47f6222dba, \
for GNU/Linux 3.2.0, not stripped

$ strip values

$ ls -l values
-rwxr-xr-x. 1 ec2-user ec2-user 14696 Apr  2 15:56 values*

$ file values
values: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, \
interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=f691308c805e0c81fd3ba87831d73b47f6222dba, \
for GNU/Linux 3.2.0, stripped

$ nm values
nm: values: no symbols
```

- strip 後はファイルサイズが小さくなったのを確認できた。
- `file` コマンドからも、strip前は "not stripped" だったが strip後は "stripped" という表記に変わっている。
- `nm` コマンドでも strip 後は "no symbols" と表示され、シンボル情報（テーブル）が消えていることを確認できた。

### リスト 1.17 アプリケーション用のリンカ・スクリプト

書籍では `/usr/libdata/ldscripts/` 以下にある `elf_i386.x` の内容を元に解説している。
今回は Amazon Linux 2023 の binutils を使う。

試しに、gccコンパイル時に `-Wl,` で渡せるリンカ向けオプションとして以下のように `--verbose` をつけてみて、リンカの詳細出力を出してみたところ、どうも内部的に組み込まれた linker script を使うようになっているらしい。

(全部コピペしてみたが大分長い)

```
$ gcc -Wl,--verbose -o values values.o values_sub.o
GNU ld version 2.39-6.amzn2023.0.11
  Supported emulations:
   elf_x86_64
   elf32_x86_64
   elf_i386
   elf_iamcu
   i386pep
   i386pe
   elf64bpf
using internal linker script:
==================================================
/* Script for -z combreloc -z separate-code */
/* Copyright (C) 2014-2022 Free Software Foundation, Inc.
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */
OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64",
              "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)
SEARCH_DIR("=/usr/x86_64-amazon-linux/lib64"); SEARCH_DIR("=/usr/lib64"); SEARCH_DIR("=/usr/local/lib64"); SEARCH_DIR("=/lib64"); SEARCH_DIR("=/usr/x86_64-amazon-linux/lib"); SEARCH_DIR("=/usr/local/lib"); SEARCH_DIR("=/lib"); SEARCH_DIR("=/usr/lib");
SECTIONS
{
  PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x400000)); . = SEGMENT_START("text-segment", 0x400000) + SIZEOF_HEADERS;
  .interp         : { *(.interp) }
  .note.gnu.build-id  : { *(.note.gnu.build-id) }
  .hash           : { *(.hash) }
  .gnu.hash       : { *(.gnu.hash) }
  .dynsym         : { *(.dynsym) }
  .dynstr         : { *(.dynstr) }
  .gnu.version    : { *(.gnu.version) }
  .gnu.version_d  : { *(.gnu.version_d) }
  .gnu.version_r  : { *(.gnu.version_r) }
  .rela.dyn       :
    {
      *(.rela.init)
      *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
      *(.rela.fini)
      *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
      *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
      *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*)
      *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*)
      *(.rela.ctors)
      *(.rela.dtors)
      *(.rela.got)
      *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*)
      *(.rela.ldata .rela.ldata.* .rela.gnu.linkonce.l.*)
      *(.rela.lbss .rela.lbss.* .rela.gnu.linkonce.lb.*)
      *(.rela.lrodata .rela.lrodata.* .rela.gnu.linkonce.lr.*)
      *(.rela.ifunc)
    }
  .rela.plt       :
    {
      *(.rela.plt)
      PROVIDE_HIDDEN (__rela_iplt_start = .);
      *(.rela.iplt)
      PROVIDE_HIDDEN (__rela_iplt_end = .);
    }
  .relr.dyn : { *(.relr.dyn) }
  . = ALIGN(CONSTANT (MAXPAGESIZE));
  .init           :
  {
    KEEP (*(SORT_NONE(.init)))
  }
  .plt            : { *(.plt) *(.iplt) }
.plt.got        : { *(.plt.got) }
.plt.sec        : { *(.plt.sec) }
  .text           :
  {
    *(.text.unlikely .text.*_unlikely .text.unlikely.*)
    *(.text.exit .text.exit.*)
    *(.text.startup .text.startup.*)
    *(.text.hot .text.hot.*)
    *(SORT(.text.sorted.*))
    *(.text .stub .text.* .gnu.linkonce.t.*)
    /* .gnu.warning sections are handled specially by elf.em.  */
    *(.gnu.warning)
  }
  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  }
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  . = ALIGN(CONSTANT (MAXPAGESIZE));
  /* Adjust the address for the rodata segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = SEGMENT_START("rodata-segment", ALIGN(CONSTANT (MAXPAGESIZE)) + (. & (CONSTANT (MAXPAGESIZE) - 1)));
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  .rodata1        : { *(.rodata1) }
  .eh_frame_hdr   : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) }
  .eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) *(.eh_frame.*) }
  .gcc_except_table   : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) }
  .gnu_extab   : ONLY_IF_RO { *(.gnu_extab*) }
  /* These sections are generated by the Sun/Oracle C++ compiler.  */
  .exception_ranges   : ONLY_IF_RO { *(.exception_ranges*) }
  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
  /* Exception handling  */
  .eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) *(.eh_frame.*) }
  .gnu_extab      : ONLY_IF_RW { *(.gnu_extab) }
  .gcc_except_table   : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) }
  .exception_ranges   : ONLY_IF_RW { *(.exception_ranges*) }
  /* Thread Local Storage sections  */
  .tdata          :
   {
     PROVIDE_HIDDEN (__tdata_start = .);
     *(.tdata .tdata.* .gnu.linkonce.td.*)
   }
  .tbss           : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) }
  .preinit_array    :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  }
  .init_array    :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  }
  .fini_array    :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  }
  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  }
  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  }
  .jcr            : { KEEP (*(.jcr)) }
  .data.rel.ro : { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*) }
  .dynamic        : { *(.dynamic) }
  .got            : { *(.got) *(.igot) }
  . = DATA_SEGMENT_RELRO_END (SIZEOF (.got.plt) >= 24 ? 24 : 0, .);
  .got.plt        : { *(.got.plt) *(.igot.plt) }
  .data           :
  {
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
  }
  .data1          : { *(.data1) }
  _edata = .; PROVIDE (edata = .);
  . = .;
  __bss_start = .;
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.
      FIXME: Why do we need it? When there is no .bss section, we do not
      pad the .data section.  */
   . = ALIGN(. != 0 ? 64 / 8 : 1);
  }
  .lbss   :
  {
    *(.dynlbss)
    *(.lbss .lbss.* .gnu.linkonce.lb.*)
    *(LARGE_COMMON)
  }
  . = ALIGN(64 / 8);
  . = SEGMENT_START("ldata-segment", .);
  .lrodata   ALIGN(CONSTANT (MAXPAGESIZE)) + (. & (CONSTANT (MAXPAGESIZE) - 1)) :
  {
    *(.lrodata .lrodata.* .gnu.linkonce.lr.*)
  }
  .ldata   ALIGN(CONSTANT (MAXPAGESIZE)) + (. & (CONSTANT (MAXPAGESIZE) - 1)) :
  {
    *(.ldata .ldata.* .gnu.linkonce.l.*)
    . = ALIGN(. != 0 ? 64 / 8 : 1);
  }
  . = ALIGN(64 / 8);
  _end = .; PROVIDE (end = .);
  . = DATA_SEGMENT_END (.);
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  .gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1.  */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions.  */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2.  */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2.  */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions.  */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* DWARF 3.  */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  /* DWARF 5.  */
  .debug_addr     0 : { *(.debug_addr) }
  .debug_line_str 0 : { *(.debug_line_str) }
  .debug_loclists 0 : { *(.debug_loclists) }
  .debug_macro    0 : { *(.debug_macro) }
  .debug_names    0 : { *(.debug_names) }
  .debug_rnglists 0 : { *(.debug_rnglists) }
  .debug_str_offsets 0 : { *(.debug_str_offsets) }
  .debug_sup      0 : { *(.debug_sup) }
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}


==================================================
/usr/bin/ld: mode elf_x86_64
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crt1.o succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crt1.o
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crti.o succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crti.o
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/crtbegin.o succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/crtbegin.o
attempt to open values.o succeeded
values.o
attempt to open values_sub.o succeeded
values_sub.o
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libgcc.so failed
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libgcc.a succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/libgcc.a
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libgcc_s.so succeeded
opened script file /usr/lib/gcc/x86_64-amazon-linux/11/libgcc_s.so
/usr/lib/gcc/x86_64-amazon-linux/11/libgcc_s.so
opened script file /usr/lib/gcc/x86_64-amazon-linux/11/libgcc_s.so
attempt to open /lib64/libgcc_s.so.1 succeeded
/lib64/libgcc_s.so.1
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libgcc.a succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/libgcc.a
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libc.so failed
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libc.a failed
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/libc.so succeeded
opened script file /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/libc.so
/usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/libc.so
opened script file /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/libc.so
attempt to open /lib64/libc.so.6 succeeded
/lib64/libc.so.6
attempt to open /usr/lib64/libc_nonshared.a succeeded
/usr/lib64/libc_nonshared.a
attempt to open /lib64/ld-linux-x86-64.so.2 succeeded
/lib64/ld-linux-x86-64.so.2
/usr/lib64/libc_nonshared.a
/lib64/ld-linux-x86-64.so.2
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libgcc.so failed
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libgcc.a succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/libgcc.a
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libgcc_s.so succeeded
opened script file /usr/lib/gcc/x86_64-amazon-linux/11/libgcc_s.so
/usr/lib/gcc/x86_64-amazon-linux/11/libgcc_s.so
opened script file /usr/lib/gcc/x86_64-amazon-linux/11/libgcc_s.so
attempt to open /lib64/libgcc_s.so.1 succeeded
/lib64/libgcc_s.so.1
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/libgcc.a succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/libgcc.a
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/crtend.o succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/crtend.o
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crtn.o succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crtn.o
ld-linux-x86-64.so.2 needed by /lib64/libc.so.6
found ld-linux-x86-64.so.2 at /lib64/ld-linux-x86-64.so.2
```

- リスト 1.19 で紹介されている ENTRY の指定をこちらでも確認できた。
  - `ENTRY(_start)`
- `crt1.o, crti.o, crtbegin.o, crtn.o, crtend.o` などのスタートアップ・ルーチンをリンクしている様子をこちらでも確認できた。

