# 第4章 実験 - リンカで遊んでみよう

### リスト 4.1 配列とポインタ

- [pointer.c](CHAPTER-04/pointer.c)
- 書籍化からの変更点:
  - ポインタ表示を `%08x` からより汎用的なポインタアドレス表記ができる `%p` に変更。それに伴い `(unsigned int)` へのキャストも削除。

コンパイルと実行:

```
$ gcc pointer.c -Wall -o pointer

$ ./pointer
 s1 = 0x404010
&s1 = 0x404010
 s2 = 0x402010
&s2 = 0x404020
sizeof(s1) = 15
sizeof(s2) = 8
```

書籍とアドレス値は異なるが、s1 が `0x404010` に配置され、s2 自身は `0x404020` に配置されそのデータ（＝アドレス）は `0x402010` を指しているという関係性は確認できた。

nmコマンドと `objdump -h` によるsection情報の表示を確認してみる。

```
$ nm pointer
(...)
0000000000404010 D s1
0000000000404020 D s2

$ objdump -h pointer

pointer:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
(...)
 15 .rodata       0000006d  0000000000402000  0000000000402000  00002000  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
(...)
 23 .data         00000020  0000000000404008  0000000000404008  00003008  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 24 .bss          00000008  0000000000404028  0000000000404028  00003028  2**0
                  ALLOC
```

書籍と同様、s1, s2 は `.data` セクションに配置されており、s2 が指すアドレスは `.rodata` セクション内にあることを確認できた。

### リスト 4.2, 4.3 変数に値を代入する, const定義した変数に値を代入する

- [const_sub.c](CHAPTER-04/const_sub.c)
- [const.c](CHAPTER-04/const.c)
- 書籍からの変更点は無し。

`const_sub.c` のコンパイルとnmコマンド実行結果:

```
$ gcc -c const_sub.c -Wall
$ nm const_sub.o
0000000000000000 R a
```

書籍と同様 `a` が read-only 領域に配置されたことを確認できた。

`const.c` のコンパイルとリンク、nmコマンド実行結果:

```
$ gcc -c const.c -Wall
$ gcc const.o const_sub.o -Wall -o const
$ nm const
(...)
0000000000402014 R a
(...)
```

書籍と同様、実行ファイルにおいても `a` が read-only 領域に配置されたことを確認できた。

`objdump -h const` の結果と、`const` の実行結果:

```
$ objdump -h const
(...)
 15 .rodata       00000018  0000000000402000  0000000000402000  00002000  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
(...)

$ ./const
Segmentation fault (core dumped)
```

書籍とアドレス番地は異なるが、nmコマンドで `a` が `0x402014` に配置され、それが `.rodata` セクションのエリアであることは書籍と同様に確認できた。
実行すると read-only 領域への書き込みで segfault したことを確認できた。

### リスト 4.4 リンカ・スクリプトの変更

書籍では `/usr/libdata/ldscripts/elf_i386.x` をコピーして修正している。
今回の環境は Linux のため上記ファイルは存在しない。
一方で `ld` コマンドに `--verbose` オプションを指定すると、使用しているデフォルトのリンカスクリプトを表示してくれる。
これを使ってデフォルトのリンカスクリプトをファイルに保存し、それをカスタマイズしてみる。

まず gcc で `-Wl,--verbose` オプションを指定してリンカに `--verbose` オプションを認識させる。

```
$ gcc const.o const_sub.o -Wall -Wl,--verbose -o const
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
(...)
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}


==================================================
/usr/bin/ld: mode elf_x86_64
attempt to open /usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crt1.o succeeded
/usr/lib/gcc/x86_64-amazon-linux/11/../../../../lib64/crt1.o
(...)
```

ここで `====...====` で囲まれた部分がデフォルトで使用している内部のリンカスクリプトになる。
これを　`const-linker-script-default` という名前で保存したうえで、さらに今回用に `const-linker-script-4.4` というファイル名にコピーする。

`const-linker-script-default` 側では `.rodata` および `.data` セクションに関して以下のような指定があった。

