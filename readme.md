# PIC32MK 3.58MHz Sin wave generator

## 仕様
- 3.58MHz
- 5bitDAC
- 14.31818MHz clystal使用
- 内部32倍(114.56MHz)
- 出力サンプリング周波数 28.64MHz(8倍)

## 回路

```
61 (RPB11 = SDO3) --- 1k  Ohm --+
62 (PRB12 = SDO2) --- 200 Ohm --+
63 (RPB13 = SDO6) --- 510 Ohm --+----+
 2 (RPB14 = SDO4) --- 2k0 Ohm --+   51 Ohm
 3 (RPB15 = SDO5) --- 4k2 Ohm --+    |
                                    GND
```

## 仕組みというか
### SPI
Enhanced buffer mode is enabled.
BRG = 1(114.56/4)

### DMA
12バイト転送
TMR2タイミングを見計らって、データが切れる前に前もって送信
