// dllmain.cpp

#include "util.h"

typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;

#undef min
#undef max

template<typename T1, typename T2>
inline T1 saturate_cast(T2 value)
{
	if (value < std::numeric_limits<T1>::min())
		return std::numeric_limits<T1>::min();
	if (value > std::numeric_limits<T1>::max())
		return std::numeric_limits<T1>::max();
	return static_cast<T1>(value);
}

uint64_t stru_549040[] = {
        0xC000400040004000,
        0xAC6122A322A3539F,
        0x40004000C0004000,
        0xDD5DAC61539FDD5D,
        0xA73B4B42324958C5,
        0xCDB7EE5811A84B42,
        0x4B4211A811A83249,
        0xA73BCDB74B42A73B,
};
uint64_t stru_549080[] = {
        0xA73B58C558C558C5,
        0x8C04300B300B73FC,
        0x58C558C5A73B58C5,
        0xCFF58C0473FCCFF5,
        0x84DF686245BF7B21,
        0xBA41E782187E6862,
        0x6862187E187E45BF,
        0x84DFBA41686284DF,
};
uint64_t stru_5490C0[] = {
        0xAC61539F539F539F,
        0x92BF2D412D416D41,
        0x539F539FAC61539F,
        0xD2BF92BF6D41D2BF,
        0x8C04625441B373FC,
        0xBE4DE8EE17126254,
        0x62541712171241B3,
        0x8C04BE4D62548C04,
};
uint64_t stru_549100[] = {
        0xB4BE4B424B424B42,
        0x9DAC28BA28BA6254,
        0x4B424B42B4BE4B42,
        0xD7469DAC6254D746,
        0x979E587E3B216862,
        0xC4DFEB3D14C3587E,
        0x587E14C314C33B21,
        0x979EC4DF587E979E,
};

void pass_row(int16_t* data, uint64_t* constant)
{
    int16_t* c = (int16_t*)constant;

    i16 tmp0 = data[0];
    i16 tmp1 = data[2];
    i16 tmp2 = data[4];
    i16 tmp3 = data[6];
    i16 tmp4 = data[1];
    i16 tmp5 = data[3];
    i16 tmp6 = data[5];
    i16 tmp7 = data[7];

    i32 v8x0 = ((c[8] * tmp0 + c[9] * tmp2) + (c[12] * tmp1 + c[13] * tmp3)) + 0x400;
    i32 v8x1 = ((c[10] * tmp0 + c[11] * tmp2) + (c[14] * tmp1 + c[15] * tmp3)) + 0x400;
    i32 v9x0 = ((c[0] * tmp0 + c[1] * tmp2) + (c[4] * tmp1 + c[5] * tmp3)) + 0x400;
    i32 v9x1 = ((c[2] * tmp0 + c[3] * tmp2) + (c[6] * tmp1 + c[7] * tmp3)) + 0x400;

    i32 v10x0 = (c[24] * tmp4 + c[25] * tmp6) + (c[28] * tmp5 + c[29] * tmp7);
    i32 v10x1 = (c[26] * tmp4 + c[27] * tmp6) + (c[30] * tmp5 + c[31] * tmp7);
    i32 v11x0 = (c[16] * tmp4 + c[17] * tmp6) + (c[20] * tmp5 + c[21] * tmp7);
    i32 v11x1 = (c[18] * tmp4 + c[19] * tmp6) + (c[22] * tmp5 + c[23] * tmp7);

    data[0] = SaturateCast<i16>((v9x0 + v11x0) >> 11);
    data[7] = SaturateCast<i16>((v9x0 - v11x0) >> 11);
    data[1] = SaturateCast<i16>((v9x1 + v11x1) >> 11);
    data[6] = SaturateCast<i16>((v9x1 - v11x1) >> 11);
    data[2] = SaturateCast<i16>((v8x0 + v10x0) >> 11);
    data[5] = SaturateCast<i16>((v8x0 - v10x0) >> 11);
    data[3] = SaturateCast<i16>((v8x1 + v10x1) >> 11);
    data[4] = SaturateCast<i16>((v8x1 - v10x1) >> 11);
}

