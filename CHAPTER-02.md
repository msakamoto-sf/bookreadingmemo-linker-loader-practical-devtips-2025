# 第2賞 ELF形式の解析

Amazon Linux 2023 における ELF 関連ヘッダーファイル:

```
$ find /usr/include -name elf*.h
/usr/include/sys/elf.h  (-> 非推奨のヘッダーファイル)
/usr/include/linux/elf-em.h (-> EM_xxx の定義値の補完)
/usr/include/linux/elf-fdpic.h (不明)
/usr/include/linux/elf.h (-> Linux用に絞ったELF定義ヘッダーファイル？)
/usr/include/bits/elfclass.h
/usr/include/elf.h (-> フルスペックのELF定義ヘッダーファイル)
/usr/include/elfutils/elf-knowledge.h
```

書籍では `e_ident[]` のインデックス値が `sys/elf_common.h` で定数定義されていると書かれている。
Amazon Linux 2023 においては `/usr/include/elf.h` に定義されていた。
抜粋:

```
(...)
     99 /* Fields in the e_ident array.  The EI_* macros are indices into the
    100    array.  The macros under each EI_* macro are the values the byte
    101    may have.  */
    102
    103 #define EI_MAG0         0               /* File identification byte 0 index */
    104 #define ELFMAG0         0x7f            /* Magic number byte 0 */
    105
    106 #define EI_MAG1         1               /* File identification byte 1 index */
    107 #define ELFMAG1         'E'             /* Magic number byte 1 */
    108
    109 #define EI_MAG2         2               /* File identification byte 2 index */
    110 #define ELFMAG2         'L'             /* Magic number byte 2 */
    111
    112 #define EI_MAG3         3               /* File identification byte 3 index */
    113 #define ELFMAG3         'F'             /* Magic number byte 3 */
    114
    115 /* Conglomeration of the identification bytes, for easy testing as a word.  */
    116 #define ELFMAG          "\177ELF"
    117 #define SELFMAG         4
    118
    119 #define EI_CLASS        4               /* File class byte index */
    120 #define ELFCLASSNONE    0               /* Invalid class */
    121 #define ELFCLASS32      1               /* 32-bit objects */
    122 #define ELFCLASS64      2               /* 64-bit objects */
    123 #define ELFCLASSNUM     3
(...)
```

また書籍においては `elf32.h` と `elf64.h` で分かれていたが、こちらでは `elf.h` にまとめられていた。

### リスト 2.1, 2.2 ELFファイル解析用サンプル

- [elfsamp.c](CHAPTER-02/elfsamp.c)
- [main.c](CHAPTER-02/main.c)

コンパイルと実行:

```
$ gcc -c elfsamp.c -Wall
$ gcc -c main.c -Wall
$ gcc elfsamp.o main.o -Wall -o elfsamp
$ ./elfsamp
sample
```

### リスト 2.4 readelf -h elfsamp.o の結果

```
$ readelf -h elfsamp.o
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          1424 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         14
  Section header string table index: 13
```

64bit用になっていること以外は、書籍の内容とほぼ同じ。

### リスト 2.7 readelf -S elfsamp.o の結果

```
$ readelf -S elfsamp.o
There are 14 section headers, starting at offset 0x590:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       0000000000000096  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  00000380
       0000000000000168  0000000000000018   I      11     1     8
  [ 3] .data             PROGBITS         0000000000000000  000000d8
       0000000000000008  0000000000000000  WA       0     0     4
  [ 4] .bss              NOBITS           0000000000000000  000000e0
       0000000000000008  0000000000000000  WA       0     0     4
  [ 5] .rodata           PROGBITS         0000000000000000  000000e0
       000000000000000c  0000000000000000   A       0     0     4
  [ 6] .comment          PROGBITS         0000000000000000  000000ec
       000000000000002f  0000000000000001  MS       0     0     1
  [ 7] .note.GNU-stack   PROGBITS         0000000000000000  0000011b
       0000000000000000  0000000000000000           0     0     1
  [ 8] .note.gnu.pr[...] NOTE             0000000000000000  00000120
       0000000000000030  0000000000000000   A       0     0     8
  [ 9] .eh_frame         PROGBITS         0000000000000000  00000150
       0000000000000058  0000000000000000   A       0     0     8
  [10] .rela.eh_frame    RELA             0000000000000000  000004e8
       0000000000000030  0000000000000018   I      11     9     8
  [11] .symtab           SYMTAB           0000000000000000  000001a8
       0000000000000198  0000000000000018          12     9     8
  [12] .strtab           STRTAB           0000000000000000  00000340
       000000000000003d  0000000000000000           0     0     1
  [13] .shstrtab         STRTAB           0000000000000000  00000518
       0000000000000074  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), l (large), p (processor specific)
```

セクション数と表示フォーマットが書籍とは異なるが、内容としては概ね書籍通りと思われる。

### リスト 2.8 readelf -x 8 elfsamp.o の結果

