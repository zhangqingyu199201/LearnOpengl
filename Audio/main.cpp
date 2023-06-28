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



struct HuffmanCode {
    int x{ 0 };
    int y{ 0 };
    int hlen{ 0 };
    ::std::string hcod;
    int is_leaf{ false };
    HuffmanCode* l{ nullptr };
    HuffmanCode* r{ nullptr };

    HuffmanCode() {}

    HuffmanCode(int _x, int _y, int _hlen, ::std::string _hcod) :
        x(_x), y(_y), hlen(_hlen), hcod(_hcod) {
        is_leaf = true;
    }
};

static HuffmanCode* GetBigHuffmanCodeTableRoot() {
    static HuffmanCode* root = nullptr;
    if (root != nullptr) {
        return root;
    }

    HuffmanCode* tmp_0 = new HuffmanCode(0, 0, 1, "1");
    HuffmanCode* tmp_1 = new HuffmanCode(0, 1, 3, "010");
    HuffmanCode* tmp_2 = new HuffmanCode(0, 2, 6, "001010");
    HuffmanCode* tmp_3 = new HuffmanCode(0, 3, 8, "00010011");
    HuffmanCode* tmp_4 = new HuffmanCode(0, 4, 8, "00010000");
    HuffmanCode* tmp_5 = new HuffmanCode(0, 5, 9, "000001010");
    HuffmanCode* tmp_6 = new HuffmanCode(1, 0, 3, "011");
    HuffmanCode* tmp_7 = new HuffmanCode(1, 1, 4, "0011");
    HuffmanCode* tmp_8 = new HuffmanCode(1, 2, 6, "000111");
    HuffmanCode* tmp_9 = new HuffmanCode(1, 3, 7, "0001010");
    HuffmanCode* tmp_10 = new HuffmanCode(1, 4, 7, "0000101");
    HuffmanCode* tmp_11 = new HuffmanCode(1, 5, 8, "00000011");
    HuffmanCode* tmp_12 = new HuffmanCode(2, 0, 6, "001011");
    HuffmanCode* tmp_13 = new HuffmanCode(2, 1, 5, "00100");
    HuffmanCode* tmp_14 = new HuffmanCode(2, 2, 7, "0001101");
    HuffmanCode* tmp_15 = new HuffmanCode(2, 3, 8, "00010001");
    HuffmanCode* tmp_16 = new HuffmanCode(2, 4, 8, "00001000");
    HuffmanCode* tmp_17 = new HuffmanCode(2, 5, 9, "000000100");
    HuffmanCode* tmp_18 = new HuffmanCode(3, 0, 7, "0001100");
    HuffmanCode* tmp_19 = new HuffmanCode(3, 1, 7, "0001011");
    HuffmanCode* tmp_20 = new HuffmanCode(3, 2, 8, "00010010");
    HuffmanCode* tmp_21 = new HuffmanCode(3, 3, 9, "000001111");
    HuffmanCode* tmp_22 = new HuffmanCode(3, 4, 9, "000001011");
    HuffmanCode* tmp_23 = new HuffmanCode(3, 5, 9, "000000010");
    HuffmanCode* tmp_24 = new HuffmanCode(4, 0, 7, "0000111");
    HuffmanCode* tmp_25 = new HuffmanCode(4, 1, 7, "0000110");
    HuffmanCode* tmp_26 = new HuffmanCode(4, 2, 8, "00001001");
    HuffmanCode* tmp_27 = new HuffmanCode(4, 3, 9, "000001110");
    HuffmanCode* tmp_28 = new HuffmanCode(4, 4, 9, "000000011");
    HuffmanCode* tmp_29 = new HuffmanCode(4, 5, 10, "0000000001");
    HuffmanCode* tmp_30 = new HuffmanCode(5, 0, 8, "00000110");
    HuffmanCode* tmp_31 = new HuffmanCode(5, 1, 8, "00000100");
    HuffmanCode* tmp_32 = new HuffmanCode(5, 2, 9, "000000101");
    HuffmanCode* tmp_33 = new HuffmanCode(5, 3, 10, "0000000011");
    HuffmanCode* tmp_34 = new HuffmanCode(5, 4, 10, "0000000010");
    HuffmanCode* tmp_35 = new HuffmanCode(5, 5, 10, "0000000000");

    root = new HuffmanCode();
    root->r = tmp_0;
    root->l = new HuffmanCode();
    root->l->r = new HuffmanCode();
    root->l->r->r = tmp_6;
    root->l->r->l = tmp_1;
    root->l->l = new HuffmanCode();
    root->l->l->r = new HuffmanCode();
    root->l->l->r->r = tmp_7;
    root->l->l->r->l = new HuffmanCode();
    root->l->l->r->l->r = new HuffmanCode();
    root->l->l->r->l->r->r = tmp_12;
    root->l->l->r->l->r->l = tmp_2;
    root->l->l->r->l->l = tmp_13;
    root->l->l->l = new HuffmanCode();
    root->l->l->l->r = new HuffmanCode();
    root->l->l->l->r->r = new HuffmanCode();
    root->l->l->l->r->r->r = tmp_8;
    root->l->l->l->r->r->l = new HuffmanCode();
    root->l->l->l->r->r->l->r = tmp_14;
    root->l->l->l->r->r->l->l = tmp_18;
    root->l->l->l->r->l = new HuffmanCode();
    root->l->l->l->r->l->r = new HuffmanCode();
    root->l->l->l->r->l->r->r = tmp_19;
    root->l->l->l->r->l->r->l = tmp_9;
    root->l->l->l->r->l->l = new HuffmanCode();
    root->l->l->l->r->l->l->r = new HuffmanCode();
    root->l->l->l->r->l->l->r->r = tmp_3;
    root->l->l->l->r->l->l->r->l = tmp_20;
    root->l->l->l->r->l->l->l = new HuffmanCode();
    root->l->l->l->r->l->l->l->r = tmp_15;
    root->l->l->l->r->l->l->l->l = tmp_4;
    root->l->l->l->l = new HuffmanCode();
    root->l->l->l->l->r = new HuffmanCode();
    root->l->l->l->l->r->r = new HuffmanCode();
    root->l->l->l->l->r->r->r = tmp_24;
    root->l->l->l->l->r->r->l = tmp_25;
    root->l->l->l->l->r->l = new HuffmanCode();
    root->l->l->l->l->r->l->r = tmp_10;
    root->l->l->l->l->r->l->l = new HuffmanCode();
    root->l->l->l->l->r->l->l->r = tmp_26;
    root->l->l->l->l->r->l->l->l = tmp_16;
    root->l->l->l->l->l = new HuffmanCode();
    root->l->l->l->l->l->r = new HuffmanCode();
    root->l->l->l->l->l->r->r = new HuffmanCode();
    root->l->l->l->l->l->r->r->r = new HuffmanCode();
    root->l->l->l->l->l->r->r->r->r = tmp_21;
    root->l->l->l->l->l->r->r->r->l = tmp_27;
    root->l->l->l->l->l->r->r->l = tmp_30;
    root->l->l->l->l->l->r->l = new HuffmanCode();
    root->l->l->l->l->l->r->l->r = new HuffmanCode();
    root->l->l->l->l->l->r->l->r->r = tmp_22;
    root->l->l->l->l->l->r->l->r->l = tmp_5;
    root->l->l->l->l->l->r->l->l = tmp_31;
    root->l->l->l->l->l->l = new HuffmanCode();
    root->l->l->l->l->l->l->r = new HuffmanCode();
    root->l->l->l->l->l->l->r->r = tmp_11;
    root->l->l->l->l->l->l->r->l = new HuffmanCode();
    root->l->l->l->l->l->l->r->l->r = tmp_32;
    root->l->l->l->l->l->l->r->l->l = tmp_17;
    root->l->l->l->l->l->l->l = new HuffmanCode();
    root->l->l->l->l->l->l->l->r = new HuffmanCode();
    root->l->l->l->l->l->l->l->r->r = tmp_28;
    root->l->l->l->l->l->l->l->r->l = tmp_23;
    root->l->l->l->l->l->l->l->l = new HuffmanCode();
    root->l->l->l->l->l->l->l->l->r = new HuffmanCode();
    root->l->l->l->l->l->l->l->l->r->r = tmp_33;
    root->l->l->l->l->l->l->l->l->r->l = tmp_34;
    root->l->l->l->l->l->l->l->l->l = new HuffmanCode();
    root->l->l->l->l->l->l->l->l->l->r = tmp_29;
    root->l->l->l->l->l->l->l->l->l->l = tmp_35;

    return root;
}


