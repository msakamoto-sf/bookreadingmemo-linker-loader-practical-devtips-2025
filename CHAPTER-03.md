# 第3章 ライブラリ・アーカイブの解析

### リスト 3.1 アーカイブ・ヘッダの定義

今回の環境では `/usr/include/ar.h` を参照した。
このファイルは `glibc-headers` パッケージが提供していた。

```
 /* Archive files start with the ARMAG identifying string.  Then follows a
    `struct ar_hdr', and as many bytes of member file data as its `ar_size'
    member indicates, for each member file.  */

 #define ARMAG   "!<arch>\n"     /* String that begins an archive file.  */
 #define SARMAG  8               /* Size of that string.  */

 #define ARFMAG  "`\n"           /* String in ar_fmag at end of each header.  */

 __BEGIN_DECLS

 struct ar_hdr
   {
     char ar_name[16];           /* Member file name, sometimes / terminated. */
     char ar_date[12];           /* File date, decimal seconds since Epoch.  */
     char ar_uid[6], ar_gid[6];  /* User and group IDs, in ASCII decimal.  */
     char ar_mode[8];            /* File mode, in ASCII octal.  */
     char ar_size[10];           /* File size, in ASCII decimal.  */
     char ar_fmag[2];            /* Always contains ARFMAG.  */
   };

 __END_DECLS
```

書籍の内容と同等のヘッダ定義内容となっていた。

### リスト 3.3 GNU binutils のアーカイブ

binutils の ar コマンドを使ってアーカイブを作ってみる。

```
$ echo "ABCD" > f12345678901234.txt
$ echo "ABCDE" > f123456789012345.txt
$ echo "ABCDEF" > f1234567890123456.txt
$ echo "ABCDEFG" > f12345678901234567.txt

$ /usr/bin/ar ruc gnuar.a f*.txt
```

内容を表示してみる。

```
$ cat gnuar.a
!<arch>
//                                              90        `
f12345678901234.txt/
f123456789012345.txt/
f1234567890123456.txt/
f12345678901234567.txt/
/0              1743934054  1000  1000  100644  5         `
ABCD

/21             1743934058  1000  1000  100644  6         `
ABCDE
/43             1743934061  1000  1000  100644  7         `
ABCDEF

/66             1743934065  1000  1000  100644  8         `
ABCDEFG
```

書籍の通り、最初のアーカイブエントリーとして `//` という名前でファイル名が格納されたエントリーが配置されている。
その後に `ar_hdr` とそれに続くファイル本体が格納されていることを確認できる。

hexdump結果：

```
$ /usr/bin/hexdump -C -v gnuar.a
00000000  21 3c 61 72 63 68 3e 0a  2f 2f 20 20 20 20 20 20  |!<arch>.//      |
00000010  20 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20  |                |
00000020  20 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20  |                |
00000030  20 20 20 20 20 20 20 20  39 30 20 20 20 20 20 20  |        90      |
00000040  20 20 60 0a 66 31 32 33  34 35 36 37 38 39 30 31  |  `.f12345678901|
00000050  32 33 34 2e 74 78 74 2f  0a 66 31 32 33 34 35 36  |234.txt/.f123456|
00000060  37 38 39 30 31 32 33 34  35 2e 74 78 74 2f 0a 66  |789012345.txt/.f|
00000070  31 32 33 34 35 36 37 38  39 30 31 32 33 34 35 36  |1234567890123456|
00000080  2e 74 78 74 2f 0a 66 31  32 33 34 35 36 37 38 39  |.txt/.f123456789|
00000090  30 31 32 33 34 35 36 37  2e 74 78 74 2f 0a 2f 30  |01234567.txt/./0|
000000a0  20 20 20 20 20 20 20 20  20 20 20 20 20 20 31 37  |              17|
000000b0  34 33 39 33 34 30 35 34  20 20 31 30 30 30 20 20  |43934054  1000  |
000000c0  31 30 30 30 20 20 31 30  30 36 34 34 20 20 35 20  |1000  100644  5 |
000000d0  20 20 20 20 20 20 20 20  60 0a 41 42 43 44 0a 0a  |        `.ABCD..|
000000e0  2f 32 31 20 20 20 20 20  20 20 20 20 20 20 20 20  |/21             |
000000f0  31 37 34 33 39 33 34 30  35 38 20 20 31 30 30 30  |1743934058  1000|
00000100  20 20 31 30 30 30 20 20  31 30 30 36 34 34 20 20  |  1000  100644  |
00000110  36 20 20 20 20 20 20 20  20 20 60 0a 41 42 43 44  |6         `.ABCD|
00000120  45 0a 2f 34 33 20 20 20  20 20 20 20 20 20 20 20  |E./43           |
00000130  20 20 31 37 34 33 39 33  34 30 36 31 20 20 31 30  |  1743934061  10|
00000140  30 30 20 20 31 30 30 30  20 20 31 30 30 36 34 34  |00  1000  100644|
00000150  20 20 37 20 20 20 20 20  20 20 20 20 60 0a 41 42  |  7         `.AB|
00000160  43 44 45 46 0a 0a 2f 36  36 20 20 20 20 20 20 20  |CDEF../66       |
00000170  20 20 20 20 20 20 31 37  34 33 39 33 34 30 36 35  |      1743934065|
00000180  20 20 31 30 30 30 20 20  31 30 30 30 20 20 31 30  |  1000  1000  10|
00000190  30 36 34 34 20 20 38 20  20 20 20 20 20 20 20 20  |0644  8         |
000001a0  60 0a 41 42 43 44 45 46  47 0a                    |`.ABCDEFG.|
000001aa
```

- ヘッダーや本体データが 0x0a (LF) 区切りで格納されていることを確認できる。
- ファイル名など余白は 0x20 で padding されていることを確認できる。
- sizeが奇数の場合、LFが1つ余分に追加されて align 調整されていることを確認できる。

### リスト 3.4 アーカイブ・ファイルのダンプ

- [readname.c](CHAPTER-03/readname.c)
  - 書籍からの変更点はなし

コンパイルと実行:

```
$ gcc readname.c -Wall -o readname
( printf() で warning が表示されるがスルー )