```
(...)
  . = SEGMENT_START("rodata-segment", ALIGN(CONSTANT (MAXPAGESIZE)) + (. & (CONSTANT (MAXPAGESIZE) - 1)));
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  .rodata1        : { *(.rodata1) }
  .eh_frame_hdr   : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) }
(...)
  .data           :
  {
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
  }
  .data1          : { *(.data1) }
(...)
```

ここで `.rodata` および `.rodata1` セクションの指定を、 `.data` セクション側に移動する。
具体的には `const-linker-script-4.4` を以下のようにカスタマイズする。

```
(...)
  . = SEGMENT_START("rodata-segment", ALIGN(CONSTANT (MAXPAGESIZE)) + (. & (CONSTANT (MAXPAGESIZE) - 1)));
  /* 元の .rodata, .rodata1 セクションの指定を削除 */
  .eh_frame_hdr   : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) }
(...)
  .data           :
  {
    *(.data .data.* .gnu.linkonce.d.*)
    /* .rodata, .rodata1 セクションに集めていたセクションを .data に移動 */
    *(.rodata .rodata.* .gnu.linkonce.r.*)
    *(.rodata1)
    SORT(CONSTRUCTORS)
  }
(...)
```

gcc の `-T` オプションを使って、上記の通りカスタマイズした `const-linker-script-4.4` をリンカ・スクリプトとして指定して `noconst` バイナリをビルド:

```
$ gcc const.o const_sub.o -Wall -T const-linker-script-4.4 -o noconst
```

nm コマンドの実行結果から、書き込み対象の `a` のフラグが `D` と表示され、初期化済みデータとして扱われていることを確認:

```
$ nm noconst
(...)
0000000000401040 T _start
000000000040401c D a
0000000000404020 b completed.0
(...)
```

objdump の実行結果から `a` が配置されるアドレス `0x40401c` が `.data` セクションの範囲に含まれ、書き込み可能となっていることを確認:

```
$ objdump -h noconst

noconst:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
(...)
 22 .data         00000018  0000000000404008  0000000000404008  00003008  2**3
                  CONTENTS, ALLOC, LOAD, DATA
(...)
```

実行してみると、無事 `a` への書き込みが成功した。

```
$ ./noconst
2
```

### リスト 4.5 objcopy を使ったシンボル・タイプの変換

`const_sub.o` のシンボルを確認し、`a` がグローバル扱いであることを確認:

```
$ nm const_sub.o
0000000000000000 R a
```

objcopy コマンドでシンボルをローカル扱いに変更し、nmコマンドの結果からもそうなっていることを確認:

```
$ objcopy --localize-symbol=a const_sub.o const_sub_local.o

$ nm const_sub_local.o
0000000000000000 r a
```

リンクしてみると、`a` がグローバル扱いになっていないためシンボルが見つからずリンクエラーになることを確認:

```
$ gcc const.o const_sub_local.o -Wall -o const_local
/usr/bin/ld: const.o: in function `main':
const.c:(.text+0x6): undefined reference to `a'
/usr/bin/ld: const.c:(.text+0x10): undefined reference to `a'
collect2: error: ld returned 1 exit status
```

### リスト 4.6 objcopy を使ったシンボル名の変更

- [const_b.c](CHAPTER-04/const_b.c)
- 書籍化からの変更点: 特になし

const_b.c と const_sub.o をコンパイル・リンクして、シンボル `b` の参照が解決できず失敗することを確認:

```
$ gcc -c const_b.c -Wall

$ gcc const_b.o const_sub.o -Wall -o const_b
/usr/bin/ld: const_b.o: in function `main':
const_b.c:(.text+0x6): undefined reference to `b'
collect2: error: ld returned 1 exit status
```

`objcopy --redefine-sym` で `const_sub.o` の中の `a` への参照を `b` への参照に書き換えた `const_sub.o` を作成し、nmコマンドからもシンボル名の変更を確認:

```
$ objcopy --redefine-sym a=b const_sub.o const_sub_b.o

$ nm const_sub_b.o
0000000000000000 R b
```

const_sub.o と const_b.o をリンクし、正常に実行できることを確認:

```
$ gcc const_b.o const_sub_b.o -Wall -o const_b2