static HuffmanCode* GetSmallHuffmanCodeTableRoot() {
    static HuffmanCode* root = nullptr;
    if (root != nullptr) {
        return root;
    }

    HuffmanCode* tmp_0 = new HuffmanCode(0, 0, 1, "1");
    HuffmanCode* tmp_1 = new HuffmanCode(0, 1, 4, "0101");
    HuffmanCode* tmp_2 = new HuffmanCode(0, 2, 4, "0100");
    HuffmanCode* tmp_3 = new HuffmanCode(0, 3, 5, "00101");
    HuffmanCode* tmp_4 = new HuffmanCode(0, 4, 4, "0110");
    HuffmanCode* tmp_5 = new HuffmanCode(0, 5, 6, "000101");
    HuffmanCode* tmp_6 = new HuffmanCode(1, 6, 5, "00100");
    HuffmanCode* tmp_7 = new HuffmanCode(1, 7, 6, "000100");
    HuffmanCode* tmp_8 = new HuffmanCode(1, 8, 4, "0111");
    HuffmanCode* tmp_9 = new HuffmanCode(1, 9, 5, "00011");
    HuffmanCode* tmp_10 = new HuffmanCode(1, 10, 5, "00110");
    HuffmanCode* tmp_11 = new HuffmanCode(1, 11, 6, "000000");
    HuffmanCode* tmp_12 = new HuffmanCode(2, 12, 5, "00111");
    HuffmanCode* tmp_13 = new HuffmanCode(2, 13, 6, "000010");
    HuffmanCode* tmp_14 = new HuffmanCode(2, 14, 6, "000011");
    HuffmanCode* tmp_15 = new HuffmanCode(2, 15, 6, "000001");

    root = new HuffmanCode();
    root->r = tmp_0;
    root->l = new HuffmanCode();
    root->l->r = new HuffmanCode();
    root->l->r->r = new HuffmanCode();
    root->l->r->r->r = tmp_8;
    root->l->r->r->l = tmp_4;
    root->l->r->l = new HuffmanCode();
    root->l->r->l->r = tmp_1;
    root->l->r->l->l = tmp_2;
    root->l->l = new HuffmanCode();
    root->l->l->r = new HuffmanCode();
    root->l->l->r->r = new HuffmanCode();
    root->l->l->r->r->r = tmp_12;
    root->l->l->r->r->l = tmp_10;
    root->l->l->r->l = new HuffmanCode();
    root->l->l->r->l->r = tmp_3;
    root->l->l->r->l->l = tmp_6;
    root->l->l->l = new HuffmanCode();
    root->l->l->l->r = new HuffmanCode();
    root->l->l->l->r->r = tmp_9;
    root->l->l->l->r->l = new HuffmanCode();
    root->l->l->l->r->l->r = tmp_5;
    root->l->l->l->r->l->l = tmp_7;
    root->l->l->l->l = new HuffmanCode();
    root->l->l->l->l->r = new HuffmanCode();
    root->l->l->l->l->r->r = tmp_14;
    root->l->l->l->l->r->l = tmp_13;
    root->l->l->l->l->l = new HuffmanCode();
    root->l->l->l->l->l->r = tmp_15;
    root->l->l->l->l->l->l = tmp_11;

    return root;
}

