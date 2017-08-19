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

uint8_t ch0data[BUFFER_SIZE];
uint8_t ch1data[BUFFER_SIZE];
uint8_t ch2data[BUFFER_SIZE];
uint8_t ch3data[BUFFER_SIZE];
uint8_t ch4data[BUFFER_SIZE];

int drawing_line;

#define LOW_SPEED_SIGNAL 0

uint8_t vram[320*220];

uint32_t colordata[]={
  0x005ba4,0x006d92,  //(r,g,b)=(#000000) 1212212212212212
  0x00ba45,0x00db24,  //(r,g,b)=(#080000) 2122212122122122
  0x00ed12,0x006f90,  //(r,g,b)=(#100000) 2221221212212222
  0x00ed12,0x005fa0,  //(r,g,b)=(#180000) 2221221212122222
  0x00fa05,0x00bf40,  //(r,g,b)=(#200000) 2222212121222222
  0x00fd02,0x003fc0,  //(r,g,b)=(#280000) 2222221211222222
  0x00fa85,0x00bf40,  //(r,g,b)=(#300000) 3222212121222222
  0x00f5aa,0x005fa2,  //(r,g,b)=(#380000) 3232121212122232
  0x00f9a6,0x005fa3,  //(r,g,b)=(#400000) 3232211212122233
  0x00fc43,0x00bf43,  //(r,g,b)=(#480000) 2322221121222233
  0x00fa65,0x003fc5,  //(r,g,b)=(#500000) 2332212111222323
  0x00f9e6,0x003fc5,  //(r,g,b)=(#580000) 3332211211222323
  0x807c23,0x005fab,  //(r,g,b)=(#600000) 4232221112123233
  0x00f4fb,0x025da9,  //(r,g,b)=(#680000) 3333121112123243
  0x00fce3,0x013ec6,  //(r,g,b)=(#700000) 3332221111222334
  0x00f8f7,0x015eae,  //(r,g,b)=(#780000) 3333211112123334
  0x00f8f7,0x015eae,  //(r,g,b)=(#800000) 3333211112123334
  0x40bca3,0x053ac2,  //(r,g,b)=(#880000) 3432221111222434
  0x807877,0x013ece,  //(r,g,b)=(#900000) 4333211111223334
  0xc03837,0x023dcd,  //(r,g,b)=(#980000) 4433211111223343
  0xc03c33,0x033c4c,  //(r,g,b)=(#a00000) 4433221101223344
  0xe01c03,0x053a4b,  //(r,g,b)=(#a80000) 4442221101223435
  0xc03c32,0x033ccd,  //(r,g,b)=(#b00000) 4433221011223345
  0xe0180f,0x073848,  //(r,g,b)=(#b80000) 4442311101223444
  0xe0188f,0x073849,  //(r,g,b)=(#c00000) 5442311101223445
  0xe01c11,0x031cfe,  //(r,g,b)=(#c80000) 4443220111133354
  0xe01c92,0x0718f9,  //(r,g,b)=(#d00000) 5443221011133445
  0xe01c91,0x0f3081,  //(r,g,b)=(#d80000) 5443220110224445
  0xd028e6,0x0718fb,  //(r,g,b)=(#e00000) 5534211011133455
  0xf00886,0x0718fb,  //(r,g,b)=(#e80000) 5444211011133455
  0xe018de,0x0f3043,  //(r,g,b)=(#f00000) 5543311001224455
  0xf008c6,0x0f3182,  //(r,g,b)=(#f80000) 5544211010224456
  0x006b94,0x006d92,  //(r,g,b)=(#000000) 1221212212212212
  0x00db24,0x00bd42,  //(r,g,b)=(#080800) 2212212221222212
  0x00f708,0x00df20,  //(r,g,b)=(#101000) 2222122222122222
  0x00ff00,0x007fa0,  //(r,g,b)=(#181800) 2222222212322222
  0x00df60,0x00ff22,  //(r,g,b)=(#202000) 2312222222322232
  0x00f728,0x00ff95,  //(r,g,b)=(#282800) 2232122232232323
  0x00ff10,0x00ff2d,  //(r,g,b)=(#303000) 2223222222323323
  0x00ff82,0x00ff5e,  //(r,g,b)=(#383800) 3222223223233332
  0x00ffa1,0x08f765,  //(r,g,b)=(#404000) 3232222323324323
  0x00ffc8,0x00ffff,  //(r,g,b)=(#484800) 3322322233333333
  0x00ffd1,0x24db9b,  //(r,g,b)=(#505000) 3323222332433433
  0x00ffd2,0x15eaea,  //(r,g,b)=(#585800) 3323223233343434
  0x00ffd2,0x2dd2d2,  //(r,g,b)=(#606000) 3323223233434434
  0x00ffea,0x5ea1a1,  //(r,g,b)=(#686800) 3332323234344443
  0x807f5a,0x5fa0a0,  //(r,g,b)=(#707000) 4323323234344444
  0x807f75,0x7f8080,  //(r,g,b)=(#787800) 4333232334444444
  0x807f77,0x3fc0d2,  //(r,g,b)=(#808000) 4333233333454454
  0x807f77,0x7f8094,  //(r,g,b)=(#888800) 4333233334454544
  0xc03f37,0xbf406d,  //(r,g,b)=(#909000) 4433233343545545
  0xa05f57,0xff001d,  //(r,g,b)=(#989800) 4343233344455545
  0xc03f3f,0xff001f,  //(r,g,b)=(#a0a000) 4433333344455555
  0xd12e26,0x7f88d7,  //(r,g,b)=(#a8a800) 4434233435456555
  0xe01f1f,0xff0837,  //(r,g,b)=(#b0b000) 4443333344556555
  0xe01f9f,0xff0a75,  //(r,g,b)=(#b8b800) 5443333345556565
  0xd12eae,0xff1a25,  //(r,g,b)=(#c0c000) 5434333444566565
  0xe91696,0xff0e71,  //(r,g,b)=(#c8c800) 5443433445556665
  0xe21ddd,0xff1ee1,  //(r,g,b)=(#d0d000) 5543334355566665
  0xe99616,0xff3e45,  //(r,g,b)=(#d8d800) 6443433445666765
  0xf30ccc,0xff3f44,  //(r,g,b)=(#e0e000) 5544334445666766
  0xf30ccc,0xff7f04,  //(r,g,b)=(#e8e800) 5544334446666766
  0xf38c4c,0xff3fd6,  //(r,g,b)=(#f0f000) 6544334455676776
  0xfb04c4,0xff3fde,  //(r,g,b)=(#f8f800) 5544434455677776
  0x00ad52,0x00b649,  //(r,g,b)=(#000000) 2121221221221221
  0x00dd22,0x00bb44,  //(r,g,b)=(#040404) 2212221221222122
  0x00bb44,0x00dd22,  //(r,g,b)=(#080808) 2122212222122212
  0x00f708,0x00ef10,  //(r,g,b)=(#0c0c0c) 2222122222212222
  0x00ef10,0x00fd02,  //(r,g,b)=(#101010) 2221222222222212
  0x00ff00,0x00ff00,  //(r,g,b)=(#141414) 2222222222222222
  0x00ff00,0x00ff04,  //(r,g,b)=(#181818) 2222222222222322
  0x00ff00,0x00ff80,  //(r,g,b)=(#1c1c1c) 2222222232222222
  0x00ff41,0x00ff04,  //(r,g,b)=(#202020) 2322222322222322
  0x00ff22,0x00ff11,  //(r,g,b)=(#242424) 2232223222232223
  0x00ff22,0x00ff48,  //(r,g,b)=(#282828) 2232223223223222
  0x00ff94,0x00ff92,  //(r,g,b)=(#2c2c2c) 3223232232232232
  0x00ff95,0x00ff2a,  //(r,g,b)=(#303030) 3223232322323232
  0x00ff95,0x00ff55,  //(r,g,b)=(#343434) 3223232323232323
  0x00ff6a,0x00ffb5,  //(r,g,b)=(#383838) 2332323232332323
  0x00ff6b,0x00ff6b,  //(r,g,b)=(#3c3c3c) 2332323323323233
  0x00ff6d,0x00ffdb,  //(r,g,b)=(#404040) 2332332333233233
  0x00ff77,0x00ff77,  //(r,g,b)=(#444444) 2333233323332333
  0x00ff7d,0x00ffef,  //(r,g,b)=(#484848) 2333332333323333
  0x00ffbf,0x00ffbf,  //(r,g,b)=(#4c4c4c) 3233333332333333
  0x00ffef,0x00ffff,  //(r,g,b)=(#505050) 3332333333333333
  0x00ffff,0x00ffff,  //(r,g,b)=(#545454) 3333333333333333
  0x00ffff,0x807f7f,  //(r,g,b)=(#585858) 3333333343333333
  0x10efef,0x20dfdf,  //(r,g,b)=(#5c5c5c) 3334333333433333
  0x42bdbd,0x10efef,  //(r,g,b)=(#606060) 3433334333343333
  0x897676,0x11eeee,  //(r,g,b)=(#646464) 4333433433343334
  0x24dbdb,0x926d6d,  //(r,g,b)=(#686868) 3343343343343343
  0x52adad,0x52adad,  //(r,g,b)=(#6c6c6c) 3434334334343343
  0xa95656,0x55aaaa,  //(r,g,b)=(#707070) 4343433434343434
  0x55aaaa,0x55aaaa,  //(r,g,b)=(#747474) 3434343434343434
  0xab5454,0x5aa5a5,  //(r,g,b)=(#787878) 4343434434344343
  0xdb2424,0x6d9292,  //(r,g,b)=(#7c7c7c) 4434434434434434
  0xb74848,0x6e9191,  //(r,g,b)=(#808080) 4344344434434443
  0xef1010,0x778888,  //(r,g,b)=(#848484) 4443444434443444
  0xbf4040,0x7d8282,  //(r,g,b)=(#888888) 4344444434444434
  0xff0000,0xbf4040,  //(r,g,b)=(#8c8c8c) 4444444443444444
  0xff0000,0xff0000,  //(r,g,b)=(#909090) 4444444444444444
  0xff0000,0xff0000,  //(r,g,b)=(#949494) 4444444444444444
  0xff0010,0xff0004,  //(r,g,b)=(#989898) 4445444444444544
  0xff0008,0xff0021,  //(r,g,b)=(#9c9c9c) 4444544444544445
  0xff0008,0xff0088,  //(r,g,b)=(#a0a0a0) 4444544454445444
  0xff0089,0xff0022,  //(r,g,b)=(#a4a4a4) 5444544544544454
  0xff004a,0xff0049,  //(r,g,b)=(#a8a8a8) 4544545445445445
  0xff0052,0xff0095,  //(r,g,b)=(#acacac) 4545445454454545
  0xff0055,0xff0054,  //(r,g,b)=(#b0b0b0) 4545454545454544
  0xff00d5,0xff006a,  //(r,g,b)=(#b4b4b4) 5545454545545454
  0xff00d6,0xff00d6,  //(r,g,b)=(#b8b8b8) 5545455455454554
  0xff00db,0xff0076,  //(r,g,b)=(#bcbcbc) 5545545545554554
  0xff00ee,0xff00ee,  //(r,g,b)=(#c0c0c0) 5554555455545554
  0xff00f7,0xff00bd,  //(r,g,b)=(#c4c4c4) 5555455554555545
  0xff00fd,0xff00fd,  //(r,g,b)=(#c8c8c8) 5555554555555545
  0xff00ff,0xff00fd,  //(r,g,b)=(#cccccc) 5555555555555545
  0xff00ff,0xff807f,  //(r,g,b)=(#d0d0d0) 5555555565555555
  0xff00ff,0xff04fb,  //(r,g,b)=(#d4d4d4) 5555555555555655
  0xff04fb,0xff04fb,  //(r,g,b)=(#d8d8d8) 5555565555555655
  0xff11ee,0xff08f7,  //(r,g,b)=(#dcdcdc) 5556555655556555
  0xff44bb,0xff8877,  //(r,g,b)=(#e0e0e0) 5655565565556555
  0xff926d,0xff49b6,  //(r,g,b)=(#e4e4e4) 6556556556556556
  0xff29d6,0xff4ab5,  //(r,g,b)=(#e8e8e8) 5565655656556565
  0xff55aa,0xff4ab5,  //(r,g,b)=(#ececec) 5656565656556565
  0xffab54,0xff55aa,  //(r,g,b)=(#f0f0f0) 6565656656565656
  0xff5aa5,0xffd629,  //(r,g,b)=(#f4f4f4) 5656656566565665
  0xffb649,0xffdb24,  //(r,g,b)=(#f8f8f8) 6566566566566566
  0xff6e91,0xffdd22,  //(r,g,b)=(#fcfcfc) 5665666566566656
  0x00b649,0x00da25,  //(r,g,b)=(#000000) 2122122122122121
  0x00f609,0x00db24,  //(r,g,b)=(#080008) 2222122122122122
  0x00de21,0x00db24,  //(r,g,b)=(#100010) 2212222122122122
  0x00fd02,0x00b748,  //(r,g,b)=(#180018) 2222221221221222
  0x00ff00,0x006f90,  //(r,g,b)=(#200020) 2222222212212222
  0x00fe21,0x00b748,  //(r,g,b)=(#280028) 2232222121221222
  0x00ffa0,0x005fa0,  //(r,g,b)=(#300030) 3232222212122222
  0x00ff60,0x00af50,  //(r,g,b)=(#380038) 2332222221212222
  0x00ffd0,0x00af51,  //(r,g,b)=(#400040) 3323222221212223
  0x00ff70,0x009f61,  //(r,g,b)=(#480048) 2333222221122223
  0x00ffd8,0x005fa1,  //(r,g,b)=(#500050) 3323322212122223
  0x00fef5,0x00af53,  //(r,g,b)=(#580058) 3333232121212233
  0x00fff8,0x005fa1,  //(r,g,b)=(#600060) 3333322212122223
  0x40bebb,0x009f63,  //(r,g,b)=(#680068) 3433323121122233
  0x20dedd,0x00af53,  //(r,g,b)=(#700070) 3343332121212233
  0xa05e5d,0x00af53,  //(r,g,b)=(#780078) 4343332121212233
  0xd02e2b,0x009f63,  //(r,g,b)=(#800080) 4434323121122233
  0xe01f1c,0x009f63,  //(r,g,b)=(#880088) 4443332221122233
  0xf00f0c,0x002fd7,  //(r,g,b)=(#900090) 4444332211212333
  0xf00f0c,0x019e62,  //(r,g,b)=(#980098) 4444332221122234
  0xf80702,0x021de1,  //(r,g,b)=(#a000a0) 4444423211122243
  0xf80744,0x012ed6,  //(r,g,b)=(#a800a8) 4544432211212334
  0xf80724,0x011ee6,  //(r,g,b)=(#b000b0) 4454432211122334
  0xf00f5e,0x031ce0,  //(r,g,b)=(#b800b8) 4545333211122244
  0xf80667,0x038c74,  //(r,g,b)=(#c000c0) 4554433121112344
  0xf40b78,0x011ee6,  //(r,g,b)=(#c800c8) 4555342211122334
  0xf806f7,0x038c74,  //(r,g,b)=(#d000d0) 5555433121112344
  0xf807f6,0x031ce1,  //(r,g,b)=(#d800d8) 5555433211122245
  0xf807f6,0x011ee7,  //(r,g,b)=(#e000e0) 5555433211122335
  0xf827d6,0x011ee7,  //(r,g,b)=(#e800e8) 5565433211122335
  0xf847be,0x030cf5,  //(r,g,b)=(#f000f0) 5655533211112345
  0xfc6390,0x038c75,  //(r,g,b)=(#f800f8) 5665442221112345
  0x00ad52,0x00b649,  //(r,g,b)=(#000000) 2121221221221221
  0x00db24,0x00bd42,  //(r,g,b)=(#000800) 2212212221222212
  0x00b748,0x00fb04,  //(r,g,b)=(#001000) 2122122222222122
  0x006f90,0x00ff00,  //(r,g,b)=(#001800) 1221222222222222
  0x00b749,0x00fd0a,  //(r,g,b)=(#002000) 2122122322223212
  0x00df20,0x00ff50,  //(r,g,b)=(#002800) 2212222223232222
  0x006f92,0x00fe65,  //(r,g,b)=(#003000) 1221223223322321
  0x00df22,0x00ffb0,  //(r,g,b)=(#003800) 2212223232332222
  0x00df22,0x00fef3,  //(r,g,b)=(#004000) 2212223233332231
  0x00df25,0x00fff4,  //(r,g,b)=(#004800) 2212232333332322
  0x007f82,0x807f78,  //(r,g,b)=(#005000) 1222223243333222
  0x00df25,0x807f7c,  //(r,g,b)=(#005800) 2212232343333322
  0x01be44,0x20dfdc,  //(r,g,b)=(#006000) 2122232433433322
  0x00bf47,0x609f9c,  //(r,g,b)=(#006800) 2122233334433322
  0x00df27,0xe01f1e,  //(r,g,b)=(#007000) 2212233344433332
  0x007f87,0xe81714,  //(r,g,b)=(#007800) 1222233344434322
  0x01de2a,0xf00f0e,  //(r,g,b)=(#008000) 2212323444443332
  0x02dd25,0xf00f4e,  //(r,g,b)=(#008800) 2212234345443332
  0x03dc28,0xf40b28,  //(r,g,b)=(#009000) 2212324444543422
  0x01ee1e,0xf807a6,  //(r,g,b)=(#009800) 2221333454544332
  0x03bc44,0xfc03a1,  //(r,g,b)=(#00a000) 2122234454544423
  0x037c84,0xf807d6,  //(r,g,b)=(#00a800) 1222234455454332
  0x03fc04,0xf807f6,  //(r,g,b)=(#00b000) 2222234455554332
  0x03fc05,0xfc03f1,  //(r,g,b)=(#00b800) 2222234555554423
  0x03bc4c,0xfc8372,  //(r,g,b)=(#00c000) 2122334465554432
  0x07f800,0xfc837a,  //(r,g,b)=(#00c800) 2222244465555432
  0x03dc2e,0xfca35a,  //(r,g,b)=(#00d000) 2212335465655432
  0x07d829,0xfc639a,  //(r,g,b)=(#00d800) 2212344556655432
  0x07d831,0xfce31b,  //(r,g,b)=(#00e000) 2213244566655433
  0x037c8f,0xfee118,  //(r,g,b)=(#00e800) 1222335566655442
  0x07d833,0xfcf30b,  //(r,g,b)=(#00f000) 2213245566665433
  0x07d82b,0xfcf32b,  //(r,g,b)=(#00f800) 2212345566765433
  0x00b54a,0x00b649,  //(r,g,b)=(#000000) 2122121221221221
  0x00dd22,0x00ee11,  //(r,g,b)=(#000808) 2212221222212221
  0x00df20,0x00f708,  //(r,g,b)=(#001010) 2212222222221222
  0x007f80,0x00fe01,  //(r,g,b)=(#001818) 1222222222222221
  0x00ff01,0x00ff00,  //(r,g,b)=(#002020) 2222222322222222
  0x007f84,0x00fea1,  //(r,g,b)=(#002828) 1222232232322221
  0x00ff05,0x00fea1,  //(r,g,b)=(#003030) 2222232332322221
  0x00ff0b,0x00ffa0,  //(r,g,b)=(#003838) 2222323332322222
  0x00ff07,0x00ffe0,  //(r,g,b)=(#004040) 2222233333322222
  0x00ff0f,0x00fee9,  //(r,g,b)=(#004848) 2222333333323221
  0x02fd15,0x40bea9,  //(r,g,b)=(#005050) 2223234334323221
  0x01fe1e,0x40bfa8,  //(r,g,b)=(#005858) 2223333434323222
  0x037cac,0x40beb9,  //(r,g,b)=(#006060) 1232334434333221
  0x05fa1a,0xc03f38,  //(r,g,b)=(#006868) 2223343444333222
  0x03fc1c,0xe01e15,  //(r,g,b)=(#007070) 2223334444432321
  0x0bf425,0xe01f14,  //(r,g,b)=(#007878) 2232434544432322
  0x0778b9,0xe01f18,  //(r,g,b)=(#008080) 1233344544433222
  0x0ff021,0xe01f58,  //(r,g,b)=(#008888) 2232444545433222
  0x07f83b,0xe01f5c,  //(r,g,b)=(#009090) 2233345545433322
  0x0ff013,0xe01fbc,  //(r,g,b)=(#009898) 2223445554533322
  0x0f70d7,0xe817b0,  //(r,g,b)=(#00a0a0) 1323455554534222
  0x0ff132,0xf00fcc,  //(r,g,b)=(#00a8a8) 2233445655443322
  0x0ff037,0xf88740,  //(r,g,b)=(#00b0b0) 2233455565444222
  0x0ff239,0xf08f6c,  //(r,g,b)=(#00b8b8) 2233546565543322
  0x0ff13e,0xf08f6c,  //(r,g,b)=(#00c0c0) 2233555665543322
  0x1fe324,0xf88764,  //(r,g,b)=(#00c8c8) 2234456665544322
  0x0ff33c,0xf8c724,  //(r,g,b)=(#00d0d0) 2233556666544322
  0x1fe720,0xf0cf3c,  //(r,g,b)=(#00d8d8) 2234466666553322
  0x1fe748,0xf0cfbc,  //(r,g,b)=(#00e0e0) 2324566676553322
  0x0ff779,0xf0cf7e,  //(r,g,b)=(#00e8e8) 2333566767553332
  0x1fe729,0xf8e784,  //(r,g,b)=(#00f0f0) 2234566776644322
  0x1fe76b,0xf8e792,  //(r,g,b)=(#00f8f8) 2334567776654232
  0x006d92,0x00b649,  //(r,g,b)=(#000000) 1221221221221221
  0x00db24,0x006d92,  //(r,g,b)=(#000008) 2212212212212212
  0x007b84,0x00ad52,  //(r,g,b)=(#000010) 1222212221212212
  0x007b84,0x00d52a,  //(r,g,b)=(#000018) 1222212222121212
  0x00bf40,0x006a95,  //(r,g,b)=(#000020) 2122222212212121
  0x00fe01,0x00d52a,  //(r,g,b)=(#000028) 2222222122121212
  0x00bf40,0x00d52a,  //(r,g,b)=(#000030) 2122222222121212
  0x007f84,0x006996,  //(r,g,b)=(#000038) 1222232212212112
  0x007f88,0x00d22d,  //(r,g,b)=(#000040) 1222322222121121
  0x00ff04,0x00a956,  //(r,g,b)=(#000048) 2222232221212112
  0x007f92,0x00a45b,  //(r,g,b)=(#000050) 1223223221211211
  0x00ff12,0x00c837,  //(r,g,b)=(#000058) 2223223222112111
  0x00ff14,0x00d12e,  //(r,g,b)=(#000060) 2223232222121112
  0x007faa,0x00c43b,  //(r,g,b)=(#000068) 1232323222111211
  0x00ff16,0x00c23d,  //(r,g,b)=(#000070) 2223233222111121
  0x00ff16,0x00c23d,  //(r,g,b)=(#000078) 2223233222111121
  0x00ff16,0x00e01f,  //(r,g,b)=(#000080) 2223233222211111
  0x00ff1d,0x00c13e,  //(r,g,b)=(#000088) 2223332322111112
  0x007fb7,0x00c03f,  //(r,g,b)=(#000090) 1233233322111111
  0x00ff2f,0x00c03f,  //(r,g,b)=(#000098) 2232333322111111
  0x04fb19,0x00c03f,  //(r,g,b)=(#0000a0) 2223342322111111
  0x00ff3f,0x00c13a,  //(r,g,b)=(#0000a8) 2233333322111012
  0x047bda,0x00a0d7,  //(r,g,b)=(#0000b0) 1323343231210111
  0x02fd3c,0x00a0d7,  //(r,g,b)=(#0000b8) 2233334231210111
  0x04fb3b,0x00c037,  //(r,g,b)=(#0000c0) 2233343322110111
  0x08f737,0x00c03d,  //(r,g,b)=(#0000c8) 2233433322111101
  0x14eb2b,0x00c03b,  //(r,g,b)=(#0000d0) 2234343322111011
  0x0af535,0x00c037,  //(r,g,b)=(#0000d8) 2233434322110111
  0x0af535,0x00c03b,  //(r,g,b)=(#0000e0) 2233434322111011
  0x0cf333,0x00c0ab,  //(r,g,b)=(#0000e8) 2233443332101011
  0x16e929,0x00c03b,  //(r,g,b)=(#0000f0) 2234344322111011
  0x0e71f1,0x00c035,  //(r,g,b)=(#0000f8) 1333444322110101
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

  /*channel 3 low priority*/
  DCH3CON=0x11;
  DCH3ECON=(_TIMER_2_VECTOR << 8)| 0x10;
  DCH3SSA=((uint32_t)ch3data)&0x1FFFFFFF;
  DCH3DSA=((uint32_t)(&SPI6BUF))&0x1FFFFFFF;
  DCH3SSIZ=BUFFER_SIZE;
  DCH3DSIZ=1;
  DCH3CSIZ=DMA_TRANSLATION_SIZE;
  DCH3INTCLR=0x00ff00ff;
  DCH3CONSET=0x80;

  /*channel 4*/
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

  {
    volatile int i;
    for(i=0;i<100000;i++);
  }
  while(!(PB3DIV & (0x1<<11)));
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

  int i=0;
  ch0data[i] = 0xAA;
  ch1data[i] = 0xAA;
  ch2data[i] = 0xAA;
  //0xAD,0xF8,0xAD,0xF8,0x70

  for(i=1;i<BUFFER_SIZE;i++){
    ch0data[i] = i/5000%3==0?0xFF:0;
    ch1data[i] = i/5000%3==1?0xFF:00;
    ch2data[i] = i/5000%3==2?0xFF:0;
  }

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
