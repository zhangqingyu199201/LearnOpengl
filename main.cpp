#include "publicHeader.h"

#include "BitReader.h"


/**
ID3V2
while (true) {
    AudioSequence()
}
ID3V1

AudioSequence {
        MP3FrameHeader()
        crc_check()
       FrameSide()
       main_data()
       ancillary_data()
}

 */

static const uint16_t mpa_huffcodes_1[4] = {
 0x0001, 0x0001, 0x0001, 0x0000,
};

static const uint8_t mpa_huffbits_1[4] = {
  1,  3,  2,  3,
};

static const uint16_t mpa_huffcodes_2[9] = {
 0x0001, 0x0002, 0x0001, 0x0003, 0x0001, 0x0001, 0x0003, 0x0002,
 0x0000,
};

static const uint8_t mpa_huffbits_2[9] = {
  1,  3,  6,  3,  3,  5,  5,  5,
  6,
};

static const uint16_t mpa_huffcodes_3[9] = {
 0x0003, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0003, 0x0002,
 0x0000,
};

static const uint8_t mpa_huffbits_3[9] = {
  2,  2,  6,  3,  2,  5,  5,  5,
  6,
};

static const uint16_t mpa_huffcodes_5[16] = {
 0x0001, 0x0002, 0x0006, 0x0005, 0x0003, 0x0001, 0x0004, 0x0004,
 0x0007, 0x0005, 0x0007, 0x0001, 0x0006, 0x0001, 0x0001, 0x0000,
};

static const uint8_t mpa_huffbits_5[16] = {
  1,  3,  6,  7,  3,  3,  6,  7,
  6,  6,  7,  8,  7,  6,  7,  8,
};

static const uint16_t mpa_huffcodes_6[16] = {
 0x0007, 0x0003, 0x0005, 0x0001, 0x0006, 0x0002, 0x0003, 0x0002,
 0x0005, 0x0004, 0x0004, 0x0001, 0x0003, 0x0003, 0x0002, 0x0000,
};

static const uint8_t mpa_huffbits_6[16] = {
  3,  3,  5,  7,  3,  2,  4,  5,
  4,  4,  5,  6,  6,  5,  6,  7,
};

static const uint16_t mpa_huffcodes_7[36] = {
 0x0001, 0x0002, 0x000a, 0x0013, 0x0010, 0x000a, 0x0003, 0x0003,
 0x0007, 0x000a, 0x0005, 0x0003, 0x000b, 0x0004, 0x000d, 0x0011,
 0x0008, 0x0004, 0x000c, 0x000b, 0x0012, 0x000f, 0x000b, 0x0002,
 0x0007, 0x0006, 0x0009, 0x000e, 0x0003, 0x0001, 0x0006, 0x0004,
 0x0005, 0x0003, 0x0002, 0x0000,
};

static const uint8_t mpa_huffbits_7[36] = {
  1,  3,  6,  8,  8,  9,  3,  4,
  6,  7,  7,  8,  6,  5,  7,  8,
  8,  9,  7,  7,  8,  9,  9,  9,
  7,  7,  8,  9,  9, 10,  8,  8,
  9, 10, 10, 10,
};

static const uint16_t mpa_huffcodes_8[36] = {
 0x0003, 0x0004, 0x0006, 0x0012, 0x000c, 0x0005, 0x0005, 0x0001,
 0x0002, 0x0010, 0x0009, 0x0003, 0x0007, 0x0003, 0x0005, 0x000e,
 0x0007, 0x0003, 0x0013, 0x0011, 0x000f, 0x000d, 0x000a, 0x0004,
 0x000d, 0x0005, 0x0008, 0x000b, 0x0005, 0x0001, 0x000c, 0x0004,
 0x0004, 0x0001, 0x0001, 0x0000,
};

static const uint8_t mpa_huffbits_8[36] = {
  2,  3,  6,  8,  8,  9,  3,  2,
  4,  8,  8,  8,  6,  4,  6,  8,
  8,  9,  8,  8,  8,  9,  9, 10,
  8,  7,  8,  9, 10, 10,  9,  8,
  9,  9, 11, 11,
};

static const uint16_t mpa_huffcodes_9[36] = {
 0x0007, 0x0005, 0x0009, 0x000e, 0x000f, 0x0007, 0x0006, 0x0004,
 0x0005, 0x0005, 0x0006, 0x0007, 0x0007, 0x0006, 0x0008, 0x0008,
 0x0008, 0x0005, 0x000f, 0x0006, 0x0009, 0x000a, 0x0005, 0x0001,
 0x000b, 0x0007, 0x0009, 0x0006, 0x0004, 0x0001, 0x000e, 0x0004,
 0x0006, 0x0002, 0x0006, 0x0000,
};

static const uint8_t mpa_huffbits_9[36] = {
  3,  3,  5,  6,  8,  9,  3,  3,
  4,  5,  6,  8,  4,  4,  5,  6,
  7,  8,  6,  5,  6,  7,  7,  8,
  7,  6,  7,  7,  8,  9,  8,  7,
  8,  8,  9,  9,
};

static const uint16_t mpa_huffcodes_10[64] = {
 0x0001, 0x0002, 0x000a, 0x0017, 0x0023, 0x001e, 0x000c, 0x0011,
 0x0003, 0x0003, 0x0008, 0x000c, 0x0012, 0x0015, 0x000c, 0x0007,
 0x000b, 0x0009, 0x000f, 0x0015, 0x0020, 0x0028, 0x0013, 0x0006,
 0x000e, 0x000d, 0x0016, 0x0022, 0x002e, 0x0017, 0x0012, 0x0007,
 0x0014, 0x0013, 0x0021, 0x002f, 0x001b, 0x0016, 0x0009, 0x0003,
 0x001f, 0x0016, 0x0029, 0x001a, 0x0015, 0x0014, 0x0005, 0x0003,
 0x000e, 0x000d, 0x000a, 0x000b, 0x0010, 0x0006, 0x0005, 0x0001,
 0x0009, 0x0008, 0x0007, 0x0008, 0x0004, 0x0004, 0x0002, 0x0000,
};

static const uint8_t mpa_huffbits_10[64] = {
  1,  3,  6,  8,  9,  9,  9, 10,
  3,  4,  6,  7,  8,  9,  8,  8,
  6,  6,  7,  8,  9, 10,  9,  9,
  7,  7,  8,  9, 10, 10,  9, 10,
  8,  8,  9, 10, 10, 10, 10, 10,
  9,  9, 10, 10, 11, 11, 10, 11,
  8,  8,  9, 10, 10, 10, 11, 11,
  9,  8,  9, 10, 10, 11, 11, 11,
};

