/*
 * File:   newmain.c
 * Author: gombe
 *
 * Created on 2017/08/06, 20:24
 */

#include <xc.h>
#include <stdint.h>
#include <cp0defs.h>

#define DMA_TRANSLATION_SIZE 12

#define ONE_LINE_BIT 3632
#define LINE_BUFFER_SIZE (ONE_LINE_BIT/8)

// LINE_BUFFER_SIZE は DMA_TRANSLATION_SIZE　で割り切れなければならない。 
#define NUM_OF_BUFFERS 6 //6,12,...

#define BUFFER_SIZE (NUM_OF_BUFFERS * LINE_BUFFER_SIZE)

#define LOW_SPEED_SIGNAL 0

uint8_t ch0data[BUFFER_SIZE];
uint8_t ch1data[BUFFER_SIZE];
uint8_t ch2data[BUFFER_SIZE];
uint8_t ch3data[BUFFER_SIZE];
uint8_t ch4data[BUFFER_SIZE];

int drawing_line;

uint8_t vram[320*220];
uint32_t colordata[]={
  0x005ba4,0x006d92,  //(r,g,b)=(#000000) data:1212212212212212
  0x006d92,0x00b748,  //(r,g,b)=(#080000) data:1221221221221222
  0x007a85,0x00bb44,  //(r,g,b)=(#100000) data:1222212121222122
  0x00bd42,0x005fa0,  //(r,g,b)=(#180000) data:2122221212122222
  0x00f50a,0x006f90,  //(r,g,b)=(#200000) data:2222121212212222
  0x00fa05,0x00b74c,  //(r,g,b)=(#280000) data:2222212121221322
  0x00fc03,0x00bf41,  //(r,g,b)=(#300000) data:2222221121222223
  0x00f52a,0x005fa2,  //(r,g,b)=(#380000) data:2232121212122232
  0x00fa45,0x005fa5,  //(r,g,b)=(#400000) data:2322212112122323
  0x00fc43,0x00bf42,  //(r,g,b)=(#480000) data:2322221121222232
  0x00f956,0x003fc5,  //(r,g,b)=(#500000) data:2323211211222323
  0x00fa65,0x003fc7,  //(r,g,b)=(#580000) data:2332212111222333
  0x00fca3,0x005fab,  //(r,g,b)=(#600000) data:3232221112123233
  0x00f9d6,0x003fc7,  //(r,g,b)=(#680000) data:3323211211222333
  0x00fce3,0x013ec6,  //(r,g,b)=(#700000) data:3332221111222334
  0x00fce3,0x023dc9,  //(r,g,b)=(#780000) data:3332221111223243
  0x00f8f7,0x015eae,  //(r,g,b)=(#800000) data:3333211112123334
  0x40bca3,0x021df5,  //(r,g,b)=(#880000) data:3432221111132343
  0x40b8b7,0x053ac2,  //(r,g,b)=(#900000) data:3433211111222434
  0x40b8b7,0x033ccc,  //(r,g,b)=(#980000) data:3433211111223344
  0xa05c43,0x035c2c,  //(r,g,b)=(#a00000) data:4342221102123344
  0xa05857,0x0718e8,  //(r,g,b)=(#a80000) data:4343211111123444
  0xc03837,0x033ccd,  //(r,g,b)=(#b00000) data:4433211111223345
  0xc03c32,0x0738c8,  //(r,g,b)=(#b80000) data:4433221011223444
  0xe01817,0x073849,  //(r,g,b)=(#c00000) data:4443211101223445
  0xe01c11,0x0b14e6,  //(r,g,b)=(#c80000) data:4443220111124354
  0xc0387e,0x0718f9,  //(r,g,b)=(#d00000) data:4533311011133445
  0xe01857,0x07384b,  //(r,g,b)=(#d80000) data:4543211101223455
  0xe01c51,0x07384d,  //(r,g,b)=(#e00000) data:4543220101223545
  0xe0189e,0x0f10e3,  //(r,g,b)=(#e80000) data:5443311011124455
  0xe018b6,0x0718fb,  //(r,g,b)=(#f00000) data:5453211011133455
  0xe01cd1,0x07384f,  //(r,g,b)=(#f80000) data:5543220101223555
  0x005ba4,0x006d92,  //(r,g,b)=(#000000) data:1212212212212212
  0x006e91,0x00f708,  //(r,g,b)=(#080800) data:1221222122221222
  0x007b84,0x00ff00,  //(r,g,b)=(#101000) data:1222212222222222
  0x00bf40,0x00ff00,  //(r,g,b)=(#181800) data:2122222222222222
  0x00fb04,0x00ff44,  //(r,g,b)=(#202000) data:2222212223222322
  0x00ff00,0x00ff2a,  //(r,g,b)=(#282800) data:2222222222323232
  0x00ff01,0x00ff2d,  //(r,g,b)=(#303000) data:2222222322323323
  0x00ff20,0x00ffb7,  //(r,g,b)=(#383800) data:2232222232332333
  0x00ff42,0x00ff7f,  //(r,g,b)=(#404000) data:2322223223333333
  0x00ff51,0x00ff7f,  //(r,g,b)=(#484800) data:2323222323333333
  0x00ff62,0x04fbfb,  //(r,g,b)=(#505000) data:2332223233333433
  0x00ffa9,0x0af5f5,  //(r,g,b)=(#585800) data:3232322333334343
  0x00ffd2,0x2dd2d2,  //(r,g,b)=(#606000) data:3323223233434434
  0x00ffe5,0x37c8c8,  //(r,g,b)=(#686800) data:3332232333443444
  0x00ffeb,0x3fc0c0,  //(r,g,b)=(#707000) data:3332323333444444
  0x00fff6,0xbe4143,  //(r,g,b)=(#787800) data:3333233243444453
  0x40bfab,0x7f8088,  //(r,g,b)=(#808000) data:3432323334445444
  0x40bfb7,0x7f808a,  //(r,g,b)=(#888800) data:3433233334445454
  0x40bfbb,0xff0016,  //(r,g,b)=(#909000) data:3433323344454554
  0x807f7f,0xff001b,  //(r,g,b)=(#989800) data:4333333344455455
  0xa15e5a,0x7f80bf,  //(r,g,b)=(#a0a000) data:4343323434555555
  0xc03f3f,0xff043a,  //(r,g,b)=(#a8a800) data:4433333344555654
  0xd12e26,0xff0837,  //(r,g,b)=(#b0b000) data:4434233444556555
  0xe11e1e,0xff0c33,  //(r,g,b)=(#b8b800) data:4443333444556655
  0xe21d1d,0xff1669,  //(r,g,b)=(#c0c000) data:4443334345565665
  0xd12e6e,0xff1d62,  //(r,g,b)=(#c8c800) data:4534333445566656
  0xe21d5d,0xff1fe0,  //(r,g,b)=(#d0d000) data:4543334355566666
  0xe91696,0xff1ee5,  //(r,g,b)=(#d8d800) data:5443433455566765
  0xf30c8c,0xff3f48,  //(r,g,b)=(#e0e000) data:5444334445667666
  0xe916d6,0xff3fcc,  //(r,g,b)=(#e8e800) data:5543433455667766
  0xf30ccc,0xff3fd6,  //(r,g,b)=(#f0f000) data:5544334455676776
  0xf708c8,0xff3fdd,  //(r,g,b)=(#f8f800) data:5544344455677767
  0x005ba4,0x006d92,  //(r,g,b)=(#000000) data:1212212212212212
  0x006e91,0x00dd22,  //(r,g,b)=(#040404) data:1221222122122212
  0x007788,0x00bb44,  //(r,g,b)=(#080808) data:1222122221222122
  0x007d82,0x00f708,  //(r,g,b)=(#0c0c0c) data:1222221222221222
  0x007f80,0x00ef10,  //(r,g,b)=(#101010) data:1222222222212222
  0x007f80,0x00ff00,  //(r,g,b)=(#141414) data:1222222222222222
  0x00ff00,0x00ff00,  //(r,g,b)=(#181818) data:2222222222222222
  0x00ff00,0x00ff40,  //(r,g,b)=(#1c1c1c) data:2222222223222222
  0x00ff04,0x00ff10,  //(r,g,b)=(#202020) data:2222232222232222
  0x00ff08,0x00ff88,  //(r,g,b)=(#242424) data:2222322232223222
  0x00ff12,0x00ff24,  //(r,g,b)=(#282828) data:2223223222322322
  0x00ff24,0x00ff94,  //(r,g,b)=(#2c2c2c) data:2232232232232322
  0x00ff29,0x00ff52,  //(r,g,b)=(#303030) data:2232322323232232
  0x00ff2a,0x00ffaa,  //(r,g,b)=(#343434) data:2232323232323232
  0x00ff55,0x00ff6a,  //(r,g,b)=(#383838) data:2323232323323232
  0x00ff5a,0x00ffda,  //(r,g,b)=(#3c3c3c) data:2323323233233232
  0x00ff6d,0x00ffb6,  //(r,g,b)=(#404040) data:2332332332332332
  0x00ff6e,0x00ffee,  //(r,g,b)=(#444444) data:2332333233323332
  0x00ff7b,0x00ffdf,  //(r,g,b)=(#484848) data:2333323333233333
  0x00ff7f,0x00ff7f,  //(r,g,b)=(#4c4c4c) data:2333333323333333
  0x00ff7f,0x00ffff,  //(r,g,b)=(#505050) data:2333333333333333
  0x00ffff,0x00ffff,  //(r,g,b)=(#545454) data:3333333333333333
  0x00ffff,0x04fbfb,  //(r,g,b)=(#585858) data:3333333333333433
  0x01fefe,0x02fdfd,  //(r,g,b)=(#5c5c5c) data:3333333433333343
  0x08f7f7,0x42bdbd,  //(r,g,b)=(#606060) data:3333433334333343
  0x11eeee,0x11eeee,  //(r,g,b)=(#646464) data:3334333433343334
  0x12eded,0x49b6b6,  //(r,g,b)=(#686868) data:3334334334334334
  0x25dada,0x29d6d6,  //(r,g,b)=(#6c6c6c) data:3343343433434334
  0x2ad5d5,0xa55a5a,  //(r,g,b)=(#707070) data:3343434343433434
  0x55aaaa,0x55aaaa,  //(r,g,b)=(#747474) data:3434343434343434
  0x56a9a9,0xb54a4a,  //(r,g,b)=(#787878) data:3434344343443434
  0x5ba4a4,0x6d9292,  //(r,g,b)=(#7c7c7c) data:3434434434434434
  0x6e9191,0xdd2222,  //(r,g,b)=(#808080) data:3443444344344434
  0x778888,0xbb4444,  //(r,g,b)=(#848484) data:3444344443444344
  0x7d8282,0xf70808,  //(r,g,b)=(#888888) data:3444443444443444
  0x7f8080,0xef1010,  //(r,g,b)=(#8c8c8c) data:3444444444434444
  0x7f8080,0xff0000,  //(r,g,b)=(#909090) data:3444444444444444
  0xff0000,0xff0000,  //(r,g,b)=(#949494) data:4444444444444444
  0xff0000,0xff0040,  //(r,g,b)=(#989898) data:4444444445444444
  0xff0004,0xff0010,  //(r,g,b)=(#9c9c9c) data:4444454444454444
  0xff0008,0xff0088,  //(r,g,b)=(#a0a0a0) data:4444544454445444
  0xff0012,0xff0024,  //(r,g,b)=(#a4a4a4) data:4445445444544544
  0xff0024,0xff0094,  //(r,g,b)=(#a8a8a8) data:4454454454454544
  0xff0029,0xff0052,  //(r,g,b)=(#acacac) data:4454544545454454
  0xff002a,0xff00aa,  //(r,g,b)=(#b0b0b0) data:4454545454545454
  0xff0055,0xff00aa,  //(r,g,b)=(#b4b4b4) data:4545454554545454
  0xff005a,0xff00da,  //(r,g,b)=(#b8b8b8) data:4545545455455454
  0xff006d,0xff00b6,  //(r,g,b)=(#bcbcbc) data:4554554554554554
  0xff006e,0xff00ee,  //(r,g,b)=(#c0c0c0) data:4554555455545554
  0xff007b,0xff00df,  //(r,g,b)=(#c4c4c4) data:4555545555455555
  0xff007f,0xff007f,  //(r,g,b)=(#c8c8c8) data:4555555545555555
  0xff007f,0xff00ff,  //(r,g,b)=(#cccccc) data:4555555555555555
  0xff00ff,0xff00ff,  //(r,g,b)=(#d0d0d0) data:5555555555555555
  0xff00ff,0xff04fb,  //(r,g,b)=(#d4d4d4) data:5555555555555655
  0xff01fe,0xff02fd,  //(r,g,b)=(#d8d8d8) data:5555555655555565
  0xff08f7,0xff42bd,  //(r,g,b)=(#dcdcdc) data:5555655556555565
  0xff11ee,0xff11ee,  //(r,g,b)=(#e0e0e0) data:5556555655565556
  0xff12ed,0xff49b6,  //(r,g,b)=(#e4e4e4) data:5556556556556556
  0xff25da,0xff29d6,  //(r,g,b)=(#e8e8e8) data:5565565655656556
  0xff2ad5,0xffa55a,  //(r,g,b)=(#ececec) data:5565656565655656
  0xff55aa,0xff55aa,  //(r,g,b)=(#f0f0f0) data:5656565656565656
  0xff56a9,0xffb54a,  //(r,g,b)=(#f4f4f4) data:5656566565665656
  0xff5ba4,0xff6d92,  //(r,g,b)=(#f8f8f8) data:5656656656656656
  0xff6e91,0xffdd22,  //(r,g,b)=(#fcfcfc) data:5665666566566656
  0x005ba4,0x006d92,  //(r,g,b)=(#000000) data:1212212212212212
  0x007689,0x00db24,  //(r,g,b)=(#080008) data:1222122122122122
  0x007d82,0x00b748,  //(r,g,b)=(#100010) data:1222221221221222
  0x00f708,0x006e91,  //(r,g,b)=(#180018) data:2222122212212221
  0x00fe01,0x00db24,  //(r,g,b)=(#200020) data:2222222122122122
  0x00fd12,0x00af50,  //(r,g,b)=(#280028) data:2223221221212222
  0x00ff20,0x005fa0,  //(r,g,b)=(#300030) data:2232222212122222
  0x00fe51,0x00d72a,  //(r,g,b)=(#380038) data:2323222122121232
  0x00fd56,0x00af51,  //(r,g,b)=(#400040) data:2323231221212223
  0x00ff70,0x005fa1,  //(r,g,b)=(#480048) data:2333222212122223
  0x00ff78,0x0037ca,  //(r,g,b)=(#500050) data:2333322211221232
  0x40bf28,0x00af52,  //(r,g,b)=(#580058) data:2432322221212232
  0x00fff8,0x005fa1,  //(r,g,b)=(#600060) data:3333322212122223
  0x00fffc,0x002fd3,  //(r,g,b)=(#680068) data:3333332211212233
  0x20dedb,0x009f63,  //(r,g,b)=(#700070) data:3343323121122233
  0x40bfbc,0x0057af,  //(r,g,b)=(#780078) data:3433332212121333
  0x50afac,0x012ed2,  //(r,g,b)=(#800080) data:3434332211212234
  0x689693,0x009f63,  //(r,g,b)=(#880088) data:3443423121122233
  0x708f8c,0x004fb7,  //(r,g,b)=(#900090) data:3444332212112333
  0x6897d4,0x002fd7,  //(r,g,b)=(#980098) data:3543432211212333
  0xf00e0f,0x019e62,  //(r,g,b)=(#a000a0) data:4444333121122234
  0xf80704,0x014eb6,  //(r,g,b)=(#a800a8) data:4444432212112334
  0xf80724,0x011ee6,  //(r,g,b)=(#b000b0) data:4454432211122334
  0xf80647,0x039c60,  //(r,g,b)=(#b800b8) data:4544433121122244
  0xf80754,0x012ed6,  //(r,g,b)=(#c000c0) data:4545432211212334
  0xf80766,0x011ee6,  //(r,g,b)=(#c800c8) data:4554433211122334
  0xf80677,0x038c74,  //(r,g,b)=(#d000d0) data:4555433121112344
  0xfc4320,0x032cd4,  //(r,g,b)=(#d800d8) data:4654442211212344
  0xf807f6,0x011ee7,  //(r,g,b)=(#e000e0) data:5555433211122335
  0xf806ff,0x038c75,  //(r,g,b)=(#e800e8) data:5555533121112345
  0xfc23d0,0x031ce5,  //(r,g,b)=(#f000f0) data:5565442211122345
  0xf847be,0x030cfc,  //(r,g,b)=(#f800f8) data:5655533211113344
  0x005ba4,0x006d92,  //(r,g,b)=(#000000) data:1212212212212212
  0x005da2,0x00dd22,  //(r,g,b)=(#000800) data:1212221222122212
  0x005ea1,0x00fe01,  //(r,g,b)=(#001000) data:1212222122222221
  0x005fa0,0x00ff00,  //(r,g,b)=(#001800) data:1212222222222222
  0x006f90,0x00ff40,  //(r,g,b)=(#002000) data:1221222223222222
  0x006f90,0x00ffa0,  //(r,g,b)=(#002800) data:1221222232322222
  0x006f91,0x00ff50,  //(r,g,b)=(#003000) data:1221222323232222
  0x006f92,0x00fed5,  //(r,g,b)=(#003800) data:1221223233232321
  0x006f93,0x00ff74,  //(r,g,b)=(#004000) data:1221223323332322
  0x006f95,0x00fff4,  //(r,g,b)=(#004800) data:1221232333332322
  0x006f95,0x20dfd4,  //(r,g,b)=(#005000) data:1221232333432322
  0x006f97,0x10efe4,  //(r,g,b)=(#005800) data:1221233333342322
  0x006f9b,0x40bfbc,  //(r,g,b)=(#006000) data:1221323334333322
  0x006f9b,0xa85752,  //(r,g,b)=(#006800) data:1221323343434232
  0x016e9a,0x689792,  //(r,g,b)=(#007000) data:1221323434434232
  0x027d81,0xe81714,  //(r,g,b)=(#007800) data:1222224344434322
  0x007f87,0xf80704,  //(r,g,b)=(#008000) data:1222233344444322
  0x017e86,0xf80705,  //(r,g,b)=(#008800) data:1222233444444323
  0x017e86,0xf80746,  //(r,g,b)=(#009000) data:1222233445444332
  0x017e86,0xf807a6,  //(r,g,b)=(#009800) data:1222233454544332
  0x037c84,0xf80766,  //(r,g,b)=(#00a000) data:1222234445544332
  0x037c84,0xf807d6,  //(r,g,b)=(#00a800) data:1222234455454332
  0x037c84,0xf807f7,  //(r,g,b)=(#00b000) data:1222234455554333
  0x037c85,0xfc03f2,  //(r,g,b)=(#00b800) data:1222234555554432
  0x077880,0xfc43b2,  //(r,g,b)=(#00c000) data:1222244456554432
  0x077881,0xfc23d2,  //(r,g,b)=(#00c800) data:1222244555654432
  0x077881,0xfc53a2,  //(r,g,b)=(#00d000) data:1222244556564432
  0x037c8d,0xfca35b,  //(r,g,b)=(#00d800) data:1222334565655433
  0x037d8c,0xfc639b,  //(r,g,b)=(#00e000) data:1222334656655433
  0x037c8f,0xfee118,  //(r,g,b)=(#00e800) data:1222335566655442
  0x037c8f,0xfee138,  //(r,g,b)=(#00f000) data:1222335566755442
  0x077988,0xfcf30f,  //(r,g,b)=(#00f800) data:1222344666665533
  0x005ba4,0x006d92,  //(r,g,b)=(#000000) data:1212212212212212
  0x006e91,0x00f609,  //(r,g,b)=(#000808) data:1221222122221221
  0x006f90,0x00fb04,  //(r,g,b)=(#001010) data:1221222222222122
  0x007f80,0x00fe01,  //(r,g,b)=(#001818) data:1222222222222221
  0x007f81,0x00f718,  //(r,g,b)=(#002020) data:1222222322231222
  0x007f82,0x00fda2,  //(r,g,b)=(#002828) data:1222223232322212
  0x007f85,0x00ffa0,  //(r,g,b)=(#003030) data:1222232332322222
  0x007f8b,0x00fdaa,  //(r,g,b)=(#003838) data:1222323332323212
  0x007f8f,0x00ffd0,  //(r,g,b)=(#004040) data:1222333333232222
  0x007f97,0x807f50,  //(r,g,b)=(#004848) data:1223233343232222
  0x027d95,0x40bea9,  //(r,g,b)=(#005050) data:1223234334323221
  0x017e9e,0x40bfb0,  //(r,g,b)=(#005858) data:1223333434332222
  0x037c9c,0x40beb5,  //(r,g,b)=(#006060) data:1223334434332321
  0x057aaa,0xc03f38,  //(r,g,b)=(#006868) data:1232343444333222
  0x0b74a4,0xd02f20,  //(r,g,b)=(#007070) data:1232434444342222
  0x0778b8,0xe01f18,  //(r,g,b)=(#007878) data:1233344444433222
  0x0778b9,0xe01f18,  //(r,g,b)=(#008080) data:1233344544433222
  0x0778ba,0xe01f9c,  //(r,g,b)=(#008888) data:1233345454433322
  0x0f70b1,0xf00f88,  //(r,g,b)=(#009090) data:1233444554443222
  0x0f70b3,0xf00e8d,  //(r,g,b)=(#009898) data:1233445554443321
  0x0f71d4,0xe01fbc,  //(r,g,b)=(#00a0a0) data:1323454654533322
  0x0f70d7,0xf00fe8,  //(r,g,b)=(#00a8a8) data:1323455555543222
  0x1f62c1,0xf00fec,  //(r,g,b)=(#00b0b0) data:1324446555543322
  0x1f61c6,0xf84780,  //(r,g,b)=(#00b8b8) data:1324455656444222
  0x1f62c5,0xf0cf1c,  //(r,g,b)=(#00c0c0) data:1324456566453322
  0x0f71fe,0xf0cf2c,  //(r,g,b)=(#00c8c8) data:1333555666543322
  0x0f73fc,0xf8c722,  //(r,g,b)=(#00d0d0) data:1333556666544232
  0x0f73fd,0xf8c724,  //(r,g,b)=(#00d8d8) data:1333556766544322
  0x1f67e0,0xf0cebf,  //(r,g,b)=(#00e0e0) data:1334466676553331
  0x1f63ee,0xf8c7b4,  //(r,g,b)=(#00e8e8) data:1334557676554322
  0x1f67e9,0xf8e784,  //(r,g,b)=(#00f0f0) data:1334566776644322
  0x1f67eb,0xf8e794,  //(r,g,b)=(#00f8f8) data:1334567776654322
  0x005ba4,0x006d92,  //(r,g,b)=(#000000) data:1212212212212212
  0x006d92,0x00b649,  //(r,g,b)=(#000008) data:1221221221221221
  0x006e91,0x00da25,  //(r,g,b)=(#000010) data:1221222122122121
  0x007788,0x00ab54,  //(r,g,b)=(#000018) data:1222122221212122
  0x007d82,0x00d52a,  //(r,g,b)=(#000020) data:1222221222121212
  0x007f80,0x006a95,  //(r,g,b)=(#000028) data:1222222212212121
  0x007f80,0x00d22d,  //(r,g,b)=(#000030) data:1222222222121121
  0x007f80,0x00e51a,  //(r,g,b)=(#000038) data:1222222222211212
  0x007f84,0x00a956,  //(r,g,b)=(#000040) data:1222232221212112
  0x007f88,0x00d22d,  //(r,g,b)=(#000048) data:1222322222121121
  0x007f89,0x00a45b,  //(r,g,b)=(#000050) data:1222322321211211
  0x007f92,0x00c936,  //(r,g,b)=(#000058) data:1223223222112112
  0x007f94,0x00e11e,  //(r,g,b)=(#000060) data:1223232222211112
  0x007f95,0x00a25d,  //(r,g,b)=(#000068) data:1223232321211121
  0x007f9a,0x00d02f,  //(r,g,b)=(#000070) data:1223323222121111
  0x007fad,0x00a05f,  //(r,g,b)=(#000078) data:1232332321211111
  0x007fad,0x00c13e,  //(r,g,b)=(#000080) data:1232332322111112
  0x007fae,0x00e01f,  //(r,g,b)=(#000088) data:1232333222211111
  0x007fb7,0x00c03f,  //(r,g,b)=(#000090) data:1233233322111111
  0x007fbe,0x00c03f,  //(r,g,b)=(#000098) data:1233333222111111
  0x007fbe,0x00a1da,  //(r,g,b)=(#0000a0) data:1233333231211012
  0x007fbf,0x00c03f,  //(r,g,b)=(#0000a8) data:1233333322111111
  0x027dbc,0x00a0d7,  //(r,g,b)=(#0000b0) data:1233334231210111
  0x047bbb,0x00c03b,  //(r,g,b)=(#0000b8) data:1233343322111011
  0x0877b7,0x00c13c,  //(r,g,b)=(#0000c0) data:1233433322111102
  0x0a75b4,0x00c0b7,  //(r,g,b)=(#0000c8) data:1233434232110111
  0x146bab,0x00c03b,  //(r,g,b)=(#0000d0) data:1234343322111011
  0x146bcb,0x00a0cb,  //(r,g,b)=(#0000d8) data:1324343331201011
  0x1669c9,0x00c134,  //(r,g,b)=(#0000e0) data:1324344322110102
  0x0a75f5,0x00c03b,  //(r,g,b)=(#0000e8) data:1333434322111011
  0x0c73f3,0x00c0ab,  //(r,g,b)=(#0000f0) data:1333443332101011
  0x0e71f1,0x00c035,  //(r,g,b)=(#0000f8) data:1333444322110101
};