static HuffmanCode* GetBigHuffmanCode(::std::string val) {
    HuffmanCode* root = GetBigHuffmanCodeTableRoot();
    const char* v_str = val.c_str();

    while (!root->is_leaf) {
        if (*v_str == '0') {
            root = root->l;
        }
        else {
            root = root->r;
        }
        v_str++;
    }
    return root;
}

static HuffmanCode* GetBigHuffmanCode(FileBitReader& fbr) {
    HuffmanCode* root = GetBigHuffmanCodeTableRoot();

    while (!root->is_leaf) {
        if (fbr.ReadOneBits() == 0) {
            root = root->l;
        }
        else {
            root = root->r;
        }
    }
    return root;
}




static HuffmanCode* GetSmallHuffmanCode(::std::string val) {
    HuffmanCode* root = GetSmallHuffmanCodeTableRoot();
    const char* v_str = val.c_str();

    while (!root->is_leaf) {
        if (*v_str == '0') {
            root = root->l;
        }
        else {
            root = root->r;
        }
        v_str++;
    }
    return root;
}

static HuffmanCode* GetSmallHuffmanCode(FileBitReader& fbr) {
    HuffmanCode* root = GetBigHuffmanCodeTableRoot();

    while (!root->is_leaf) {
        if (fbr.ReadOneBits() == 0) {
            root = root->l;
        }
        else {
            root = root->r;
        }
    }
    return root;
}


