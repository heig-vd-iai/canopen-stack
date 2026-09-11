/**
 * This file contains the declaration of the ObjectDictionnary class.
 * Generated on 2024-11-22
 *
 * NOTICE: Auto-generated from the device's yaml file, do not edit manually!
 */
#pragma once

#define OD_NODE_ID 1
#define OD_GRANULARITY 8
#define OD_PDO_MAPPING_MAX 8
#define OD_ENTRY_SIZE_MAX 1024
#define OD_OBJECTS_COUNT 142
#define OD_TPDO_COUNT 4
#define OD_RPDO_COUNT 4
#define OBJECT_INDEX_COUNT 0

#define X1A00_MAP_DISABLED 0
#define X1A00_SAM_MPDO 0xFE
#define X1A00_DAM_MPDO 0xFF

#define SAVE_INDEX 0x1010
#define SAVE_SIGNATURE 0x65766173
#define RESTORE_INDEX 0x1011
#define RESTORE_SIGNATURE 0x64616F6C

#define MAX_SUB_INDEX 255

#define MAX_WRITE_TRY 100

#define DOMAIN_MAX_SIZE 800

#define OD_LENGTH 323


/* These defines map an object to its dictionnary array index */
#define OD_OBJECT_1000_SUB0 0
#define OD_OBJECT_1001_SUB0 1
#define OD_OBJECT_1002_SUB0 2
#define OD_OBJECT_1003_SUB0 3
#define OD_OBJECT_1003_SUB1 4
#define OD_OBJECT_1003_SUB2 5
#define OD_OBJECT_1003_SUB3 6
#define OD_OBJECT_1003_SUB4 7
#define OD_OBJECT_1003_SUB5 8
#define OD_OBJECT_1003_SUB6 9
#define OD_OBJECT_1003_SUB7 10
#define OD_OBJECT_1003_SUB8 11
#define OD_OBJECT_1010_SUB0 12
#define OD_OBJECT_1010_SUB1 13
#define OD_OBJECT_1010_SUB2 14
#define OD_OBJECT_1010_SUB3 15
#define OD_OBJECT_1010_SUB4 16
#define OD_OBJECT_1010_SUB5 17
#define OD_OBJECT_1011_SUB0 18
#define OD_OBJECT_1011_SUB1 19
#define OD_OBJECT_1011_SUB2 20
#define OD_OBJECT_1011_SUB3 21
#define OD_OBJECT_1011_SUB4 22
#define OD_OBJECT_1011_SUB5 23
#define OD_OBJECT_1017_SUB0 24
#define OD_OBJECT_1018_SUB0 25
#define OD_OBJECT_1018_SUB1 26
#define OD_OBJECT_1018_SUB2 27
#define OD_OBJECT_1018_SUB3 28
#define OD_OBJECT_1018_SUB4 29
#define OD_OBJECT_1019_SUB0 30
#define OD_OBJECT_1029_SUB0 31
#define OD_OBJECT_1029_SUB1 32
#define OD_OBJECT_1029_SUB2 33
#define OD_OBJECT_1400_SUB0 34
#define OD_OBJECT_1400_SUB1 35
#define OD_OBJECT_1400_SUB2 36
#define OD_OBJECT_1401_SUB0 37
#define OD_OBJECT_1401_SUB1 38
#define OD_OBJECT_1401_SUB2 39
#define OD_OBJECT_1402_SUB0 40
#define OD_OBJECT_1402_SUB1 41
#define OD_OBJECT_1402_SUB2 42
#define OD_OBJECT_1403_SUB0 43
#define OD_OBJECT_1403_SUB1 44
#define OD_OBJECT_1403_SUB2 45
#define OD_OBJECT_1600_SUB0 46
#define OD_OBJECT_1600_SUB1 47
#define OD_OBJECT_1600_SUB2 48
#define OD_OBJECT_1600_SUB3 49
#define OD_OBJECT_1600_SUB4 50
#define OD_OBJECT_1600_SUB5 51
#define OD_OBJECT_1600_SUB6 52
#define OD_OBJECT_1600_SUB7 53
#define OD_OBJECT_1600_SUB8 54
#define OD_OBJECT_1601_SUB0 55
#define OD_OBJECT_1601_SUB1 56
#define OD_OBJECT_1601_SUB2 57
#define OD_OBJECT_1601_SUB3 58
#define OD_OBJECT_1601_SUB4 59
#define OD_OBJECT_1601_SUB5 60
#define OD_OBJECT_1601_SUB6 61
#define OD_OBJECT_1601_SUB7 62
#define OD_OBJECT_1601_SUB8 63
#define OD_OBJECT_1602_SUB0 64
#define OD_OBJECT_1602_SUB1 65
#define OD_OBJECT_1602_SUB2 66
#define OD_OBJECT_1602_SUB3 67
#define OD_OBJECT_1602_SUB4 68
#define OD_OBJECT_1602_SUB5 69
#define OD_OBJECT_1602_SUB6 70
#define OD_OBJECT_1602_SUB7 71
#define OD_OBJECT_1602_SUB8 72
#define OD_OBJECT_1603_SUB0 73
#define OD_OBJECT_1603_SUB1 74
#define OD_OBJECT_1603_SUB2 75
#define OD_OBJECT_1603_SUB3 76
#define OD_OBJECT_1603_SUB4 77
#define OD_OBJECT_1603_SUB5 78
#define OD_OBJECT_1603_SUB6 79
#define OD_OBJECT_1603_SUB7 80
#define OD_OBJECT_1603_SUB8 81
#define OD_OBJECT_1800_SUB0 82
#define OD_OBJECT_1800_SUB1 83
#define OD_OBJECT_1800_SUB2 84
#define OD_OBJECT_1800_SUB3 85
#define OD_OBJECT_1800_SUB4 86
#define OD_OBJECT_1800_SUB5 87
#define OD_OBJECT_1800_SUB6 88
#define OD_OBJECT_1801_SUB0 89
#define OD_OBJECT_1801_SUB1 90
#define OD_OBJECT_1801_SUB2 91
#define OD_OBJECT_1801_SUB3 92
#define OD_OBJECT_1801_SUB4 93
#define OD_OBJECT_1801_SUB5 94
#define OD_OBJECT_1801_SUB6 95
#define OD_OBJECT_1802_SUB0 96
#define OD_OBJECT_1802_SUB1 97
#define OD_OBJECT_1802_SUB2 98
#define OD_OBJECT_1802_SUB3 99
#define OD_OBJECT_1802_SUB4 100
#define OD_OBJECT_1802_SUB5 101
#define OD_OBJECT_1802_SUB6 102
#define OD_OBJECT_1803_SUB0 103
#define OD_OBJECT_1803_SUB1 104
#define OD_OBJECT_1803_SUB2 105
#define OD_OBJECT_1803_SUB3 106
#define OD_OBJECT_1803_SUB4 107
#define OD_OBJECT_1803_SUB5 108
#define OD_OBJECT_1803_SUB6 109
#define OD_OBJECT_1a00_SUB0 110
#define OD_OBJECT_1a00_SUB1 111
#define OD_OBJECT_1a00_SUB2 112
#define OD_OBJECT_1a00_SUB3 113
#define OD_OBJECT_1a00_SUB4 114
#define OD_OBJECT_1a00_SUB5 115
#define OD_OBJECT_1a00_SUB6 116
#define OD_OBJECT_1a00_SUB7 117
#define OD_OBJECT_1a00_SUB8 118
#define OD_OBJECT_1a01_SUB0 119
#define OD_OBJECT_1a01_SUB1 120
#define OD_OBJECT_1a01_SUB2 121
#define OD_OBJECT_1a01_SUB3 122
#define OD_OBJECT_1a01_SUB4 123
#define OD_OBJECT_1a01_SUB5 124
#define OD_OBJECT_1a01_SUB6 125
#define OD_OBJECT_1a01_SUB7 126
#define OD_OBJECT_1a01_SUB8 127
#define OD_OBJECT_1a02_SUB0 128
#define OD_OBJECT_1a02_SUB1 129
#define OD_OBJECT_1a02_SUB2 130
#define OD_OBJECT_1a02_SUB3 131
#define OD_OBJECT_1a02_SUB4 132
#define OD_OBJECT_1a02_SUB5 133
#define OD_OBJECT_1a02_SUB6 134
#define OD_OBJECT_1a02_SUB7 135
#define OD_OBJECT_1a02_SUB8 136
#define OD_OBJECT_1a03_SUB0 137
#define OD_OBJECT_1a03_SUB1 138
#define OD_OBJECT_1a03_SUB2 139
#define OD_OBJECT_1a03_SUB3 140
#define OD_OBJECT_1a03_SUB4 141
#define OD_OBJECT_1a03_SUB5 142
#define OD_OBJECT_1a03_SUB6 143
#define OD_OBJECT_1a03_SUB7 144
#define OD_OBJECT_1a03_SUB8 145
#define OD_OBJECT_2100_SUB0 146
#define OD_OBJECT_2100_SUB1 147
#define OD_OBJECT_2100_SUB2 148
#define OD_OBJECT_2100_SUB3 149
#define OD_OBJECT_2101_SUB0 150
#define OD_OBJECT_2102_SUB0 151
#define OD_OBJECT_2102_SUB1 152
#define OD_OBJECT_2102_SUB2 153
#define OD_OBJECT_2103_SUB0 154
#define OD_OBJECT_2104_SUB0 155
#define OD_OBJECT_2105_SUB0 156
#define OD_OBJECT_2200_SUB0 157
#define OD_OBJECT_2200_SUB1 158
#define OD_OBJECT_2200_SUB2 159
#define OD_OBJECT_2200_SUB3 160
#define OD_OBJECT_2201_SUB0 161
#define OD_OBJECT_2202_SUB0 162
#define OD_OBJECT_2203_SUB0 163
#define OD_OBJECT_2204_SUB0 164
#define OD_OBJECT_2205_SUB0 165
#define OD_OBJECT_2300_SUB0 166
#define OD_OBJECT_2300_SUB1 167
#define OD_OBJECT_2301_SUB0 168
#define OD_OBJECT_2302_SUB0 169
#define OD_OBJECT_2303_SUB0 170
#define OD_OBJECT_2304_SUB0 171
#define OD_OBJECT_2310_SUB0 172
#define OD_OBJECT_2310_SUB1 173
#define OD_OBJECT_2310_SUB2 174
#define OD_OBJECT_2310_SUB3 175
#define OD_OBJECT_2310_SUB4 176
#define OD_OBJECT_2320_SUB0 177
#define OD_OBJECT_2350_SUB0 178
#define OD_OBJECT_2400_SUB0 179
#define OD_OBJECT_2400_SUB1 180
#define OD_OBJECT_2400_SUB2 181
#define OD_OBJECT_2401_SUB0 182
#define OD_OBJECT_2402_SUB0 183
#define OD_OBJECT_2403_SUB0 184
#define OD_OBJECT_2404_SUB0 185
#define OD_OBJECT_2500_SUB0 186
#define OD_OBJECT_2501_SUB0 187
#define OD_OBJECT_2600_SUB0 188
#define OD_OBJECT_2601_SUB0 189
#define OD_OBJECT_2602_SUB0 190
#define OD_OBJECT_2603_SUB0 191
#define OD_OBJECT_2604_SUB0 192
#define OD_OBJECT_2605_SUB0 193
#define OD_OBJECT_2605_SUB1 194
#define OD_OBJECT_2605_SUB2 195
#define OD_OBJECT_2606_SUB0 196
#define OD_OBJECT_2610_SUB0 197
#define OD_OBJECT_2620_SUB0 198
#define OD_OBJECT_2700_SUB0 199
#define OD_OBJECT_2700_SUB1 200
#define OD_OBJECT_2700_SUB2 201
#define OD_OBJECT_2700_SUB3 202
#define OD_OBJECT_2701_SUB0 203
#define OD_OBJECT_2701_SUB1 204
#define OD_OBJECT_2701_SUB2 205
#define OD_OBJECT_2701_SUB3 206
#define OD_OBJECT_2701_SUB4 207
#define OD_OBJECT_2701_SUB5 208
#define OD_OBJECT_2701_SUB6 209
#define OD_OBJECT_2702_SUB0 210
#define OD_OBJECT_2703_SUB0 211
#define OD_OBJECT_2800_SUB0 212
#define OD_OBJECT_2801_SUB0 213
#define OD_OBJECT_2808_SUB0 214
#define OD_OBJECT_2808_SUB1 215
#define OD_OBJECT_2808_SUB2 216
#define OD_OBJECT_2808_SUB3 217
#define OD_OBJECT_2809_SUB0 218
#define OD_OBJECT_2809_SUB1 219
#define OD_OBJECT_2809_SUB2 220
#define OD_OBJECT_2809_SUB3 221
#define OD_OBJECT_2810_SUB0 222
#define OD_OBJECT_2810_SUB1 223
#define OD_OBJECT_2810_SUB2 224
#define OD_OBJECT_2810_SUB3 225
#define OD_OBJECT_2811_SUB0 226
#define OD_OBJECT_2811_SUB1 227
#define OD_OBJECT_2811_SUB2 228
#define OD_OBJECT_2811_SUB3 229
#define OD_OBJECT_2812_SUB0 230
#define OD_OBJECT_2850_SUB0 231
#define OD_OBJECT_2850_SUB1 232
#define OD_OBJECT_2850_SUB2 233
#define OD_OBJECT_2851_SUB0 234
#define OD_OBJECT_2852_SUB0 235
#define OD_OBJECT_2853_SUB0 236
#define OD_OBJECT_2853_SUB1 237
#define OD_OBJECT_2853_SUB2 238
#define OD_OBJECT_2854_SUB0 239
#define OD_OBJECT_2855_SUB0 240
#define OD_OBJECT_2855_SUB1 241
#define OD_OBJECT_2855_SUB2 242
#define OD_OBJECT_2855_SUB3 243
#define OD_OBJECT_2855_SUB4 244
#define OD_OBJECT_2856_SUB0 245
#define OD_OBJECT_3500_SUB0 246
#define OD_OBJECT_5000_SUB0 247
#define OD_OBJECT_5001_SUB0 248
#define OD_OBJECT_5010_SUB0 249
#define OD_OBJECT_603f_SUB0 250
#define OD_OBJECT_6040_SUB0 251
#define OD_OBJECT_6041_SUB0 252
#define OD_OBJECT_605a_SUB0 253
#define OD_OBJECT_605d_SUB0 254
#define OD_OBJECT_6060_SUB0 255
#define OD_OBJECT_6061_SUB0 256
#define OD_OBJECT_6062_SUB0 257
#define OD_OBJECT_6064_SUB0 258
#define OD_OBJECT_6065_SUB0 259
#define OD_OBJECT_6066_SUB0 260
#define OD_OBJECT_6067_SUB0 261
#define OD_OBJECT_6068_SUB0 262
#define OD_OBJECT_606b_SUB0 263
#define OD_OBJECT_606c_SUB0 264
#define OD_OBJECT_606d_SUB0 265
#define OD_OBJECT_606e_SUB0 266
#define OD_OBJECT_606f_SUB0 267
#define OD_OBJECT_6070_SUB0 268
#define OD_OBJECT_6071_SUB0 269
#define OD_OBJECT_6072_SUB0 270
#define OD_OBJECT_6073_SUB0 271
#define OD_OBJECT_6074_SUB0 272
#define OD_OBJECT_6075_SUB0 273
#define OD_OBJECT_6076_SUB0 274
#define OD_OBJECT_6077_SUB0 275
#define OD_OBJECT_6078_SUB0 276
#define OD_OBJECT_6079_SUB0 277
#define OD_OBJECT_607a_SUB0 278
#define OD_OBJECT_607c_SUB0 279
#define OD_OBJECT_607d_SUB0 280
#define OD_OBJECT_607d_SUB1 281
#define OD_OBJECT_607d_SUB2 282
#define OD_OBJECT_607e_SUB0 283
#define OD_OBJECT_607f_SUB0 284
#define OD_OBJECT_6080_SUB0 285
#define OD_OBJECT_6081_SUB0 286
#define OD_OBJECT_6083_SUB0 287
#define OD_OBJECT_6084_SUB0 288
#define OD_OBJECT_6085_SUB0 289
#define OD_OBJECT_6087_SUB0 290
#define OD_OBJECT_608f_SUB0 291
#define OD_OBJECT_608f_SUB1 292
#define OD_OBJECT_608f_SUB2 293
#define OD_OBJECT_6090_SUB0 294
#define OD_OBJECT_6090_SUB1 295
#define OD_OBJECT_6090_SUB2 296
#define OD_OBJECT_6091_SUB0 297
#define OD_OBJECT_6091_SUB1 298
#define OD_OBJECT_6091_SUB2 299
#define OD_OBJECT_6092_SUB0 300
#define OD_OBJECT_6092_SUB1 301
#define OD_OBJECT_6092_SUB2 302
#define OD_OBJECT_6098_SUB0 303
#define OD_OBJECT_6099_SUB0 304
#define OD_OBJECT_6099_SUB1 305
#define OD_OBJECT_6099_SUB2 306
#define OD_OBJECT_609a_SUB0 307
#define OD_OBJECT_60a8_SUB0 308
#define OD_OBJECT_60a9_SUB0 309
#define OD_OBJECT_60aa_SUB0 310
#define OD_OBJECT_60ab_SUB0 311
#define OD_OBJECT_60c5_SUB0 312
#define OD_OBJECT_60c6_SUB0 313
#define OD_OBJECT_60e3_SUB0 314
#define OD_OBJECT_60e3_SUB1 315
#define OD_OBJECT_60e3_SUB2 316
#define OD_OBJECT_60e3_SUB3 317
#define OD_OBJECT_60f2_SUB0 318
#define OD_OBJECT_60f8_SUB0 319
#define OD_OBJECT_60ff_SUB0 320
#define OD_OBJECT_6502_SUB0 321
#define OD_OBJECT_67fe_SUB0 322