void __SPIConfig(void){
/*spi configulation*/
/* Enable enhanced buffer*/
/* baud rate is PBx / 4 */
  SPI2BRG = LOW_SPEED_SIGNAL;
  SPI3BRG = LOW_SPEED_SIGNAL;
  SPI4BRG = LOW_SPEED_SIGNAL;
  SPI5BRG = LOW_SPEED_SIGNAL;
  SPI6BRG = LOW_SPEED_SIGNAL;
  SPI5CON = 0x18120;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  SPI4CON = 0x18120;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  SPI3CON = 0x18120;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  SPI6CON = 0x18120;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  SPI2CON = 0x18120;
}


void Video_Init(void){
  __SPIConfig();
  asm("nop");
  asm("nop");
  /*TMR2 enable toggle interval:DMA_TRANSLATION_SIZE[byte] * 8[bits] * (2 * 2)SPIDIV*/
  PR2 = (DMA_TRANSLATION_SIZE<<(3+1+LOW_SPEED_SIGNAL))-1;//SPLLCON
  TMR2 = 0;
    
  /*DMA configulation*/
  DMACON = 0x8000;
  
  /* note :
   *  In the channels natural priority order, channel 0 has the highest priority. 
   */
  
  /*channel 0*/
  DCH0CON=0x12;// channel off, pri 2, no chaining, auto mode
  DCH0ECON=(_TIMER_2_VECTOR << 8)| 0x10;//set trigger :TIMER2 IRQ
  // program the transfer
  DCH0SSA=((uint32_t)ch0data)&0x1FFFFFFF;// transfer source physical address
  DCH0DSA=((uint32_t)(&SPI5BUF))&0x1FFFFFFF;// transfer destination physical address
  DCH0SSIZ=BUFFER_SIZE;// source size 15 bytes
  DCH0DSIZ=1;// destination size 1 bytes  
  DCH0CSIZ=DMA_TRANSLATION_SIZE;// 15 bytes transferred per event
  DCH0INTCLR=0x00ff00ff;// clear existing events, disable all interrupts
  DCH0CONSET=0x80;// turn channel on

  /*channel 1*/
  DCH1CON=0x12;
  DCH1ECON=(_TIMER_2_VECTOR << 8)| 0x10;  
  DCH1SSA=((uint32_t)ch1data)&0x1FFFFFFF;
  DCH1DSA=((uint32_t)(&SPI4BUF))&0x1FFFFFFF;
  DCH1SSIZ=BUFFER_SIZE;
  DCH1DSIZ=1;
  DCH1CSIZ=DMA_TRANSLATION_SIZE;
  DCH1INTCLR=0x00ff00ff;
  DCH1CONSET=0x80;

  /*channel 2*/
  DCH2CON=0x12;
  DCH2ECON=(_TIMER_2_VECTOR << 8)| 0x10;  
  DCH2SSA=((uint32_t)ch2data)&0x1FFFFFFF;
  DCH2DSA=((uint32_t)(&SPI3BUF))&0x1FFFFFFF;
  DCH2SSIZ=BUFFER_SIZE;
  DCH2DSIZ=1;
  DCH2CSIZ=DMA_TRANSLATION_SIZE;
  DCH2INTCLR=0x00ff00ff;
  DCH2CONSET=0x80;

  /*channel 3 low priority */
  DCH3CON=0x11;
  DCH3ECON=(_TIMER_2_VECTOR << 8)| 0x10;
  DCH3SSA=((uint32_t)ch3data)&0x1FFFFFFF;
  DCH3DSA=((uint32_t)(&SPI6BUF))&0x1FFFFFFF;
  DCH3SSIZ=BUFFER_SIZE;
  DCH3DSIZ=1;
  DCH3CSIZ=DMA_TRANSLATION_SIZE;
  DCH3INTCLR=0x00ff00ff;
  DCH3CONSET=0x80;

  /*channel 4 low */
  DCH4CON=0x11;
  DCH4ECON=(_TIMER_2_VECTOR << 8)| 0x10;
  DCH4SSA=((uint32_t)ch4data)&0x1FFFFFFF;
  DCH4DSA=((uint32_t)(&SPI2BUF))&0x1FFFFFFF;
  DCH4SSIZ=BUFFER_SIZE;
  DCH4DSIZ=1;
  DCH4CSIZ=DMA_TRANSLATION_SIZE;
  DCH4INTCLR=0x00ff00ff;
  DCH4CONSET=0x80;

  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  SPI5BUF = 0xFF;
  SPI5BUF = 0;
  SPI5BUF = 0;
  SPI4BUF = 0xFF;
  SPI4BUF = 0;
  SPI5BUF = 0;
  SPI3BUF = 0xFF;
  SPI4BUF = 0;
  SPI3BUF = 0;

  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  T2CONSET = 0x8000;//TMR2 ON
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
}

