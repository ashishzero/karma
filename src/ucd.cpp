#include "ucd.h"

Ucd_Script ucd_script(uint32_t codepoint) {
	if (codepoint >= 0x0000 && codepoint <= 0x001F) return Ucd_Script_COMMON;
	if (codepoint == 0x0020) return Ucd_Script_COMMON;
	if (codepoint >= 0x0021 && codepoint <= 0x0023) return Ucd_Script_COMMON;
	if (codepoint == 0x0024) return Ucd_Script_COMMON;
	if (codepoint >= 0x0025 && codepoint <= 0x0027) return Ucd_Script_COMMON;
	if (codepoint == 0x0028) return Ucd_Script_COMMON;
	if (codepoint == 0x0029) return Ucd_Script_COMMON;
	if (codepoint == 0x002A) return Ucd_Script_COMMON;
	if (codepoint == 0x002B) return Ucd_Script_COMMON;
	if (codepoint == 0x002C) return Ucd_Script_COMMON;
	if (codepoint == 0x002D) return Ucd_Script_COMMON;
	if (codepoint >= 0x002E && codepoint <= 0x002F) return Ucd_Script_COMMON;
	if (codepoint >= 0x0030 && codepoint <= 0x0039) return Ucd_Script_COMMON;
	if (codepoint >= 0x003A && codepoint <= 0x003B) return Ucd_Script_COMMON;
	if (codepoint >= 0x003C && codepoint <= 0x003E) return Ucd_Script_COMMON;
	if (codepoint >= 0x003F && codepoint <= 0x0040) return Ucd_Script_COMMON;
	if (codepoint == 0x005B) return Ucd_Script_COMMON;
	if (codepoint == 0x005C) return Ucd_Script_COMMON;
	if (codepoint == 0x005D) return Ucd_Script_COMMON;
	if (codepoint == 0x005E) return Ucd_Script_COMMON;
	if (codepoint == 0x005F) return Ucd_Script_COMMON;
	if (codepoint == 0x0060) return Ucd_Script_COMMON;
	if (codepoint == 0x007B) return Ucd_Script_COMMON;
	if (codepoint == 0x007C) return Ucd_Script_COMMON;
	if (codepoint == 0x007D) return Ucd_Script_COMMON;
	if (codepoint == 0x007E) return Ucd_Script_COMMON;
	if (codepoint >= 0x007F && codepoint <= 0x009F) return Ucd_Script_COMMON;
	if (codepoint == 0x00A0) return Ucd_Script_COMMON;
	if (codepoint == 0x00A1) return Ucd_Script_COMMON;
	if (codepoint >= 0x00A2 && codepoint <= 0x00A5) return Ucd_Script_COMMON;
	if (codepoint == 0x00A6) return Ucd_Script_COMMON;
	if (codepoint == 0x00A7) return Ucd_Script_COMMON;
	if (codepoint == 0x00A8) return Ucd_Script_COMMON;
	if (codepoint == 0x00A9) return Ucd_Script_COMMON;
	if (codepoint == 0x00AB) return Ucd_Script_COMMON;
	if (codepoint == 0x00AC) return Ucd_Script_COMMON;
	if (codepoint == 0x00AD) return Ucd_Script_COMMON;
	if (codepoint == 0x00AE) return Ucd_Script_COMMON;
	if (codepoint == 0x00AF) return Ucd_Script_COMMON;
	if (codepoint == 0x00B0) return Ucd_Script_COMMON;
	if (codepoint == 0x00B1) return Ucd_Script_COMMON;
	if (codepoint >= 0x00B2 && codepoint <= 0x00B3) return Ucd_Script_COMMON;
	if (codepoint == 0x00B4) return Ucd_Script_COMMON;
	if (codepoint == 0x00B5) return Ucd_Script_COMMON;
	if (codepoint >= 0x00B6 && codepoint <= 0x00B7) return Ucd_Script_COMMON;
	if (codepoint == 0x00B8) return Ucd_Script_COMMON;
	if (codepoint == 0x00B9) return Ucd_Script_COMMON;
	if (codepoint == 0x00BB) return Ucd_Script_COMMON;
	if (codepoint >= 0x00BC && codepoint <= 0x00BE) return Ucd_Script_COMMON;
	if (codepoint == 0x00BF) return Ucd_Script_COMMON;
	if (codepoint == 0x00D7) return Ucd_Script_COMMON;
	if (codepoint == 0x00F7) return Ucd_Script_COMMON;
	if (codepoint >= 0x02B9 && codepoint <= 0x02C1) return Ucd_Script_COMMON;
	if (codepoint >= 0x02C2 && codepoint <= 0x02C5) return Ucd_Script_COMMON;
	if (codepoint >= 0x02C6 && codepoint <= 0x02D1) return Ucd_Script_COMMON;
	if (codepoint >= 0x02D2 && codepoint <= 0x02DF) return Ucd_Script_COMMON;
	if (codepoint >= 0x02E5 && codepoint <= 0x02E9) return Ucd_Script_COMMON;
	if (codepoint == 0x02EC) return Ucd_Script_COMMON;
	if (codepoint == 0x02ED) return Ucd_Script_COMMON;
	if (codepoint == 0x02EE) return Ucd_Script_COMMON;
	if (codepoint >= 0x02EF && codepoint <= 0x02FF) return Ucd_Script_COMMON;
	if (codepoint == 0x0374) return Ucd_Script_COMMON;
	if (codepoint == 0x037E) return Ucd_Script_COMMON;
	if (codepoint == 0x0385) return Ucd_Script_COMMON;
	if (codepoint == 0x0387) return Ucd_Script_COMMON;
	if (codepoint == 0x0605) return Ucd_Script_COMMON;
	if (codepoint == 0x060C) return Ucd_Script_COMMON;
	if (codepoint == 0x061B) return Ucd_Script_COMMON;
	if (codepoint == 0x061F) return Ucd_Script_COMMON;
	if (codepoint == 0x0640) return Ucd_Script_COMMON;
	if (codepoint == 0x06DD) return Ucd_Script_COMMON;
	if (codepoint == 0x08E2) return Ucd_Script_COMMON;
	if (codepoint >= 0x0964 && codepoint <= 0x0965) return Ucd_Script_COMMON;
	if (codepoint == 0x0E3F) return Ucd_Script_COMMON;
	if (codepoint >= 0x0FD5 && codepoint <= 0x0FD8) return Ucd_Script_COMMON;
	if (codepoint == 0x10FB) return Ucd_Script_COMMON;
	if (codepoint >= 0x16EB && codepoint <= 0x16ED) return Ucd_Script_COMMON;
	if (codepoint >= 0x1735 && codepoint <= 0x1736) return Ucd_Script_COMMON;
	if (codepoint >= 0x1802 && codepoint <= 0x1803) return Ucd_Script_COMMON;
	if (codepoint == 0x1805) return Ucd_Script_COMMON;
	if (codepoint == 0x1CD3) return Ucd_Script_COMMON;
	if (codepoint == 0x1CE1) return Ucd_Script_COMMON;
	if (codepoint >= 0x1CE9 && codepoint <= 0x1CEC) return Ucd_Script_COMMON;
	if (codepoint >= 0x1CEE && codepoint <= 0x1CF3) return Ucd_Script_COMMON;
	if (codepoint >= 0x1CF5 && codepoint <= 0x1CF6) return Ucd_Script_COMMON;
	if (codepoint == 0x1CF7) return Ucd_Script_COMMON;
	if (codepoint == 0x1CFA) return Ucd_Script_COMMON;
	if (codepoint >= 0x2000 && codepoint <= 0x200A) return Ucd_Script_COMMON;
	if (codepoint == 0x200B) return Ucd_Script_COMMON;
	if (codepoint >= 0x200E && codepoint <= 0x200F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2010 && codepoint <= 0x2015) return Ucd_Script_COMMON;
	if (codepoint >= 0x2016 && codepoint <= 0x2017) return Ucd_Script_COMMON;
	if (codepoint == 0x2018) return Ucd_Script_COMMON;
	if (codepoint == 0x2019) return Ucd_Script_COMMON;
	if (codepoint == 0x201A) return Ucd_Script_COMMON;
	if (codepoint >= 0x201B && codepoint <= 0x201C) return Ucd_Script_COMMON;
	if (codepoint == 0x201D) return Ucd_Script_COMMON;
	if (codepoint == 0x201E) return Ucd_Script_COMMON;
	if (codepoint == 0x201F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2020 && codepoint <= 0x2027) return Ucd_Script_COMMON;
	if (codepoint == 0x2028) return Ucd_Script_COMMON;
	if (codepoint == 0x2029) return Ucd_Script_COMMON;
	if (codepoint >= 0x202A && codepoint <= 0x202E) return Ucd_Script_COMMON;
	if (codepoint == 0x202F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2030 && codepoint <= 0x2038) return Ucd_Script_COMMON;
	if (codepoint == 0x2039) return Ucd_Script_COMMON;
	if (codepoint == 0x203A) return Ucd_Script_COMMON;
	if (codepoint >= 0x203B && codepoint <= 0x203E) return Ucd_Script_COMMON;
	if (codepoint >= 0x203F && codepoint <= 0x2040) return Ucd_Script_COMMON;
	if (codepoint >= 0x2041 && codepoint <= 0x2043) return Ucd_Script_COMMON;
	if (codepoint == 0x2044) return Ucd_Script_COMMON;
	if (codepoint == 0x2045) return Ucd_Script_COMMON;
	if (codepoint == 0x2046) return Ucd_Script_COMMON;
	if (codepoint >= 0x2047 && codepoint <= 0x2051) return Ucd_Script_COMMON;
	if (codepoint == 0x2052) return Ucd_Script_COMMON;
	if (codepoint == 0x2053) return Ucd_Script_COMMON;
	if (codepoint == 0x2054) return Ucd_Script_COMMON;
	if (codepoint >= 0x2055 && codepoint <= 0x205E) return Ucd_Script_COMMON;
	if (codepoint == 0x205F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2060 && codepoint <= 0x2064) return Ucd_Script_COMMON;
	if (codepoint >= 0x2066 && codepoint <= 0x206F) return Ucd_Script_COMMON;
	if (codepoint == 0x2070) return Ucd_Script_COMMON;
	if (codepoint >= 0x2074 && codepoint <= 0x2079) return Ucd_Script_COMMON;
	if (codepoint >= 0x207A && codepoint <= 0x207C) return Ucd_Script_COMMON;
	if (codepoint == 0x207D) return Ucd_Script_COMMON;
	if (codepoint == 0x207E) return Ucd_Script_COMMON;
	if (codepoint >= 0x2080 && codepoint <= 0x2089) return Ucd_Script_COMMON;
	if (codepoint >= 0x208A && codepoint <= 0x208C) return Ucd_Script_COMMON;
	if (codepoint == 0x208D) return Ucd_Script_COMMON;
	if (codepoint == 0x208E) return Ucd_Script_COMMON;
	if (codepoint >= 0x20A0 && codepoint <= 0x20BF) return Ucd_Script_COMMON;
	if (codepoint >= 0x2100 && codepoint <= 0x2101) return Ucd_Script_COMMON;
	if (codepoint == 0x2102) return Ucd_Script_COMMON;
	if (codepoint >= 0x2103 && codepoint <= 0x2106) return Ucd_Script_COMMON;
	if (codepoint == 0x2107) return Ucd_Script_COMMON;
	if (codepoint >= 0x2108 && codepoint <= 0x2109) return Ucd_Script_COMMON;
	if (codepoint >= 0x210A && codepoint <= 0x2113) return Ucd_Script_COMMON;
	if (codepoint == 0x2114) return Ucd_Script_COMMON;
	if (codepoint == 0x2115) return Ucd_Script_COMMON;
	if (codepoint >= 0x2116 && codepoint <= 0x2117) return Ucd_Script_COMMON;
	if (codepoint == 0x2118) return Ucd_Script_COMMON;
	if (codepoint >= 0x2119 && codepoint <= 0x211D) return Ucd_Script_COMMON;
	if (codepoint >= 0x211E && codepoint <= 0x2123) return Ucd_Script_COMMON;
	if (codepoint == 0x2124) return Ucd_Script_COMMON;
	if (codepoint == 0x2125) return Ucd_Script_COMMON;
	if (codepoint == 0x2127) return Ucd_Script_COMMON;
	if (codepoint == 0x2128) return Ucd_Script_COMMON;
	if (codepoint == 0x2129) return Ucd_Script_COMMON;
	if (codepoint >= 0x212C && codepoint <= 0x212D) return Ucd_Script_COMMON;
	if (codepoint == 0x212E) return Ucd_Script_COMMON;
	if (codepoint >= 0x212F && codepoint <= 0x2131) return Ucd_Script_COMMON;
	if (codepoint >= 0x2133 && codepoint <= 0x2134) return Ucd_Script_COMMON;
	if (codepoint >= 0x2135 && codepoint <= 0x2138) return Ucd_Script_COMMON;
	if (codepoint == 0x2139) return Ucd_Script_COMMON;
	if (codepoint >= 0x213A && codepoint <= 0x213B) return Ucd_Script_COMMON;
	if (codepoint >= 0x213C && codepoint <= 0x213F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2140 && codepoint <= 0x2144) return Ucd_Script_COMMON;
	if (codepoint >= 0x2145 && codepoint <= 0x2149) return Ucd_Script_COMMON;
	if (codepoint == 0x214A) return Ucd_Script_COMMON;
	if (codepoint == 0x214B) return Ucd_Script_COMMON;
	if (codepoint >= 0x214C && codepoint <= 0x214D) return Ucd_Script_COMMON;
	if (codepoint == 0x214F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2150 && codepoint <= 0x215F) return Ucd_Script_COMMON;
	if (codepoint == 0x2189) return Ucd_Script_COMMON;
	if (codepoint >= 0x218A && codepoint <= 0x218B) return Ucd_Script_COMMON;
	if (codepoint >= 0x2190 && codepoint <= 0x2194) return Ucd_Script_COMMON;
	if (codepoint >= 0x2195 && codepoint <= 0x2199) return Ucd_Script_COMMON;
	if (codepoint >= 0x219A && codepoint <= 0x219B) return Ucd_Script_COMMON;
	if (codepoint >= 0x219C && codepoint <= 0x219F) return Ucd_Script_COMMON;
	if (codepoint == 0x21A0) return Ucd_Script_COMMON;
	if (codepoint >= 0x21A1 && codepoint <= 0x21A2) return Ucd_Script_COMMON;
	if (codepoint == 0x21A3) return Ucd_Script_COMMON;
	if (codepoint >= 0x21A4 && codepoint <= 0x21A5) return Ucd_Script_COMMON;
	if (codepoint == 0x21A6) return Ucd_Script_COMMON;
	if (codepoint >= 0x21A7 && codepoint <= 0x21AD) return Ucd_Script_COMMON;
	if (codepoint == 0x21AE) return Ucd_Script_COMMON;
	if (codepoint >= 0x21AF && codepoint <= 0x21CD) return Ucd_Script_COMMON;
	if (codepoint >= 0x21CE && codepoint <= 0x21CF) return Ucd_Script_COMMON;
	if (codepoint >= 0x21D0 && codepoint <= 0x21D1) return Ucd_Script_COMMON;
	if (codepoint == 0x21D2) return Ucd_Script_COMMON;
	if (codepoint == 0x21D3) return Ucd_Script_COMMON;
	if (codepoint == 0x21D4) return Ucd_Script_COMMON;
	if (codepoint >= 0x21D5 && codepoint <= 0x21F3) return Ucd_Script_COMMON;
	if (codepoint >= 0x21F4 && codepoint <= 0x22FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x2300 && codepoint <= 0x2307) return Ucd_Script_COMMON;
	if (codepoint == 0x2308) return Ucd_Script_COMMON;
	if (codepoint == 0x2309) return Ucd_Script_COMMON;
	if (codepoint == 0x230A) return Ucd_Script_COMMON;
	if (codepoint == 0x230B) return Ucd_Script_COMMON;
	if (codepoint >= 0x230C && codepoint <= 0x231F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2320 && codepoint <= 0x2321) return Ucd_Script_COMMON;
	if (codepoint >= 0x2322 && codepoint <= 0x2328) return Ucd_Script_COMMON;
	if (codepoint == 0x2329) return Ucd_Script_COMMON;
	if (codepoint == 0x232A) return Ucd_Script_COMMON;
	if (codepoint >= 0x232B && codepoint <= 0x237B) return Ucd_Script_COMMON;
	if (codepoint == 0x237C) return Ucd_Script_COMMON;
	if (codepoint >= 0x237D && codepoint <= 0x239A) return Ucd_Script_COMMON;
	if (codepoint >= 0x239B && codepoint <= 0x23B3) return Ucd_Script_COMMON;
	if (codepoint >= 0x23B4 && codepoint <= 0x23DB) return Ucd_Script_COMMON;
	if (codepoint >= 0x23DC && codepoint <= 0x23E1) return Ucd_Script_COMMON;
	if (codepoint >= 0x23E2 && codepoint <= 0x2426) return Ucd_Script_COMMON;
	if (codepoint >= 0x2440 && codepoint <= 0x244A) return Ucd_Script_COMMON;
	if (codepoint >= 0x2460 && codepoint <= 0x249B) return Ucd_Script_COMMON;
	if (codepoint >= 0x249C && codepoint <= 0x24E9) return Ucd_Script_COMMON;
	if (codepoint >= 0x24EA && codepoint <= 0x24FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x2500 && codepoint <= 0x25B6) return Ucd_Script_COMMON;
	if (codepoint == 0x25B7) return Ucd_Script_COMMON;
	if (codepoint >= 0x25B8 && codepoint <= 0x25C0) return Ucd_Script_COMMON;
	if (codepoint == 0x25C1) return Ucd_Script_COMMON;
	if (codepoint >= 0x25C2 && codepoint <= 0x25F7) return Ucd_Script_COMMON;
	if (codepoint >= 0x25F8 && codepoint <= 0x25FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x2600 && codepoint <= 0x266E) return Ucd_Script_COMMON;
	if (codepoint == 0x266F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2670 && codepoint <= 0x2767) return Ucd_Script_COMMON;
	if (codepoint == 0x2768) return Ucd_Script_COMMON;
	if (codepoint == 0x2769) return Ucd_Script_COMMON;
	if (codepoint == 0x276A) return Ucd_Script_COMMON;
	if (codepoint == 0x276B) return Ucd_Script_COMMON;
	if (codepoint == 0x276C) return Ucd_Script_COMMON;
	if (codepoint == 0x276D) return Ucd_Script_COMMON;
	if (codepoint == 0x276E) return Ucd_Script_COMMON;
	if (codepoint == 0x276F) return Ucd_Script_COMMON;
	if (codepoint == 0x2770) return Ucd_Script_COMMON;
	if (codepoint == 0x2771) return Ucd_Script_COMMON;
	if (codepoint == 0x2772) return Ucd_Script_COMMON;
	if (codepoint == 0x2773) return Ucd_Script_COMMON;
	if (codepoint == 0x2774) return Ucd_Script_COMMON;
	if (codepoint == 0x2775) return Ucd_Script_COMMON;
	if (codepoint >= 0x2776 && codepoint <= 0x2793) return Ucd_Script_COMMON;
	if (codepoint >= 0x2794 && codepoint <= 0x27BF) return Ucd_Script_COMMON;
	if (codepoint >= 0x27C0 && codepoint <= 0x27C4) return Ucd_Script_COMMON;
	if (codepoint == 0x27C5) return Ucd_Script_COMMON;
	if (codepoint == 0x27C6) return Ucd_Script_COMMON;
	if (codepoint >= 0x27C7 && codepoint <= 0x27E5) return Ucd_Script_COMMON;
	if (codepoint == 0x27E6) return Ucd_Script_COMMON;
	if (codepoint == 0x27E7) return Ucd_Script_COMMON;
	if (codepoint == 0x27E8) return Ucd_Script_COMMON;
	if (codepoint == 0x27E9) return Ucd_Script_COMMON;
	if (codepoint == 0x27EA) return Ucd_Script_COMMON;
	if (codepoint == 0x27EB) return Ucd_Script_COMMON;
	if (codepoint == 0x27EC) return Ucd_Script_COMMON;
	if (codepoint == 0x27ED) return Ucd_Script_COMMON;
	if (codepoint == 0x27EE) return Ucd_Script_COMMON;
	if (codepoint == 0x27EF) return Ucd_Script_COMMON;
	if (codepoint >= 0x27F0 && codepoint <= 0x27FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x2900 && codepoint <= 0x2982) return Ucd_Script_COMMON;
	if (codepoint == 0x2983) return Ucd_Script_COMMON;
	if (codepoint == 0x2984) return Ucd_Script_COMMON;
	if (codepoint == 0x2985) return Ucd_Script_COMMON;
	if (codepoint == 0x2986) return Ucd_Script_COMMON;
	if (codepoint == 0x2987) return Ucd_Script_COMMON;
	if (codepoint == 0x2988) return Ucd_Script_COMMON;
	if (codepoint == 0x2989) return Ucd_Script_COMMON;
	if (codepoint == 0x298A) return Ucd_Script_COMMON;
	if (codepoint == 0x298B) return Ucd_Script_COMMON;
	if (codepoint == 0x298C) return Ucd_Script_COMMON;
	if (codepoint == 0x298D) return Ucd_Script_COMMON;
	if (codepoint == 0x298E) return Ucd_Script_COMMON;
	if (codepoint == 0x298F) return Ucd_Script_COMMON;
	if (codepoint == 0x2990) return Ucd_Script_COMMON;
	if (codepoint == 0x2991) return Ucd_Script_COMMON;
	if (codepoint == 0x2992) return Ucd_Script_COMMON;
	if (codepoint == 0x2993) return Ucd_Script_COMMON;
	if (codepoint == 0x2994) return Ucd_Script_COMMON;
	if (codepoint == 0x2995) return Ucd_Script_COMMON;
	if (codepoint == 0x2996) return Ucd_Script_COMMON;
	if (codepoint == 0x2997) return Ucd_Script_COMMON;
	if (codepoint == 0x2998) return Ucd_Script_COMMON;
	if (codepoint >= 0x2999 && codepoint <= 0x29D7) return Ucd_Script_COMMON;
	if (codepoint == 0x29D8) return Ucd_Script_COMMON;
	if (codepoint == 0x29D9) return Ucd_Script_COMMON;
	if (codepoint == 0x29DA) return Ucd_Script_COMMON;
	if (codepoint == 0x29DB) return Ucd_Script_COMMON;
	if (codepoint >= 0x29DC && codepoint <= 0x29FB) return Ucd_Script_COMMON;
	if (codepoint == 0x29FC) return Ucd_Script_COMMON;
	if (codepoint == 0x29FD) return Ucd_Script_COMMON;
	if (codepoint >= 0x29FE && codepoint <= 0x2AFF) return Ucd_Script_COMMON;
	if (codepoint >= 0x2B00 && codepoint <= 0x2B2F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2B30 && codepoint <= 0x2B44) return Ucd_Script_COMMON;
	if (codepoint >= 0x2B45 && codepoint <= 0x2B46) return Ucd_Script_COMMON;
	if (codepoint >= 0x2B47 && codepoint <= 0x2B4C) return Ucd_Script_COMMON;
	if (codepoint >= 0x2B4D && codepoint <= 0x2B73) return Ucd_Script_COMMON;
	if (codepoint >= 0x2B76 && codepoint <= 0x2B95) return Ucd_Script_COMMON;
	if (codepoint >= 0x2B97 && codepoint <= 0x2BFF) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E00 && codepoint <= 0x2E01) return Ucd_Script_COMMON;
	if (codepoint == 0x2E02) return Ucd_Script_COMMON;
	if (codepoint == 0x2E03) return Ucd_Script_COMMON;
	if (codepoint == 0x2E04) return Ucd_Script_COMMON;
	if (codepoint == 0x2E05) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E06 && codepoint <= 0x2E08) return Ucd_Script_COMMON;
	if (codepoint == 0x2E09) return Ucd_Script_COMMON;
	if (codepoint == 0x2E0A) return Ucd_Script_COMMON;
	if (codepoint == 0x2E0B) return Ucd_Script_COMMON;
	if (codepoint == 0x2E0C) return Ucd_Script_COMMON;
	if (codepoint == 0x2E0D) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E0E && codepoint <= 0x2E16) return Ucd_Script_COMMON;
	if (codepoint == 0x2E17) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E18 && codepoint <= 0x2E19) return Ucd_Script_COMMON;
	if (codepoint == 0x2E1A) return Ucd_Script_COMMON;
	if (codepoint == 0x2E1B) return Ucd_Script_COMMON;
	if (codepoint == 0x2E1C) return Ucd_Script_COMMON;
	if (codepoint == 0x2E1D) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E1E && codepoint <= 0x2E1F) return Ucd_Script_COMMON;
	if (codepoint == 0x2E20) return Ucd_Script_COMMON;
	if (codepoint == 0x2E21) return Ucd_Script_COMMON;
	if (codepoint == 0x2E22) return Ucd_Script_COMMON;
	if (codepoint == 0x2E23) return Ucd_Script_COMMON;
	if (codepoint == 0x2E24) return Ucd_Script_COMMON;
	if (codepoint == 0x2E25) return Ucd_Script_COMMON;
	if (codepoint == 0x2E26) return Ucd_Script_COMMON;
	if (codepoint == 0x2E27) return Ucd_Script_COMMON;
	if (codepoint == 0x2E28) return Ucd_Script_COMMON;
	if (codepoint == 0x2E29) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E2A && codepoint <= 0x2E2E) return Ucd_Script_COMMON;
	if (codepoint == 0x2E2F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E30 && codepoint <= 0x2E39) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E3A && codepoint <= 0x2E3B) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E3C && codepoint <= 0x2E3F) return Ucd_Script_COMMON;
	if (codepoint == 0x2E40) return Ucd_Script_COMMON;
	if (codepoint == 0x2E41) return Ucd_Script_COMMON;
	if (codepoint == 0x2E42) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E43 && codepoint <= 0x2E4F) return Ucd_Script_COMMON;
	if (codepoint >= 0x2E50 && codepoint <= 0x2E51) return Ucd_Script_COMMON;
	if (codepoint == 0x2E52) return Ucd_Script_COMMON;
	if (codepoint >= 0x2FF0 && codepoint <= 0x2FFB) return Ucd_Script_COMMON;
	if (codepoint == 0x3000) return Ucd_Script_COMMON;
	if (codepoint >= 0x3001 && codepoint <= 0x3003) return Ucd_Script_COMMON;
	if (codepoint == 0x3004) return Ucd_Script_COMMON;
	if (codepoint == 0x3006) return Ucd_Script_COMMON;
	if (codepoint == 0x3008) return Ucd_Script_COMMON;
	if (codepoint == 0x3009) return Ucd_Script_COMMON;
	if (codepoint == 0x300A) return Ucd_Script_COMMON;
	if (codepoint == 0x300B) return Ucd_Script_COMMON;
	if (codepoint == 0x300C) return Ucd_Script_COMMON;
	if (codepoint == 0x300D) return Ucd_Script_COMMON;
	if (codepoint == 0x300E) return Ucd_Script_COMMON;
	if (codepoint == 0x300F) return Ucd_Script_COMMON;
	if (codepoint == 0x3010) return Ucd_Script_COMMON;
	if (codepoint == 0x3011) return Ucd_Script_COMMON;
	if (codepoint >= 0x3012 && codepoint <= 0x3013) return Ucd_Script_COMMON;
	if (codepoint == 0x3014) return Ucd_Script_COMMON;
	if (codepoint == 0x3015) return Ucd_Script_COMMON;
	if (codepoint == 0x3016) return Ucd_Script_COMMON;
	if (codepoint == 0x3017) return Ucd_Script_COMMON;
	if (codepoint == 0x3018) return Ucd_Script_COMMON;
	if (codepoint == 0x3019) return Ucd_Script_COMMON;
	if (codepoint == 0x301A) return Ucd_Script_COMMON;
	if (codepoint == 0x301B) return Ucd_Script_COMMON;
	if (codepoint == 0x301C) return Ucd_Script_COMMON;
	if (codepoint == 0x301D) return Ucd_Script_COMMON;
	if (codepoint >= 0x301E && codepoint <= 0x301F) return Ucd_Script_COMMON;
	if (codepoint == 0x3020) return Ucd_Script_COMMON;
	if (codepoint == 0x3030) return Ucd_Script_COMMON;
	if (codepoint >= 0x3031 && codepoint <= 0x3035) return Ucd_Script_COMMON;
	if (codepoint >= 0x3036 && codepoint <= 0x3037) return Ucd_Script_COMMON;
	if (codepoint == 0x303C) return Ucd_Script_COMMON;
	if (codepoint == 0x303D) return Ucd_Script_COMMON;
	if (codepoint >= 0x303E && codepoint <= 0x303F) return Ucd_Script_COMMON;
	if (codepoint >= 0x309B && codepoint <= 0x309C) return Ucd_Script_COMMON;
	if (codepoint == 0x30A0) return Ucd_Script_COMMON;
	if (codepoint == 0x30FB) return Ucd_Script_COMMON;
	if (codepoint == 0x30FC) return Ucd_Script_COMMON;
	if (codepoint >= 0x3190 && codepoint <= 0x3191) return Ucd_Script_COMMON;
	if (codepoint >= 0x3192 && codepoint <= 0x3195) return Ucd_Script_COMMON;
	if (codepoint >= 0x3196 && codepoint <= 0x319F) return Ucd_Script_COMMON;
	if (codepoint >= 0x31C0 && codepoint <= 0x31E3) return Ucd_Script_COMMON;
	if (codepoint >= 0x3220 && codepoint <= 0x3229) return Ucd_Script_COMMON;
	if (codepoint >= 0x322A && codepoint <= 0x3247) return Ucd_Script_COMMON;
	if (codepoint >= 0x3248 && codepoint <= 0x324F) return Ucd_Script_COMMON;
	if (codepoint == 0x3250) return Ucd_Script_COMMON;
	if (codepoint >= 0x3251 && codepoint <= 0x325F) return Ucd_Script_COMMON;
	if (codepoint == 0x327F) return Ucd_Script_COMMON;
	if (codepoint >= 0x3280 && codepoint <= 0x3289) return Ucd_Script_COMMON;
	if (codepoint >= 0x328A && codepoint <= 0x32B0) return Ucd_Script_COMMON;
	if (codepoint >= 0x32B1 && codepoint <= 0x32BF) return Ucd_Script_COMMON;
	if (codepoint >= 0x32C0 && codepoint <= 0x32CF) return Ucd_Script_COMMON;
	if (codepoint == 0x32FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x3358 && codepoint <= 0x33FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x4DC0 && codepoint <= 0x4DFF) return Ucd_Script_COMMON;
	if (codepoint >= 0xA700 && codepoint <= 0xA716) return Ucd_Script_COMMON;
	if (codepoint >= 0xA717 && codepoint <= 0xA71F) return Ucd_Script_COMMON;
	if (codepoint >= 0xA720 && codepoint <= 0xA721) return Ucd_Script_COMMON;
	if (codepoint == 0xA788) return Ucd_Script_COMMON;
	if (codepoint >= 0xA789 && codepoint <= 0xA78A) return Ucd_Script_COMMON;
	if (codepoint >= 0xA830 && codepoint <= 0xA835) return Ucd_Script_COMMON;
	if (codepoint >= 0xA836 && codepoint <= 0xA837) return Ucd_Script_COMMON;
	if (codepoint == 0xA838) return Ucd_Script_COMMON;
	if (codepoint == 0xA839) return Ucd_Script_COMMON;
	if (codepoint == 0xA92E) return Ucd_Script_COMMON;
	if (codepoint == 0xA9CF) return Ucd_Script_COMMON;
	if (codepoint == 0xAB5B) return Ucd_Script_COMMON;
	if (codepoint >= 0xAB6A && codepoint <= 0xAB6B) return Ucd_Script_COMMON;
	if (codepoint == 0xFD3E) return Ucd_Script_COMMON;
	if (codepoint == 0xFD3F) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE10 && codepoint <= 0xFE16) return Ucd_Script_COMMON;
	if (codepoint == 0xFE17) return Ucd_Script_COMMON;
	if (codepoint == 0xFE18) return Ucd_Script_COMMON;
	if (codepoint == 0xFE19) return Ucd_Script_COMMON;
	if (codepoint == 0xFE30) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE31 && codepoint <= 0xFE32) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE33 && codepoint <= 0xFE34) return Ucd_Script_COMMON;
	if (codepoint == 0xFE35) return Ucd_Script_COMMON;
	if (codepoint == 0xFE36) return Ucd_Script_COMMON;
	if (codepoint == 0xFE37) return Ucd_Script_COMMON;
	if (codepoint == 0xFE38) return Ucd_Script_COMMON;
	if (codepoint == 0xFE39) return Ucd_Script_COMMON;
	if (codepoint == 0xFE3A) return Ucd_Script_COMMON;
	if (codepoint == 0xFE3B) return Ucd_Script_COMMON;
	if (codepoint == 0xFE3C) return Ucd_Script_COMMON;
	if (codepoint == 0xFE3D) return Ucd_Script_COMMON;
	if (codepoint == 0xFE3E) return Ucd_Script_COMMON;
	if (codepoint == 0xFE3F) return Ucd_Script_COMMON;
	if (codepoint == 0xFE40) return Ucd_Script_COMMON;
	if (codepoint == 0xFE41) return Ucd_Script_COMMON;
	if (codepoint == 0xFE42) return Ucd_Script_COMMON;
	if (codepoint == 0xFE43) return Ucd_Script_COMMON;
	if (codepoint == 0xFE44) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE45 && codepoint <= 0xFE46) return Ucd_Script_COMMON;
	if (codepoint == 0xFE47) return Ucd_Script_COMMON;
	if (codepoint == 0xFE48) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE49 && codepoint <= 0xFE4C) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE4D && codepoint <= 0xFE4F) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE50 && codepoint <= 0xFE52) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE54 && codepoint <= 0xFE57) return Ucd_Script_COMMON;
	if (codepoint == 0xFE58) return Ucd_Script_COMMON;
	if (codepoint == 0xFE59) return Ucd_Script_COMMON;
	if (codepoint == 0xFE5A) return Ucd_Script_COMMON;
	if (codepoint == 0xFE5B) return Ucd_Script_COMMON;
	if (codepoint == 0xFE5C) return Ucd_Script_COMMON;
	if (codepoint == 0xFE5D) return Ucd_Script_COMMON;
	if (codepoint == 0xFE5E) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE5F && codepoint <= 0xFE61) return Ucd_Script_COMMON;
	if (codepoint == 0xFE62) return Ucd_Script_COMMON;
	if (codepoint == 0xFE63) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE64 && codepoint <= 0xFE66) return Ucd_Script_COMMON;
	if (codepoint == 0xFE68) return Ucd_Script_COMMON;
	if (codepoint == 0xFE69) return Ucd_Script_COMMON;
	if (codepoint >= 0xFE6A && codepoint <= 0xFE6B) return Ucd_Script_COMMON;
	if (codepoint == 0xFEFF) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF01 && codepoint <= 0xFF03) return Ucd_Script_COMMON;
	if (codepoint == 0xFF04) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF05 && codepoint <= 0xFF07) return Ucd_Script_COMMON;
	if (codepoint == 0xFF08) return Ucd_Script_COMMON;
	if (codepoint == 0xFF09) return Ucd_Script_COMMON;
	if (codepoint == 0xFF0A) return Ucd_Script_COMMON;
	if (codepoint == 0xFF0B) return Ucd_Script_COMMON;
	if (codepoint == 0xFF0C) return Ucd_Script_COMMON;
	if (codepoint == 0xFF0D) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF0E && codepoint <= 0xFF0F) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF10 && codepoint <= 0xFF19) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF1A && codepoint <= 0xFF1B) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF1C && codepoint <= 0xFF1E) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF1F && codepoint <= 0xFF20) return Ucd_Script_COMMON;
	if (codepoint == 0xFF3B) return Ucd_Script_COMMON;
	if (codepoint == 0xFF3C) return Ucd_Script_COMMON;
	if (codepoint == 0xFF3D) return Ucd_Script_COMMON;
	if (codepoint == 0xFF3E) return Ucd_Script_COMMON;
	if (codepoint == 0xFF3F) return Ucd_Script_COMMON;
	if (codepoint == 0xFF40) return Ucd_Script_COMMON;
	if (codepoint == 0xFF5B) return Ucd_Script_COMMON;
	if (codepoint == 0xFF5C) return Ucd_Script_COMMON;
	if (codepoint == 0xFF5D) return Ucd_Script_COMMON;
	if (codepoint == 0xFF5E) return Ucd_Script_COMMON;
	if (codepoint == 0xFF5F) return Ucd_Script_COMMON;
	if (codepoint == 0xFF60) return Ucd_Script_COMMON;
	if (codepoint == 0xFF61) return Ucd_Script_COMMON;
	if (codepoint == 0xFF62) return Ucd_Script_COMMON;
	if (codepoint == 0xFF63) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF64 && codepoint <= 0xFF65) return Ucd_Script_COMMON;
	if (codepoint == 0xFF70) return Ucd_Script_COMMON;
	if (codepoint >= 0xFF9E && codepoint <= 0xFF9F) return Ucd_Script_COMMON;
	if (codepoint >= 0xFFE0 && codepoint <= 0xFFE1) return Ucd_Script_COMMON;
	if (codepoint == 0xFFE2) return Ucd_Script_COMMON;
	if (codepoint == 0xFFE3) return Ucd_Script_COMMON;
	if (codepoint == 0xFFE4) return Ucd_Script_COMMON;
	if (codepoint >= 0xFFE5 && codepoint <= 0xFFE6) return Ucd_Script_COMMON;
	if (codepoint == 0xFFE8) return Ucd_Script_COMMON;
	if (codepoint >= 0xFFE9 && codepoint <= 0xFFEC) return Ucd_Script_COMMON;
	if (codepoint >= 0xFFED && codepoint <= 0xFFEE) return Ucd_Script_COMMON;
	if (codepoint >= 0xFFF9 && codepoint <= 0xFFFB) return Ucd_Script_COMMON;
	if (codepoint >= 0xFFFC && codepoint <= 0xFFFD) return Ucd_Script_COMMON;
	if (codepoint >= 0x10100 && codepoint <= 0x10102) return Ucd_Script_COMMON;
	if (codepoint >= 0x10107 && codepoint <= 0x10133) return Ucd_Script_COMMON;
	if (codepoint >= 0x10137 && codepoint <= 0x1013F) return Ucd_Script_COMMON;
	if (codepoint >= 0x10190 && codepoint <= 0x1019C) return Ucd_Script_COMMON;
	if (codepoint >= 0x101D0 && codepoint <= 0x101FC) return Ucd_Script_COMMON;
	if (codepoint >= 0x102E1 && codepoint <= 0x102FB) return Ucd_Script_COMMON;
	if (codepoint == 0x16FE2) return Ucd_Script_COMMON;
	if (codepoint == 0x16FE3) return Ucd_Script_COMMON;
	if (codepoint >= 0x1BCA0 && codepoint <= 0x1BCA3) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D000 && codepoint <= 0x1D0F5) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D100 && codepoint <= 0x1D126) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D129 && codepoint <= 0x1D164) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D165 && codepoint <= 0x1D166) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D16A && codepoint <= 0x1D16C) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D16D && codepoint <= 0x1D172) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D173 && codepoint <= 0x1D17A) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D183 && codepoint <= 0x1D184) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D18C && codepoint <= 0x1D1A9) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D1AE && codepoint <= 0x1D1E8) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D2E0 && codepoint <= 0x1D2F3) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D300 && codepoint <= 0x1D356) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D360 && codepoint <= 0x1D378) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D400 && codepoint <= 0x1D454) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D456 && codepoint <= 0x1D49C) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D49E && codepoint <= 0x1D49F) return Ucd_Script_COMMON;
	if (codepoint == 0x1D4A2) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D4A5 && codepoint <= 0x1D4A6) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D4A9 && codepoint <= 0x1D4AC) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D4AE && codepoint <= 0x1D4B9) return Ucd_Script_COMMON;
	if (codepoint == 0x1D4BB) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D4BD && codepoint <= 0x1D4C3) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D4C5 && codepoint <= 0x1D505) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D507 && codepoint <= 0x1D50A) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D50D && codepoint <= 0x1D514) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D516 && codepoint <= 0x1D51C) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D51E && codepoint <= 0x1D539) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D53B && codepoint <= 0x1D53E) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D540 && codepoint <= 0x1D544) return Ucd_Script_COMMON;
	if (codepoint == 0x1D546) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D54A && codepoint <= 0x1D550) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D552 && codepoint <= 0x1D6A5) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D6A8 && codepoint <= 0x1D6C0) return Ucd_Script_COMMON;
	if (codepoint == 0x1D6C1) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D6C2 && codepoint <= 0x1D6DA) return Ucd_Script_COMMON;
	if (codepoint == 0x1D6DB) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D6DC && codepoint <= 0x1D6FA) return Ucd_Script_COMMON;
	if (codepoint == 0x1D6FB) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D6FC && codepoint <= 0x1D714) return Ucd_Script_COMMON;
	if (codepoint == 0x1D715) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D716 && codepoint <= 0x1D734) return Ucd_Script_COMMON;
	if (codepoint == 0x1D735) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D736 && codepoint <= 0x1D74E) return Ucd_Script_COMMON;
	if (codepoint == 0x1D74F) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D750 && codepoint <= 0x1D76E) return Ucd_Script_COMMON;
	if (codepoint == 0x1D76F) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D770 && codepoint <= 0x1D788) return Ucd_Script_COMMON;
	if (codepoint == 0x1D789) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D78A && codepoint <= 0x1D7A8) return Ucd_Script_COMMON;
	if (codepoint == 0x1D7A9) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D7AA && codepoint <= 0x1D7C2) return Ucd_Script_COMMON;
	if (codepoint == 0x1D7C3) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D7C4 && codepoint <= 0x1D7CB) return Ucd_Script_COMMON;
	if (codepoint >= 0x1D7CE && codepoint <= 0x1D7FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x1EC71 && codepoint <= 0x1ECAB) return Ucd_Script_COMMON;
	if (codepoint == 0x1ECAC) return Ucd_Script_COMMON;
	if (codepoint >= 0x1ECAD && codepoint <= 0x1ECAF) return Ucd_Script_COMMON;
	if (codepoint == 0x1ECB0) return Ucd_Script_COMMON;
	if (codepoint >= 0x1ECB1 && codepoint <= 0x1ECB4) return Ucd_Script_COMMON;
	if (codepoint >= 0x1ED01 && codepoint <= 0x1ED2D) return Ucd_Script_COMMON;
	if (codepoint == 0x1ED2E) return Ucd_Script_COMMON;
	if (codepoint >= 0x1ED2F && codepoint <= 0x1ED3D) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F000 && codepoint <= 0x1F02B) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F030 && codepoint <= 0x1F093) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F0A0 && codepoint <= 0x1F0AE) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F0B1 && codepoint <= 0x1F0BF) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F0C1 && codepoint <= 0x1F0CF) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F0D1 && codepoint <= 0x1F0F5) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F100 && codepoint <= 0x1F10C) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F10D && codepoint <= 0x1F1AD) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F1E6 && codepoint <= 0x1F1FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F201 && codepoint <= 0x1F202) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F210 && codepoint <= 0x1F23B) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F240 && codepoint <= 0x1F248) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F250 && codepoint <= 0x1F251) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F260 && codepoint <= 0x1F265) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F300 && codepoint <= 0x1F3FA) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F3FB && codepoint <= 0x1F3FF) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F400 && codepoint <= 0x1F6D7) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F6E0 && codepoint <= 0x1F6EC) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F6F0 && codepoint <= 0x1F6FC) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F700 && codepoint <= 0x1F773) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F780 && codepoint <= 0x1F7D8) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F7E0 && codepoint <= 0x1F7EB) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F800 && codepoint <= 0x1F80B) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F810 && codepoint <= 0x1F847) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F850 && codepoint <= 0x1F859) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F860 && codepoint <= 0x1F887) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F890 && codepoint <= 0x1F8AD) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F8B0 && codepoint <= 0x1F8B1) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F900 && codepoint <= 0x1F978) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F97A && codepoint <= 0x1F9CB) return Ucd_Script_COMMON;
	if (codepoint >= 0x1F9CD && codepoint <= 0x1FA53) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FA60 && codepoint <= 0x1FA6D) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FA70 && codepoint <= 0x1FA74) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FA78 && codepoint <= 0x1FA7A) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FA80 && codepoint <= 0x1FA86) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FA90 && codepoint <= 0x1FAA8) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FAB0 && codepoint <= 0x1FAB6) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FAC0 && codepoint <= 0x1FAC2) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FAD0 && codepoint <= 0x1FAD6) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FB00 && codepoint <= 0x1FB92) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FB94 && codepoint <= 0x1FBCA) return Ucd_Script_COMMON;
	if (codepoint >= 0x1FBF0 && codepoint <= 0x1FBF9) return Ucd_Script_COMMON;
	if (codepoint == 0xE0001) return Ucd_Script_COMMON;
	if (codepoint >= 0xE0020 && codepoint <= 0xE007F) return Ucd_Script_COMMON;
	if (codepoint >= 0x0041 && codepoint <= 0x005A) return Ucd_Script_LATIN;
	if (codepoint >= 0x0061 && codepoint <= 0x007A) return Ucd_Script_LATIN;
	if (codepoint == 0x00AA) return Ucd_Script_LATIN;
	if (codepoint == 0x00BA) return Ucd_Script_LATIN;
	if (codepoint >= 0x00C0 && codepoint <= 0x00D6) return Ucd_Script_LATIN;
	if (codepoint >= 0x00D8 && codepoint <= 0x00F6) return Ucd_Script_LATIN;
	if (codepoint >= 0x00F8 && codepoint <= 0x01BA) return Ucd_Script_LATIN;
	if (codepoint == 0x01BB) return Ucd_Script_LATIN;
	if (codepoint >= 0x01BC && codepoint <= 0x01BF) return Ucd_Script_LATIN;
	if (codepoint >= 0x01C0 && codepoint <= 0x01C3) return Ucd_Script_LATIN;
	if (codepoint >= 0x01C4 && codepoint <= 0x0293) return Ucd_Script_LATIN;
	if (codepoint == 0x0294) return Ucd_Script_LATIN;
	if (codepoint >= 0x0295 && codepoint <= 0x02AF) return Ucd_Script_LATIN;
	if (codepoint >= 0x02B0 && codepoint <= 0x02B8) return Ucd_Script_LATIN;
	if (codepoint >= 0x02E0 && codepoint <= 0x02E4) return Ucd_Script_LATIN;
	if (codepoint >= 0x1D00 && codepoint <= 0x1D25) return Ucd_Script_LATIN;
	if (codepoint >= 0x1D2C && codepoint <= 0x1D5C) return Ucd_Script_LATIN;
	if (codepoint >= 0x1D62 && codepoint <= 0x1D65) return Ucd_Script_LATIN;
	if (codepoint >= 0x1D6B && codepoint <= 0x1D77) return Ucd_Script_LATIN;
	if (codepoint >= 0x1D79 && codepoint <= 0x1D9A) return Ucd_Script_LATIN;
	if (codepoint >= 0x1D9B && codepoint <= 0x1DBE) return Ucd_Script_LATIN;
	if (codepoint >= 0x1E00 && codepoint <= 0x1EFF) return Ucd_Script_LATIN;
	if (codepoint == 0x2071) return Ucd_Script_LATIN;
	if (codepoint == 0x207F) return Ucd_Script_LATIN;
	if (codepoint >= 0x2090 && codepoint <= 0x209C) return Ucd_Script_LATIN;
	if (codepoint >= 0x212A && codepoint <= 0x212B) return Ucd_Script_LATIN;
	if (codepoint == 0x2132) return Ucd_Script_LATIN;
	if (codepoint == 0x214E) return Ucd_Script_LATIN;
	if (codepoint >= 0x2160 && codepoint <= 0x2182) return Ucd_Script_LATIN;
	if (codepoint >= 0x2183 && codepoint <= 0x2184) return Ucd_Script_LATIN;
	if (codepoint >= 0x2185 && codepoint <= 0x2188) return Ucd_Script_LATIN;
	if (codepoint >= 0x2C60 && codepoint <= 0x2C7B) return Ucd_Script_LATIN;
	if (codepoint >= 0x2C7C && codepoint <= 0x2C7D) return Ucd_Script_LATIN;
	if (codepoint >= 0x2C7E && codepoint <= 0x2C7F) return Ucd_Script_LATIN;
	if (codepoint >= 0xA722 && codepoint <= 0xA76F) return Ucd_Script_LATIN;
	if (codepoint == 0xA770) return Ucd_Script_LATIN;
	if (codepoint >= 0xA771 && codepoint <= 0xA787) return Ucd_Script_LATIN;
	if (codepoint >= 0xA78B && codepoint <= 0xA78E) return Ucd_Script_LATIN;
	if (codepoint == 0xA78F) return Ucd_Script_LATIN;
	if (codepoint >= 0xA790 && codepoint <= 0xA7BF) return Ucd_Script_LATIN;
	if (codepoint >= 0xA7C2 && codepoint <= 0xA7CA) return Ucd_Script_LATIN;
	if (codepoint >= 0xA7F5 && codepoint <= 0xA7F6) return Ucd_Script_LATIN;
	if (codepoint == 0xA7F7) return Ucd_Script_LATIN;
	if (codepoint >= 0xA7F8 && codepoint <= 0xA7F9) return Ucd_Script_LATIN;
	if (codepoint == 0xA7FA) return Ucd_Script_LATIN;
	if (codepoint >= 0xA7FB && codepoint <= 0xA7FF) return Ucd_Script_LATIN;
	if (codepoint >= 0xAB30 && codepoint <= 0xAB5A) return Ucd_Script_LATIN;
	if (codepoint >= 0xAB5C && codepoint <= 0xAB5F) return Ucd_Script_LATIN;
	if (codepoint >= 0xAB60 && codepoint <= 0xAB64) return Ucd_Script_LATIN;
	if (codepoint >= 0xAB66 && codepoint <= 0xAB68) return Ucd_Script_LATIN;
	if (codepoint == 0xAB69) return Ucd_Script_LATIN;
	if (codepoint >= 0xFB00 && codepoint <= 0xFB06) return Ucd_Script_LATIN;
	if (codepoint >= 0xFF21 && codepoint <= 0xFF3A) return Ucd_Script_LATIN;
	if (codepoint >= 0xFF41 && codepoint <= 0xFF5A) return Ucd_Script_LATIN;
	if (codepoint >= 0x0370 && codepoint <= 0x0373) return Ucd_Script_GREEK;
	if (codepoint == 0x0375) return Ucd_Script_GREEK;
	if (codepoint >= 0x0376 && codepoint <= 0x0377) return Ucd_Script_GREEK;
	if (codepoint == 0x037A) return Ucd_Script_GREEK;
	if (codepoint >= 0x037B && codepoint <= 0x037D) return Ucd_Script_GREEK;
	if (codepoint == 0x037F) return Ucd_Script_GREEK;
	if (codepoint == 0x0384) return Ucd_Script_GREEK;
	if (codepoint == 0x0386) return Ucd_Script_GREEK;
	if (codepoint >= 0x0388 && codepoint <= 0x038A) return Ucd_Script_GREEK;
	if (codepoint == 0x038C) return Ucd_Script_GREEK;
	if (codepoint >= 0x038E && codepoint <= 0x03A1) return Ucd_Script_GREEK;
	if (codepoint >= 0x03A3 && codepoint <= 0x03E1) return Ucd_Script_GREEK;
	if (codepoint >= 0x03F0 && codepoint <= 0x03F5) return Ucd_Script_GREEK;
	if (codepoint == 0x03F6) return Ucd_Script_GREEK;
	if (codepoint >= 0x03F7 && codepoint <= 0x03FF) return Ucd_Script_GREEK;
	if (codepoint >= 0x1D26 && codepoint <= 0x1D2A) return Ucd_Script_GREEK;
	if (codepoint >= 0x1D5D && codepoint <= 0x1D61) return Ucd_Script_GREEK;
	if (codepoint >= 0x1D66 && codepoint <= 0x1D6A) return Ucd_Script_GREEK;
	if (codepoint == 0x1DBF) return Ucd_Script_GREEK;
	if (codepoint >= 0x1F00 && codepoint <= 0x1F15) return Ucd_Script_GREEK;
	if (codepoint >= 0x1F18 && codepoint <= 0x1F1D) return Ucd_Script_GREEK;
	if (codepoint >= 0x1F20 && codepoint <= 0x1F45) return Ucd_Script_GREEK;
	if (codepoint >= 0x1F48 && codepoint <= 0x1F4D) return Ucd_Script_GREEK;
	if (codepoint >= 0x1F50 && codepoint <= 0x1F57) return Ucd_Script_GREEK;
	if (codepoint == 0x1F59) return Ucd_Script_GREEK;
	if (codepoint == 0x1F5B) return Ucd_Script_GREEK;
	if (codepoint == 0x1F5D) return Ucd_Script_GREEK;
	if (codepoint >= 0x1F5F && codepoint <= 0x1F7D) return Ucd_Script_GREEK;
	if (codepoint >= 0x1F80 && codepoint <= 0x1FB4) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FB6 && codepoint <= 0x1FBC) return Ucd_Script_GREEK;
	if (codepoint == 0x1FBD) return Ucd_Script_GREEK;
	if (codepoint == 0x1FBE) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FBF && codepoint <= 0x1FC1) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FC2 && codepoint <= 0x1FC4) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FC6 && codepoint <= 0x1FCC) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FCD && codepoint <= 0x1FCF) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FD0 && codepoint <= 0x1FD3) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FD6 && codepoint <= 0x1FDB) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FDD && codepoint <= 0x1FDF) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FE0 && codepoint <= 0x1FEC) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FED && codepoint <= 0x1FEF) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FF2 && codepoint <= 0x1FF4) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FF6 && codepoint <= 0x1FFC) return Ucd_Script_GREEK;
	if (codepoint >= 0x1FFD && codepoint <= 0x1FFE) return Ucd_Script_GREEK;
	if (codepoint == 0x2126) return Ucd_Script_GREEK;
	if (codepoint == 0xAB65) return Ucd_Script_GREEK;
	if (codepoint >= 0x10140 && codepoint <= 0x10174) return Ucd_Script_GREEK;
	if (codepoint >= 0x10175 && codepoint <= 0x10178) return Ucd_Script_GREEK;
	if (codepoint >= 0x10179 && codepoint <= 0x10189) return Ucd_Script_GREEK;
	if (codepoint >= 0x1018A && codepoint <= 0x1018B) return Ucd_Script_GREEK;
	if (codepoint >= 0x1018C && codepoint <= 0x1018E) return Ucd_Script_GREEK;
	if (codepoint == 0x101A0) return Ucd_Script_GREEK;
	if (codepoint >= 0x1D200 && codepoint <= 0x1D241) return Ucd_Script_GREEK;
	if (codepoint >= 0x1D242 && codepoint <= 0x1D244) return Ucd_Script_GREEK;
	if (codepoint == 0x1D245) return Ucd_Script_GREEK;
	if (codepoint >= 0x0400 && codepoint <= 0x0481) return Ucd_Script_CYRILLIC;
	if (codepoint == 0x0482) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0x0483 && codepoint <= 0x0484) return Ucd_Script_CYRILLIC;
	if (codepoint == 0x0487) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0x0488 && codepoint <= 0x0489) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0x048A && codepoint <= 0x052F) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0x1C80 && codepoint <= 0x1C88) return Ucd_Script_CYRILLIC;
	if (codepoint == 0x1D2B) return Ucd_Script_CYRILLIC;
	if (codepoint == 0x1D78) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0x2DE0 && codepoint <= 0x2DFF) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0xA640 && codepoint <= 0xA66D) return Ucd_Script_CYRILLIC;
	if (codepoint == 0xA66E) return Ucd_Script_CYRILLIC;
	if (codepoint == 0xA66F) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0xA670 && codepoint <= 0xA672) return Ucd_Script_CYRILLIC;
	if (codepoint == 0xA673) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0xA674 && codepoint <= 0xA67D) return Ucd_Script_CYRILLIC;
	if (codepoint == 0xA67E) return Ucd_Script_CYRILLIC;
	if (codepoint == 0xA67F) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0xA680 && codepoint <= 0xA69B) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0xA69C && codepoint <= 0xA69D) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0xA69E && codepoint <= 0xA69F) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0xFE2E && codepoint <= 0xFE2F) return Ucd_Script_CYRILLIC;
	if (codepoint >= 0x0531 && codepoint <= 0x0556) return Ucd_Script_ARMENIAN;
	if (codepoint == 0x0559) return Ucd_Script_ARMENIAN;
	if (codepoint >= 0x055A && codepoint <= 0x055F) return Ucd_Script_ARMENIAN;
	if (codepoint >= 0x0560 && codepoint <= 0x0588) return Ucd_Script_ARMENIAN;
	if (codepoint == 0x0589) return Ucd_Script_ARMENIAN;
	if (codepoint == 0x058A) return Ucd_Script_ARMENIAN;
	if (codepoint >= 0x058D && codepoint <= 0x058E) return Ucd_Script_ARMENIAN;
	if (codepoint == 0x058F) return Ucd_Script_ARMENIAN;
	if (codepoint >= 0xFB13 && codepoint <= 0xFB17) return Ucd_Script_ARMENIAN;
	if (codepoint >= 0x0591 && codepoint <= 0x05BD) return Ucd_Script_HEBREW;
	if (codepoint == 0x05BE) return Ucd_Script_HEBREW;
	if (codepoint == 0x05BF) return Ucd_Script_HEBREW;
	if (codepoint == 0x05C0) return Ucd_Script_HEBREW;
	if (codepoint >= 0x05C1 && codepoint <= 0x05C2) return Ucd_Script_HEBREW;
	if (codepoint == 0x05C3) return Ucd_Script_HEBREW;
	if (codepoint >= 0x05C4 && codepoint <= 0x05C5) return Ucd_Script_HEBREW;
	if (codepoint == 0x05C6) return Ucd_Script_HEBREW;
	if (codepoint == 0x05C7) return Ucd_Script_HEBREW;
	if (codepoint >= 0x05D0 && codepoint <= 0x05EA) return Ucd_Script_HEBREW;
	if (codepoint >= 0x05EF && codepoint <= 0x05F2) return Ucd_Script_HEBREW;
	if (codepoint >= 0x05F3 && codepoint <= 0x05F4) return Ucd_Script_HEBREW;
	if (codepoint == 0xFB1D) return Ucd_Script_HEBREW;
	if (codepoint == 0xFB1E) return Ucd_Script_HEBREW;
	if (codepoint >= 0xFB1F && codepoint <= 0xFB28) return Ucd_Script_HEBREW;
	if (codepoint == 0xFB29) return Ucd_Script_HEBREW;
	if (codepoint >= 0xFB2A && codepoint <= 0xFB36) return Ucd_Script_HEBREW;
	if (codepoint >= 0xFB38 && codepoint <= 0xFB3C) return Ucd_Script_HEBREW;
	if (codepoint == 0xFB3E) return Ucd_Script_HEBREW;
	if (codepoint >= 0xFB40 && codepoint <= 0xFB41) return Ucd_Script_HEBREW;
	if (codepoint >= 0xFB43 && codepoint <= 0xFB44) return Ucd_Script_HEBREW;
	if (codepoint >= 0xFB46 && codepoint <= 0xFB4F) return Ucd_Script_HEBREW;
	if (codepoint >= 0x0600 && codepoint <= 0x0604) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0606 && codepoint <= 0x0608) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0609 && codepoint <= 0x060A) return Ucd_Script_ARABIC;
	if (codepoint == 0x060B) return Ucd_Script_ARABIC;
	if (codepoint == 0x060D) return Ucd_Script_ARABIC;
	if (codepoint >= 0x060E && codepoint <= 0x060F) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0610 && codepoint <= 0x061A) return Ucd_Script_ARABIC;
	if (codepoint == 0x061C) return Ucd_Script_ARABIC;
	if (codepoint == 0x061E) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0620 && codepoint <= 0x063F) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0641 && codepoint <= 0x064A) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0656 && codepoint <= 0x065F) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0660 && codepoint <= 0x0669) return Ucd_Script_ARABIC;
	if (codepoint >= 0x066A && codepoint <= 0x066D) return Ucd_Script_ARABIC;
	if (codepoint >= 0x066E && codepoint <= 0x066F) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0671 && codepoint <= 0x06D3) return Ucd_Script_ARABIC;
	if (codepoint == 0x06D4) return Ucd_Script_ARABIC;
	if (codepoint == 0x06D5) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06D6 && codepoint <= 0x06DC) return Ucd_Script_ARABIC;
	if (codepoint == 0x06DE) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06DF && codepoint <= 0x06E4) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06E5 && codepoint <= 0x06E6) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06E7 && codepoint <= 0x06E8) return Ucd_Script_ARABIC;
	if (codepoint == 0x06E9) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06EA && codepoint <= 0x06ED) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06EE && codepoint <= 0x06EF) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06F0 && codepoint <= 0x06F9) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06FA && codepoint <= 0x06FC) return Ucd_Script_ARABIC;
	if (codepoint >= 0x06FD && codepoint <= 0x06FE) return Ucd_Script_ARABIC;
	if (codepoint == 0x06FF) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0750 && codepoint <= 0x077F) return Ucd_Script_ARABIC;
	if (codepoint >= 0x08A0 && codepoint <= 0x08B4) return Ucd_Script_ARABIC;
	if (codepoint >= 0x08B6 && codepoint <= 0x08C7) return Ucd_Script_ARABIC;
	if (codepoint >= 0x08D3 && codepoint <= 0x08E1) return Ucd_Script_ARABIC;
	if (codepoint >= 0x08E3 && codepoint <= 0x08FF) return Ucd_Script_ARABIC;
	if (codepoint >= 0xFB50 && codepoint <= 0xFBB1) return Ucd_Script_ARABIC;
	if (codepoint >= 0xFBB2 && codepoint <= 0xFBC1) return Ucd_Script_ARABIC;
	if (codepoint >= 0xFBD3 && codepoint <= 0xFD3D) return Ucd_Script_ARABIC;
	if (codepoint >= 0xFD50 && codepoint <= 0xFD8F) return Ucd_Script_ARABIC;
	if (codepoint >= 0xFD92 && codepoint <= 0xFDC7) return Ucd_Script_ARABIC;
	if (codepoint >= 0xFDF0 && codepoint <= 0xFDFB) return Ucd_Script_ARABIC;
	if (codepoint == 0xFDFC) return Ucd_Script_ARABIC;
	if (codepoint == 0xFDFD) return Ucd_Script_ARABIC;
	if (codepoint >= 0xFE70 && codepoint <= 0xFE74) return Ucd_Script_ARABIC;
	if (codepoint >= 0xFE76 && codepoint <= 0xFEFC) return Ucd_Script_ARABIC;
	if (codepoint >= 0x10E60 && codepoint <= 0x10E7E) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE00 && codepoint <= 0x1EE03) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE05 && codepoint <= 0x1EE1F) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE21 && codepoint <= 0x1EE22) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE24) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE27) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE29 && codepoint <= 0x1EE32) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE34 && codepoint <= 0x1EE37) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE39) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE3B) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE42) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE47) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE49) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE4B) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE4D && codepoint <= 0x1EE4F) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE51 && codepoint <= 0x1EE52) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE54) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE57) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE59) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE5B) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE5D) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE5F) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE61 && codepoint <= 0x1EE62) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE64) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE67 && codepoint <= 0x1EE6A) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE6C && codepoint <= 0x1EE72) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE74 && codepoint <= 0x1EE77) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE79 && codepoint <= 0x1EE7C) return Ucd_Script_ARABIC;
	if (codepoint == 0x1EE7E) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE80 && codepoint <= 0x1EE89) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EE8B && codepoint <= 0x1EE9B) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EEA1 && codepoint <= 0x1EEA3) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EEA5 && codepoint <= 0x1EEA9) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EEAB && codepoint <= 0x1EEBB) return Ucd_Script_ARABIC;
	if (codepoint >= 0x1EEF0 && codepoint <= 0x1EEF1) return Ucd_Script_ARABIC;
	if (codepoint >= 0x0700 && codepoint <= 0x070D) return Ucd_Script_SYRIAC;
	if (codepoint == 0x070F) return Ucd_Script_SYRIAC;
	if (codepoint == 0x0710) return Ucd_Script_SYRIAC;
	if (codepoint == 0x0711) return Ucd_Script_SYRIAC;
	if (codepoint >= 0x0712 && codepoint <= 0x072F) return Ucd_Script_SYRIAC;
	if (codepoint >= 0x0730 && codepoint <= 0x074A) return Ucd_Script_SYRIAC;
	if (codepoint >= 0x074D && codepoint <= 0x074F) return Ucd_Script_SYRIAC;
	if (codepoint >= 0x0860 && codepoint <= 0x086A) return Ucd_Script_SYRIAC;
	if (codepoint >= 0x0780 && codepoint <= 0x07A5) return Ucd_Script_THAANA;
	if (codepoint >= 0x07A6 && codepoint <= 0x07B0) return Ucd_Script_THAANA;
	if (codepoint == 0x07B1) return Ucd_Script_THAANA;
	if (codepoint >= 0x0900 && codepoint <= 0x0902) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x0903) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x0904 && codepoint <= 0x0939) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x093A) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x093B) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x093C) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x093D) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x093E && codepoint <= 0x0940) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x0941 && codepoint <= 0x0948) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x0949 && codepoint <= 0x094C) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x094D) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x094E && codepoint <= 0x094F) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x0950) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x0955 && codepoint <= 0x0957) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x0958 && codepoint <= 0x0961) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x0962 && codepoint <= 0x0963) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x0966 && codepoint <= 0x096F) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x0970) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x0971) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0x0972 && codepoint <= 0x097F) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0xA8E0 && codepoint <= 0xA8F1) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0xA8F2 && codepoint <= 0xA8F7) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0xA8F8 && codepoint <= 0xA8FA) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0xA8FB) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0xA8FC) return Ucd_Script_DEVANAGARI;
	if (codepoint >= 0xA8FD && codepoint <= 0xA8FE) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0xA8FF) return Ucd_Script_DEVANAGARI;
	if (codepoint == 0x0980) return Ucd_Script_BENGALI;
	if (codepoint == 0x0981) return Ucd_Script_BENGALI;
	if (codepoint >= 0x0982 && codepoint <= 0x0983) return Ucd_Script_BENGALI;
	if (codepoint >= 0x0985 && codepoint <= 0x098C) return Ucd_Script_BENGALI;
	if (codepoint >= 0x098F && codepoint <= 0x0990) return Ucd_Script_BENGALI;
	if (codepoint >= 0x0993 && codepoint <= 0x09A8) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09AA && codepoint <= 0x09B0) return Ucd_Script_BENGALI;
	if (codepoint == 0x09B2) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09B6 && codepoint <= 0x09B9) return Ucd_Script_BENGALI;
	if (codepoint == 0x09BC) return Ucd_Script_BENGALI;
	if (codepoint == 0x09BD) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09BE && codepoint <= 0x09C0) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09C1 && codepoint <= 0x09C4) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09C7 && codepoint <= 0x09C8) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09CB && codepoint <= 0x09CC) return Ucd_Script_BENGALI;
	if (codepoint == 0x09CD) return Ucd_Script_BENGALI;
	if (codepoint == 0x09CE) return Ucd_Script_BENGALI;
	if (codepoint == 0x09D7) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09DC && codepoint <= 0x09DD) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09DF && codepoint <= 0x09E1) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09E2 && codepoint <= 0x09E3) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09E6 && codepoint <= 0x09EF) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09F0 && codepoint <= 0x09F1) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09F2 && codepoint <= 0x09F3) return Ucd_Script_BENGALI;
	if (codepoint >= 0x09F4 && codepoint <= 0x09F9) return Ucd_Script_BENGALI;
	if (codepoint == 0x09FA) return Ucd_Script_BENGALI;
	if (codepoint == 0x09FB) return Ucd_Script_BENGALI;
	if (codepoint == 0x09FC) return Ucd_Script_BENGALI;
	if (codepoint == 0x09FD) return Ucd_Script_BENGALI;
	if (codepoint == 0x09FE) return Ucd_Script_BENGALI;
	if (codepoint >= 0x0A01 && codepoint <= 0x0A02) return Ucd_Script_GURMUKHI;
	if (codepoint == 0x0A03) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A05 && codepoint <= 0x0A0A) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A0F && codepoint <= 0x0A10) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A13 && codepoint <= 0x0A28) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A2A && codepoint <= 0x0A30) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A32 && codepoint <= 0x0A33) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A35 && codepoint <= 0x0A36) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A38 && codepoint <= 0x0A39) return Ucd_Script_GURMUKHI;
	if (codepoint == 0x0A3C) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A3E && codepoint <= 0x0A40) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A41 && codepoint <= 0x0A42) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A47 && codepoint <= 0x0A48) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A4B && codepoint <= 0x0A4D) return Ucd_Script_GURMUKHI;
	if (codepoint == 0x0A51) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A59 && codepoint <= 0x0A5C) return Ucd_Script_GURMUKHI;
	if (codepoint == 0x0A5E) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A66 && codepoint <= 0x0A6F) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A70 && codepoint <= 0x0A71) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A72 && codepoint <= 0x0A74) return Ucd_Script_GURMUKHI;
	if (codepoint == 0x0A75) return Ucd_Script_GURMUKHI;
	if (codepoint == 0x0A76) return Ucd_Script_GURMUKHI;
	if (codepoint >= 0x0A81 && codepoint <= 0x0A82) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0A83) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0A85 && codepoint <= 0x0A8D) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0A8F && codepoint <= 0x0A91) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0A93 && codepoint <= 0x0AA8) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AAA && codepoint <= 0x0AB0) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AB2 && codepoint <= 0x0AB3) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AB5 && codepoint <= 0x0AB9) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0ABC) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0ABD) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0ABE && codepoint <= 0x0AC0) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AC1 && codepoint <= 0x0AC5) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AC7 && codepoint <= 0x0AC8) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0AC9) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0ACB && codepoint <= 0x0ACC) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0ACD) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0AD0) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AE0 && codepoint <= 0x0AE1) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AE2 && codepoint <= 0x0AE3) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AE6 && codepoint <= 0x0AEF) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0AF0) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0AF1) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0AF9) return Ucd_Script_GUJARATI;
	if (codepoint >= 0x0AFA && codepoint <= 0x0AFF) return Ucd_Script_GUJARATI;
	if (codepoint == 0x0B01) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B02 && codepoint <= 0x0B03) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B05 && codepoint <= 0x0B0C) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B0F && codepoint <= 0x0B10) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B13 && codepoint <= 0x0B28) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B2A && codepoint <= 0x0B30) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B32 && codepoint <= 0x0B33) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B35 && codepoint <= 0x0B39) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B3C) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B3D) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B3E) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B3F) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B40) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B41 && codepoint <= 0x0B44) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B47 && codepoint <= 0x0B48) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B4B && codepoint <= 0x0B4C) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B4D) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B55 && codepoint <= 0x0B56) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B57) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B5C && codepoint <= 0x0B5D) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B5F && codepoint <= 0x0B61) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B62 && codepoint <= 0x0B63) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B66 && codepoint <= 0x0B6F) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B70) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B71) return Ucd_Script_ORIYA;
	if (codepoint >= 0x0B72 && codepoint <= 0x0B77) return Ucd_Script_ORIYA;
	if (codepoint == 0x0B82) return Ucd_Script_TAMIL;
	if (codepoint == 0x0B83) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0B85 && codepoint <= 0x0B8A) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0B8E && codepoint <= 0x0B90) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0B92 && codepoint <= 0x0B95) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0B99 && codepoint <= 0x0B9A) return Ucd_Script_TAMIL;
	if (codepoint == 0x0B9C) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0B9E && codepoint <= 0x0B9F) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BA3 && codepoint <= 0x0BA4) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BA8 && codepoint <= 0x0BAA) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BAE && codepoint <= 0x0BB9) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BBE && codepoint <= 0x0BBF) return Ucd_Script_TAMIL;
	if (codepoint == 0x0BC0) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BC1 && codepoint <= 0x0BC2) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BC6 && codepoint <= 0x0BC8) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BCA && codepoint <= 0x0BCC) return Ucd_Script_TAMIL;
	if (codepoint == 0x0BCD) return Ucd_Script_TAMIL;
	if (codepoint == 0x0BD0) return Ucd_Script_TAMIL;
	if (codepoint == 0x0BD7) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BE6 && codepoint <= 0x0BEF) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BF0 && codepoint <= 0x0BF2) return Ucd_Script_TAMIL;
	if (codepoint >= 0x0BF3 && codepoint <= 0x0BF8) return Ucd_Script_TAMIL;
	if (codepoint == 0x0BF9) return Ucd_Script_TAMIL;
	if (codepoint == 0x0BFA) return Ucd_Script_TAMIL;
	if (codepoint >= 0x11FC0 && codepoint <= 0x11FD4) return Ucd_Script_TAMIL;
	if (codepoint >= 0x11FD5 && codepoint <= 0x11FDC) return Ucd_Script_TAMIL;
	if (codepoint >= 0x11FDD && codepoint <= 0x11FE0) return Ucd_Script_TAMIL;
	if (codepoint >= 0x11FE1 && codepoint <= 0x11FF1) return Ucd_Script_TAMIL;
	if (codepoint == 0x11FFF) return Ucd_Script_TAMIL;
	if (codepoint == 0x0C00) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C01 && codepoint <= 0x0C03) return Ucd_Script_TELUGU;
	if (codepoint == 0x0C04) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C05 && codepoint <= 0x0C0C) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C0E && codepoint <= 0x0C10) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C12 && codepoint <= 0x0C28) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C2A && codepoint <= 0x0C39) return Ucd_Script_TELUGU;
	if (codepoint == 0x0C3D) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C3E && codepoint <= 0x0C40) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C41 && codepoint <= 0x0C44) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C46 && codepoint <= 0x0C48) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C4A && codepoint <= 0x0C4D) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C55 && codepoint <= 0x0C56) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C58 && codepoint <= 0x0C5A) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C60 && codepoint <= 0x0C61) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C62 && codepoint <= 0x0C63) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C66 && codepoint <= 0x0C6F) return Ucd_Script_TELUGU;
	if (codepoint == 0x0C77) return Ucd_Script_TELUGU;
	if (codepoint >= 0x0C78 && codepoint <= 0x0C7E) return Ucd_Script_TELUGU;
	if (codepoint == 0x0C7F) return Ucd_Script_TELUGU;
	if (codepoint == 0x0C80) return Ucd_Script_KANNADA;
	if (codepoint == 0x0C81) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0C82 && codepoint <= 0x0C83) return Ucd_Script_KANNADA;
	if (codepoint == 0x0C84) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0C85 && codepoint <= 0x0C8C) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0C8E && codepoint <= 0x0C90) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0C92 && codepoint <= 0x0CA8) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CAA && codepoint <= 0x0CB3) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CB5 && codepoint <= 0x0CB9) return Ucd_Script_KANNADA;
	if (codepoint == 0x0CBC) return Ucd_Script_KANNADA;
	if (codepoint == 0x0CBD) return Ucd_Script_KANNADA;
	if (codepoint == 0x0CBE) return Ucd_Script_KANNADA;
	if (codepoint == 0x0CBF) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CC0 && codepoint <= 0x0CC4) return Ucd_Script_KANNADA;
	if (codepoint == 0x0CC6) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CC7 && codepoint <= 0x0CC8) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CCA && codepoint <= 0x0CCB) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CCC && codepoint <= 0x0CCD) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CD5 && codepoint <= 0x0CD6) return Ucd_Script_KANNADA;
	if (codepoint == 0x0CDE) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CE0 && codepoint <= 0x0CE1) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CE2 && codepoint <= 0x0CE3) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CE6 && codepoint <= 0x0CEF) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0CF1 && codepoint <= 0x0CF2) return Ucd_Script_KANNADA;
	if (codepoint >= 0x0D00 && codepoint <= 0x0D01) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D02 && codepoint <= 0x0D03) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D04 && codepoint <= 0x0D0C) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D0E && codepoint <= 0x0D10) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D12 && codepoint <= 0x0D3A) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D3B && codepoint <= 0x0D3C) return Ucd_Script_MALAYALAM;
	if (codepoint == 0x0D3D) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D3E && codepoint <= 0x0D40) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D41 && codepoint <= 0x0D44) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D46 && codepoint <= 0x0D48) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D4A && codepoint <= 0x0D4C) return Ucd_Script_MALAYALAM;
	if (codepoint == 0x0D4D) return Ucd_Script_MALAYALAM;
	if (codepoint == 0x0D4E) return Ucd_Script_MALAYALAM;
	if (codepoint == 0x0D4F) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D54 && codepoint <= 0x0D56) return Ucd_Script_MALAYALAM;
	if (codepoint == 0x0D57) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D58 && codepoint <= 0x0D5E) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D5F && codepoint <= 0x0D61) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D62 && codepoint <= 0x0D63) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D66 && codepoint <= 0x0D6F) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D70 && codepoint <= 0x0D78) return Ucd_Script_MALAYALAM;
	if (codepoint == 0x0D79) return Ucd_Script_MALAYALAM;
	if (codepoint >= 0x0D7A && codepoint <= 0x0D7F) return Ucd_Script_MALAYALAM;
	if (codepoint == 0x0D81) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0D82 && codepoint <= 0x0D83) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0D85 && codepoint <= 0x0D96) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0D9A && codepoint <= 0x0DB1) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0DB3 && codepoint <= 0x0DBB) return Ucd_Script_SINHALA;
	if (codepoint == 0x0DBD) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0DC0 && codepoint <= 0x0DC6) return Ucd_Script_SINHALA;
	if (codepoint == 0x0DCA) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0DCF && codepoint <= 0x0DD1) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0DD2 && codepoint <= 0x0DD4) return Ucd_Script_SINHALA;
	if (codepoint == 0x0DD6) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0DD8 && codepoint <= 0x0DDF) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0DE6 && codepoint <= 0x0DEF) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0DF2 && codepoint <= 0x0DF3) return Ucd_Script_SINHALA;
	if (codepoint == 0x0DF4) return Ucd_Script_SINHALA;
	if (codepoint >= 0x111E1 && codepoint <= 0x111F4) return Ucd_Script_SINHALA;
	if (codepoint >= 0x0E01 && codepoint <= 0x0E30) return Ucd_Script_THAI;
	if (codepoint == 0x0E31) return Ucd_Script_THAI;
	if (codepoint >= 0x0E32 && codepoint <= 0x0E33) return Ucd_Script_THAI;
	if (codepoint >= 0x0E34 && codepoint <= 0x0E3A) return Ucd_Script_THAI;
	if (codepoint >= 0x0E40 && codepoint <= 0x0E45) return Ucd_Script_THAI;
	if (codepoint == 0x0E46) return Ucd_Script_THAI;
	if (codepoint >= 0x0E47 && codepoint <= 0x0E4E) return Ucd_Script_THAI;
	if (codepoint == 0x0E4F) return Ucd_Script_THAI;
	if (codepoint >= 0x0E50 && codepoint <= 0x0E59) return Ucd_Script_THAI;
	if (codepoint >= 0x0E5A && codepoint <= 0x0E5B) return Ucd_Script_THAI;
	if (codepoint >= 0x0E81 && codepoint <= 0x0E82) return Ucd_Script_LAO;
	if (codepoint == 0x0E84) return Ucd_Script_LAO;
	if (codepoint >= 0x0E86 && codepoint <= 0x0E8A) return Ucd_Script_LAO;
	if (codepoint >= 0x0E8C && codepoint <= 0x0EA3) return Ucd_Script_LAO;
	if (codepoint == 0x0EA5) return Ucd_Script_LAO;
	if (codepoint >= 0x0EA7 && codepoint <= 0x0EB0) return Ucd_Script_LAO;
	if (codepoint == 0x0EB1) return Ucd_Script_LAO;
	if (codepoint >= 0x0EB2 && codepoint <= 0x0EB3) return Ucd_Script_LAO;
	if (codepoint >= 0x0EB4 && codepoint <= 0x0EBC) return Ucd_Script_LAO;
	if (codepoint == 0x0EBD) return Ucd_Script_LAO;
	if (codepoint >= 0x0EC0 && codepoint <= 0x0EC4) return Ucd_Script_LAO;
	if (codepoint == 0x0EC6) return Ucd_Script_LAO;
	if (codepoint >= 0x0EC8 && codepoint <= 0x0ECD) return Ucd_Script_LAO;
	if (codepoint >= 0x0ED0 && codepoint <= 0x0ED9) return Ucd_Script_LAO;
	if (codepoint >= 0x0EDC && codepoint <= 0x0EDF) return Ucd_Script_LAO;
	if (codepoint == 0x0F00) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F01 && codepoint <= 0x0F03) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F04 && codepoint <= 0x0F12) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F13) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F14) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F15 && codepoint <= 0x0F17) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F18 && codepoint <= 0x0F19) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F1A && codepoint <= 0x0F1F) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F20 && codepoint <= 0x0F29) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F2A && codepoint <= 0x0F33) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F34) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F35) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F36) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F37) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F38) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F39) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F3A) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F3B) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F3C) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F3D) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F3E && codepoint <= 0x0F3F) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F40 && codepoint <= 0x0F47) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F49 && codepoint <= 0x0F6C) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F71 && codepoint <= 0x0F7E) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F7F) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F80 && codepoint <= 0x0F84) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0F85) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F86 && codepoint <= 0x0F87) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F88 && codepoint <= 0x0F8C) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F8D && codepoint <= 0x0F97) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0F99 && codepoint <= 0x0FBC) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0FBE && codepoint <= 0x0FC5) return Ucd_Script_TIBETAN;
	if (codepoint == 0x0FC6) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0FC7 && codepoint <= 0x0FCC) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0FCE && codepoint <= 0x0FCF) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0FD0 && codepoint <= 0x0FD4) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x0FD9 && codepoint <= 0x0FDA) return Ucd_Script_TIBETAN;
	if (codepoint >= 0x1000 && codepoint <= 0x102A) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x102B && codepoint <= 0x102C) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x102D && codepoint <= 0x1030) return Ucd_Script_MYANMAR;
	if (codepoint == 0x1031) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1032 && codepoint <= 0x1037) return Ucd_Script_MYANMAR;
	if (codepoint == 0x1038) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1039 && codepoint <= 0x103A) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x103B && codepoint <= 0x103C) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x103D && codepoint <= 0x103E) return Ucd_Script_MYANMAR;
	if (codepoint == 0x103F) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1040 && codepoint <= 0x1049) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x104A && codepoint <= 0x104F) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1050 && codepoint <= 0x1055) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1056 && codepoint <= 0x1057) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1058 && codepoint <= 0x1059) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x105A && codepoint <= 0x105D) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x105E && codepoint <= 0x1060) return Ucd_Script_MYANMAR;
	if (codepoint == 0x1061) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1062 && codepoint <= 0x1064) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1065 && codepoint <= 0x1066) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1067 && codepoint <= 0x106D) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x106E && codepoint <= 0x1070) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1071 && codepoint <= 0x1074) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1075 && codepoint <= 0x1081) return Ucd_Script_MYANMAR;
	if (codepoint == 0x1082) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1083 && codepoint <= 0x1084) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1085 && codepoint <= 0x1086) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1087 && codepoint <= 0x108C) return Ucd_Script_MYANMAR;
	if (codepoint == 0x108D) return Ucd_Script_MYANMAR;
	if (codepoint == 0x108E) return Ucd_Script_MYANMAR;
	if (codepoint == 0x108F) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x1090 && codepoint <= 0x1099) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x109A && codepoint <= 0x109C) return Ucd_Script_MYANMAR;
	if (codepoint == 0x109D) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x109E && codepoint <= 0x109F) return Ucd_Script_MYANMAR;
	if (codepoint >= 0xA9E0 && codepoint <= 0xA9E4) return Ucd_Script_MYANMAR;
	if (codepoint == 0xA9E5) return Ucd_Script_MYANMAR;
	if (codepoint == 0xA9E6) return Ucd_Script_MYANMAR;
	if (codepoint >= 0xA9E7 && codepoint <= 0xA9EF) return Ucd_Script_MYANMAR;
	if (codepoint >= 0xA9F0 && codepoint <= 0xA9F9) return Ucd_Script_MYANMAR;
	if (codepoint >= 0xA9FA && codepoint <= 0xA9FE) return Ucd_Script_MYANMAR;
	if (codepoint >= 0xAA60 && codepoint <= 0xAA6F) return Ucd_Script_MYANMAR;
	if (codepoint == 0xAA70) return Ucd_Script_MYANMAR;
	if (codepoint >= 0xAA71 && codepoint <= 0xAA76) return Ucd_Script_MYANMAR;
	if (codepoint >= 0xAA77 && codepoint <= 0xAA79) return Ucd_Script_MYANMAR;
	if (codepoint == 0xAA7A) return Ucd_Script_MYANMAR;
	if (codepoint == 0xAA7B) return Ucd_Script_MYANMAR;
	if (codepoint == 0xAA7C) return Ucd_Script_MYANMAR;
	if (codepoint == 0xAA7D) return Ucd_Script_MYANMAR;
	if (codepoint >= 0xAA7E && codepoint <= 0xAA7F) return Ucd_Script_MYANMAR;
	if (codepoint >= 0x10A0 && codepoint <= 0x10C5) return Ucd_Script_GEORGIAN;
	if (codepoint == 0x10C7) return Ucd_Script_GEORGIAN;
	if (codepoint == 0x10CD) return Ucd_Script_GEORGIAN;
	if (codepoint >= 0x10D0 && codepoint <= 0x10FA) return Ucd_Script_GEORGIAN;
	if (codepoint == 0x10FC) return Ucd_Script_GEORGIAN;
	if (codepoint >= 0x10FD && codepoint <= 0x10FF) return Ucd_Script_GEORGIAN;
	if (codepoint >= 0x1C90 && codepoint <= 0x1CBA) return Ucd_Script_GEORGIAN;
	if (codepoint >= 0x1CBD && codepoint <= 0x1CBF) return Ucd_Script_GEORGIAN;
	if (codepoint >= 0x2D00 && codepoint <= 0x2D25) return Ucd_Script_GEORGIAN;
	if (codepoint == 0x2D27) return Ucd_Script_GEORGIAN;
	if (codepoint == 0x2D2D) return Ucd_Script_GEORGIAN;
	if (codepoint >= 0x1100 && codepoint <= 0x11FF) return Ucd_Script_HANGUL;
	if (codepoint >= 0x302E && codepoint <= 0x302F) return Ucd_Script_HANGUL;
	if (codepoint >= 0x3131 && codepoint <= 0x318E) return Ucd_Script_HANGUL;
	if (codepoint >= 0x3200 && codepoint <= 0x321E) return Ucd_Script_HANGUL;
	if (codepoint >= 0x3260 && codepoint <= 0x327E) return Ucd_Script_HANGUL;
	if (codepoint >= 0xA960 && codepoint <= 0xA97C) return Ucd_Script_HANGUL;
	if (codepoint >= 0xAC00 && codepoint <= 0xD7A3) return Ucd_Script_HANGUL;
	if (codepoint >= 0xD7B0 && codepoint <= 0xD7C6) return Ucd_Script_HANGUL;
	if (codepoint >= 0xD7CB && codepoint <= 0xD7FB) return Ucd_Script_HANGUL;
	if (codepoint >= 0xFFA0 && codepoint <= 0xFFBE) return Ucd_Script_HANGUL;
	if (codepoint >= 0xFFC2 && codepoint <= 0xFFC7) return Ucd_Script_HANGUL;
	if (codepoint >= 0xFFCA && codepoint <= 0xFFCF) return Ucd_Script_HANGUL;
	if (codepoint >= 0xFFD2 && codepoint <= 0xFFD7) return Ucd_Script_HANGUL;
	if (codepoint >= 0xFFDA && codepoint <= 0xFFDC) return Ucd_Script_HANGUL;
	if (codepoint >= 0x1200 && codepoint <= 0x1248) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x124A && codepoint <= 0x124D) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1250 && codepoint <= 0x1256) return Ucd_Script_ETHIOPIC;
	if (codepoint == 0x1258) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x125A && codepoint <= 0x125D) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1260 && codepoint <= 0x1288) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x128A && codepoint <= 0x128D) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1290 && codepoint <= 0x12B0) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x12B2 && codepoint <= 0x12B5) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x12B8 && codepoint <= 0x12BE) return Ucd_Script_ETHIOPIC;
	if (codepoint == 0x12C0) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x12C2 && codepoint <= 0x12C5) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x12C8 && codepoint <= 0x12D6) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x12D8 && codepoint <= 0x1310) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1312 && codepoint <= 0x1315) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1318 && codepoint <= 0x135A) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x135D && codepoint <= 0x135F) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1360 && codepoint <= 0x1368) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1369 && codepoint <= 0x137C) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1380 && codepoint <= 0x138F) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x1390 && codepoint <= 0x1399) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2D80 && codepoint <= 0x2D96) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2DA0 && codepoint <= 0x2DA6) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2DA8 && codepoint <= 0x2DAE) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2DB0 && codepoint <= 0x2DB6) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2DB8 && codepoint <= 0x2DBE) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2DC0 && codepoint <= 0x2DC6) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2DC8 && codepoint <= 0x2DCE) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2DD0 && codepoint <= 0x2DD6) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x2DD8 && codepoint <= 0x2DDE) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0xAB01 && codepoint <= 0xAB06) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0xAB09 && codepoint <= 0xAB0E) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0xAB11 && codepoint <= 0xAB16) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0xAB20 && codepoint <= 0xAB26) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0xAB28 && codepoint <= 0xAB2E) return Ucd_Script_ETHIOPIC;
	if (codepoint >= 0x13A0 && codepoint <= 0x13F5) return Ucd_Script_CHEROKEE;
	if (codepoint >= 0x13F8 && codepoint <= 0x13FD) return Ucd_Script_CHEROKEE;
	if (codepoint >= 0xAB70 && codepoint <= 0xABBF) return Ucd_Script_CHEROKEE;
	if (codepoint == 0x1400) return Ucd_Script_CANADIAN_ABORIGINAL;
	if (codepoint >= 0x1401 && codepoint <= 0x166C) return Ucd_Script_CANADIAN_ABORIGINAL;
	if (codepoint == 0x166D) return Ucd_Script_CANADIAN_ABORIGINAL;
	if (codepoint == 0x166E) return Ucd_Script_CANADIAN_ABORIGINAL;
	if (codepoint >= 0x166F && codepoint <= 0x167F) return Ucd_Script_CANADIAN_ABORIGINAL;
	if (codepoint >= 0x18B0 && codepoint <= 0x18F5) return Ucd_Script_CANADIAN_ABORIGINAL;
	if (codepoint == 0x1680) return Ucd_Script_OGHAM;
	if (codepoint >= 0x1681 && codepoint <= 0x169A) return Ucd_Script_OGHAM;
	if (codepoint == 0x169B) return Ucd_Script_OGHAM;
	if (codepoint == 0x169C) return Ucd_Script_OGHAM;
	if (codepoint >= 0x16A0 && codepoint <= 0x16EA) return Ucd_Script_RUNIC;
	if (codepoint >= 0x16EE && codepoint <= 0x16F0) return Ucd_Script_RUNIC;
	if (codepoint >= 0x16F1 && codepoint <= 0x16F8) return Ucd_Script_RUNIC;
	if (codepoint >= 0x1780 && codepoint <= 0x17B3) return Ucd_Script_KHMER;
	if (codepoint >= 0x17B4 && codepoint <= 0x17B5) return Ucd_Script_KHMER;
	if (codepoint == 0x17B6) return Ucd_Script_KHMER;
	if (codepoint >= 0x17B7 && codepoint <= 0x17BD) return Ucd_Script_KHMER;
	if (codepoint >= 0x17BE && codepoint <= 0x17C5) return Ucd_Script_KHMER;
	if (codepoint == 0x17C6) return Ucd_Script_KHMER;
	if (codepoint >= 0x17C7 && codepoint <= 0x17C8) return Ucd_Script_KHMER;
	if (codepoint >= 0x17C9 && codepoint <= 0x17D3) return Ucd_Script_KHMER;
	if (codepoint >= 0x17D4 && codepoint <= 0x17D6) return Ucd_Script_KHMER;
	if (codepoint == 0x17D7) return Ucd_Script_KHMER;
	if (codepoint >= 0x17D8 && codepoint <= 0x17DA) return Ucd_Script_KHMER;
	if (codepoint == 0x17DB) return Ucd_Script_KHMER;
	if (codepoint == 0x17DC) return Ucd_Script_KHMER;
	if (codepoint == 0x17DD) return Ucd_Script_KHMER;
	if (codepoint >= 0x17E0 && codepoint <= 0x17E9) return Ucd_Script_KHMER;
	if (codepoint >= 0x17F0 && codepoint <= 0x17F9) return Ucd_Script_KHMER;
	if (codepoint >= 0x19E0 && codepoint <= 0x19FF) return Ucd_Script_KHMER;
	if (codepoint >= 0x1800 && codepoint <= 0x1801) return Ucd_Script_MONGOLIAN;
	if (codepoint == 0x1804) return Ucd_Script_MONGOLIAN;
	if (codepoint == 0x1806) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x1807 && codepoint <= 0x180A) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x180B && codepoint <= 0x180D) return Ucd_Script_MONGOLIAN;
	if (codepoint == 0x180E) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x1810 && codepoint <= 0x1819) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x1820 && codepoint <= 0x1842) return Ucd_Script_MONGOLIAN;
	if (codepoint == 0x1843) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x1844 && codepoint <= 0x1878) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x1880 && codepoint <= 0x1884) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x1885 && codepoint <= 0x1886) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x1887 && codepoint <= 0x18A8) return Ucd_Script_MONGOLIAN;
	if (codepoint == 0x18A9) return Ucd_Script_MONGOLIAN;
	if (codepoint == 0x18AA) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x11660 && codepoint <= 0x1166C) return Ucd_Script_MONGOLIAN;
	if (codepoint >= 0x3041 && codepoint <= 0x3096) return Ucd_Script_HIRAGANA;
	if (codepoint >= 0x309D && codepoint <= 0x309E) return Ucd_Script_HIRAGANA;
	if (codepoint == 0x309F) return Ucd_Script_HIRAGANA;
	if (codepoint >= 0x1B001 && codepoint <= 0x1B11E) return Ucd_Script_HIRAGANA;
	if (codepoint >= 0x1B150 && codepoint <= 0x1B152) return Ucd_Script_HIRAGANA;
	if (codepoint == 0x1F200) return Ucd_Script_HIRAGANA;
	if (codepoint >= 0x30A1 && codepoint <= 0x30FA) return Ucd_Script_KATAKANA;
	if (codepoint >= 0x30FD && codepoint <= 0x30FE) return Ucd_Script_KATAKANA;
	if (codepoint == 0x30FF) return Ucd_Script_KATAKANA;
	if (codepoint >= 0x31F0 && codepoint <= 0x31FF) return Ucd_Script_KATAKANA;
	if (codepoint >= 0x32D0 && codepoint <= 0x32FE) return Ucd_Script_KATAKANA;
	if (codepoint >= 0x3300 && codepoint <= 0x3357) return Ucd_Script_KATAKANA;
	if (codepoint >= 0xFF66 && codepoint <= 0xFF6F) return Ucd_Script_KATAKANA;
	if (codepoint >= 0xFF71 && codepoint <= 0xFF9D) return Ucd_Script_KATAKANA;
	if (codepoint == 0x1B000) return Ucd_Script_KATAKANA;
	if (codepoint >= 0x1B164 && codepoint <= 0x1B167) return Ucd_Script_KATAKANA;
	if (codepoint >= 0x02EA && codepoint <= 0x02EB) return Ucd_Script_BOPOMOFO;
	if (codepoint >= 0x3105 && codepoint <= 0x312F) return Ucd_Script_BOPOMOFO;
	if (codepoint >= 0x31A0 && codepoint <= 0x31BF) return Ucd_Script_BOPOMOFO;
	if (codepoint >= 0x2E80 && codepoint <= 0x2E99) return Ucd_Script_HAN;
	if (codepoint >= 0x2E9B && codepoint <= 0x2EF3) return Ucd_Script_HAN;
	if (codepoint >= 0x2F00 && codepoint <= 0x2FD5) return Ucd_Script_HAN;
	if (codepoint == 0x3005) return Ucd_Script_HAN;
	if (codepoint == 0x3007) return Ucd_Script_HAN;
	if (codepoint >= 0x3021 && codepoint <= 0x3029) return Ucd_Script_HAN;
	if (codepoint >= 0x3038 && codepoint <= 0x303A) return Ucd_Script_HAN;
	if (codepoint == 0x303B) return Ucd_Script_HAN;
	if (codepoint >= 0x3400 && codepoint <= 0x4DBF) return Ucd_Script_HAN;
	if (codepoint >= 0x4E00 && codepoint <= 0x9FFC) return Ucd_Script_HAN;
	if (codepoint >= 0xF900 && codepoint <= 0xFA6D) return Ucd_Script_HAN;
	if (codepoint >= 0xFA70 && codepoint <= 0xFAD9) return Ucd_Script_HAN;
	if (codepoint >= 0x16FF0 && codepoint <= 0x16FF1) return Ucd_Script_HAN;
	if (codepoint >= 0x20000 && codepoint <= 0x2A6DD) return Ucd_Script_HAN;
	if (codepoint >= 0x2A700 && codepoint <= 0x2B734) return Ucd_Script_HAN;
	if (codepoint >= 0x2B740 && codepoint <= 0x2B81D) return Ucd_Script_HAN;
	if (codepoint >= 0x2B820 && codepoint <= 0x2CEA1) return Ucd_Script_HAN;
	if (codepoint >= 0x2CEB0 && codepoint <= 0x2EBE0) return Ucd_Script_HAN;
	if (codepoint >= 0x2F800 && codepoint <= 0x2FA1D) return Ucd_Script_HAN;
	if (codepoint >= 0x30000 && codepoint <= 0x3134A) return Ucd_Script_HAN;
	if (codepoint >= 0xA000 && codepoint <= 0xA014) return Ucd_Script_YI;
	if (codepoint == 0xA015) return Ucd_Script_YI;
	if (codepoint >= 0xA016 && codepoint <= 0xA48C) return Ucd_Script_YI;
	if (codepoint >= 0xA490 && codepoint <= 0xA4C6) return Ucd_Script_YI;
	if (codepoint >= 0x10300 && codepoint <= 0x1031F) return Ucd_Script_OLD_ITALIC;
	if (codepoint >= 0x10320 && codepoint <= 0x10323) return Ucd_Script_OLD_ITALIC;
	if (codepoint >= 0x1032D && codepoint <= 0x1032F) return Ucd_Script_OLD_ITALIC;
	if (codepoint >= 0x10330 && codepoint <= 0x10340) return Ucd_Script_GOTHIC;
	if (codepoint == 0x10341) return Ucd_Script_GOTHIC;
	if (codepoint >= 0x10342 && codepoint <= 0x10349) return Ucd_Script_GOTHIC;
	if (codepoint == 0x1034A) return Ucd_Script_GOTHIC;
	if (codepoint >= 0x10400 && codepoint <= 0x1044F) return Ucd_Script_DESERET;
	if (codepoint >= 0x0300 && codepoint <= 0x036F) return Ucd_Script_INHERITED;
	if (codepoint >= 0x0485 && codepoint <= 0x0486) return Ucd_Script_INHERITED;
	if (codepoint >= 0x064B && codepoint <= 0x0655) return Ucd_Script_INHERITED;
	if (codepoint == 0x0670) return Ucd_Script_INHERITED;
	if (codepoint >= 0x0951 && codepoint <= 0x0954) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1AB0 && codepoint <= 0x1ABD) return Ucd_Script_INHERITED;
	if (codepoint == 0x1ABE) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1ABF && codepoint <= 0x1AC0) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1CD0 && codepoint <= 0x1CD2) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1CD4 && codepoint <= 0x1CE0) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1CE2 && codepoint <= 0x1CE8) return Ucd_Script_INHERITED;
	if (codepoint == 0x1CED) return Ucd_Script_INHERITED;
	if (codepoint == 0x1CF4) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1CF8 && codepoint <= 0x1CF9) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1DC0 && codepoint <= 0x1DF9) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1DFB && codepoint <= 0x1DFF) return Ucd_Script_INHERITED;
	if (codepoint >= 0x200C && codepoint <= 0x200D) return Ucd_Script_INHERITED;
	if (codepoint >= 0x20D0 && codepoint <= 0x20DC) return Ucd_Script_INHERITED;
	if (codepoint >= 0x20DD && codepoint <= 0x20E0) return Ucd_Script_INHERITED;
	if (codepoint == 0x20E1) return Ucd_Script_INHERITED;
	if (codepoint >= 0x20E2 && codepoint <= 0x20E4) return Ucd_Script_INHERITED;
	if (codepoint >= 0x20E5 && codepoint <= 0x20F0) return Ucd_Script_INHERITED;
	if (codepoint >= 0x302A && codepoint <= 0x302D) return Ucd_Script_INHERITED;
	if (codepoint >= 0x3099 && codepoint <= 0x309A) return Ucd_Script_INHERITED;
	if (codepoint >= 0xFE00 && codepoint <= 0xFE0F) return Ucd_Script_INHERITED;
	if (codepoint >= 0xFE20 && codepoint <= 0xFE2D) return Ucd_Script_INHERITED;
	if (codepoint == 0x101FD) return Ucd_Script_INHERITED;
	if (codepoint == 0x102E0) return Ucd_Script_INHERITED;
	if (codepoint == 0x1133B) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1D167 && codepoint <= 0x1D169) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1D17B && codepoint <= 0x1D182) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1D185 && codepoint <= 0x1D18B) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1D1AA && codepoint <= 0x1D1AD) return Ucd_Script_INHERITED;
	if (codepoint >= 0xE0100 && codepoint <= 0xE01EF) return Ucd_Script_INHERITED;
	if (codepoint >= 0x1700 && codepoint <= 0x170C) return Ucd_Script_TAGALOG;
	if (codepoint >= 0x170E && codepoint <= 0x1711) return Ucd_Script_TAGALOG;
	if (codepoint >= 0x1712 && codepoint <= 0x1714) return Ucd_Script_TAGALOG;
	if (codepoint >= 0x1720 && codepoint <= 0x1731) return Ucd_Script_HANUNOO;
	if (codepoint >= 0x1732 && codepoint <= 0x1734) return Ucd_Script_HANUNOO;
	if (codepoint >= 0x1740 && codepoint <= 0x1751) return Ucd_Script_BUHID;
	if (codepoint >= 0x1752 && codepoint <= 0x1753) return Ucd_Script_BUHID;
	if (codepoint >= 0x1760 && codepoint <= 0x176C) return Ucd_Script_TAGBANWA;
	if (codepoint >= 0x176E && codepoint <= 0x1770) return Ucd_Script_TAGBANWA;
	if (codepoint >= 0x1772 && codepoint <= 0x1773) return Ucd_Script_TAGBANWA;
	if (codepoint >= 0x1900 && codepoint <= 0x191E) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1920 && codepoint <= 0x1922) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1923 && codepoint <= 0x1926) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1927 && codepoint <= 0x1928) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1929 && codepoint <= 0x192B) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1930 && codepoint <= 0x1931) return Ucd_Script_LIMBU;
	if (codepoint == 0x1932) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1933 && codepoint <= 0x1938) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1939 && codepoint <= 0x193B) return Ucd_Script_LIMBU;
	if (codepoint == 0x1940) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1944 && codepoint <= 0x1945) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1946 && codepoint <= 0x194F) return Ucd_Script_LIMBU;
	if (codepoint >= 0x1950 && codepoint <= 0x196D) return Ucd_Script_TAI_LE;
	if (codepoint >= 0x1970 && codepoint <= 0x1974) return Ucd_Script_TAI_LE;
	if (codepoint >= 0x10000 && codepoint <= 0x1000B) return Ucd_Script_LINEAR_B;
	if (codepoint >= 0x1000D && codepoint <= 0x10026) return Ucd_Script_LINEAR_B;
	if (codepoint >= 0x10028 && codepoint <= 0x1003A) return Ucd_Script_LINEAR_B;
	if (codepoint >= 0x1003C && codepoint <= 0x1003D) return Ucd_Script_LINEAR_B;
	if (codepoint >= 0x1003F && codepoint <= 0x1004D) return Ucd_Script_LINEAR_B;
	if (codepoint >= 0x10050 && codepoint <= 0x1005D) return Ucd_Script_LINEAR_B;
	if (codepoint >= 0x10080 && codepoint <= 0x100FA) return Ucd_Script_LINEAR_B;
	if (codepoint >= 0x10380 && codepoint <= 0x1039D) return Ucd_Script_UGARITIC;
	if (codepoint == 0x1039F) return Ucd_Script_UGARITIC;
	if (codepoint >= 0x10450 && codepoint <= 0x1047F) return Ucd_Script_SHAVIAN;
	if (codepoint >= 0x10480 && codepoint <= 0x1049D) return Ucd_Script_OSMANYA;
	if (codepoint >= 0x104A0 && codepoint <= 0x104A9) return Ucd_Script_OSMANYA;
	if (codepoint >= 0x10800 && codepoint <= 0x10805) return Ucd_Script_CYPRIOT;
	if (codepoint == 0x10808) return Ucd_Script_CYPRIOT;
	if (codepoint >= 0x1080A && codepoint <= 0x10835) return Ucd_Script_CYPRIOT;
	if (codepoint >= 0x10837 && codepoint <= 0x10838) return Ucd_Script_CYPRIOT;
	if (codepoint == 0x1083C) return Ucd_Script_CYPRIOT;
	if (codepoint == 0x1083F) return Ucd_Script_CYPRIOT;
	if (codepoint >= 0x2800 && codepoint <= 0x28FF) return Ucd_Script_BRAILLE;
	if (codepoint >= 0x1A00 && codepoint <= 0x1A16) return Ucd_Script_BUGINESE;
	if (codepoint >= 0x1A17 && codepoint <= 0x1A18) return Ucd_Script_BUGINESE;
	if (codepoint >= 0x1A19 && codepoint <= 0x1A1A) return Ucd_Script_BUGINESE;
	if (codepoint == 0x1A1B) return Ucd_Script_BUGINESE;
	if (codepoint >= 0x1A1E && codepoint <= 0x1A1F) return Ucd_Script_BUGINESE;
	if (codepoint >= 0x03E2 && codepoint <= 0x03EF) return Ucd_Script_COPTIC;
	if (codepoint >= 0x2C80 && codepoint <= 0x2CE4) return Ucd_Script_COPTIC;
	if (codepoint >= 0x2CE5 && codepoint <= 0x2CEA) return Ucd_Script_COPTIC;
	if (codepoint >= 0x2CEB && codepoint <= 0x2CEE) return Ucd_Script_COPTIC;
	if (codepoint >= 0x2CEF && codepoint <= 0x2CF1) return Ucd_Script_COPTIC;
	if (codepoint >= 0x2CF2 && codepoint <= 0x2CF3) return Ucd_Script_COPTIC;
	if (codepoint >= 0x2CF9 && codepoint <= 0x2CFC) return Ucd_Script_COPTIC;
	if (codepoint == 0x2CFD) return Ucd_Script_COPTIC;
	if (codepoint >= 0x2CFE && codepoint <= 0x2CFF) return Ucd_Script_COPTIC;
	if (codepoint >= 0x1980 && codepoint <= 0x19AB) return Ucd_Script_NEW_TAI_LUE;
	if (codepoint >= 0x19B0 && codepoint <= 0x19C9) return Ucd_Script_NEW_TAI_LUE;
	if (codepoint >= 0x19D0 && codepoint <= 0x19D9) return Ucd_Script_NEW_TAI_LUE;
	if (codepoint == 0x19DA) return Ucd_Script_NEW_TAI_LUE;
	if (codepoint >= 0x19DE && codepoint <= 0x19DF) return Ucd_Script_NEW_TAI_LUE;
	if (codepoint >= 0x2C00 && codepoint <= 0x2C2E) return Ucd_Script_GLAGOLITIC;
	if (codepoint >= 0x2C30 && codepoint <= 0x2C5E) return Ucd_Script_GLAGOLITIC;
	if (codepoint >= 0x1E000 && codepoint <= 0x1E006) return Ucd_Script_GLAGOLITIC;
	if (codepoint >= 0x1E008 && codepoint <= 0x1E018) return Ucd_Script_GLAGOLITIC;
	if (codepoint >= 0x1E01B && codepoint <= 0x1E021) return Ucd_Script_GLAGOLITIC;
	if (codepoint >= 0x1E023 && codepoint <= 0x1E024) return Ucd_Script_GLAGOLITIC;
	if (codepoint >= 0x1E026 && codepoint <= 0x1E02A) return Ucd_Script_GLAGOLITIC;
	if (codepoint >= 0x2D30 && codepoint <= 0x2D67) return Ucd_Script_TIFINAGH;
	if (codepoint == 0x2D6F) return Ucd_Script_TIFINAGH;
	if (codepoint == 0x2D70) return Ucd_Script_TIFINAGH;
	if (codepoint == 0x2D7F) return Ucd_Script_TIFINAGH;
	if (codepoint >= 0xA800 && codepoint <= 0xA801) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint == 0xA802) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint >= 0xA803 && codepoint <= 0xA805) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint == 0xA806) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint >= 0xA807 && codepoint <= 0xA80A) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint == 0xA80B) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint >= 0xA80C && codepoint <= 0xA822) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint >= 0xA823 && codepoint <= 0xA824) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint >= 0xA825 && codepoint <= 0xA826) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint == 0xA827) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint >= 0xA828 && codepoint <= 0xA82B) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint == 0xA82C) return Ucd_Script_SYLOTI_NAGRI;
	if (codepoint >= 0x103A0 && codepoint <= 0x103C3) return Ucd_Script_OLD_PERSIAN;
	if (codepoint >= 0x103C8 && codepoint <= 0x103CF) return Ucd_Script_OLD_PERSIAN;
	if (codepoint == 0x103D0) return Ucd_Script_OLD_PERSIAN;
	if (codepoint >= 0x103D1 && codepoint <= 0x103D5) return Ucd_Script_OLD_PERSIAN;
	if (codepoint == 0x10A00) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A01 && codepoint <= 0x10A03) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A05 && codepoint <= 0x10A06) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A0C && codepoint <= 0x10A0F) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A10 && codepoint <= 0x10A13) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A15 && codepoint <= 0x10A17) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A19 && codepoint <= 0x10A35) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A38 && codepoint <= 0x10A3A) return Ucd_Script_KHAROSHTHI;
	if (codepoint == 0x10A3F) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A40 && codepoint <= 0x10A48) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x10A50 && codepoint <= 0x10A58) return Ucd_Script_KHAROSHTHI;
	if (codepoint >= 0x1B00 && codepoint <= 0x1B03) return Ucd_Script_BALINESE;
	if (codepoint == 0x1B04) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B05 && codepoint <= 0x1B33) return Ucd_Script_BALINESE;
	if (codepoint == 0x1B34) return Ucd_Script_BALINESE;
	if (codepoint == 0x1B35) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B36 && codepoint <= 0x1B3A) return Ucd_Script_BALINESE;
	if (codepoint == 0x1B3B) return Ucd_Script_BALINESE;
	if (codepoint == 0x1B3C) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B3D && codepoint <= 0x1B41) return Ucd_Script_BALINESE;
	if (codepoint == 0x1B42) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B43 && codepoint <= 0x1B44) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B45 && codepoint <= 0x1B4B) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B50 && codepoint <= 0x1B59) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B5A && codepoint <= 0x1B60) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B61 && codepoint <= 0x1B6A) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B6B && codepoint <= 0x1B73) return Ucd_Script_BALINESE;
	if (codepoint >= 0x1B74 && codepoint <= 0x1B7C) return Ucd_Script_BALINESE;
	if (codepoint >= 0x12000 && codepoint <= 0x12399) return Ucd_Script_CUNEIFORM;
	if (codepoint >= 0x12400 && codepoint <= 0x1246E) return Ucd_Script_CUNEIFORM;
	if (codepoint >= 0x12470 && codepoint <= 0x12474) return Ucd_Script_CUNEIFORM;
	if (codepoint >= 0x12480 && codepoint <= 0x12543) return Ucd_Script_CUNEIFORM;
	if (codepoint >= 0x10900 && codepoint <= 0x10915) return Ucd_Script_PHOENICIAN;
	if (codepoint >= 0x10916 && codepoint <= 0x1091B) return Ucd_Script_PHOENICIAN;
	if (codepoint == 0x1091F) return Ucd_Script_PHOENICIAN;
	if (codepoint >= 0xA840 && codepoint <= 0xA873) return Ucd_Script_PHAGS_PA;
	if (codepoint >= 0xA874 && codepoint <= 0xA877) return Ucd_Script_PHAGS_PA;
	if (codepoint >= 0x07C0 && codepoint <= 0x07C9) return Ucd_Script_NKO;
	if (codepoint >= 0x07CA && codepoint <= 0x07EA) return Ucd_Script_NKO;
	if (codepoint >= 0x07EB && codepoint <= 0x07F3) return Ucd_Script_NKO;
	if (codepoint >= 0x07F4 && codepoint <= 0x07F5) return Ucd_Script_NKO;
	if (codepoint == 0x07F6) return Ucd_Script_NKO;
	if (codepoint >= 0x07F7 && codepoint <= 0x07F9) return Ucd_Script_NKO;
	if (codepoint == 0x07FA) return Ucd_Script_NKO;
	if (codepoint == 0x07FD) return Ucd_Script_NKO;
	if (codepoint >= 0x07FE && codepoint <= 0x07FF) return Ucd_Script_NKO;
	if (codepoint >= 0x1B80 && codepoint <= 0x1B81) return Ucd_Script_SUNDANESE;
	if (codepoint == 0x1B82) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1B83 && codepoint <= 0x1BA0) return Ucd_Script_SUNDANESE;
	if (codepoint == 0x1BA1) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1BA2 && codepoint <= 0x1BA5) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1BA6 && codepoint <= 0x1BA7) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1BA8 && codepoint <= 0x1BA9) return Ucd_Script_SUNDANESE;
	if (codepoint == 0x1BAA) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1BAB && codepoint <= 0x1BAD) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1BAE && codepoint <= 0x1BAF) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1BB0 && codepoint <= 0x1BB9) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1BBA && codepoint <= 0x1BBF) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1CC0 && codepoint <= 0x1CC7) return Ucd_Script_SUNDANESE;
	if (codepoint >= 0x1C00 && codepoint <= 0x1C23) return Ucd_Script_LEPCHA;
	if (codepoint >= 0x1C24 && codepoint <= 0x1C2B) return Ucd_Script_LEPCHA;
	if (codepoint >= 0x1C2C && codepoint <= 0x1C33) return Ucd_Script_LEPCHA;
	if (codepoint >= 0x1C34 && codepoint <= 0x1C35) return Ucd_Script_LEPCHA;
	if (codepoint >= 0x1C36 && codepoint <= 0x1C37) return Ucd_Script_LEPCHA;
	if (codepoint >= 0x1C3B && codepoint <= 0x1C3F) return Ucd_Script_LEPCHA;
	if (codepoint >= 0x1C40 && codepoint <= 0x1C49) return Ucd_Script_LEPCHA;
	if (codepoint >= 0x1C4D && codepoint <= 0x1C4F) return Ucd_Script_LEPCHA;
	if (codepoint >= 0x1C50 && codepoint <= 0x1C59) return Ucd_Script_OL_CHIKI;
	if (codepoint >= 0x1C5A && codepoint <= 0x1C77) return Ucd_Script_OL_CHIKI;
	if (codepoint >= 0x1C78 && codepoint <= 0x1C7D) return Ucd_Script_OL_CHIKI;
	if (codepoint >= 0x1C7E && codepoint <= 0x1C7F) return Ucd_Script_OL_CHIKI;
	if (codepoint >= 0xA500 && codepoint <= 0xA60B) return Ucd_Script_VAI;
	if (codepoint == 0xA60C) return Ucd_Script_VAI;
	if (codepoint >= 0xA60D && codepoint <= 0xA60F) return Ucd_Script_VAI;
	if (codepoint >= 0xA610 && codepoint <= 0xA61F) return Ucd_Script_VAI;
	if (codepoint >= 0xA620 && codepoint <= 0xA629) return Ucd_Script_VAI;
	if (codepoint >= 0xA62A && codepoint <= 0xA62B) return Ucd_Script_VAI;
	if (codepoint >= 0xA880 && codepoint <= 0xA881) return Ucd_Script_SAURASHTRA;
	if (codepoint >= 0xA882 && codepoint <= 0xA8B3) return Ucd_Script_SAURASHTRA;
	if (codepoint >= 0xA8B4 && codepoint <= 0xA8C3) return Ucd_Script_SAURASHTRA;
	if (codepoint >= 0xA8C4 && codepoint <= 0xA8C5) return Ucd_Script_SAURASHTRA;
	if (codepoint >= 0xA8CE && codepoint <= 0xA8CF) return Ucd_Script_SAURASHTRA;
	if (codepoint >= 0xA8D0 && codepoint <= 0xA8D9) return Ucd_Script_SAURASHTRA;
	if (codepoint >= 0xA900 && codepoint <= 0xA909) return Ucd_Script_KAYAH_LI;
	if (codepoint >= 0xA90A && codepoint <= 0xA925) return Ucd_Script_KAYAH_LI;
	if (codepoint >= 0xA926 && codepoint <= 0xA92D) return Ucd_Script_KAYAH_LI;
	if (codepoint == 0xA92F) return Ucd_Script_KAYAH_LI;
	if (codepoint >= 0xA930 && codepoint <= 0xA946) return Ucd_Script_REJANG;
	if (codepoint >= 0xA947 && codepoint <= 0xA951) return Ucd_Script_REJANG;
	if (codepoint >= 0xA952 && codepoint <= 0xA953) return Ucd_Script_REJANG;
	if (codepoint == 0xA95F) return Ucd_Script_REJANG;
	if (codepoint >= 0x10280 && codepoint <= 0x1029C) return Ucd_Script_LYCIAN;
	if (codepoint >= 0x102A0 && codepoint <= 0x102D0) return Ucd_Script_CARIAN;
	if (codepoint >= 0x10920 && codepoint <= 0x10939) return Ucd_Script_LYDIAN;
	if (codepoint == 0x1093F) return Ucd_Script_LYDIAN;
	if (codepoint >= 0xAA00 && codepoint <= 0xAA28) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA29 && codepoint <= 0xAA2E) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA2F && codepoint <= 0xAA30) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA31 && codepoint <= 0xAA32) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA33 && codepoint <= 0xAA34) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA35 && codepoint <= 0xAA36) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA40 && codepoint <= 0xAA42) return Ucd_Script_CHAM;
	if (codepoint == 0xAA43) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA44 && codepoint <= 0xAA4B) return Ucd_Script_CHAM;
	if (codepoint == 0xAA4C) return Ucd_Script_CHAM;
	if (codepoint == 0xAA4D) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA50 && codepoint <= 0xAA59) return Ucd_Script_CHAM;
	if (codepoint >= 0xAA5C && codepoint <= 0xAA5F) return Ucd_Script_CHAM;
	if (codepoint >= 0x1A20 && codepoint <= 0x1A54) return Ucd_Script_TAI_THAM;
	if (codepoint == 0x1A55) return Ucd_Script_TAI_THAM;
	if (codepoint == 0x1A56) return Ucd_Script_TAI_THAM;
	if (codepoint == 0x1A57) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1A58 && codepoint <= 0x1A5E) return Ucd_Script_TAI_THAM;
	if (codepoint == 0x1A60) return Ucd_Script_TAI_THAM;
	if (codepoint == 0x1A61) return Ucd_Script_TAI_THAM;
	if (codepoint == 0x1A62) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1A63 && codepoint <= 0x1A64) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1A65 && codepoint <= 0x1A6C) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1A6D && codepoint <= 0x1A72) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1A73 && codepoint <= 0x1A7C) return Ucd_Script_TAI_THAM;
	if (codepoint == 0x1A7F) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1A80 && codepoint <= 0x1A89) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1A90 && codepoint <= 0x1A99) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1AA0 && codepoint <= 0x1AA6) return Ucd_Script_TAI_THAM;
	if (codepoint == 0x1AA7) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0x1AA8 && codepoint <= 0x1AAD) return Ucd_Script_TAI_THAM;
	if (codepoint >= 0xAA80 && codepoint <= 0xAAAF) return Ucd_Script_TAI_VIET;
	if (codepoint == 0xAAB0) return Ucd_Script_TAI_VIET;
	if (codepoint == 0xAAB1) return Ucd_Script_TAI_VIET;
	if (codepoint >= 0xAAB2 && codepoint <= 0xAAB4) return Ucd_Script_TAI_VIET;
	if (codepoint >= 0xAAB5 && codepoint <= 0xAAB6) return Ucd_Script_TAI_VIET;
	if (codepoint >= 0xAAB7 && codepoint <= 0xAAB8) return Ucd_Script_TAI_VIET;
	if (codepoint >= 0xAAB9 && codepoint <= 0xAABD) return Ucd_Script_TAI_VIET;
	if (codepoint >= 0xAABE && codepoint <= 0xAABF) return Ucd_Script_TAI_VIET;
	if (codepoint == 0xAAC0) return Ucd_Script_TAI_VIET;
	if (codepoint == 0xAAC1) return Ucd_Script_TAI_VIET;
	if (codepoint == 0xAAC2) return Ucd_Script_TAI_VIET;
	if (codepoint >= 0xAADB && codepoint <= 0xAADC) return Ucd_Script_TAI_VIET;
	if (codepoint == 0xAADD) return Ucd_Script_TAI_VIET;
	if (codepoint >= 0xAADE && codepoint <= 0xAADF) return Ucd_Script_TAI_VIET;
	if (codepoint >= 0x10B00 && codepoint <= 0x10B35) return Ucd_Script_AVESTAN;
	if (codepoint >= 0x10B39 && codepoint <= 0x10B3F) return Ucd_Script_AVESTAN;
	if (codepoint >= 0x13000 && codepoint <= 0x1342E) return Ucd_Script_EGYPTIAN_HIEROGLYPHS;
	if (codepoint >= 0x13430 && codepoint <= 0x13438) return Ucd_Script_EGYPTIAN_HIEROGLYPHS;
	if (codepoint >= 0x0800 && codepoint <= 0x0815) return Ucd_Script_SAMARITAN;
	if (codepoint >= 0x0816 && codepoint <= 0x0819) return Ucd_Script_SAMARITAN;
	if (codepoint == 0x081A) return Ucd_Script_SAMARITAN;
	if (codepoint >= 0x081B && codepoint <= 0x0823) return Ucd_Script_SAMARITAN;
	if (codepoint == 0x0824) return Ucd_Script_SAMARITAN;
	if (codepoint >= 0x0825 && codepoint <= 0x0827) return Ucd_Script_SAMARITAN;
	if (codepoint == 0x0828) return Ucd_Script_SAMARITAN;
	if (codepoint >= 0x0829 && codepoint <= 0x082D) return Ucd_Script_SAMARITAN;
	if (codepoint >= 0x0830 && codepoint <= 0x083E) return Ucd_Script_SAMARITAN;
	if (codepoint >= 0xA4D0 && codepoint <= 0xA4F7) return Ucd_Script_LISU;
	if (codepoint >= 0xA4F8 && codepoint <= 0xA4FD) return Ucd_Script_LISU;
	if (codepoint >= 0xA4FE && codepoint <= 0xA4FF) return Ucd_Script_LISU;
	if (codepoint == 0x11FB0) return Ucd_Script_LISU;
	if (codepoint >= 0xA6A0 && codepoint <= 0xA6E5) return Ucd_Script_BAMUM;
	if (codepoint >= 0xA6E6 && codepoint <= 0xA6EF) return Ucd_Script_BAMUM;
	if (codepoint >= 0xA6F0 && codepoint <= 0xA6F1) return Ucd_Script_BAMUM;
	if (codepoint >= 0xA6F2 && codepoint <= 0xA6F7) return Ucd_Script_BAMUM;
	if (codepoint >= 0x16800 && codepoint <= 0x16A38) return Ucd_Script_BAMUM;
	if (codepoint >= 0xA980 && codepoint <= 0xA982) return Ucd_Script_JAVANESE;
	if (codepoint == 0xA983) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA984 && codepoint <= 0xA9B2) return Ucd_Script_JAVANESE;
	if (codepoint == 0xA9B3) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA9B4 && codepoint <= 0xA9B5) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA9B6 && codepoint <= 0xA9B9) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA9BA && codepoint <= 0xA9BB) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA9BC && codepoint <= 0xA9BD) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA9BE && codepoint <= 0xA9C0) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA9C1 && codepoint <= 0xA9CD) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA9D0 && codepoint <= 0xA9D9) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xA9DE && codepoint <= 0xA9DF) return Ucd_Script_JAVANESE;
	if (codepoint >= 0xAAE0 && codepoint <= 0xAAEA) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xAAEB) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xAAEC && codepoint <= 0xAAED) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xAAEE && codepoint <= 0xAAEF) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xAAF0 && codepoint <= 0xAAF1) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xAAF2) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xAAF3 && codepoint <= 0xAAF4) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xAAF5) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xAAF6) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xABC0 && codepoint <= 0xABE2) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xABE3 && codepoint <= 0xABE4) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xABE5) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xABE6 && codepoint <= 0xABE7) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xABE8) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xABE9 && codepoint <= 0xABEA) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xABEB) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xABEC) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint == 0xABED) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0xABF0 && codepoint <= 0xABF9) return Ucd_Script_MEETEI_MAYEK;
	if (codepoint >= 0x10840 && codepoint <= 0x10855) return Ucd_Script_IMPERIAL_ARAMAIC;
	if (codepoint == 0x10857) return Ucd_Script_IMPERIAL_ARAMAIC;
	if (codepoint >= 0x10858 && codepoint <= 0x1085F) return Ucd_Script_IMPERIAL_ARAMAIC;
	if (codepoint >= 0x10A60 && codepoint <= 0x10A7C) return Ucd_Script_OLD_SOUTH_ARABIAN;
	if (codepoint >= 0x10A7D && codepoint <= 0x10A7E) return Ucd_Script_OLD_SOUTH_ARABIAN;
	if (codepoint == 0x10A7F) return Ucd_Script_OLD_SOUTH_ARABIAN;
	if (codepoint >= 0x10B40 && codepoint <= 0x10B55) return Ucd_Script_INSCRIPTIONAL_PARTHIAN;
	if (codepoint >= 0x10B58 && codepoint <= 0x10B5F) return Ucd_Script_INSCRIPTIONAL_PARTHIAN;
	if (codepoint >= 0x10B60 && codepoint <= 0x10B72) return Ucd_Script_INSCRIPTIONAL_PAHLAVI;
	if (codepoint >= 0x10B78 && codepoint <= 0x10B7F) return Ucd_Script_INSCRIPTIONAL_PAHLAVI;
	if (codepoint >= 0x10C00 && codepoint <= 0x10C48) return Ucd_Script_OLD_TURKIC;
	if (codepoint >= 0x11080 && codepoint <= 0x11081) return Ucd_Script_KAITHI;
	if (codepoint == 0x11082) return Ucd_Script_KAITHI;
	if (codepoint >= 0x11083 && codepoint <= 0x110AF) return Ucd_Script_KAITHI;
	if (codepoint >= 0x110B0 && codepoint <= 0x110B2) return Ucd_Script_KAITHI;
	if (codepoint >= 0x110B3 && codepoint <= 0x110B6) return Ucd_Script_KAITHI;
	if (codepoint >= 0x110B7 && codepoint <= 0x110B8) return Ucd_Script_KAITHI;
	if (codepoint >= 0x110B9 && codepoint <= 0x110BA) return Ucd_Script_KAITHI;
	if (codepoint >= 0x110BB && codepoint <= 0x110BC) return Ucd_Script_KAITHI;
	if (codepoint == 0x110BD) return Ucd_Script_KAITHI;
	if (codepoint >= 0x110BE && codepoint <= 0x110C1) return Ucd_Script_KAITHI;
	if (codepoint == 0x110CD) return Ucd_Script_KAITHI;
	if (codepoint >= 0x1BC0 && codepoint <= 0x1BE5) return Ucd_Script_BATAK;
	if (codepoint == 0x1BE6) return Ucd_Script_BATAK;
	if (codepoint == 0x1BE7) return Ucd_Script_BATAK;
	if (codepoint >= 0x1BE8 && codepoint <= 0x1BE9) return Ucd_Script_BATAK;
	if (codepoint >= 0x1BEA && codepoint <= 0x1BEC) return Ucd_Script_BATAK;
	if (codepoint == 0x1BED) return Ucd_Script_BATAK;
	if (codepoint == 0x1BEE) return Ucd_Script_BATAK;
	if (codepoint >= 0x1BEF && codepoint <= 0x1BF1) return Ucd_Script_BATAK;
	if (codepoint >= 0x1BF2 && codepoint <= 0x1BF3) return Ucd_Script_BATAK;
	if (codepoint >= 0x1BFC && codepoint <= 0x1BFF) return Ucd_Script_BATAK;
	if (codepoint == 0x11000) return Ucd_Script_BRAHMI;
	if (codepoint == 0x11001) return Ucd_Script_BRAHMI;
	if (codepoint == 0x11002) return Ucd_Script_BRAHMI;
	if (codepoint >= 0x11003 && codepoint <= 0x11037) return Ucd_Script_BRAHMI;
	if (codepoint >= 0x11038 && codepoint <= 0x11046) return Ucd_Script_BRAHMI;
	if (codepoint >= 0x11047 && codepoint <= 0x1104D) return Ucd_Script_BRAHMI;
	if (codepoint >= 0x11052 && codepoint <= 0x11065) return Ucd_Script_BRAHMI;
	if (codepoint >= 0x11066 && codepoint <= 0x1106F) return Ucd_Script_BRAHMI;
	if (codepoint == 0x1107F) return Ucd_Script_BRAHMI;
	if (codepoint >= 0x0840 && codepoint <= 0x0858) return Ucd_Script_MANDAIC;
	if (codepoint >= 0x0859 && codepoint <= 0x085B) return Ucd_Script_MANDAIC;
	if (codepoint == 0x085E) return Ucd_Script_MANDAIC;
	if (codepoint >= 0x11100 && codepoint <= 0x11102) return Ucd_Script_CHAKMA;
	if (codepoint >= 0x11103 && codepoint <= 0x11126) return Ucd_Script_CHAKMA;
	if (codepoint >= 0x11127 && codepoint <= 0x1112B) return Ucd_Script_CHAKMA;
	if (codepoint == 0x1112C) return Ucd_Script_CHAKMA;
	if (codepoint >= 0x1112D && codepoint <= 0x11134) return Ucd_Script_CHAKMA;
	if (codepoint >= 0x11136 && codepoint <= 0x1113F) return Ucd_Script_CHAKMA;
	if (codepoint >= 0x11140 && codepoint <= 0x11143) return Ucd_Script_CHAKMA;
	if (codepoint == 0x11144) return Ucd_Script_CHAKMA;
	if (codepoint >= 0x11145 && codepoint <= 0x11146) return Ucd_Script_CHAKMA;
	if (codepoint == 0x11147) return Ucd_Script_CHAKMA;
	if (codepoint >= 0x109A0 && codepoint <= 0x109B7) return Ucd_Script_MEROITIC_CURSIVE;
	if (codepoint >= 0x109BC && codepoint <= 0x109BD) return Ucd_Script_MEROITIC_CURSIVE;
	if (codepoint >= 0x109BE && codepoint <= 0x109BF) return Ucd_Script_MEROITIC_CURSIVE;
	if (codepoint >= 0x109C0 && codepoint <= 0x109CF) return Ucd_Script_MEROITIC_CURSIVE;
	if (codepoint >= 0x109D2 && codepoint <= 0x109FF) return Ucd_Script_MEROITIC_CURSIVE;
	if (codepoint >= 0x10980 && codepoint <= 0x1099F) return Ucd_Script_MEROITIC_HIEROGLYPHS;
	if (codepoint >= 0x16F00 && codepoint <= 0x16F4A) return Ucd_Script_MIAO;
	if (codepoint == 0x16F4F) return Ucd_Script_MIAO;
	if (codepoint == 0x16F50) return Ucd_Script_MIAO;
	if (codepoint >= 0x16F51 && codepoint <= 0x16F87) return Ucd_Script_MIAO;
	if (codepoint >= 0x16F8F && codepoint <= 0x16F92) return Ucd_Script_MIAO;
	if (codepoint >= 0x16F93 && codepoint <= 0x16F9F) return Ucd_Script_MIAO;
	if (codepoint >= 0x11180 && codepoint <= 0x11181) return Ucd_Script_SHARADA;
	if (codepoint == 0x11182) return Ucd_Script_SHARADA;
	if (codepoint >= 0x11183 && codepoint <= 0x111B2) return Ucd_Script_SHARADA;
	if (codepoint >= 0x111B3 && codepoint <= 0x111B5) return Ucd_Script_SHARADA;
	if (codepoint >= 0x111B6 && codepoint <= 0x111BE) return Ucd_Script_SHARADA;
	if (codepoint >= 0x111BF && codepoint <= 0x111C0) return Ucd_Script_SHARADA;
	if (codepoint >= 0x111C1 && codepoint <= 0x111C4) return Ucd_Script_SHARADA;
	if (codepoint >= 0x111C5 && codepoint <= 0x111C8) return Ucd_Script_SHARADA;
	if (codepoint >= 0x111C9 && codepoint <= 0x111CC) return Ucd_Script_SHARADA;
	if (codepoint == 0x111CD) return Ucd_Script_SHARADA;
	if (codepoint == 0x111CE) return Ucd_Script_SHARADA;
	if (codepoint == 0x111CF) return Ucd_Script_SHARADA;
	if (codepoint >= 0x111D0 && codepoint <= 0x111D9) return Ucd_Script_SHARADA;
	if (codepoint == 0x111DA) return Ucd_Script_SHARADA;
	if (codepoint == 0x111DB) return Ucd_Script_SHARADA;
	if (codepoint == 0x111DC) return Ucd_Script_SHARADA;
	if (codepoint >= 0x111DD && codepoint <= 0x111DF) return Ucd_Script_SHARADA;
	if (codepoint >= 0x110D0 && codepoint <= 0x110E8) return Ucd_Script_SORA_SOMPENG;
	if (codepoint >= 0x110F0 && codepoint <= 0x110F9) return Ucd_Script_SORA_SOMPENG;
	if (codepoint >= 0x11680 && codepoint <= 0x116AA) return Ucd_Script_TAKRI;
	if (codepoint == 0x116AB) return Ucd_Script_TAKRI;
	if (codepoint == 0x116AC) return Ucd_Script_TAKRI;
	if (codepoint == 0x116AD) return Ucd_Script_TAKRI;
	if (codepoint >= 0x116AE && codepoint <= 0x116AF) return Ucd_Script_TAKRI;
	if (codepoint >= 0x116B0 && codepoint <= 0x116B5) return Ucd_Script_TAKRI;
	if (codepoint == 0x116B6) return Ucd_Script_TAKRI;
	if (codepoint == 0x116B7) return Ucd_Script_TAKRI;
	if (codepoint == 0x116B8) return Ucd_Script_TAKRI;
	if (codepoint >= 0x116C0 && codepoint <= 0x116C9) return Ucd_Script_TAKRI;
	if (codepoint >= 0x10530 && codepoint <= 0x10563) return Ucd_Script_CAUCASIAN_ALBANIAN;
	if (codepoint == 0x1056F) return Ucd_Script_CAUCASIAN_ALBANIAN;
	if (codepoint >= 0x16AD0 && codepoint <= 0x16AED) return Ucd_Script_BASSA_VAH;
	if (codepoint >= 0x16AF0 && codepoint <= 0x16AF4) return Ucd_Script_BASSA_VAH;
	if (codepoint == 0x16AF5) return Ucd_Script_BASSA_VAH;
	if (codepoint >= 0x1BC00 && codepoint <= 0x1BC6A) return Ucd_Script_DUPLOYAN;
	if (codepoint >= 0x1BC70 && codepoint <= 0x1BC7C) return Ucd_Script_DUPLOYAN;
	if (codepoint >= 0x1BC80 && codepoint <= 0x1BC88) return Ucd_Script_DUPLOYAN;
	if (codepoint >= 0x1BC90 && codepoint <= 0x1BC99) return Ucd_Script_DUPLOYAN;
	if (codepoint == 0x1BC9C) return Ucd_Script_DUPLOYAN;
	if (codepoint >= 0x1BC9D && codepoint <= 0x1BC9E) return Ucd_Script_DUPLOYAN;
	if (codepoint == 0x1BC9F) return Ucd_Script_DUPLOYAN;
	if (codepoint >= 0x10500 && codepoint <= 0x10527) return Ucd_Script_ELBASAN;
	if (codepoint >= 0x11300 && codepoint <= 0x11301) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11302 && codepoint <= 0x11303) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11305 && codepoint <= 0x1130C) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x1130F && codepoint <= 0x11310) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11313 && codepoint <= 0x11328) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x1132A && codepoint <= 0x11330) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11332 && codepoint <= 0x11333) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11335 && codepoint <= 0x11339) return Ucd_Script_GRANTHA;
	if (codepoint == 0x1133C) return Ucd_Script_GRANTHA;
	if (codepoint == 0x1133D) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x1133E && codepoint <= 0x1133F) return Ucd_Script_GRANTHA;
	if (codepoint == 0x11340) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11341 && codepoint <= 0x11344) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11347 && codepoint <= 0x11348) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x1134B && codepoint <= 0x1134D) return Ucd_Script_GRANTHA;
	if (codepoint == 0x11350) return Ucd_Script_GRANTHA;
	if (codepoint == 0x11357) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x1135D && codepoint <= 0x11361) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11362 && codepoint <= 0x11363) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11366 && codepoint <= 0x1136C) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x11370 && codepoint <= 0x11374) return Ucd_Script_GRANTHA;
	if (codepoint >= 0x16B00 && codepoint <= 0x16B2F) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x16B30 && codepoint <= 0x16B36) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x16B37 && codepoint <= 0x16B3B) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x16B3C && codepoint <= 0x16B3F) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x16B40 && codepoint <= 0x16B43) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint == 0x16B44) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint == 0x16B45) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x16B50 && codepoint <= 0x16B59) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x16B5B && codepoint <= 0x16B61) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x16B63 && codepoint <= 0x16B77) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x16B7D && codepoint <= 0x16B8F) return Ucd_Script_PAHAWH_HMONG;
	if (codepoint >= 0x11200 && codepoint <= 0x11211) return Ucd_Script_KHOJKI;
	if (codepoint >= 0x11213 && codepoint <= 0x1122B) return Ucd_Script_KHOJKI;
	if (codepoint >= 0x1122C && codepoint <= 0x1122E) return Ucd_Script_KHOJKI;
	if (codepoint >= 0x1122F && codepoint <= 0x11231) return Ucd_Script_KHOJKI;
	if (codepoint >= 0x11232 && codepoint <= 0x11233) return Ucd_Script_KHOJKI;
	if (codepoint == 0x11234) return Ucd_Script_KHOJKI;
	if (codepoint == 0x11235) return Ucd_Script_KHOJKI;
	if (codepoint >= 0x11236 && codepoint <= 0x11237) return Ucd_Script_KHOJKI;
	if (codepoint >= 0x11238 && codepoint <= 0x1123D) return Ucd_Script_KHOJKI;
	if (codepoint == 0x1123E) return Ucd_Script_KHOJKI;
	if (codepoint >= 0x10600 && codepoint <= 0x10736) return Ucd_Script_LINEAR_A;
	if (codepoint >= 0x10740 && codepoint <= 0x10755) return Ucd_Script_LINEAR_A;
	if (codepoint >= 0x10760 && codepoint <= 0x10767) return Ucd_Script_LINEAR_A;
	if (codepoint >= 0x11150 && codepoint <= 0x11172) return Ucd_Script_MAHAJANI;
	if (codepoint == 0x11173) return Ucd_Script_MAHAJANI;
	if (codepoint >= 0x11174 && codepoint <= 0x11175) return Ucd_Script_MAHAJANI;
	if (codepoint == 0x11176) return Ucd_Script_MAHAJANI;
	if (codepoint >= 0x10AC0 && codepoint <= 0x10AC7) return Ucd_Script_MANICHAEAN;
	if (codepoint == 0x10AC8) return Ucd_Script_MANICHAEAN;
	if (codepoint >= 0x10AC9 && codepoint <= 0x10AE4) return Ucd_Script_MANICHAEAN;
	if (codepoint >= 0x10AE5 && codepoint <= 0x10AE6) return Ucd_Script_MANICHAEAN;
	if (codepoint >= 0x10AEB && codepoint <= 0x10AEF) return Ucd_Script_MANICHAEAN;
	if (codepoint >= 0x10AF0 && codepoint <= 0x10AF6) return Ucd_Script_MANICHAEAN;
	if (codepoint >= 0x1E800 && codepoint <= 0x1E8C4) return Ucd_Script_MENDE_KIKAKUI;
	if (codepoint >= 0x1E8C7 && codepoint <= 0x1E8CF) return Ucd_Script_MENDE_KIKAKUI;
	if (codepoint >= 0x1E8D0 && codepoint <= 0x1E8D6) return Ucd_Script_MENDE_KIKAKUI;
	if (codepoint >= 0x11600 && codepoint <= 0x1162F) return Ucd_Script_MODI;
	if (codepoint >= 0x11630 && codepoint <= 0x11632) return Ucd_Script_MODI;
	if (codepoint >= 0x11633 && codepoint <= 0x1163A) return Ucd_Script_MODI;
	if (codepoint >= 0x1163B && codepoint <= 0x1163C) return Ucd_Script_MODI;
	if (codepoint == 0x1163D) return Ucd_Script_MODI;
	if (codepoint == 0x1163E) return Ucd_Script_MODI;
	if (codepoint >= 0x1163F && codepoint <= 0x11640) return Ucd_Script_MODI;
	if (codepoint >= 0x11641 && codepoint <= 0x11643) return Ucd_Script_MODI;
	if (codepoint == 0x11644) return Ucd_Script_MODI;
	if (codepoint >= 0x11650 && codepoint <= 0x11659) return Ucd_Script_MODI;
	if (codepoint >= 0x16A40 && codepoint <= 0x16A5E) return Ucd_Script_MRO;
	if (codepoint >= 0x16A60 && codepoint <= 0x16A69) return Ucd_Script_MRO;
	if (codepoint >= 0x16A6E && codepoint <= 0x16A6F) return Ucd_Script_MRO;
	if (codepoint >= 0x10A80 && codepoint <= 0x10A9C) return Ucd_Script_OLD_NORTH_ARABIAN;
	if (codepoint >= 0x10A9D && codepoint <= 0x10A9F) return Ucd_Script_OLD_NORTH_ARABIAN;
	if (codepoint >= 0x10880 && codepoint <= 0x1089E) return Ucd_Script_NABATAEAN;
	if (codepoint >= 0x108A7 && codepoint <= 0x108AF) return Ucd_Script_NABATAEAN;
	if (codepoint >= 0x10860 && codepoint <= 0x10876) return Ucd_Script_PALMYRENE;
	if (codepoint >= 0x10877 && codepoint <= 0x10878) return Ucd_Script_PALMYRENE;
	if (codepoint >= 0x10879 && codepoint <= 0x1087F) return Ucd_Script_PALMYRENE;
	if (codepoint >= 0x11AC0 && codepoint <= 0x11AF8) return Ucd_Script_PAU_CIN_HAU;
	if (codepoint >= 0x10350 && codepoint <= 0x10375) return Ucd_Script_OLD_PERMIC;
	if (codepoint >= 0x10376 && codepoint <= 0x1037A) return Ucd_Script_OLD_PERMIC;
	if (codepoint >= 0x10B80 && codepoint <= 0x10B91) return Ucd_Script_PSALTER_PAHLAVI;
	if (codepoint >= 0x10B99 && codepoint <= 0x10B9C) return Ucd_Script_PSALTER_PAHLAVI;
	if (codepoint >= 0x10BA9 && codepoint <= 0x10BAF) return Ucd_Script_PSALTER_PAHLAVI;
	if (codepoint >= 0x11580 && codepoint <= 0x115AE) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x115AF && codepoint <= 0x115B1) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x115B2 && codepoint <= 0x115B5) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x115B8 && codepoint <= 0x115BB) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x115BC && codepoint <= 0x115BD) return Ucd_Script_SIDDHAM;
	if (codepoint == 0x115BE) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x115BF && codepoint <= 0x115C0) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x115C1 && codepoint <= 0x115D7) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x115D8 && codepoint <= 0x115DB) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x115DC && codepoint <= 0x115DD) return Ucd_Script_SIDDHAM;
	if (codepoint >= 0x112B0 && codepoint <= 0x112DE) return Ucd_Script_KHUDAWADI;
	if (codepoint == 0x112DF) return Ucd_Script_KHUDAWADI;
	if (codepoint >= 0x112E0 && codepoint <= 0x112E2) return Ucd_Script_KHUDAWADI;
	if (codepoint >= 0x112E3 && codepoint <= 0x112EA) return Ucd_Script_KHUDAWADI;
	if (codepoint >= 0x112F0 && codepoint <= 0x112F9) return Ucd_Script_KHUDAWADI;
	if (codepoint >= 0x11480 && codepoint <= 0x114AF) return Ucd_Script_TIRHUTA;
	if (codepoint >= 0x114B0 && codepoint <= 0x114B2) return Ucd_Script_TIRHUTA;
	if (codepoint >= 0x114B3 && codepoint <= 0x114B8) return Ucd_Script_TIRHUTA;
	if (codepoint == 0x114B9) return Ucd_Script_TIRHUTA;
	if (codepoint == 0x114BA) return Ucd_Script_TIRHUTA;
	if (codepoint >= 0x114BB && codepoint <= 0x114BE) return Ucd_Script_TIRHUTA;
	if (codepoint >= 0x114BF && codepoint <= 0x114C0) return Ucd_Script_TIRHUTA;
	if (codepoint == 0x114C1) return Ucd_Script_TIRHUTA;
	if (codepoint >= 0x114C2 && codepoint <= 0x114C3) return Ucd_Script_TIRHUTA;
	if (codepoint >= 0x114C4 && codepoint <= 0x114C5) return Ucd_Script_TIRHUTA;
	if (codepoint == 0x114C6) return Ucd_Script_TIRHUTA;
	if (codepoint == 0x114C7) return Ucd_Script_TIRHUTA;
	if (codepoint >= 0x114D0 && codepoint <= 0x114D9) return Ucd_Script_TIRHUTA;
	if (codepoint >= 0x118A0 && codepoint <= 0x118DF) return Ucd_Script_WARANG_CITI;
	if (codepoint >= 0x118E0 && codepoint <= 0x118E9) return Ucd_Script_WARANG_CITI;
	if (codepoint >= 0x118EA && codepoint <= 0x118F2) return Ucd_Script_WARANG_CITI;
	if (codepoint == 0x118FF) return Ucd_Script_WARANG_CITI;
	if (codepoint >= 0x11700 && codepoint <= 0x1171A) return Ucd_Script_AHOM;
	if (codepoint >= 0x1171D && codepoint <= 0x1171F) return Ucd_Script_AHOM;
	if (codepoint >= 0x11720 && codepoint <= 0x11721) return Ucd_Script_AHOM;
	if (codepoint >= 0x11722 && codepoint <= 0x11725) return Ucd_Script_AHOM;
	if (codepoint == 0x11726) return Ucd_Script_AHOM;
	if (codepoint >= 0x11727 && codepoint <= 0x1172B) return Ucd_Script_AHOM;
	if (codepoint >= 0x11730 && codepoint <= 0x11739) return Ucd_Script_AHOM;
	if (codepoint >= 0x1173A && codepoint <= 0x1173B) return Ucd_Script_AHOM;
	if (codepoint >= 0x1173C && codepoint <= 0x1173E) return Ucd_Script_AHOM;
	if (codepoint == 0x1173F) return Ucd_Script_AHOM;
	if (codepoint >= 0x14400 && codepoint <= 0x14646) return Ucd_Script_ANATOLIAN_HIEROGLYPHS;
	if (codepoint >= 0x108E0 && codepoint <= 0x108F2) return Ucd_Script_HATRAN;
	if (codepoint >= 0x108F4 && codepoint <= 0x108F5) return Ucd_Script_HATRAN;
	if (codepoint >= 0x108FB && codepoint <= 0x108FF) return Ucd_Script_HATRAN;
	if (codepoint >= 0x11280 && codepoint <= 0x11286) return Ucd_Script_MULTANI;
	if (codepoint == 0x11288) return Ucd_Script_MULTANI;
	if (codepoint >= 0x1128A && codepoint <= 0x1128D) return Ucd_Script_MULTANI;
	if (codepoint >= 0x1128F && codepoint <= 0x1129D) return Ucd_Script_MULTANI;
	if (codepoint >= 0x1129F && codepoint <= 0x112A8) return Ucd_Script_MULTANI;
	if (codepoint == 0x112A9) return Ucd_Script_MULTANI;
	if (codepoint >= 0x10C80 && codepoint <= 0x10CB2) return Ucd_Script_OLD_HUNGARIAN;
	if (codepoint >= 0x10CC0 && codepoint <= 0x10CF2) return Ucd_Script_OLD_HUNGARIAN;
	if (codepoint >= 0x10CFA && codepoint <= 0x10CFF) return Ucd_Script_OLD_HUNGARIAN;
	if (codepoint >= 0x1D800 && codepoint <= 0x1D9FF) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DA00 && codepoint <= 0x1DA36) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DA37 && codepoint <= 0x1DA3A) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DA3B && codepoint <= 0x1DA6C) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DA6D && codepoint <= 0x1DA74) return Ucd_Script_SIGNWRITING;
	if (codepoint == 0x1DA75) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DA76 && codepoint <= 0x1DA83) return Ucd_Script_SIGNWRITING;
	if (codepoint == 0x1DA84) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DA85 && codepoint <= 0x1DA86) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DA87 && codepoint <= 0x1DA8B) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DA9B && codepoint <= 0x1DA9F) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1DAA1 && codepoint <= 0x1DAAF) return Ucd_Script_SIGNWRITING;
	if (codepoint >= 0x1E900 && codepoint <= 0x1E943) return Ucd_Script_ADLAM;
	if (codepoint >= 0x1E944 && codepoint <= 0x1E94A) return Ucd_Script_ADLAM;
	if (codepoint == 0x1E94B) return Ucd_Script_ADLAM;
	if (codepoint >= 0x1E950 && codepoint <= 0x1E959) return Ucd_Script_ADLAM;
	if (codepoint >= 0x1E95E && codepoint <= 0x1E95F) return Ucd_Script_ADLAM;
	if (codepoint >= 0x11C00 && codepoint <= 0x11C08) return Ucd_Script_BHAIKSUKI;
	if (codepoint >= 0x11C0A && codepoint <= 0x11C2E) return Ucd_Script_BHAIKSUKI;
	if (codepoint == 0x11C2F) return Ucd_Script_BHAIKSUKI;
	if (codepoint >= 0x11C30 && codepoint <= 0x11C36) return Ucd_Script_BHAIKSUKI;
	if (codepoint >= 0x11C38 && codepoint <= 0x11C3D) return Ucd_Script_BHAIKSUKI;
	if (codepoint == 0x11C3E) return Ucd_Script_BHAIKSUKI;
	if (codepoint == 0x11C3F) return Ucd_Script_BHAIKSUKI;
	if (codepoint == 0x11C40) return Ucd_Script_BHAIKSUKI;
	if (codepoint >= 0x11C41 && codepoint <= 0x11C45) return Ucd_Script_BHAIKSUKI;
	if (codepoint >= 0x11C50 && codepoint <= 0x11C59) return Ucd_Script_BHAIKSUKI;
	if (codepoint >= 0x11C5A && codepoint <= 0x11C6C) return Ucd_Script_BHAIKSUKI;
	if (codepoint >= 0x11C70 && codepoint <= 0x11C71) return Ucd_Script_MARCHEN;
	if (codepoint >= 0x11C72 && codepoint <= 0x11C8F) return Ucd_Script_MARCHEN;
	if (codepoint >= 0x11C92 && codepoint <= 0x11CA7) return Ucd_Script_MARCHEN;
	if (codepoint == 0x11CA9) return Ucd_Script_MARCHEN;
	if (codepoint >= 0x11CAA && codepoint <= 0x11CB0) return Ucd_Script_MARCHEN;
	if (codepoint == 0x11CB1) return Ucd_Script_MARCHEN;
	if (codepoint >= 0x11CB2 && codepoint <= 0x11CB3) return Ucd_Script_MARCHEN;
	if (codepoint == 0x11CB4) return Ucd_Script_MARCHEN;
	if (codepoint >= 0x11CB5 && codepoint <= 0x11CB6) return Ucd_Script_MARCHEN;
	if (codepoint >= 0x11400 && codepoint <= 0x11434) return Ucd_Script_NEWA;
	if (codepoint >= 0x11435 && codepoint <= 0x11437) return Ucd_Script_NEWA;
	if (codepoint >= 0x11438 && codepoint <= 0x1143F) return Ucd_Script_NEWA;
	if (codepoint >= 0x11440 && codepoint <= 0x11441) return Ucd_Script_NEWA;
	if (codepoint >= 0x11442 && codepoint <= 0x11444) return Ucd_Script_NEWA;
	if (codepoint == 0x11445) return Ucd_Script_NEWA;
	if (codepoint == 0x11446) return Ucd_Script_NEWA;
	if (codepoint >= 0x11447 && codepoint <= 0x1144A) return Ucd_Script_NEWA;
	if (codepoint >= 0x1144B && codepoint <= 0x1144F) return Ucd_Script_NEWA;
	if (codepoint >= 0x11450 && codepoint <= 0x11459) return Ucd_Script_NEWA;
	if (codepoint >= 0x1145A && codepoint <= 0x1145B) return Ucd_Script_NEWA;
	if (codepoint == 0x1145D) return Ucd_Script_NEWA;
	if (codepoint == 0x1145E) return Ucd_Script_NEWA;
	if (codepoint >= 0x1145F && codepoint <= 0x11461) return Ucd_Script_NEWA;
	if (codepoint >= 0x104B0 && codepoint <= 0x104D3) return Ucd_Script_OSAGE;
	if (codepoint >= 0x104D8 && codepoint <= 0x104FB) return Ucd_Script_OSAGE;
	if (codepoint == 0x16FE0) return Ucd_Script_TANGUT;
	if (codepoint >= 0x17000 && codepoint <= 0x187F7) return Ucd_Script_TANGUT;
	if (codepoint >= 0x18800 && codepoint <= 0x18AFF) return Ucd_Script_TANGUT;
	if (codepoint >= 0x18D00 && codepoint <= 0x18D08) return Ucd_Script_TANGUT;
	if (codepoint >= 0x11D00 && codepoint <= 0x11D06) return Ucd_Script_MASARAM_GONDI;
	if (codepoint >= 0x11D08 && codepoint <= 0x11D09) return Ucd_Script_MASARAM_GONDI;
	if (codepoint >= 0x11D0B && codepoint <= 0x11D30) return Ucd_Script_MASARAM_GONDI;
	if (codepoint >= 0x11D31 && codepoint <= 0x11D36) return Ucd_Script_MASARAM_GONDI;
	if (codepoint == 0x11D3A) return Ucd_Script_MASARAM_GONDI;
	if (codepoint >= 0x11D3C && codepoint <= 0x11D3D) return Ucd_Script_MASARAM_GONDI;
	if (codepoint >= 0x11D3F && codepoint <= 0x11D45) return Ucd_Script_MASARAM_GONDI;
	if (codepoint == 0x11D46) return Ucd_Script_MASARAM_GONDI;
	if (codepoint == 0x11D47) return Ucd_Script_MASARAM_GONDI;
	if (codepoint >= 0x11D50 && codepoint <= 0x11D59) return Ucd_Script_MASARAM_GONDI;
	if (codepoint == 0x16FE1) return Ucd_Script_NUSHU;
	if (codepoint >= 0x1B170 && codepoint <= 0x1B2FB) return Ucd_Script_NUSHU;
	if (codepoint == 0x11A50) return Ucd_Script_SOYOMBO;
	if (codepoint >= 0x11A51 && codepoint <= 0x11A56) return Ucd_Script_SOYOMBO;
	if (codepoint >= 0x11A57 && codepoint <= 0x11A58) return Ucd_Script_SOYOMBO;
	if (codepoint >= 0x11A59 && codepoint <= 0x11A5B) return Ucd_Script_SOYOMBO;
	if (codepoint >= 0x11A5C && codepoint <= 0x11A89) return Ucd_Script_SOYOMBO;
	if (codepoint >= 0x11A8A && codepoint <= 0x11A96) return Ucd_Script_SOYOMBO;
	if (codepoint == 0x11A97) return Ucd_Script_SOYOMBO;
	if (codepoint >= 0x11A98 && codepoint <= 0x11A99) return Ucd_Script_SOYOMBO;
	if (codepoint >= 0x11A9A && codepoint <= 0x11A9C) return Ucd_Script_SOYOMBO;
	if (codepoint == 0x11A9D) return Ucd_Script_SOYOMBO;
	if (codepoint >= 0x11A9E && codepoint <= 0x11AA2) return Ucd_Script_SOYOMBO;
	if (codepoint == 0x11A00) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint >= 0x11A01 && codepoint <= 0x11A0A) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint >= 0x11A0B && codepoint <= 0x11A32) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint >= 0x11A33 && codepoint <= 0x11A38) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint == 0x11A39) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint == 0x11A3A) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint >= 0x11A3B && codepoint <= 0x11A3E) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint >= 0x11A3F && codepoint <= 0x11A46) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint == 0x11A47) return Ucd_Script_ZANABAZAR_SQUARE;
	if (codepoint >= 0x11800 && codepoint <= 0x1182B) return Ucd_Script_DOGRA;
	if (codepoint >= 0x1182C && codepoint <= 0x1182E) return Ucd_Script_DOGRA;
	if (codepoint >= 0x1182F && codepoint <= 0x11837) return Ucd_Script_DOGRA;
	if (codepoint == 0x11838) return Ucd_Script_DOGRA;
	if (codepoint >= 0x11839 && codepoint <= 0x1183A) return Ucd_Script_DOGRA;
	if (codepoint == 0x1183B) return Ucd_Script_DOGRA;
	if (codepoint >= 0x11D60 && codepoint <= 0x11D65) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint >= 0x11D67 && codepoint <= 0x11D68) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint >= 0x11D6A && codepoint <= 0x11D89) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint >= 0x11D8A && codepoint <= 0x11D8E) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint >= 0x11D90 && codepoint <= 0x11D91) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint >= 0x11D93 && codepoint <= 0x11D94) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint == 0x11D95) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint == 0x11D96) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint == 0x11D97) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint == 0x11D98) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint >= 0x11DA0 && codepoint <= 0x11DA9) return Ucd_Script_GUNJALA_GONDI;
	if (codepoint >= 0x11EE0 && codepoint <= 0x11EF2) return Ucd_Script_MAKASAR;
	if (codepoint >= 0x11EF3 && codepoint <= 0x11EF4) return Ucd_Script_MAKASAR;
	if (codepoint >= 0x11EF5 && codepoint <= 0x11EF6) return Ucd_Script_MAKASAR;
	if (codepoint >= 0x11EF7 && codepoint <= 0x11EF8) return Ucd_Script_MAKASAR;
	if (codepoint >= 0x16E40 && codepoint <= 0x16E7F) return Ucd_Script_MEDEFAIDRIN;
	if (codepoint >= 0x16E80 && codepoint <= 0x16E96) return Ucd_Script_MEDEFAIDRIN;
	if (codepoint >= 0x16E97 && codepoint <= 0x16E9A) return Ucd_Script_MEDEFAIDRIN;
	if (codepoint >= 0x10D00 && codepoint <= 0x10D23) return Ucd_Script_HANIFI_ROHINGYA;
	if (codepoint >= 0x10D24 && codepoint <= 0x10D27) return Ucd_Script_HANIFI_ROHINGYA;
	if (codepoint >= 0x10D30 && codepoint <= 0x10D39) return Ucd_Script_HANIFI_ROHINGYA;
	if (codepoint >= 0x10F30 && codepoint <= 0x10F45) return Ucd_Script_SOGDIAN;
	if (codepoint >= 0x10F46 && codepoint <= 0x10F50) return Ucd_Script_SOGDIAN;
	if (codepoint >= 0x10F51 && codepoint <= 0x10F54) return Ucd_Script_SOGDIAN;
	if (codepoint >= 0x10F55 && codepoint <= 0x10F59) return Ucd_Script_SOGDIAN;
	if (codepoint >= 0x10F00 && codepoint <= 0x10F1C) return Ucd_Script_OLD_SOGDIAN;
	if (codepoint >= 0x10F1D && codepoint <= 0x10F26) return Ucd_Script_OLD_SOGDIAN;
	if (codepoint == 0x10F27) return Ucd_Script_OLD_SOGDIAN;
	if (codepoint >= 0x10FE0 && codepoint <= 0x10FF6) return Ucd_Script_ELYMAIC;
	if (codepoint >= 0x119A0 && codepoint <= 0x119A7) return Ucd_Script_NANDINAGARI;
	if (codepoint >= 0x119AA && codepoint <= 0x119D0) return Ucd_Script_NANDINAGARI;
	if (codepoint >= 0x119D1 && codepoint <= 0x119D3) return Ucd_Script_NANDINAGARI;
	if (codepoint >= 0x119D4 && codepoint <= 0x119D7) return Ucd_Script_NANDINAGARI;
	if (codepoint >= 0x119DA && codepoint <= 0x119DB) return Ucd_Script_NANDINAGARI;
	if (codepoint >= 0x119DC && codepoint <= 0x119DF) return Ucd_Script_NANDINAGARI;
	if (codepoint == 0x119E0) return Ucd_Script_NANDINAGARI;
	if (codepoint == 0x119E1) return Ucd_Script_NANDINAGARI;
	if (codepoint == 0x119E2) return Ucd_Script_NANDINAGARI;
	if (codepoint == 0x119E3) return Ucd_Script_NANDINAGARI;
	if (codepoint == 0x119E4) return Ucd_Script_NANDINAGARI;
	if (codepoint >= 0x1E100 && codepoint <= 0x1E12C) return Ucd_Script_NYIAKENG_PUACHUE_HMONG;
	if (codepoint >= 0x1E130 && codepoint <= 0x1E136) return Ucd_Script_NYIAKENG_PUACHUE_HMONG;
	if (codepoint >= 0x1E137 && codepoint <= 0x1E13D) return Ucd_Script_NYIAKENG_PUACHUE_HMONG;
	if (codepoint >= 0x1E140 && codepoint <= 0x1E149) return Ucd_Script_NYIAKENG_PUACHUE_HMONG;
	if (codepoint == 0x1E14E) return Ucd_Script_NYIAKENG_PUACHUE_HMONG;
	if (codepoint == 0x1E14F) return Ucd_Script_NYIAKENG_PUACHUE_HMONG;
	if (codepoint >= 0x1E2C0 && codepoint <= 0x1E2EB) return Ucd_Script_WANCHO;
	if (codepoint >= 0x1E2EC && codepoint <= 0x1E2EF) return Ucd_Script_WANCHO;
	if (codepoint >= 0x1E2F0 && codepoint <= 0x1E2F9) return Ucd_Script_WANCHO;
	if (codepoint == 0x1E2FF) return Ucd_Script_WANCHO;
	if (codepoint >= 0x10FB0 && codepoint <= 0x10FC4) return Ucd_Script_CHORASMIAN;
	if (codepoint >= 0x10FC5 && codepoint <= 0x10FCB) return Ucd_Script_CHORASMIAN;
	if (codepoint >= 0x11900 && codepoint <= 0x11906) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x11909) return Ucd_Script_DIVES_AKURU;
	if (codepoint >= 0x1190C && codepoint <= 0x11913) return Ucd_Script_DIVES_AKURU;
	if (codepoint >= 0x11915 && codepoint <= 0x11916) return Ucd_Script_DIVES_AKURU;
	if (codepoint >= 0x11918 && codepoint <= 0x1192F) return Ucd_Script_DIVES_AKURU;
	if (codepoint >= 0x11930 && codepoint <= 0x11935) return Ucd_Script_DIVES_AKURU;
	if (codepoint >= 0x11937 && codepoint <= 0x11938) return Ucd_Script_DIVES_AKURU;
	if (codepoint >= 0x1193B && codepoint <= 0x1193C) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x1193D) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x1193E) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x1193F) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x11940) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x11941) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x11942) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x11943) return Ucd_Script_DIVES_AKURU;
	if (codepoint >= 0x11944 && codepoint <= 0x11946) return Ucd_Script_DIVES_AKURU;
	if (codepoint >= 0x11950 && codepoint <= 0x11959) return Ucd_Script_DIVES_AKURU;
	if (codepoint == 0x16FE4) return Ucd_Script_KHITAN_SMALL_SCRIPT;
	if (codepoint >= 0x18B00 && codepoint <= 0x18CD5) return Ucd_Script_KHITAN_SMALL_SCRIPT;
	if (codepoint >= 0x10E80 && codepoint <= 0x10EA9) return Ucd_Script_YEZIDI;
	if (codepoint >= 0x10EAB && codepoint <= 0x10EAC) return Ucd_Script_YEZIDI;
	if (codepoint == 0x10EAD) return Ucd_Script_YEZIDI;
	if (codepoint >= 0x10EB0 && codepoint <= 0x10EB1) return Ucd_Script_YEZIDI;

	return Ucd_Script_UNKNOWN;
}

const char *enum_string(Ucd_Script value) {
	static const char *strings[] = {
		"UNKNOWN",
		"COMMON",
		"LATIN",
		"GREEK",
		"CYRILLIC",
		"ARMENIAN",
		"HEBREW",
		"ARABIC",
		"SYRIAC",
		"THAANA",
		"DEVANAGARI",
		"BENGALI",
		"GURMUKHI",
		"GUJARATI",
		"ORIYA",
		"TAMIL",
		"TELUGU",
		"KANNADA",
		"MALAYALAM",
		"SINHALA",
		"THAI",
		"LAO",
		"TIBETAN",
		"MYANMAR",
		"GEORGIAN",
		"HANGUL",
		"ETHIOPIC",
		"CHEROKEE",
		"CANADIAN_ABORIGINAL",
		"OGHAM",
		"RUNIC",
		"KHMER",
		"MONGOLIAN",
		"HIRAGANA",
		"KATAKANA",
		"BOPOMOFO",
		"HAN",
		"YI",
		"OLD_ITALIC",
		"GOTHIC",
		"DESERET",
		"INHERITED",
		"TAGALOG",
		"HANUNOO",
		"BUHID",
		"TAGBANWA",
		"LIMBU",
		"TAI_LE",
		"LINEAR_B",
		"UGARITIC",
		"SHAVIAN",
		"OSMANYA",
		"CYPRIOT",
		"BRAILLE",
		"BUGINESE",
		"COPTIC",
		"NEW_TAI_LUE",
		"GLAGOLITIC",
		"TIFINAGH",
		"SYLOTI_NAGRI",
		"OLD_PERSIAN",
		"KHAROSHTHI",
		"BALINESE",
		"CUNEIFORM",
		"PHOENICIAN",
		"PHAGS_PA",
		"NKO",
		"SUNDANESE",
		"LEPCHA",
		"OL_CHIKI",
		"VAI",
		"SAURASHTRA",
		"KAYAH_LI",
		"REJANG",
		"LYCIAN",
		"CARIAN",
		"LYDIAN",
		"CHAM",
		"TAI_THAM",
		"TAI_VIET",
		"AVESTAN",
		"EGYPTIAN_HIEROGLYPHS",
		"SAMARITAN",
		"LISU",
		"BAMUM",
		"JAVANESE",
		"MEETEI_MAYEK",
		"IMPERIAL_ARAMAIC",
		"OLD_SOUTH_ARABIAN",
		"INSCRIPTIONAL_PARTHIAN",
		"INSCRIPTIONAL_PAHLAVI",
		"OLD_TURKIC",
		"KAITHI",
		"BATAK",
		"BRAHMI",
		"MANDAIC",
		"CHAKMA",
		"MEROITIC_CURSIVE",
		"MEROITIC_HIEROGLYPHS",
		"MIAO",
		"SHARADA",
		"SORA_SOMPENG",
		"TAKRI",
		"CAUCASIAN_ALBANIAN",
		"BASSA_VAH",
		"DUPLOYAN",
		"ELBASAN",
		"GRANTHA",
		"PAHAWH_HMONG",
		"KHOJKI",
		"LINEAR_A",
		"MAHAJANI",
		"MANICHAEAN",
		"MENDE_KIKAKUI",
		"MODI",
		"MRO",
		"OLD_NORTH_ARABIAN",
		"NABATAEAN",
		"PALMYRENE",
		"PAU_CIN_HAU",
		"OLD_PERMIC",
		"PSALTER_PAHLAVI",
		"SIDDHAM",
		"KHUDAWADI",
		"TIRHUTA",
		"WARANG_CITI",
		"AHOM",
		"ANATOLIAN_HIEROGLYPHS",
		"HATRAN",
		"MULTANI",
		"OLD_HUNGARIAN",
		"SIGNWRITING",
		"ADLAM",
		"BHAIKSUKI",
		"MARCHEN",
		"NEWA",
		"OSAGE",
		"TANGUT",
		"MASARAM_GONDI",
		"NUSHU",
		"SOYOMBO",
		"ZANABAZAR_SQUARE",
		"DOGRA",
		"GUNJALA_GONDI",
		"MAKASAR",
		"MEDEFAIDRIN",
		"HANIFI_ROHINGYA",
		"SOGDIAN",
		"OLD_SOGDIAN",
		"ELYMAIC",
		"NANDINAGARI",
		"NYIAKENG_PUACHUE_HMONG",
		"WANCHO",
		"CHORASMIAN",
		"DIVES_AKURU",
		"KHITAN_SMALL_SCRIPT",
		"YEZIDI",
		"COUNT",
	};
	return strings[value];
}

Ucd_Script_Ext ucd_script_ext(uint32_t codepoint) {
	if (codepoint == 0x1CF7) return Ucd_Script_Ext_BENG;
	if (codepoint == 0x1CD1) return Ucd_Script_Ext_DEVA;
	if (codepoint == 0x1CD4) return Ucd_Script_Ext_DEVA;
	if (codepoint == 0x1CDB) return Ucd_Script_Ext_DEVA;
	if (codepoint >= 0x1CDE && codepoint <= 0x1CDF) return Ucd_Script_Ext_DEVA;
	if (codepoint >= 0x1CE2 && codepoint <= 0x1CE8) return Ucd_Script_Ext_DEVA;
	if (codepoint >= 0x1CEB && codepoint <= 0x1CEC) return Ucd_Script_Ext_DEVA;
	if (codepoint >= 0x1CEE && codepoint <= 0x1CF1) return Ucd_Script_Ext_DEVA;
	if (codepoint >= 0x1BCA0 && codepoint <= 0x1BCA3) return Ucd_Script_Ext_DUPL;
	if (codepoint == 0x0342) return Ucd_Script_Ext_GREK;
	if (codepoint == 0x0345) return Ucd_Script_Ext_GREK;
	if (codepoint >= 0x1DC0 && codepoint <= 0x1DC1) return Ucd_Script_Ext_GREK;
	if (codepoint == 0x3006) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x303E && codepoint <= 0x303F) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x3190 && codepoint <= 0x3191) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x3192 && codepoint <= 0x3195) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x3196 && codepoint <= 0x319F) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x31C0 && codepoint <= 0x31E3) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x3220 && codepoint <= 0x3229) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x322A && codepoint <= 0x3247) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x3280 && codepoint <= 0x3289) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x328A && codepoint <= 0x32B0) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x32C0 && codepoint <= 0x32CB) return Ucd_Script_Ext_HANI;
	if (codepoint == 0x32FF) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x3358 && codepoint <= 0x3370) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x337B && codepoint <= 0x337F) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x33E0 && codepoint <= 0x33FE) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x1D360 && codepoint <= 0x1D371) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x1F250 && codepoint <= 0x1F251) return Ucd_Script_Ext_HANI;
	if (codepoint >= 0x0363 && codepoint <= 0x036F) return Ucd_Script_Ext_LATN;
	if (codepoint == 0x1CFA) return Ucd_Script_Ext_NAND;
	if (codepoint == 0x102E0) return Ucd_Script_Ext_ARAB_COPT;
	if (codepoint >= 0x102E1 && codepoint <= 0x102FB) return Ucd_Script_Ext_ARAB_COPT;
	if (codepoint == 0x06D4) return Ucd_Script_Ext_ARAB_ROHG;
	if (codepoint >= 0x064B && codepoint <= 0x0655) return Ucd_Script_Ext_ARAB_SYRC;
	if (codepoint == 0x0670) return Ucd_Script_Ext_ARAB_SYRC;
	if (codepoint == 0xFDF2) return Ucd_Script_Ext_ARAB_THAA;
	if (codepoint == 0xFDFD) return Ucd_Script_Ext_ARAB_THAA;
	if (codepoint >= 0x1CD5 && codepoint <= 0x1CD6) return Ucd_Script_Ext_BENG_DEVA;
	if (codepoint == 0x1CD8) return Ucd_Script_Ext_BENG_DEVA;
	if (codepoint == 0x1CE1) return Ucd_Script_Ext_BENG_DEVA;
	if (codepoint == 0x1CEA) return Ucd_Script_Ext_BENG_DEVA;
	if (codepoint == 0x1CED) return Ucd_Script_Ext_BENG_DEVA;
	if (codepoint >= 0x1CF5 && codepoint <= 0x1CF6) return Ucd_Script_Ext_BENG_DEVA;
	if (codepoint == 0xA8F1) return Ucd_Script_Ext_BENG_DEVA;
	if (codepoint >= 0x302A && codepoint <= 0x302D) return Ucd_Script_Ext_BOPO_HANI;
	if (codepoint == 0xA9CF) return Ucd_Script_Ext_BUGI_JAVA;
	if (codepoint >= 0x10100 && codepoint <= 0x10102) return Ucd_Script_Ext_CPRT_LINB;
	if (codepoint >= 0x10137 && codepoint <= 0x1013F) return Ucd_Script_Ext_CPRT_LINB;
	if (codepoint == 0x0484) return Ucd_Script_Ext_CYRL_GLAG;
	if (codepoint == 0x0487) return Ucd_Script_Ext_CYRL_GLAG;
	if (codepoint == 0x2E43) return Ucd_Script_Ext_CYRL_GLAG;
	if (codepoint == 0xA66F) return Ucd_Script_Ext_CYRL_GLAG;
	if (codepoint >= 0x0485 && codepoint <= 0x0486) return Ucd_Script_Ext_CYRL_LATN;
	if (codepoint == 0x0483) return Ucd_Script_Ext_CYRL_PERM;
	if (codepoint == 0x1DF8) return Ucd_Script_Ext_CYRL_SYRC;
	if (codepoint == 0x1CD3) return Ucd_Script_Ext_DEVA_GRAN;
	if (codepoint == 0x1CF3) return Ucd_Script_Ext_DEVA_GRAN;
	if (codepoint >= 0x1CF8 && codepoint <= 0x1CF9) return Ucd_Script_Ext_DEVA_GRAN;
	if (codepoint == 0x1CE9) return Ucd_Script_Ext_DEVA_NAND;
	if (codepoint == 0x1CD7) return Ucd_Script_Ext_DEVA_SHRD;
	if (codepoint == 0x1CD9) return Ucd_Script_Ext_DEVA_SHRD;
	if (codepoint >= 0x1CDC && codepoint <= 0x1CDD) return Ucd_Script_Ext_DEVA_SHRD;
	if (codepoint == 0x1CE0) return Ucd_Script_Ext_DEVA_SHRD;
	if (codepoint == 0xA8F3) return Ucd_Script_Ext_DEVA_TAML;
	if (codepoint == 0x10FB) return Ucd_Script_Ext_GEOR_LATN;
	if (codepoint >= 0x0BE6 && codepoint <= 0x0BEF) return Ucd_Script_Ext_GRAN_TAML;
	if (codepoint >= 0x0BF0 && codepoint <= 0x0BF2) return Ucd_Script_Ext_GRAN_TAML;
	if (codepoint == 0x0BF3) return Ucd_Script_Ext_GRAN_TAML;
	if (codepoint == 0x11301) return Ucd_Script_Ext_GRAN_TAML;
	if (codepoint == 0x11303) return Ucd_Script_Ext_GRAN_TAML;
	if (codepoint >= 0x1133B && codepoint <= 0x1133C) return Ucd_Script_Ext_GRAN_TAML;
	if (codepoint >= 0x11FD0 && codepoint <= 0x11FD1) return Ucd_Script_Ext_GRAN_TAML;
	if (codepoint == 0x11FD3) return Ucd_Script_Ext_GRAN_TAML;
	if (codepoint >= 0x0AE6 && codepoint <= 0x0AEF) return Ucd_Script_Ext_GUJR_KHOJ;
	if (codepoint >= 0x0A66 && codepoint <= 0x0A6F) return Ucd_Script_Ext_GURU_MULT;
	if (codepoint >= 0xA700 && codepoint <= 0xA707) return Ucd_Script_Ext_HANI_LATN;
	if (codepoint >= 0x3031 && codepoint <= 0x3035) return Ucd_Script_Ext_HIRA_KANA;
	if (codepoint >= 0x3099 && codepoint <= 0x309A) return Ucd_Script_Ext_HIRA_KANA;
	if (codepoint >= 0x309B && codepoint <= 0x309C) return Ucd_Script_Ext_HIRA_KANA;
	if (codepoint == 0x30A0) return Ucd_Script_Ext_HIRA_KANA;
	if (codepoint == 0x30FC) return Ucd_Script_Ext_HIRA_KANA;
	if (codepoint == 0xFF70) return Ucd_Script_Ext_HIRA_KANA;
	if (codepoint >= 0xFF9E && codepoint <= 0xFF9F) return Ucd_Script_Ext_HIRA_KANA;
	if (codepoint >= 0x0CE6 && codepoint <= 0x0CEF) return Ucd_Script_Ext_KNDA_NAND;
	if (codepoint == 0x202F) return Ucd_Script_Ext_LATN_MONG;
	if (codepoint >= 0x1802 && codepoint <= 0x1803) return Ucd_Script_Ext_MONG_PHAG;
	if (codepoint == 0x1805) return Ucd_Script_Ext_MONG_PHAG;
	if (codepoint == 0x061C) return Ucd_Script_Ext_ARAB_SYRC_THAA;
	if (codepoint >= 0x0660 && codepoint <= 0x0669) return Ucd_Script_Ext_ARAB_THAA_YEZI;
	if (codepoint >= 0x09E6 && codepoint <= 0x09EF) return Ucd_Script_Ext_BENG_CAKM_SYLO;
	if (codepoint >= 0x1040 && codepoint <= 0x1049) return Ucd_Script_Ext_CAKM_MYMR_TALE;
	if (codepoint >= 0x10107 && codepoint <= 0x10133) return Ucd_Script_Ext_CPRT_LINA_LINB;
	if (codepoint == 0x1CF4) return Ucd_Script_Ext_DEVA_GRAN_KNDA;
	if (codepoint == 0x20F0) return Ucd_Script_Ext_DEVA_GRAN_LATN;
	if (codepoint == 0x303C) return Ucd_Script_Ext_HANI_HIRA_KANA;
	if (codepoint == 0x303D) return Ucd_Script_Ext_HANI_HIRA_KANA;
	if (codepoint == 0xA92E) return Ucd_Script_Ext_KALI_LATN_MYMR;
	if (codepoint == 0x1CD0) return Ucd_Script_Ext_BENG_DEVA_GRAN_KNDA;
	if (codepoint == 0x1CD2) return Ucd_Script_Ext_BENG_DEVA_GRAN_KNDA;
	if (codepoint >= 0x1735 && codepoint <= 0x1736) return Ucd_Script_Ext_BUHD_HANO_TAGB_TGLG;
	if (codepoint >= 0x0966 && codepoint <= 0x096F) return Ucd_Script_Ext_DEVA_DOGR_KTHI_MAHJ;
	if (codepoint == 0x060C) return Ucd_Script_Ext_ARAB_ROHG_SYRC_THAA_YEZI;
	if (codepoint == 0x061B) return Ucd_Script_Ext_ARAB_ROHG_SYRC_THAA_YEZI;
	if (codepoint == 0x061F) return Ucd_Script_Ext_ARAB_ROHG_SYRC_THAA_YEZI;
	if (codepoint == 0x3003) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA;
	if (codepoint == 0x3013) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA;
	if (codepoint == 0x301C) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA;
	if (codepoint == 0x301D) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA;
	if (codepoint >= 0x301E && codepoint <= 0x301F) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA;
	if (codepoint == 0x3030) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA;
	if (codepoint == 0x3037) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA;
	if (codepoint >= 0xFE45 && codepoint <= 0xFE46) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA;
	if (codepoint >= 0x3001 && codepoint <= 0x3002) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3008) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3009) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x300A) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x300B) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x300C) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x300D) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x300E) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x300F) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3010) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3011) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3014) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3015) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3016) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3017) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3018) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x3019) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x301A) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x301B) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x30FB) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0xFF61) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0xFF62) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0xFF63) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint >= 0xFF64 && codepoint <= 0xFF65) return Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII;
	if (codepoint == 0x1CDA) return Ucd_Script_Ext_DEVA_KNDA_MLYM_ORYA_TAML_TELU;
	if (codepoint == 0x0640) return Ucd_Script_Ext_ADLM_ARAB_MAND_MANI_PHLP_ROHG_SOGD_SYRC;
	if (codepoint == 0x1CF2) return Ucd_Script_Ext_BENG_DEVA_GRAN_KNDA_NAND_ORYA_TELU_TIRH;
	if (codepoint >= 0xA836 && codepoint <= 0xA837) return Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KTHI_MAHJ_MODI_SIND_TAKR_TIRH;
	if (codepoint == 0xA838) return Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KTHI_MAHJ_MODI_SIND_TAKR_TIRH;
	if (codepoint == 0xA839) return Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KTHI_MAHJ_MODI_SIND_TAKR_TIRH;
	if (codepoint == 0x0952) return Ucd_Script_Ext_BENG_DEVA_GRAN_GUJR_GURU_KNDA_LATN_MLYM_ORYA_TAML_TELU_TIRH;
	if (codepoint == 0x0951) return Ucd_Script_Ext_BENG_DEVA_GRAN_GUJR_GURU_KNDA_LATN_MLYM_ORYA_SHRD_TAML_TELU_TIRH;
	if (codepoint >= 0xA833 && codepoint <= 0xA835) return Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KNDA_KTHI_MAHJ_MODI_NAND_SIND_TAKR_TIRH;
	if (codepoint >= 0xA830 && codepoint <= 0xA832) return Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KNDA_KTHI_MAHJ_MLYM_MODI_NAND_SIND_TAKR_TIRH;
	if (codepoint == 0x0964) return Ucd_Script_Ext_BENG_DEVA_DOGR_GONG_GONM_GRAN_GUJR_GURU_KNDA_MAHJ_MLYM_NAND_ORYA_SIND_SINH_SYLO_TAKR_TAML_TELU_TIRH;
	if (codepoint == 0x0965) return Ucd_Script_Ext_BENG_DEVA_DOGR_GONG_GONM_GRAN_GUJR_GURU_KNDA_LIMB_MAHJ_MLYM_NAND_ORYA_SIND_SINH_SYLO_TAKR_TAML_TELU_TIRH;

	return Ucd_Script_Ext_UNKNOWN;
}

const char *enum_string(Ucd_Script_Ext value) {
	static const char *strings[] = {
		"UNKNOWN",
		"BENG",
		"DEVA",
		"DUPL",
		"GREK",
		"HANI",
		"LATN",
		"NAND",
		"ARAB_COPT",
		"ARAB_ROHG",
		"ARAB_SYRC",
		"ARAB_THAA",
		"BENG_DEVA",
		"BOPO_HANI",
		"BUGI_JAVA",
		"CPRT_LINB",
		"CYRL_GLAG",
		"CYRL_LATN",
		"CYRL_PERM",
		"CYRL_SYRC",
		"DEVA_GRAN",
		"DEVA_NAND",
		"DEVA_SHRD",
		"DEVA_TAML",
		"GEOR_LATN",
		"GRAN_TAML",
		"GUJR_KHOJ",
		"GURU_MULT",
		"HANI_LATN",
		"HIRA_KANA",
		"KNDA_NAND",
		"LATN_MONG",
		"MONG_PHAG",
		"ARAB_SYRC_THAA",
		"ARAB_THAA_YEZI",
		"BENG_CAKM_SYLO",
		"CAKM_MYMR_TALE",
		"CPRT_LINA_LINB",
		"DEVA_GRAN_KNDA",
		"DEVA_GRAN_LATN",
		"HANI_HIRA_KANA",
		"KALI_LATN_MYMR",
		"BENG_DEVA_GRAN_KNDA",
		"BUHD_HANO_TAGB_TGLG",
		"DEVA_DOGR_KTHI_MAHJ",
		"ARAB_ROHG_SYRC_THAA_YEZI",
		"BOPO_HANG_HANI_HIRA_KANA",
		"BOPO_HANG_HANI_HIRA_KANA_YIII",
		"DEVA_KNDA_MLYM_ORYA_TAML_TELU",
		"ADLM_ARAB_MAND_MANI_PHLP_ROHG_SOGD_SYRC",
		"BENG_DEVA_GRAN_KNDA_NAND_ORYA_TELU_TIRH",
		"DEVA_DOGR_GUJR_GURU_KHOJ_KTHI_MAHJ_MODI_SIND_TAKR_TIRH",
		"BENG_DEVA_GRAN_GUJR_GURU_KNDA_LATN_MLYM_ORYA_TAML_TELU_TIRH",
		"BENG_DEVA_GRAN_GUJR_GURU_KNDA_LATN_MLYM_ORYA_SHRD_TAML_TELU_TIRH",
		"DEVA_DOGR_GUJR_GURU_KHOJ_KNDA_KTHI_MAHJ_MODI_NAND_SIND_TAKR_TIRH",
		"DEVA_DOGR_GUJR_GURU_KHOJ_KNDA_KTHI_MAHJ_MLYM_MODI_NAND_SIND_TAKR_TIRH",
		"BENG_DEVA_DOGR_GONG_GONM_GRAN_GUJR_GURU_KNDA_MAHJ_MLYM_NAND_ORYA_SIND_SINH_SYLO_TAKR_TAML_TELU_TIRH",
		"BENG_DEVA_DOGR_GONG_GONM_GRAN_GUJR_GURU_KNDA_LIMB_MAHJ_MLYM_NAND_ORYA_SIND_SINH_SYLO_TAKR_TAML_TELU_TIRH",
		"COUNT",
	};
	return strings[value];
}

Ucd_Block ucd_block(uint32_t codepoint) {
	if (codepoint >= 0x0000 && codepoint <= 0x007F) return Ucd_Block_BASIC_LATIN;
	if (codepoint >= 0x0080 && codepoint <= 0x00FF) return Ucd_Block_LATIN_1_SUPPLEMENT;
	if (codepoint >= 0x0100 && codepoint <= 0x017F) return Ucd_Block_LATIN_EXTENDED_A;
	if (codepoint >= 0x0180 && codepoint <= 0x024F) return Ucd_Block_LATIN_EXTENDED_B;
	if (codepoint >= 0x0250 && codepoint <= 0x02AF) return Ucd_Block_IPA_EXTENSIONS;
	if (codepoint >= 0x02B0 && codepoint <= 0x02FF) return Ucd_Block_SPACING_MODIFIER_LETTERS;
	if (codepoint >= 0x0300 && codepoint <= 0x036F) return Ucd_Block_COMBINING_DIACRITICAL_MARKS;
	if (codepoint >= 0x0370 && codepoint <= 0x03FF) return Ucd_Block_GREEK_AND_COPTIC;
	if (codepoint >= 0x0400 && codepoint <= 0x04FF) return Ucd_Block_CYRILLIC;
	if (codepoint >= 0x0500 && codepoint <= 0x052F) return Ucd_Block_CYRILLIC_SUPPLEMENT;
	if (codepoint >= 0x0530 && codepoint <= 0x058F) return Ucd_Block_ARMENIAN;
	if (codepoint >= 0x0590 && codepoint <= 0x05FF) return Ucd_Block_HEBREW;
	if (codepoint >= 0x0600 && codepoint <= 0x06FF) return Ucd_Block_ARABIC;
	if (codepoint >= 0x0700 && codepoint <= 0x074F) return Ucd_Block_SYRIAC;
	if (codepoint >= 0x0750 && codepoint <= 0x077F) return Ucd_Block_ARABIC_SUPPLEMENT;
	if (codepoint >= 0x0780 && codepoint <= 0x07BF) return Ucd_Block_THAANA;
	if (codepoint >= 0x07C0 && codepoint <= 0x07FF) return Ucd_Block_NKO;
	if (codepoint >= 0x0800 && codepoint <= 0x083F) return Ucd_Block_SAMARITAN;
	if (codepoint >= 0x0840 && codepoint <= 0x085F) return Ucd_Block_MANDAIC;
	if (codepoint >= 0x0860 && codepoint <= 0x086F) return Ucd_Block_SYRIAC_SUPPLEMENT;
	if (codepoint >= 0x08A0 && codepoint <= 0x08FF) return Ucd_Block_ARABIC_EXTENDED_A;
	if (codepoint >= 0x0900 && codepoint <= 0x097F) return Ucd_Block_DEVANAGARI;
	if (codepoint >= 0x0980 && codepoint <= 0x09FF) return Ucd_Block_BENGALI;
	if (codepoint >= 0x0A00 && codepoint <= 0x0A7F) return Ucd_Block_GURMUKHI;
	if (codepoint >= 0x0A80 && codepoint <= 0x0AFF) return Ucd_Block_GUJARATI;
	if (codepoint >= 0x0B00 && codepoint <= 0x0B7F) return Ucd_Block_ORIYA;
	if (codepoint >= 0x0B80 && codepoint <= 0x0BFF) return Ucd_Block_TAMIL;
	if (codepoint >= 0x0C00 && codepoint <= 0x0C7F) return Ucd_Block_TELUGU;
	if (codepoint >= 0x0C80 && codepoint <= 0x0CFF) return Ucd_Block_KANNADA;
	if (codepoint >= 0x0D00 && codepoint <= 0x0D7F) return Ucd_Block_MALAYALAM;
	if (codepoint >= 0x0D80 && codepoint <= 0x0DFF) return Ucd_Block_SINHALA;
	if (codepoint >= 0x0E00 && codepoint <= 0x0E7F) return Ucd_Block_THAI;
	if (codepoint >= 0x0E80 && codepoint <= 0x0EFF) return Ucd_Block_LAO;
	if (codepoint >= 0x0F00 && codepoint <= 0x0FFF) return Ucd_Block_TIBETAN;
	if (codepoint >= 0x1000 && codepoint <= 0x109F) return Ucd_Block_MYANMAR;
	if (codepoint >= 0x10A0 && codepoint <= 0x10FF) return Ucd_Block_GEORGIAN;
	if (codepoint >= 0x1100 && codepoint <= 0x11FF) return Ucd_Block_HANGUL_JAMO;
	if (codepoint >= 0x1200 && codepoint <= 0x137F) return Ucd_Block_ETHIOPIC;
	if (codepoint >= 0x1380 && codepoint <= 0x139F) return Ucd_Block_ETHIOPIC_SUPPLEMENT;
	if (codepoint >= 0x13A0 && codepoint <= 0x13FF) return Ucd_Block_CHEROKEE;
	if (codepoint >= 0x1400 && codepoint <= 0x167F) return Ucd_Block_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS;
	if (codepoint >= 0x1680 && codepoint <= 0x169F) return Ucd_Block_OGHAM;
	if (codepoint >= 0x16A0 && codepoint <= 0x16FF) return Ucd_Block_RUNIC;
	if (codepoint >= 0x1700 && codepoint <= 0x171F) return Ucd_Block_TAGALOG;
	if (codepoint >= 0x1720 && codepoint <= 0x173F) return Ucd_Block_HANUNOO;
	if (codepoint >= 0x1740 && codepoint <= 0x175F) return Ucd_Block_BUHID;
	if (codepoint >= 0x1760 && codepoint <= 0x177F) return Ucd_Block_TAGBANWA;
	if (codepoint >= 0x1780 && codepoint <= 0x17FF) return Ucd_Block_KHMER;
	if (codepoint >= 0x1800 && codepoint <= 0x18AF) return Ucd_Block_MONGOLIAN;
	if (codepoint >= 0x18B0 && codepoint <= 0x18FF) return Ucd_Block_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED;
	if (codepoint >= 0x1900 && codepoint <= 0x194F) return Ucd_Block_LIMBU;
	if (codepoint >= 0x1950 && codepoint <= 0x197F) return Ucd_Block_TAI_LE;
	if (codepoint >= 0x1980 && codepoint <= 0x19DF) return Ucd_Block_NEW_TAI_LUE;
	if (codepoint >= 0x19E0 && codepoint <= 0x19FF) return Ucd_Block_KHMER_SYMBOLS;
	if (codepoint >= 0x1A00 && codepoint <= 0x1A1F) return Ucd_Block_BUGINESE;
	if (codepoint >= 0x1A20 && codepoint <= 0x1AAF) return Ucd_Block_TAI_THAM;
	if (codepoint >= 0x1AB0 && codepoint <= 0x1AFF) return Ucd_Block_COMBINING_DIACRITICAL_MARKS_EXTENDED;
	if (codepoint >= 0x1B00 && codepoint <= 0x1B7F) return Ucd_Block_BALINESE;
	if (codepoint >= 0x1B80 && codepoint <= 0x1BBF) return Ucd_Block_SUNDANESE;
	if (codepoint >= 0x1BC0 && codepoint <= 0x1BFF) return Ucd_Block_BATAK;
	if (codepoint >= 0x1C00 && codepoint <= 0x1C4F) return Ucd_Block_LEPCHA;
	if (codepoint >= 0x1C50 && codepoint <= 0x1C7F) return Ucd_Block_OL_CHIKI;
	if (codepoint >= 0x1C80 && codepoint <= 0x1C8F) return Ucd_Block_CYRILLIC_EXTENDED_C;
	if (codepoint >= 0x1C90 && codepoint <= 0x1CBF) return Ucd_Block_GEORGIAN_EXTENDED;
	if (codepoint >= 0x1CC0 && codepoint <= 0x1CCF) return Ucd_Block_SUNDANESE_SUPPLEMENT;
	if (codepoint >= 0x1CD0 && codepoint <= 0x1CFF) return Ucd_Block_VEDIC_EXTENSIONS;
	if (codepoint >= 0x1D00 && codepoint <= 0x1D7F) return Ucd_Block_PHONETIC_EXTENSIONS;
	if (codepoint >= 0x1D80 && codepoint <= 0x1DBF) return Ucd_Block_PHONETIC_EXTENSIONS_SUPPLEMENT;
	if (codepoint >= 0x1DC0 && codepoint <= 0x1DFF) return Ucd_Block_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT;
	if (codepoint >= 0x1E00 && codepoint <= 0x1EFF) return Ucd_Block_LATIN_EXTENDED_ADDITIONAL;
	if (codepoint >= 0x1F00 && codepoint <= 0x1FFF) return Ucd_Block_GREEK_EXTENDED;
	if (codepoint >= 0x2000 && codepoint <= 0x206F) return Ucd_Block_GENERAL_PUNCTUATION;
	if (codepoint >= 0x2070 && codepoint <= 0x209F) return Ucd_Block_SUPERSCRIPTS_AND_SUBSCRIPTS;
	if (codepoint >= 0x20A0 && codepoint <= 0x20CF) return Ucd_Block_CURRENCY_SYMBOLS;
	if (codepoint >= 0x20D0 && codepoint <= 0x20FF) return Ucd_Block_COMBINING_DIACRITICAL_MARKS_FOR_SYMBOLS;
	if (codepoint >= 0x2100 && codepoint <= 0x214F) return Ucd_Block_LETTERLIKE_SYMBOLS;
	if (codepoint >= 0x2150 && codepoint <= 0x218F) return Ucd_Block_NUMBER_FORMS;
	if (codepoint >= 0x2190 && codepoint <= 0x21FF) return Ucd_Block_ARROWS;
	if (codepoint >= 0x2200 && codepoint <= 0x22FF) return Ucd_Block_MATHEMATICAL_OPERATORS;
	if (codepoint >= 0x2300 && codepoint <= 0x23FF) return Ucd_Block_MISCELLANEOUS_TECHNICAL;
	if (codepoint >= 0x2400 && codepoint <= 0x243F) return Ucd_Block_CONTROL_PICTURES;
	if (codepoint >= 0x2440 && codepoint <= 0x245F) return Ucd_Block_OPTICAL_CHARACTER_RECOGNITION;
	if (codepoint >= 0x2460 && codepoint <= 0x24FF) return Ucd_Block_ENCLOSED_ALPHANUMERICS;
	if (codepoint >= 0x2500 && codepoint <= 0x257F) return Ucd_Block_BOX_DRAWING;
	if (codepoint >= 0x2580 && codepoint <= 0x259F) return Ucd_Block_BLOCK_ELEMENTS;
	if (codepoint >= 0x25A0 && codepoint <= 0x25FF) return Ucd_Block_GEOMETRIC_SHAPES;
	if (codepoint >= 0x2600 && codepoint <= 0x26FF) return Ucd_Block_MISCELLANEOUS_SYMBOLS;
	if (codepoint >= 0x2700 && codepoint <= 0x27BF) return Ucd_Block_DINGBATS;
	if (codepoint >= 0x27C0 && codepoint <= 0x27EF) return Ucd_Block_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A;
	if (codepoint >= 0x27F0 && codepoint <= 0x27FF) return Ucd_Block_SUPPLEMENTAL_ARROWS_A;
	if (codepoint >= 0x2800 && codepoint <= 0x28FF) return Ucd_Block_BRAILLE_PATTERNS;
	if (codepoint >= 0x2900 && codepoint <= 0x297F) return Ucd_Block_SUPPLEMENTAL_ARROWS_B;
	if (codepoint >= 0x2980 && codepoint <= 0x29FF) return Ucd_Block_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B;
	if (codepoint >= 0x2A00 && codepoint <= 0x2AFF) return Ucd_Block_SUPPLEMENTAL_MATHEMATICAL_OPERATORS;
	if (codepoint >= 0x2B00 && codepoint <= 0x2BFF) return Ucd_Block_MISCELLANEOUS_SYMBOLS_AND_ARROWS;
	if (codepoint >= 0x2C00 && codepoint <= 0x2C5F) return Ucd_Block_GLAGOLITIC;
	if (codepoint >= 0x2C60 && codepoint <= 0x2C7F) return Ucd_Block_LATIN_EXTENDED_C;
	if (codepoint >= 0x2C80 && codepoint <= 0x2CFF) return Ucd_Block_COPTIC;
	if (codepoint >= 0x2D00 && codepoint <= 0x2D2F) return Ucd_Block_GEORGIAN_SUPPLEMENT;
	if (codepoint >= 0x2D30 && codepoint <= 0x2D7F) return Ucd_Block_TIFINAGH;
	if (codepoint >= 0x2D80 && codepoint <= 0x2DDF) return Ucd_Block_ETHIOPIC_EXTENDED;
	if (codepoint >= 0x2DE0 && codepoint <= 0x2DFF) return Ucd_Block_CYRILLIC_EXTENDED_A;
	if (codepoint >= 0x2E00 && codepoint <= 0x2E7F) return Ucd_Block_SUPPLEMENTAL_PUNCTUATION;
	if (codepoint >= 0x2E80 && codepoint <= 0x2EFF) return Ucd_Block_CJK_RADICALS_SUPPLEMENT;
	if (codepoint >= 0x2F00 && codepoint <= 0x2FDF) return Ucd_Block_KANGXI_RADICALS;
	if (codepoint >= 0x2FF0 && codepoint <= 0x2FFF) return Ucd_Block_IDEOGRAPHIC_DESCRIPTION_CHARACTERS;
	if (codepoint >= 0x3000 && codepoint <= 0x303F) return Ucd_Block_CJK_SYMBOLS_AND_PUNCTUATION;
	if (codepoint >= 0x3040 && codepoint <= 0x309F) return Ucd_Block_HIRAGANA;
	if (codepoint >= 0x30A0 && codepoint <= 0x30FF) return Ucd_Block_KATAKANA;
	if (codepoint >= 0x3100 && codepoint <= 0x312F) return Ucd_Block_BOPOMOFO;
	if (codepoint >= 0x3130 && codepoint <= 0x318F) return Ucd_Block_HANGUL_COMPATIBILITY_JAMO;
	if (codepoint >= 0x3190 && codepoint <= 0x319F) return Ucd_Block_KANBUN;
	if (codepoint >= 0x31A0 && codepoint <= 0x31BF) return Ucd_Block_BOPOMOFO_EXTENDED;
	if (codepoint >= 0x31C0 && codepoint <= 0x31EF) return Ucd_Block_CJK_STROKES;
	if (codepoint >= 0x31F0 && codepoint <= 0x31FF) return Ucd_Block_KATAKANA_PHONETIC_EXTENSIONS;
	if (codepoint >= 0x3200 && codepoint <= 0x32FF) return Ucd_Block_ENCLOSED_CJK_LETTERS_AND_MONTHS;
	if (codepoint >= 0x3300 && codepoint <= 0x33FF) return Ucd_Block_CJK_COMPATIBILITY;
	if (codepoint >= 0x3400 && codepoint <= 0x4DBF) return Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A;
	if (codepoint >= 0x4DC0 && codepoint <= 0x4DFF) return Ucd_Block_YIJING_HEXAGRAM_SYMBOLS;
	if (codepoint >= 0x4E00 && codepoint <= 0x9FFF) return Ucd_Block_CJK_UNIFIED_IDEOGRAPHS;
	if (codepoint >= 0xA000 && codepoint <= 0xA48F) return Ucd_Block_YI_SYLLABLES;
	if (codepoint >= 0xA490 && codepoint <= 0xA4CF) return Ucd_Block_YI_RADICALS;
	if (codepoint >= 0xA4D0 && codepoint <= 0xA4FF) return Ucd_Block_LISU;
	if (codepoint >= 0xA500 && codepoint <= 0xA63F) return Ucd_Block_VAI;
	if (codepoint >= 0xA640 && codepoint <= 0xA69F) return Ucd_Block_CYRILLIC_EXTENDED_B;
	if (codepoint >= 0xA6A0 && codepoint <= 0xA6FF) return Ucd_Block_BAMUM;
	if (codepoint >= 0xA700 && codepoint <= 0xA71F) return Ucd_Block_MODIFIER_TONE_LETTERS;
	if (codepoint >= 0xA720 && codepoint <= 0xA7FF) return Ucd_Block_LATIN_EXTENDED_D;
	if (codepoint >= 0xA800 && codepoint <= 0xA82F) return Ucd_Block_SYLOTI_NAGRI;
	if (codepoint >= 0xA830 && codepoint <= 0xA83F) return Ucd_Block_COMMON_INDIC_NUMBER_FORMS;
	if (codepoint >= 0xA840 && codepoint <= 0xA87F) return Ucd_Block_PHAGS_PA;
	if (codepoint >= 0xA880 && codepoint <= 0xA8DF) return Ucd_Block_SAURASHTRA;
	if (codepoint >= 0xA8E0 && codepoint <= 0xA8FF) return Ucd_Block_DEVANAGARI_EXTENDED;
	if (codepoint >= 0xA900 && codepoint <= 0xA92F) return Ucd_Block_KAYAH_LI;
	if (codepoint >= 0xA930 && codepoint <= 0xA95F) return Ucd_Block_REJANG;
	if (codepoint >= 0xA960 && codepoint <= 0xA97F) return Ucd_Block_HANGUL_JAMO_EXTENDED_A;
	if (codepoint >= 0xA980 && codepoint <= 0xA9DF) return Ucd_Block_JAVANESE;
	if (codepoint >= 0xA9E0 && codepoint <= 0xA9FF) return Ucd_Block_MYANMAR_EXTENDED_B;
	if (codepoint >= 0xAA00 && codepoint <= 0xAA5F) return Ucd_Block_CHAM;
	if (codepoint >= 0xAA60 && codepoint <= 0xAA7F) return Ucd_Block_MYANMAR_EXTENDED_A;
	if (codepoint >= 0xAA80 && codepoint <= 0xAADF) return Ucd_Block_TAI_VIET;
	if (codepoint >= 0xAAE0 && codepoint <= 0xAAFF) return Ucd_Block_MEETEI_MAYEK_EXTENSIONS;
	if (codepoint >= 0xAB00 && codepoint <= 0xAB2F) return Ucd_Block_ETHIOPIC_EXTENDED_A;
	if (codepoint >= 0xAB30 && codepoint <= 0xAB6F) return Ucd_Block_LATIN_EXTENDED_E;
	if (codepoint >= 0xAB70 && codepoint <= 0xABBF) return Ucd_Block_CHEROKEE_SUPPLEMENT;
	if (codepoint >= 0xABC0 && codepoint <= 0xABFF) return Ucd_Block_MEETEI_MAYEK;
	if (codepoint >= 0xAC00 && codepoint <= 0xD7AF) return Ucd_Block_HANGUL_SYLLABLES;
	if (codepoint >= 0xD7B0 && codepoint <= 0xD7FF) return Ucd_Block_HANGUL_JAMO_EXTENDED_B;
	if (codepoint >= 0xD800 && codepoint <= 0xDB7F) return Ucd_Block_HIGH_SURROGATES;
	if (codepoint >= 0xDB80 && codepoint <= 0xDBFF) return Ucd_Block_HIGH_PRIVATE_USE_SURROGATES;
	if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) return Ucd_Block_LOW_SURROGATES;
	if (codepoint >= 0xE000 && codepoint <= 0xF8FF) return Ucd_Block_PRIVATE_USE_AREA;
	if (codepoint >= 0xF900 && codepoint <= 0xFAFF) return Ucd_Block_CJK_COMPATIBILITY_IDEOGRAPHS;
	if (codepoint >= 0xFB00 && codepoint <= 0xFB4F) return Ucd_Block_ALPHABETIC_PRESENTATION_FORMS;
	if (codepoint >= 0xFB50 && codepoint <= 0xFDFF) return Ucd_Block_ARABIC_PRESENTATION_FORMS_A;
	if (codepoint >= 0xFE00 && codepoint <= 0xFE0F) return Ucd_Block_VARIATION_SELECTORS;
	if (codepoint >= 0xFE10 && codepoint <= 0xFE1F) return Ucd_Block_VERTICAL_FORMS;
	if (codepoint >= 0xFE20 && codepoint <= 0xFE2F) return Ucd_Block_COMBINING_HALF_MARKS;
	if (codepoint >= 0xFE30 && codepoint <= 0xFE4F) return Ucd_Block_CJK_COMPATIBILITY_FORMS;
	if (codepoint >= 0xFE50 && codepoint <= 0xFE6F) return Ucd_Block_SMALL_FORM_VARIANTS;
	if (codepoint >= 0xFE70 && codepoint <= 0xFEFF) return Ucd_Block_ARABIC_PRESENTATION_FORMS_B;
	if (codepoint >= 0xFF00 && codepoint <= 0xFFEF) return Ucd_Block_HALFWIDTH_AND_FULLWIDTH_FORMS;
	if (codepoint >= 0xFFF0 && codepoint <= 0xFFFF) return Ucd_Block_SPECIALS;
	if (codepoint >= 0x10000 && codepoint <= 0x1007F) return Ucd_Block_LINEAR_B_SYLLABARY;
	if (codepoint >= 0x10080 && codepoint <= 0x100FF) return Ucd_Block_LINEAR_B_IDEOGRAMS;
	if (codepoint >= 0x10100 && codepoint <= 0x1013F) return Ucd_Block_AEGEAN_NUMBERS;
	if (codepoint >= 0x10140 && codepoint <= 0x1018F) return Ucd_Block_ANCIENT_GREEK_NUMBERS;
	if (codepoint >= 0x10190 && codepoint <= 0x101CF) return Ucd_Block_ANCIENT_SYMBOLS;
	if (codepoint >= 0x101D0 && codepoint <= 0x101FF) return Ucd_Block_PHAISTOS_DISC;
	if (codepoint >= 0x10280 && codepoint <= 0x1029F) return Ucd_Block_LYCIAN;
	if (codepoint >= 0x102A0 && codepoint <= 0x102DF) return Ucd_Block_CARIAN;
	if (codepoint >= 0x102E0 && codepoint <= 0x102FF) return Ucd_Block_COPTIC_EPACT_NUMBERS;
	if (codepoint >= 0x10300 && codepoint <= 0x1032F) return Ucd_Block_OLD_ITALIC;
	if (codepoint >= 0x10330 && codepoint <= 0x1034F) return Ucd_Block_GOTHIC;
	if (codepoint >= 0x10350 && codepoint <= 0x1037F) return Ucd_Block_OLD_PERMIC;
	if (codepoint >= 0x10380 && codepoint <= 0x1039F) return Ucd_Block_UGARITIC;
	if (codepoint >= 0x103A0 && codepoint <= 0x103DF) return Ucd_Block_OLD_PERSIAN;
	if (codepoint >= 0x10400 && codepoint <= 0x1044F) return Ucd_Block_DESERET;
	if (codepoint >= 0x10450 && codepoint <= 0x1047F) return Ucd_Block_SHAVIAN;
	if (codepoint >= 0x10480 && codepoint <= 0x104AF) return Ucd_Block_OSMANYA;
	if (codepoint >= 0x104B0 && codepoint <= 0x104FF) return Ucd_Block_OSAGE;
	if (codepoint >= 0x10500 && codepoint <= 0x1052F) return Ucd_Block_ELBASAN;
	if (codepoint >= 0x10530 && codepoint <= 0x1056F) return Ucd_Block_CAUCASIAN_ALBANIAN;
	if (codepoint >= 0x10600 && codepoint <= 0x1077F) return Ucd_Block_LINEAR_A;
	if (codepoint >= 0x10800 && codepoint <= 0x1083F) return Ucd_Block_CYPRIOT_SYLLABARY;
	if (codepoint >= 0x10840 && codepoint <= 0x1085F) return Ucd_Block_IMPERIAL_ARAMAIC;
	if (codepoint >= 0x10860 && codepoint <= 0x1087F) return Ucd_Block_PALMYRENE;
	if (codepoint >= 0x10880 && codepoint <= 0x108AF) return Ucd_Block_NABATAEAN;
	if (codepoint >= 0x108E0 && codepoint <= 0x108FF) return Ucd_Block_HATRAN;
	if (codepoint >= 0x10900 && codepoint <= 0x1091F) return Ucd_Block_PHOENICIAN;
	if (codepoint >= 0x10920 && codepoint <= 0x1093F) return Ucd_Block_LYDIAN;
	if (codepoint >= 0x10980 && codepoint <= 0x1099F) return Ucd_Block_MEROITIC_HIEROGLYPHS;
	if (codepoint >= 0x109A0 && codepoint <= 0x109FF) return Ucd_Block_MEROITIC_CURSIVE;
	if (codepoint >= 0x10A00 && codepoint <= 0x10A5F) return Ucd_Block_KHAROSHTHI;
	if (codepoint >= 0x10A60 && codepoint <= 0x10A7F) return Ucd_Block_OLD_SOUTH_ARABIAN;
	if (codepoint >= 0x10A80 && codepoint <= 0x10A9F) return Ucd_Block_OLD_NORTH_ARABIAN;
	if (codepoint >= 0x10AC0 && codepoint <= 0x10AFF) return Ucd_Block_MANICHAEAN;
	if (codepoint >= 0x10B00 && codepoint <= 0x10B3F) return Ucd_Block_AVESTAN;
	if (codepoint >= 0x10B40 && codepoint <= 0x10B5F) return Ucd_Block_INSCRIPTIONAL_PARTHIAN;
	if (codepoint >= 0x10B60 && codepoint <= 0x10B7F) return Ucd_Block_INSCRIPTIONAL_PAHLAVI;
	if (codepoint >= 0x10B80 && codepoint <= 0x10BAF) return Ucd_Block_PSALTER_PAHLAVI;
	if (codepoint >= 0x10C00 && codepoint <= 0x10C4F) return Ucd_Block_OLD_TURKIC;
	if (codepoint >= 0x10C80 && codepoint <= 0x10CFF) return Ucd_Block_OLD_HUNGARIAN;
	if (codepoint >= 0x10D00 && codepoint <= 0x10D3F) return Ucd_Block_HANIFI_ROHINGYA;
	if (codepoint >= 0x10E60 && codepoint <= 0x10E7F) return Ucd_Block_RUMI_NUMERAL_SYMBOLS;
	if (codepoint >= 0x10E80 && codepoint <= 0x10EBF) return Ucd_Block_YEZIDI;
	if (codepoint >= 0x10F00 && codepoint <= 0x10F2F) return Ucd_Block_OLD_SOGDIAN;
	if (codepoint >= 0x10F30 && codepoint <= 0x10F6F) return Ucd_Block_SOGDIAN;
	if (codepoint >= 0x10FB0 && codepoint <= 0x10FDF) return Ucd_Block_CHORASMIAN;
	if (codepoint >= 0x10FE0 && codepoint <= 0x10FFF) return Ucd_Block_ELYMAIC;
	if (codepoint >= 0x11000 && codepoint <= 0x1107F) return Ucd_Block_BRAHMI;
	if (codepoint >= 0x11080 && codepoint <= 0x110CF) return Ucd_Block_KAITHI;
	if (codepoint >= 0x110D0 && codepoint <= 0x110FF) return Ucd_Block_SORA_SOMPENG;
	if (codepoint >= 0x11100 && codepoint <= 0x1114F) return Ucd_Block_CHAKMA;
	if (codepoint >= 0x11150 && codepoint <= 0x1117F) return Ucd_Block_MAHAJANI;
	if (codepoint >= 0x11180 && codepoint <= 0x111DF) return Ucd_Block_SHARADA;
	if (codepoint >= 0x111E0 && codepoint <= 0x111FF) return Ucd_Block_SINHALA_ARCHAIC_NUMBERS;
	if (codepoint >= 0x11200 && codepoint <= 0x1124F) return Ucd_Block_KHOJKI;
	if (codepoint >= 0x11280 && codepoint <= 0x112AF) return Ucd_Block_MULTANI;
	if (codepoint >= 0x112B0 && codepoint <= 0x112FF) return Ucd_Block_KHUDAWADI;
	if (codepoint >= 0x11300 && codepoint <= 0x1137F) return Ucd_Block_GRANTHA;
	if (codepoint >= 0x11400 && codepoint <= 0x1147F) return Ucd_Block_NEWA;
	if (codepoint >= 0x11480 && codepoint <= 0x114DF) return Ucd_Block_TIRHUTA;
	if (codepoint >= 0x11580 && codepoint <= 0x115FF) return Ucd_Block_SIDDHAM;
	if (codepoint >= 0x11600 && codepoint <= 0x1165F) return Ucd_Block_MODI;
	if (codepoint >= 0x11660 && codepoint <= 0x1167F) return Ucd_Block_MONGOLIAN_SUPPLEMENT;
	if (codepoint >= 0x11680 && codepoint <= 0x116CF) return Ucd_Block_TAKRI;
	if (codepoint >= 0x11700 && codepoint <= 0x1173F) return Ucd_Block_AHOM;
	if (codepoint >= 0x11800 && codepoint <= 0x1184F) return Ucd_Block_DOGRA;
	if (codepoint >= 0x118A0 && codepoint <= 0x118FF) return Ucd_Block_WARANG_CITI;
	if (codepoint >= 0x11900 && codepoint <= 0x1195F) return Ucd_Block_DIVES_AKURU;
	if (codepoint >= 0x119A0 && codepoint <= 0x119FF) return Ucd_Block_NANDINAGARI;
	if (codepoint >= 0x11A00 && codepoint <= 0x11A4F) return Ucd_Block_ZANABAZAR_SQUARE;
	if (codepoint >= 0x11A50 && codepoint <= 0x11AAF) return Ucd_Block_SOYOMBO;
	if (codepoint >= 0x11AC0 && codepoint <= 0x11AFF) return Ucd_Block_PAU_CIN_HAU;
	if (codepoint >= 0x11C00 && codepoint <= 0x11C6F) return Ucd_Block_BHAIKSUKI;
	if (codepoint >= 0x11C70 && codepoint <= 0x11CBF) return Ucd_Block_MARCHEN;
	if (codepoint >= 0x11D00 && codepoint <= 0x11D5F) return Ucd_Block_MASARAM_GONDI;
	if (codepoint >= 0x11D60 && codepoint <= 0x11DAF) return Ucd_Block_GUNJALA_GONDI;
	if (codepoint >= 0x11EE0 && codepoint <= 0x11EFF) return Ucd_Block_MAKASAR;
	if (codepoint >= 0x11FB0 && codepoint <= 0x11FBF) return Ucd_Block_LISU_SUPPLEMENT;
	if (codepoint >= 0x11FC0 && codepoint <= 0x11FFF) return Ucd_Block_TAMIL_SUPPLEMENT;
	if (codepoint >= 0x12000 && codepoint <= 0x123FF) return Ucd_Block_CUNEIFORM;
	if (codepoint >= 0x12400 && codepoint <= 0x1247F) return Ucd_Block_CUNEIFORM_NUMBERS_AND_PUNCTUATION;
	if (codepoint >= 0x12480 && codepoint <= 0x1254F) return Ucd_Block_EARLY_DYNASTIC_CUNEIFORM;
	if (codepoint >= 0x13000 && codepoint <= 0x1342F) return Ucd_Block_EGYPTIAN_HIEROGLYPHS;
	if (codepoint >= 0x13430 && codepoint <= 0x1343F) return Ucd_Block_EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS;
	if (codepoint >= 0x14400 && codepoint <= 0x1467F) return Ucd_Block_ANATOLIAN_HIEROGLYPHS;
	if (codepoint >= 0x16800 && codepoint <= 0x16A3F) return Ucd_Block_BAMUM_SUPPLEMENT;
	if (codepoint >= 0x16A40 && codepoint <= 0x16A6F) return Ucd_Block_MRO;
	if (codepoint >= 0x16AD0 && codepoint <= 0x16AFF) return Ucd_Block_BASSA_VAH;
	if (codepoint >= 0x16B00 && codepoint <= 0x16B8F) return Ucd_Block_PAHAWH_HMONG;
	if (codepoint >= 0x16E40 && codepoint <= 0x16E9F) return Ucd_Block_MEDEFAIDRIN;
	if (codepoint >= 0x16F00 && codepoint <= 0x16F9F) return Ucd_Block_MIAO;
	if (codepoint >= 0x16FE0 && codepoint <= 0x16FFF) return Ucd_Block_IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION;
	if (codepoint >= 0x17000 && codepoint <= 0x187FF) return Ucd_Block_TANGUT;
	if (codepoint >= 0x18800 && codepoint <= 0x18AFF) return Ucd_Block_TANGUT_COMPONENTS;
	if (codepoint >= 0x18B00 && codepoint <= 0x18CFF) return Ucd_Block_KHITAN_SMALL_SCRIPT;
	if (codepoint >= 0x18D00 && codepoint <= 0x18D8F) return Ucd_Block_TANGUT_SUPPLEMENT;
	if (codepoint >= 0x1B000 && codepoint <= 0x1B0FF) return Ucd_Block_KANA_SUPPLEMENT;
	if (codepoint >= 0x1B100 && codepoint <= 0x1B12F) return Ucd_Block_KANA_EXTENDED_A;
	if (codepoint >= 0x1B130 && codepoint <= 0x1B16F) return Ucd_Block_SMALL_KANA_EXTENSION;
	if (codepoint >= 0x1B170 && codepoint <= 0x1B2FF) return Ucd_Block_NUSHU;
	if (codepoint >= 0x1BC00 && codepoint <= 0x1BC9F) return Ucd_Block_DUPLOYAN;
	if (codepoint >= 0x1BCA0 && codepoint <= 0x1BCAF) return Ucd_Block_SHORTHAND_FORMAT_CONTROLS;
	if (codepoint >= 0x1D000 && codepoint <= 0x1D0FF) return Ucd_Block_BYZANTINE_MUSICAL_SYMBOLS;
	if (codepoint >= 0x1D100 && codepoint <= 0x1D1FF) return Ucd_Block_MUSICAL_SYMBOLS;
	if (codepoint >= 0x1D200 && codepoint <= 0x1D24F) return Ucd_Block_ANCIENT_GREEK_MUSICAL_NOTATION;
	if (codepoint >= 0x1D2E0 && codepoint <= 0x1D2FF) return Ucd_Block_MAYAN_NUMERALS;
	if (codepoint >= 0x1D300 && codepoint <= 0x1D35F) return Ucd_Block_TAI_XUAN_JING_SYMBOLS;
	if (codepoint >= 0x1D360 && codepoint <= 0x1D37F) return Ucd_Block_COUNTING_ROD_NUMERALS;
	if (codepoint >= 0x1D400 && codepoint <= 0x1D7FF) return Ucd_Block_MATHEMATICAL_ALPHANUMERIC_SYMBOLS;
	if (codepoint >= 0x1D800 && codepoint <= 0x1DAAF) return Ucd_Block_SUTTON_SIGNWRITING;
	if (codepoint >= 0x1E000 && codepoint <= 0x1E02F) return Ucd_Block_GLAGOLITIC_SUPPLEMENT;
	if (codepoint >= 0x1E100 && codepoint <= 0x1E14F) return Ucd_Block_NYIAKENG_PUACHUE_HMONG;
	if (codepoint >= 0x1E2C0 && codepoint <= 0x1E2FF) return Ucd_Block_WANCHO;
	if (codepoint >= 0x1E800 && codepoint <= 0x1E8DF) return Ucd_Block_MENDE_KIKAKUI;
	if (codepoint >= 0x1E900 && codepoint <= 0x1E95F) return Ucd_Block_ADLAM;
	if (codepoint >= 0x1EC70 && codepoint <= 0x1ECBF) return Ucd_Block_INDIC_SIYAQ_NUMBERS;
	if (codepoint >= 0x1ED00 && codepoint <= 0x1ED4F) return Ucd_Block_OTTOMAN_SIYAQ_NUMBERS;
	if (codepoint >= 0x1EE00 && codepoint <= 0x1EEFF) return Ucd_Block_ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS;
	if (codepoint >= 0x1F000 && codepoint <= 0x1F02F) return Ucd_Block_MAHJONG_TILES;
	if (codepoint >= 0x1F030 && codepoint <= 0x1F09F) return Ucd_Block_DOMINO_TILES;
	if (codepoint >= 0x1F0A0 && codepoint <= 0x1F0FF) return Ucd_Block_PLAYING_CARDS;
	if (codepoint >= 0x1F100 && codepoint <= 0x1F1FF) return Ucd_Block_ENCLOSED_ALPHANUMERIC_SUPPLEMENT;
	if (codepoint >= 0x1F200 && codepoint <= 0x1F2FF) return Ucd_Block_ENCLOSED_IDEOGRAPHIC_SUPPLEMENT;
	if (codepoint >= 0x1F300 && codepoint <= 0x1F5FF) return Ucd_Block_MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS;
	if (codepoint >= 0x1F600 && codepoint <= 0x1F64F) return Ucd_Block_EMOTICONS;
	if (codepoint >= 0x1F650 && codepoint <= 0x1F67F) return Ucd_Block_ORNAMENTAL_DINGBATS;
	if (codepoint >= 0x1F680 && codepoint <= 0x1F6FF) return Ucd_Block_TRANSPORT_AND_MAP_SYMBOLS;
	if (codepoint >= 0x1F700 && codepoint <= 0x1F77F) return Ucd_Block_ALCHEMICAL_SYMBOLS;
	if (codepoint >= 0x1F780 && codepoint <= 0x1F7FF) return Ucd_Block_GEOMETRIC_SHAPES_EXTENDED;
	if (codepoint >= 0x1F800 && codepoint <= 0x1F8FF) return Ucd_Block_SUPPLEMENTAL_ARROWS_C;
	if (codepoint >= 0x1F900 && codepoint <= 0x1F9FF) return Ucd_Block_SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS;
	if (codepoint >= 0x1FA00 && codepoint <= 0x1FA6F) return Ucd_Block_CHESS_SYMBOLS;
	if (codepoint >= 0x1FA70 && codepoint <= 0x1FAFF) return Ucd_Block_SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A;
	if (codepoint >= 0x1FB00 && codepoint <= 0x1FBFF) return Ucd_Block_SYMBOLS_FOR_LEGACY_COMPUTING;
	if (codepoint >= 0x20000 && codepoint <= 0x2A6DF) return Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B;
	if (codepoint >= 0x2A700 && codepoint <= 0x2B73F) return Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C;
	if (codepoint >= 0x2B740 && codepoint <= 0x2B81F) return Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D;
	if (codepoint >= 0x2B820 && codepoint <= 0x2CEAF) return Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E;
	if (codepoint >= 0x2CEB0 && codepoint <= 0x2EBEF) return Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F;
	if (codepoint >= 0x2F800 && codepoint <= 0x2FA1F) return Ucd_Block_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT;
	if (codepoint >= 0x30000 && codepoint <= 0x3134F) return Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G;
	if (codepoint >= 0xE0000 && codepoint <= 0xE007F) return Ucd_Block_TAGS;
	if (codepoint >= 0xE0100 && codepoint <= 0xE01EF) return Ucd_Block_VARIATION_SELECTORS_SUPPLEMENT;
	if (codepoint >= 0xF0000 && codepoint <= 0xFFFFF) return Ucd_Block_SUPPLEMENTARY_PRIVATE_USE_AREA_A;
	if (codepoint >= 0x100000 && codepoint <= 0x10FFFF) return Ucd_Block_SUPPLEMENTARY_PRIVATE_USE_AREA_B;

	return Ucd_Block_UNKNOWN;
}

const char *enum_string(Ucd_Block value) {
	static const char *strings[] = {
		"UNKNOWN",
		"BASIC_LATIN",
		"LATIN_1_SUPPLEMENT",
		"LATIN_EXTENDED_A",
		"LATIN_EXTENDED_B",
		"IPA_EXTENSIONS",
		"SPACING_MODIFIER_LETTERS",
		"COMBINING_DIACRITICAL_MARKS",
		"GREEK_AND_COPTIC",
		"CYRILLIC",
		"CYRILLIC_SUPPLEMENT",
		"ARMENIAN",
		"HEBREW",
		"ARABIC",
		"SYRIAC",
		"ARABIC_SUPPLEMENT",
		"THAANA",
		"NKO",
		"SAMARITAN",
		"MANDAIC",
		"SYRIAC_SUPPLEMENT",
		"ARABIC_EXTENDED_A",
		"DEVANAGARI",
		"BENGALI",
		"GURMUKHI",
		"GUJARATI",
		"ORIYA",
		"TAMIL",
		"TELUGU",
		"KANNADA",
		"MALAYALAM",
		"SINHALA",
		"THAI",
		"LAO",
		"TIBETAN",
		"MYANMAR",
		"GEORGIAN",
		"HANGUL_JAMO",
		"ETHIOPIC",
		"ETHIOPIC_SUPPLEMENT",
		"CHEROKEE",
		"UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS",
		"OGHAM",
		"RUNIC",
		"TAGALOG",
		"HANUNOO",
		"BUHID",
		"TAGBANWA",
		"KHMER",
		"MONGOLIAN",
		"UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED",
		"LIMBU",
		"TAI_LE",
		"NEW_TAI_LUE",
		"KHMER_SYMBOLS",
		"BUGINESE",
		"TAI_THAM",
		"COMBINING_DIACRITICAL_MARKS_EXTENDED",
		"BALINESE",
		"SUNDANESE",
		"BATAK",
		"LEPCHA",
		"OL_CHIKI",
		"CYRILLIC_EXTENDED_C",
		"GEORGIAN_EXTENDED",
		"SUNDANESE_SUPPLEMENT",
		"VEDIC_EXTENSIONS",
		"PHONETIC_EXTENSIONS",
		"PHONETIC_EXTENSIONS_SUPPLEMENT",
		"COMBINING_DIACRITICAL_MARKS_SUPPLEMENT",
		"LATIN_EXTENDED_ADDITIONAL",
		"GREEK_EXTENDED",
		"GENERAL_PUNCTUATION",
		"SUPERSCRIPTS_AND_SUBSCRIPTS",
		"CURRENCY_SYMBOLS",
		"COMBINING_DIACRITICAL_MARKS_FOR_SYMBOLS",
		"LETTERLIKE_SYMBOLS",
		"NUMBER_FORMS",
		"ARROWS",
		"MATHEMATICAL_OPERATORS",
		"MISCELLANEOUS_TECHNICAL",
		"CONTROL_PICTURES",
		"OPTICAL_CHARACTER_RECOGNITION",
		"ENCLOSED_ALPHANUMERICS",
		"BOX_DRAWING",
		"BLOCK_ELEMENTS",
		"GEOMETRIC_SHAPES",
		"MISCELLANEOUS_SYMBOLS",
		"DINGBATS",
		"MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A",
		"SUPPLEMENTAL_ARROWS_A",
		"BRAILLE_PATTERNS",
		"SUPPLEMENTAL_ARROWS_B",
		"MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B",
		"SUPPLEMENTAL_MATHEMATICAL_OPERATORS",
		"MISCELLANEOUS_SYMBOLS_AND_ARROWS",
		"GLAGOLITIC",
		"LATIN_EXTENDED_C",
		"COPTIC",
		"GEORGIAN_SUPPLEMENT",
		"TIFINAGH",
		"ETHIOPIC_EXTENDED",
		"CYRILLIC_EXTENDED_A",
		"SUPPLEMENTAL_PUNCTUATION",
		"CJK_RADICALS_SUPPLEMENT",
		"KANGXI_RADICALS",
		"IDEOGRAPHIC_DESCRIPTION_CHARACTERS",
		"CJK_SYMBOLS_AND_PUNCTUATION",
		"HIRAGANA",
		"KATAKANA",
		"BOPOMOFO",
		"HANGUL_COMPATIBILITY_JAMO",
		"KANBUN",
		"BOPOMOFO_EXTENDED",
		"CJK_STROKES",
		"KATAKANA_PHONETIC_EXTENSIONS",
		"ENCLOSED_CJK_LETTERS_AND_MONTHS",
		"CJK_COMPATIBILITY",
		"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A",
		"YIJING_HEXAGRAM_SYMBOLS",
		"CJK_UNIFIED_IDEOGRAPHS",
		"YI_SYLLABLES",
		"YI_RADICALS",
		"LISU",
		"VAI",
		"CYRILLIC_EXTENDED_B",
		"BAMUM",
		"MODIFIER_TONE_LETTERS",
		"LATIN_EXTENDED_D",
		"SYLOTI_NAGRI",
		"COMMON_INDIC_NUMBER_FORMS",
		"PHAGS_PA",
		"SAURASHTRA",
		"DEVANAGARI_EXTENDED",
		"KAYAH_LI",
		"REJANG",
		"HANGUL_JAMO_EXTENDED_A",
		"JAVANESE",
		"MYANMAR_EXTENDED_B",
		"CHAM",
		"MYANMAR_EXTENDED_A",
		"TAI_VIET",
		"MEETEI_MAYEK_EXTENSIONS",
		"ETHIOPIC_EXTENDED_A",
		"LATIN_EXTENDED_E",
		"CHEROKEE_SUPPLEMENT",
		"MEETEI_MAYEK",
		"HANGUL_SYLLABLES",
		"HANGUL_JAMO_EXTENDED_B",
		"HIGH_SURROGATES",
		"HIGH_PRIVATE_USE_SURROGATES",
		"LOW_SURROGATES",
		"PRIVATE_USE_AREA",
		"CJK_COMPATIBILITY_IDEOGRAPHS",
		"ALPHABETIC_PRESENTATION_FORMS",
		"ARABIC_PRESENTATION_FORMS_A",
		"VARIATION_SELECTORS",
		"VERTICAL_FORMS",
		"COMBINING_HALF_MARKS",
		"CJK_COMPATIBILITY_FORMS",
		"SMALL_FORM_VARIANTS",
		"ARABIC_PRESENTATION_FORMS_B",
		"HALFWIDTH_AND_FULLWIDTH_FORMS",
		"SPECIALS",
		"LINEAR_B_SYLLABARY",
		"LINEAR_B_IDEOGRAMS",
		"AEGEAN_NUMBERS",
		"ANCIENT_GREEK_NUMBERS",
		"ANCIENT_SYMBOLS",
		"PHAISTOS_DISC",
		"LYCIAN",
		"CARIAN",
		"COPTIC_EPACT_NUMBERS",
		"OLD_ITALIC",
		"GOTHIC",
		"OLD_PERMIC",
		"UGARITIC",
		"OLD_PERSIAN",
		"DESERET",
		"SHAVIAN",
		"OSMANYA",
		"OSAGE",
		"ELBASAN",
		"CAUCASIAN_ALBANIAN",
		"LINEAR_A",
		"CYPRIOT_SYLLABARY",
		"IMPERIAL_ARAMAIC",
		"PALMYRENE",
		"NABATAEAN",
		"HATRAN",
		"PHOENICIAN",
		"LYDIAN",
		"MEROITIC_HIEROGLYPHS",
		"MEROITIC_CURSIVE",
		"KHAROSHTHI",
		"OLD_SOUTH_ARABIAN",
		"OLD_NORTH_ARABIAN",
		"MANICHAEAN",
		"AVESTAN",
		"INSCRIPTIONAL_PARTHIAN",
		"INSCRIPTIONAL_PAHLAVI",
		"PSALTER_PAHLAVI",
		"OLD_TURKIC",
		"OLD_HUNGARIAN",
		"HANIFI_ROHINGYA",
		"RUMI_NUMERAL_SYMBOLS",
		"YEZIDI",
		"OLD_SOGDIAN",
		"SOGDIAN",
		"CHORASMIAN",
		"ELYMAIC",
		"BRAHMI",
		"KAITHI",
		"SORA_SOMPENG",
		"CHAKMA",
		"MAHAJANI",
		"SHARADA",
		"SINHALA_ARCHAIC_NUMBERS",
		"KHOJKI",
		"MULTANI",
		"KHUDAWADI",
		"GRANTHA",
		"NEWA",
		"TIRHUTA",
		"SIDDHAM",
		"MODI",
		"MONGOLIAN_SUPPLEMENT",
		"TAKRI",
		"AHOM",
		"DOGRA",
		"WARANG_CITI",
		"DIVES_AKURU",
		"NANDINAGARI",
		"ZANABAZAR_SQUARE",
		"SOYOMBO",
		"PAU_CIN_HAU",
		"BHAIKSUKI",
		"MARCHEN",
		"MASARAM_GONDI",
		"GUNJALA_GONDI",
		"MAKASAR",
		"LISU_SUPPLEMENT",
		"TAMIL_SUPPLEMENT",
		"CUNEIFORM",
		"CUNEIFORM_NUMBERS_AND_PUNCTUATION",
		"EARLY_DYNASTIC_CUNEIFORM",
		"EGYPTIAN_HIEROGLYPHS",
		"EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS",
		"ANATOLIAN_HIEROGLYPHS",
		"BAMUM_SUPPLEMENT",
		"MRO",
		"BASSA_VAH",
		"PAHAWH_HMONG",
		"MEDEFAIDRIN",
		"MIAO",
		"IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION",
		"TANGUT",
		"TANGUT_COMPONENTS",
		"KHITAN_SMALL_SCRIPT",
		"TANGUT_SUPPLEMENT",
		"KANA_SUPPLEMENT",
		"KANA_EXTENDED_A",
		"SMALL_KANA_EXTENSION",
		"NUSHU",
		"DUPLOYAN",
		"SHORTHAND_FORMAT_CONTROLS",
		"BYZANTINE_MUSICAL_SYMBOLS",
		"MUSICAL_SYMBOLS",
		"ANCIENT_GREEK_MUSICAL_NOTATION",
		"MAYAN_NUMERALS",
		"TAI_XUAN_JING_SYMBOLS",
		"COUNTING_ROD_NUMERALS",
		"MATHEMATICAL_ALPHANUMERIC_SYMBOLS",
		"SUTTON_SIGNWRITING",
		"GLAGOLITIC_SUPPLEMENT",
		"NYIAKENG_PUACHUE_HMONG",
		"WANCHO",
		"MENDE_KIKAKUI",
		"ADLAM",
		"INDIC_SIYAQ_NUMBERS",
		"OTTOMAN_SIYAQ_NUMBERS",
		"ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS",
		"MAHJONG_TILES",
		"DOMINO_TILES",
		"PLAYING_CARDS",
		"ENCLOSED_ALPHANUMERIC_SUPPLEMENT",
		"ENCLOSED_IDEOGRAPHIC_SUPPLEMENT",
		"MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS",
		"EMOTICONS",
		"ORNAMENTAL_DINGBATS",
		"TRANSPORT_AND_MAP_SYMBOLS",
		"ALCHEMICAL_SYMBOLS",
		"GEOMETRIC_SHAPES_EXTENDED",
		"SUPPLEMENTAL_ARROWS_C",
		"SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS",
		"CHESS_SYMBOLS",
		"SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A",
		"SYMBOLS_FOR_LEGACY_COMPUTING",
		"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B",
		"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C",
		"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D",
		"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E",
		"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F",
		"CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT",
		"CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G",
		"TAGS",
		"VARIATION_SELECTORS_SUPPLEMENT",
		"SUPPLEMENTARY_PRIVATE_USE_AREA_A",
		"SUPPLEMENTARY_PRIVATE_USE_AREA_B",
		"COUNT",
	};
	return strings[value];
}

Ucd_Indic_Syllable ucd_indic_syllable(uint32_t codepoint) {
	if (codepoint >= 0x0900 && codepoint <= 0x0902) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0981) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0982) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x09FC) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x0A01 && codepoint <= 0x0A02) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0A70) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x0A81 && codepoint <= 0x0A82) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0B01) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0B02) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0B82) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0C00) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x0C01 && codepoint <= 0x0C02) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0C04) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0C80) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0C81) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0C82) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x0D00 && codepoint <= 0x0D01) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0D02) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0D04) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0D81) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0D82) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0E4D) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0ECD) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0F7E) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x0F82 && codepoint <= 0x0F83) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x1036) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x17C6) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x1932) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x1A74) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x1B00 && codepoint <= 0x1B02) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x1B80) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x1C34 && codepoint <= 0x1C35) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0xA80B) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0xA873) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0xA880) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0xA8C5) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0xA8F2 && codepoint <= 0xA8F3) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0xA980 && codepoint <= 0xA981) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x10A0E) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x11000) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x11001) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x11080 && codepoint <= 0x11081) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x11100 && codepoint <= 0x11101) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x11180 && codepoint <= 0x11181) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x111CF) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x11234) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x112DF) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x11300 && codepoint <= 0x11301) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x11302) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x1135E && codepoint <= 0x1135F) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x11443 && codepoint <= 0x11444) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x1145F) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x114BF && codepoint <= 0x114C0) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x115BC && codepoint <= 0x115BD) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x1163D) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x116AB) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x11837) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x1193B && codepoint <= 0x1193C) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x119DE) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x11A35 && codepoint <= 0x11A38) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x11A96) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x11C3C && codepoint <= 0x11C3D) return Ucd_Indic_Syllable_BINDU;
	if (codepoint >= 0x11CB5 && codepoint <= 0x11CB6) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x11D40) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x11D95) return Ucd_Indic_Syllable_BINDU;
	if (codepoint == 0x0903) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0983) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0A03) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0A83) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0B03) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0C03) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0C83) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0D03) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0D83) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x0F7F) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x1038) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x17C7) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x1B04) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x1B82) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0xA881) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0xA983) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0xAAF5) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x10A0F) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11002) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11082) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11102) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11182) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11303) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11445) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x114C1) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x115BE) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x1163E) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x116AC) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11838) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x119DF) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11A39) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11A97) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11C3E) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11D41) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x11D96) return Ucd_Indic_Syllable_VISARGA;
	if (codepoint == 0x093D) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x09BD) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x0ABD) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x0B3D) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x0C3D) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x0CBD) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x0D3D) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x0F85) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x17DC) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x1BBA) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x111C1) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x1133D) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x11447) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x114C4) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x119E1) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x11A9D) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x11C40) return Ucd_Indic_Syllable_AVAGRAHA;
	if (codepoint == 0x093C) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x09BC) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x0A3C) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x0ABC) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint >= 0x0AFD && codepoint <= 0x0AFF) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x0B3C) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x0CBC) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x0F39) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x1B34) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x1BE6) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x1C37) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0xA9B3) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint >= 0x10A38 && codepoint <= 0x10A3A) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x110BA) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x11173) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x111CA) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x11236) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x112E9) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint >= 0x1133B && codepoint <= 0x1133C) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x11446) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x114C3) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x115C0) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x116B7) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x1183A) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x11943) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x11D42) return Ucd_Indic_Syllable_NUKTA;
	if (codepoint == 0x094D) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x09CD) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x0A4D) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x0ACD) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x0B4D) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x0BCD) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x0C4D) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x0CCD) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x0D4D) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x0DCA) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x1B44) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0xA806) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0xA8C4) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0xA9C0) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x11046) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x110B9) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x111C0) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x11235) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x1134D) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x11442) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x114C2) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x115BF) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x1163F) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x116B6) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x11839) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x119E0) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint == 0x11C3F) return Ucd_Indic_Syllable_VIRAMA;
	if (codepoint >= 0x0D3B && codepoint <= 0x0D3C) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x0E3A) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x0E4E) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x0EBA) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x0F84) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x103A) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x1714) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x1734) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x17D1) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x1A7A) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x1BAA) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint >= 0x1BF2 && codepoint <= 0x1BF3) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0xA82C) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0xA953) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0xABED) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x11134) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x112EA) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x1172B) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x1193D) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x11A34) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x11D44) return Ucd_Indic_Syllable_PURE_KILLER;
	if (codepoint == 0x1039) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x17D2) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x1A60) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x1BAB) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0xAAF6) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x10A3F) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x11133) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x1193E) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x11A47) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x11A99) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x11D45) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint == 0x11D97) return Ucd_Indic_Syllable_INVISIBLE_STACKER;
	if (codepoint >= 0x0904 && codepoint <= 0x0914) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0960 && codepoint <= 0x0961) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0972 && codepoint <= 0x0977) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0985 && codepoint <= 0x098C) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x098F && codepoint <= 0x0990) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0993 && codepoint <= 0x0994) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x09E0 && codepoint <= 0x09E1) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0A05 && codepoint <= 0x0A0A) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0A0F && codepoint <= 0x0A10) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0A13 && codepoint <= 0x0A14) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0A85 && codepoint <= 0x0A8D) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0A8F && codepoint <= 0x0A91) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0A93 && codepoint <= 0x0A94) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0AE0 && codepoint <= 0x0AE1) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0B05 && codepoint <= 0x0B0C) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0B0F && codepoint <= 0x0B10) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0B13 && codepoint <= 0x0B14) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0B60 && codepoint <= 0x0B61) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0B85 && codepoint <= 0x0B8A) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0B8E && codepoint <= 0x0B90) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0B92 && codepoint <= 0x0B94) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0C05 && codepoint <= 0x0C0C) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0C0E && codepoint <= 0x0C10) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0C12 && codepoint <= 0x0C14) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0C60 && codepoint <= 0x0C61) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0C85 && codepoint <= 0x0C8C) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0C8E && codepoint <= 0x0C90) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0C92 && codepoint <= 0x0C94) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0CE0 && codepoint <= 0x0CE1) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0D05 && codepoint <= 0x0D0C) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0D0E && codepoint <= 0x0D10) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0D12 && codepoint <= 0x0D14) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0D5F && codepoint <= 0x0D61) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x0D85 && codepoint <= 0x0D96) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1021 && codepoint <= 0x102A) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1052 && codepoint <= 0x1055) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1700 && codepoint <= 0x1702) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1720 && codepoint <= 0x1722) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1740 && codepoint <= 0x1742) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1760 && codepoint <= 0x1762) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x17A3 && codepoint <= 0x17B3) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1A4D && codepoint <= 0x1A52) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1B05 && codepoint <= 0x1B12) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1B83 && codepoint <= 0x1B89) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1BE4 && codepoint <= 0x1BE5) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0xA800 && codepoint <= 0xA801) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0xA803 && codepoint <= 0xA805) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0xA882 && codepoint <= 0xA891) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint == 0xA8FE) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0xA984 && codepoint <= 0xA988) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0xA98C && codepoint <= 0xA98E) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0xAA00 && codepoint <= 0xAA05) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0xAAE0 && codepoint <= 0xAAE1) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0xABCE && codepoint <= 0xABCF) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint == 0xABD1) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11005 && codepoint <= 0x11012) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11083 && codepoint <= 0x1108C) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11103 && codepoint <= 0x11106) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11183 && codepoint <= 0x11190) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11200 && codepoint <= 0x11207) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11280 && codepoint <= 0x11283) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x112B0 && codepoint <= 0x112B9) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11305 && codepoint <= 0x1130C) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x1130F && codepoint <= 0x11310) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11313 && codepoint <= 0x11314) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11360 && codepoint <= 0x11361) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11400 && codepoint <= 0x1140D) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11481 && codepoint <= 0x1148E) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11580 && codepoint <= 0x1158D) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x115D8 && codepoint <= 0x115DB) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11600 && codepoint <= 0x1160D) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11680 && codepoint <= 0x11689) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11800 && codepoint <= 0x11809) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11900 && codepoint <= 0x11906) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint == 0x11909) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x119A0 && codepoint <= 0x119A7) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x119AA && codepoint <= 0x119AD) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint == 0x11A00) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint == 0x11A50) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11C00 && codepoint <= 0x11C08) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11C0A && codepoint <= 0x11C0D) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11D00 && codepoint <= 0x11D06) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11D08 && codepoint <= 0x11D09) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint == 0x11D0B) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11D60 && codepoint <= 0x11D65) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11D67 && codepoint <= 0x11D68) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint >= 0x11D6A && codepoint <= 0x11D6B) return Ucd_Indic_Syllable_VOWEL_INDEPENDENT;
	if (codepoint == 0x093A) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x093B) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x093E && codepoint <= 0x0940) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0941 && codepoint <= 0x0948) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0949 && codepoint <= 0x094C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x094E && codepoint <= 0x094F) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0955 && codepoint <= 0x0957) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0962 && codepoint <= 0x0963) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x09BE && codepoint <= 0x09C0) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x09C1 && codepoint <= 0x09C4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x09C7 && codepoint <= 0x09C8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x09CB && codepoint <= 0x09CC) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x09D7) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x09E2 && codepoint <= 0x09E3) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0A3E && codepoint <= 0x0A40) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0A41 && codepoint <= 0x0A42) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0A47 && codepoint <= 0x0A48) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0A4B && codepoint <= 0x0A4C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0ABE && codepoint <= 0x0AC0) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0AC1 && codepoint <= 0x0AC5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0AC7 && codepoint <= 0x0AC8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0AC9) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0ACB && codepoint <= 0x0ACC) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0AE2 && codepoint <= 0x0AE3) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0B3E) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0B3F) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0B40) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0B41 && codepoint <= 0x0B44) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0B47 && codepoint <= 0x0B48) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0B4B && codepoint <= 0x0B4C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0B55 && codepoint <= 0x0B56) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0B57) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0B62 && codepoint <= 0x0B63) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0BBE && codepoint <= 0x0BBF) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0BC0) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0BC1 && codepoint <= 0x0BC2) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0BC6 && codepoint <= 0x0BC8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0BCA && codepoint <= 0x0BCC) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0BD7) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0C3E && codepoint <= 0x0C40) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0C41 && codepoint <= 0x0C44) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0C46 && codepoint <= 0x0C48) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0C4A && codepoint <= 0x0C4C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0C55 && codepoint <= 0x0C56) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0C62 && codepoint <= 0x0C63) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0CBE) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0CBF) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0CC0 && codepoint <= 0x0CC4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0CC6) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0CC7 && codepoint <= 0x0CC8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0CCA && codepoint <= 0x0CCB) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0CCC) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0CD5 && codepoint <= 0x0CD6) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0CE2 && codepoint <= 0x0CE3) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0D3E && codepoint <= 0x0D40) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0D41 && codepoint <= 0x0D44) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0D46 && codepoint <= 0x0D48) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0D4A && codepoint <= 0x0D4C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0D57) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0D62 && codepoint <= 0x0D63) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0DCF && codepoint <= 0x0DD1) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0DD2 && codepoint <= 0x0DD4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0DD6) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0DD8 && codepoint <= 0x0DDF) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0DF2 && codepoint <= 0x0DF3) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0E30) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0E31) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0E32 && codepoint <= 0x0E33) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0E34 && codepoint <= 0x0E39) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0E40 && codepoint <= 0x0E45) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0E47) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0EB0) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0EB1) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0EB2 && codepoint <= 0x0EB3) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0EB4 && codepoint <= 0x0EB9) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x0EBB) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0EC0 && codepoint <= 0x0EC4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0F71 && codepoint <= 0x0F7D) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x0F80 && codepoint <= 0x0F81) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x102B && codepoint <= 0x102C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x102D && codepoint <= 0x1030) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1031) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1032 && codepoint <= 0x1035) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1056 && codepoint <= 0x1057) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1058 && codepoint <= 0x1059) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1062) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1067 && codepoint <= 0x1068) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1071 && codepoint <= 0x1074) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1083 && codepoint <= 0x1084) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1085 && codepoint <= 0x1086) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x109C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x109D) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1712 && codepoint <= 0x1713) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1732 && codepoint <= 0x1733) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1752 && codepoint <= 0x1753) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1772 && codepoint <= 0x1773) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x17B6) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x17B7 && codepoint <= 0x17BD) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x17BE && codepoint <= 0x17C5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x17C8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1920 && codepoint <= 0x1922) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1923 && codepoint <= 0x1926) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1927 && codepoint <= 0x1928) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x193A) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x19B0 && codepoint <= 0x19C0) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1A17 && codepoint <= 0x1A18) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1A19 && codepoint <= 0x1A1A) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1A1B) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1A61) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1A62) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1A63 && codepoint <= 0x1A64) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1A65 && codepoint <= 0x1A6C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1A6D && codepoint <= 0x1A72) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1A73) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1B35) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1B36 && codepoint <= 0x1B3A) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1B3B) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1B3C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1B3D && codepoint <= 0x1B41) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1B42) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1B43) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1BA4 && codepoint <= 0x1BA5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1BA6 && codepoint <= 0x1BA7) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1BA8 && codepoint <= 0x1BA9) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1BE7) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1BE8 && codepoint <= 0x1BE9) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1BEA && codepoint <= 0x1BEC) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1BED) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1BEE) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1BEF) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1C26 && codepoint <= 0x1C2B) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1C2C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xA802) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xA823 && codepoint <= 0xA824) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xA825 && codepoint <= 0xA826) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xA827) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xA8B5 && codepoint <= 0xA8C3) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xA8FF) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xA947 && codepoint <= 0xA94E) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xA9B4 && codepoint <= 0xA9B5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xA9B6 && codepoint <= 0xA9B9) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xA9BA && codepoint <= 0xA9BB) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xA9BC) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xA9E5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAA29 && codepoint <= 0xAA2E) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAA2F && codepoint <= 0xAA30) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAA31 && codepoint <= 0xAA32) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xAAB0) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xAAB1) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAAB2 && codepoint <= 0xAAB4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAAB5 && codepoint <= 0xAAB6) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAAB7 && codepoint <= 0xAAB8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAAB9 && codepoint <= 0xAABD) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xAABE) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xAAEB) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAAEC && codepoint <= 0xAAED) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xAAEE && codepoint <= 0xAAEF) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xABE3 && codepoint <= 0xABE4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xABE5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xABE6 && codepoint <= 0xABE7) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0xABE8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0xABE9 && codepoint <= 0xABEA) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x10A01 && codepoint <= 0x10A03) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x10A05 && codepoint <= 0x10A06) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x10A0C && codepoint <= 0x10A0D) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11038 && codepoint <= 0x11045) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x110B0 && codepoint <= 0x110B2) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x110B3 && codepoint <= 0x110B6) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x110B7 && codepoint <= 0x110B8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11127 && codepoint <= 0x1112B) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x1112C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1112D && codepoint <= 0x11132) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11145 && codepoint <= 0x11146) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x111B3 && codepoint <= 0x111B5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x111B6 && codepoint <= 0x111BE) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x111BF) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x111CB && codepoint <= 0x111CC) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x111CE) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1122C && codepoint <= 0x1122E) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1122F && codepoint <= 0x11231) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11232 && codepoint <= 0x11233) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x112E0 && codepoint <= 0x112E2) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x112E3 && codepoint <= 0x112E8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1133E && codepoint <= 0x1133F) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11340) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11341 && codepoint <= 0x11344) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11347 && codepoint <= 0x11348) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1134B && codepoint <= 0x1134C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11357) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11362 && codepoint <= 0x11363) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11435 && codepoint <= 0x11437) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11438 && codepoint <= 0x1143F) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11440 && codepoint <= 0x11441) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x114B0 && codepoint <= 0x114B2) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x114B3 && codepoint <= 0x114B8) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x114B9) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x114BA) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x114BB && codepoint <= 0x114BE) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x115AF && codepoint <= 0x115B1) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x115B2 && codepoint <= 0x115B5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x115B8 && codepoint <= 0x115BB) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x115DC && codepoint <= 0x115DD) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11630 && codepoint <= 0x11632) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11633 && codepoint <= 0x1163A) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1163B && codepoint <= 0x1163C) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11640) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x116AD) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x116AE && codepoint <= 0x116AF) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x116B0 && codepoint <= 0x116B5) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11720 && codepoint <= 0x11721) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11722 && codepoint <= 0x11725) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11726) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11727 && codepoint <= 0x1172A) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1182C && codepoint <= 0x1182E) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1182F && codepoint <= 0x11836) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11930 && codepoint <= 0x11935) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11937 && codepoint <= 0x11938) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x119D1 && codepoint <= 0x119D3) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x119D4 && codepoint <= 0x119D7) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x119DA && codepoint <= 0x119DB) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x119DC && codepoint <= 0x119DD) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x119E4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11A01 && codepoint <= 0x11A0A) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11A51 && codepoint <= 0x11A56) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11A57 && codepoint <= 0x11A58) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11A59 && codepoint <= 0x11A5B) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11C2F) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11C30 && codepoint <= 0x11C36) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11C38 && codepoint <= 0x11C3B) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11CB0) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11CB1) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11CB2 && codepoint <= 0x11CB3) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11CB4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11D31 && codepoint <= 0x11D36) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11D3A) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11D3C && codepoint <= 0x11D3D) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11D3F) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint == 0x11D43) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11D8A && codepoint <= 0x11D8E) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11D90 && codepoint <= 0x11D91) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11D93 && codepoint <= 0x11D94) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11EF3 && codepoint <= 0x11EF4) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x11EF5 && codepoint <= 0x11EF6) return Ucd_Indic_Syllable_VOWEL_DEPENDENT;
	if (codepoint >= 0x1963 && codepoint <= 0x196D) return Ucd_Indic_Syllable_VOWEL;
	if (codepoint >= 0xA85E && codepoint <= 0xA861) return Ucd_Indic_Syllable_VOWEL;
	if (codepoint == 0xA866) return Ucd_Indic_Syllable_VOWEL;
	if (codepoint >= 0xA922 && codepoint <= 0xA925) return Ucd_Indic_Syllable_VOWEL;
	if (codepoint >= 0xA926 && codepoint <= 0xA92A) return Ucd_Indic_Syllable_VOWEL;
	if (codepoint >= 0x11150 && codepoint <= 0x11154) return Ucd_Indic_Syllable_VOWEL;
	if (codepoint == 0x002D) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x00A0) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x00D7) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x0980) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint >= 0x0A72 && codepoint <= 0x0A73) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x104B) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x104E) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x1900) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x1CFA) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint >= 0x2010 && codepoint <= 0x2014) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x25CC) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint >= 0xAA74 && codepoint <= 0xAA76) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x11A3F) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x11A45) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint == 0x11EF2) return Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER;
	if (codepoint >= 0x0915 && codepoint <= 0x0939) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0958 && codepoint <= 0x095F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0978 && codepoint <= 0x097F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0995 && codepoint <= 0x09A8) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x09AA && codepoint <= 0x09B0) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x09B2) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x09B6 && codepoint <= 0x09B9) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x09DC && codepoint <= 0x09DD) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x09DF) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x09F0 && codepoint <= 0x09F1) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0A15 && codepoint <= 0x0A28) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0A2A && codepoint <= 0x0A30) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0A32 && codepoint <= 0x0A33) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0A35 && codepoint <= 0x0A36) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0A38 && codepoint <= 0x0A39) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0A59 && codepoint <= 0x0A5C) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0A5E) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0A95 && codepoint <= 0x0AA8) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0AAA && codepoint <= 0x0AB0) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0AB2 && codepoint <= 0x0AB3) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0AB5 && codepoint <= 0x0AB9) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0AF9) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0B15 && codepoint <= 0x0B28) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0B2A && codepoint <= 0x0B30) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0B32 && codepoint <= 0x0B33) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0B35 && codepoint <= 0x0B39) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0B5C && codepoint <= 0x0B5D) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0B5F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0B71) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0B95) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0B99 && codepoint <= 0x0B9A) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0B9C) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0B9E && codepoint <= 0x0B9F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0BA3 && codepoint <= 0x0BA4) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0BA8 && codepoint <= 0x0BAA) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0BAE && codepoint <= 0x0BB9) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0C15 && codepoint <= 0x0C28) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0C2A && codepoint <= 0x0C39) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0C58 && codepoint <= 0x0C5A) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0C95 && codepoint <= 0x0CA8) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0CAA && codepoint <= 0x0CB3) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0CB5 && codepoint <= 0x0CB9) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0CDE) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0D15 && codepoint <= 0x0D3A) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0D9A && codepoint <= 0x0DB1) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0DB3 && codepoint <= 0x0DBB) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0DBD) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0DC0 && codepoint <= 0x0DC6) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0E01 && codepoint <= 0x0E2E) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0E81 && codepoint <= 0x0E82) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0E84) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0E86 && codepoint <= 0x0E8A) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0E8C && codepoint <= 0x0EA3) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x0EA5) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0EA7 && codepoint <= 0x0EAE) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0EDC && codepoint <= 0x0EDF) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0F40 && codepoint <= 0x0F47) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x0F49 && codepoint <= 0x0F6C) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1000 && codepoint <= 0x1020) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x103F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1050 && codepoint <= 0x1051) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x105A && codepoint <= 0x105D) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x1061) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1065 && codepoint <= 0x1066) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x106E && codepoint <= 0x1070) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1075 && codepoint <= 0x1081) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x108E) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1703 && codepoint <= 0x170C) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x170E && codepoint <= 0x1711) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1723 && codepoint <= 0x1731) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1743 && codepoint <= 0x1751) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1763 && codepoint <= 0x176C) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x176E && codepoint <= 0x1770) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1780 && codepoint <= 0x17A2) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1901 && codepoint <= 0x191E) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1950 && codepoint <= 0x1962) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1980 && codepoint <= 0x19AB) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1A00 && codepoint <= 0x1A16) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1A20 && codepoint <= 0x1A4C) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1A53 && codepoint <= 0x1A54) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1B13 && codepoint <= 0x1B33) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1B45 && codepoint <= 0x1B4B) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1B8A && codepoint <= 0x1BA0) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1BAE && codepoint <= 0x1BAF) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1BBB && codepoint <= 0x1BBD) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1BC0 && codepoint <= 0x1BE3) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1C00 && codepoint <= 0x1C23) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1C4D && codepoint <= 0x1C4F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA807 && codepoint <= 0xA80A) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA80C && codepoint <= 0xA822) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA840 && codepoint <= 0xA85D) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA862 && codepoint <= 0xA865) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA869 && codepoint <= 0xA870) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0xA872) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA892 && codepoint <= 0xA8B3) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA90A && codepoint <= 0xA921) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA930 && codepoint <= 0xA946) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA989 && codepoint <= 0xA98B) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA98F && codepoint <= 0xA9B2) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA9E0 && codepoint <= 0xA9E4) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA9E7 && codepoint <= 0xA9EF) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xA9FA && codepoint <= 0xA9FE) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xAA06 && codepoint <= 0xAA28) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xAA60 && codepoint <= 0xAA6F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xAA71 && codepoint <= 0xAA73) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0xAA7A) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xAA7E && codepoint <= 0xAA7F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xAA80 && codepoint <= 0xAAAF) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xAAE2 && codepoint <= 0xAAEA) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xABC0 && codepoint <= 0xABCD) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0xABD0) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0xABD2 && codepoint <= 0xABDA) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x10A00) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x10A10 && codepoint <= 0x10A13) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x10A15 && codepoint <= 0x10A17) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x10A19 && codepoint <= 0x10A35) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11013 && codepoint <= 0x11037) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1108D && codepoint <= 0x110AF) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11107 && codepoint <= 0x11126) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x11144) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x11147) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11155 && codepoint <= 0x11172) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11191 && codepoint <= 0x111B2) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11208 && codepoint <= 0x11211) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11213 && codepoint <= 0x1122B) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11284 && codepoint <= 0x11286) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x11288) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1128A && codepoint <= 0x1128D) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1128F && codepoint <= 0x1129D) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1129F && codepoint <= 0x112A8) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x112BA && codepoint <= 0x112DE) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11315 && codepoint <= 0x11328) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1132A && codepoint <= 0x11330) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11332 && codepoint <= 0x11333) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11335 && codepoint <= 0x11339) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1140E && codepoint <= 0x11434) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1148F && codepoint <= 0x114AF) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1158E && codepoint <= 0x115AE) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1160E && codepoint <= 0x1162F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1168A && codepoint <= 0x116AA) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x116B8) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11700 && codepoint <= 0x1171A) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1180A && codepoint <= 0x1182B) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x1190C && codepoint <= 0x11913) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11915 && codepoint <= 0x11916) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11918 && codepoint <= 0x1192F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x119AE && codepoint <= 0x119D0) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11A0B && codepoint <= 0x11A32) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11A5C && codepoint <= 0x11A83) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11C0E && codepoint <= 0x11C2E) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11C72 && codepoint <= 0x11C8F) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11D0C && codepoint <= 0x11D30) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11D6C && codepoint <= 0x11D89) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint >= 0x11EE0 && codepoint <= 0x11EF1) return Ucd_Indic_Syllable_CONSONANT;
	if (codepoint == 0x09CE) return Ucd_Indic_Syllable_CONSONANT_DEAD;
	if (codepoint >= 0x0D54 && codepoint <= 0x0D56) return Ucd_Indic_Syllable_CONSONANT_DEAD;
	if (codepoint >= 0x0D7A && codepoint <= 0x0D7F) return Ucd_Indic_Syllable_CONSONANT_DEAD;
	if (codepoint >= 0x1CF2 && codepoint <= 0x1CF3) return Ucd_Indic_Syllable_CONSONANT_DEAD;
	if (codepoint >= 0x0CF1 && codepoint <= 0x0CF2) return Ucd_Indic_Syllable_CONSONANT_WITH_STACKER;
	if (codepoint >= 0x1CF5 && codepoint <= 0x1CF6) return Ucd_Indic_Syllable_CONSONANT_WITH_STACKER;
	if (codepoint >= 0x11003 && codepoint <= 0x11004) return Ucd_Indic_Syllable_CONSONANT_WITH_STACKER;
	if (codepoint >= 0x11460 && codepoint <= 0x11461) return Ucd_Indic_Syllable_CONSONANT_WITH_STACKER;
	if (codepoint >= 0x111C2 && codepoint <= 0x111C3) return Ucd_Indic_Syllable_CONSONANT_PREFIXED;
	if (codepoint == 0x1193F) return Ucd_Indic_Syllable_CONSONANT_PREFIXED;
	if (codepoint == 0x11A3A) return Ucd_Indic_Syllable_CONSONANT_PREFIXED;
	if (codepoint >= 0x11A84 && codepoint <= 0x11A89) return Ucd_Indic_Syllable_CONSONANT_PREFIXED;
	if (codepoint == 0x0D4E) return Ucd_Indic_Syllable_CONSONANT_PRECEDING_REPHA;
	if (codepoint == 0x11941) return Ucd_Indic_Syllable_CONSONANT_PRECEDING_REPHA;
	if (codepoint == 0x11D46) return Ucd_Indic_Syllable_CONSONANT_PRECEDING_REPHA;
	if (codepoint == 0x1A5A) return Ucd_Indic_Syllable_CONSONANT_INITIAL_POSTFIXED;
	if (codepoint == 0x17CC) return Ucd_Indic_Syllable_CONSONANT_SUCCEEDING_REPHA;
	if (codepoint == 0x1B03) return Ucd_Indic_Syllable_CONSONANT_SUCCEEDING_REPHA;
	if (codepoint == 0x1B81) return Ucd_Indic_Syllable_CONSONANT_SUCCEEDING_REPHA;
	if (codepoint == 0xA982) return Ucd_Indic_Syllable_CONSONANT_SUCCEEDING_REPHA;
	if (codepoint >= 0x0F8D && codepoint <= 0x0F97) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0x0F99 && codepoint <= 0x0FBC) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0x1929 && codepoint <= 0x192B) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint == 0x1A57) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0x1A5B && codepoint <= 0x1A5E) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint == 0x1BA1) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0x1BA2 && codepoint <= 0x1BA3) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0x1BAC && codepoint <= 0x1BAD) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0x1C24 && codepoint <= 0x1C25) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0xA867 && codepoint <= 0xA868) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint == 0xA871) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0x11C92 && codepoint <= 0x11CA7) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint == 0x11CA9) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint >= 0x11CAA && codepoint <= 0x11CAF) return Ucd_Indic_Syllable_CONSONANT_SUBJOINED;
	if (codepoint == 0x0A75) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0x0EBC) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0x0EBD) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0x103B && codepoint <= 0x103C) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0x103D && codepoint <= 0x103E) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0x105E && codepoint <= 0x1060) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0x1082) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0x1A55) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0x1A56) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0xA8B4) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0xA9BD) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0xA9BE && codepoint <= 0xA9BF) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0xAA33 && codepoint <= 0xAA34) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0xAA35 && codepoint <= 0xAA36) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0x1171D && codepoint <= 0x1171F) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0x11940) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0x11942) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0x11A3B && codepoint <= 0x11A3E) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint == 0x11D47) return Ucd_Indic_Syllable_CONSONANT_MEDIAL;
	if (codepoint >= 0x1930 && codepoint <= 0x1931) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0x1933 && codepoint <= 0x1938) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint == 0x1939) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0x19C1 && codepoint <= 0x19C7) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0x1A58 && codepoint <= 0x1A59) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0x1BBE && codepoint <= 0x1BBF) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0x1BF0 && codepoint <= 0x1BF1) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0x1C2D && codepoint <= 0x1C33) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0xA94F && codepoint <= 0xA951) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint == 0xA952) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0xAA40 && codepoint <= 0xAA42) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint == 0xAA43) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0xAA44 && codepoint <= 0xAA4B) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint == 0xAA4C) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint == 0xAA4D) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0xABDB && codepoint <= 0xABE2) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0x11A8A && codepoint <= 0x11A95) return Ucd_Indic_Syllable_CONSONANT_FINAL;
	if (codepoint >= 0x0F88 && codepoint <= 0x0F8C) return Ucd_Indic_Syllable_CONSONANT_HEAD_LETTER;
	if (codepoint == 0x0B83) return Ucd_Indic_Syllable_MODIFYING_LETTER;
	if (codepoint >= 0x1970 && codepoint <= 0x1974) return Ucd_Indic_Syllable_TONE_LETTER;
	if (codepoint == 0xAAC0) return Ucd_Indic_Syllable_TONE_LETTER;
	if (codepoint == 0xAAC2) return Ucd_Indic_Syllable_TONE_LETTER;
	if (codepoint >= 0x0E48 && codepoint <= 0x0E4B) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint >= 0x0EC8 && codepoint <= 0x0ECB) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0x1037) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint >= 0x1063 && codepoint <= 0x1064) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint >= 0x1069 && codepoint <= 0x106D) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint >= 0x1087 && codepoint <= 0x108C) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0x108D) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0x108F) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint >= 0x109A && codepoint <= 0x109B) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint >= 0x19C8 && codepoint <= 0x19C9) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint >= 0x1A75 && codepoint <= 0x1A79) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint >= 0xA92B && codepoint <= 0xA92D) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0xAA7B) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0xAA7C) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0xAA7D) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0xAABF) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0xAAC1) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0xABEC) return Ucd_Indic_Syllable_TONE_MARK;
	if (codepoint == 0x0A71) return Ucd_Indic_Syllable_GEMINATION_MARK;
	if (codepoint == 0x11237) return Ucd_Indic_Syllable_GEMINATION_MARK;
	if (codepoint == 0x11A98) return Ucd_Indic_Syllable_GEMINATION_MARK;
	if (codepoint >= 0x0951 && codepoint <= 0x0952) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint == 0x0A51) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint >= 0x0AFA && codepoint <= 0x0AFC) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint >= 0x1CD0 && codepoint <= 0x1CD2) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint >= 0x1CD4 && codepoint <= 0x1CE0) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint == 0x1CE1) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint == 0x1CF4) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint == 0x1CF7) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint >= 0x1CF8 && codepoint <= 0x1CF9) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint == 0x20F0) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint >= 0xA8E0 && codepoint <= 0xA8F1) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint == 0x1123E) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint >= 0x11366 && codepoint <= 0x1136C) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint >= 0x11370 && codepoint <= 0x11374) return Ucd_Indic_Syllable_CANTILLATION_MARK;
	if (codepoint >= 0x17C9 && codepoint <= 0x17CA) return Ucd_Indic_Syllable_REGISTER_SHIFTER;
	if (codepoint >= 0x00B2 && codepoint <= 0x00B3) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x09FE) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x0F35) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x0F37) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x0FC6) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x17CB) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint >= 0x17CE && codepoint <= 0x17D0) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x17D3) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x17DD) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x193B) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint >= 0x1A7B && codepoint <= 0x1A7C) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x1A7F) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x1C36) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x1DFB) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x2074) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint >= 0x2082 && codepoint <= 0x2084) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x111C9) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x1145E) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x11A33) return Ucd_Indic_Syllable_SYLLABLE_MODIFIER;
	if (codepoint == 0x0E4C) return Ucd_Indic_Syllable_CONSONANT_KILLER;
	if (codepoint == 0x17CD) return Ucd_Indic_Syllable_CONSONANT_KILLER;
	if (codepoint == 0x200C) return Ucd_Indic_Syllable_NON_JOINER;
	if (codepoint == 0x200D) return Ucd_Indic_Syllable_JOINER;
	if (codepoint == 0x1107F) return Ucd_Indic_Syllable_NUMBER_JOINER;
	if (codepoint >= 0x0030 && codepoint <= 0x0039) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0966 && codepoint <= 0x096F) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x09E6 && codepoint <= 0x09EF) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0A66 && codepoint <= 0x0A6F) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0AE6 && codepoint <= 0x0AEF) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0B66 && codepoint <= 0x0B6F) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0BE6 && codepoint <= 0x0BEF) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0C66 && codepoint <= 0x0C6F) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0CE6 && codepoint <= 0x0CEF) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0D66 && codepoint <= 0x0D6F) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0DE6 && codepoint <= 0x0DEF) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0E50 && codepoint <= 0x0E59) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0ED0 && codepoint <= 0x0ED9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0F20 && codepoint <= 0x0F29) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x0F2A && codepoint <= 0x0F33) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1040 && codepoint <= 0x1049) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1090 && codepoint <= 0x1099) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x17E0 && codepoint <= 0x17E9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1946 && codepoint <= 0x194F) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x19D0 && codepoint <= 0x19D9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint == 0x19DA) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1A80 && codepoint <= 0x1A89) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1A90 && codepoint <= 0x1A99) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1B50 && codepoint <= 0x1B59) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1BB0 && codepoint <= 0x1BB9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1C40 && codepoint <= 0x1C49) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0xA8D0 && codepoint <= 0xA8D9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0xA900 && codepoint <= 0xA909) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0xA9D0 && codepoint <= 0xA9D9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0xA9F0 && codepoint <= 0xA9F9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0xAA50 && codepoint <= 0xAA59) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0xABF0 && codepoint <= 0xABF9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x10A40 && codepoint <= 0x10A48) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11066 && codepoint <= 0x1106F) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11136 && codepoint <= 0x1113F) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x111D0 && codepoint <= 0x111D9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x111E1 && codepoint <= 0x111F4) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x112F0 && codepoint <= 0x112F9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11450 && codepoint <= 0x11459) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x114D0 && codepoint <= 0x114D9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11650 && codepoint <= 0x11659) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x116C0 && codepoint <= 0x116C9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11730 && codepoint <= 0x11739) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x1173A && codepoint <= 0x1173B) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11950 && codepoint <= 0x11959) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11C50 && codepoint <= 0x11C59) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11C5A && codepoint <= 0x11C6C) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11D50 && codepoint <= 0x11D59) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11DA0 && codepoint <= 0x11DA9) return Ucd_Indic_Syllable_NUMBER;
	if (codepoint >= 0x11052 && codepoint <= 0x11065) return Ucd_Indic_Syllable_BRAHMI_JOINING_NUMBER;

	return Ucd_Indic_Syllable_UNKNOWN;
}

const char *enum_string(Ucd_Indic_Syllable value) {
	static const char *strings[] = {
		"UNKNOWN",
		"BINDU",
		"VISARGA",
		"AVAGRAHA",
		"NUKTA",
		"VIRAMA",
		"PURE_KILLER",
		"INVISIBLE_STACKER",
		"VOWEL_INDEPENDENT",
		"VOWEL_DEPENDENT",
		"VOWEL",
		"CONSONANT_PLACEHOLDER",
		"CONSONANT",
		"CONSONANT_DEAD",
		"CONSONANT_WITH_STACKER",
		"CONSONANT_PREFIXED",
		"CONSONANT_PRECEDING_REPHA",
		"CONSONANT_INITIAL_POSTFIXED",
		"CONSONANT_SUCCEEDING_REPHA",
		"CONSONANT_SUBJOINED",
		"CONSONANT_MEDIAL",
		"CONSONANT_FINAL",
		"CONSONANT_HEAD_LETTER",
		"MODIFYING_LETTER",
		"TONE_LETTER",
		"TONE_MARK",
		"GEMINATION_MARK",
		"CANTILLATION_MARK",
		"REGISTER_SHIFTER",
		"SYLLABLE_MODIFIER",
		"CONSONANT_KILLER",
		"NON_JOINER",
		"JOINER",
		"NUMBER_JOINER",
		"NUMBER",
		"BRAHMI_JOINING_NUMBER",
		"COUNT",
	};
	return strings[value];
}

Ucd_Indic_Position ucd_indic_position(uint32_t codepoint) {
	if (codepoint == 0x0903) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x093B) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x093E) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0940) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0949 && codepoint <= 0x094C) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x094F) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0982 && codepoint <= 0x0983) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x09BE) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x09C0) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x09D7) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0A03) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0A3E) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0A40) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0A83) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0ABE) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0AC0) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0ACB && codepoint <= 0x0ACC) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0B02 && codepoint <= 0x0B03) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0B3E) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0B40) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0BBE && codepoint <= 0x0BBF) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0BC1 && codepoint <= 0x0BC2) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0BD7) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0C01 && codepoint <= 0x0C03) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0C41 && codepoint <= 0x0C44) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0C82 && codepoint <= 0x0C83) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0CBE) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0CC1 && codepoint <= 0x0CC4) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0CD5 && codepoint <= 0x0CD6) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0D02 && codepoint <= 0x0D03) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0D3E && codepoint <= 0x0D40) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0D41 && codepoint <= 0x0D42) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0D57) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0D82 && codepoint <= 0x0D83) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0DCF && codepoint <= 0x0DD1) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0DD8) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0DDF) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0DF2 && codepoint <= 0x0DF3) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0E30) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0E32 && codepoint <= 0x0E33) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0E45) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0EB0) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x0EB2 && codepoint <= 0x0EB3) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0F3E) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x0F7F) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x102B && codepoint <= 0x102C) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1038) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x103B) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1056 && codepoint <= 0x1057) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1062 && codepoint <= 0x1064) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1067 && codepoint <= 0x106D) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1083) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1087 && codepoint <= 0x108C) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x108F) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x109A && codepoint <= 0x109C) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x17B6) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x17C7 && codepoint <= 0x17C8) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1923 && codepoint <= 0x1924) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1929 && codepoint <= 0x192B) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1930 && codepoint <= 0x1931) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1933 && codepoint <= 0x1938) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x19B0 && codepoint <= 0x19B4) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x19B8 && codepoint <= 0x19B9) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x19BB && codepoint <= 0x19C0) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x19C8 && codepoint <= 0x19C9) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1A1A) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1A57) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1A61) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1A63 && codepoint <= 0x1A64) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1A6D) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1B04) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1B35) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1B44) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1B82) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1BA1) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1BA7) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1BAA) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1BE7) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1BEA && codepoint <= 0x1BEC) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1BEE) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1BF2 && codepoint <= 0x1BF3) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1C24 && codepoint <= 0x1C26) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1C2A && codepoint <= 0x1C2B) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1CE1) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1CF7) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0xA823 && codepoint <= 0xA824) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xA827) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0xA880 && codepoint <= 0xA881) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0xA8B4 && codepoint <= 0xA8C3) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0xA952 && codepoint <= 0xA953) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xA983) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0xA9B4 && codepoint <= 0xA9B5) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAA33) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAA4D) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAA7B) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAA7D) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAAB1) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAABA) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAABD) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAAEF) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xAAF5) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0xABE3 && codepoint <= 0xABE4) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0xABE6 && codepoint <= 0xABE7) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0xABE9 && codepoint <= 0xABEA) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0xABEC) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11000) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11002) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11082) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x110B0) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x110B2) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x110B7 && codepoint <= 0x110B8) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11145 && codepoint <= 0x11146) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11182) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x111B3) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x111B5) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x111C0) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1122C && codepoint <= 0x1122E) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11235) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x112E0) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x112E2) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11302 && codepoint <= 0x11303) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1133E && codepoint <= 0x1133F) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11341 && codepoint <= 0x11344) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1134D) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11357) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11362 && codepoint <= 0x11363) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11435) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11437) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11440 && codepoint <= 0x11441) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11445) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x114B0) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x114B2) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x114BD) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x114C1) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x115AF) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x115B1) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x115BE) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11630 && codepoint <= 0x11632) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x1163B && codepoint <= 0x1163C) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1163E) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x116AC) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x116AF) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x116B6) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11720 && codepoint <= 0x11721) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1182C) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1182E) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11838) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11930 && codepoint <= 0x11934) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x1193D) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11940) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x119D1) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x119D3) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x119DC && codepoint <= 0x119DF) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11A39) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11A57 && codepoint <= 0x11A58) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11A97) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11C2F) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11C3E) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11CA9) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11CB4) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11D46) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11D8A && codepoint <= 0x11D8E) return Ucd_Indic_Position_RIGHT;
	if (codepoint >= 0x11D93 && codepoint <= 0x11D94) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11D96) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x11EF6) return Ucd_Indic_Position_RIGHT;
	if (codepoint == 0x093F) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x094E) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x09BF) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x09C7 && codepoint <= 0x09C8) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x0A3F) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x0ABF) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x0B47) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x0BC6 && codepoint <= 0x0BC8) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x0D46 && codepoint <= 0x0D48) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x0DD9) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x0DDB) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x0F3F) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x1031) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x1084) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x17C1 && codepoint <= 0x17C3) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x1A19) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x1A55) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x1A6E && codepoint <= 0x1A72) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x1B3E && codepoint <= 0x1B3F) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x1BA6) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x1C27 && codepoint <= 0x1C28) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x1C34 && codepoint <= 0x1C35) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0xA9BA && codepoint <= 0xA9BB) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0xAA2F && codepoint <= 0xAA30) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0xAA34) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0xAAEB) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0xAAEE) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x110B1) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x1112C) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x111B4) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x111CE) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x112E1) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x11347 && codepoint <= 0x11348) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x11436) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x114B1) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x114B9) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x115B0) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x115B8) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x116AE) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x11726) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x1182D) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x11935) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x11937) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x119D2) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x119E4) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x11CB1) return Ucd_Indic_Position_LEFT;
	if (codepoint == 0x11EF5) return Ucd_Indic_Position_LEFT;
	if (codepoint >= 0x0E40 && codepoint <= 0x0E44) return Ucd_Indic_Position_VISUAL_ORDER_LEFT;
	if (codepoint >= 0x0EC0 && codepoint <= 0x0EC4) return Ucd_Indic_Position_VISUAL_ORDER_LEFT;
	if (codepoint >= 0x19B5 && codepoint <= 0x19B7) return Ucd_Indic_Position_VISUAL_ORDER_LEFT;
	if (codepoint == 0x19BA) return Ucd_Indic_Position_VISUAL_ORDER_LEFT;
	if (codepoint >= 0xAAB5 && codepoint <= 0xAAB6) return Ucd_Indic_Position_VISUAL_ORDER_LEFT;
	if (codepoint == 0xAAB9) return Ucd_Indic_Position_VISUAL_ORDER_LEFT;
	if (codepoint >= 0xAABB && codepoint <= 0xAABC) return Ucd_Indic_Position_VISUAL_ORDER_LEFT;
	if (codepoint >= 0x09CB && codepoint <= 0x09CC) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint == 0x0B4B) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint >= 0x0BCA && codepoint <= 0x0BCC) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint >= 0x0D4A && codepoint <= 0x0D4C) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint == 0x0DDC) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint == 0x0DDE) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint == 0x17C0) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint >= 0x17C4 && codepoint <= 0x17C5) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint >= 0x1B40 && codepoint <= 0x1B41) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint >= 0x1134B && codepoint <= 0x1134C) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint == 0x114BC) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint == 0x114BE) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint == 0x115BA) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint == 0x11938) return Ucd_Indic_Position_LEFT_AND_RIGHT;
	if (codepoint >= 0x0900 && codepoint <= 0x0902) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x093A) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0945 && codepoint <= 0x0948) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0951) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0953 && codepoint <= 0x0955) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0981) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x09FE) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0A01 && codepoint <= 0x0A02) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0A47 && codepoint <= 0x0A48) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0A4B && codepoint <= 0x0A4C) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0A70 && codepoint <= 0x0A71) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0A81 && codepoint <= 0x0A82) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0AC5) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0AC7 && codepoint <= 0x0AC8) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0AFA && codepoint <= 0x0AFF) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0B01) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0B3F) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0B55 && codepoint <= 0x0B56) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0B82) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0BC0) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0BCD) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0C00) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0C04) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0C3E && codepoint <= 0x0C40) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0C46 && codepoint <= 0x0C47) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0C4A && codepoint <= 0x0C4D) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0C55) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0C81) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0CBF) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0CC6) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0CCC && codepoint <= 0x0CCD) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0D00 && codepoint <= 0x0D01) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0D3B && codepoint <= 0x0D3C) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0D4D) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0D4E) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0D81) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0DCA) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0DD2 && codepoint <= 0x0DD3) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0E31) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0E34 && codepoint <= 0x0E37) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0E47 && codepoint <= 0x0E4E) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0EB1) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0EB4 && codepoint <= 0x0EB7) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0EBB) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0EC8 && codepoint <= 0x0ECD) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0F39) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0F72) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0F7A && codepoint <= 0x0F7E) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x0F80) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0F82 && codepoint <= 0x0F83) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x0F86 && codepoint <= 0x0F87) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x102D && codepoint <= 0x102E) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1032 && codepoint <= 0x1036) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x103A) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1071 && codepoint <= 0x1074) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1085 && codepoint <= 0x1086) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x109D) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1712) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1732) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1752) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1772) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x17B7 && codepoint <= 0x17BA) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x17C6) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x17C9 && codepoint <= 0x17D1) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x17D3) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x17DD) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1920 && codepoint <= 0x1921) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1927 && codepoint <= 0x1928) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x193A) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1A17) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1A1B) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1A58 && codepoint <= 0x1A5A) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1A62) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1A65 && codepoint <= 0x1A68) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1A6B) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1A73 && codepoint <= 0x1A7C) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1B00 && codepoint <= 0x1B03) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1B34) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1B36 && codepoint <= 0x1B37) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1B42) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1B6B) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1B6D && codepoint <= 0x1B73) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1B80 && codepoint <= 0x1B81) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1BA4) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1BA8 && codepoint <= 0x1BA9) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1BE6) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1BE8 && codepoint <= 0x1BE9) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1BED) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1BEF && codepoint <= 0x1BF1) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1C2D && codepoint <= 0x1C33) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1C36) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1CD0 && codepoint <= 0x1CD2) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1CDA && codepoint <= 0x1CDB) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1CE0) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1CF4) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1DFB) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x20F0) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA802) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA806) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA80B) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA826) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA8C5) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0xA8E0 && codepoint <= 0xA8F1) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA8FF) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA94A) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0xA94F && codepoint <= 0xA951) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0xA980 && codepoint <= 0xA982) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA9B3) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0xA9B6 && codepoint <= 0xA9B7) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA9BC) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xA9E5) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0xAA29 && codepoint <= 0xAA2C) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xAA2E) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xAA31) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xAA43) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xAA4C) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xAA7C) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xAAB0) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0xAAB2 && codepoint <= 0xAAB3) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0xAAB7 && codepoint <= 0xAAB8) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0xAABE && codepoint <= 0xAABF) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xAAC1) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xAAED) return Ucd_Indic_Position_TOP;
	if (codepoint == 0xABE5) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x10A05) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x10A0F) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x10A38) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11001) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11038 && codepoint <= 0x1103B) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11042 && codepoint <= 0x11046) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11080 && codepoint <= 0x11081) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x110B5 && codepoint <= 0x110B6) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11100 && codepoint <= 0x11102) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11127 && codepoint <= 0x11129) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1112D) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11130) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11134) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11180 && codepoint <= 0x11181) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x111BC && codepoint <= 0x111BE) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x111C2 && codepoint <= 0x111C3) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x111CB) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x111CF) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11230 && codepoint <= 0x11231) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11234) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11236 && codepoint <= 0x11237) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1123E) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x112DF) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x112E5 && codepoint <= 0x112E8) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11300 && codepoint <= 0x11301) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11340) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11366 && codepoint <= 0x1136C) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11370 && codepoint <= 0x11374) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1143E && codepoint <= 0x1143F) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11443 && codepoint <= 0x11444) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1145E) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x114BA) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x114BF && codepoint <= 0x114C0) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x115BC && codepoint <= 0x115BD) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11639 && codepoint <= 0x1163A) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1163D) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11640) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x116AB) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x116AD) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x116B2 && codepoint <= 0x116B5) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1171F) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11722 && codepoint <= 0x11723) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11727) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11729 && codepoint <= 0x1172B) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11833 && codepoint <= 0x11837) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x1193B && codepoint <= 0x1193C) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x1193F) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11941) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x119DA && codepoint <= 0x119DB) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11A01) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11A04 && codepoint <= 0x11A09) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11A35 && codepoint <= 0x11A38) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11A3A) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11A51) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11A54 && codepoint <= 0x11A56) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11A84 && codepoint <= 0x11A89) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11A96) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11A98) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11C30 && codepoint <= 0x11C31) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11C38 && codepoint <= 0x11C3D) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11CB3) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11CB5 && codepoint <= 0x11CB6) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11D31 && codepoint <= 0x11D35) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11D3A) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11D3C && codepoint <= 0x11D3D) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11D3F && codepoint <= 0x11D41) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11D43) return Ucd_Indic_Position_TOP;
	if (codepoint >= 0x11D90 && codepoint <= 0x11D91) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11D95) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x11EF3) return Ucd_Indic_Position_TOP;
	if (codepoint == 0x093C) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0941 && codepoint <= 0x0944) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x094D) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0952) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0956 && codepoint <= 0x0957) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0962 && codepoint <= 0x0963) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x09BC) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x09C1 && codepoint <= 0x09C4) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x09CD) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x09E2 && codepoint <= 0x09E3) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0A3C) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0A41 && codepoint <= 0x0A42) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0A4D) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0A51) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0A75) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0ABC) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0AC1 && codepoint <= 0x0AC4) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0ACD) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0AE2 && codepoint <= 0x0AE3) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0B3C) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0B41 && codepoint <= 0x0B44) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0B4D) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0B62 && codepoint <= 0x0B63) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0C56) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0C62 && codepoint <= 0x0C63) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0CBC) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0CE2 && codepoint <= 0x0CE3) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0D43 && codepoint <= 0x0D44) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0D62 && codepoint <= 0x0D63) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0DD4) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0DD6) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0E38 && codepoint <= 0x0E3A) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0EB8 && codepoint <= 0x0EBA) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0EBC) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0F18 && codepoint <= 0x0F19) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0F35) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0F37) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0F71) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0F74 && codepoint <= 0x0F75) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0F84) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0F8D && codepoint <= 0x0F97) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x0F99 && codepoint <= 0x0FBC) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0FC6) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x102F && codepoint <= 0x1030) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1037) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x103D && codepoint <= 0x103E) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1058 && codepoint <= 0x1059) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x105E && codepoint <= 0x1060) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1082) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x108D) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1713 && codepoint <= 0x1714) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1733 && codepoint <= 0x1734) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1753) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1773) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x17BB && codepoint <= 0x17BD) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1922) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1932) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1939) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x193B) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1A18) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1A56) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1A5B && codepoint <= 0x1A5E) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1A69 && codepoint <= 0x1A6A) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1A6C) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1A7F) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1B38 && codepoint <= 0x1B3A) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1B6C) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1BA2 && codepoint <= 0x1BA3) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1BA5) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1BAC && codepoint <= 0x1BAD) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1C2C) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1C37) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1CD5 && codepoint <= 0x1CD9) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1CDC && codepoint <= 0x1CDF) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1CED) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xA825) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xA82C) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xA8C4) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0xA92B && codepoint <= 0xA92D) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0xA947 && codepoint <= 0xA949) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0xA94B && codepoint <= 0xA94E) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0xA9B8 && codepoint <= 0xA9B9) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xA9BD) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xAA2D) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xAA32) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0xAA35 && codepoint <= 0xAA36) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xAAB4) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xAAEC) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xABE8) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0xABED) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x10A02 && codepoint <= 0x10A03) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x10A0C && codepoint <= 0x10A0E) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x10A39 && codepoint <= 0x10A3A) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1103C && codepoint <= 0x11041) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x110B3 && codepoint <= 0x110B4) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x110B9 && codepoint <= 0x110BA) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1112A && codepoint <= 0x1112B) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11131 && codepoint <= 0x11132) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11173) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x111B6 && codepoint <= 0x111BB) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x111C9 && codepoint <= 0x111CA) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x111CC) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1122F) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x112E3 && codepoint <= 0x112E4) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x112E9 && codepoint <= 0x112EA) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1133B && codepoint <= 0x1133C) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11438 && codepoint <= 0x1143D) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11442) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11446) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x114B3 && codepoint <= 0x114B8) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x114C2 && codepoint <= 0x114C3) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x115B2 && codepoint <= 0x115B5) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x115BF && codepoint <= 0x115C0) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x115DC && codepoint <= 0x115DD) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11633 && codepoint <= 0x11638) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1163F) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x116B0 && codepoint <= 0x116B1) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x116B7) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x1171D) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11724 && codepoint <= 0x11725) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11728) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x1182F && codepoint <= 0x11832) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11839 && codepoint <= 0x1183A) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11943) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x119D4 && codepoint <= 0x119D7) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x119E0) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11A02 && codepoint <= 0x11A03) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11A0A) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11A33 && codepoint <= 0x11A34) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11A3B && codepoint <= 0x11A3E) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11A52 && codepoint <= 0x11A53) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11A59 && codepoint <= 0x11A5B) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11A8A && codepoint <= 0x11A95) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11C32 && codepoint <= 0x11C36) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11C3F) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11C92 && codepoint <= 0x11CA7) return Ucd_Indic_Position_BOTTOM;
	if (codepoint >= 0x11CAA && codepoint <= 0x11CB0) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11CB2) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11D36) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11D42) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11D44) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11D47) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x11EF4) return Ucd_Indic_Position_BOTTOM;
	if (codepoint == 0x0C48) return Ucd_Indic_Position_TOP_AND_BOTTOM;
	if (codepoint == 0x0F73) return Ucd_Indic_Position_TOP_AND_BOTTOM;
	if (codepoint >= 0x0F76 && codepoint <= 0x0F79) return Ucd_Indic_Position_TOP_AND_BOTTOM;
	if (codepoint == 0x0F81) return Ucd_Indic_Position_TOP_AND_BOTTOM;
	if (codepoint == 0x1B3C) return Ucd_Indic_Position_TOP_AND_BOTTOM;
	if (codepoint >= 0x1112E && codepoint <= 0x1112F) return Ucd_Indic_Position_TOP_AND_BOTTOM;
	if (codepoint == 0x0AC9) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint == 0x0B57) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint == 0x0CC0) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint >= 0x0CC7 && codepoint <= 0x0CC8) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint >= 0x0CCA && codepoint <= 0x0CCB) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint >= 0x1925 && codepoint <= 0x1926) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint == 0x1B43) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint == 0x111BF) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint >= 0x11232 && codepoint <= 0x11233) return Ucd_Indic_Position_TOP_AND_RIGHT;
	if (codepoint == 0x0B48) return Ucd_Indic_Position_TOP_AND_LEFT;
	if (codepoint == 0x0DDA) return Ucd_Indic_Position_TOP_AND_LEFT;
	if (codepoint == 0x17BE) return Ucd_Indic_Position_TOP_AND_LEFT;
	if (codepoint == 0x1C29) return Ucd_Indic_Position_TOP_AND_LEFT;
	if (codepoint == 0x114BB) return Ucd_Indic_Position_TOP_AND_LEFT;
	if (codepoint == 0x115B9) return Ucd_Indic_Position_TOP_AND_LEFT;
	if (codepoint == 0x0B4C) return Ucd_Indic_Position_TOP_AND_LEFT_AND_RIGHT;
	if (codepoint == 0x0DDD) return Ucd_Indic_Position_TOP_AND_LEFT_AND_RIGHT;
	if (codepoint == 0x17BF) return Ucd_Indic_Position_TOP_AND_LEFT_AND_RIGHT;
	if (codepoint == 0x115BB) return Ucd_Indic_Position_TOP_AND_LEFT_AND_RIGHT;
	if (codepoint == 0x1B3B) return Ucd_Indic_Position_BOTTOM_AND_RIGHT;
	if (codepoint == 0xA9BE) return Ucd_Indic_Position_BOTTOM_AND_RIGHT;
	if (codepoint == 0xA9C0) return Ucd_Indic_Position_BOTTOM_AND_RIGHT;
	if (codepoint == 0x11942) return Ucd_Indic_Position_BOTTOM_AND_RIGHT;
	if (codepoint == 0xA9BF) return Ucd_Indic_Position_BOTTOM_AND_LEFT;
	if (codepoint == 0x1B3D) return Ucd_Indic_Position_TOP_AND_BOTTOM_AND_RIGHT;
	if (codepoint == 0x103C) return Ucd_Indic_Position_TOP_AND_BOTTOM_AND_LEFT;
	if (codepoint == 0x1171E) return Ucd_Indic_Position_TOP_AND_BOTTOM_AND_LEFT;
	if (codepoint == 0x1CD4) return Ucd_Indic_Position_OVERSTRUCK;
	if (codepoint >= 0x1CE2 && codepoint <= 0x1CE8) return Ucd_Indic_Position_OVERSTRUCK;
	if (codepoint == 0x10A01) return Ucd_Indic_Position_OVERSTRUCK;
	if (codepoint == 0x10A06) return Ucd_Indic_Position_OVERSTRUCK;

	return Ucd_Indic_Position_UNKNOWN;
}

const char *enum_string(Ucd_Indic_Position value) {
	static const char *strings[] = {
		"UNKNOWN",
		"RIGHT",
		"LEFT",
		"VISUAL_ORDER_LEFT",
		"LEFT_AND_RIGHT",
		"TOP",
		"BOTTOM",
		"TOP_AND_BOTTOM",
		"TOP_AND_RIGHT",
		"TOP_AND_LEFT",
		"TOP_AND_LEFT_AND_RIGHT",
		"BOTTOM_AND_RIGHT",
		"BOTTOM_AND_LEFT",
		"TOP_AND_BOTTOM_AND_RIGHT",
		"TOP_AND_BOTTOM_AND_LEFT",
		"OVERSTRUCK",
		"COUNT",
	};
	return strings[value];
}

Ucd_Grapheme_Property ucd_grapheme_property(uint32_t codepoint) {
	if (codepoint >= 0x0600 && codepoint <= 0x0605) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x06DD) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x070F) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x08E2) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x0D4E) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x110BD) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x110CD) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint >= 0x111C2 && codepoint <= 0x111C3) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x1193F) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x11941) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x11A3A) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint >= 0x11A84 && codepoint <= 0x11A89) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x11D46) return Ucd_Grapheme_Property_PREPEND;
	if (codepoint == 0x000D) return Ucd_Grapheme_Property_CR;
	if (codepoint == 0x000A) return Ucd_Grapheme_Property_LF;
	if (codepoint >= 0x0000 && codepoint <= 0x0009) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x000B && codepoint <= 0x000C) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x000E && codepoint <= 0x001F) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x007F && codepoint <= 0x009F) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0x00AD) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0x061C) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0x180E) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0x200B) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x200E && codepoint <= 0x200F) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0x2028) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0x2029) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x202A && codepoint <= 0x202E) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x2060 && codepoint <= 0x2064) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0x2065) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x2066 && codepoint <= 0x206F) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0xFEFF) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0xFFF0 && codepoint <= 0xFFF8) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0xFFF9 && codepoint <= 0xFFFB) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x13430 && codepoint <= 0x13438) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x1BCA0 && codepoint <= 0x1BCA3) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x1D173 && codepoint <= 0x1D17A) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0xE0000) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint == 0xE0001) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0xE0002 && codepoint <= 0xE001F) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0xE0080 && codepoint <= 0xE00FF) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0xE01F0 && codepoint <= 0xE0FFF) return Ucd_Grapheme_Property_CONTROL;
	if (codepoint >= 0x0300 && codepoint <= 0x036F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0483 && codepoint <= 0x0487) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0488 && codepoint <= 0x0489) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0591 && codepoint <= 0x05BD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x05BF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x05C1 && codepoint <= 0x05C2) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x05C4 && codepoint <= 0x05C5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x05C7) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0610 && codepoint <= 0x061A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x064B && codepoint <= 0x065F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0670) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x06D6 && codepoint <= 0x06DC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x06DF && codepoint <= 0x06E4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x06E7 && codepoint <= 0x06E8) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x06EA && codepoint <= 0x06ED) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0711) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0730 && codepoint <= 0x074A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x07A6 && codepoint <= 0x07B0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x07EB && codepoint <= 0x07F3) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x07FD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0816 && codepoint <= 0x0819) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x081B && codepoint <= 0x0823) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0825 && codepoint <= 0x0827) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0829 && codepoint <= 0x082D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0859 && codepoint <= 0x085B) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x08D3 && codepoint <= 0x08E1) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x08E3 && codepoint <= 0x0902) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x093A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x093C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0941 && codepoint <= 0x0948) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x094D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0951 && codepoint <= 0x0957) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0962 && codepoint <= 0x0963) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0981) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x09BC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x09BE) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x09C1 && codepoint <= 0x09C4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x09CD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x09D7) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x09E2 && codepoint <= 0x09E3) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x09FE) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0A01 && codepoint <= 0x0A02) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0A3C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0A41 && codepoint <= 0x0A42) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0A47 && codepoint <= 0x0A48) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0A4B && codepoint <= 0x0A4D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0A51) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0A70 && codepoint <= 0x0A71) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0A75) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0A81 && codepoint <= 0x0A82) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0ABC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0AC1 && codepoint <= 0x0AC5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0AC7 && codepoint <= 0x0AC8) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0ACD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0AE2 && codepoint <= 0x0AE3) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0AFA && codepoint <= 0x0AFF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0B01) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0B3C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0B3E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0B3F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0B41 && codepoint <= 0x0B44) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0B4D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0B55 && codepoint <= 0x0B56) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0B57) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0B62 && codepoint <= 0x0B63) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0B82) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0BBE) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0BC0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0BCD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0BD7) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0C00) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0C04) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0C3E && codepoint <= 0x0C40) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0C46 && codepoint <= 0x0C48) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0C4A && codepoint <= 0x0C4D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0C55 && codepoint <= 0x0C56) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0C62 && codepoint <= 0x0C63) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0C81) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0CBC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0CBF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0CC2) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0CC6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0CCC && codepoint <= 0x0CCD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0CD5 && codepoint <= 0x0CD6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0CE2 && codepoint <= 0x0CE3) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0D00 && codepoint <= 0x0D01) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0D3B && codepoint <= 0x0D3C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0D3E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0D41 && codepoint <= 0x0D44) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0D4D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0D57) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0D62 && codepoint <= 0x0D63) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0D81) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0DCA) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0DCF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0DD2 && codepoint <= 0x0DD4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0DD6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0DDF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0E31) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0E34 && codepoint <= 0x0E3A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0E47 && codepoint <= 0x0E4E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0EB1) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0EB4 && codepoint <= 0x0EBC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0EC8 && codepoint <= 0x0ECD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0F18 && codepoint <= 0x0F19) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0F35) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0F37) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0F39) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0F71 && codepoint <= 0x0F7E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0F80 && codepoint <= 0x0F84) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0F86 && codepoint <= 0x0F87) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0F8D && codepoint <= 0x0F97) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x0F99 && codepoint <= 0x0FBC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x0FC6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x102D && codepoint <= 0x1030) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1032 && codepoint <= 0x1037) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1039 && codepoint <= 0x103A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x103D && codepoint <= 0x103E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1058 && codepoint <= 0x1059) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x105E && codepoint <= 0x1060) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1071 && codepoint <= 0x1074) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1082) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1085 && codepoint <= 0x1086) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x108D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x109D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x135D && codepoint <= 0x135F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1712 && codepoint <= 0x1714) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1732 && codepoint <= 0x1734) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1752 && codepoint <= 0x1753) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1772 && codepoint <= 0x1773) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x17B4 && codepoint <= 0x17B5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x17B7 && codepoint <= 0x17BD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x17C6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x17C9 && codepoint <= 0x17D3) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x17DD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x180B && codepoint <= 0x180D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1885 && codepoint <= 0x1886) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x18A9) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1920 && codepoint <= 0x1922) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1927 && codepoint <= 0x1928) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1932) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1939 && codepoint <= 0x193B) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1A17 && codepoint <= 0x1A18) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1A1B) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1A56) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1A58 && codepoint <= 0x1A5E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1A60) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1A62) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1A65 && codepoint <= 0x1A6C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1A73 && codepoint <= 0x1A7C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1A7F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1AB0 && codepoint <= 0x1ABD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1ABE) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1ABF && codepoint <= 0x1AC0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1B00 && codepoint <= 0x1B03) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1B34) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1B35) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1B36 && codepoint <= 0x1B3A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1B3C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1B42) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1B6B && codepoint <= 0x1B73) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1B80 && codepoint <= 0x1B81) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1BA2 && codepoint <= 0x1BA5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1BA8 && codepoint <= 0x1BA9) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1BAB && codepoint <= 0x1BAD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1BE6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1BE8 && codepoint <= 0x1BE9) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1BED) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1BEF && codepoint <= 0x1BF1) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1C2C && codepoint <= 0x1C33) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1C36 && codepoint <= 0x1C37) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1CD0 && codepoint <= 0x1CD2) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1CD4 && codepoint <= 0x1CE0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1CE2 && codepoint <= 0x1CE8) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1CED) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1CF4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1CF8 && codepoint <= 0x1CF9) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1DC0 && codepoint <= 0x1DF9) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1DFB && codepoint <= 0x1DFF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x200C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x20D0 && codepoint <= 0x20DC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x20DD && codepoint <= 0x20E0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x20E1) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x20E2 && codepoint <= 0x20E4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x20E5 && codepoint <= 0x20F0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x2CEF && codepoint <= 0x2CF1) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x2D7F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x2DE0 && codepoint <= 0x2DFF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x302A && codepoint <= 0x302D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x302E && codepoint <= 0x302F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x3099 && codepoint <= 0x309A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xA66F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA670 && codepoint <= 0xA672) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA674 && codepoint <= 0xA67D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA69E && codepoint <= 0xA69F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA6F0 && codepoint <= 0xA6F1) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xA802) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xA806) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xA80B) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA825 && codepoint <= 0xA826) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xA82C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA8C4 && codepoint <= 0xA8C5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA8E0 && codepoint <= 0xA8F1) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xA8FF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA926 && codepoint <= 0xA92D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA947 && codepoint <= 0xA951) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA980 && codepoint <= 0xA982) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xA9B3) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA9B6 && codepoint <= 0xA9B9) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xA9BC && codepoint <= 0xA9BD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xA9E5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xAA29 && codepoint <= 0xAA2E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xAA31 && codepoint <= 0xAA32) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xAA35 && codepoint <= 0xAA36) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xAA43) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xAA4C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xAA7C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xAAB0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xAAB2 && codepoint <= 0xAAB4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xAAB7 && codepoint <= 0xAAB8) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xAABE && codepoint <= 0xAABF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xAAC1) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xAAEC && codepoint <= 0xAAED) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xAAF6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xABE5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xABE8) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xABED) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0xFB1E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xFE00 && codepoint <= 0xFE0F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xFE20 && codepoint <= 0xFE2F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xFF9E && codepoint <= 0xFF9F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x101FD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x102E0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10376 && codepoint <= 0x1037A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10A01 && codepoint <= 0x10A03) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10A05 && codepoint <= 0x10A06) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10A0C && codepoint <= 0x10A0F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10A38 && codepoint <= 0x10A3A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x10A3F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10AE5 && codepoint <= 0x10AE6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10D24 && codepoint <= 0x10D27) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10EAB && codepoint <= 0x10EAC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x10F46 && codepoint <= 0x10F50) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11001) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11038 && codepoint <= 0x11046) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1107F && codepoint <= 0x11081) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x110B3 && codepoint <= 0x110B6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x110B9 && codepoint <= 0x110BA) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11100 && codepoint <= 0x11102) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11127 && codepoint <= 0x1112B) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1112D && codepoint <= 0x11134) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11173) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11180 && codepoint <= 0x11181) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x111B6 && codepoint <= 0x111BE) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x111C9 && codepoint <= 0x111CC) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x111CF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1122F && codepoint <= 0x11231) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11234) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11236 && codepoint <= 0x11237) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1123E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x112DF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x112E3 && codepoint <= 0x112EA) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11300 && codepoint <= 0x11301) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1133B && codepoint <= 0x1133C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1133E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11340) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11357) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11366 && codepoint <= 0x1136C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11370 && codepoint <= 0x11374) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11438 && codepoint <= 0x1143F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11442 && codepoint <= 0x11444) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11446) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1145E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x114B0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x114B3 && codepoint <= 0x114B8) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x114BA) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x114BD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x114BF && codepoint <= 0x114C0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x114C2 && codepoint <= 0x114C3) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x115AF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x115B2 && codepoint <= 0x115B5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x115BC && codepoint <= 0x115BD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x115BF && codepoint <= 0x115C0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x115DC && codepoint <= 0x115DD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11633 && codepoint <= 0x1163A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1163D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1163F && codepoint <= 0x11640) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x116AB) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x116AD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x116B0 && codepoint <= 0x116B5) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x116B7) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1171D && codepoint <= 0x1171F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11722 && codepoint <= 0x11725) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11727 && codepoint <= 0x1172B) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1182F && codepoint <= 0x11837) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11839 && codepoint <= 0x1183A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11930) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1193B && codepoint <= 0x1193C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1193E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11943) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x119D4 && codepoint <= 0x119D7) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x119DA && codepoint <= 0x119DB) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x119E0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11A01 && codepoint <= 0x11A0A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11A33 && codepoint <= 0x11A38) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11A3B && codepoint <= 0x11A3E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11A47) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11A51 && codepoint <= 0x11A56) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11A59 && codepoint <= 0x11A5B) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11A8A && codepoint <= 0x11A96) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11A98 && codepoint <= 0x11A99) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11C30 && codepoint <= 0x11C36) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11C38 && codepoint <= 0x11C3D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11C3F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11C92 && codepoint <= 0x11CA7) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11CAA && codepoint <= 0x11CB0) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11CB2 && codepoint <= 0x11CB3) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11CB5 && codepoint <= 0x11CB6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11D31 && codepoint <= 0x11D36) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11D3A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11D3C && codepoint <= 0x11D3D) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11D3F && codepoint <= 0x11D45) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11D47) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11D90 && codepoint <= 0x11D91) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11D95) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x11D97) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x11EF3 && codepoint <= 0x11EF4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x16AF0 && codepoint <= 0x16AF4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x16B30 && codepoint <= 0x16B36) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x16F4F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x16F8F && codepoint <= 0x16F92) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x16FE4) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1BC9D && codepoint <= 0x1BC9E) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1D165) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1D167 && codepoint <= 0x1D169) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1D16E && codepoint <= 0x1D172) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1D17B && codepoint <= 0x1D182) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1D185 && codepoint <= 0x1D18B) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1D1AA && codepoint <= 0x1D1AD) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1D242 && codepoint <= 0x1D244) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1DA00 && codepoint <= 0x1DA36) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1DA3B && codepoint <= 0x1DA6C) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1DA75) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint == 0x1DA84) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1DA9B && codepoint <= 0x1DA9F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1DAA1 && codepoint <= 0x1DAAF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E000 && codepoint <= 0x1E006) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E008 && codepoint <= 0x1E018) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E01B && codepoint <= 0x1E021) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E023 && codepoint <= 0x1E024) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E026 && codepoint <= 0x1E02A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E130 && codepoint <= 0x1E136) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E2EC && codepoint <= 0x1E2EF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E8D0 && codepoint <= 0x1E8D6) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1E944 && codepoint <= 0x1E94A) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1F3FB && codepoint <= 0x1F3FF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xE0020 && codepoint <= 0xE007F) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0xE0100 && codepoint <= 0xE01EF) return Ucd_Grapheme_Property_EXTEND;
	if (codepoint >= 0x1F1E6 && codepoint <= 0x1F1FF) return Ucd_Grapheme_Property_REGIONAL_INDICATOR;
	if (codepoint == 0x0903) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x093B) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x093E && codepoint <= 0x0940) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0949 && codepoint <= 0x094C) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x094E && codepoint <= 0x094F) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0982 && codepoint <= 0x0983) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x09BF && codepoint <= 0x09C0) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x09C7 && codepoint <= 0x09C8) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x09CB && codepoint <= 0x09CC) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0A03) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0A3E && codepoint <= 0x0A40) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0A83) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0ABE && codepoint <= 0x0AC0) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0AC9) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0ACB && codepoint <= 0x0ACC) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0B02 && codepoint <= 0x0B03) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0B40) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0B47 && codepoint <= 0x0B48) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0B4B && codepoint <= 0x0B4C) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0BBF) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0BC1 && codepoint <= 0x0BC2) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0BC6 && codepoint <= 0x0BC8) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0BCA && codepoint <= 0x0BCC) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0C01 && codepoint <= 0x0C03) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0C41 && codepoint <= 0x0C44) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0C82 && codepoint <= 0x0C83) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0CBE) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0CC0 && codepoint <= 0x0CC1) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0CC3 && codepoint <= 0x0CC4) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0CC7 && codepoint <= 0x0CC8) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0CCA && codepoint <= 0x0CCB) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0D02 && codepoint <= 0x0D03) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0D3F && codepoint <= 0x0D40) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0D46 && codepoint <= 0x0D48) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0D4A && codepoint <= 0x0D4C) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0D82 && codepoint <= 0x0D83) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0DD0 && codepoint <= 0x0DD1) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0DD8 && codepoint <= 0x0DDE) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0DF2 && codepoint <= 0x0DF3) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0E33) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0EB3) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x0F3E && codepoint <= 0x0F3F) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x0F7F) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1031) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x103B && codepoint <= 0x103C) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1056 && codepoint <= 0x1057) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1084) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x17B6) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x17BE && codepoint <= 0x17C5) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x17C7 && codepoint <= 0x17C8) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1923 && codepoint <= 0x1926) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1929 && codepoint <= 0x192B) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1930 && codepoint <= 0x1931) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1933 && codepoint <= 0x1938) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1A19 && codepoint <= 0x1A1A) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1A55) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1A57) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1A6D && codepoint <= 0x1A72) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1B04) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1B3B) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1B3D && codepoint <= 0x1B41) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1B43 && codepoint <= 0x1B44) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1B82) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1BA1) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1BA6 && codepoint <= 0x1BA7) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1BAA) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1BE7) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1BEA && codepoint <= 0x1BEC) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1BEE) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1BF2 && codepoint <= 0x1BF3) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1C24 && codepoint <= 0x1C2B) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1C34 && codepoint <= 0x1C35) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1CE1) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1CF7) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xA823 && codepoint <= 0xA824) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0xA827) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xA880 && codepoint <= 0xA881) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xA8B4 && codepoint <= 0xA8C3) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xA952 && codepoint <= 0xA953) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0xA983) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xA9B4 && codepoint <= 0xA9B5) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xA9BA && codepoint <= 0xA9BB) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xA9BE && codepoint <= 0xA9C0) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xAA2F && codepoint <= 0xAA30) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xAA33 && codepoint <= 0xAA34) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0xAA4D) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0xAAEB) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xAAEE && codepoint <= 0xAAEF) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0xAAF5) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xABE3 && codepoint <= 0xABE4) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xABE6 && codepoint <= 0xABE7) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0xABE9 && codepoint <= 0xABEA) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0xABEC) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11000) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11002) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11082) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x110B0 && codepoint <= 0x110B2) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x110B7 && codepoint <= 0x110B8) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1112C) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11145 && codepoint <= 0x11146) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11182) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x111B3 && codepoint <= 0x111B5) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x111BF && codepoint <= 0x111C0) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x111CE) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1122C && codepoint <= 0x1122E) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11232 && codepoint <= 0x11233) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11235) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x112E0 && codepoint <= 0x112E2) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11302 && codepoint <= 0x11303) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1133F) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11341 && codepoint <= 0x11344) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11347 && codepoint <= 0x11348) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1134B && codepoint <= 0x1134D) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11362 && codepoint <= 0x11363) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11435 && codepoint <= 0x11437) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11440 && codepoint <= 0x11441) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11445) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x114B1 && codepoint <= 0x114B2) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x114B9) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x114BB && codepoint <= 0x114BC) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x114BE) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x114C1) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x115B0 && codepoint <= 0x115B1) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x115B8 && codepoint <= 0x115BB) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x115BE) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11630 && codepoint <= 0x11632) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1163B && codepoint <= 0x1163C) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1163E) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x116AC) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x116AE && codepoint <= 0x116AF) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x116B6) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11720 && codepoint <= 0x11721) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11726) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1182C && codepoint <= 0x1182E) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11838) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11931 && codepoint <= 0x11935) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11937 && codepoint <= 0x11938) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1193D) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11940) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11942) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x119D1 && codepoint <= 0x119D3) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x119DC && codepoint <= 0x119DF) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x119E4) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11A39) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11A57 && codepoint <= 0x11A58) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11A97) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11C2F) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11C3E) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11CA9) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11CB1) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11CB4) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11D8A && codepoint <= 0x11D8E) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11D93 && codepoint <= 0x11D94) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x11D96) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x11EF5 && codepoint <= 0x11EF6) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x16F51 && codepoint <= 0x16F87) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x16FF0 && codepoint <= 0x16FF1) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1D166) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint == 0x1D16D) return Ucd_Grapheme_Property_SPACINGMARK;
	if (codepoint >= 0x1100 && codepoint <= 0x115F) return Ucd_Grapheme_Property_L;
	if (codepoint >= 0xA960 && codepoint <= 0xA97C) return Ucd_Grapheme_Property_L;
	if (codepoint >= 0x1160 && codepoint <= 0x11A7) return Ucd_Grapheme_Property_V;
	if (codepoint >= 0xD7B0 && codepoint <= 0xD7C6) return Ucd_Grapheme_Property_V;
	if (codepoint >= 0x11A8 && codepoint <= 0x11FF) return Ucd_Grapheme_Property_T;
	if (codepoint >= 0xD7CB && codepoint <= 0xD7FB) return Ucd_Grapheme_Property_T;
	if (codepoint == 0xAC00) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAC1C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAC38) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAC54) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAC70) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAC8C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xACA8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xACC4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xACE0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xACFC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAD18) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAD34) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAD50) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAD6C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAD88) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xADA4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xADC0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xADDC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xADF8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAE14) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAE30) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAE4C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAE68) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAE84) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAEA0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAEBC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAED8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAEF4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAF10) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAF2C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAF48) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAF64) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAF80) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAF9C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAFB8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAFD4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xAFF0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB00C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB028) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB044) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB060) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB07C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB098) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB0B4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB0D0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB0EC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB108) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB124) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB140) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB15C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB178) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB194) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB1B0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB1CC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB1E8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB204) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB220) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB23C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB258) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB274) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB290) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB2AC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB2C8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB2E4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB300) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB31C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB338) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB354) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB370) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB38C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB3A8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB3C4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB3E0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB3FC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB418) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB434) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB450) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB46C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB488) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB4A4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB4C0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB4DC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB4F8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB514) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB530) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB54C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB568) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB584) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB5A0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB5BC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB5D8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB5F4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB610) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB62C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB648) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB664) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB680) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB69C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB6B8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB6D4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB6F0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB70C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB728) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB744) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB760) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB77C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB798) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB7B4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB7D0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB7EC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB808) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB824) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB840) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB85C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB878) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB894) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB8B0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB8CC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB8E8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB904) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB920) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB93C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB958) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB974) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB990) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB9AC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB9C8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xB9E4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBA00) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBA1C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBA38) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBA54) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBA70) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBA8C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBAA8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBAC4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBAE0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBAFC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBB18) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBB34) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBB50) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBB6C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBB88) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBBA4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBBC0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBBDC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBBF8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBC14) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBC30) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBC4C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBC68) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBC84) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBCA0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBCBC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBCD8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBCF4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBD10) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBD2C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBD48) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBD64) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBD80) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBD9C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBDB8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBDD4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBDF0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBE0C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBE28) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBE44) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBE60) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBE7C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBE98) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBEB4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBED0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBEEC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBF08) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBF24) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBF40) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBF5C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBF78) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBF94) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBFB0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBFCC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xBFE8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC004) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC020) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC03C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC058) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC074) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC090) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC0AC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC0C8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC0E4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC100) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC11C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC138) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC154) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC170) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC18C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC1A8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC1C4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC1E0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC1FC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC218) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC234) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC250) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC26C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC288) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC2A4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC2C0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC2DC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC2F8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC314) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC330) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC34C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC368) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC384) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC3A0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC3BC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC3D8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC3F4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC410) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC42C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC448) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC464) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC480) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC49C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC4B8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC4D4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC4F0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC50C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC528) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC544) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC560) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC57C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC598) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC5B4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC5D0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC5EC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC608) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC624) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC640) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC65C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC678) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC694) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC6B0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC6CC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC6E8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC704) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC720) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC73C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC758) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC774) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC790) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC7AC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC7C8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC7E4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC800) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC81C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC838) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC854) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC870) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC88C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC8A8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC8C4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC8E0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC8FC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC918) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC934) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC950) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC96C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC988) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC9A4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC9C0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC9DC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xC9F8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCA14) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCA30) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCA4C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCA68) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCA84) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCAA0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCABC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCAD8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCAF4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCB10) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCB2C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCB48) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCB64) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCB80) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCB9C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCBB8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCBD4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCBF0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCC0C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCC28) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCC44) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCC60) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCC7C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCC98) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCCB4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCCD0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCCEC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCD08) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCD24) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCD40) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCD5C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCD78) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCD94) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCDB0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCDCC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCDE8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCE04) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCE20) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCE3C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCE58) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCE74) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCE90) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCEAC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCEC8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCEE4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCF00) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCF1C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCF38) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCF54) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCF70) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCF8C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCFA8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCFC4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCFE0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xCFFC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD018) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD034) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD050) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD06C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD088) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD0A4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD0C0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD0DC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD0F8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD114) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD130) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD14C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD168) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD184) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD1A0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD1BC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD1D8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD1F4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD210) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD22C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD248) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD264) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD280) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD29C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD2B8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD2D4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD2F0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD30C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD328) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD344) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD360) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD37C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD398) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD3B4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD3D0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD3EC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD408) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD424) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD440) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD45C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD478) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD494) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD4B0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD4CC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD4E8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD504) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD520) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD53C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD558) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD574) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD590) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD5AC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD5C8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD5E4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD600) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD61C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD638) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD654) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD670) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD68C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD6A8) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD6C4) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD6E0) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD6FC) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD718) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD734) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD750) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD76C) return Ucd_Grapheme_Property_LV;
	if (codepoint == 0xD788) return Ucd_Grapheme_Property_LV;
	if (codepoint >= 0xAC01 && codepoint <= 0xAC1B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAC1D && codepoint <= 0xAC37) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAC39 && codepoint <= 0xAC53) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAC55 && codepoint <= 0xAC6F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAC71 && codepoint <= 0xAC8B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAC8D && codepoint <= 0xACA7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xACA9 && codepoint <= 0xACC3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xACC5 && codepoint <= 0xACDF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xACE1 && codepoint <= 0xACFB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xACFD && codepoint <= 0xAD17) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAD19 && codepoint <= 0xAD33) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAD35 && codepoint <= 0xAD4F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAD51 && codepoint <= 0xAD6B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAD6D && codepoint <= 0xAD87) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAD89 && codepoint <= 0xADA3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xADA5 && codepoint <= 0xADBF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xADC1 && codepoint <= 0xADDB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xADDD && codepoint <= 0xADF7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xADF9 && codepoint <= 0xAE13) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAE15 && codepoint <= 0xAE2F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAE31 && codepoint <= 0xAE4B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAE4D && codepoint <= 0xAE67) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAE69 && codepoint <= 0xAE83) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAE85 && codepoint <= 0xAE9F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAEA1 && codepoint <= 0xAEBB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAEBD && codepoint <= 0xAED7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAED9 && codepoint <= 0xAEF3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAEF5 && codepoint <= 0xAF0F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAF11 && codepoint <= 0xAF2B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAF2D && codepoint <= 0xAF47) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAF49 && codepoint <= 0xAF63) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAF65 && codepoint <= 0xAF7F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAF81 && codepoint <= 0xAF9B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAF9D && codepoint <= 0xAFB7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAFB9 && codepoint <= 0xAFD3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAFD5 && codepoint <= 0xAFEF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xAFF1 && codepoint <= 0xB00B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB00D && codepoint <= 0xB027) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB029 && codepoint <= 0xB043) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB045 && codepoint <= 0xB05F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB061 && codepoint <= 0xB07B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB07D && codepoint <= 0xB097) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB099 && codepoint <= 0xB0B3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB0B5 && codepoint <= 0xB0CF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB0D1 && codepoint <= 0xB0EB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB0ED && codepoint <= 0xB107) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB109 && codepoint <= 0xB123) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB125 && codepoint <= 0xB13F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB141 && codepoint <= 0xB15B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB15D && codepoint <= 0xB177) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB179 && codepoint <= 0xB193) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB195 && codepoint <= 0xB1AF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB1B1 && codepoint <= 0xB1CB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB1CD && codepoint <= 0xB1E7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB1E9 && codepoint <= 0xB203) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB205 && codepoint <= 0xB21F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB221 && codepoint <= 0xB23B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB23D && codepoint <= 0xB257) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB259 && codepoint <= 0xB273) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB275 && codepoint <= 0xB28F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB291 && codepoint <= 0xB2AB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB2AD && codepoint <= 0xB2C7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB2C9 && codepoint <= 0xB2E3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB2E5 && codepoint <= 0xB2FF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB301 && codepoint <= 0xB31B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB31D && codepoint <= 0xB337) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB339 && codepoint <= 0xB353) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB355 && codepoint <= 0xB36F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB371 && codepoint <= 0xB38B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB38D && codepoint <= 0xB3A7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB3A9 && codepoint <= 0xB3C3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB3C5 && codepoint <= 0xB3DF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB3E1 && codepoint <= 0xB3FB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB3FD && codepoint <= 0xB417) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB419 && codepoint <= 0xB433) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB435 && codepoint <= 0xB44F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB451 && codepoint <= 0xB46B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB46D && codepoint <= 0xB487) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB489 && codepoint <= 0xB4A3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB4A5 && codepoint <= 0xB4BF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB4C1 && codepoint <= 0xB4DB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB4DD && codepoint <= 0xB4F7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB4F9 && codepoint <= 0xB513) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB515 && codepoint <= 0xB52F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB531 && codepoint <= 0xB54B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB54D && codepoint <= 0xB567) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB569 && codepoint <= 0xB583) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB585 && codepoint <= 0xB59F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB5A1 && codepoint <= 0xB5BB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB5BD && codepoint <= 0xB5D7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB5D9 && codepoint <= 0xB5F3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB5F5 && codepoint <= 0xB60F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB611 && codepoint <= 0xB62B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB62D && codepoint <= 0xB647) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB649 && codepoint <= 0xB663) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB665 && codepoint <= 0xB67F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB681 && codepoint <= 0xB69B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB69D && codepoint <= 0xB6B7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB6B9 && codepoint <= 0xB6D3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB6D5 && codepoint <= 0xB6EF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB6F1 && codepoint <= 0xB70B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB70D && codepoint <= 0xB727) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB729 && codepoint <= 0xB743) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB745 && codepoint <= 0xB75F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB761 && codepoint <= 0xB77B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB77D && codepoint <= 0xB797) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB799 && codepoint <= 0xB7B3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB7B5 && codepoint <= 0xB7CF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB7D1 && codepoint <= 0xB7EB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB7ED && codepoint <= 0xB807) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB809 && codepoint <= 0xB823) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB825 && codepoint <= 0xB83F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB841 && codepoint <= 0xB85B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB85D && codepoint <= 0xB877) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB879 && codepoint <= 0xB893) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB895 && codepoint <= 0xB8AF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB8B1 && codepoint <= 0xB8CB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB8CD && codepoint <= 0xB8E7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB8E9 && codepoint <= 0xB903) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB905 && codepoint <= 0xB91F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB921 && codepoint <= 0xB93B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB93D && codepoint <= 0xB957) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB959 && codepoint <= 0xB973) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB975 && codepoint <= 0xB98F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB991 && codepoint <= 0xB9AB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB9AD && codepoint <= 0xB9C7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB9C9 && codepoint <= 0xB9E3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xB9E5 && codepoint <= 0xB9FF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBA01 && codepoint <= 0xBA1B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBA1D && codepoint <= 0xBA37) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBA39 && codepoint <= 0xBA53) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBA55 && codepoint <= 0xBA6F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBA71 && codepoint <= 0xBA8B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBA8D && codepoint <= 0xBAA7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBAA9 && codepoint <= 0xBAC3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBAC5 && codepoint <= 0xBADF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBAE1 && codepoint <= 0xBAFB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBAFD && codepoint <= 0xBB17) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBB19 && codepoint <= 0xBB33) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBB35 && codepoint <= 0xBB4F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBB51 && codepoint <= 0xBB6B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBB6D && codepoint <= 0xBB87) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBB89 && codepoint <= 0xBBA3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBBA5 && codepoint <= 0xBBBF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBBC1 && codepoint <= 0xBBDB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBBDD && codepoint <= 0xBBF7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBBF9 && codepoint <= 0xBC13) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBC15 && codepoint <= 0xBC2F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBC31 && codepoint <= 0xBC4B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBC4D && codepoint <= 0xBC67) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBC69 && codepoint <= 0xBC83) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBC85 && codepoint <= 0xBC9F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBCA1 && codepoint <= 0xBCBB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBCBD && codepoint <= 0xBCD7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBCD9 && codepoint <= 0xBCF3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBCF5 && codepoint <= 0xBD0F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBD11 && codepoint <= 0xBD2B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBD2D && codepoint <= 0xBD47) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBD49 && codepoint <= 0xBD63) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBD65 && codepoint <= 0xBD7F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBD81 && codepoint <= 0xBD9B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBD9D && codepoint <= 0xBDB7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBDB9 && codepoint <= 0xBDD3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBDD5 && codepoint <= 0xBDEF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBDF1 && codepoint <= 0xBE0B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBE0D && codepoint <= 0xBE27) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBE29 && codepoint <= 0xBE43) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBE45 && codepoint <= 0xBE5F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBE61 && codepoint <= 0xBE7B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBE7D && codepoint <= 0xBE97) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBE99 && codepoint <= 0xBEB3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBEB5 && codepoint <= 0xBECF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBED1 && codepoint <= 0xBEEB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBEED && codepoint <= 0xBF07) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBF09 && codepoint <= 0xBF23) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBF25 && codepoint <= 0xBF3F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBF41 && codepoint <= 0xBF5B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBF5D && codepoint <= 0xBF77) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBF79 && codepoint <= 0xBF93) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBF95 && codepoint <= 0xBFAF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBFB1 && codepoint <= 0xBFCB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBFCD && codepoint <= 0xBFE7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xBFE9 && codepoint <= 0xC003) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC005 && codepoint <= 0xC01F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC021 && codepoint <= 0xC03B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC03D && codepoint <= 0xC057) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC059 && codepoint <= 0xC073) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC075 && codepoint <= 0xC08F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC091 && codepoint <= 0xC0AB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC0AD && codepoint <= 0xC0C7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC0C9 && codepoint <= 0xC0E3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC0E5 && codepoint <= 0xC0FF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC101 && codepoint <= 0xC11B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC11D && codepoint <= 0xC137) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC139 && codepoint <= 0xC153) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC155 && codepoint <= 0xC16F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC171 && codepoint <= 0xC18B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC18D && codepoint <= 0xC1A7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC1A9 && codepoint <= 0xC1C3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC1C5 && codepoint <= 0xC1DF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC1E1 && codepoint <= 0xC1FB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC1FD && codepoint <= 0xC217) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC219 && codepoint <= 0xC233) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC235 && codepoint <= 0xC24F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC251 && codepoint <= 0xC26B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC26D && codepoint <= 0xC287) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC289 && codepoint <= 0xC2A3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC2A5 && codepoint <= 0xC2BF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC2C1 && codepoint <= 0xC2DB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC2DD && codepoint <= 0xC2F7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC2F9 && codepoint <= 0xC313) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC315 && codepoint <= 0xC32F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC331 && codepoint <= 0xC34B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC34D && codepoint <= 0xC367) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC369 && codepoint <= 0xC383) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC385 && codepoint <= 0xC39F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC3A1 && codepoint <= 0xC3BB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC3BD && codepoint <= 0xC3D7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC3D9 && codepoint <= 0xC3F3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC3F5 && codepoint <= 0xC40F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC411 && codepoint <= 0xC42B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC42D && codepoint <= 0xC447) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC449 && codepoint <= 0xC463) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC465 && codepoint <= 0xC47F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC481 && codepoint <= 0xC49B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC49D && codepoint <= 0xC4B7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC4B9 && codepoint <= 0xC4D3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC4D5 && codepoint <= 0xC4EF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC4F1 && codepoint <= 0xC50B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC50D && codepoint <= 0xC527) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC529 && codepoint <= 0xC543) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC545 && codepoint <= 0xC55F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC561 && codepoint <= 0xC57B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC57D && codepoint <= 0xC597) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC599 && codepoint <= 0xC5B3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC5B5 && codepoint <= 0xC5CF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC5D1 && codepoint <= 0xC5EB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC5ED && codepoint <= 0xC607) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC609 && codepoint <= 0xC623) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC625 && codepoint <= 0xC63F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC641 && codepoint <= 0xC65B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC65D && codepoint <= 0xC677) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC679 && codepoint <= 0xC693) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC695 && codepoint <= 0xC6AF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC6B1 && codepoint <= 0xC6CB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC6CD && codepoint <= 0xC6E7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC6E9 && codepoint <= 0xC703) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC705 && codepoint <= 0xC71F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC721 && codepoint <= 0xC73B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC73D && codepoint <= 0xC757) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC759 && codepoint <= 0xC773) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC775 && codepoint <= 0xC78F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC791 && codepoint <= 0xC7AB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC7AD && codepoint <= 0xC7C7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC7C9 && codepoint <= 0xC7E3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC7E5 && codepoint <= 0xC7FF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC801 && codepoint <= 0xC81B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC81D && codepoint <= 0xC837) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC839 && codepoint <= 0xC853) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC855 && codepoint <= 0xC86F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC871 && codepoint <= 0xC88B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC88D && codepoint <= 0xC8A7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC8A9 && codepoint <= 0xC8C3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC8C5 && codepoint <= 0xC8DF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC8E1 && codepoint <= 0xC8FB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC8FD && codepoint <= 0xC917) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC919 && codepoint <= 0xC933) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC935 && codepoint <= 0xC94F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC951 && codepoint <= 0xC96B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC96D && codepoint <= 0xC987) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC989 && codepoint <= 0xC9A3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC9A5 && codepoint <= 0xC9BF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC9C1 && codepoint <= 0xC9DB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC9DD && codepoint <= 0xC9F7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xC9F9 && codepoint <= 0xCA13) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCA15 && codepoint <= 0xCA2F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCA31 && codepoint <= 0xCA4B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCA4D && codepoint <= 0xCA67) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCA69 && codepoint <= 0xCA83) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCA85 && codepoint <= 0xCA9F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCAA1 && codepoint <= 0xCABB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCABD && codepoint <= 0xCAD7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCAD9 && codepoint <= 0xCAF3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCAF5 && codepoint <= 0xCB0F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCB11 && codepoint <= 0xCB2B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCB2D && codepoint <= 0xCB47) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCB49 && codepoint <= 0xCB63) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCB65 && codepoint <= 0xCB7F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCB81 && codepoint <= 0xCB9B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCB9D && codepoint <= 0xCBB7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCBB9 && codepoint <= 0xCBD3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCBD5 && codepoint <= 0xCBEF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCBF1 && codepoint <= 0xCC0B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCC0D && codepoint <= 0xCC27) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCC29 && codepoint <= 0xCC43) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCC45 && codepoint <= 0xCC5F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCC61 && codepoint <= 0xCC7B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCC7D && codepoint <= 0xCC97) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCC99 && codepoint <= 0xCCB3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCCB5 && codepoint <= 0xCCCF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCCD1 && codepoint <= 0xCCEB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCCED && codepoint <= 0xCD07) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCD09 && codepoint <= 0xCD23) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCD25 && codepoint <= 0xCD3F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCD41 && codepoint <= 0xCD5B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCD5D && codepoint <= 0xCD77) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCD79 && codepoint <= 0xCD93) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCD95 && codepoint <= 0xCDAF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCDB1 && codepoint <= 0xCDCB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCDCD && codepoint <= 0xCDE7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCDE9 && codepoint <= 0xCE03) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCE05 && codepoint <= 0xCE1F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCE21 && codepoint <= 0xCE3B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCE3D && codepoint <= 0xCE57) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCE59 && codepoint <= 0xCE73) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCE75 && codepoint <= 0xCE8F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCE91 && codepoint <= 0xCEAB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCEAD && codepoint <= 0xCEC7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCEC9 && codepoint <= 0xCEE3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCEE5 && codepoint <= 0xCEFF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCF01 && codepoint <= 0xCF1B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCF1D && codepoint <= 0xCF37) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCF39 && codepoint <= 0xCF53) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCF55 && codepoint <= 0xCF6F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCF71 && codepoint <= 0xCF8B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCF8D && codepoint <= 0xCFA7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCFA9 && codepoint <= 0xCFC3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCFC5 && codepoint <= 0xCFDF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCFE1 && codepoint <= 0xCFFB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xCFFD && codepoint <= 0xD017) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD019 && codepoint <= 0xD033) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD035 && codepoint <= 0xD04F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD051 && codepoint <= 0xD06B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD06D && codepoint <= 0xD087) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD089 && codepoint <= 0xD0A3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD0A5 && codepoint <= 0xD0BF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD0C1 && codepoint <= 0xD0DB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD0DD && codepoint <= 0xD0F7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD0F9 && codepoint <= 0xD113) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD115 && codepoint <= 0xD12F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD131 && codepoint <= 0xD14B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD14D && codepoint <= 0xD167) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD169 && codepoint <= 0xD183) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD185 && codepoint <= 0xD19F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD1A1 && codepoint <= 0xD1BB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD1BD && codepoint <= 0xD1D7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD1D9 && codepoint <= 0xD1F3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD1F5 && codepoint <= 0xD20F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD211 && codepoint <= 0xD22B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD22D && codepoint <= 0xD247) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD249 && codepoint <= 0xD263) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD265 && codepoint <= 0xD27F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD281 && codepoint <= 0xD29B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD29D && codepoint <= 0xD2B7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD2B9 && codepoint <= 0xD2D3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD2D5 && codepoint <= 0xD2EF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD2F1 && codepoint <= 0xD30B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD30D && codepoint <= 0xD327) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD329 && codepoint <= 0xD343) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD345 && codepoint <= 0xD35F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD361 && codepoint <= 0xD37B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD37D && codepoint <= 0xD397) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD399 && codepoint <= 0xD3B3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD3B5 && codepoint <= 0xD3CF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD3D1 && codepoint <= 0xD3EB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD3ED && codepoint <= 0xD407) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD409 && codepoint <= 0xD423) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD425 && codepoint <= 0xD43F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD441 && codepoint <= 0xD45B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD45D && codepoint <= 0xD477) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD479 && codepoint <= 0xD493) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD495 && codepoint <= 0xD4AF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD4B1 && codepoint <= 0xD4CB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD4CD && codepoint <= 0xD4E7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD4E9 && codepoint <= 0xD503) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD505 && codepoint <= 0xD51F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD521 && codepoint <= 0xD53B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD53D && codepoint <= 0xD557) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD559 && codepoint <= 0xD573) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD575 && codepoint <= 0xD58F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD591 && codepoint <= 0xD5AB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD5AD && codepoint <= 0xD5C7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD5C9 && codepoint <= 0xD5E3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD5E5 && codepoint <= 0xD5FF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD601 && codepoint <= 0xD61B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD61D && codepoint <= 0xD637) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD639 && codepoint <= 0xD653) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD655 && codepoint <= 0xD66F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD671 && codepoint <= 0xD68B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD68D && codepoint <= 0xD6A7) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD6A9 && codepoint <= 0xD6C3) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD6C5 && codepoint <= 0xD6DF) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD6E1 && codepoint <= 0xD6FB) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD6FD && codepoint <= 0xD717) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD719 && codepoint <= 0xD733) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD735 && codepoint <= 0xD74F) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD751 && codepoint <= 0xD76B) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD76D && codepoint <= 0xD787) return Ucd_Grapheme_Property_LVT;
	if (codepoint >= 0xD789 && codepoint <= 0xD7A3) return Ucd_Grapheme_Property_LVT;
	if (codepoint == 0x200D) return Ucd_Grapheme_Property_ZWJ;

	return Ucd_Grapheme_Property_UNKNOWN;
}

const char *enum_string(Ucd_Grapheme_Property value) {
	static const char *strings[] = {
		"UNKNOWN",
		"PREPEND",
		"CR",
		"LF",
		"CONTROL",
		"EXTEND",
		"REGIONAL_INDICATOR",
		"SPACINGMARK",
		"L",
		"V",
		"T",
		"LV",
		"LVT",
		"ZWJ",
		"COUNT",
	};
	return strings[value];
}