static const uint16_t mpa_huffcodes_11[64] = {
 0x0003, 0x0004, 0x000a, 0x0018, 0x0022, 0x0021, 0x0015, 0x000f,
 0x0005, 0x0003, 0x0004, 0x000a, 0x0020, 0x0011, 0x000b, 0x000a,
 0x000b, 0x0007, 0x000d, 0x0012, 0x001e, 0x001f, 0x0014, 0x0005,
 0x0019, 0x000b, 0x0013, 0x003b, 0x001b, 0x0012, 0x000c, 0x0005,
 0x0023, 0x0021, 0x001f, 0x003a, 0x001e, 0x0010, 0x0007, 0x0005,
 0x001c, 0x001a, 0x0020, 0x0013, 0x0011, 0x000f, 0x0008, 0x000e,
 0x000e, 0x000c, 0x0009, 0x000d, 0x000e, 0x0009, 0x0004, 0x0001,
 0x000b, 0x0004, 0x0006, 0x0006, 0x0006, 0x0003, 0x0002, 0x0000,
};

static const uint8_t mpa_huffbits_11[64] = {
  2,  3,  5,  7,  8,  9,  8,  9,
  3,  3,  4,  6,  8,  8,  7,  8,
  5,  5,  6,  7,  8,  9,  8,  8,
  7,  6,  7,  9,  8, 10,  8,  9,
  8,  8,  8,  9,  9, 10,  9, 10,
  8,  8,  9, 10, 10, 11, 10, 11,
  8,  7,  7,  8,  9, 10, 10, 10,
  8,  7,  8,  9, 10, 10, 10, 10,
};

static const uint16_t mpa_huffcodes_12[64] = {
 0x0009, 0x0006, 0x0010, 0x0021, 0x0029, 0x0027, 0x0026, 0x001a,
 0x0007, 0x0005, 0x0006, 0x0009, 0x0017, 0x0010, 0x001a, 0x000b,
 0x0011, 0x0007, 0x000b, 0x000e, 0x0015, 0x001e, 0x000a, 0x0007,
 0x0011, 0x000a, 0x000f, 0x000c, 0x0012, 0x001c, 0x000e, 0x0005,
 0x0020, 0x000d, 0x0016, 0x0013, 0x0012, 0x0010, 0x0009, 0x0005,
 0x0028, 0x0011, 0x001f, 0x001d, 0x0011, 0x000d, 0x0004, 0x0002,
 0x001b, 0x000c, 0x000b, 0x000f, 0x000a, 0x0007, 0x0004, 0x0001,
 0x001b, 0x000c, 0x0008, 0x000c, 0x0006, 0x0003, 0x0001, 0x0000,
};

static const uint8_t mpa_huffbits_12[64] = {
  4,  3,  5,  7,  8,  9,  9,  9,
  3,  3,  4,  5,  7,  7,  8,  8,
  5,  4,  5,  6,  7,  8,  7,  8,
  6,  5,  6,  6,  7,  8,  8,  8,
  7,  6,  7,  7,  8,  8,  8,  9,
  8,  7,  8,  8,  8,  9,  8,  9,
  8,  7,  7,  8,  8,  9,  9, 10,
  9,  8,  8,  9,  9,  9,  9, 10,
};

static const uint16_t mpa_huffcodes_13[256] = {
 0x0001, 0x0005, 0x000e, 0x0015, 0x0022, 0x0033, 0x002e, 0x0047,
 0x002a, 0x0034, 0x0044, 0x0034, 0x0043, 0x002c, 0x002b, 0x0013,
 0x0003, 0x0004, 0x000c, 0x0013, 0x001f, 0x001a, 0x002c, 0x0021,
 0x001f, 0x0018, 0x0020, 0x0018, 0x001f, 0x0023, 0x0016, 0x000e,
 0x000f, 0x000d, 0x0017, 0x0024, 0x003b, 0x0031, 0x004d, 0x0041,
 0x001d, 0x0028, 0x001e, 0x0028, 0x001b, 0x0021, 0x002a, 0x0010,
 0x0016, 0x0014, 0x0025, 0x003d, 0x0038, 0x004f, 0x0049, 0x0040,
 0x002b, 0x004c, 0x0038, 0x0025, 0x001a, 0x001f, 0x0019, 0x000e,
 0x0023, 0x0010, 0x003c, 0x0039, 0x0061, 0x004b, 0x0072, 0x005b,
 0x0036, 0x0049, 0x0037, 0x0029, 0x0030, 0x0035, 0x0017, 0x0018,
 0x003a, 0x001b, 0x0032, 0x0060, 0x004c, 0x0046, 0x005d, 0x0054,
 0x004d, 0x003a, 0x004f, 0x001d, 0x004a, 0x0031, 0x0029, 0x0011,
 0x002f, 0x002d, 0x004e, 0x004a, 0x0073, 0x005e, 0x005a, 0x004f,
 0x0045, 0x0053, 0x0047, 0x0032, 0x003b, 0x0026, 0x0024, 0x000f,
 0x0048, 0x0022, 0x0038, 0x005f, 0x005c, 0x0055, 0x005b, 0x005a,
 0x0056, 0x0049, 0x004d, 0x0041, 0x0033, 0x002c, 0x002b, 0x002a,
 0x002b, 0x0014, 0x001e, 0x002c, 0x0037, 0x004e, 0x0048, 0x0057,
 0x004e, 0x003d, 0x002e, 0x0036, 0x0025, 0x001e, 0x0014, 0x0010,
 0x0035, 0x0019, 0x0029, 0x0025, 0x002c, 0x003b, 0x0036, 0x0051,
 0x0042, 0x004c, 0x0039, 0x0036, 0x0025, 0x0012, 0x0027, 0x000b,
 0x0023, 0x0021, 0x001f, 0x0039, 0x002a, 0x0052, 0x0048, 0x0050,
 0x002f, 0x003a, 0x0037, 0x0015, 0x0016, 0x001a, 0x0026, 0x0016,
 0x0035, 0x0019, 0x0017, 0x0026, 0x0046, 0x003c, 0x0033, 0x0024,
 0x0037, 0x001a, 0x0022, 0x0017, 0x001b, 0x000e, 0x0009, 0x0007,
 0x0022, 0x0020, 0x001c, 0x0027, 0x0031, 0x004b, 0x001e, 0x0034,
 0x0030, 0x0028, 0x0034, 0x001c, 0x0012, 0x0011, 0x0009, 0x0005,
 0x002d, 0x0015, 0x0022, 0x0040, 0x0038, 0x0032, 0x0031, 0x002d,
 0x001f, 0x0013, 0x000c, 0x000f, 0x000a, 0x0007, 0x0006, 0x0003,
 0x0030, 0x0017, 0x0014, 0x0027, 0x0024, 0x0023, 0x0035, 0x0015,
 0x0010, 0x0017, 0x000d, 0x000a, 0x0006, 0x0001, 0x0004, 0x0002,
 0x0010, 0x000f, 0x0011, 0x001b, 0x0019, 0x0014, 0x001d, 0x000b,
 0x0011, 0x000c, 0x0010, 0x0008, 0x0001, 0x0001, 0x0000, 0x0001,
};