$ ./const_b2
1
```

### リスト 4.7 - 4.10 変数の隠蔽の階層化

サンプルコード:

- [layer.c](CHAPTER-04/layer.c)
- [layer2.c](CHAPTER-04/layer2.c)
- [layer_sub1.c](CHAPTER-04/layer_sub1.c)
- [layer_sub2.c](CHAPTER-04/layer_sub2.c)
- 書籍化からの変更点: 特になし

コンパイルと実行:

```
$ gcc -c layer_sub1.c -Wall
$ gcc -c layer_sub2.c -Wall
$ gcc -c layer.c -Wall
$ gcc layer.o layer_sub1.o layer_sub2.o -o layer

$ ./layer
num = 0
num = 1
num = 2
num = 3
```

書籍ではこのあと、`num` 変数を `layer.c` から隠蔽するための試行錯誤が続く。

まずオブジェクトファイルをまとめてアーカイブにして、アーカイブの中で変数を非公開にすることを試みる。

アーカイブの作成と、アーカイブをリンクしたバイナリの生成と実行:

```
$ ar ruc liblayer.a layer_sub1.o layer_sub2.o
$ gcc layer.o -o alayer -L. -llayer
$ ./alayer
num = 0
num = 1
num = 2
num = 3
```

正常に実行できていることを確認した。
nmコマンドで中のシンボルを確認してみると、書籍では種別が `D` だがこちらでは `B` と表記され、BSS(未初期化変数)扱いであることを確認した。

```
$ nm liblayer.a

layer_sub1.o:
000000000000001e T func1
0000000000000000 B num
                 U printf
0000000000000000 T printnum

layer_sub2.o:
0000000000000000 T func2
                 U num
```

シンボルタイプは異なるものの、書籍に従い `objcopy --localize-symbol` オプションでアーカイブ内の `num` をローカル扱いに変更してみる。

```
$ objcopy --localize-symbol=num liblayer.a liblayer_local.a

$ nm liblayer_local.a

layer_sub1.o:
000000000000001e T func1
0000000000000000 b num
                 U printf
0000000000000000 T printnum

layer_sub2.o:
0000000000000000 T func2
                 U num

$ gcc layer.o -o alayer_local -L. -llayer_local
/usr/bin/ld: layer.o: in function `main':
layer.c:(.text+0x10): undefined reference to `num'
/usr/bin/ld: layer.c:(.text+0x19): undefined reference to `num'
/usr/bin/ld: ./liblayer_local.a(layer_sub2.o): in function `func2':
layer_sub2.c:(.text+0x6): undefined reference to `num'
/usr/bin/ld: layer_sub2.c:(.text+0xf): undefined reference to `num'
collect2: error: ld returned 1 exit status
```

nmコマンドでは num のシンボルタイプが `b` に変更され、ローカル扱いになったことを確認できた。
リンク時には書籍と同様、`layer.c` と `layer_sub2.c` それぞれで num シンボルが見つからずエラーとなったことを確認できた。

書籍に従い、アーカイブにするのではなく `ld -r` でオブジェクトファイルとして一つにまとめてから隠蔽するアプローチを試す。

```
$ ld -r layer_sub1.o layer_sub2.o  -o layer_lib.o

$ nm layer_lib.o
000000000000001e T func1
0000000000000034 T func2
0000000000000000 B num
                 U printf
0000000000000000 T printnum

$ gcc layer.o layer_lib.o -o olayer
$ ./olayer
num = 0
num = 1
num = 2
num = 3
```

正常動作まで確認できたので、 `layer_lib.o` の変数をローカル扱いに変更してみる。

```
$ objcopy --localize-symbol=num layer_lib.o layer_lib_local.o

$ nm layer_lib_local.o
000000000000001e T func1
0000000000000034 T func2
0000000000000000 b num
                 U printf
0000000000000000 T printnum

$ gcc layer.o layer_lib_local.o -o layer_local
/usr/bin/ld: layer.o: in function `main':
layer.c:(.text+0x10): undefined reference to `num'
/usr/bin/ld: layer.c:(.text+0x19): undefined reference to `num'
collect2: error: ld returned 1 exit status
```

