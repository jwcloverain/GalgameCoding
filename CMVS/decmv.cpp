// dllmain.cpp

#include "util.h"


typedef void(_stdcall* unpack_block_16x16_proc_t)(uint8_t*, int, const uint16_t*);
unpack_block_16x16_proc_t pfn_unpack_block_16x16 = (unpack_block_16x16_proc_t)0x549B30;


#undef min
#undef max

template<typename T1, typename T2>
inline T1 saturate_cast(T2 value)
{
	if (value < 0)
		return 0;
	else if (value > std::numeric_limits<T1>::max())
		return std::numeric_limits<T1>::max();
	return static_cast<T1>(value);
}

void unpack_pixel_pair(const uint16_t* y, const uint16_t* color, uint8_t* output)
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

void unpack_block_8x8(const uint16_t* input, int i, int j, uint8_t* output, int stride)
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

void _stdcall unpack_block_16x16(uint8_t* output, int stride, const uint16_t* input)
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