#ifndef _OD_DEFINES_ONLY
#include "IObjectDictionnary.hpp"

namespace CANopenOD{
// INDEX
// This table is used to find an object by its index.
constexpr std::pair<uint16_t, uint8_t> objectIndexTable[323] = {
    std::make_pair(0x1000, 0),
    std::make_pair(0x1001, 0),
    std::make_pair(0x1002, 0),
    std::make_pair(0x1003, 0),
    std::make_pair(0x1003, 1),
    std::make_pair(0x1003, 2),
    std::make_pair(0x1003, 3),
    std::make_pair(0x1003, 4),
    std::make_pair(0x1003, 5),
    std::make_pair(0x1003, 6),
    std::make_pair(0x1003, 7),
    std::make_pair(0x1003, 8),
    std::make_pair(0x1010, 0),
    std::make_pair(0x1010, 1),
    std::make_pair(0x1010, 2),
    std::make_pair(0x1010, 3),
    std::make_pair(0x1010, 4),
    std::make_pair(0x1010, 5),
    std::make_pair(0x1011, 0),
    std::make_pair(0x1011, 1),
    std::make_pair(0x1011, 2),
    std::make_pair(0x1011, 3),
    std::make_pair(0x1011, 4),
    std::make_pair(0x1011, 5),
    std::make_pair(0x1017, 0),
    std::make_pair(0x1018, 0),
    std::make_pair(0x1018, 1),
    std::make_pair(0x1018, 2),
    std::make_pair(0x1018, 3),
    std::make_pair(0x1018, 4),
    std::make_pair(0x1019, 0),
    std::make_pair(0x1029, 0),
    std::make_pair(0x1029, 1),
    std::make_pair(0x1029, 2),
    std::make_pair(0x1400, 0),
    std::make_pair(0x1400, 1),
    std::make_pair(0x1400, 2),
    std::make_pair(0x1401, 0),
    std::make_pair(0x1401, 1),
    std::make_pair(0x1401, 2),
    std::make_pair(0x1402, 0),
    std::make_pair(0x1402, 1),
    std::make_pair(0x1402, 2),
    std::make_pair(0x1403, 0),
    std::make_pair(0x1403, 1),
    std::make_pair(0x1403, 2),
    std::make_pair(0x1600, 0),
    std::make_pair(0x1600, 1),
    std::make_pair(0x1600, 2),
    std::make_pair(0x1600, 3),
    std::make_pair(0x1600, 4),
    std::make_pair(0x1600, 5),
    std::make_pair(0x1600, 6),
    std::make_pair(0x1600, 7),
    std::make_pair(0x1600, 8),
    std::make_pair(0x1601, 0),
    std::make_pair(0x1601, 1),
    std::make_pair(0x1601, 2),
    std::make_pair(0x1601, 3),
    std::make_pair(0x1601, 4),
    std::make_pair(0x1601, 5),
    std::make_pair(0x1601, 6),
    std::make_pair(0x1601, 7),
    std::make_pair(0x1601, 8),
    std::make_pair(0x1602, 0),
    std::make_pair(0x1602, 1),
    std::make_pair(0x1602, 2),
    std::make_pair(0x1602, 3),
    std::make_pair(0x1602, 4),
    std::make_pair(0x1602, 5),
    std::make_pair(0x1602, 6),
    std::make_pair(0x1602, 7),
    std::make_pair(0x1602, 8),
    std::make_pair(0x1603, 0),
    std::make_pair(0x1603, 1),
    std::make_pair(0x1603, 2),
    std::make_pair(0x1603, 3),
    std::make_pair(0x1603, 4),
    std::make_pair(0x1603, 5),
    std::make_pair(0x1603, 6),
    std::make_pair(0x1603, 7),
    std::make_pair(0x1603, 8),
    std::make_pair(0x1800, 0),
    std::make_pair(0x1800, 1),
    std::make_pair(0x1800, 2),
    std::make_pair(0x1800, 3),
    std::make_pair(0x1800, 4),
    std::make_pair(0x1800, 5),
    std::make_pair(0x1800, 6),
    std::make_pair(0x1801, 0),
    std::make_pair(0x1801, 1),
    std::make_pair(0x1801, 2),
    std::make_pair(0x1801, 3),
    std::make_pair(0x1801, 4),
    std::make_pair(0x1801, 5),
    std::make_pair(0x1801, 6),
    std::make_pair(0x1802, 0),
    std::make_pair(0x1802, 1),
    std::make_pair(0x1802, 2),
    std::make_pair(0x1802, 3),
    std::make_pair(0x1802, 4),
    std::make_pair(0x1802, 5),
    std::make_pair(0x1802, 6),
    std::make_pair(0x1803, 0),
    std::make_pair(0x1803, 1),
    std::make_pair(0x1803, 2),
    std::make_pair(0x1803, 3),
    std::make_pair(0x1803, 4),
    std::make_pair(0x1803, 5),
    std::make_pair(0x1803, 6),
    std::make_pair(0x1a00, 0),
    std::make_pair(0x1a00, 1),
    std::make_pair(0x1a00, 2),
    std::make_pair(0x1a00, 3),
    std::make_pair(0x1a00, 4),
    std::make_pair(0x1a00, 5),
    std::make_pair(0x1a00, 6),
    std::make_pair(0x1a00, 7),
    std::make_pair(0x1a00, 8),
    std::make_pair(0x1a01, 0),
    std::make_pair(0x1a01, 1),
    std::make_pair(0x1a01, 2),
    std::make_pair(0x1a01, 3),
    std::make_pair(0x1a01, 4),
    std::make_pair(0x1a01, 5),
    std::make_pair(0x1a01, 6),
    std::make_pair(0x1a01, 7),
    std::make_pair(0x1a01, 8),
    std::make_pair(0x1a02, 0),
    std::make_pair(0x1a02, 1),
    std::make_pair(0x1a02, 2),
    std::make_pair(0x1a02, 3),
    std::make_pair(0x1a02, 4),
    std::make_pair(0x1a02, 5),
    std::make_pair(0x1a02, 6),
    std::make_pair(0x1a02, 7),
    std::make_pair(0x1a02, 8),
    std::make_pair(0x1a03, 0),
    std::make_pair(0x1a03, 1),
    std::make_pair(0x1a03, 2),
    std::make_pair(0x1a03, 3),
    std::make_pair(0x1a03, 4),
    std::make_pair(0x1a03, 5),
    std::make_pair(0x1a03, 6),
    std::make_pair(0x1a03, 7),
    std::make_pair(0x1a03, 8),
    std::make_pair(0x2100, 0),
    std::make_pair(0x2100, 1),
    std::make_pair(0x2100, 2),
    std::make_pair(0x2100, 3),
    std::make_pair(0x2101, 0),
    std::make_pair(0x2102, 0),
    std::make_pair(0x2102, 1),
    std::make_pair(0x2102, 2),
    std::make_pair(0x2103, 0),
    std::make_pair(0x2104, 0),
    std::make_pair(0x2105, 0),
    std::make_pair(0x2200, 0),
    std::make_pair(0x2200, 1),
    std::make_pair(0x2200, 2),
    std::make_pair(0x2200, 3),
    std::make_pair(0x2201, 0),
    std::make_pair(0x2202, 0),
    std::make_pair(0x2203, 0),
    std::make_pair(0x2204, 0),
    std::make_pair(0x2205, 0),
    std::make_pair(0x2300, 0),
    std::make_pair(0x2300, 1),
    std::make_pair(0x2301, 0),
    std::make_pair(0x2302, 0),
    std::make_pair(0x2303, 0),
    std::make_pair(0x2304, 0),
    std::make_pair(0x2310, 0),
    std::make_pair(0x2310, 1),
    std::make_pair(0x2310, 2),
    std::make_pair(0x2310, 3),
    std::make_pair(0x2310, 4),
    std::make_pair(0x2320, 0),
    std::make_pair(0x2350, 0),
    std::make_pair(0x2400, 0),
    std::make_pair(0x2400, 1),
    std::make_pair(0x2400, 2),
    std::make_pair(0x2401, 0),
    std::make_pair(0x2402, 0),
    std::make_pair(0x2403, 0),
    std::make_pair(0x2404, 0),
    std::make_pair(0x2500, 0),
    std::make_pair(0x2501, 0),
    std::make_pair(0x2600, 0),
    std::make_pair(0x2601, 0),
    std::make_pair(0x2602, 0),
    std::make_pair(0x2603, 0),
    std::make_pair(0x2604, 0),
    std::make_pair(0x2605, 0),
    std::make_pair(0x2605, 1),
    std::make_pair(0x2605, 2),
    std::make_pair(0x2606, 0),
    std::make_pair(0x2610, 0),
    std::make_pair(0x2620, 0),
    std::make_pair(0x2700, 0),
    std::make_pair(0x2700, 1),
    std::make_pair(0x2700, 2),
    std::make_pair(0x2700, 3),
    std::make_pair(0x2701, 0),
    std::make_pair(0x2701, 1),
    std::make_pair(0x2701, 2),
    std::make_pair(0x2701, 3),
    std::make_pair(0x2701, 4),
    std::make_pair(0x2701, 5),
    std::make_pair(0x2701, 6),
    std::make_pair(0x2702, 0),
    std::make_pair(0x2703, 0),
    std::make_pair(0x2800, 0),
    std::make_pair(0x2801, 0),
    std::make_pair(0x2808, 0),
    std::make_pair(0x2808, 1),
    std::make_pair(0x2808, 2),
    std::make_pair(0x2808, 3),
    std::make_pair(0x2809, 0),
    std::make_pair(0x2809, 1),
    std::make_pair(0x2809, 2),
    std::make_pair(0x2809, 3),
    std::make_pair(0x2810, 0),
    std::make_pair(0x2810, 1),
    std::make_pair(0x2810, 2),
    std::make_pair(0x2810, 3),
    std::make_pair(0x2811, 0),
    std::make_pair(0x2811, 1),
    std::make_pair(0x2811, 2),
    std::make_pair(0x2811, 3),
    std::make_pair(0x2812, 0),
    std::make_pair(0x2850, 0),
    std::make_pair(0x2850, 1),
    std::make_pair(0x2850, 2),
    std::make_pair(0x2851, 0),
    std::make_pair(0x2852, 0),
    std::make_pair(0x2853, 0),
    std::make_pair(0x2853, 1),
    std::make_pair(0x2853, 2),
    std::make_pair(0x2854, 0),
    std::make_pair(0x2855, 0),
    std::make_pair(0x2855, 1),
    std::make_pair(0x2855, 2),
    std::make_pair(0x2855, 3),
    std::make_pair(0x2855, 4),
    std::make_pair(0x2856, 0),
    std::make_pair(0x3500, 0),
    std::make_pair(0x5000, 0),
    std::make_pair(0x5001, 0),
    std::make_pair(0x5010, 0),
    std::make_pair(0x603f, 0),
    std::make_pair(0x6040, 0),
    std::make_pair(0x6041, 0),
    std::make_pair(0x605a, 0),
    std::make_pair(0x605d, 0),
    std::make_pair(0x6060, 0),
    std::make_pair(0x6061, 0),
    std::make_pair(0x6062, 0),
    std::make_pair(0x6064, 0),
    std::make_pair(0x6065, 0),
    std::make_pair(0x6066, 0),
    std::make_pair(0x6067, 0),
    std::make_pair(0x6068, 0),
    std::make_pair(0x606b, 0),
    std::make_pair(0x606c, 0),
    std::make_pair(0x606d, 0),
    std::make_pair(0x606e, 0),
    std::make_pair(0x606f, 0),
    std::make_pair(0x6070, 0),
    std::make_pair(0x6071, 0),
    std::make_pair(0x6072, 0),
    std::make_pair(0x6073, 0),
    std::make_pair(0x6074, 0),
    std::make_pair(0x6075, 0),
    std::make_pair(0x6076, 0),
    std::make_pair(0x6077, 0),
    std::make_pair(0x6078, 0),
    std::make_pair(0x6079, 0),
    std::make_pair(0x607a, 0),
    std::make_pair(0x607c, 0),
    std::make_pair(0x607d, 0),
    std::make_pair(0x607d, 1),
    std::make_pair(0x607d, 2),
    std::make_pair(0x607e, 0),
    std::make_pair(0x607f, 0),
    std::make_pair(0x6080, 0),
    std::make_pair(0x6081, 0),
    std::make_pair(0x6083, 0),
    std::make_pair(0x6084, 0),
    std::make_pair(0x6085, 0),
    std::make_pair(0x6087, 0),
    std::make_pair(0x608f, 0),
    std::make_pair(0x608f, 1),
    std::make_pair(0x608f, 2),
    std::make_pair(0x6090, 0),
    std::make_pair(0x6090, 1),
    std::make_pair(0x6090, 2),
    std::make_pair(0x6091, 0),
    std::make_pair(0x6091, 1),
    std::make_pair(0x6091, 2),
    std::make_pair(0x6092, 0),
    std::make_pair(0x6092, 1),
    std::make_pair(0x6092, 2),
    std::make_pair(0x6098, 0),
    std::make_pair(0x6099, 0),
    std::make_pair(0x6099, 1),
    std::make_pair(0x6099, 2),
    std::make_pair(0x609a, 0),
    std::make_pair(0x60a8, 0),
    std::make_pair(0x60a9, 0),
    std::make_pair(0x60aa, 0),
    std::make_pair(0x60ab, 0),
    std::make_pair(0x60c5, 0),
    std::make_pair(0x60c6, 0),
    std::make_pair(0x60e3, 0),
    std::make_pair(0x60e3, 1),
    std::make_pair(0x60e3, 2),
    std::make_pair(0x60e3, 3),
    std::make_pair(0x60f2, 0),
    std::make_pair(0x60f8, 0),
    std::make_pair(0x60ff, 0),
    std::make_pair(0x6502, 0),
    std::make_pair(0x67fe, 0),
};

}