nmコマンドで num シンボルがローカルのBSS扱いになったことを確認できた。
layer.o とリンクしたところ、書籍と同様に layer.o から参照している num を解決できずエラーになったことを確認できた。

書籍に従い layer2.c の方をリンクしてみる。

```
$ gcc -c layer2.c -Wall

$ gcc layer2.o layer_lib_local.o -o layer2_local

$ ./layer2_local
num = 0
num = 1
num = 2
```

正常に実行できた。
書籍と同様、ライブラリとしての layer_lib_local.o の中だけに `num` を隠蔽できることを確認できた。

### リスト 4.13 シンボルのアドレス表示

- [edata.c](CHAPTER-04/edata.c)
- 書籍化からの変更点: 特になし

なお今回の環境では、リンカスクリプト [const-linker-script-default](CHAPTER-04/const-linker-script-default) 中で以下のようにシンボルが定義されていた。

```
(...)
  .fini_array    :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  }
(...)
  .data1          : { *(.data1) }
  _edata = .; PROVIDE (edata = .);
  . = .;
(...)
  . = ALIGN(64 / 8);
  _end = .; PROVIDE (end = .);
  . = DATA_SEGMENT_END (.);
(...)
```

コンパイルして実行してみると、書籍と同様にリンカスクリプト中で

```
$ gcc edata.c -Wall -o edata

$ ./edata
&__fini_array_end = 0x403df8
&edata = 0x40400c
&end = 0x404010

$ objdump -h edata
(...)
 18 .init_array   00000008  0000000000403de8  0000000000403de8  00002de8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .fini_array   00000008  0000000000403df0  0000000000403df0  00002df0  2**3
                  CONTENTS, ALLOC, LOAD, DATA

#### <-- "&__fini_array_end = 0x403df8"

20 .dynamic      000001d0  0000000000403df8  0000000000403df8  00002df8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 21 .got          00000020  0000000000403fc8  0000000000403fc8  00002fc8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 22 .got.plt      00000020  0000000000403fe8  0000000000403fe8  00002fe8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 23 .data         00000004  0000000000404008  0000000000404008  00003008  2**0
                  CONTENTS, ALLOC, LOAD, DATA

#### <-- "&edata = 0x40400c"

 24 .bss          00000004  000000000040400c  000000000040400c  0000300c  2**0
                  ALLOC

#### <-- "&end = 0x404010"

 25 .comment      0000005c  0000000000000000  0000000000000000  0000300c  2**0
                  CONTENTS, READONLY
(...)
```

objdump コマンドの結果からも、それぞれのシンボルのアドレスがちょうどリンカスクリプトで定義されているアドレスを指していることを確認できた。

### リスト 4.14 初期化を任意に行う

- [initialize.c](CHAPTER-04/initialize.c)
- 書籍化からの変更点:
  - 日本語コメントについてファイルの文字コードをUTF-8に変更
  - `memset()`, `memcpy()` 呼び出しで関数が未定義の警告が表示されたので、案内に従い `string.h` を include に追加。
  - `edata.c` と同様に `__fini_array_end, edata, end` シンボルのアドレス表示を追加。

コンパイルと実行:

```
$ gcc initialize.c -Wall -o initialize

$ ./initialize
&__fini_array_end = 0x403df8
&edata = 0x404048
&end = 0x404068
i0 = 0, i1 = 1, d0 = 0.000000, d1 = 0.100000, s0 = (null), s1 = sample string
i0 = 1, i1 = 2, d0 = 1.100000, d1 = 1.200000, s0 = sample1, s1 = sample2
Segmentation fault (core dumped)
```

書籍とは異なり segfault が発生してしまった。

`readelf` でセグメント情報をダンプし、`__fini_array_end, edata, end` シンボルのアドレス周辺に何があるか確認してみる。

```
$ readelf -l initialize
(...)
  LOAD           0x0000000000002000 0x0000000000402000 0x0000000000402000
                 0x0000000000000180 0x0000000000000180  R      0x1000
  LOAD           0x0000000000002de8 0x0000000000403de8 0x0000000000403de8
                 0x0000000000000260 0x0000000000000280  RW     0x1000

 Section to Segment mapping:
  Segment Sections...
(...)
   04     .rodata .eh_frame_hdr .eh_frame
   05     .init_array .fini_array .dynamic .got .got.plt .data .bss
(...)
```

