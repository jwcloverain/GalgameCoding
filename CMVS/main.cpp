#include <iostream>
#include <array>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#undef min
#undef max


namespace CMV
{
    template<typename T1, typename T2>
    T1 SaturateCast(T2 val)
    {
        if (val < std::numeric_limits<T1>::min())
            return std::numeric_limits<T1>::min();
        if (val > std::numeric_limits<T1>::max())
            return std::numeric_limits<T1>::max();
        return static_cast<T1>(val);
    }


// IDCT CODE FROM https://github.com/ShiftMediaProject/xvid/blob/master/src/dct/simple_idct.c

#define W1  22725  /* cos(i*M_PI/16)*sqrt(2)*(1<<14) + 0.5 */
#define W2  21407  /* cos(i*M_PI/16)*sqrt(2)*(1<<14) + 0.5 */
#define W3  19266  /* cos(i*M_PI/16)*sqrt(2)*(1<<14) + 0.5 */
#define W4  16383  /* cos(i*M_PI/16)*sqrt(2)*(1<<14) + 0.5 */
#define W5  12873  /* cos(i*M_PI/16)*sqrt(2)*(1<<14) + 0.5 */
#define W6  8867   /* cos(i*M_PI/16)*sqrt(2)*(1<<14) + 0.5 */
#define W7  4520   /* cos(i*M_PI/16)*sqrt(2)*(1<<14) + 0.5 */
#define ROW_SHIFT 11
#define COL_SHIFT 20 /* 6 */


#define MAC16(rt, ra, rb) rt += ((ra) * (rb))
#define MUL16(rt, ra, rb) rt = ((ra) * (rb))


    void IdctRowCondDC(int16_t* row)
    {
        int a0, a1, a2, a3, b0, b1, b2, b3;

        if (row[1] == 0 && row[2] == 0 && row[3] == 0 && row[4] == 0 && row[5] == 0 && row[6] == 0 && row[7] == 0)
        {
            row[0] = row[1] = row[2] = row[3] = row[4] = row[5] = row[6] = row[7] = (int16_t)(row[0] << 3);
            return;
        }

        a0 = (W4 * row[0]) + (1 << (ROW_SHIFT - 1));
        a1 = a0;
        a2 = a0;
        a3 = a0;

        a0 += W2 * row[2];
        a1 += W6 * row[2];
        a2 -= W6 * row[2];
        a3 -= W2 * row[2];

        MUL16(b0, W1, row[1]);
        MAC16(b0, W3, row[3]);
        MUL16(b1, W3, row[1]);
        MAC16(b1, -W7, row[3]);
        MUL16(b2, W5, row[1]);
        MAC16(b2, -W1, row[3]);
        MUL16(b3, W7, row[1]);
        MAC16(b3, -W5, row[3]);

        if (row[4] != 0 || row[5] != 0 || row[6] != 0 || row[7] != 0)
        {
            a0 += W4 * row[4] + W6 * row[6];
            a1 += -W4 * row[4] - W2 * row[6];
            a2 += -W4 * row[4] + W2 * row[6];
            a3 += W4 * row[4] - W6 * row[6];

            MAC16(b0, W5, row[5]);
            MAC16(b0, W7, row[7]);

            MAC16(b1, -W1, row[5]);
            MAC16(b1, -W5, row[7]);

            MAC16(b2, W7, row[5]);
            MAC16(b2, W3, row[7]);

            MAC16(b3, W3, row[5]);
            MAC16(b3, -W1, row[7]);
        }

        row[0] = (int16_t)((a0 + b0) >> ROW_SHIFT);
        row[7] = (int16_t)((a0 - b0) >> ROW_SHIFT);
        row[1] = (int16_t)((a1 + b1) >> ROW_SHIFT);
        row[6] = (int16_t)((a1 - b1) >> ROW_SHIFT);
        row[2] = (int16_t)((a2 + b2) >> ROW_SHIFT);
        row[5] = (int16_t)((a2 - b2) >> ROW_SHIFT);
        row[3] = (int16_t)((a3 + b3) >> ROW_SHIFT);
        row[4] = (int16_t)((a3 - b3) >> ROW_SHIFT);
    }


