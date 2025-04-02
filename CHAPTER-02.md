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