int calc_buffer_pointer(void){
  return DCH0SPTR / LINE_BUFFER_SIZE;
}

void main(void) {
  __builtin_mtc0(16, 0,(__builtin_mfc0(16, 0) | 0x3));

  CHECONbits.PFMWS = 2;
  //    CHECONbits.PREFEN = 0x1;

  /*remap pins*/
  RPB10R=0x0A; //RPB10:SCK5
  RPB11R=0x0E; //RPB11:SDO3
  RPB12R=0x04; //PRB12:SDO2
  RPB13R=0x11; //RPB13:SDO6
  RPB14R=0x0F; //RPB14:SDO4
  RPB15R=0x10; //RPB15:SDO5

  TRISBCLR = 1<<9;

  SYSKEY = 0x0;             // ensure OSCCON is locked
  SYSKEY = 0xAA996655;      // Write Key1 to SYSKEY
  SYSKEY = 0x556699AA;      // Write Key2 to SYSKEY
  // OSCCON is now unlocked //PB2DIV
  // make the desired change
  PB2DIV = 0x8000;
  PB3DIV = 0x8000;
  PB6DIVSET = 0x7F;
  // Relock the SYSKEY
  SYSKEY = 0x0;

  /* int i=0; */
  /* ch0data[i] = 0xAA; */
  /* ch1data[i] = 0xAA; */
  /* ch2data[i] = 0xAA; */
  /* //0xAD,0xF8,0xAD,0xF8,0x70 */

  /* for(i=1;i<BUFFER_SIZE;i++){ */
  /*   ch0data[i] = i/5000%3==0?0xFF:0; */
  /*   ch1data[i] = i/5000%3==1?0xFF:00; */
  /*   ch2data[i] = i/5000%3==2?0xFF:0; */
  /* } */

  Video_Init();  
  
  testapp();  
  
  while(1);
  return;
}