    void IdctSparseCol(int16_t* col)
    {
        int a0, a1, a2, a3, b0, b1, b2, b3;

        a0 = W4 * (col[8 * 0] + ((1 << (COL_SHIFT - 1)) / W4));
        a1 = a0;
        a2 = a0;
        a3 = a0;

        a0 += +W2 * col[8 * 2];
        a1 += +W6 * col[8 * 2];
        a2 += -W6 * col[8 * 2];
        a3 += -W2 * col[8 * 2];

        MUL16(b0, W1, col[8 * 1]);
        MUL16(b1, W3, col[8 * 1]);
        MUL16(b2, W5, col[8 * 1]);
        MUL16(b3, W7, col[8 * 1]);

        MAC16(b0, +W3, col[8 * 3]);
        MAC16(b1, -W7, col[8 * 3]);
        MAC16(b2, -W1, col[8 * 3]);
        MAC16(b3, -W5, col[8 * 3]);

        if (col[8 * 4])
        {
            a0 += +W4 * col[8 * 4];
            a1 += -W4 * col[8 * 4];
            a2 += -W4 * col[8 * 4];
            a3 += +W4 * col[8 * 4];
        }

        if (col[8 * 5])
        {
            MAC16(b0, +W5, col[8 * 5]);
            MAC16(b1, -W1, col[8 * 5]);
            MAC16(b2, +W7, col[8 * 5]);
            MAC16(b3, +W3, col[8 * 5]);
        }

        if (col[8 * 6])
        {
            a0 += +W6 * col[8 * 6];
            a1 += -W2 * col[8 * 6];
            a2 += +W2 * col[8 * 6];
            a3 += -W6 * col[8 * 6];
        }

        if (col[8 * 7])
        {
            MAC16(b0, +W7, col[8 * 7]);
            MAC16(b1, -W5, col[8 * 7]);
            MAC16(b2, +W3, col[8 * 7]);
            MAC16(b3, -W1, col[8 * 7]);
        }

        col[0] = (int16_t)((a0 + b0) >> COL_SHIFT);
        col[8] = (int16_t)((a1 + b1) >> COL_SHIFT);
        col[16] = (int16_t)((a2 + b2) >> COL_SHIFT);
        col[24] = (int16_t)((a3 + b3) >> COL_SHIFT);
        col[32] = (int16_t)((a3 - b3) >> COL_SHIFT);
        col[40] = (int16_t)((a2 - b2) >> COL_SHIFT);
        col[48] = (int16_t)((a1 - b1) >> COL_SHIFT);
        col[56] = (int16_t)((a0 - b0) >> COL_SHIFT);
    }


    void IdctBlock(int16_t* data)
    {
        for (int i = 0; i < 8; i++)
        {
            IdctRowCondDC(data + i * 8);
        }

        for (int i = 0; i < 8; i++)
        {
            IdctSparseCol(data + i);
        }
    }


    void UnpackPixelPair(const int16_t* y, const uint16_t* color, uint8_t* output)
    {
        uint8_t* outputPtr = output;

        for (int i = 0; i < 2; i++)
        {
            *outputPtr++ = SaturateCast<uint8_t>((int16_t)(y[i] + 128 + color[2])); // R
            *outputPtr++ = SaturateCast<uint8_t>((int16_t)(y[i] + 128 + color[1])); // G
            *outputPtr++ = SaturateCast<uint8_t>((int16_t)(y[i] + 128 + color[0])); // B
            // *outputPtr++ = SaturateCast<uint8_t>((int16_t)(y[i] + 128 + color[3]));
        }
    }