using namespace CANopen;

const uint64_t od_signature = 202411220814u;

int8_t getLocalData_bool(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_bool(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_int8_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_int8_t(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_int16_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_int16_t(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_int32_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_int32_t(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_int64_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_int64_t(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_uint8_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_uint8_t(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_uint16_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_uint16_t(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_uint32_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_uint32_t(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_uint64_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_uint64_t(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_float(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_float(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_double(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_double(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_char(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_char(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getLocalData_domain(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_domain(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getRemoteData(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setRemoteData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1001sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1001sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1003sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1003sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1010sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1010sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1010sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1010sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1010sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1010sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1010sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1010sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1010sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1010sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1010sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1010sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1011sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1011sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1011sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1011sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1011sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1011sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1011sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1011sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1011sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1011sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1011sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1011sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1017sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1017sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1019sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1019sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1029sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1029sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1029sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1029sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1029sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1029sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1400sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1400sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1400sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1400sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1400sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1400sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1401sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1401sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1401sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1401sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1401sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1401sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1402sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1402sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1402sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1402sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1402sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1402sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1403sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1403sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1403sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1403sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1403sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1403sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1600sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1600sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1601sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1601sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1602sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1602sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1603sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1603sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1800sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1800sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1800sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1800sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1800sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1800sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1800sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1800sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1800sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1800sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1800sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1800sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1800sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1800sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1801sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1801sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1801sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1801sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1801sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1801sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1801sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1801sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1801sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1801sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1801sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1801sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1801sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1801sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1802sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1802sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1802sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1802sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1802sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1802sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1802sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1802sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1802sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1802sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1802sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1802sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1802sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1802sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1803sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1803sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1803sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1803sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1803sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1803sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1803sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1803sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1803sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1803sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1803sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1803sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1803sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1803sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a00sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a00sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a01sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a01sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a02sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a02sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub0(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub1(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub2(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub3(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub4(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub5(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub6(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub7(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t getobject1a03sub8(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setobject1a03sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode);


class ObjectDictionnary : IObjectDictionnary{

private:
    //METADATA
    Metadata_uint32_t object1000sub0Metadata{0b00011, 0x00000192};
    Metadata_uint8_t object1001sub0Metadata{0b00111, 0};
    Metadata_uint32_t object1002sub0Metadata{0b00111, 0};
    Metadata_uint8_t object1003sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1003sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1003sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1003sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1003sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1003sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1003sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1003sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1003sub8Metadata{0b00011, 0};
    Metadata_uint8_t object1010sub0Metadata{0b00011, 5};
    Metadata_uint32_t object1010sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1010sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1010sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1010sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1010sub5Metadata{0b00011, 0};
    Metadata_uint8_t object1011sub0Metadata{0b00011, 5};
    Metadata_uint32_t object1011sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1011sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1011sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1011sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1011sub5Metadata{0b00011, 0};
    Metadata_uint16_t object1017sub0Metadata{0b00011, 0};
    Metadata_uint8_t object1018sub0Metadata{0b00011, 4};
    Metadata_uint32_t object1018sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1018sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1018sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1018sub4Metadata{0b00011, 0};
    Metadata_uint8_t object1019sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1029sub0Metadata{0b00011, 2};
    Metadata_uint8_t object1029sub1Metadata{0b00011, 1};
    Metadata_uint8_t object1029sub2Metadata{0b00011, 1};
    Metadata_uint8_t object1400sub0Metadata{0b00011, 2};
    Metadata_uint32_t object1400sub1Metadata{0b00011, 2147484161};
    Metadata_uint8_t object1400sub2Metadata{0b00011, 0};
    Metadata_uint8_t object1401sub0Metadata{0b00011, 2};
    Metadata_uint32_t object1401sub1Metadata{0b00011, 2147484417};
    Metadata_uint8_t object1401sub2Metadata{0b00011, 0};
    Metadata_uint8_t object1402sub0Metadata{0b00011, 2};
    Metadata_uint32_t object1402sub1Metadata{0b00011, 2147484673};
    Metadata_uint8_t object1402sub2Metadata{0b00011, 0};
    Metadata_uint8_t object1403sub0Metadata{0b00011, 2};
    Metadata_uint32_t object1403sub1Metadata{0b00011, 2147484929};
    Metadata_uint8_t object1403sub2Metadata{0b00011, 0};
    Metadata_uint8_t object1600sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1600sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1600sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1600sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1600sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1600sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1600sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1600sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1600sub8Metadata{0b00011, 0};
    Metadata_uint8_t object1601sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1601sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1601sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1601sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1601sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1601sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1601sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1601sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1601sub8Metadata{0b00011, 0};
    Metadata_uint8_t object1602sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1602sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1602sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1602sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1602sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1602sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1602sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1602sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1602sub8Metadata{0b00011, 0};
    Metadata_uint8_t object1603sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1603sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1603sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1603sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1603sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1603sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1603sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1603sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1603sub8Metadata{0b00011, 0};
    Metadata_uint8_t object1800sub0Metadata{0b00011, 6};
    Metadata_uint32_t object1800sub1Metadata{0b00011, 2147484033};
    Metadata_uint8_t object1800sub2Metadata{0b00011, 0};
    Metadata_uint16_t object1800sub3Metadata{0b00011, 0};
    Metadata_uint8_t object1800sub4Metadata{0b00011, 0};
    Metadata_uint16_t object1800sub5Metadata{0b00011, 0};
    Metadata_uint8_t object1800sub6Metadata{0b00011, 0};
    Metadata_uint8_t object1801sub0Metadata{0b00011, 6};
    Metadata_uint32_t object1801sub1Metadata{0b00011, 2147484289};
    Metadata_uint8_t object1801sub2Metadata{0b00011, 0};
    Metadata_uint16_t object1801sub3Metadata{0b00011, 0};
    Metadata_uint8_t object1801sub4Metadata{0b00011, 0};
    Metadata_uint16_t object1801sub5Metadata{0b00011, 0};
    Metadata_uint8_t object1801sub6Metadata{0b00011, 0};
    Metadata_uint8_t object1802sub0Metadata{0b00011, 6};
    Metadata_uint32_t object1802sub1Metadata{0b00011, 2147484545};
    Metadata_uint8_t object1802sub2Metadata{0b00011, 0};
    Metadata_uint16_t object1802sub3Metadata{0b00011, 0};
    Metadata_uint8_t object1802sub4Metadata{0b00011, 0};
    Metadata_uint16_t object1802sub5Metadata{0b00011, 0};
    Metadata_uint8_t object1802sub6Metadata{0b00011, 0};
    Metadata_uint8_t object1803sub0Metadata{0b00011, 6};
    Metadata_uint32_t object1803sub1Metadata{0b00011, 2147484801};
    Metadata_uint8_t object1803sub2Metadata{0b00011, 0};
    Metadata_uint16_t object1803sub3Metadata{0b00011, 0};
    Metadata_uint8_t object1803sub4Metadata{0b00011, 0};
    Metadata_uint16_t object1803sub5Metadata{0b00011, 0};
    Metadata_uint8_t object1803sub6Metadata{0b00011, 0};
    Metadata_uint8_t object1a00sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1a00sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1a00sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1a00sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1a00sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1a00sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1a00sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1a00sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1a00sub8Metadata{0b00011, 0};
    Metadata_uint8_t object1a01sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1a01sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1a01sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1a01sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1a01sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1a01sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1a01sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1a01sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1a01sub8Metadata{0b00011, 0};
    Metadata_uint8_t object1a02sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1a02sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1a02sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1a02sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1a02sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1a02sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1a02sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1a02sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1a02sub8Metadata{0b00011, 0};
    Metadata_uint8_t object1a03sub0Metadata{0b00011, 0};
    Metadata_uint32_t object1a03sub1Metadata{0b00011, 0};
    Metadata_uint32_t object1a03sub2Metadata{0b00011, 0};
    Metadata_uint32_t object1a03sub3Metadata{0b00011, 0};
    Metadata_uint32_t object1a03sub4Metadata{0b00011, 0};
    Metadata_uint32_t object1a03sub5Metadata{0b00011, 0};
    Metadata_uint32_t object1a03sub6Metadata{0b00011, 0};
    Metadata_uint32_t object1a03sub7Metadata{0b00011, 0};
    Metadata_uint32_t object1a03sub8Metadata{0b00011, 0};
    Metadata_uint8_t object2100sub0Metadata{0b10011, 3};
    Metadata_float object2100sub1Metadata{0b10011, 0};
    Metadata_float object2100sub2Metadata{0b10011, 0};
    Metadata_float object2100sub3Metadata{0b10011, 0};
    Metadata_uint32_t object2101sub0Metadata{0b10011, 0};
    Metadata_uint8_t object2102sub0Metadata{0b10011, 2};
    Metadata_uint32_t object2102sub1Metadata{0b10011, 0};
    Metadata_uint32_t object2102sub2Metadata{0b10011, 0};
    Metadata_bool object2103sub0Metadata{0b10011, 0};
    Metadata_uint16_t object2104sub0Metadata{0b10011, 0};
    Metadata_int32_t object2105sub0Metadata{0b10111, 0};
    Metadata_uint8_t object2200sub0Metadata{0b10011, 3};
    Metadata_float object2200sub1Metadata{0b10011, 0};
    Metadata_float object2200sub2Metadata{0b10011, 0};
    Metadata_float object2200sub3Metadata{0b10011, 0};
    Metadata_uint16_t object2201sub0Metadata{0b10011, 0};
    Metadata_bool object2202sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2203sub0Metadata{0b10011, 0};
    Metadata_uint16_t object2204sub0Metadata{0b10011, 0};
    Metadata_int32_t object2205sub0Metadata{0b10111, 0};
    Metadata_uint8_t object2300sub0Metadata{0b10011, 1};
    Metadata_float object2300sub1Metadata{0b10011, 0};
    Metadata_bool object2301sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2302sub0Metadata{0b10011, 0};
    Metadata_uint16_t object2303sub0Metadata{0b10011, 0};
    Metadata_int32_t object2304sub0Metadata{0b10111, 0};
    Metadata_uint8_t object2310sub0Metadata{0b10011, 4};
    Metadata_float object2310sub1Metadata{0b10011, 0};
    Metadata_float object2310sub2Metadata{0b10011, 0};
    Metadata_float object2310sub3Metadata{0b10011, 0};
    Metadata_float object2310sub4Metadata{0b10011, 0};
    Metadata_uint16_t object2320sub0Metadata{0b10011, 0};
    Metadata_uint8_t object2350sub0Metadata{0b10011, 0};
    Metadata_uint8_t object2400sub0Metadata{0b10011, 2};
    Metadata_int32_t object2400sub1Metadata{0b10011, 0};
    Metadata_int32_t object2400sub2Metadata{0b10011, 0};
    Metadata_uint32_t object2401sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2402sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2403sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2404sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2500sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2501sub0Metadata{0b10011, 0};
    Metadata_uint16_t object2600sub0Metadata{0b10011, 0};
    Metadata_float object2601sub0Metadata{0b10011, 0};
    Metadata_float object2602sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2603sub0Metadata{0b10011, 0};
    Metadata_float object2604sub0Metadata{0b10011, 0};
    Metadata_uint8_t object2605sub0Metadata{0b10011, 2};
    Metadata_uint32_t object2605sub1Metadata{0b10011, 0};
    Metadata_uint32_t object2605sub2Metadata{0b10011, 0};
    Metadata_uint32_t object2606sub0Metadata{0b10011, 0};
    Metadata_uint16_t object2610sub0Metadata{0b10011, 0};
    Metadata_uint16_t object2620sub0Metadata{0b10011, 0};
    Metadata_uint8_t object2700sub0Metadata{0b10011, 3};
    Metadata_int8_t object2700sub1Metadata{0b10011, 0};
    Metadata_int8_t object2700sub2Metadata{0b10011, 0};
    Metadata_int8_t object2700sub3Metadata{0b10011, 0};
    Metadata_uint8_t object2701sub0Metadata{0b10011, 6};
    Metadata_uint8_t object2701sub1Metadata{0b10011, 0};
    Metadata_float object2701sub2Metadata{0b10011, 0};
    Metadata_float object2701sub3Metadata{0b10011, 0};
    Metadata_float object2701sub4Metadata{0b10011, 0};
    Metadata_float object2701sub5Metadata{0b10011, 0};
    Metadata_float object2701sub6Metadata{0b10011, 0};
    Metadata_uint32_t object2702sub0Metadata{0b10011, 0};
    Metadata_uint32_t object2703sub0Metadata{0b10011, 0};
    Metadata_float object2800sub0Metadata{0b10111, 0};
    Metadata_float object2801sub0Metadata{0b10111, 0};
    Metadata_uint8_t object2808sub0Metadata{0b10011, 3};
    Metadata_float object2808sub1Metadata{0b10011, 0};
    Metadata_float object2808sub2Metadata{0b10011, 0};
    Metadata_float object2808sub3Metadata{0b10011, 0};
    Metadata_uint8_t object2809sub0Metadata{0b10011, 3};
    Metadata_float object2809sub1Metadata{0b10011, 0};
    Metadata_float object2809sub2Metadata{0b10011, 0};
    Metadata_float object2809sub3Metadata{0b10011, 0};
    Metadata_uint8_t object2810sub0Metadata{0b10011, 3};
    Metadata_float object2810sub1Metadata{0b10011, 0};
    Metadata_float object2810sub2Metadata{0b10011, 0};
    Metadata_float object2810sub3Metadata{0b10011, 0};
    Metadata_uint8_t object2811sub0Metadata{0b10011, 3};
    Metadata_float object2811sub1Metadata{0b10011, 0};
    Metadata_float object2811sub2Metadata{0b10011, 0};
    Metadata_float object2811sub3Metadata{0b10011, 0};
    Metadata_float object2812sub0Metadata{0b10111, 0};
    Metadata_uint8_t object2850sub0Metadata{0b10011, 2};
    Metadata_uint8_t object2850sub1Metadata{0b10011, 0};
    Metadata_uint8_t object2850sub2Metadata{0b10011, 0};
    Metadata_uint8_t object2851sub0Metadata{0b10011, 0};
    Metadata_bool object2852sub0Metadata{0b10010, 0};
    Metadata_uint8_t object2853sub0Metadata{0b10011, 2};
    Metadata_float object2853sub1Metadata{0b10011, 0};
    Metadata_uint8_t object2853sub2Metadata{0b10011, 0};
    Metadata_uint32_t object2854sub0Metadata{0b10011, 0};
    Metadata_uint8_t object2855sub0Metadata{0b10011, 4};
    Metadata_int16_t object2855sub1Metadata{0b10011, 0};
    Metadata_int16_t object2855sub2Metadata{0b10011, 0};
    Metadata_int16_t object2855sub3Metadata{0b10011, 0};
    Metadata_int16_t object2855sub4Metadata{0b10011, 0};
    Metadata_uint16_t object2856sub0Metadata{0b10011, 0};
    Metadata_bool object3500sub0Metadata{0b10010, 0};
    Metadata_domain object5000sub0Metadata{0b10011};
    Metadata_domain object5001sub0Metadata{0b10011};
    Metadata_domain object5010sub0Metadata{0b10011};
    Metadata_uint16_t object603fsub0Metadata{0b10111, 0};
    Metadata_uint16_t object6040sub0Metadata{0b10111, 0};
    Metadata_uint16_t object6041sub0Metadata{0b10111, 0};
    Metadata_int16_t object605asub0Metadata{0b10011, 2};
    Metadata_int16_t object605dsub0Metadata{0b10011, 1};
    Metadata_int16_t object6060sub0Metadata{0b10111, 0};
    Metadata_int16_t object6061sub0Metadata{0b10111, 0};
    Metadata_int32_t object6062sub0Metadata{0b10111, 0};
    Metadata_int32_t object6064sub0Metadata{0b10111, 0};
    Metadata_uint32_t object6065sub0Metadata{0b10011, 0};
    Metadata_uint16_t object6066sub0Metadata{0b10011, 0};
    Metadata_uint32_t object6067sub0Metadata{0b10011, 0};
    Metadata_uint16_t object6068sub0Metadata{0b10011, 0};
    Metadata_int32_t object606bsub0Metadata{0b10111, 0};
    Metadata_int32_t object606csub0Metadata{0b10111, 0};
    Metadata_uint16_t object606dsub0Metadata{0b10011, 0};
    Metadata_uint16_t object606esub0Metadata{0b10011, 0};
    Metadata_uint16_t object606fsub0Metadata{0b10011, 0};
    Metadata_uint16_t object6070sub0Metadata{0b10011, 0};
    Metadata_int16_t object6071sub0Metadata{0b10111, 0};
    Metadata_uint16_t object6072sub0Metadata{0b10011, 0};
    Metadata_uint16_t object6073sub0Metadata{0b10011, 0};
    Metadata_int16_t object6074sub0Metadata{0b10111, 0};
    Metadata_uint32_t object6075sub0Metadata{0b10011, 0};
    Metadata_uint32_t object6076sub0Metadata{0b10011, 0};
    Metadata_int16_t object6077sub0Metadata{0b10111, 0};
    Metadata_int16_t object6078sub0Metadata{0b10111, 0};
    Metadata_uint32_t object6079sub0Metadata{0b10011, 0};
    Metadata_int32_t object607asub0Metadata{0b10111, 0};
    Metadata_int32_t object607csub0Metadata{0b10011, 0};
    Metadata_uint8_t object607dsub0Metadata{0b10011, 2};
    Metadata_int32_t object607dsub1Metadata{0b10011, 0};
    Metadata_int32_t object607dsub2Metadata{0b10011, 0};
    Metadata_uint16_t object607esub0Metadata{0b10011, 0};
    Metadata_uint32_t object607fsub0Metadata{0b10011, 0};
    Metadata_uint32_t object6080sub0Metadata{0b10011, 0};
    Metadata_uint32_t object6081sub0Metadata{0b10011, 0};
    Metadata_uint32_t object6083sub0Metadata{0b10011, 0};
    Metadata_uint32_t object6084sub0Metadata{0b10011, 0};
    Metadata_uint32_t object6085sub0Metadata{0b10011, 0};
    Metadata_uint32_t object6087sub0Metadata{0b10011, 0};
    Metadata_uint8_t object608fsub0Metadata{0b10011, 2};
    Metadata_uint32_t object608fsub1Metadata{0b10011, 1};
    Metadata_uint32_t object608fsub2Metadata{0b10011, 1};
    Metadata_uint8_t object6090sub0Metadata{0b10011, 2};
    Metadata_uint32_t object6090sub1Metadata{0b10011, 1};
    Metadata_uint32_t object6090sub2Metadata{0b10011, 1};
    Metadata_uint8_t object6091sub0Metadata{0b10011, 2};
    Metadata_uint32_t object6091sub1Metadata{0b10011, 1};
    Metadata_uint32_t object6091sub2Metadata{0b10011, 1};
    Metadata_uint8_t object6092sub0Metadata{0b10011, 2};
    Metadata_uint32_t object6092sub1Metadata{0b10011, 1};
    Metadata_uint32_t object6092sub2Metadata{0b10011, 1};
    Metadata_int16_t object6098sub0Metadata{0b10011, 0};
    Metadata_uint8_t object6099sub0Metadata{0b10011, 2};
    Metadata_uint32_t object6099sub1Metadata{0b10011, 0};
    Metadata_uint32_t object6099sub2Metadata{0b10011, 0};
    Metadata_uint32_t object609asub0Metadata{0b10011, 0};
    Metadata_uint32_t object60a8sub0Metadata{0b10011, 4244701184};
    Metadata_uint32_t object60a9sub0Metadata{0b10011, 4244701952};
    Metadata_uint32_t object60aasub0Metadata{0b10011, 4244723456};
    Metadata_uint32_t object60absub0Metadata{0b10011, 4244742144};
    Metadata_uint32_t object60c5sub0Metadata{0b10011, 0};
    Metadata_uint32_t object60c6sub0Metadata{0b10011, 0};
    Metadata_uint8_t object60e3sub0Metadata{0b00011, 2};
    Metadata_int16_t object60e3sub1Metadata{0b00011, 37};
    Metadata_int16_t object60e3sub2Metadata{0b00011, -1};
    Metadata_int16_t object60e3sub3Metadata{0b00011, -2};
    Metadata_uint16_t object60f2sub0Metadata{0b10011, 0};
    Metadata_int32_t object60f8sub0Metadata{0b10011, 0};
    Metadata_int32_t object60ffsub0Metadata{0b10011, 0};
    Metadata_uint32_t object6502sub0Metadata{0b10011, 0};
    Metadata_uint32_t object67fesub0Metadata{0b00011, 196864};
    //END METADATA

public:
    const uint32_t length = 323;
    // INDEX
    // This table is used to find an object by its index.

    const Metadata *objectMetadataTable[323] = {
    &object1000sub0Metadata,
    &object1001sub0Metadata,
    &object1002sub0Metadata,
    &object1003sub0Metadata,
    &object1003sub1Metadata,
    &object1003sub2Metadata,
    &object1003sub3Metadata,
    &object1003sub4Metadata,
    &object1003sub5Metadata,
    &object1003sub6Metadata,
    &object1003sub7Metadata,
    &object1003sub8Metadata,
    &object1010sub0Metadata,
    &object1010sub1Metadata,
    &object1010sub2Metadata,
    &object1010sub3Metadata,
    &object1010sub4Metadata,
    &object1010sub5Metadata,
    &object1011sub0Metadata,
    &object1011sub1Metadata,
    &object1011sub2Metadata,
    &object1011sub3Metadata,
    &object1011sub4Metadata,
    &object1011sub5Metadata,
    &object1017sub0Metadata,
    &object1018sub0Metadata,
    &object1018sub1Metadata,
    &object1018sub2Metadata,
    &object1018sub3Metadata,
    &object1018sub4Metadata,
    &object1019sub0Metadata,
    &object1029sub0Metadata,
    &object1029sub1Metadata,
    &object1029sub2Metadata,
    &object1400sub0Metadata,
    &object1400sub1Metadata,
    &object1400sub2Metadata,
    &object1401sub0Metadata,
    &object1401sub1Metadata,
    &object1401sub2Metadata,
    &object1402sub0Metadata,
    &object1402sub1Metadata,
    &object1402sub2Metadata,
    &object1403sub0Metadata,
    &object1403sub1Metadata,
    &object1403sub2Metadata,
    &object1600sub0Metadata,
    &object1600sub1Metadata,
    &object1600sub2Metadata,
    &object1600sub3Metadata,
    &object1600sub4Metadata,
    &object1600sub5Metadata,
    &object1600sub6Metadata,
    &object1600sub7Metadata,
    &object1600sub8Metadata,
    &object1601sub0Metadata,
    &object1601sub1Metadata,
    &object1601sub2Metadata,
    &object1601sub3Metadata,
    &object1601sub4Metadata,
    &object1601sub5Metadata,
    &object1601sub6Metadata,
    &object1601sub7Metadata,
    &object1601sub8Metadata,
    &object1602sub0Metadata,
    &object1602sub1Metadata,
    &object1602sub2Metadata,
    &object1602sub3Metadata,
    &object1602sub4Metadata,
    &object1602sub5Metadata,
    &object1602sub6Metadata,
    &object1602sub7Metadata,
    &object1602sub8Metadata,
    &object1603sub0Metadata,
    &object1603sub1Metadata,
    &object1603sub2Metadata,
    &object1603sub3Metadata,
    &object1603sub4Metadata,
    &object1603sub5Metadata,
    &object1603sub6Metadata,
    &object1603sub7Metadata,
    &object1603sub8Metadata,
    &object1800sub0Metadata,
    &object1800sub1Metadata,
    &object1800sub2Metadata,
    &object1800sub3Metadata,
    &object1800sub4Metadata,
    &object1800sub5Metadata,
    &object1800sub6Metadata,
    &object1801sub0Metadata,
    &object1801sub1Metadata,
    &object1801sub2Metadata,
    &object1801sub3Metadata,
    &object1801sub4Metadata,
    &object1801sub5Metadata,
    &object1801sub6Metadata,
    &object1802sub0Metadata,
    &object1802sub1Metadata,
    &object1802sub2Metadata,
    &object1802sub3Metadata,
    &object1802sub4Metadata,
    &object1802sub5Metadata,
    &object1802sub6Metadata,
    &object1803sub0Metadata,
    &object1803sub1Metadata,
    &object1803sub2Metadata,
    &object1803sub3Metadata,
    &object1803sub4Metadata,
    &object1803sub5Metadata,
    &object1803sub6Metadata,
    &object1a00sub0Metadata,
    &object1a00sub1Metadata,
    &object1a00sub2Metadata,
    &object1a00sub3Metadata,
    &object1a00sub4Metadata,
    &object1a00sub5Metadata,
    &object1a00sub6Metadata,
    &object1a00sub7Metadata,
    &object1a00sub8Metadata,
    &object1a01sub0Metadata,
    &object1a01sub1Metadata,
    &object1a01sub2Metadata,
    &object1a01sub3Metadata,
    &object1a01sub4Metadata,
    &object1a01sub5Metadata,
    &object1a01sub6Metadata,
    &object1a01sub7Metadata,
    &object1a01sub8Metadata,
    &object1a02sub0Metadata,
    &object1a02sub1Metadata,
    &object1a02sub2Metadata,
    &object1a02sub3Metadata,
    &object1a02sub4Metadata,
    &object1a02sub5Metadata,
    &object1a02sub6Metadata,
    &object1a02sub7Metadata,
    &object1a02sub8Metadata,
    &object1a03sub0Metadata,
    &object1a03sub1Metadata,
    &object1a03sub2Metadata,
    &object1a03sub3Metadata,
    &object1a03sub4Metadata,
    &object1a03sub5Metadata,
    &object1a03sub6Metadata,
    &object1a03sub7Metadata,
    &object1a03sub8Metadata,
    &object2100sub0Metadata,
    &object2100sub1Metadata,
    &object2100sub2Metadata,
    &object2100sub3Metadata,
    &object2101sub0Metadata,
    &object2102sub0Metadata,
    &object2102sub1Metadata,
    &object2102sub2Metadata,
    &object2103sub0Metadata,
    &object2104sub0Metadata,
    &object2105sub0Metadata,
    &object2200sub0Metadata,
    &object2200sub1Metadata,
    &object2200sub2Metadata,
    &object2200sub3Metadata,
    &object2201sub0Metadata,
    &object2202sub0Metadata,
    &object2203sub0Metadata,
    &object2204sub0Metadata,
    &object2205sub0Metadata,
    &object2300sub0Metadata,
    &object2300sub1Metadata,
    &object2301sub0Metadata,
    &object2302sub0Metadata,
    &object2303sub0Metadata,
    &object2304sub0Metadata,
    &object2310sub0Metadata,
    &object2310sub1Metadata,
    &object2310sub2Metadata,
    &object2310sub3Metadata,
    &object2310sub4Metadata,
    &object2320sub0Metadata,
    &object2350sub0Metadata,
    &object2400sub0Metadata,
    &object2400sub1Metadata,
    &object2400sub2Metadata,
    &object2401sub0Metadata,
    &object2402sub0Metadata,
    &object2403sub0Metadata,
    &object2404sub0Metadata,
    &object2500sub0Metadata,
    &object2501sub0Metadata,
    &object2600sub0Metadata,
    &object2601sub0Metadata,
    &object2602sub0Metadata,
    &object2603sub0Metadata,
    &object2604sub0Metadata,
    &object2605sub0Metadata,
    &object2605sub1Metadata,
    &object2605sub2Metadata,
    &object2606sub0Metadata,
    &object2610sub0Metadata,
    &object2620sub0Metadata,
    &object2700sub0Metadata,
    &object2700sub1Metadata,
    &object2700sub2Metadata,
    &object2700sub3Metadata,
    &object2701sub0Metadata,
    &object2701sub1Metadata,
    &object2701sub2Metadata,
    &object2701sub3Metadata,
    &object2701sub4Metadata,
    &object2701sub5Metadata,
    &object2701sub6Metadata,
    &object2702sub0Metadata,
    &object2703sub0Metadata,
    &object2800sub0Metadata,
    &object2801sub0Metadata,
    &object2808sub0Metadata,
    &object2808sub1Metadata,
    &object2808sub2Metadata,
    &object2808sub3Metadata,
    &object2809sub0Metadata,
    &object2809sub1Metadata,
    &object2809sub2Metadata,
    &object2809sub3Metadata,
    &object2810sub0Metadata,
    &object2810sub1Metadata,
    &object2810sub2Metadata,
    &object2810sub3Metadata,
    &object2811sub0Metadata,
    &object2811sub1Metadata,
    &object2811sub2Metadata,
    &object2811sub3Metadata,
    &object2812sub0Metadata,
    &object2850sub0Metadata,
    &object2850sub1Metadata,
    &object2850sub2Metadata,
    &object2851sub0Metadata,
    &object2852sub0Metadata,
    &object2853sub0Metadata,
    &object2853sub1Metadata,
    &object2853sub2Metadata,
    &object2854sub0Metadata,
    &object2855sub0Metadata,
    &object2855sub1Metadata,
    &object2855sub2Metadata,
    &object2855sub3Metadata,
    &object2855sub4Metadata,
    &object2856sub0Metadata,
    &object3500sub0Metadata,
    &object5000sub0Metadata,
    &object5001sub0Metadata,
    &object5010sub0Metadata,
    &object603fsub0Metadata,
    &object6040sub0Metadata,
    &object6041sub0Metadata,
    &object605asub0Metadata,
    &object605dsub0Metadata,
    &object6060sub0Metadata,
    &object6061sub0Metadata,
    &object6062sub0Metadata,
    &object6064sub0Metadata,
    &object6065sub0Metadata,
    &object6066sub0Metadata,
    &object6067sub0Metadata,
    &object6068sub0Metadata,
    &object606bsub0Metadata,
    &object606csub0Metadata,
    &object606dsub0Metadata,
    &object606esub0Metadata,
    &object606fsub0Metadata,
    &object6070sub0Metadata,
    &object6071sub0Metadata,
    &object6072sub0Metadata,
    &object6073sub0Metadata,
    &object6074sub0Metadata,
    &object6075sub0Metadata,
    &object6076sub0Metadata,
    &object6077sub0Metadata,
    &object6078sub0Metadata,
    &object6079sub0Metadata,
    &object607asub0Metadata,
    &object607csub0Metadata,
    &object607dsub0Metadata,
    &object607dsub1Metadata,
    &object607dsub2Metadata,
    &object607esub0Metadata,
    &object607fsub0Metadata,
    &object6080sub0Metadata,
    &object6081sub0Metadata,
    &object6083sub0Metadata,
    &object6084sub0Metadata,
    &object6085sub0Metadata,
    &object6087sub0Metadata,
    &object608fsub0Metadata,
    &object608fsub1Metadata,
    &object608fsub2Metadata,
    &object6090sub0Metadata,
    &object6090sub1Metadata,
    &object6090sub2Metadata,
    &object6091sub0Metadata,
    &object6091sub1Metadata,
    &object6091sub2Metadata,
    &object6092sub0Metadata,
    &object6092sub1Metadata,
    &object6092sub2Metadata,
    &object6098sub0Metadata,
    &object6099sub0Metadata,
    &object6099sub1Metadata,
    &object6099sub2Metadata,
    &object609asub0Metadata,
    &object60a8sub0Metadata,
    &object60a9sub0Metadata,
    &object60aasub0Metadata,
    &object60absub0Metadata,
    &object60c5sub0Metadata,
    &object60c6sub0Metadata,
    &object60e3sub0Metadata,
    &object60e3sub1Metadata,
    &object60e3sub2Metadata,
    &object60e3sub3Metadata,
    &object60f2sub0Metadata,
    &object60f8sub0Metadata,
    &object60ffsub0Metadata,
    &object6502sub0Metadata,
    &object67fesub0Metadata,
    };
    // END INDEX

    // GETTER/SETTER
    // This table contain all the objects getter
    int8_t (*objectGetterTable[323])(Data &data, int32_t id, SDOAbortCodes &abortCode) = {
        &getLocalData_uint32_t,
        &getobject1001sub0,
        &getLocalData_uint32_t,
        &getobject1003sub0,
        &getobject1003sub1,
        &getobject1003sub2,
        &getobject1003sub3,
        &getobject1003sub4,
        &getobject1003sub5,
        &getobject1003sub6,
        &getobject1003sub7,
        &getobject1003sub8,
        &getobject1010sub0,
        &getobject1010sub1,
        &getobject1010sub2,
        &getobject1010sub3,
        &getobject1010sub4,
        &getobject1010sub5,
        &getobject1011sub0,
        &getobject1011sub1,
        &getobject1011sub2,
        &getobject1011sub3,
        &getobject1011sub4,
        &getobject1011sub5,
        &getobject1017sub0,
        &getLocalData_uint8_t,
        &getLocalData_uint32_t,
        &getLocalData_uint32_t,
        &getLocalData_uint32_t,
        &getLocalData_uint32_t,
        &getobject1019sub0,
        &getobject1029sub0,
        &getobject1029sub1,
        &getobject1029sub2,
        &getobject1400sub0,
        &getobject1400sub1,
        &getobject1400sub2,
        &getobject1401sub0,
        &getobject1401sub1,
        &getobject1401sub2,
        &getobject1402sub0,
        &getobject1402sub1,
        &getobject1402sub2,
        &getobject1403sub0,
        &getobject1403sub1,
        &getobject1403sub2,
        &getobject1600sub0,
        &getobject1600sub1,
        &getobject1600sub2,
        &getobject1600sub3,
        &getobject1600sub4,
        &getobject1600sub5,
        &getobject1600sub6,
        &getobject1600sub7,
        &getobject1600sub8,
        &getobject1601sub0,
        &getobject1601sub1,
        &getobject1601sub2,
        &getobject1601sub3,
        &getobject1601sub4,
        &getobject1601sub5,
        &getobject1601sub6,
        &getobject1601sub7,
        &getobject1601sub8,
        &getobject1602sub0,
        &getobject1602sub1,
        &getobject1602sub2,
        &getobject1602sub3,
        &getobject1602sub4,
        &getobject1602sub5,
        &getobject1602sub6,
        &getobject1602sub7,
        &getobject1602sub8,
        &getobject1603sub0,
        &getobject1603sub1,
        &getobject1603sub2,
        &getobject1603sub3,
        &getobject1603sub4,
        &getobject1603sub5,
        &getobject1603sub6,
        &getobject1603sub7,
        &getobject1603sub8,
        &getobject1800sub0,
        &getobject1800sub1,
        &getobject1800sub2,
        &getobject1800sub3,
        &getobject1800sub4,
        &getobject1800sub5,
        &getobject1800sub6,
        &getobject1801sub0,
        &getobject1801sub1,
        &getobject1801sub2,
        &getobject1801sub3,
        &getobject1801sub4,
        &getobject1801sub5,
        &getobject1801sub6,
        &getobject1802sub0,
        &getobject1802sub1,
        &getobject1802sub2,
        &getobject1802sub3,
        &getobject1802sub4,
        &getobject1802sub5,
        &getobject1802sub6,
        &getobject1803sub0,
        &getobject1803sub1,
        &getobject1803sub2,
        &getobject1803sub3,
        &getobject1803sub4,
        &getobject1803sub5,
        &getobject1803sub6,
        &getobject1a00sub0,
        &getobject1a00sub1,
        &getobject1a00sub2,
        &getobject1a00sub3,
        &getobject1a00sub4,
        &getobject1a00sub5,
        &getobject1a00sub6,
        &getobject1a00sub7,
        &getobject1a00sub8,
        &getobject1a01sub0,
        &getobject1a01sub1,
        &getobject1a01sub2,
        &getobject1a01sub3,
        &getobject1a01sub4,
        &getobject1a01sub5,
        &getobject1a01sub6,
        &getobject1a01sub7,
        &getobject1a01sub8,
        &getobject1a02sub0,
        &getobject1a02sub1,
        &getobject1a02sub2,
        &getobject1a02sub3,
        &getobject1a02sub4,
        &getobject1a02sub5,
        &getobject1a02sub6,
        &getobject1a02sub7,
        &getobject1a02sub8,
        &getobject1a03sub0,
        &getobject1a03sub1,
        &getobject1a03sub2,
        &getobject1a03sub3,
        &getobject1a03sub4,
        &getobject1a03sub5,
        &getobject1a03sub6,
        &getobject1a03sub7,
        &getobject1a03sub8,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_bool,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_int16_t,
        &getLocalData_int16_t,
        &getRemoteData,
        &getLocalData_bool,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint8_t,
        &getLocalData_int16_t,
        &getLocalData_int16_t,
        &getLocalData_int16_t,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getRemoteData,
        &getLocalData_uint32_t,
    };

    // This table contain all the objects setter
    int8_t (*objectSetterTable[323])(const Data &data, int32_t id, SDOAbortCodes &abortCode) = { 
        &setLocalData_uint32_t,
        &setLocalData_uint8_t,
        &setLocalData_uint32_t,
        &setobject1003sub0,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint8_t,
        &setobject1010sub1,
        &setobject1010sub2,
        &setobject1010sub3,
        &setobject1010sub4,
        &setobject1010sub5,
        &setLocalData_uint8_t,
        &setobject1011sub1,
        &setobject1011sub2,
        &setobject1011sub3,
        &setobject1011sub4,
        &setobject1011sub5,
        &setobject1017sub0,
        &setLocalData_uint8_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
        &setobject1019sub0,
        &setLocalData_uint32_t,
        &setobject1029sub1,
        &setobject1029sub2,
        &setLocalData_uint8_t,
        &setobject1400sub1,
        &setobject1400sub2,
        &setLocalData_uint8_t,
        &setobject1401sub1,
        &setobject1401sub2,
        &setLocalData_uint8_t,
        &setobject1402sub1,
        &setobject1402sub2,
        &setLocalData_uint8_t,
        &setobject1403sub1,
        &setobject1403sub2,
        &setobject1600sub0,
        &setobject1600sub1,
        &setobject1600sub2,
        &setobject1600sub3,
        &setobject1600sub4,
        &setobject1600sub5,
        &setobject1600sub6,
        &setobject1600sub7,
        &setobject1600sub8,
        &setobject1601sub0,
        &setobject1601sub1,
        &setobject1601sub2,
        &setobject1601sub3,
        &setobject1601sub4,
        &setobject1601sub5,
        &setobject1601sub6,
        &setobject1601sub7,
        &setobject1601sub8,
        &setobject1602sub0,
        &setobject1602sub1,
        &setobject1602sub2,
        &setobject1602sub3,
        &setobject1602sub4,
        &setobject1602sub5,
        &setobject1602sub6,
        &setobject1602sub7,
        &setobject1602sub8,
        &setobject1603sub0,
        &setobject1603sub1,
        &setobject1603sub2,
        &setobject1603sub3,
        &setobject1603sub4,
        &setobject1603sub5,
        &setobject1603sub6,
        &setobject1603sub7,
        &setobject1603sub8,
        &setLocalData_uint8_t,
        &setobject1800sub1,
        &setobject1800sub2,
        &setobject1800sub3,
        &setobject1800sub4,
        &setobject1800sub5,
        &setobject1800sub6,
        &setLocalData_uint8_t,
        &setobject1801sub1,
        &setobject1801sub2,
        &setobject1801sub3,
        &setobject1801sub4,
        &setobject1801sub5,
        &setobject1801sub6,
        &setLocalData_uint8_t,
        &setobject1802sub1,
        &setobject1802sub2,
        &setobject1802sub3,
        &setobject1802sub4,
        &setobject1802sub5,
        &setobject1802sub6,
        &setLocalData_uint8_t,
        &setobject1803sub1,
        &setobject1803sub2,
        &setobject1803sub3,
        &setobject1803sub4,
        &setobject1803sub5,
        &setobject1803sub6,
        &setobject1a00sub0,
        &setobject1a00sub1,
        &setobject1a00sub2,
        &setobject1a00sub3,
        &setobject1a00sub4,
        &setobject1a00sub5,
        &setobject1a00sub6,
        &setobject1a00sub7,
        &setobject1a00sub8,
        &setobject1a01sub0,
        &setobject1a01sub1,
        &setobject1a01sub2,
        &setobject1a01sub3,
        &setobject1a01sub4,
        &setobject1a01sub5,
        &setobject1a01sub6,
        &setobject1a01sub7,
        &setobject1a01sub8,
        &setobject1a02sub0,
        &setobject1a02sub1,
        &setobject1a02sub2,
        &setobject1a02sub3,
        &setobject1a02sub4,
        &setobject1a02sub5,
        &setobject1a02sub6,
        &setobject1a02sub7,
        &setobject1a02sub8,
        &setobject1a03sub0,
        &setobject1a03sub1,
        &setobject1a03sub2,
        &setobject1a03sub3,
        &setobject1a03sub4,
        &setobject1a03sub5,
        &setobject1a03sub6,
        &setobject1a03sub7,
        &setobject1a03sub8,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_int32_t,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_int32_t,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_int32_t,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint32_t,
        &setRemoteData,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_uint8_t,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_uint8_t,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_uint8_t,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_uint8_t,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_float,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setLocalData_int16_t,
        &setLocalData_int16_t,
        &setLocalData_int16_t,
        &setLocalData_int16_t,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_domain,
        &setLocalData_domain,
        &setLocalData_domain,
        &setLocalData_uint16_t,
        &setRemoteData,
        &setLocalData_uint16_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_int16_t,
        &setLocalData_int32_t,
        &setLocalData_int32_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_int32_t,
        &setLocalData_int32_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_int16_t,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_int16_t,
        &setLocalData_int16_t,
        &setLocalData_uint32_t,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint8_t,
        &setLocalData_int16_t,
        &setLocalData_int16_t,
        &setLocalData_int16_t,
        &setRemoteData,
        &setRemoteData,
        &setRemoteData,
        &setLocalData_uint32_t,
        &setLocalData_uint32_t,
    };
    // END GETTER/SETTER

    //DATA
    // bool type data table
    bool boolTable[5] = {
    0,
    0,
    0,
    0,
    0,
    };

    // int8_t type data table
    int8_t i8Table[3] = {
    0,
    0,
    0,
    };

    // int16_t type data table
    int16_t i16Table[16] = {
    0,
    0,
    0,
    0,
    2,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    37,
    -1,
    -2,
    };

    // int32_t type data table
    int32_t i32Table[15] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    };

    // int64_t type data table
    int64_t i64Table[0] = {
    };

    // uint8_t type data table
    uint8_t u8Table[69] = {
    0,
    0,
    5,
    5,
    4,
    0,
    1,
    1,
    2,
    0,
    2,
    0,
    2,
    0,
    2,
    0,
    0,
    0,
    0,
    0,
    6,
    0,
    0,
    0,
    6,
    0,
    0,
    0,
    6,
    0,
    0,
    0,
    6,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    3,
    2,
    3,
    1,
    4,
    0,
    2,
    2,
    3,
    6,
    0,
    3,
    3,
    3,
    3,
    2,
    0,
    0,
    0,
    2,
    0,
    4,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    };

    // uint16_t type data table
    uint16_t u16Table[31] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    };

    // uint32_t type data table
    uint32_t u32Table[146] = {
    0x00000192,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    2147484161,
    2147484417,
    2147484673,
    2147484929,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2147484033,
    2147484289,
    2147484545,
    2147484801,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    4244701184,
    4244701952,
    4244723456,
    4244742144,
    0,
    0,
    0,
    196864,
    };

    // uint64_t type data table
    uint64_t u64Table[0] = {
    };

    // float type data table
    float f32Table[35] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    };

    // float64 type data table
    double f64Table[0] = {
    };

    // string type data variable

    // string type data table
    char *stringTable[0] = {
    };

    // index data table

    uint8_t dataIndexTable[323] = {
    0,
    0,
    1,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    2,
    10,
    11,
    12,
    13,
    14,
    3,
    15,
    16,
    17,
    18,
    19,
    0,
    4,
    20,
    21,
    22,
    23,
    5,
    24,
    6,
    7,
    8,
    25,
    9,
    10,
    26,
    11,
    12,
    27,
    13,
    14,
    28,
    15,
    16,
    29,
    30,
    31,
    32,
    33,
    34,
    35,
    36,
    17,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    18,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    19,
    53,
    54,
    55,
    56,
    57,
    58,
    59,
    60,
    20,
    61,
    21,
    1,
    22,
    2,
    23,
    24,
    62,
    25,
    3,
    26,
    4,
    27,
    28,
    63,
    29,
    5,
    30,
    6,
    31,
    32,
    64,
    33,
    7,
    34,
    8,
    35,
    36,
    65,
    66,
    67,
    68,
    69,
    70,
    71,
    72,
    37,
    73,
    74,
    75,
    76,
    77,
    78,
    79,
    80,
    38,
    81,
    82,
    83,
    84,
    85,
    86,
    87,
    88,
    39,
    89,
    90,
    91,
    92,
    93,
    94,
    95,
    96,
    40,
    0,
    1,
    2,
    97,
    41,
    98,
    99,
    0,
    9,
    0,
    42,
    3,
    4,
    5,
    10,
    1,
    100,
    11,
    1,
    43,
    6,
    2,
    101,
    12,
    2,
    44,
    7,
    8,
    9,
    10,
    13,
    45,
    46,
    3,
    4,
    102,
    103,
    104,
    105,
    106,
    107,
    14,
    11,
    12,
    108,
    13,
    47,
    109,
    110,
    111,
    15,
    16,
    48,
    0,
    1,
    2,
    49,
    50,
    14,
    15,
    16,
    17,
    18,
    112,
    113,
    19,
    20,
    51,
    21,
    22,
    23,
    52,
    24,
    25,
    26,
    53,
    27,
    28,
    29,
    54,
    30,
    31,
    32,
    33,
    55,
    56,
    57,
    58,
    3,
    59,
    34,
    60,
    114,
    61,
    0,
    1,
    2,
    3,
    17,
    4,
    18,
    19,
    20,
    4,
    5,
    6,
    7,
    5,
    6,
    115,
    21,
    116,
    22,
    7,
    8,
    23,
    24,
    25,
    26,
    8,
    27,
    28,
    9,
    117,
    118,
    10,
    11,
    119,
    9,
    10,
    62,
    11,
    12,
    29,
    120,
    121,
    122,
    123,
    124,
    125,
    126,
    63,
    127,
    128,
    64,
    129,
    130,
    65,
    131,
    132,
    66,
    133,
    134,
    12,
    67,
    135,
    136,
    137,
    138,
    139,
    140,
    141,
    142,
    143,
    68,
    13,
    14,
    15,
    30,
    13,
    14,
    144,
    145,
    };
    //END DATA

    /**
     * Find an object in the dictionary.
     * @param index The hex index of the object to find.
     * @return index in objectDataTable if found, -1 otherwise.
     */
    int32_t findObject(uint16_t index);
    int32_t findObject(uint16_t index, uint8_t subIndex);

    /**
     * Access an object in the dictionary by array index using the subscript operator.
     * Under the hood, this method uses the at() method.
     * @param index The array index of the object to access.
     * @return index in objectDataTable if found, -1 otherwise.
     */
    int32_t operator[](uint16_t index);

    /**
     * Save data to non-volatile storage for a specified parameter group.
     * This method is called by object 0x1010.
     * @param parameterGroup The parameter group number.
     * @return True if saving was successful, false otherwise.
     */
    bool saveData(uint8_t parameterGroup);

    /**
    * Setter called by the od to save data
    * @param data must contain "save" 
    * @param id object id for parameter group
    * @param abortCode to set if the object is not found.
    */
    int8_t saveData(const Data &data, int32_t id, SDOAbortCodes &abortCode);

    /**
     * Getter called by the od to send information about the save
     * @param data to fill with the save status
     * @param id object id for parameter group
     * @param abortCode to set if the object is not found. 
    */
    int8_t getSave(Data &data, int32_t id, SDOAbortCodes &abortCode);

    /**
     * Load data from non-volatile storage for a specified parameter group.
     * This method should be called on application startup to load the object dictionnary.
     * @param parameterGroup The parameter group number.
     * @return True if loading was successful, false otherwise.
     */
    bool loadData(uint8_t parameterGroup);

    /**
     * Restore data to default for a specified parameter group.
     * This method is called by object 0x1011.
     * @param parameterGroup The parameter group number.
     * @return True if restoring was successful, false otherwise.
     */
    bool restoreData(uint8_t parameterGroup);

    /**
    * Setter called by the od to restore data
    * @param data must contain "load"
    * @param id object id for parameter group
    * @param abortCode to set if the object is not found.
    */
    int8_t restoreData(const Data &data, int32_t id, SDOAbortCodes &abortCode);

    /**
     * Getter called by the od to send information about the restore
     * @param data to fill with the save status
     * @param id object id for parameter group
     * @param abortCode to set if the object is not found.
    */
    int8_t getRestore(Data &data, int32_t id, SDOAbortCodes &abortCode);

    /**
     * Test if an object exists in the dictionary.
     * @param index The hex index of the object to find.
     * @param subIndex The subindex of the object to find.
     * @return True if the object exists, false otherwise.
    */
    bool isSubValid(uint16_t index, uint8_t subIndex);

    /**
     * Get the value of an object in the dictionary.
     * @param data The data object to store the value in.
     * @param index The hex index of the object to find.
     * @param subIndex The subindex of the object to find.
     * @param abortCode The abort code to set if the object is not found.
     * @return 0 if the object was found, 1 if a request is pending, -1 otherwise.
    */
    int8_t readData(Data &data, uint16_t index, uint8_t subIndex,
                    SDOAbortCodes &abortCode);

    int8_t readData(Data &data, int32_t id, SDOAbortCodes &abortCode);

    int8_t readData(Data &data, uint16_t index, uint8_t subIndex);

    int8_t readData(Data &data, int32_t id);

    /**
     * Set the value of an object in the dictionary.
     * @param data The data object to store the value in.
     * @param index The hex index of the object to find.
     * @param subIndex The subindex of the object to find.
     * @param abortCode The abort code to set if the object is not found.
     * @return 0 if the object was found, 1 if a request is pending, -1
     * otherwise.
     */
    int8_t writeData(const Data &data, uint16_t index, uint8_t subIndex,
                     SDOAbortCodes &abortCode);

    int8_t writeData(const Data &data, int32_t id, SDOAbortCodes &abortCode);

    int8_t writeData(const Data &data, uint16_t index, uint8_t subIndex);

    int8_t writeData(const Data &data, int32_t id);

    /**
     * Get the metadata of an object in the dictionary.
     * @param index The index to the object
     * @param subIndex The subindex to the object
     * @return The metadata of the object.
     */
    Metadata* getMetadata(uint16_t index, uint8_t subindex);

    Metadata* getMetadata(int32_t id);

    Data* getData(uint16_t index, uint8_t subindex);

    Data* getData(int32_t id);

    void setData(const Data &data, uint16_t index, uint8_t subindex);

    void setData(const Data &data, int32_t id);

    uint16_t getSize(uint16_t index, uint8_t subindex);

    uint16_t getSize(int32_t id);
};
#endif // _OD_DEFINES_ONLY