static const uint8_t mpa_huffbits_13[256] = {
  1,  4,  6,  7,  8,  9,  9, 10,
  9, 10, 11, 11, 12, 12, 13, 13,
  3,  4,  6,  7,  8,  8,  9,  9,
  9,  9, 10, 10, 11, 12, 12, 12,
  6,  6,  7,  8,  9,  9, 10, 10,
  9, 10, 10, 11, 11, 12, 13, 13,
  7,  7,  8,  9,  9, 10, 10, 10,
 10, 11, 11, 11, 11, 12, 13, 13,
  8,  7,  9,  9, 10, 10, 11, 11,
 10, 11, 11, 12, 12, 13, 13, 14,
  9,  8,  9, 10, 10, 10, 11, 11,
 11, 11, 12, 11, 13, 13, 14, 14,
  9,  9, 10, 10, 11, 11, 11, 11,
 11, 12, 12, 12, 13, 13, 14, 14,
 10,  9, 10, 11, 11, 11, 12, 12,
 12, 12, 13, 13, 13, 14, 16, 16,
  9,  8,  9, 10, 10, 11, 11, 12,
 12, 12, 12, 13, 13, 14, 15, 15,
 10,  9, 10, 10, 11, 11, 11, 13,
 12, 13, 13, 14, 14, 14, 16, 15,
 10, 10, 10, 11, 11, 12, 12, 13,
 12, 13, 14, 13, 14, 15, 16, 17,
 11, 10, 10, 11, 12, 12, 12, 12,
 13, 13, 13, 14, 15, 15, 15, 16,
 11, 11, 11, 12, 12, 13, 12, 13,
 14, 14, 15, 15, 15, 16, 16, 16,
 12, 11, 12, 13, 13, 13, 14, 14,
 14, 14, 14, 15, 16, 15, 16, 16,
 13, 12, 12, 13, 13, 13, 15, 14,
 14, 17, 15, 15, 15, 17, 16, 16,
 12, 12, 13, 14, 14, 14, 15, 14,
 15, 15, 16, 16, 19, 18, 19, 16,
};

static const uint16_t mpa_huffcodes_15[256] = {
 0x0007, 0x000c, 0x0012, 0x0035, 0x002f, 0x004c, 0x007c, 0x006c,
 0x0059, 0x007b, 0x006c, 0x0077, 0x006b, 0x0051, 0x007a, 0x003f,
 0x000d, 0x0005, 0x0010, 0x001b, 0x002e, 0x0024, 0x003d, 0x0033,
 0x002a, 0x0046, 0x0034, 0x0053, 0x0041, 0x0029, 0x003b, 0x0024,
 0x0013, 0x0011, 0x000f, 0x0018, 0x0029, 0x0022, 0x003b, 0x0030,
 0x0028, 0x0040, 0x0032, 0x004e, 0x003e, 0x0050, 0x0038, 0x0021,
 0x001d, 0x001c, 0x0019, 0x002b, 0x0027, 0x003f, 0x0037, 0x005d,
 0x004c, 0x003b, 0x005d, 0x0048, 0x0036, 0x004b, 0x0032, 0x001d,
 0x0034, 0x0016, 0x002a, 0x0028, 0x0043, 0x0039, 0x005f, 0x004f,
 0x0048, 0x0039, 0x0059, 0x0045, 0x0031, 0x0042, 0x002e, 0x001b,
 0x004d, 0x0025, 0x0023, 0x0042, 0x003a, 0x0034, 0x005b, 0x004a,
 0x003e, 0x0030, 0x004f, 0x003f, 0x005a, 0x003e, 0x0028, 0x0026,
 0x007d, 0x0020, 0x003c, 0x0038, 0x0032, 0x005c, 0x004e, 0x0041,
 0x0037, 0x0057, 0x0047, 0x0033, 0x0049, 0x0033, 0x0046, 0x001e,
 0x006d, 0x0035, 0x0031, 0x005e, 0x0058, 0x004b, 0x0042, 0x007a,
 0x005b, 0x0049, 0x0038, 0x002a, 0x0040, 0x002c, 0x0015, 0x0019,
 0x005a, 0x002b, 0x0029, 0x004d, 0x0049, 0x003f, 0x0038, 0x005c,
 0x004d, 0x0042, 0x002f, 0x0043, 0x0030, 0x0035, 0x0024, 0x0014,
 0x0047, 0x0022, 0x0043, 0x003c, 0x003a, 0x0031, 0x0058, 0x004c,
 0x0043, 0x006a, 0x0047, 0x0036, 0x0026, 0x0027, 0x0017, 0x000f,
 0x006d, 0x0035, 0x0033, 0x002f, 0x005a, 0x0052, 0x003a, 0x0039,
 0x0030, 0x0048, 0x0039, 0x0029, 0x0017, 0x001b, 0x003e, 0x0009,
 0x0056, 0x002a, 0x0028, 0x0025, 0x0046, 0x0040, 0x0034, 0x002b,
 0x0046, 0x0037, 0x002a, 0x0019, 0x001d, 0x0012, 0x000b, 0x000b,
 0x0076, 0x0044, 0x001e, 0x0037, 0x0032, 0x002e, 0x004a, 0x0041,
 0x0031, 0x0027, 0x0018, 0x0010, 0x0016, 0x000d, 0x000e, 0x0007,
 0x005b, 0x002c, 0x0027, 0x0026, 0x0022, 0x003f, 0x0034, 0x002d,
 0x001f, 0x0034, 0x001c, 0x0013, 0x000e, 0x0008, 0x0009, 0x0003,
 0x007b, 0x003c, 0x003a, 0x0035, 0x002f, 0x002b, 0x0020, 0x0016,
 0x0025, 0x0018, 0x0011, 0x000c, 0x000f, 0x000a, 0x0002, 0x0001,
 0x0047, 0x0025, 0x0022, 0x001e, 0x001c, 0x0014, 0x0011, 0x001a,
 0x0015, 0x0010, 0x000a, 0x0006, 0x0008, 0x0006, 0x0002, 0x0000,
};