static void TestBigHuffmanCode() {
    auto fun = [](int x, int y, int hlen, ::std::string code) {
        ::std::cout << code << ::std::endl;
        HuffmanCode* root = GetBigHuffmanCode(code);
        MYASSERT(root->x == x);
        MYASSERT(root->y == y);
        MYASSERT(root->hlen == hlen);
        MYASSERT(root->hcod == code);
    };
    fun(0, 0, 1, "1");
    fun(0, 1, 3, "010");
    fun(0, 2, 6, "001010");
    fun(0, 3, 8, "00010011");
    fun(0, 4, 8, "00010000");
    fun(0, 5, 9, "000001010");
    fun(1, 0, 3, "011");
    fun(1, 1, 4, "0011");
    fun(1, 2, 6, "000111");
    fun(1, 3, 7, "0001010");
    fun(1, 4, 7, "0000101");
    fun(1, 5, 8, "00000011");
    fun(2, 0, 6, "001011");
    fun(2, 1, 5, "00100");
    fun(2, 2, 7, "0001101");
    fun(2, 3, 8, "00010001");
    fun(2, 4, 8, "00001000");
    fun(2, 5, 9, "000000100");
    fun(3, 0, 7, "0001100");
    fun(3, 1, 7, "0001011");
    fun(3, 2, 8, "00010010");
    fun(3, 3, 9, "000001111");
    fun(3, 4, 9, "000001011");
    fun(3, 5, 9, "000000010");
    fun(4, 0, 7, "0000111");
    fun(4, 1, 7, "0000110");
    fun(4, 2, 8, "00001001");
    fun(4, 3, 9, "000001110");
    fun(4, 4, 9, "000000011");
    fun(4, 5, 10, "0000000001");
    fun(5, 0, 8, "00000110");
    fun(5, 1, 8, "00000100");
    fun(5, 2, 9, "000000101");
    fun(5, 3, 10, "0000000011");
    fun(5, 4, 10, "0000000010");
    fun(5, 5, 10, "0000000000");
}

static void TestSmallHuffmanCode() {
    auto fun = [](int x, int y, int hlen, ::std::string code) {
        ::std::cout << code << ::std::endl;
        HuffmanCode* root = GetSmallHuffmanCode(code);
        MYASSERT(root->x == x);
        MYASSERT(root->y == y);
        MYASSERT(root->hlen == hlen);
        MYASSERT(root->hcod == code);
    };
    fun(0, 0, 1, "1");
    fun(0, 1, 4, "0101");
    fun(0, 2, 4, "0100");
    fun(0, 3, 5, "00101");
    fun(0, 4, 4, "0110");
    fun(0, 5, 6, "000101");
    fun(1, 6, 5, "00100");
    fun(1, 7, 6, "000100");
    fun(1, 8, 4, "0111");
    fun(1, 9, 5, "00011");
    fun(1, 10, 5, "00110");
    fun(1, 11, 6, "000000");
    fun(2, 12, 5, "00111");
    fun(2, 13, 6, "000010");
    fun(2, 14, 6, "000011");
    fun(2, 15, 6, "000001");
}









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

        if (frame_side.main_data_begin != 0) {
            char* buff_tmp = new char[frame_side.main_data_begin];
            fbr.ReadBuffer(buff_tmp, frame_side.main_data_begin);
            delete[]buff_tmp;
        }


        int debug = 0;

        ::std::cout << "--------------------------------------" << ::std::endl;

        frame_idx++;
    }



    int debug = 0;
    return 0;
}