    void UnpackBlock8x8(const std::array<int16_t, 384>& input, int yIndex, int cIndex, uint8_t* output, int stride)
    {
        uint8_t* outputPtr = output;

        for (int i = 0; i < 4; i++)
        {
            std::array<uint16_t, 4> v5{};
            std::array<uint16_t, 4> v6{};
            std::array<uint16_t, 4> v7{};

            for (int j = 0; j < 4; j++)
            {
                uint16_t v1, v2, v3, v4;

                v1 = input[cIndex + j];
                v2 = input[cIndex + j + 64];
                v3 = (uint16_t)((uint32_t)((int16_t)0x5810 * (int16_t)v1) >> 16);
                v4 = (uint16_t)((uint32_t)((int16_t)0x5B60 * (int16_t)v2) >> 16);
                v5[j] = (v3 + (v4 << 1)) * 0xFFFF;
                v6[j] = (uint16_t)((uint32_t)((int16_t)0x7164 * (int16_t)(v1 << 2)) >> 16);
                v7[j] = (uint16_t)((uint32_t)((int16_t)0x59BC * (int16_t)(v2 << 2)) >> 16);
            }

            uint16_t color[4][4]
                    {
                            {v6[0], v5[0], v7[0], v7[1]},
                            {v6[1], v5[1], v7[1], v7[2]},
                            {v6[2], v5[2], v7[2], v7[3]},
                            {v6[3], v5[3], v7[3], 0}
                    };

            for (int iy = 0, ic = 0, io = 0; ic < 4; iy += 2, ic += 1, io += 6)
            {
                UnpackPixelPair(&input[yIndex + iy], color[ic], &outputPtr[io]);
            }

            for (int iy = 8, ic = 0, io = 0; ic < 4; iy += 2, ic += 1, io += 6)
            {
                UnpackPixelPair(&input[yIndex + iy], color[ic], &outputPtr[stride + io]);
            }

            outputPtr += stride * 2;
            cIndex += 8;
            yIndex += 16;
        }
    }


    void UnpackBlock16x16(const std::array<int16_t, 384>& input, uint8_t* output, int stride)
    {
        const int Y0 = 0;
        const int Y1 = 64;
        const int Y2 = 128;
        const int Y3 = 192;

        const int C0 = 256;
        const int C1 = 260;
        const int C2 = 288;
        const int C3 = 292;

        const int O0 = 0;
        const int O1 = 24;
        const int O2 = 8 * stride;
        const int O3 = 8 * stride + 24;

        UnpackBlock8x8(input, Y0, C0, output + O0, stride);
        UnpackBlock8x8(input, Y1, C1, output + O1, stride);
        UnpackBlock8x8(input, Y2, C2, output + O2, stride);
        UnpackBlock8x8(input, Y3, C3, output + O3, stride);
    }


    static std::array<int, 1024> HuffmanTreeNodes;


    int CreateHuffmanTree(std::array<int, 32>& freq, int deep)
    {
        constexpr int MAX_FREQ = 2100000000;

        for (int i = 0; i < 258; i++)
        {
            HuffmanTreeNodes[i] = 0;
            HuffmanTreeNodes[i + 512] = 0;
        }

        for (;;)
        {
            int left = -100;
            int right = -100;

            for (int i = 0, v = MAX_FREQ - 1; i < deep; i++)
            {
                if (freq[i] < v)
                {
                    left = i;
                    v = freq[i];
                }
            }

            for (int i = 0, v = MAX_FREQ - 1; i < deep; i++)
            {
                if (i != left)
                {
                    if (freq[i] < v)
                    {
                        right = i;
                        v = freq[i];
                    }
                }
            }

            if (left < 0 || right < 0)
            {
                break;
            }

            HuffmanTreeNodes[deep] = left;
            HuffmanTreeNodes[deep + 512] = right;

            freq[deep++] = freq[left] + freq[right];
            freq[left] = MAX_FREQ;
            freq[right] = MAX_FREQ;
        }

        return deep;
    }


    static uint8_t* BitStreamData;
    static int BitStreamRead;


    int BitStreamReadOneBit()
    {
        int read = BitStreamRead;
        uint32_t bits = *(uint32_t*)BitStreamData >> read;
        int count = read + 1;
        int j = count >> 5;
        BitStreamData += 4 * j;
        BitStreamRead = count & 31;
        return (int)(bits & 1);
    }


    int BitStreamReadBits(int count)
    {
        int val = 0;

        for (int i = 0; i < count; i++)
        {
            val <<= 1;
            val |= BitStreamReadOneBit();
        }

        return val;
    }


