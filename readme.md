# PIC32MK NTSC生成

# なんと！！kenkenさんが拡張してくれました！！！
こっちのほうが使いやすいのでこちらを参照。
http://www.ze.em-net.ne.jp/~kenken/spicomposite/index.html

## 仕様
- 3.58MHz
- 5bitDAC
- 14.31818MHz clystal使用
- 内部32倍(114.56MHz)
- 出力サンプリング周波数 57.28MHz(16倍)

## 回路

抵抗値は適当なの選んでください。

```
61 (RPB11 = SDO3) --- 200 Ohm --+
 2 (RPB14 = SDO4) --- 400 Ohm --+---51 Ohm
 3 (RPB15 = SDO5) --- 800 Ohm --+    |
                                    GND
```

## 仕組みというか
### SPI
拡張バッファを有効にしてDMAの転送トリガーの負荷を減らしています。

### DMA
12バイト転送
TMR2タイミングを見計らって、データが切れる前に前もって送信

### その他
ここにも僅かに書きました。雑ですね。すみません。
http://gombe.azurewebsites.net/2017/09/16/pic32mk-spix3%e3%81%a7ntsc/
