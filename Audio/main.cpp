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
    unsigned char Title[30]; /*����*/
    unsigned char Artist[30]; /*����*/
    unsigned char Album[30]; /*ר��*/
    unsigned char Year[4]; /*��Ʒ���*/
    unsigned char Comment[30]; /*��ע   28��ע��+2�������ʶ */
    unsigned char Genre; /*����*/
};


// id3v2 
struct ID3V2 {
    unsigned char Header[3]; /*����Ϊ"ID3"������Ϊ��ǩ������*/
    unsigned char Ver; /*�汾��;ID3V2.3�ͼ�¼03,ID3V2.4�ͼ�¼04*/
    unsigned char Revision; /*���汾��;�˰汾��¼Ϊ00*/
    unsigned char Flag; /*��ű�־���ֽڣ�����汾ֻ��������λ���Ժ���ϸ��˵*/
    unsigned char Size[4]; /*��ǩ��С��������ǩ֡�ͱ�ǩͷ������������չ��ǩͷ��10���ֽڣ�*/
};

#if 0
1.��־�ֽ�
��־�ֽ�һ��Ϊ0���������£�
abc00000
a -- ��ʾ�Ƿ�ʹ�ò�ͬ��(һ�㲻����)
b -- ��ʾ�Ƿ�����չͷ����һ��û��(����Winampû�м�¼)������һ��Ҳ������
c -- ��ʾ�Ƿ�Ϊ���Ա�ǩ(99.99 % �ı�ǩ�����ǲ����õ���������һ��Ҳ������)

2.��ǩ��С
һ���ĸ��ֽڣ���ÿ���ֽ�ֻ��7λ�����λ��ʹ�ú�Ϊ0�����Ը�ʽ����
0xxxxxxx 0xxxxxxx 0xxxxxxx 0xxxxxxx
�����СʱҪ��0ȥ�����õ�һ��28λ�Ķ������������Ǳ�ǩ��С(����ΪʲôҪ������)�����㹫ʽ���£�
total_size = Size[0] * 0x200000
+ Size[1] * 0x4000
+ Size[2] * 0x80
+ Size[3]
#endif


struct ID3V2TagFrameHeader {
    unsigned char FrameID[4]; /*���ĸ��ַ���ʶһ��֡��˵�������ݣ��Ժ��г��õı�ʶ���ձ�*/
    unsigned char Size[4]; /*֡���ݵĴ�С��������֡ͷ������С��1*/
    unsigned char Flags[2]; /*��ű�־��ֻ������6λ���Ժ���ϸ��˵*/
};

#if 0
TIT2 = ���� ��ʾ����Ϊ���׸�ı��⣬��ͬ
TPE1 = ����
TALB = ר��
TRCK = ���� ��ʽ��N / M ����NΪר���еĵ�N�ף�MΪר���й�M�ף�N��MΪASCII���ʾ������
TYER = ��� ����ASCII���ʾ������
TCON = ���� ֱ�����ַ�����ʾ
COMM = ��ע ��ʽ��"eng\0��ע����"������eng��ʾ��ע��ʹ�õ���Ȼ����

ֻ������6λ�������10λΪ0�����󲿷ֵ������16λ��Ϊ0�Ϳ����ˡ���ʽ���£�
abc00000 ijk00000
a -- ��ǩ������־������ʱ��Ϊ��֡����
b -- �ļ�������־������ʱ��Ϊ��֡����
c -- ֻ����־������ʱ��Ϊ��֡�����޸�(����û���ҵ�һ�������������־)
i -- ѹ����־������ʱһ���ֽڴ������BCD���ʾ����
j -- ���ܱ�־��(û�м����ĸ�MP3�ļ��ı�ǩ���˼���)
k -- ���־������ʱ˵����֡��������ĳ֡��һ��
ֵ��һ�����winamp�ڱ���Ͷ�ȡ֡���ݵ�ʱ���������ǰ��Ӹ�'\0'����������ֽڼ�����֡���ݵĴ�С�С�

#endif

struct MP3FrameHeader {
    unsigned int sync1;                      //ͬ����Ϣ :12  ͬ����Ϣ��synchronizationword��11λ��Ϊ1
    unsigned int version;                    //�汾 :2   00-MPEG 2.5  01-û�ж���    10-MPEG 2    11-MPEG 1
    unsigned int layer;                      //�� :2   00-û�ж���     01-Layer 3    10-Layer 2     11-Layer 1
    unsigned int crc_check;                  //CRCУ�� :1 0-У��       1-��У��

    unsigned int bit_rate_index;             //���������� :4
    unsigned int sample_rate_index;          //���������� :2
    unsigned int padding;                    //֡������λ :1
    unsigned int reserved;                   //������ :1