`__fini_array_end` から `edata` までの間に `.got` や `.got.plt` など、共有ライブラリ(今回のソースでは `printf()`)呼び出しに関わるセクションが存在している。
メモリセグメントとしては `RW` なので、メモリ保護でのエラーではないと思われる。
`.got` や `.got.plt` は実行時に初めて遅延初期化される内容も含まれているので、おそらく `reset_ivalue()` による初期化で実行前の状態に書き戻されてしまい、他の領域で管理している共有ライブラリのロード情報との整合性が取れず、おかしなアドレスにジャンプしたせいではないかと思われる。

### リスト 4.18 関数の自動呼び出し

- [ctors.c](CHAPTER-04/ctors.c)
- 書籍化からの変更点: 特になし
- gcc の `__attribute__((section("section-name")))` の解説:
  - [Variable Attributes - Using the GNU Compiler Collection (GCC)](https://gcc.gnu.org/onlinedocs/gcc-3.4.3/gcc/Variable-Attributes.html)
  - [C言語 (GCC) で配列の配置先を切り替えて実行可能にする #セクション - Qiita](https://qiita.com/mikecat_mixc/items/e80ded9fcb13fb50a331)
  - [GCCプロジェクトで変数や関数を指定のセクションに配置する方法 | Renesas Customer Hub](https://ja-support.renesas.com/knowledgeBase/19508395)

コンパイルと実行: `main()` が実行されるまえに `init1()`, `init2()` が実行されることを確認できた。

```
$ gcc ctors.c -Wall -o ctors

$ ./ctors
ctors test. (init2)
ctors test. (init1)
3
```

nmコマンドの結果から、`fp1, fp2` が `0x403de0` から配置されていることが分かったので、`objdump -h` でどのセクションに存在するのか逆算してみる。

```
$ nm ctors
(...)
0000000000403de0 D fp1
0000000000403de8 D fp2
(...)

$ objdump -h ctors
(...)
 18 .init_array   00000018  0000000000403dd8  0000000000403dd8  00002dd8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .fini_array   00000008  0000000000403df0  0000000000403df0  00002df0  2**3
                  CONTENTS, ALLOC, LOAD, DATA
(...)
```

書籍では `.ctors, .dtors` セクションがあり `.ctors` に配置される例が掲載されているが、こちらでは `.ctors, .dtors` とも存在せず、代わりに `.init_array, .fini_array` セクションが存在した。
また `fp1, fp2` は `.init_array` セクションの領域に存在することを確認できた。

`.init_array, .fini_array` セクションについては、 [今回の検証環境のデフォルトのリンカスクリプト](CHAPTER-04/const-linker-script-default) に以下のように定義されており、 `.ctors` および `.dtors` をそれぞれ集約している。

```
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
```

このため、書籍と異なり `.init_array` セクションに配置されたものと思われる。

### リスト 4.22 constructor, desctructor 属性の利用

- [constructor.c](CHAPTER-04/constructor.c)
- 書籍化からの変更点: 特になし
- gcc の `__attribute__((constructor))` の解説:
  - [Function Attributes - Using the GNU Compiler Collection (GCC)](https://gcc.gnu.org/onlinedocs/gcc-3.4.3/gcc/Function-Attributes.html)

コンパイルと実行: `main()` の前後で `init()/fini()` が呼ばれていることを確認できた。

```
$ gcc constructor.c -Wall -o constructor

$ ./constructor
init()
main()
fini()
```

シンボル配置を確認してみると、 `init()/fini()` ともに `.text` セクションに配置されていると思われる。

```
$ nm constructor
(...)
0000000000401040 T _start
000000000040400c b completed.0
0000000000404008 W data_start
0000000000401080 t deregister_tm_clones
0000000000401137 T fini
0000000000401120 t frame_dummy
0000000000401126 T init
0000000000401148 T main
(...)
```

