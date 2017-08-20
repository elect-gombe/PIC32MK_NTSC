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
  0x002ad5,0x00956a,  //(r,g,b)=(#000000) data:1121212121121212
  0x0054ab,0x00aa55,  //(r,g,b)=(#080000) data:1212121121212121
  0x0055aa,0x002dd2,  //(r,g,b)=(#100000) data:1212121211212212
  0x006a95,0x0056a9,  //(r,g,b)=(#180000) data:1221212112121221
  0x00748b,0x009b64,  //(r,g,b)=(#200000) data:1222121121122122
  0x00d926,0x002fd0,  //(r,g,b)=(#280000) data:2212211211212222
  0x00ea15,0x002fd0,  //(r,g,b)=(#300000) data:2221212111212222
  0x00f40b,0x0057aa,  //(r,g,b)=(#380000) data:2222121112121232
  0x00f807,0x009f61,  //(r,g,b)=(#400000) data:2222211121122223
  0x00ea35,0x001fe2,  //(r,g,b)=(#480000) data:2231212111122232
  0x00f44b,0x002fd5,  //(r,g,b)=(#500000) data:2322121111212323
  0x00f847,0x003fc2,  //(r,g,b)=(#580000) data:2322211111222232
  0x00f45b,0x001fe3,  //(r,g,b)=(#600000) data:2323121111122233
  0x00f867,0x001fe7,  //(r,g,b)=(#680000) data:2332211111122333
  0x00f8c7,0x001feb,  //(r,g,b)=(#700000) data:3322211111123233
  0x00f4da,0x003fc7,  //(r,g,b)=(#780000) data:3323121011222333
  0x00f8e7,0x011ee6,  //(r,g,b)=(#800000) data:3332211111122334
  0x00f8e7,0x021de5,  //(r,g,b)=(#880000) data:3332211111122343
  0x00f4fa,0x011eee,  //(r,g,b)=(#900000) data:3333121011123334
  0x40b8a7,0x023d45,  //(r,g,b)=(#980000) data:3432211101222343
  0x40b8a7,0x053a82,  //(r,g,b)=(#a00000) data:3432211110222434
  0x40b8b6,0x031cec,  //(r,g,b)=(#a80000) data:3433211011123344
  0xa05847,0x031c6c,  //(r,g,b)=(#b00000) data:4342211101123344
  0xc03c21,0x071870,  //(r,g,b)=(#b80000) data:4432220101132444
  0xc03836,0x033c8c,  //(r,g,b)=(#c00000) data:4433211010223344
  0xc03836,0x031cee,  //(r,g,b)=(#c80000) data:4433211011123354
  0xe01419,0x071869,  //(r,g,b)=(#d00000) data:4443120101123445
  0xe01816,0x07186a,  //(r,g,b)=(#d80000) data:4443211001123454
  0xc03876,0x073889,  //(r,g,b)=(#e00000) data:4533211010223445
  0xe01855,0x07380b,  //(r,g,b)=(#e80000) data:4543210100223455
  0xe01856,0x07186b,  //(r,g,b)=(#f00000) data:4543211001123455
  0xe01c90,0x0f1063,  //(r,g,b)=(#f80000) data:5443220001124455
  0x002ad5,0x00956a,  //(r,g,b)=(#000000) data:1121212121121212
  0x0055aa,0x005aa5,  //(r,g,b)=(#080800) data:1212121212122121
  0x005aa5,0x00de21,  //(r,g,b)=(#101000) data:1212212122122221
  0x006d92,0x00df20,  //(r,g,b)=(#181800) data:1221221222122222
  0x007788,0x007f90,  //(r,g,b)=(#202000) data:1222122212232222
  0x00dd22,0x00ff12,  //(r,g,b)=(#282800) data:2212221222232232
  0x00f708,0x00ff15,  //(r,g,b)=(#303000) data:2222122222232323
  0x00ff00,0x00ff2a,  //(r,g,b)=(#383800) data:2222222222323232
  0x00ff00,0x00ff6e,  //(r,g,b)=(#404000) data:2222222223323332
  0x00ff20,0x00ff6f,  //(r,g,b)=(#484800) data:2232222223323333
  0x00ff41,0x00ff7f,  //(r,g,b)=(#505000) data:2322222323333333
  0x00ff48,0x02fdfd,  //(r,g,b)=(#585800) data:2322322233333343
  0x00ff61,0x25da5a,  //(r,g,b)=(#606000) data:2332222323433434
  0x00ffa4,0x2ad5d5,  //(r,g,b)=(#686800) data:3232232233434343
  0x00ffd2,0x2ed1d1,  //(r,g,b)=(#707000) data:3323223233434443
  0x00ffe5,0x2dd2da,  //(r,g,b)=(#787800) data:3332232333435434
  0x00ffea,0x7f8080,  //(r,g,b)=(#808000) data:3332323234444444
  0x00fff5,0x7f8084,  //(r,g,b)=(#888800) data:3333232334444544
  0x40bfab,0x7f8089,  //(r,g,b)=(#909000) data:3432323334445445
  0x40bfb5,0xbf406c,  //(r,g,b)=(#989800) data:3433232343545544
  0x40bfbb,0x7f80ae,  //(r,g,b)=(#a0a000) data:3433323334545554
  0x817e7c,0x7f80bd,  //(r,g,b)=(#a8a800) data:4333332434555545
  0xa05f57,0xff003f,  //(r,g,b)=(#b0b000) data:4343233344555555
  0xc03f3f,0xff043b,  //(r,g,b)=(#b8b800) data:4433333344555655
  0xc13e3e,0x7f8af5,  //(r,g,b)=(#c0c000) data:4433333435556565
  0xe01f1f,0xff0c73,  //(r,g,b)=(#c8c800) data:4443333345556655
  0xe11e1e,0xff0e71,  //(r,g,b)=(#d0d000) data:4443333445556665
  0xd02f6f,0xff3e81,  //(r,g,b)=(#d8d800) data:4534333354666665
  0xe11e9e,0xff3f40,  //(r,g,b)=(#e0e000) data:5443333445666666
  0xe91696,0xff3f44,  //(r,g,b)=(#e8e800) data:5443433445666766
  0xf20d8d,0xff3fca,  //(r,g,b)=(#f0f000) data:5444334355667676
  0xe51ada,0xff3fcc,  //(r,g,b)=(#f8f800) data:5543343455667766
  0x002ad5,0x00956a,  //(r,g,b)=(#000000) data:1121212121121212
  0x0055aa,0x0055aa,  //(r,g,b)=(#040404) data:1212121212121212
  0x0056a9,0x00ad52,  //(r,g,b)=(#080808) data:1212122121212212
  0x005ba4,0x006b94,  //(r,g,b)=(#0c0c0c) data:1212212212212122
  0x006d92,0x00db24,  //(r,g,b)=(#101010) data:1221221222122122
  0x007788,0x007788,  //(r,g,b)=(#141414) data:1222122212221222
  0x007b84,0x00ef10,  //(r,g,b)=(#181818) data:1222212222212222
  0x007f80,0x00bf40,  //(r,g,b)=(#1c1c1c) data:1222222221222222
  0x007f80,0x00ff00,  //(r,g,b)=(#202020) data:1222222222222222
  0x00ff00,0x00ff00,  //(r,g,b)=(#242424) data:2222222222222222
  0x00ff00,0x00ff10,  //(r,g,b)=(#282828) data:2222222222232222
  0x00ff02,0x00ff08,  //(r,g,b)=(#2c2c2c) data:2222223222223222
  0x00ff08,0x00ff44,  //(r,g,b)=(#303030) data:2222322223222322
  0x00ff11,0x00ff22,  //(r,g,b)=(#343434) data:2223222322322232
  0x00ff24,0x00ff92,  //(r,g,b)=(#383838) data:2232232232232232
  0x00ff29,0x00ff4a,  //(r,g,b)=(#3c3c3c) data:2232322323223232
  0x00ff2a,0x00ffaa,  //(r,g,b)=(#404040) data:2232323232323232
  0x00ff55,0x00ff5a,  //(r,g,b)=(#444444) data:2323232323233232
  0x00ff5a,0x00ffd6,  //(r,g,b)=(#484848) data:2323323233232332
  0x00ff6d,0x00ffb6,  //(r,g,b)=(#4c4c4c) data:2332332332332332
  0x00ff6e,0x00ffee,  //(r,g,b)=(#505050) data:2332333233323332
  0x00ff7b,0x00ffde,  //(r,g,b)=(#545454) data:2333323333233332
  0x00ff7e,0x00fffd,  //(r,g,b)=(#585858) data:2333333233333323
  0x00ff7f,0x00fffd,  //(r,g,b)=(#5c5c5c) data:2333333333333323
  0x00ff7f,0x00ffff,  //(r,g,b)=(#606060) data:2333333333333333
  0x00ffff,0x00ffff,  //(r,g,b)=(#646464) data:3333333333333333
  0x00ffff,0x807f7f,  //(r,g,b)=(#686868) data:3333333343333333
  0x04fbfb,0x21dede,  //(r,g,b)=(#6c6c6c) data:3333343333433334
  0x11eeee,0x11eeee,  //(r,g,b)=(#707070) data:3334333433343334
  0x12eded,0x49b6b6,  //(r,g,b)=(#747474) data:3334334334334334
  0x25dada,0x25dada,  //(r,g,b)=(#787878) data:3343343433433434
  0x2ad5d5,0x956a6a,  //(r,g,b)=(#7c7c7c) data:3343434343343434
  0x55aaaa,0x55aaaa,  //(r,g,b)=(#808080) data:3434343434343434
  0x56a9a9,0xad5252,  //(r,g,b)=(#848484) data:3434344343434434
  0x5ba4a4,0x6b9494,  //(r,g,b)=(#888888) data:3434434434434344
  0x6d9292,0xdb2424,  //(r,g,b)=(#8c8c8c) data:3443443444344344
  0x778888,0x778888,  //(r,g,b)=(#909090) data:3444344434443444
  0x7b8484,0xef1010,  //(r,g,b)=(#949494) data:3444434444434444
  0x7f8080,0xbf4040,  //(r,g,b)=(#989898) data:3444444443444444
  0x7f8080,0xff0000,  //(r,g,b)=(#9c9c9c) data:3444444444444444
  0xff0000,0xff0000,  //(r,g,b)=(#a0a0a0) data:4444444444444444
  0xff0000,0xff0020,  //(r,g,b)=(#a4a4a4) data:4444444444544444
  0xff0002,0xff0008,  //(r,g,b)=(#a8a8a8) data:4444445444445444
  0xff0008,0xff0044,  //(r,g,b)=(#acacac) data:4444544445444544
  0xff0011,0xff0022,  //(r,g,b)=(#b0b0b0) data:4445444544544454
  0xff0024,0xff0092,  //(r,g,b)=(#b4b4b4) data:4454454454454454
  0xff0029,0xff004a,  //(r,g,b)=(#b8b8b8) data:4454544545445454
  0xff002a,0xff00aa,  //(r,g,b)=(#bcbcbc) data:4454545454545454
  0xff0055,0xff005a,  //(r,g,b)=(#c0c0c0) data:4545454545455454
  0xff005a,0xff00d6,  //(r,g,b)=(#c4c4c4) data:4545545455454554
  0xff006d,0xff00b6,  //(r,g,b)=(#c8c8c8) data:4554554554554554
  0xff006e,0xff00ee,  //(r,g,b)=(#cccccc) data:4554555455545554
  0xff007b,0xff00de,  //(r,g,b)=(#d0d0d0) data:4555545555455554
  0xff007e,0xff00fd,  //(r,g,b)=(#d4d4d4) data:4555555455555545
  0xff007f,0xff00fd,  //(r,g,b)=(#d8d8d8) data:4555555555555545
  0xff007f,0xff00ff,  //(r,g,b)=(#dcdcdc) data:4555555555555555
  0xff00ff,0xff00ff,  //(r,g,b)=(#e0e0e0) data:5555555555555555
  0xff00ff,0xff807f,  //(r,g,b)=(#e4e4e4) data:5555555565555555
  0xff04fb,0xff21de,  //(r,g,b)=(#e8e8e8) data:5555565555655556
  0xff11ee,0xff11ee,  //(r,g,b)=(#ececec) data:5556555655565556
  0xff12ed,0xff49b6,  //(r,g,b)=(#f0f0f0) data:5556556556556556
  0xff25da,0xff25da,  //(r,g,b)=(#f4f4f4) data:5565565655655656
  0xff2ad5,0xff956a,  //(r,g,b)=(#f8f8f8) data:5565656565565656
  0xff55aa,0xff55aa,  //(r,g,b)=(#fcfcfc) data:5656565656565656
  0x002ad5,0x00956a,  //(r,g,b)=(#000000) data:1121212121121212
  0x0055aa,0x004ab5,  //(r,g,b)=(#080008) data:1212121212112121
  0x006a95,0x00aa55,  //(r,g,b)=(#100010) data:1221212121212121
  0x007689,0x00956a,  //(r,g,b)=(#180018) data:1222122121121212
  0x007d82,0x004bb4,  //(r,g,b)=(#200020) data:1222221212112122
  0x00fa05,0x00a758,  //(r,g,b)=(#280028) data:2222212121211222
  0x00fe01,0x009768,  //(r,g,b)=(#300030) data:2222222121121222
  0x00fd12,0x002bd5,  //(r,g,b)=(#380038) data:2223221211212123
  0x00ff20,0x0017e8,  //(r,g,b)=(#400040) data:2232222211121222
  0x00fe51,0x008f70,  //(r,g,b)=(#480048) data:2323222121112222
  0x00fd6a,0x002fd0,  //(r,g,b)=(#500050) data:2332321211212222
  0x00ff70,0x0017e9,  //(r,g,b)=(#580058) data:2333222211121223
  0x00fe79,0x008f71,  //(r,g,b)=(#600060) data:2333322121112223
  0x00fdf6,0x002fd1,  //(r,g,b)=(#680068) data:3333231211212223
  0x00fff8,0x0017eb,  //(r,g,b)=(#700070) data:3333322211121233
  0x10eee5,0x008f71,  //(r,g,b)=(#780078) data:3334232121112223
  0x20dfd8,0x0017ed,  //(r,g,b)=(#800080) data:3343322211121323
  0x50afa8,0x000ff3,  //(r,g,b)=(#880088) data:3434322211112233
  0x609e9d,0x008f73,  //(r,g,b)=(#900090) data:3443332121112233
  0x708f88,0x0116ec,  //(r,g,b)=(#980098) data:3444322211121324
  0x708f8c,0x010ef2,  //(r,g,b)=(#a000a0) data:3444332211112234
  0x708ecd,0x01ae12,  //(r,g,b)=(#a800a8) data:3544332120212234
  0xf00f0c,0x000ff7,  //(r,g,b)=(#b000b0) data:4444332211112333
  0xf00f1c,0x010ef2,  //(r,g,b)=(#b800b8) data:4445332211112234
  0xf80625,0x019e22,  //(r,g,b)=(#c000c0) data:4454432120122234
  0xf80744,0x010ef6,  //(r,g,b)=(#c800c8) data:4544432211112334
  0xf80764,0x030cf0,  //(r,g,b)=(#d000d0) data:4554432211112244
  0xf80667,0x01962f,  //(r,g,b)=(#d800d8) data:4554433120121335
  0xf80774,0x010ef7,  //(r,g,b)=(#e000e0) data:4555432211112335
  0xf8472c,0x030cf0,  //(r,g,b)=(#e800e8) data:4654532211112244
  0xf806f7,0x038c34,  //(r,g,b)=(#f000f0) data:5555433120112344
  0xf806ff,0x030cf4,  //(r,g,b)=(#f800f8) data:5555533111112344
  0x002ad5,0x00956a,  //(r,g,b)=(#000000) data:1121212121121212
  0x002ad5,0x00d52a,  //(r,g,b)=(#000800) data:1121212122121212
  0x002bd4,0x007689,  //(r,g,b)=(#001000) data:1121212212221221
  0x002dd2,0x00f609,  //(r,g,b)=(#001800) data:1121221222221221
  0x002ed1,0x00f649,  //(r,g,b)=(#002000) data:1121222123221221
  0x002fd0,0x00fb0c,  //(r,g,b)=(#002800) data:1121222222223122
  0x002fd0,0x00fd4a,  //(r,g,b)=(#003000) data:1121222223223212
  0x002fd1,0x00fd2a,  //(r,g,b)=(#003800) data:1121222322323212
  0x0037c9,0x00fe69,  //(r,g,b)=(#004000) data:1122122323323221
  0x0037ca,0x00ffe8,  //(r,g,b)=(#004800) data:1122123233323222
  0x0037cd,0x00ff78,  //(r,g,b)=(#005000) data:1122132323333222
  0x0037cd,0x00fff8,  //(r,g,b)=(#005800) data:1122132333333222
  0x005fa1,0x40bfb8,  //(r,g,b)=(#006000) data:1212222334333222
  0x005fa3,0x20dedb,  //(r,g,b)=(#006800) data:1212223333433231
  0x005fa3,0x609f9c,  //(r,g,b)=(#007000) data:1212223334433322
  0x005fa3,0xd02f2c,  //(r,g,b)=(#007800) data:1212223344343322
  0x015ea2,0x708f8c,  //(r,g,b)=(#008000) data:1212223434443322
  0x025da1,0xf00f0c,  //(r,g,b)=(#008800) data:1212224344443322
  0x005fa7,0xf00f4c,  //(r,g,b)=(#009000) data:1212233345443322
  0x015ea6,0xf00f2e,  //(r,g,b)=(#009800) data:1212233444543332
  0x015ea6,0xf80746,  //(r,g,b)=(#00a000) data:1212233445444332
  0x015ea6,0xf807c6,  //(r,g,b)=(#00a800) data:1212233455444332
  0x035ca4,0xfc0360,  //(r,g,b)=(#00b000) data:1212234445544422
  0x035ca4,0xfc03e1,  //(r,g,b)=(#00b800) data:1212234455544423
  0x035ca5,0xfc0372,  //(r,g,b)=(#00c000) data:1212234545554432
  0x035ca5,0xfc03f2,  //(r,g,b)=(#00c800) data:1212234555554432
  0x0758a0,0xfc43b2,  //(r,g,b)=(#00d000) data:1212244456554432
  0x035cad,0xfc23d2,  //(r,g,b)=(#00d800) data:1212334555654432
  0x035cad,0xf8679f,  //(r,g,b)=(#00e000) data:1212334556655333
  0x035cad,0xfad52c,  //(r,g,b)=(#00e800) data:1212334566565342
  0x035dac,0xfc738a,  //(r,g,b)=(#00f000) data:1212334656665432
  0x035caf,0xfcf30a,  //(r,g,b)=(#00f800) data:1212335566665432
  0x002ad5,0x00956a,  //(r,g,b)=(#000000) data:1121212121121212
  0x002dd2,0x006a95,  //(r,g,b)=(#000808) data:1121221212212121
  0x0037c8,0x006d92,  //(r,g,b)=(#001010) data:1122122212212212
  0x003bc4,0x00f50a,  //(r,g,b)=(#001818) data:1122212222221212
  0x005fa0,0x00fa05,  //(r,g,b)=(#002020) data:1212222222222121
  0x005fa1,0x00fd02,  //(r,g,b)=(#002828) data:1212222322222212
  0x005fa5,0x00fa25,  //(r,g,b)=(#003030) data:1212232322322121
  0x005faa,0x00fda2,  //(r,g,b)=(#003838) data:1212323232322212
  0x007f83,0x00fea1,  //(r,g,b)=(#004040) data:1222223332322221
  0x007f87,0x00fbd4,  //(r,g,b)=(#004848) data:1222233333232122
  0x007f87,0x807e51,  //(r,g,b)=(#005050) data:1222233343232221
  0x007f8f,0x807f60,  //(r,g,b)=(#005858) data:1222333343322222
  0x017e8e,0x40bdaa,  //(r,g,b)=(#006060) data:1222333434323212
  0x037c8c,0x40beb1,  //(r,g,b)=(#006868) data:1222334434332221
  0x027d9d,0xc03d3a,  //(r,g,b)=(#007070) data:1223334344333212
  0x037c9c,0xc03e39,  //(r,g,b)=(#007878) data:1223334444333221
  0x077898,0xe01f10,  //(r,g,b)=(#008080) data:1223344444432222
  0x077899,0xe01e19,  //(r,g,b)=(#008888) data:1223344544433221
  0x0f7090,0xe01f98,  //(r,g,b)=(#009090) data:1223444454433222
  0x0f7091,0xf00d86,  //(r,g,b)=(#009898) data:1223444554442312
  0x0778bb,0xf00f88,  //(r,g,b)=(#00a0a0) data:1233345554443222
  0x0778bf,0xe01fb8,  //(r,g,b)=(#00a8a8) data:1233355554533222
  0x0f70b3,0xf08e29,  //(r,g,b)=(#00b0b0) data:1233445564543221
  0x0f70b7,0xf00fe8,  //(r,g,b)=(#00b8b8) data:1233455555543222
  0x0f71b6,0xf04e8d,  //(r,g,b)=(#00c0c0) data:1233455656443321
  0x0f73b4,0xf84780,  //(r,g,b)=(#00c8c8) data:1233456656444222
  0x0f71be,0xf0ce2d,  //(r,g,b)=(#00d0d0) data:1233555666543321
  0x1f63a4,0xf0cf2c,  //(r,g,b)=(#00d8d8) data:1234456666543322
  0x1f67a0,0xf8c720,  //(r,g,b)=(#00e0e0) data:1234466666544222
  0x1f63ad,0xf0cf3c,  //(r,g,b)=(#00e8e8) data:1234556766553322
  0x1f63ae,0xf8c7b0,  //(r,g,b)=(#00f0f0) data:1234557676554222
  0x1f67a9,0xf8e784,  //(r,g,b)=(#00f8f8) data:1234566776644322
  0x002ad5,0x00956a,  //(r,g,b)=(#000000) data:1121212121121212
  0x002ad5,0x00a956,  //(r,g,b)=(#000008) data:1121212121212112
  0x0035ca,0x0052ad,  //(r,g,b)=(#000010) data:1122121212121121
  0x0036c9,0x00a45b,  //(r,g,b)=(#000018) data:1122122121211211
  0x003bc4,0x0049b6,  //(r,g,b)=(#000020) data:1122212212112112
  0x005da2,0x00916e,  //(r,g,b)=(#000028) data:1212221221121112
  0x005ea1,0x00a25d,  //(r,g,b)=(#000030) data:1212222121211121
  0x005fa0,0x0048b7,  //(r,g,b)=(#000038) data:1212222212112111
  0x005fa0,0x00a15e,  //(r,g,b)=(#000040) data:1212222221211112
  0x005fa8,0x0041be,  //(r,g,b)=(#000048) data:1212322212111112
  0x007f80,0x00906f,  //(r,g,b)=(#000050) data:1222222221121111
  0x007f80,0x00c03f,  //(r,g,b)=(#000058) data:1222222222111111
  0x007f82,0x0040bf,  //(r,g,b)=(#000060) data:1222223212111111
  0x007f84,0x00a15a,  //(r,g,b)=(#000068) data:1222232221211012
  0x007f88,0x00c03f,  //(r,g,b)=(#000070) data:1222322222111111
  0x007f8a,0x00807f,  //(r,g,b)=(#000078) data:1222323221111111
  0x007f8c,0x00c03b,  //(r,g,b)=(#000080) data:1222332222111011
  0x007f95,0x00807f,  //(r,g,b)=(#000088) data:1223232321111111
  0x007f96,0x00c037,  //(r,g,b)=(#000090) data:1223233222110111
  0x007f9a,0x00c03b,  //(r,g,b)=(#000098) data:1223323222111011
  0x007f9d,0x00807d,  //(r,g,b)=(#0000a0) data:1223332321111101
  0x007f9e,0x00c037,  //(r,g,b)=(#0000a8) data:1223333222110111
  0x007f9f,0x00807b,  //(r,g,b)=(#0000b0) data:1223333321111011
  0x047b99,0x00c02b,  //(r,g,b)=(#0000b8) data:1223342322101011
  0x0877a6,0x00c035,  //(r,g,b)=(#0000c0) data:1232433222110101
  0x007fbf,0x00a04b,  //(r,g,b)=(#0000c8) data:1233333321201011
  0x007fbf,0x00c033,  //(r,g,b)=(#0000d0) data:1233333322110011
  0x047bbb,0x008075,  //(r,g,b)=(#0000d8) data:1233343321110101
  0x047bbb,0x00c029,  //(r,g,b)=(#0000e0) data:1233343322101001
  0x0877b7,0x0080f2,  //(r,g,b)=(#0000e8) data:1233433331110010
  0x0a75b5,0x00c023,  //(r,g,b)=(#0000f0) data:1233434322100011
  0x0c73b3,0x00c029,  //(r,g,b)=(#0000f8) data:1233443322101001
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
  ch0data[i] = 0x55>>(8-3);
  ch1data[i] = 0xAA>>(8-3);
  ch2data[i] = 0;
  i++;
  for(;i<421+33;i++){
    ch0data[i] = 0x55;
    ch1data[i] = 0xAA;
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
  ch0data[i] = 0x55>>5;
  ch1data[i] = 0xAA>>5;
  ch2data[i] = 0x0;
  for(;i<38;i++){
    ch0data[i] = 0x55;
    ch1data[i] = 0xAA;
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
    ch0data[i] = 0x55;
    ch1data[i] = 0xAA;
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
  uint8_t *vramp = vram+(drawing_line-45)*320;
  if(drawing_line < V_SYNC){
    VSYNC(ch0data,ch1data,ch2data);
    vc = 0;
  }else if(drawing_line < 45){
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
    vram[i*321+320*30] = i;
  }
  for(i=0;i<127;i++){
    vram[i*321+320*30+20] = i+127;
  }
  while(1){
    for(i=0;i<NUM_OF_BUFFERS;i++){
      while(calc_buffer_pointer()!=(i+5)%NUM_OF_BUFFERS)LATBINV = 1<<9;
      VTask(ch0data+LINE_BUFFER_SIZE*i, ch1data+LINE_BUFFER_SIZE*i, ch2data+LINE_BUFFER_SIZE*i);
    }
  }
  while(1);
}