$ ./readname gnuar.a
string table found.
f12345678901234.txt     5
f123456789012345.txt    6
f1234567890123456.txt   7
f12345678901234567.txt  8
```

書籍と同様にアーカイブファイル内のファイル名とサイズを表示することができた。

### リスト 3.5 アーカイブ・ファイルの解析

- [ardump.c](CHAPTER-03/ardump.c)
- 書籍からの変更点
  - 日本語コメントの文字コードをEUC-JPからUTF-8に変更
  - `IS_ELF` マクロの処理をコメントアウト
  - ELF定義を64bit用に変更

コンパイルと実行:

```
$ gcc -c ardump.c -Wall
( printf() で warning が表示されるがスルー )

$ gcc ardump.o -Wall -o ardump
```

### リスト 3.6 オブジェクト・ファイルの解析例

ardump.o の解析:

```
$ ./ardump ardump.o
Sections:
        [0]
        [1]     .text
        [2]     .rela.text
        [3]     .data
        [4]     .bss
        [5]     .rodata
        [6]     .comment
        [7]     .note.GNU-stack
        [8]     .note.gnu.property
        [9]     .eh_frame
        [10]    .rela.eh_frame
        [11]    .symtab
        [12]    .strtab
        [13]    .shstrtab
Segments:
Symbols:
        [1]     4       0       ardump.c
        [4]     2       1337    elfdump
        [5]     0       0       stderr
        [6]     0       0       fprintf
        [7]     0       0       puts
        [8]     0       0       printf
        [9]     0       0       strcmp
        [10]    0       0       putchar
        [11]    2       658     main
        [12]    0       0       open
        [13]    0       0       exit
        [14]    0       0       fstat
        [15]    0       0       mmap
        [16]    0       0       memcmp
        [17]    0       0       fwrite
        [18]    0       0       atoi
        [19]    0       0       strchr
        [20]    0       0       munmap
        [21]    0       0       close
Relocations:
        [0]     5       stderr
        [2]     6       fprintf
        [3]     5       stderr
        [5]     6       fprintf
        [7]     7       puts
        [9]     8       printf
        [11]    9       strcmp
        [13]    7       puts
        [15]    8       printf
        [17]    8       printf
        [18]    10      putchar
        [20]    7       puts
        [22]    8       printf
        [24]    7       puts
        [26]    8       printf
        [27]    12      open
        [28]    13      exit
        [29]    14      fstat
        [30]    15      mmap
        [32]    16      memcmp
        [33]    5       stderr
        [35]    17      fwrite
        [36]    18      atoi
        [38]    16      memcmp
        [39]    5       stderr
        [41]    17      fwrite
        [43]    16      memcmp
        [44]    5       stderr
        [46]    17      fwrite
        [47]    5       stderr
        [49]    17      fwrite
        [50]    13      exit
        [51]    18      atoi
        [52]    19      strchr
        [53]    5       stderr
        [55]    6       fprintf
        [56]    20      munmap
        [57]    21      close
        [58]    13      exit