    int HuffmanTreeReadSymbol(int root, int deep)
    {
        int val = root - 1;

        while (val >= deep)
        {
            int j = 512 * BitStreamReadOneBit();
            val = HuffmanTreeNodes[j + val];
        }

        return val;
    }


    static int16_t ZigzagOrder[] = {
            0, 1, 8, 16, 9, 2, 3, 10,
            17, 24, 32, 25, 18, 11, 4, 5,
            12, 19, 26, 33, 40, 48, 41, 34,
            27, 20, 13, 6, 7, 14, 21, 28,
            35, 42, 49, 56, 57, 50, 43, 36,
            29, 22, 15, 23, 30, 37, 44, 51,
            58, 59, 52, 45, 38, 31, 39, 46,
            53, 60, 61, 54, 47, 55, 62, 63,
    };


    void DecodeJbpd(uint8_t* data)
    {
        int dataOffset = *(int*)(data + 4);
        int format = *(int*)(data + 8);
        int width = *(uint16_t*)(data + 16);
        int height = *(uint16_t*)(data + 18);
        int dcTermSize = *(int*)(data + 28);

        uint8_t* readPtr = data + dataOffset + 4;


        std::array<int, 32> dcFreqTable{};
        for (int i = 0; i < 16; i++)
        {
            dcFreqTable[i] = *(int*)readPtr;
            readPtr += 4;
        }

        std::array<int, 32> acFreqTable{};
        for (int i = 0; i < 16; i++)
        {
            acFreqTable[i] = *(int*)readPtr;
            readPtr += 4;
        }

        std::array<int, 16> treeData{};
        for (int i = 0; i < 16; i++)
        {
            treeData[i] = *readPtr + 1;
            readPtr++;
        }

        std::array<int16_t, 192> quantTable{};
        if (format & 0x8000000)
        {
            int j = 0;
            for (int16_t i : ZigzagOrder)
            {
                quantTable[j++] = *(readPtr + i);
            }
            readPtr += 64;
            for (int16_t i : ZigzagOrder)
            {
                quantTable[j++] = *(readPtr + i);
            }
            readPtr += 64;
        }

        uint8_t* bigBlockInfoPtr = readPtr;

        int xBigBlockCount = *(readPtr + 1);
        int yBigBlockCount = *(readPtr + 0);
        readPtr += 2;

        int bigBlockCount = *(uint16_t*)readPtr;
        readPtr += 2;

        struct BigBlockInfo
        {
            int xMidBlockCount;
            int yMidBlockCount;
        };
        std::vector<BigBlockInfo> bigBlockInfo;
        bigBlockInfo.reserve(bigBlockCount);
        for (int i = 0; i < bigBlockCount; i++)
        {
            BigBlockInfo info{};
            info.xMidBlockCount = *(readPtr + 1);
            info.yMidBlockCount = *(readPtr + 0);
            readPtr += 2;
            bigBlockInfo.push_back(info);
        }

        int bigBlockInfoSize = (2 * bigBlockCount + 7) & ~3;
        readPtr = bigBlockInfoPtr + bigBlockInfoSize;

        int alignedWidth;
        int alignedHeight;

        int alignment = (format >> 28) & 3;
        switch (alignment)
        {
            case 0:
                alignedWidth = (width + 7) & ~7;
                alignedHeight = (height + 7) & ~7;
                break;
            case 1:
                alignedWidth = (width + 15) & ~15;
                alignedHeight = (height + 15) & ~15;
                break;
            case 2:
                alignedWidth = (width + 31) & ~31;
                alignedHeight = (height + 15) & ~15;
                break;
            default:
                return;
        }

        int xMidBlockCount = alignedWidth >> 4;
        int yMidBlockCount = alignedHeight >> 4;

        if (format & 0x1000)
        {
            readPtr += (((xMidBlockCount * yMidBlockCount + 7) >> 3) + 3) & ~3;
        }

        /* decompress dc terms */

        BitStreamData = readPtr;
        BitStreamRead = 0;
        readPtr += dcTermSize;
        int dcTermTree = CreateHuffmanTree(dcFreqTable, 16);

        int dcTermCount = xMidBlockCount * yMidBlockCount * 6;
        std::vector<int16_t> dcTerms;
        dcTerms.reserve(dcTermCount);

        for (int i = 0; i < dcTermCount; i++)
        {
            int val = 0;
            int c = HuffmanTreeReadSymbol(dcTermTree, 16);

            if (c > 0)
            {
                val = BitStreamReadBits(c);
            }

            if (val < 1 << (c - 1))
            {
                val = val - (1 << c) + 1;
            }

            if (i)
            {
                val += dcTerms[i - 1];
            }

            dcTerms.push_back((int16_t)(val));
        }

        /* unpack all the blocks of image */

        std::vector<uint8_t> buffer;
        buffer.resize(xMidBlockCount * yMidBlockCount * 16 * 16 * 3);
        int imageWidth = xBigBlockCount * 256;

        BitStreamData = readPtr;
        BitStreamRead = 0;
        int acTermTree = CreateHuffmanTree(acFreqTable, 16);
        int dcTermIndex = 0;

        for (int iy = 0; iy < yBigBlockCount; iy++)
        {
            for (int ix = 0; ix < xBigBlockCount; ix++)
            {
                uint8_t* bigPtr = buffer.data() + (iy * imageWidth + ix) * 256 * 3;

                int xMidBlockCount2 = bigBlockInfo[iy * xBigBlockCount + ix].xMidBlockCount;
                int yMidBlockCount2 = bigBlockInfo[iy * xBigBlockCount + ix].yMidBlockCount;

                for (int y = 0; y < yMidBlockCount2; y++)
                {
                    for (int x = 0; x < xMidBlockCount2; x++)
                    {
                        uint8_t* midPtr = bigPtr + (y * imageWidth + x) * 16 * 3;

                        std::array<int16_t, 384> dctTable{};

                        for (int j = 0; j < 6; j++)
                        {
                            int q = 64 * (j >> 2);
                            int d = 64 * j;

                            dctTable[d] = (int16_t)(dcTerms[dcTermIndex++] * quantTable[q]);

                            /* decompress ac terms of this block */

                            for (int k = 0; k < 63;)
                            {
                                int c = HuffmanTreeReadSymbol(acTermTree, 16);
                                if (c == 15)
                                {
                                    break;
                                }
                                else if (c)
                                {
                                    int val = BitStreamReadBits(c);

                                    if (val < 1 << (c - 1))
                                    {
                                        val = val - (1 << c) + 1;
                                    }

                                    dctTable[d + ZigzagOrder[k + 1]] = (int16_t)(val * quantTable[q + 1 + k]);

                                    k++;
                                }
                                else
                                {
                                    int nodeIndex = 0;

                                    while (BitStreamReadOneBit() != 0)
                                    {
                                        nodeIndex++;
                                    }

                                    k += treeData[nodeIndex];
                                }
                            }
                        }

                        IdctBlock(&dctTable[0]);    // Y
                        IdctBlock(&dctTable[64]);   // Y
                        IdctBlock(&dctTable[128]);  // Y
                        IdctBlock(&dctTable[192]);  // Y
                        IdctBlock(&dctTable[256]);  // C
                        IdctBlock(&dctTable[320]);  // C

                        UnpackBlock16x16(dctTable, midPtr, imageWidth * 3);
                    }
                }
            }
        }

        /* final */

        FILE* fp = fopen("./out.raw", "wb");
        if (fp)
        {
            fwrite(buffer.data(), buffer.size(), 1, fp);
            fflush(fp);
            fclose(fp);
        }

        stbi_write_png("./out.png", width, height, 3, buffer.data(), imageWidth * 3);
    }
}

int main()
{
    FILE* fp = fopen("./frame_2516.bin", "rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        auto size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        auto data = new uint8_t[size];
        fread(data, size, 1, fp);
        fclose(fp);

        CMV::DecodeJbpd(data);
        delete[] data;
    }

    return 0;
}