static const uint8_t mpa_huffbits_15[256] = {
  3,  4,  5,  7,  7,  8,  9,  9,
  9, 10, 10, 11, 11, 11, 12, 13,
  4,  3,  5,  6,  7,  7,  8,  8,
  8,  9,  9, 10, 10, 10, 11, 11,
  5,  5,  5,  6,  7,  7,  8,  8,
  8,  9,  9, 10, 10, 11, 11, 11,
  6,  6,  6,  7,  7,  8,  8,  9,
  9,  9, 10, 10, 10, 11, 11, 11,
  7,  6,  7,  7,  8,  8,  9,  9,
  9,  9, 10, 10, 10, 11, 11, 11,
  8,  7,  7,  8,  8,  8,  9,  9,
  9,  9, 10, 10, 11, 11, 11, 12,
  9,  7,  8,  8,  8,  9,  9,  9,
  9, 10, 10, 10, 11, 11, 12, 12,
  9,  8,  8,  9,  9,  9,  9, 10,
 10, 10, 10, 10, 11, 11, 11, 12,
  9,  8,  8,  9,  9,  9,  9, 10,
 10, 10, 10, 11, 11, 12, 12, 12,
  9,  8,  9,  9,  9,  9, 10, 10,
 10, 11, 11, 11, 11, 12, 12, 12,
 10,  9,  9,  9, 10, 10, 10, 10,
 10, 11, 11, 11, 11, 12, 13, 12,
 10,  9,  9,  9, 10, 10, 10, 10,
 11, 11, 11, 11, 12, 12, 12, 13,
 11, 10,  9, 10, 10, 10, 11, 11,
 11, 11, 11, 11, 12, 12, 13, 13,
 11, 10, 10, 10, 10, 11, 11, 11,
 11, 12, 12, 12, 12, 12, 13, 13,
 12, 11, 11, 11, 11, 11, 11, 11,
 12, 12, 12, 12, 13, 13, 12, 13,
 12, 11, 11, 11, 11, 11, 11, 12,
 12, 12, 12, 12, 13, 13, 13, 13,
};

static const uint16_t mpa_huffcodes_16[256] = {
 0x0001, 0x0005, 0x000e, 0x002c, 0x004a, 0x003f, 0x006e, 0x005d,
 0x00ac, 0x0095, 0x008a, 0x00f2, 0x00e1, 0x00c3, 0x0178, 0x0011,
 0x0003, 0x0004, 0x000c, 0x0014, 0x0023, 0x003e, 0x0035, 0x002f,
 0x0053, 0x004b, 0x0044, 0x0077, 0x00c9, 0x006b, 0x00cf, 0x0009,
 0x000f, 0x000d, 0x0017, 0x0026, 0x0043, 0x003a, 0x0067, 0x005a,
 0x00a1, 0x0048, 0x007f, 0x0075, 0x006e, 0x00d1, 0x00ce, 0x0010,
 0x002d, 0x0015, 0x0027, 0x0045, 0x0040, 0x0072, 0x0063, 0x0057,
 0x009e, 0x008c, 0x00fc, 0x00d4, 0x00c7, 0x0183, 0x016d, 0x001a,
 0x004b, 0x0024, 0x0044, 0x0041, 0x0073, 0x0065, 0x00b3, 0x00a4,
 0x009b, 0x0108, 0x00f6, 0x00e2, 0x018b, 0x017e, 0x016a, 0x0009,
 0x0042, 0x001e, 0x003b, 0x0038, 0x0066, 0x00b9, 0x00ad, 0x0109,
 0x008e, 0x00fd, 0x00e8, 0x0190, 0x0184, 0x017a, 0x01bd, 0x0010,
 0x006f, 0x0036, 0x0034, 0x0064, 0x00b8, 0x00b2, 0x00a0, 0x0085,
 0x0101, 0x00f4, 0x00e4, 0x00d9, 0x0181, 0x016e, 0x02cb, 0x000a,
 0x0062, 0x0030, 0x005b, 0x0058, 0x00a5, 0x009d, 0x0094, 0x0105,
 0x00f8, 0x0197, 0x018d, 0x0174, 0x017c, 0x0379, 0x0374, 0x0008,
 0x0055, 0x0054, 0x0051, 0x009f, 0x009c, 0x008f, 0x0104, 0x00f9,
 0x01ab, 0x0191, 0x0188, 0x017f, 0x02d7, 0x02c9, 0x02c4, 0x0007,
 0x009a, 0x004c, 0x0049, 0x008d, 0x0083, 0x0100, 0x00f5, 0x01aa,
 0x0196, 0x018a, 0x0180, 0x02df, 0x0167, 0x02c6, 0x0160, 0x000b,
 0x008b, 0x0081, 0x0043, 0x007d, 0x00f7, 0x00e9, 0x00e5, 0x00db,
 0x0189, 0x02e7, 0x02e1, 0x02d0, 0x0375, 0x0372, 0x01b7, 0x0004,
 0x00f3, 0x0078, 0x0076, 0x0073, 0x00e3, 0x00df, 0x018c, 0x02ea,
 0x02e6, 0x02e0, 0x02d1, 0x02c8, 0x02c2, 0x00df, 0x01b4, 0x0006,
 0x00ca, 0x00e0, 0x00de, 0x00da, 0x00d8, 0x0185, 0x0182, 0x017d,
 0x016c, 0x0378, 0x01bb, 0x02c3, 0x01b8, 0x01b5, 0x06c0, 0x0004,
 0x02eb, 0x00d3, 0x00d2, 0x00d0, 0x0172, 0x017b, 0x02de, 0x02d3,
 0x02ca, 0x06c7, 0x0373, 0x036d, 0x036c, 0x0d83, 0x0361, 0x0002,
 0x0179, 0x0171, 0x0066, 0x00bb, 0x02d6, 0x02d2, 0x0166, 0x02c7,
 0x02c5, 0x0362, 0x06c6, 0x0367, 0x0d82, 0x0366, 0x01b2, 0x0000,
 0x000c, 0x000a, 0x0007, 0x000b, 0x000a, 0x0011, 0x000b, 0x0009,
 0x000d, 0x000c, 0x000a, 0x0007, 0x0005, 0x0003, 0x0001, 0x0003,
};