/* VSYNC 
 * 
 *  2                                        +-------+
 *                                           |       |
 *  0  --------------------------------------+       +--
 *     |<           420 3/8                 >|<33 5/8>|
 */

void VSYNC(uint8_t *ch0data,uint8_t *ch1data,uint8_t *ch2data){
  int i;
  for(i=0;i<420;i++){
    ch0data[i] = 0;
    ch1data[i] = 0;
    ch2data[i] = 0;
  }
  ch0data[i] = 0;
  ch1data[i] = 0xFF>>(8-3);
  ch2data[i] = 0;
  i++;
  for(;i<421+33;i++){
    ch0data[i] = 0;
    ch1data[i] = 0xFF;
    ch2data[i] = 0;
  }
}

/*  HSYNC and colorburst or so     
 *                                        +------------------+
 *                                +       |                  |         +
 *                                |       |   < NO IMAGE >   |         |
 *                -   -   -       |       |                  |         |
 *  +       +----/ \ / \ / \ /----+       +------------------+         +----+
 *  |       |       -   -   -                                               |
 *  +-------+                                                               +---
 *
 *  |<HSYNC>|    |<Colorburst>|   |<          Image Data              >|    |
 *  |33 5/8 |4 3/8|     18                        320
 * 
 *  |-33 5/8>| 38>|    56    >|74>|              394                  >|454 >|     
 */