書籍では 8 番目のセクションが `.shstrtab` セクションになっていた。
今回は 13 番目が `.shstrtab となっているため、そちらをダンプしてみる。

```
$ readelf -x 13 elfsamp.o

Hex dump of section '.shstrtab':
  0x00000000 002e7379 6d746162 002e7374 72746162 ..symtab..strtab
  0x00000010 002e7368 73747274 6162002e 72656c61 ..shstrtab..rela
  0x00000020 2e746578 74002e64 61746100 2e627373 .text..data..bss
  0x00000030 002e726f 64617461 002e636f 6d6d656e ..rodata..commen
  0x00000040 74002e6e 6f74652e 474e552d 73746163 t..note.GNU-stac
  0x00000050 6b002e6e 6f74652e 676e752e 70726f70 k..note.gnu.prop
  0x00000060 65727479 002e7265 6c612e65 685f6672 erty..rela.eh_fr
  0x00000070 616d6500                            ame.
```

書籍と同様、セクション名が収められていることを確認できた。
ちなみに変数名などは 12 番目の `.strtab` に収められていた。

```
$ readelf -x 12 elfsamp.o

Hex dump of section '.strtab':
  0x00000000 00656c66 73616d70 2e630073 31007332 .elfsamp.c.s1.s2
  0x00000010 00736675 6e630067 31006732 00673300 .sfunc.g1.g2.g3.
  0x00000020 6566756e 63006533 00676675 6e630073 efunc.e3.gfunc.s
  0x00000030 74646f75 74006677 72697465 00       tdout.fwrite.
```

### リスト 2.9 セクション名一覧の表示の例

- [readsec.c](CHAPTER-02/readsec.c)
- 書籍からの変更点:
   1. 今回の検証環境では `IS_ELF` マクロが `/usr/include/` 以下で見つからなかったため、ELFファイルかどうかの判定IF文を丸ごとコメントアウト。
   2. 今回の検証環境では　`Elf_` で参照している32bit/64bitを自動切り替えしているような構造体定義が `/usr/include/` 以下で見つからなかったため、`Elf64_` にハードコード。
   3. セクション名となる文字列を `printf("%s\n", ...)` している箇所を、`printf("[%s]\n", ...)` と変更して出力されている文字列範囲を分かりやすくした。

コンパイルと実行例:

```
$ gcc -c readsec.c -Wall
$ gcc readsec.c -Wall -o readsec
$ ./readsec elfsamp.o
[]
[.text]
[.rela.text]
[.data]
[.bss]
[.rodata]
[.comment]
[.note.GNU-stack]
[.note.gnu.property]
[.eh_frame]
[.rela.eh_frame]
[.symtab]
[.strtab]
[.shstrtab]
```

リスト2.7と同様に `elfsamp.o` のセクション名を表示することができた。

### リスト 2.11 readelf -l elfsamp の結果

```
$ readelf -l elfsamp

Elf file type is EXEC (Executable file)
Entry point 0x401050
There are 13 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000400040 0x0000000000400040
                 0x00000000000002d8 0x00000000000002d8  R      0x8
  INTERP         0x0000000000000318 0x0000000000400318 0x0000000000400318
                 0x000000000000001c 0x000000000000001c  R      0x1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x0000000000000620 0x0000000000000620  R      0x1000
  LOAD           0x0000000000001000 0x0000000000401000 0x0000000000401000
                 0x00000000000001f9 0x00000000000001f9  R E    0x1000
  LOAD           0x0000000000002000 0x0000000000402000 0x0000000000402000
                 0x0000000000000148 0x0000000000000148  R      0x1000
  LOAD           0x0000000000002de8 0x0000000000403de8 0x0000000000403de8
                 0x0000000000000234 0x0000000000000250  RW     0x1000
  DYNAMIC        0x0000000000002df8 0x0000000000403df8 0x0000000000403df8
                 0x00000000000001d0 0x00000000000001d0  RW     0x8
  NOTE           0x0000000000000338 0x0000000000400338 0x0000000000400338
                 0x0000000000000040 0x0000000000000040  R      0x8
  NOTE           0x0000000000000378 0x0000000000400378 0x0000000000400378
                 0x0000000000000044 0x0000000000000044  R      0x4
  GNU_PROPERTY   0x0000000000000338 0x0000000000400338 0x0000000000400338
                 0x0000000000000040 0x0000000000000040  R      0x8
  GNU_EH_FRAME   0x000000000000201c 0x000000000040201c 0x000000000040201c
                 0x0000000000000044 0x0000000000000044  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x0000000000002de8 0x0000000000403de8 0x0000000000403de8
                 0x0000000000000218 0x0000000000000218  R      0x1

 Section to Segment mapping:
  Segment Sections...
   00
   01     .interp
   02     .interp .note.gnu.property .note.gnu.build-id .note.ABI-tag .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt
   03     .init .plt .text .fini
   04     .rodata .eh_frame_hdr .eh_frame
   05     .init_array .fini_array .dynamic .got .got.plt .data .bss
   06     .dynamic
   07     .note.gnu.property
   08     .note.gnu.build-id .note.ABI-tag
   09     .note.gnu.property
   10     .eh_frame_hdr
   11
   12     .init_array .fini_array .dynamic .got