static const uint8_t mpa_huffbits_16[256] = {
  1,  4,  6,  8,  9,  9, 10, 10,
 11, 11, 11, 12, 12, 12, 13,  9,
  3,  4,  6,  7,  8,  9,  9,  9,
 10, 10, 10, 11, 12, 11, 12,  8,
  6,  6,  7,  8,  9,  9, 10, 10,
 11, 10, 11, 11, 11, 12, 12,  9,
  8,  7,  8,  9,  9, 10, 10, 10,
 11, 11, 12, 12, 12, 13, 13, 10,
  9,  8,  9,  9, 10, 10, 11, 11,
 11, 12, 12, 12, 13, 13, 13,  9,
  9,  8,  9,  9, 10, 11, 11, 12,
 11, 12, 12, 13, 13, 13, 14, 10,
 10,  9,  9, 10, 11, 11, 11, 11,
 12, 12, 12, 12, 13, 13, 14, 10,
 10,  9, 10, 10, 11, 11, 11, 12,
 12, 13, 13, 13, 13, 15, 15, 10,
 10, 10, 10, 11, 11, 11, 12, 12,
 13, 13, 13, 13, 14, 14, 14, 10,
 11, 10, 10, 11, 11, 12, 12, 13,
 13, 13, 13, 14, 13, 14, 13, 11,
 11, 11, 10, 11, 12, 12, 12, 12,
 13, 14, 14, 14, 15, 15, 14, 10,
 12, 11, 11, 11, 12, 12, 13, 14,
 14, 14, 14, 14, 14, 13, 14, 11,
 12, 12, 12, 12, 12, 13, 13, 13,
 13, 15, 14, 14, 14, 14, 16, 11,
 14, 12, 12, 12, 13, 13, 14, 14,
 14, 16, 15, 15, 15, 17, 15, 11,
 13, 13, 11, 12, 14, 14, 13, 14,
 14, 15, 16, 15, 17, 15, 14, 11,
  9,  8,  8,  9,  9, 10, 10, 10,
 11, 11, 11, 11, 11, 11, 11,  8,
};

static const uint16_t mpa_huffcodes_24[256] = {
 0x000f, 0x000d, 0x002e, 0x0050, 0x0092, 0x0106, 0x00f8, 0x01b2,
 0x01aa, 0x029d, 0x028d, 0x0289, 0x026d, 0x0205, 0x0408, 0x0058,
 0x000e, 0x000c, 0x0015, 0x0026, 0x0047, 0x0082, 0x007a, 0x00d8,
 0x00d1, 0x00c6, 0x0147, 0x0159, 0x013f, 0x0129, 0x0117, 0x002a,
 0x002f, 0x0016, 0x0029, 0x004a, 0x0044, 0x0080, 0x0078, 0x00dd,
 0x00cf, 0x00c2, 0x00b6, 0x0154, 0x013b, 0x0127, 0x021d, 0x0012,
 0x0051, 0x0027, 0x004b, 0x0046, 0x0086, 0x007d, 0x0074, 0x00dc,
 0x00cc, 0x00be, 0x00b2, 0x0145, 0x0137, 0x0125, 0x010f, 0x0010,
 0x0093, 0x0048, 0x0045, 0x0087, 0x007f, 0x0076, 0x0070, 0x00d2,
 0x00c8, 0x00bc, 0x0160, 0x0143, 0x0132, 0x011d, 0x021c, 0x000e,
 0x0107, 0x0042, 0x0081, 0x007e, 0x0077, 0x0072, 0x00d6, 0x00ca,
 0x00c0, 0x00b4, 0x0155, 0x013d, 0x012d, 0x0119, 0x0106, 0x000c,
 0x00f9, 0x007b, 0x0079, 0x0075, 0x0071, 0x00d7, 0x00ce, 0x00c3,
 0x00b9, 0x015b, 0x014a, 0x0134, 0x0123, 0x0110, 0x0208, 0x000a,
 0x01b3, 0x0073, 0x006f, 0x006d, 0x00d3, 0x00cb, 0x00c4, 0x00bb,
 0x0161, 0x014c, 0x0139, 0x012a, 0x011b, 0x0213, 0x017d, 0x0011,
 0x01ab, 0x00d4, 0x00d0, 0x00cd, 0x00c9, 0x00c1, 0x00ba, 0x00b1,
 0x00a9, 0x0140, 0x012f, 0x011e, 0x010c, 0x0202, 0x0179, 0x0010,
 0x014f, 0x00c7, 0x00c5, 0x00bf, 0x00bd, 0x00b5, 0x00ae, 0x014d,
 0x0141, 0x0131, 0x0121, 0x0113, 0x0209, 0x017b, 0x0173, 0x000b,
 0x029c, 0x00b8, 0x00b7, 0x00b3, 0x00af, 0x0158, 0x014b, 0x013a,
 0x0130, 0x0122, 0x0115, 0x0212, 0x017f, 0x0175, 0x016e, 0x000a,
 0x028c, 0x015a, 0x00ab, 0x00a8, 0x00a4, 0x013e, 0x0135, 0x012b,
 0x011f, 0x0114, 0x0107, 0x0201, 0x0177, 0x0170, 0x016a, 0x0006,
 0x0288, 0x0142, 0x013c, 0x0138, 0x0133, 0x012e, 0x0124, 0x011c,
 0x010d, 0x0105, 0x0200, 0x0178, 0x0172, 0x016c, 0x0167, 0x0004,
 0x026c, 0x012c, 0x0128, 0x0126, 0x0120, 0x011a, 0x0111, 0x010a,
 0x0203, 0x017c, 0x0176, 0x0171, 0x016d, 0x0169, 0x0165, 0x0002,
 0x0409, 0x0118, 0x0116, 0x0112, 0x010b, 0x0108, 0x0103, 0x017e,
 0x017a, 0x0174, 0x016f, 0x016b, 0x0168, 0x0166, 0x0164, 0x0000,
 0x002b, 0x0014, 0x0013, 0x0011, 0x000f, 0x000d, 0x000b, 0x0009,
 0x0007, 0x0006, 0x0004, 0x0007, 0x0005, 0x0003, 0x0001, 0x0003,
};

static const uint8_t mpa_huffbits_24[256] = {
  4,  4,  6,  7,  8,  9,  9, 10,
 10, 11, 11, 11, 11, 11, 12,  9,
  4,  4,  5,  6,  7,  8,  8,  9,
  9,  9, 10, 10, 10, 10, 10,  8,
  6,  5,  6,  7,  7,  8,  8,  9,
  9,  9,  9, 10, 10, 10, 11,  7,
  7,  6,  7,  7,  8,  8,  8,  9,
  9,  9,  9, 10, 10, 10, 10,  7,
  8,  7,  7,  8,  8,  8,  8,  9,
  9,  9, 10, 10, 10, 10, 11,  7,
  9,  7,  8,  8,  8,  8,  9,  9,
  9,  9, 10, 10, 10, 10, 10,  7,
  9,  8,  8,  8,  8,  9,  9,  9,
  9, 10, 10, 10, 10, 10, 11,  7,
 10,  8,  8,  8,  9,  9,  9,  9,
 10, 10, 10, 10, 10, 11, 11,  8,
 10,  9,  9,  9,  9,  9,  9,  9,
  9, 10, 10, 10, 10, 11, 11,  8,
 10,  9,  9,  9,  9,  9,  9, 10,
 10, 10, 10, 10, 11, 11, 11,  8,
 11,  9,  9,  9,  9, 10, 10, 10,
 10, 10, 10, 11, 11, 11, 11,  8,
 11, 10,  9,  9,  9, 10, 10, 10,
 10, 10, 10, 11, 11, 11, 11,  8,
 11, 10, 10, 10, 10, 10, 10, 10,
 10, 10, 11, 11, 11, 11, 11,  8,
 11, 10, 10, 10, 10, 10, 10, 10,
 11, 11, 11, 11, 11, 11, 11,  8,
 12, 10, 10, 10, 10, 10, 10, 11,
 11, 11, 11, 11, 11, 11, 11,  8,
  8,  7,  7,  7,  7,  7,  7,  7,
  7,  7,  7,  8,  8,  8,  8,  4,
};