void pass_col(int16_t* data)
{
    i16 m1x0 = data[0];
    i16 m1x1 = data[1];
    i16 m1x2 = data[2];
    i16 m1x3 = data[3];

    i16 m2x0 = data[8];
    i16 m2x1 = data[9];
    i16 m2x2 = data[10];
    i16 m2x3 = data[11];

    i16 m3x0 = data[16];
    i16 m3x1 = data[17];
    i16 m3x2 = data[18];
    i16 m3x3 = data[19];

    i16 m4x0 = data[24];
    i16 m4x1 = data[25];
    i16 m4x2 = data[26];
    i16 m4x3 = data[27];

    i16 m5x0 = data[32];
    i16 m5x1 = data[33];
    i16 m5x2 = data[34];
    i16 m5x3 = data[35];

    i16 m6x0 = data[40];
    i16 m6x1 = data[41];
    i16 m6x2 = data[42];
    i16 m6x3 = data[43];

    i16 m7x0 = data[48];
    i16 m7x1 = data[49];
    i16 m7x2 = data[50];
    i16 m7x3 = data[51];

    i16 m8x0 = data[56];
    i16 m8x1 = data[57];
    i16 m8x2 = data[58];
    i16 m8x3 = data[59];

    /* 111 */

    i16 z1x0 = (i16)((u32)((i16)m6x0 * (i16)0xAB0E) >> 16);
    i16 z1x1 = (i16)((u32)((i16)m6x1 * (i16)0xAB0E) >> 16);
    i16 z1x2 = (i16)((u32)((i16)m6x2 * (i16)0xAB0E) >> 16);
    i16 z1x3 = (i16)((u32)((i16)m6x3 * (i16)0xAB0E) >> 16);

    i16 v1x0 = (z1x0 + m6x0) + m4x0;
    i16 v1x1 = (z1x1 + m6x1) + m4x1;
    i16 v1x2 = (z1x2 + m6x2) + m4x2;
    i16 v1x3 = (z1x3 + m6x3) + m4x3;

    /* 111 */

    i16 z2x0 = (i16)((u32)((i16)m4x0 * (i16)0xAB0E) >> 16);
    i16 z2x1 = (i16)((u32)((i16)m4x1 * (i16)0xAB0E) >> 16);
    i16 z2x2 = (i16)((u32)((i16)m4x2 * (i16)0xAB0E) >> 16);
    i16 z2x3 = (i16)((u32)((i16)m4x3 * (i16)0xAB0E) >> 16);

    i16 v2x0 = m6x0 - (z2x0 + m4x0);
    i16 v2x1 = m6x1 - (z2x1 + m4x1);
    i16 v2x2 = m6x2 - (z2x2 + m4x2);
    i16 v2x3 = m6x3 - (z2x3 + m4x3);

    /* 111 */

    i16 z3x0 = (i16)((u32)((i16)m8x0 * (i16)0x32EC) >> 16);
    i16 z3x1 = (i16)((u32)((i16)m8x1 * (i16)0x32EC) >> 16);
    i16 z3x2 = (i16)((u32)((i16)m8x2 * (i16)0x32EC) >> 16);
    i16 z3x3 = (i16)((u32)((i16)m8x3 * (i16)0x32EC) >> 16);

    i16 v3x0 = z3x0 + m2x0;
    i16 v3x1 = z3x1 + m2x1;
    i16 v3x2 = z3x2 + m2x2;
    i16 v3x3 = z3x3 + m2x3;

    /* 111 */

    i16 z4x0 = (i16)((u32)((i16)m7x0 * (i16)0x6A0A) >> 16);
    i16 z4x1 = (i16)((u32)((i16)m7x1 * (i16)0x6A0A) >> 16);
    i16 z4x2 = (i16)((u32)((i16)m7x2 * (i16)0x6A0A) >> 16);
    i16 z4x3 = (i16)((u32)((i16)m7x3 * (i16)0x6A0A) >> 16);

    i16 v4x0 = z4x0 + m3x0;
    i16 v4x1 = z4x1 + m3x1;
    i16 v4x2 = z4x2 + m3x2;
    i16 v4x3 = z4x3 + m3x3;

    /* 111 */

    i16 z5x0 = (i16)((u32)((i16)m2x0 * (i16)0x32EC) >> 16);
    i16 z5x1 = (i16)((u32)((i16)m2x1 * (i16)0x32EC) >> 16);
    i16 z5x2 = (i16)((u32)((i16)m2x2 * (i16)0x32EC) >> 16);
    i16 z5x3 = (i16)((u32)((i16)m2x3 * (i16)0x32EC) >> 16);

    i16 v5x0 = z5x0 - m8x0;
    i16 v5x1 = z5x1 - m8x1;
    i16 v5x2 = z5x2 - m8x2;
    i16 v5x3 = z5x3 - m8x3;

    /* 111 */

    i16 z6x0 = (i16)((u32)((i16)m3x0 * (i16)0x6A0A) >> 16);
    i16 z6x1 = (i16)((u32)((i16)m3x1 * (i16)0x6A0A) >> 16);
    i16 z6x2 = (i16)((u32)((i16)m3x2 * (i16)0x6A0A) >> 16);
    i16 z6x3 = (i16)((u32)((i16)m3x3 * (i16)0x6A0A) >> 16);

    i16 v6x0 = z6x0 - m7x0;
    i16 v6x1 = z6x1 - m7x1;
    i16 v6x2 = z6x2 - m7x2;
    i16 v6x3 = z6x3 - m7x3;

    /* 111 */

    i16 v8x0 = (v5x0 - v2x0) + 1;
    i16 v8x1 = (v5x1 - v2x1) + 1;
    i16 v8x2 = (v5x2 - v2x2) + 1;
    i16 v8x3 = (v5x3 - v2x3) + 1;

    i16 v10x0 = (v1x0 + v3x0) + 1;
    i16 v10x1 = (v1x1 + v3x1) + 1;
    i16 v10x2 = (v1x2 + v3x2) + 1;
    i16 v10x3 = (v1x3 + v3x3) + 1;

    /* 111 */

    i16 v11x0 = v3x0 - v1x0;
    i16 v11x1 = v3x1 - v1x1;
    i16 v11x2 = v3x2 - v1x2;
    i16 v11x3 = v3x3 - v1x3;

    i16 v12x0 = v5x0 + v2x0;
    i16 v12x1 = v5x1 + v2x1;
    i16 v12x2 = v5x2 + v2x2;
    i16 v12x3 = v5x3 + v2x3;

    /* 111 */

    i16 v13x0 = v11x0 + v8x0;
    i16 v13x1 = v11x1 + v8x1;
    i16 v13x2 = v11x2 + v8x2;
    i16 v13x3 = v11x3 + v8x3;

    i16 z7x0 = (i16)((u32)((i16)0xB505 * (i16)v13x0) >> 16);
    i16 z7x1 = (i16)((u32)((i16)0xB505 * (i16)v13x1) >> 16);
    i16 z7x2 = (i16)((u32)((i16)0xB505 * (i16)v13x2) >> 16);
    i16 z7x3 = (i16)((u32)((i16)0xB505 * (i16)v13x3) >> 16);

    i16 v14x0 = (z7x0 + v13x0) | 1;
    i16 v14x1 = (z7x1 + v13x1) | 1;
    i16 v14x2 = (z7x2 + v13x2) | 1;
    i16 v14x3 = (z7x3 + v13x3) | 1;

    /* 111 */

    i16 v15x0 = v11x0 - v8x0;
    i16 v15x1 = v11x1 - v8x1;
    i16 v15x2 = v11x2 - v8x2;
    i16 v15x3 = v11x3 - v8x3;

    i16 z8x0 = (i16)((u32)((i16)0xB505 * (i16)v15x0) >> 16);
    i16 z8x1 = (i16)((u32)((i16)0xB505 * (i16)v15x1) >> 16);
    i16 z8x2 = (i16)((u32)((i16)0xB505 * (i16)v15x2) >> 16);
    i16 z8x3 = (i16)((u32)((i16)0xB505 * (i16)v15x3) >> 16);

    i16 v16x0 = (z8x0 + v15x0) | 1;
    i16 v16x1 = (z8x1 + v15x1) | 1;
    i16 v16x2 = (z8x2 + v15x2) | 1;
    i16 v16x3 = (z8x3 + v15x3) | 1;

    /* 111 */

    i16 v17x0 = m5x0 + m1x0;
    i16 v17x1 = m5x1 + m1x1;
    i16 v17x2 = m5x2 + m1x2;
    i16 v17x3 = m5x3 + m1x3;

    i16 v18x0 = (v17x0 + v4x0) + 0x20;
    i16 v18x1 = (v17x1 + v4x1) + 0x20;
    i16 v18x2 = (v17x2 + v4x2) + 0x20;
    i16 v18x3 = (v17x3 + v4x3) + 0x20;

    i16 v19x0 = (v17x0 - v4x0) + 0x1F;
    i16 v19x1 = (v17x1 - v4x1) + 0x1F;
    i16 v19x2 = (v17x2 - v4x2) + 0x1F;
    i16 v19x3 = (v17x3 - v4x3) + 0x1F;

    /* 111 */

    i16 v20x0 = m1x0 - m5x0;
    i16 v20x1 = m1x1 - m5x1;
    i16 v20x2 = m1x2 - m5x2;
    i16 v20x3 = m1x3 - m5x3;

    i16 v21x0 = (v20x0 + v6x0) + 0x20;
    i16 v21x1 = (v20x1 + v6x1) + 0x20;
    i16 v21x2 = (v20x2 + v6x2) + 0x20;
    i16 v21x3 = (v20x3 + v6x3) + 0x20;

    i16 v22x0 = (v20x0 - v6x0) + 0x1F;
    i16 v22x1 = (v20x1 - v6x1) + 0x1F;
    i16 v22x2 = (v20x2 - v6x2) + 0x1F;
    i16 v22x3 = (v20x3 - v6x3) + 0x1F;

    /* 111 */

    data[0] = (v18x0 + v10x0) >> 6;
    data[1] = (v18x1 + v10x1) >> 6;
    data[2] = (v18x2 + v10x2) >> 6;
    data[3] = (v18x3 + v10x3) >> 6;
    data[56] = (v18x0 - v10x0) >> 6;
    data[57] = (v18x1 - v10x1) >> 6;
    data[58] = (v18x2 - v10x2) >> 6;
    data[59] = (v18x3 - v10x3) >> 6;
    data[8] = (v21x0 + v14x0) >> 6;
    data[9] = (v21x1 + v14x1) >> 6;
    data[10] = (v21x2 + v14x2) >> 6;
    data[11] = (v21x3 + v14x3) >> 6;
    data[48] = (v21x0 - v14x0) >> 6;
    data[49] = (v21x1 - v14x1) >> 6;
    data[50] = (v21x2 - v14x2) >> 6;
    data[51] = (v21x3 - v14x3) >> 6;
    data[16] = (v22x0 + v16x0) >> 6;
    data[17] = (v22x1 + v16x1) >> 6;
    data[18] = (v22x2 + v16x2) >> 6;
    data[19] = (v22x3 + v16x3) >> 6;
    data[40] = (v22x0 - v16x0) >> 6;
    data[41] = (v22x1 - v16x1) >> 6;
    data[42] = (v22x2 - v16x2) >> 6;
    data[43] = (v22x3 - v16x3) >> 6;
    data[24] = (v19x0 + v12x0) >> 6;
    data[25] = (v19x1 + v12x1) >> 6;
    data[26] = (v19x2 + v12x2) >> 6;
    data[27] = (v19x3 + v12x3) >> 6;
    data[32] = (v19x0 - v12x0) >> 6;
    data[33] = (v19x1 - v12x1) >> 6;
    data[34] = (v19x2 - v12x2) >> 6;
    data[35] = (v19x3 - v12x3) >> 6;
}

