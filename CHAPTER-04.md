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