// https://en.wikipedia.org/wiki/ID3#ID3v1
// https://baike.baidu.com/item/ID3/1196982?fr=aladdin

// id3v1   ---  128bit end tag
struct ID3V1 {
    unsigned char Title[30]; /*标题*/
    unsigned char Artist[30]; /*作者*/
    unsigned char Album[30]; /*专集*/
    unsigned char Year[4]; /*出品年代*/
    unsigned char Comment[30]; /*备注   28个注释+2个额外标识 */
    unsigned char Genre; /*类型*/
};


// id3v2 
struct ID3V2 {
    unsigned char Header[3]; /*必须为"ID3"否则认为标签不存在*/
    unsigned char Ver; /*版本号;ID3V2.3就记录03,ID3V2.4就记录04*/
    unsigned char Revision; /*副版本号;此版本记录为00*/
    unsigned char Flag; /*存放标志的字节，这个版本只定义了三位，稍后详细解说*/
    unsigned char Size[4]; /*标签大小，包括标签帧和标签头。（不包括扩展标签头的10个字节）*/
};

#if 0
1.标志字节
标志字节一般为0，定义如下：
abc00000
a -- 表示是否使用不同步(一般不设置)
b -- 表示是否有扩展头部，一般没有(至少Winamp没有记录)，所以一般也不设置
c -- 表示是否为测试标签(99.99 % 的标签都不是测试用的啦，所以一般也不设置)

2.标签大小
一共四个字节，但每个字节只用7位，最高位不使用恒为0。所以格式如下
0xxxxxxx 0xxxxxxx 0xxxxxxx 0xxxxxxx
计算大小时要将0去掉，得到一个28位的二进制数，就是标签大小(不懂为什么要这样做)，计算公式如下：
total_size = Size[0] * 0x200000
+ Size[1] * 0x4000
+ Size[2] * 0x80
+ Size[3]
#endif


struct ID3V2TagFrameHeader {
    unsigned char FrameID[4]; /*用四个字符标识一个帧，说明其内容，稍后有常用的标识对照表*/
    unsigned char Size[4]; /*帧内容的大小，不包括帧头，不得小于1*/
    unsigned char Flags[2]; /*存放标志，只定义了6位，稍后详细解说*/
};

#if 0
TIT2 = 标题 表示内容为这首歌的标题，下同
TPE1 = 作者
TALB = 专集
TRCK = 音轨 格式：N / M 其中N为专集中的第N首，M为专集中共M首，N和M为ASCII码表示的数字
TYER = 年代 是用ASCII码表示的数字
TCON = 类型 直接用字符串表示
COMM = 备注 格式："eng\0备注内容"，其中eng表示备注所使用的自然语言

只定义了6位，另外的10位为0，但大部分的情况下16位都为0就可以了。格式如下：
abc00000 ijk00000
a -- 标签保护标志，设置时认为此帧作废
b -- 文件保护标志，设置时认为此帧作废
c -- 只读标志，设置时认为此帧不能修改(但我没有找到一个软件理会这个标志)
i -- 压缩标志，设置时一个字节存放两个BCD码表示数字
j -- 加密标志，(没有见过哪个MP3文件的标签用了加密)
k -- 组标志，设置时说明此帧和其他的某帧是一组
值得一提的是winamp在保存和读取帧内容的时候会在内容前面加个'\0'，并把这个字节计算在帧内容的大小中。

#endif

struct MP3FrameHeader {
    unsigned int sync1;                      //同步信息 :12  同步信息（synchronizationword）11位皆为1
    unsigned int version;                    //版本 :2   00-MPEG 2.5  01-没有定义    10-MPEG 2    11-MPEG 1
    unsigned int layer;                      //层 :2   00-没有定义     01-Layer 3    10-Layer 2     11-Layer 1
    unsigned int crc_check;                  //CRC校验 :1 0-校验       1-不校验

    unsigned int bit_rate_index;             //比特率索引 :4
    unsigned int sample_rate_index;          //采样率索引 :2
    unsigned int padding;                    //帧长调节位 :1
    unsigned int reserved;                   //保留字 :1

    unsigned int nchannel;
    unsigned int channel_mode;               //声道模式 :2
    unsigned int mode_extension;             //扩展模式，仅用于联合立体声 :2
    unsigned int copyright;                  //版权标志 :1
    unsigned int original;                   //原版标志 :1
    unsigned int emphasis;                   //强调方式 :2
};




#if 0
bits   V1, L1     V1, L2     V1, L3     V2, L1      V2, L2      V2, L3
0000   free      free      free      free       free       free
0001   32        32        32        32(32)     32(8)      8(8)
0010   64        48        40        64(48)     48(16)     16(16)
0011   96        56        48        96(56)     56(24)     24(24)
0100   128       64        56        128(64)    64(32)     32(32)
0101   160       80        64        160(80)    80(40)     64(40)
0110   192       96        80        192(96)    96(48)     80(48)
0111   224       112       96        224(112)   112(56)    56(56)
1000   256       128       112       256(128)   128(64)    64(64)
1001   288       160       128       288(144)   160(80)    128(80)
1010   320       192       160       320(160)   192(96)    160(96)
1011   352       224       192       352(176)   224(112)   112(112)
1100   384       256       224       384(192)   256(128)   128(128)
1101   416       320       256       416(224)   320(144)   256(144)
1110   448       384       320       448(256)   384(160)   320(160)
1111   bad       bad       bad       bad        bad        bad
"free"表示位率可变
"bad" 表示不同意值

採样频率  2bit
MPEG - 1：   00 - 44.1kHz     01 - 48kHz   10 - 32kHz     11 - 没有定义
MPEG - 2：   00 - 22.05kHz    01 - 24kHz   10 - 16kHz     11 - 没有定义
MPEG - 2.5： 00 - 11.025kHz   01 - 12kHz   10 - 8kHz      11 - 没有定义

声道模式 : 2
00 - 立体声Stereo   01 - Joint Stereo   10 - 双声道       11 - 单声道

扩展模式，仅用于联合立体声 : 2
intensity stereo Mid - Side steeo
00  off                 off
01  off                 on
10  on                  off
11  on                  on
#endif