void cmv_idct(int16_t* data)
{
    pass_row(data + 0, stru_549040);
    pass_row(data + 32, stru_549040);
    pass_row(data + 8, stru_549080);
    pass_row(data + 56, stru_549080);
    pass_row(data + 16, stru_5490C0);
    pass_row(data + 48, stru_5490C0);
    pass_row(data + 24, stru_549100);
    pass_row(data + 40, stru_549100);

    pass_col(data + 0);
    pass_col(data + 4);
}

typedef void(_stdcall* unpack_block_16x16_proc_t)(uint8_t*, int, const uint16_t*);
unpack_block_16x16_proc_t pfn_unpack_block_16x16 = (unpack_block_16x16_proc_t)0x549B30;

void unpack_pixel_pair(const int16_t* y, const uint16_t* color, uint8_t* output)
{
	uint8_t* ptr = output;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			*ptr++ = saturate_cast<uint8_t, int16_t>(y[i] + 0x80 + color[j]);
		}
	}
}

void unpack_block_8x8(const int16_t* input, int i, int j, uint8_t* output, int stride)
{
	uint8_t* output_ptr = output;

	for (int n = 0; n < 4; n++)
	{
		uint16_t temp0_0 = input[j + 0];
		uint16_t temp0_1 = input[j + 1];
		uint16_t temp0_2 = input[j + 2];
		uint16_t temp0_3 = input[j + 3];

		uint16_t temp1_0 = (uint16_t)((uint32_t)((int16_t)0x5810 * (int16_t)temp0_0) >> 16);
		uint16_t temp1_1 = (uint16_t)((uint32_t)((int16_t)0x5810 * (int16_t)temp0_1) >> 16);
		uint16_t temp1_2 = (uint16_t)((uint32_t)((int16_t)0x5810 * (int16_t)temp0_2) >> 16);
		uint16_t temp1_3 = (uint16_t)((uint32_t)((int16_t)0x5810 * (int16_t)temp0_3) >> 16);

		uint16_t v7_0 = input[j + 64];
		uint16_t v7_1 = input[j + 65];
		uint16_t v7_2 = input[j + 66];
		uint16_t v7_3 = input[j + 67];

		uint16_t temp2_0 = (uint16_t)((uint32_t)((int16_t)0x5B60 * (int16_t)v7_0) >> 16);
		uint16_t temp2_1 = (uint16_t)((uint32_t)((int16_t)0x5B60 * (int16_t)v7_1) >> 16);
		uint16_t temp2_2 = (uint16_t)((uint32_t)((int16_t)0x5B60 * (int16_t)v7_2) >> 16);
		uint16_t temp2_3 = (uint16_t)((uint32_t)((int16_t)0x5B60 * (int16_t)v7_3) >> 16);

		uint16_t v8_0 = (temp1_0 + (temp2_0 << 1)) * 0xFFFF;
		uint16_t v8_1 = (temp1_1 + (temp2_1 << 1)) * 0xFFFF;
		uint16_t v8_2 = (temp1_2 + (temp2_2 << 1)) * 0xFFFF;
		uint16_t v8_3 = (temp1_3 + (temp2_3 << 1)) * 0xFFFF;

		uint16_t v9_0 = (uint16_t)((uint32_t)((int16_t)0x7164 * (int16_t)(temp0_0 << 2)) >> 16);
		uint16_t v9_1 = (uint16_t)((uint32_t)((int16_t)0x7164 * (int16_t)(temp0_1 << 2)) >> 16);
		uint16_t v9_2 = (uint16_t)((uint32_t)((int16_t)0x7164 * (int16_t)(temp0_2 << 2)) >> 16);
		uint16_t v9_3 = (uint16_t)((uint32_t)((int16_t)0x7164 * (int16_t)(temp0_3 << 2)) >> 16);

		uint16_t v10_0 = (uint16_t)((uint32_t)((int16_t)0x59BC * (int16_t)(v7_0 << 2)) >> 16);
		uint16_t v10_1 = (uint16_t)((uint32_t)((int16_t)0x59BC * (int16_t)(v7_1 << 2)) >> 16);
		uint16_t v10_2 = (uint16_t)((uint32_t)((int16_t)0x59BC * (int16_t)(v7_2 << 2)) >> 16);
		uint16_t v10_3 = (uint16_t)((uint32_t)((int16_t)0x59BC * (int16_t)(v7_3 << 2)) >> 16);

		uint16_t color[4][4]
		{
			{ v9_0, v8_0, v10_0, v10_1 },
			{ v9_1, v8_1, v10_1, v10_2 },
			{ v9_2, v8_2, v10_2, v10_3 },
			{ v9_3, v8_3, v10_3, 0 }
		};

		for (int a = 0, b = 0, c = 0; b < 4; a += 2, b += 1, c += 8)
		{
			unpack_pixel_pair(&input[i + a], color[b], &output_ptr[c]);
		}

		for (int a = 8, b = 0, c = 0; b < 4; a += 2, b += 1, c += 8)
		{
			unpack_pixel_pair(&input[i + a], color[b], &output_ptr[stride + c]);
		}

		output_ptr += stride * 2;
		j += 8;
		i += 16;
	}
}