void HSYNC_colorburst(uint8_t *ch0data,uint8_t *ch1data,uint8_t *ch2data){
  int i;
  /*HSYNC*/
  for(i=0;i<33;i++){
    ch0data[i] = 0;
    ch1data[i] = 0;
    ch2data[i] = 0;
  }
  ch0data[i] = 0x0;
  ch1data[i] = 0xFF>>5;
  ch2data[i] = 0x0;
  for(;i<38;i++){
    ch0data[i] = 0;
    ch1data[i] = 0xFF;
    ch2data[i] = 0;
  }
  /*colorburst*/
  for(;i<56;i++){
    ch0data[i] = 0x80;
    ch1data[i] = 0;
    ch2data[i] = 0;
    i++;
    ch0data[i] = 0xE5;
    ch1data[i] = 0x18;
    ch2data[i] = 0;
  }
  for(;i<454;i++){
    ch0data[i] = 0;
    ch1data[i] = 0xFF;
    ch2data[i] = 0;
  }
}

/*
  HSYNC COUNT ... 10
  ELSE  COUNT ... 252
  SUM 262
 */


// パルス幅定数
#define V_NTSC		262					// 262本/画面
#define V_SYNC		10					// 垂直同期本数
int vc;
void VTask(uint8_t *ch0data,uint8_t *ch1data,uint8_t *ch2data){
  uint8_t *vramp = vram+(drawing_line-60)*320;
  if(drawing_line < V_SYNC){
    VSYNC(ch0data,ch1data,ch2data);
    vc = 0;
  }else if(drawing_line < 60){
    HSYNC_colorburst(ch0data, ch1data, ch2data);
  }else if(drawing_line < 250){
    int i=96;
    for(;i<416;i++){
      uint32_t d;
      d = colordata[*vramp++*2];
      ch0data[i] = d;
      ch1data[i] = d>>8;
      ch2data[i] = d>>16;
      i++;
      d = colordata[*vramp++*2+1];
      ch0data[i] = d;
      ch1data[i] = d>>8;
      ch2data[i] = d>>16;
    }
    vc+=2;
    if(vc == 512)vc = 0;
  }
  drawing_line++;
  if(drawing_line==V_NTSC)drawing_line = 0;
}

void testapp(void){
  int i,j;
  for(i=0;i<255;i++){
    for(j=0;j<30;j++){
      vram[i+j*320] = i;
    }
  }
  for(i=0;i<127;i++){
    vram[i*321+320*30] = i*2;
  }
  while(1){
    for(i=0;i<NUM_OF_BUFFERS;i++){
      while(calc_buffer_pointer()!=(i+5)%NUM_OF_BUFFERS)LATBINV = 1<<9;
      VTask(ch0data+LINE_BUFFER_SIZE*i, ch1data+LINE_BUFFER_SIZE*i, ch2data+LINE_BUFFER_SIZE*i);
    }
  }
  while(1);
}