// 帧边信息解码的主要目的在于找出解这帧的各个參数。包含主数据開始位置，尺度因子长度等。
//  mpeg 1：单声道17个字节，双声道32字节
//  mpeg 2：单声道占9字节，双声道占17字节
struct FrameSide {
    int  main_data_begin; // 主数据开始指针：  9      
    int private_bit; //  single_channel:  5  else 3 

    // 粒度通常指的是将音频信号划分为不同的时间块或频率块的程度
    //   长粒度表示较长的时间窗口或频率窗口。
    //     在长粒度下，音频信号被划分为较大的时间块，通常为23.2毫秒。这种粒度级别适用于音频中的低频内容，如低音和持续音。
    //   短粒度表示较短的时间窗口或频率窗口 
    //     在短粒度下，音频信号被划分为较小的时间块，通常为11.6毫秒。这种粒度级别适用于音频中的高频内容，如快速变化的音符和音效。
    int scfsi[2][4]; // 两个粒度共有选择信息 nch * scfsi_band * 1

    struct Granularity {
        int part2_3_length; // 主数据位数:12   2 * 12
        int big_values;  // 大值:9       2 * 9
        int global_gain;  // 全局增益:8   2 * 8
        int scalefac_compress;  // 比例因子压缩:4   2 * 4
        int window_switching_flag;  // 窗切换标志:1  2 * 1

        // 窗切换标志为1时
        int block_type_1;  // 块类型:2 2 * 2
                            // block_type = 0 长块
                            // block_type = 1 开始块 
                            // block_type = 3 结束块 
                            // block_type = 2 短块



        int mixed_block_flag_1;  // 混合块标志:1  2 * 1
        int table_select_1[2];  // 表选择:5    2 * 5   2 * 2 * 5
        int subblock_gain_1[3];  // 子块增益:3    3 * 3    2 * 3 * 3

        // 窗切换标志为0时
        int table_select_0[3];  // 表选择:5    3 * 5  2 * 3 * 5
        int region0_count;  // 块类型:4  2 * 4
        int region1_count;  // 混合块标志:1  2 * 3

        // 
        int preflag; // 预标志:1  2 * 1
        int scalefac_scale;  // 比例因子-缩放:1   2 * 1
        int count1table_select; // 计数1表的选择:1   2 * 1
    }  gr[2][2];  // 2 * nch
};

struct ScaleCompress
{
    int slen1_;
    int slen2_;
    ScaleCompress(int slen1, int slen2) : slen1_(slen1), slen2_(slen2) {}
} s_scaleCompressLen[] = {
    ScaleCompress(0,0),
    ScaleCompress(0,1),
    ScaleCompress(0,2),
    ScaleCompress(0,3),
    ScaleCompress(3,0),
    ScaleCompress(1,1),
    ScaleCompress(1,2),
    ScaleCompress(1,3),
    ScaleCompress(2,0),
    ScaleCompress(2,1),
    ScaleCompress(2,2),
    ScaleCompress(3,3),
    ScaleCompress(3,0),
    ScaleCompress(3,1),
    ScaleCompress(4,2),
    ScaleCompress(4,3),
};


/*
       Gr0         |         Gr1
ch0_data  ch1_data   ch0_data  ch1_data


帧头+边信息+缩放因子+哈夫曼码

block_type= 0 1 3
slen1   0-10
slen2   11-20
part2_length = 11 * slen1 + 10 * slen2

block_type= 2  and mixed block flag = 0
slen1   0-5
slen2   6-11
part2_length = 3 * 6 * slen1 + 3 * 6 * slen2

block_type= 2  and mixed block flag = 1
slen1   0-10
slen2   11-16
part2_length = (8 + 3 * 3) * slen1 + 6 * 3 * slen2
*/
struct ChData {
    int scale_factor; // Granularity --- part2_length

    // huffman code --- part3_length
    //      * huffman表共有34个, 其中2个表给count1使用, 另外32个表给big_value使用. big_value区间
    // 		* 分成3个子区间region0, region1, region2. 各个region拥有各自的huffmantable.
    //     	* huffman表0-14的最大值不超过15(也就是说只能针对最大值不超过15的子区间编码), 表15-31的最
    //     	* 小值皆为15, 如果要针对超过15的频线编码, 就需要使用linbits的方法.

 


    int rzero;  // part3_length + rzero_lenght = 576

};