void _stdcall unpack_block_16x16(uint8_t* output, int stride, const int16_t* input)
{
	int i, j, x;

	x = 0;
	i = 0; // y
	j = 256; // cbcr
	unpack_block_8x8(input, i, j, &output[x], stride);

	x = 32;
	i = 64; // y
	j = 260; // cbcr
	unpack_block_8x8(input, i, j, &output[x], stride);

	x = 8 * stride;
	i = 128; // y
	j = 288; // cbcr
	unpack_block_8x8(input, i, j, &output[x], stride);

	x = 8 * stride + 32;
	i = 192; // y
	j = 292; // cbcr
	unpack_block_8x8(input, i, j, &output[x], stride);
}

//=============================================================================
// Startup Code
//=============================================================================


void InstallPatches()
{
}


void InstallHooks()
{
	InlineHook(pfn_unpack_block_16x16, unpack_block_16x16);
}


//=============================================================================
// DLL Entry Point
//=============================================================================


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			// See https://github.com/microsoft/Detours/wiki/DetourRestoreAfterWith
			DetourRestoreAfterWith();

			// LogInit(L"dev.log");

			InstallPatches();
			InstallHooks();

			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}


//=============================================================================
// Dummy Export Symbol
//=============================================================================


BOOL APIENTRY CreateObject()
{
	return TRUE;
}