    unsigned int nchannel;
    unsigned int channel_mode;               //����ģʽ :2
    unsigned int mode_extension;             //��չģʽ������������������ :2
    unsigned int copyright;                  //��Ȩ��־ :1
    unsigned int original;                   //ԭ���־ :1
    unsigned int emphasis;                   //ǿ����ʽ :2
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
"free"��ʾλ�ʿɱ�
"bad" ��ʾ��ͬ��ֵ

����Ƶ��  2bit
MPEG - 1��   00 - 44.1kHz     01 - 48kHz   10 - 32kHz     11 - û�ж���
MPEG - 2��   00 - 22.05kHz    01 - 24kHz   10 - 16kHz     11 - û�ж���
MPEG - 2.5�� 00 - 11.025kHz   01 - 12kHz   10 - 8kHz      11 - û�ж���

����ģʽ : 2
00 - ������Stereo   01 - Joint Stereo   10 - ˫����       11 - ������

��չģʽ������������������ : 2
intensity stereo Mid - Side steeo
00  off                 off
01  off                 on
10  on                  off
11  on                  on
#endif

// ֡����Ϣ�������ҪĿ�������ҳ�����֡�ĸ��������������������_ʼλ�ã��߶����ӳ��ȵȡ�
//  mpeg 1��������17���ֽڣ�˫����32�ֽ�
//  mpeg 2��������ռ9�ֽڣ�˫����ռ17�ֽ�
struct FrameSide {
    int  main_data_begin; // �����ݿ�ʼָ�룺  9      
    int private_bit; //  single_channel:  5  else 3 

    // ����ͨ��ָ���ǽ���Ƶ�źŻ���Ϊ��ͬ��ʱ����Ƶ�ʿ�ĳ̶�
    //   �����ȱ�ʾ�ϳ���ʱ�䴰�ڻ�Ƶ�ʴ��ڡ�
    //     �ڳ������£���Ƶ�źű�����Ϊ�ϴ��ʱ��飬ͨ��Ϊ23.2���롣�������ȼ�����������Ƶ�еĵ�Ƶ���ݣ�������ͳ�������
    //   �����ȱ�ʾ�϶̵�ʱ�䴰�ڻ�Ƶ�ʴ��� 
    //     �ڶ������£���Ƶ�źű�����Ϊ��С��ʱ��飬ͨ��Ϊ11.6���롣�������ȼ�����������Ƶ�еĸ�Ƶ���ݣ�����ٱ仯����������Ч��
    int scfsi[2][4]; // �������ȹ���ѡ����Ϣ nch * scfsi_band * 1

    struct Granularity {
        int part2_3_length; // ������λ��:12   2 * 12
        int big_values;  // ��ֵ:9       2 * 9
        int global_gain;  // ȫ������:8   2 * 8
        int scalefac_compress;  // ��������ѹ��:4   2 * 4
        int window_switching_flag;  // ���л���־:1  2 * 1

        // ���л���־Ϊ1ʱ
        int block_type_1;  // ������:2 2 * 2
                            // block_type = 0 ����
                            // block_type = 1 ��ʼ�� 
                            // block_type = 3 ������ 
                            // block_type = 2 �̿�



        int mixed_block_flag_1;  // ��Ͽ��־:1  2 * 1
        int table_select_1[2];  // ��ѡ��:5    2 * 5   2 * 2 * 5
        int subblock_gain_1[3];  // �ӿ�����:3    3 * 3    2 * 3 * 3

        // ���л���־Ϊ0ʱ
        int table_select_0[3];  // ��ѡ��:5    3 * 5  2 * 3 * 5
        int region0_count;  // ������:4  2 * 4
        int region1_count;  // ��Ͽ��־:1  2 * 3

        // 
        int preflag; // Ԥ��־:1  2 * 1
        int scalefac_scale;  // ��������-����:1   2 * 1
        int count1table_select; // ����1���ѡ��:1   2 * 1
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


֡ͷ+����Ϣ+��������+��������

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
    //      * huffman����34��, ����2�����count1ʹ��, ����32�����big_valueʹ��. big_value����
    // 		* �ֳ�3��������region0, region1, region2. ����regionӵ�и��Ե�huffmantable.
    //     	* huffman��0-14�����ֵ������15(Ҳ����˵ֻ��������ֵ������15�����������), ��15-31����
    //     	* Сֵ��Ϊ15, ���Ҫ��Գ���15��Ƶ�߱���, ����Ҫʹ��linbits�ķ���.

 


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
            const char* version_desc[] = { " MPEG 2.5", "û�ж���", "MPEG 2", "MPEG 1" };
            ::std::cout << "version : " << version_desc[frame_header.version] << ::std::endl;
        }


        frame_header.layer = fbr.ReadBits(2);
        {
            const char* layer_desc[] = { "û�ж���", "Layer 3", "Layer 2", "Layer 11" };
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
                "MPEG - 1: û�ж���  MPEG - 2: û�ж���   MPEG - 2.5: û�ж���  ",
            };
            ::std::cout << "layer : " << sample_rate_desc[frame_header.sample_rate_index] << ::std::endl;
        }


        frame_header.padding = fbr.ReadBits(1);
        frame_header.reserved = fbr.ReadBits(1);

        frame_header.channel_mode = fbr.ReadBits(2);
        frame_header.nchannel = (frame_header.channel_mode == 3) ? 1 : 2;
        {
            const char* channel_mode_desc[] = { "������Stereo", "Joint Stereo", "˫����", "������" };
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