int main(int argn, char** argv) {
    FileBitReader fbr("D://test.mp3");


    // ------------------------ parse id3v2 head ----------------------------
    ID3V2 v2_head;
    fbr.ReadBuffer((char*)&v2_head, sizeof(v2_head));
    int v2_size = (v2_head.Size[0] * 0x200000) + (v2_head.Size[1] * 0x4000 ) + (v2_head.Size[2] * 0x80) + (v2_head.Size[3]);
    if (true)
    {
        ::std::cout << "--------------------------------------" << ::std::endl;
        ::std::cout << "ID3V2 Header: " << v2_head.Header[0] << v2_head.Header[1] << v2_head.Header[2] << ::std::endl;
        ::std::cout << "ID3V2 Ver: " << (int)v2_head.Ver << ::std::endl;
        ::std::cout << "ID3V2 Revision: " << (int)v2_head.Revision << ::std::endl;
        ::std::cout << "ID3V2 Flag: " << (int)v2_head.Flag << ::std::endl;
        ::std::cout << "ID3V2 Size: " << v2_size << ::std::endl;
        ::std::cout << "--------------------------------------" << ::std::endl;
    }


    // ------------------------ parse id3v2 tag ----------------------------
    int total_count = 0;// sizeof(v2_head);
    while (total_count < v2_size) {

        ID3V2TagFrameHeader v2_tag_header;
        fbr.ReadBuffer((char*)&v2_tag_header, sizeof(v2_tag_header)); 

        int v2_tag_size = (v2_tag_header.Size[0] * 0x1000000) + (v2_tag_header.Size[1] * 0x10000) + (v2_tag_header.Size[2] * 0x100) + (v2_tag_header.Size[3]);
        char* v2_tag_data = new char[v2_tag_size + 1];
        fbr.ReadBuffer((char*)v2_tag_data, v2_tag_size);
        v2_tag_data[v2_tag_size] = '\0';

        ::std::cout << v2_tag_header.FrameID[0] << v2_tag_header.FrameID[1] << v2_tag_header.FrameID[2]
            << v2_tag_header.FrameID[3] << "  :  " << v2_tag_data << ::std::endl;

        delete []v2_tag_data;

        total_count += v2_tag_size + sizeof(v2_tag_header);
    }

    // ------------------------ skip padding filling code ----------------------------
    unsigned char tmp = 0;
    bool already_read_one_bits = false;
    while (true) {
        fbr.ReadBuffer((char*)&tmp, sizeof(tmp));
        if (tmp == 0xff) {
            already_read_one_bits = true;
            break;
        }
    }
    
    // ------------------------ parse every audio frame ----------------------------
    int frame_idx = 0;
    {
          ::std::cout << "------------------ " << frame_idx << " --------------------" << ::std::endl;

        // ------------------------ parse frame header ----------------------------
        MP3FrameHeader frame_header;
        if (already_read_one_bits) {
            frame_header.sync1 = ((unsigned int)tmp << 3) | (fbr.ReadBits(3));
            already_read_one_bits = false;
        } else   {
            frame_header.sync1 = fbr.ReadBits(11);
        }
        MYASSERT(frame_header.sync1 == 2047);
        frame_header.version = fbr.ReadBits(2);
    
        {
            const char* version_desc[] = { " MPEG 2.5", "没有定义", "MPEG 2", "MPEG 1" };
            ::std::cout << "version : " << version_desc[frame_header.version] << ::std::endl;
        }


        frame_header.layer = fbr.ReadBits(2);
        {
            const char* layer_desc[] = { "没有定义", "Layer 3", "Layer 2", "Layer 11" };
            ::std::cout << "layer : " << layer_desc[frame_header.layer] << ::std::endl;
        }

        frame_header.crc_check = fbr.ReadBits(1);

        frame_header.bit_rate_index = fbr.ReadBits(4);
        {
            const char* bitrate_desc[] = { 
"V1, L1    V1, L2    V1, L3    V2, L1     V2, L2     V2, L3",
"free      free      free      free       free       free",
"32        32        32        32(32)     32(8)      8(8)",
"64        48        40        64(48)     48(16)     16(16)",
"96        56        48        96(56)     56(24)     24(24)",
"128       64        56        128(64)    64(32)     32(32)",
"160       80        64        160(80)    80(40)     64(40)",
"192       96        80        192(96)    96(48)     80(48)",
"224       112       96        224(112)   112(56)    56(56)",
"256       128       112       256(128)   128(64)    64(64)",
"288       160       128       288(144)   160(80)    128(80)",
"320       192       160       320(160)   192(96)    160(96)",
"352       224       192       352(176)   224(112)   112(112)",
"384       256       224       384(192)   256(128)   128(128)",
"416       320       256       416(224)   320(144)   256(144))",
"448       384       320       448(256)   384(160)   320(160))",
"bad       bad       bad       bad        bad        bad",
            };
            ::std::cout << "bitrate : " << bitrate_desc[0] << ::std::endl;
            ::std::cout << "        : " << bitrate_desc[frame_header.bit_rate_index + 1] << ::std::endl;
        }

        frame_header.sample_rate_index = fbr.ReadBits(2);
        {
            const char* sample_rate_desc[] = {
                "MPEG - 1: 44.1k     MPEG - 2: 22.05k     MPEG - 2.5: 11.025k  ",
                "MPEG - 1: 48k       MPEG - 2: 24k        MPEG - 2.5: 12k  ",
                "MPEG - 1: 32k       MPEG - 2: 16k        MPEG - 2.5: 85k  ",
                "MPEG - 1: 没有定义  MPEG - 2: 没有定义   MPEG - 2.5: 没有定义  ",
            };
            ::std::cout << "layer : " << sample_rate_desc[frame_header.sample_rate_index] << ::std::endl;
        }


        frame_header.padding = fbr.ReadBits(1);
        frame_header.reserved = fbr.ReadBits(1);

        frame_header.channel_mode = fbr.ReadBits(2);
        frame_header.nchannel = (frame_header.channel_mode == 3) ? 1 : 2;
        {
            const char* channel_mode_desc[] = { "立体声Stereo", "Joint Stereo", "双声道", "单声道" };
            ::std::cout << "layer : " << channel_mode_desc[frame_header.channel_mode] << ::std::endl;
        }

        frame_header.mode_extension = fbr.ReadBits(2);
        {
            const char* mode_extension_desc[] = { "IS off, MS off", "IS off, MS on", "IS on, MS off", "IS on, MS on" };
            ::std::cout << "layer : " << mode_extension_desc[frame_header.mode_extension] << ::std::endl;
        }


        frame_header.copyright = fbr.ReadBits(1);
        frame_header.original = fbr.ReadBits(1);
        frame_header.emphasis = fbr.ReadBits(2);

        // ------------------------ parse crc check ----------------------------
        if (frame_header.crc_check == 0) {
            fbr.ReadBits(2);
        }

        // ------------------------ parse frame side ----------------------------
        FrameSide frame_side;
        frame_side.main_data_begin = fbr.ReadBits(9);
        if (frame_header.nchannel == 1) {
            frame_side.private_bit = fbr.ReadBits(5);
        }  else {
            frame_side.private_bit = fbr.ReadBits(3);
        }

        for (int nch = 0; nch < frame_header.nchannel; nch++) {
            for (int scfsi_band = 0; scfsi_band < 4; scfsi_band++) {
                frame_side.scfsi[nch][scfsi_band] = fbr.ReadBits(1);
            }
        }

        for (int gr = 0; gr < 2; gr++) {
            for (int nch = 0; nch < frame_header.nchannel; nch++) {
                auto& gran = frame_side.gr[gr][nch];
                gran.part2_3_length = fbr.ReadBits(12);
                gran.big_values = fbr.ReadBits(9);
                gran.global_gain = fbr.ReadBits(8);
                gran.scalefac_compress = fbr.ReadBits(4);
                gran.window_switching_flag = fbr.ReadBits(1);
               
                if (gran.window_switching_flag == 1) {
                    gran.block_type_1 = fbr.ReadBits(2);
                    gran.mixed_block_flag_1 = fbr.ReadBits(1);
                    gran.table_select_1[0] = fbr.ReadBits(5);
                    gran.table_select_1[1] = fbr.ReadBits(5);
                    gran.subblock_gain_1[0] = fbr.ReadBits(3);
                    gran.subblock_gain_1[1] = fbr.ReadBits(3);
                    gran.subblock_gain_1[2] = fbr.ReadBits(3);
                }
                else {
                    gran.table_select_0[0] = fbr.ReadBits(5);
                    gran.table_select_0[1] = fbr.ReadBits(5);
                    gran.table_select_0[2] = fbr.ReadBits(5);
                    gran.region0_count = fbr.ReadBits(4);
                    gran.region1_count = fbr.ReadBits(3);
                }

                gran.preflag = fbr.ReadBits(1);
                gran.scalefac_scale = fbr.ReadBits(1);
                gran.count1table_select = fbr.ReadBits(1);
            }
        }

        int debug = 0;

        ::std::cout << "--------------------------------------" << ::std::endl;

        frame_idx++;
    }



    int debug = 0;
    return 0;
}