```

アドレスが64bit表記となったためか、書籍では1行ずつだったのが、2行に分かれて表示されるようになった。
`LOAD` Type の数が異なるものの、内容としては書籍に書かれているとおりと思われる。

### リスト 2.12 自身のELFヘッダを読み込む例

- [elfread.c](CHAPTER-02/elfread.c)
- 書籍からの変更点:
  - 今回の環境だと `0x400000` から割り当てられる状況だった。
  - そのためまず `main()` のアドレスを page size (= `getpagesize(2)` ) で丸めた。これは結果として2番目の `LOAD` segment の先頭アドレスになる。実際はその前に `INTERP` 兼 `LOAD` segment が page size 分入っているので、それを更に引くことで、ELFヘッダーの先頭アドレスとなった。

コンパイルと実行:

```
$ gcc -o elfread elfread.c -Wall
$ ./elfread
address of main() = [0x401136]
getpagesize(2) =[0x1000]
address of main() % 4KB page = [0x136]
nearest 0x400000
heading 4 byte in ELF header = [0x7fELF]

$ readelf -h elfread
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x401050
  Start of program headers:          64 (bytes into file)
  Start of section headers:          23184 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         13
  Size of section headers:           64 (bytes)
  Number of section headers:         31
  Section header string table index: 30

$ readelf -l elfread

Elf file type is EXEC (Executable file)
Entry point 0x401050
There are 13 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000400040 0x0000000000400040
                 0x00000000000002d8 0x00000000000002d8  R      0x8
  INTERP         0x0000000000000318 0x0000000000400318 0x0000000000400318
                 0x000000000000001c 0x000000000000001c  R      0x1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x00000000000005e8 0x00000000000005e8  R      0x1000
  LOAD           0x0000000000001000 0x0000000000401000 0x0000000000401000
                 0x0000000000000231 0x0000000000000231  R E    0x1000
(...)
```

書籍の通りにELFヘッダーの先頭4バイトを表示することができた。

### リスト 2.14 readelf -s elfsamp.o の結果(symbol table)

```
$ readelf -s elfsamp.o

Symbol table '.symtab' contains 17 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS elfsamp.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 .bss
     5: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    4 s1
     6: 0000000000000000     4 OBJECT  LOCAL  DEFAULT    3 s2
     7: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     8: 0000000000000000    71 FUNC    LOCAL  DEFAULT    1 sfunc
     9: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    4 g1
    10: 0000000000000004     4 OBJECT  GLOBAL DEFAULT    3 g2
    11: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    5 g3
    12: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND efunc
    13: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND e3
    14: 0000000000000047    79 FUNC    GLOBAL DEFAULT    1 gfunc
    15: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND stdout
    16: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND fwrite
```

表示される具体的な値は書籍と異なっているが、その意味については書籍と同じと思われる。

### リスト 2.14 readelf -r elfsamp.o の結果(再配置情報, relocation)

```
$ readelf -r elfsamp.o

Relocation section '.rela.text' at offset 0x380 contains 15 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
00000000000a  000c00000004 R_X86_64_PLT32    0000000000000000 efunc - 4
000000000010  000300000002 R_X86_64_PC32     0000000000000000 .data - 8
00000000001a  000300000002 R_X86_64_PC32     0000000000000000 .data - 4
000000000020  000400000002 R_X86_64_PC32     0000000000000000 .bss + 0
000000000026  000a00000002 R_X86_64_PC32     0000000000000004 g2 - 8
000000000030  000a00000002 R_X86_64_PC32     0000000000000004 g2 - 4
000000000036  000900000002 R_X86_64_PC32     0000000000000000 g1 - 4
00000000003c  000d00000002 R_X86_64_PC32     0000000000000000 e3 - 8
00000000005b  000c00000004 R_X86_64_PLT32    0000000000000000 efunc - 4
000000000061  000d00000002 R_X86_64_PC32     0000000000000000 e3 - 8
00000000006b  000d00000002 R_X86_64_PC32     0000000000000000 e3 - 4
000000000071  000900000002 R_X86_64_PC32     0000000000000000 g1 - 4
000000000078  000f00000002 R_X86_64_PC32     0000000000000000 stdout - 4
00000000008a  00070000000a R_X86_64_32       0000000000000000 .rodata + 4
00000000008f  001000000004 R_X86_64_PLT32    0000000000000000 fwrite - 4

Relocation section '.rela.eh_frame' at offset 0x4e8 contains 2 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000020  000200000002 R_X86_64_PC32     0000000000000000 .text + 0
000000000040  000200000002 R_X86_64_PC32     0000000000000000 .text + 47
```

表示される具体的な値は書籍と異なっているが、その意味については書籍と同じと思われる。