```

書籍と同様、ELFファイルの内容を正常に表示できている。

### リスト 3.7 ライブラリ・ファイルの解析例

`binutils-devel` パッケージの提供する `/usr/lib64/libbfd.a` ファイルを解析してみる：

```
$ ./ardump /usr/lib64/libbfd.a
```

結果は省略するが、書籍と同様、ライブラリファイルの中のオブジェクトファイルごとにELFの内容を表示できた。

### リスト 3.8 - 3.12 サンプルファイル

- [main.c](CHAPTER-03/main.c)
- [samp1.c](CHAPTER-03/samp1.c)
- [samp2.c](CHAPTER-03/samp2.c)
- [samp3.c](CHAPTER-03/samp3.c)
- [samp4.c](CHAPTER-03/samp4.c)

コンパイル:

```
$ gcc -c samp1.c -Wall
$ gcc -c samp2.c -Wall
$ gcc -c samp3.c -Wall
$ gcc -c samp4.c -Wall
$ ar ruc libsamp.a samp?.o
$ gcc main.c samp?.o -Wall -o samp_obj
```

オブジェクトファイルを直接合体させた `samp_obj` の解析結果:

```
$ ./ardump samp_obj | grep samp
        [44]    4       0       samp1.c
        [45]    4       0       samp2.c
        [46]    4       0       samp3.c
        [47]    4       0       samp4.c
        [57]    2       17      samp31
        [59]    2       7       samp41
        [61]    2       17      samp11
        [63]    2       7       samp21
        [67]    2       7       samp32
        [73]    2       7       samp42
        [76]    2       7       samp12
        [78]    2       7       samp22
```

samp1.o - samp4.o まですべて合体していることを確認できた。

アーカイブファイルを合体させた `samp_ar` の作成と解析結果:

```
$ gcc main.c -Wall -L. -lsamp -o samp_ar

$ ./ardump samp_ar | grep samp
        [44]    4       0       samp1.c
        [45]    4       0       samp2.c
        [57]    2       17      samp11
        [59]    2       7       samp21
        [70]    2       7       samp12
        [72]    2       7       samp22
```

書籍と同様、参照されていない samp3.o, samp4.o が含まれていないことを確認できた。

### リスト 3.13 サンプル・ファイル2の修正

samp2.c の `samp22()` を以下のように修正し、samp3.c 中の `samp31()` を呼び出すようにする。

```
void samp22()
{
    samp31();
}
```

コンパイルとアーカイブファイルの更新:

```
$ gcc -c samp2.c -Wall
$ ar ruc libsamp.a samp2.o
```

オブジェクトファイルを直接合体させた `samp_obj` の作成と解析結果:

```
$ gcc main.c samp?.o -Wall -o samp_obj

$ ./ardump samp_obj | grep samp
        [44]    4       0       samp1.c
        [45]    4       0       samp2.c
        [46]    4       0       samp3.c
        [47]    4       0       samp4.c
        [57]    2       17      samp31
        [59]    2       7       samp41
        [61]    2       17      samp11
        [63]    2       7       samp21
        [67]    2       7       samp32
        [73]    2       7       samp42
        [76]    2       7       samp12
        [78]    2       17      samp22
```

修正前と同様、samp1.o - samp4.o まですべて合体していることを確認できた。

アーカイブファイルを合体させた `samp_ar` の作成と解析結果:

```
$ gcc main.c -Wall -L. -lsamp -o samp_ar

$ ./ardump samp_ar | grep samp
        [44]    4       0       samp1.c
        [45]    4       0       samp2.c
        [46]    4       0       samp3.c
        [47]    4       0       samp4.c
        [57]    2       17      samp31
        [59]    2       7       samp41
        [61]    2       17      samp11
        [63]    2       7       samp21
        [67]    2       7       samp32
        [73]    2       7       samp42
        [76]    2       7       samp12
        [78]    2       17      samp22
```

書籍と同様、samp3.o, samp4.o も含まれていることを確認できた。

