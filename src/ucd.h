#pragma once
#include <stdint.h>

enum Ucd_Script : uint32_t {
	Ucd_Script_UNKNOWN,
	Ucd_Script_COMMON,
	Ucd_Script_LATIN,
	Ucd_Script_GREEK,
	Ucd_Script_CYRILLIC,
	Ucd_Script_ARMENIAN,
	Ucd_Script_HEBREW,
	Ucd_Script_ARABIC,
	Ucd_Script_SYRIAC,
	Ucd_Script_THAANA,
	Ucd_Script_DEVANAGARI,
	Ucd_Script_BENGALI,
	Ucd_Script_GURMUKHI,
	Ucd_Script_GUJARATI,
	Ucd_Script_ORIYA,
	Ucd_Script_TAMIL,
	Ucd_Script_TELUGU,
	Ucd_Script_KANNADA,
	Ucd_Script_MALAYALAM,
	Ucd_Script_SINHALA,
	Ucd_Script_THAI,
	Ucd_Script_LAO,
	Ucd_Script_TIBETAN,
	Ucd_Script_MYANMAR,
	Ucd_Script_GEORGIAN,
	Ucd_Script_HANGUL,
	Ucd_Script_ETHIOPIC,
	Ucd_Script_CHEROKEE,
	Ucd_Script_CANADIAN_ABORIGINAL,
	Ucd_Script_OGHAM,
	Ucd_Script_RUNIC,
	Ucd_Script_KHMER,
	Ucd_Script_MONGOLIAN,
	Ucd_Script_HIRAGANA,
	Ucd_Script_KATAKANA,
	Ucd_Script_BOPOMOFO,
	Ucd_Script_HAN,
	Ucd_Script_YI,
	Ucd_Script_OLD_ITALIC,
	Ucd_Script_GOTHIC,
	Ucd_Script_DESERET,
	Ucd_Script_INHERITED,
	Ucd_Script_TAGALOG,
	Ucd_Script_HANUNOO,
	Ucd_Script_BUHID,
	Ucd_Script_TAGBANWA,
	Ucd_Script_LIMBU,
	Ucd_Script_TAI_LE,
	Ucd_Script_LINEAR_B,
	Ucd_Script_UGARITIC,
	Ucd_Script_SHAVIAN,
	Ucd_Script_OSMANYA,
	Ucd_Script_CYPRIOT,
	Ucd_Script_BRAILLE,
	Ucd_Script_BUGINESE,
	Ucd_Script_COPTIC,
	Ucd_Script_NEW_TAI_LUE,
	Ucd_Script_GLAGOLITIC,
	Ucd_Script_TIFINAGH,
	Ucd_Script_SYLOTI_NAGRI,
	Ucd_Script_OLD_PERSIAN,
	Ucd_Script_KHAROSHTHI,
	Ucd_Script_BALINESE,
	Ucd_Script_CUNEIFORM,
	Ucd_Script_PHOENICIAN,
	Ucd_Script_PHAGS_PA,
	Ucd_Script_NKO,
	Ucd_Script_SUNDANESE,
	Ucd_Script_LEPCHA,
	Ucd_Script_OL_CHIKI,
	Ucd_Script_VAI,
	Ucd_Script_SAURASHTRA,
	Ucd_Script_KAYAH_LI,
	Ucd_Script_REJANG,
	Ucd_Script_LYCIAN,
	Ucd_Script_CARIAN,
	Ucd_Script_LYDIAN,
	Ucd_Script_CHAM,
	Ucd_Script_TAI_THAM,
	Ucd_Script_TAI_VIET,
	Ucd_Script_AVESTAN,
	Ucd_Script_EGYPTIAN_HIEROGLYPHS,
	Ucd_Script_SAMARITAN,
	Ucd_Script_LISU,
	Ucd_Script_BAMUM,
	Ucd_Script_JAVANESE,
	Ucd_Script_MEETEI_MAYEK,
	Ucd_Script_IMPERIAL_ARAMAIC,
	Ucd_Script_OLD_SOUTH_ARABIAN,
	Ucd_Script_INSCRIPTIONAL_PARTHIAN,
	Ucd_Script_INSCRIPTIONAL_PAHLAVI,
	Ucd_Script_OLD_TURKIC,
	Ucd_Script_KAITHI,
	Ucd_Script_BATAK,
	Ucd_Script_BRAHMI,
	Ucd_Script_MANDAIC,
	Ucd_Script_CHAKMA,
	Ucd_Script_MEROITIC_CURSIVE,
	Ucd_Script_MEROITIC_HIEROGLYPHS,
	Ucd_Script_MIAO,
	Ucd_Script_SHARADA,
	Ucd_Script_SORA_SOMPENG,
	Ucd_Script_TAKRI,
	Ucd_Script_CAUCASIAN_ALBANIAN,
	Ucd_Script_BASSA_VAH,
	Ucd_Script_DUPLOYAN,
	Ucd_Script_ELBASAN,
	Ucd_Script_GRANTHA,
	Ucd_Script_PAHAWH_HMONG,
	Ucd_Script_KHOJKI,
	Ucd_Script_LINEAR_A,
	Ucd_Script_MAHAJANI,
	Ucd_Script_MANICHAEAN,
	Ucd_Script_MENDE_KIKAKUI,
	Ucd_Script_MODI,
	Ucd_Script_MRO,
	Ucd_Script_OLD_NORTH_ARABIAN,
	Ucd_Script_NABATAEAN,
	Ucd_Script_PALMYRENE,
	Ucd_Script_PAU_CIN_HAU,
	Ucd_Script_OLD_PERMIC,
	Ucd_Script_PSALTER_PAHLAVI,
	Ucd_Script_SIDDHAM,
	Ucd_Script_KHUDAWADI,
	Ucd_Script_TIRHUTA,
	Ucd_Script_WARANG_CITI,
	Ucd_Script_AHOM,
	Ucd_Script_ANATOLIAN_HIEROGLYPHS,
	Ucd_Script_HATRAN,
	Ucd_Script_MULTANI,
	Ucd_Script_OLD_HUNGARIAN,
	Ucd_Script_SIGNWRITING,
	Ucd_Script_ADLAM,
	Ucd_Script_BHAIKSUKI,
	Ucd_Script_MARCHEN,
	Ucd_Script_NEWA,
	Ucd_Script_OSAGE,
	Ucd_Script_TANGUT,
	Ucd_Script_MASARAM_GONDI,
	Ucd_Script_NUSHU,
	Ucd_Script_SOYOMBO,
	Ucd_Script_ZANABAZAR_SQUARE,
	Ucd_Script_DOGRA,
	Ucd_Script_GUNJALA_GONDI,
	Ucd_Script_MAKASAR,
	Ucd_Script_MEDEFAIDRIN,
	Ucd_Script_HANIFI_ROHINGYA,
	Ucd_Script_SOGDIAN,
	Ucd_Script_OLD_SOGDIAN,
	Ucd_Script_ELYMAIC,
	Ucd_Script_NANDINAGARI,
	Ucd_Script_NYIAKENG_PUACHUE_HMONG,
	Ucd_Script_WANCHO,
	Ucd_Script_CHORASMIAN,
	Ucd_Script_DIVES_AKURU,
	Ucd_Script_KHITAN_SMALL_SCRIPT,
	Ucd_Script_YEZIDI,
	Ucd_Script_COUNT,
};


Ucd_Script ucd_script(uint32_t codepoint);
const char *enum_string(Ucd_Script value);


enum Ucd_Script_Ext : uint32_t {
	Ucd_Script_Ext_UNKNOWN,
	Ucd_Script_Ext_BENG,
	Ucd_Script_Ext_DEVA,
	Ucd_Script_Ext_DUPL,
	Ucd_Script_Ext_GREK,
	Ucd_Script_Ext_HANI,
	Ucd_Script_Ext_LATN,
	Ucd_Script_Ext_NAND,
	Ucd_Script_Ext_ARAB_COPT,
	Ucd_Script_Ext_ARAB_ROHG,
	Ucd_Script_Ext_ARAB_SYRC,
	Ucd_Script_Ext_ARAB_THAA,
	Ucd_Script_Ext_BENG_DEVA,
	Ucd_Script_Ext_BOPO_HANI,
	Ucd_Script_Ext_BUGI_JAVA,
	Ucd_Script_Ext_CPRT_LINB,
	Ucd_Script_Ext_CYRL_GLAG,
	Ucd_Script_Ext_CYRL_LATN,
	Ucd_Script_Ext_CYRL_PERM,
	Ucd_Script_Ext_CYRL_SYRC,
	Ucd_Script_Ext_DEVA_GRAN,
	Ucd_Script_Ext_DEVA_NAND,
	Ucd_Script_Ext_DEVA_SHRD,
	Ucd_Script_Ext_DEVA_TAML,
	Ucd_Script_Ext_GEOR_LATN,
	Ucd_Script_Ext_GRAN_TAML,
	Ucd_Script_Ext_GUJR_KHOJ,
	Ucd_Script_Ext_GURU_MULT,
	Ucd_Script_Ext_HANI_LATN,
	Ucd_Script_Ext_HIRA_KANA,
	Ucd_Script_Ext_KNDA_NAND,
	Ucd_Script_Ext_LATN_MONG,
	Ucd_Script_Ext_MONG_PHAG,
	Ucd_Script_Ext_ARAB_SYRC_THAA,
	Ucd_Script_Ext_ARAB_THAA_YEZI,
	Ucd_Script_Ext_BENG_CAKM_SYLO,
	Ucd_Script_Ext_CAKM_MYMR_TALE,
	Ucd_Script_Ext_CPRT_LINA_LINB,
	Ucd_Script_Ext_DEVA_GRAN_KNDA,
	Ucd_Script_Ext_DEVA_GRAN_LATN,
	Ucd_Script_Ext_HANI_HIRA_KANA,
	Ucd_Script_Ext_KALI_LATN_MYMR,
	Ucd_Script_Ext_BENG_DEVA_GRAN_KNDA,
	Ucd_Script_Ext_BUHD_HANO_TAGB_TGLG,
	Ucd_Script_Ext_DEVA_DOGR_KTHI_MAHJ,
	Ucd_Script_Ext_ARAB_ROHG_SYRC_THAA_YEZI,
	Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA,
	Ucd_Script_Ext_BOPO_HANG_HANI_HIRA_KANA_YIII,
	Ucd_Script_Ext_DEVA_KNDA_MLYM_ORYA_TAML_TELU,
	Ucd_Script_Ext_ADLM_ARAB_MAND_MANI_PHLP_ROHG_SOGD_SYRC,
	Ucd_Script_Ext_BENG_DEVA_GRAN_KNDA_NAND_ORYA_TELU_TIRH,
	Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KTHI_MAHJ_MODI_SIND_TAKR_TIRH,
	Ucd_Script_Ext_BENG_DEVA_GRAN_GUJR_GURU_KNDA_LATN_MLYM_ORYA_TAML_TELU_TIRH,
	Ucd_Script_Ext_BENG_DEVA_GRAN_GUJR_GURU_KNDA_LATN_MLYM_ORYA_SHRD_TAML_TELU_TIRH,
	Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KNDA_KTHI_MAHJ_MODI_NAND_SIND_TAKR_TIRH,
	Ucd_Script_Ext_DEVA_DOGR_GUJR_GURU_KHOJ_KNDA_KTHI_MAHJ_MLYM_MODI_NAND_SIND_TAKR_TIRH,
	Ucd_Script_Ext_BENG_DEVA_DOGR_GONG_GONM_GRAN_GUJR_GURU_KNDA_MAHJ_MLYM_NAND_ORYA_SIND_SINH_SYLO_TAKR_TAML_TELU_TIRH,
	Ucd_Script_Ext_BENG_DEVA_DOGR_GONG_GONM_GRAN_GUJR_GURU_KNDA_LIMB_MAHJ_MLYM_NAND_ORYA_SIND_SINH_SYLO_TAKR_TAML_TELU_TIRH,
	Ucd_Script_Ext_COUNT,
};


Ucd_Script_Ext ucd_script_ext(uint32_t codepoint);
const char *enum_string(Ucd_Script_Ext value);


enum Ucd_Block : uint32_t {
	Ucd_Block_UNKNOWN,
	Ucd_Block_BASIC_LATIN,
	Ucd_Block_LATIN_1_SUPPLEMENT,
	Ucd_Block_LATIN_EXTENDED_A,
	Ucd_Block_LATIN_EXTENDED_B,
	Ucd_Block_IPA_EXTENSIONS,
	Ucd_Block_SPACING_MODIFIER_LETTERS,
	Ucd_Block_COMBINING_DIACRITICAL_MARKS,
	Ucd_Block_GREEK_AND_COPTIC,
	Ucd_Block_CYRILLIC,
	Ucd_Block_CYRILLIC_SUPPLEMENT,
	Ucd_Block_ARMENIAN,
	Ucd_Block_HEBREW,
	Ucd_Block_ARABIC,
	Ucd_Block_SYRIAC,
	Ucd_Block_ARABIC_SUPPLEMENT,
	Ucd_Block_THAANA,
	Ucd_Block_NKO,
	Ucd_Block_SAMARITAN,
	Ucd_Block_MANDAIC,
	Ucd_Block_SYRIAC_SUPPLEMENT,
	Ucd_Block_ARABIC_EXTENDED_A,
	Ucd_Block_DEVANAGARI,
	Ucd_Block_BENGALI,
	Ucd_Block_GURMUKHI,
	Ucd_Block_GUJARATI,
	Ucd_Block_ORIYA,
	Ucd_Block_TAMIL,
	Ucd_Block_TELUGU,
	Ucd_Block_KANNADA,
	Ucd_Block_MALAYALAM,
	Ucd_Block_SINHALA,
	Ucd_Block_THAI,
	Ucd_Block_LAO,
	Ucd_Block_TIBETAN,
	Ucd_Block_MYANMAR,
	Ucd_Block_GEORGIAN,
	Ucd_Block_HANGUL_JAMO,
	Ucd_Block_ETHIOPIC,
	Ucd_Block_ETHIOPIC_SUPPLEMENT,
	Ucd_Block_CHEROKEE,
	Ucd_Block_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS,
	Ucd_Block_OGHAM,
	Ucd_Block_RUNIC,
	Ucd_Block_TAGALOG,
	Ucd_Block_HANUNOO,
	Ucd_Block_BUHID,
	Ucd_Block_TAGBANWA,
	Ucd_Block_KHMER,
	Ucd_Block_MONGOLIAN,
	Ucd_Block_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED,
	Ucd_Block_LIMBU,
	Ucd_Block_TAI_LE,
	Ucd_Block_NEW_TAI_LUE,
	Ucd_Block_KHMER_SYMBOLS,
	Ucd_Block_BUGINESE,
	Ucd_Block_TAI_THAM,
	Ucd_Block_COMBINING_DIACRITICAL_MARKS_EXTENDED,
	Ucd_Block_BALINESE,
	Ucd_Block_SUNDANESE,
	Ucd_Block_BATAK,
	Ucd_Block_LEPCHA,
	Ucd_Block_OL_CHIKI,
	Ucd_Block_CYRILLIC_EXTENDED_C,
	Ucd_Block_GEORGIAN_EXTENDED,
	Ucd_Block_SUNDANESE_SUPPLEMENT,
	Ucd_Block_VEDIC_EXTENSIONS,
	Ucd_Block_PHONETIC_EXTENSIONS,
	Ucd_Block_PHONETIC_EXTENSIONS_SUPPLEMENT,
	Ucd_Block_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT,
	Ucd_Block_LATIN_EXTENDED_ADDITIONAL,
	Ucd_Block_GREEK_EXTENDED,
	Ucd_Block_GENERAL_PUNCTUATION,
	Ucd_Block_SUPERSCRIPTS_AND_SUBSCRIPTS,
	Ucd_Block_CURRENCY_SYMBOLS,
	Ucd_Block_COMBINING_DIACRITICAL_MARKS_FOR_SYMBOLS,
	Ucd_Block_LETTERLIKE_SYMBOLS,
	Ucd_Block_NUMBER_FORMS,
	Ucd_Block_ARROWS,
	Ucd_Block_MATHEMATICAL_OPERATORS,
	Ucd_Block_MISCELLANEOUS_TECHNICAL,
	Ucd_Block_CONTROL_PICTURES,
	Ucd_Block_OPTICAL_CHARACTER_RECOGNITION,
	Ucd_Block_ENCLOSED_ALPHANUMERICS,
	Ucd_Block_BOX_DRAWING,
	Ucd_Block_BLOCK_ELEMENTS,
	Ucd_Block_GEOMETRIC_SHAPES,
	Ucd_Block_MISCELLANEOUS_SYMBOLS,
	Ucd_Block_DINGBATS,
	Ucd_Block_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A,
	Ucd_Block_SUPPLEMENTAL_ARROWS_A,
	Ucd_Block_BRAILLE_PATTERNS,
	Ucd_Block_SUPPLEMENTAL_ARROWS_B,
	Ucd_Block_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B,
	Ucd_Block_SUPPLEMENTAL_MATHEMATICAL_OPERATORS,
	Ucd_Block_MISCELLANEOUS_SYMBOLS_AND_ARROWS,
	Ucd_Block_GLAGOLITIC,
	Ucd_Block_LATIN_EXTENDED_C,
	Ucd_Block_COPTIC,
	Ucd_Block_GEORGIAN_SUPPLEMENT,
	Ucd_Block_TIFINAGH,
	Ucd_Block_ETHIOPIC_EXTENDED,
	Ucd_Block_CYRILLIC_EXTENDED_A,
	Ucd_Block_SUPPLEMENTAL_PUNCTUATION,
	Ucd_Block_CJK_RADICALS_SUPPLEMENT,
	Ucd_Block_KANGXI_RADICALS,
	Ucd_Block_IDEOGRAPHIC_DESCRIPTION_CHARACTERS,
	Ucd_Block_CJK_SYMBOLS_AND_PUNCTUATION,
	Ucd_Block_HIRAGANA,
	Ucd_Block_KATAKANA,
	Ucd_Block_BOPOMOFO,
	Ucd_Block_HANGUL_COMPATIBILITY_JAMO,
	Ucd_Block_KANBUN,
	Ucd_Block_BOPOMOFO_EXTENDED,
	Ucd_Block_CJK_STROKES,
	Ucd_Block_KATAKANA_PHONETIC_EXTENSIONS,
	Ucd_Block_ENCLOSED_CJK_LETTERS_AND_MONTHS,
	Ucd_Block_CJK_COMPATIBILITY,
	Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A,
	Ucd_Block_YIJING_HEXAGRAM_SYMBOLS,
	Ucd_Block_CJK_UNIFIED_IDEOGRAPHS,
	Ucd_Block_YI_SYLLABLES,
	Ucd_Block_YI_RADICALS,
	Ucd_Block_LISU,
	Ucd_Block_VAI,
	Ucd_Block_CYRILLIC_EXTENDED_B,
	Ucd_Block_BAMUM,
	Ucd_Block_MODIFIER_TONE_LETTERS,
	Ucd_Block_LATIN_EXTENDED_D,
	Ucd_Block_SYLOTI_NAGRI,
	Ucd_Block_COMMON_INDIC_NUMBER_FORMS,
	Ucd_Block_PHAGS_PA,
	Ucd_Block_SAURASHTRA,
	Ucd_Block_DEVANAGARI_EXTENDED,
	Ucd_Block_KAYAH_LI,
	Ucd_Block_REJANG,
	Ucd_Block_HANGUL_JAMO_EXTENDED_A,
	Ucd_Block_JAVANESE,
	Ucd_Block_MYANMAR_EXTENDED_B,
	Ucd_Block_CHAM,
	Ucd_Block_MYANMAR_EXTENDED_A,
	Ucd_Block_TAI_VIET,
	Ucd_Block_MEETEI_MAYEK_EXTENSIONS,
	Ucd_Block_ETHIOPIC_EXTENDED_A,
	Ucd_Block_LATIN_EXTENDED_E,
	Ucd_Block_CHEROKEE_SUPPLEMENT,
	Ucd_Block_MEETEI_MAYEK,
	Ucd_Block_HANGUL_SYLLABLES,
	Ucd_Block_HANGUL_JAMO_EXTENDED_B,
	Ucd_Block_HIGH_SURROGATES,
	Ucd_Block_HIGH_PRIVATE_USE_SURROGATES,
	Ucd_Block_LOW_SURROGATES,
	Ucd_Block_PRIVATE_USE_AREA,
	Ucd_Block_CJK_COMPATIBILITY_IDEOGRAPHS,
	Ucd_Block_ALPHABETIC_PRESENTATION_FORMS,
	Ucd_Block_ARABIC_PRESENTATION_FORMS_A,
	Ucd_Block_VARIATION_SELECTORS,
	Ucd_Block_VERTICAL_FORMS,
	Ucd_Block_COMBINING_HALF_MARKS,
	Ucd_Block_CJK_COMPATIBILITY_FORMS,
	Ucd_Block_SMALL_FORM_VARIANTS,
	Ucd_Block_ARABIC_PRESENTATION_FORMS_B,
	Ucd_Block_HALFWIDTH_AND_FULLWIDTH_FORMS,
	Ucd_Block_SPECIALS,
	Ucd_Block_LINEAR_B_SYLLABARY,
	Ucd_Block_LINEAR_B_IDEOGRAMS,
	Ucd_Block_AEGEAN_NUMBERS,
	Ucd_Block_ANCIENT_GREEK_NUMBERS,
	Ucd_Block_ANCIENT_SYMBOLS,
	Ucd_Block_PHAISTOS_DISC,
	Ucd_Block_LYCIAN,
	Ucd_Block_CARIAN,
	Ucd_Block_COPTIC_EPACT_NUMBERS,
	Ucd_Block_OLD_ITALIC,
	Ucd_Block_GOTHIC,
	Ucd_Block_OLD_PERMIC,
	Ucd_Block_UGARITIC,
	Ucd_Block_OLD_PERSIAN,
	Ucd_Block_DESERET,
	Ucd_Block_SHAVIAN,
	Ucd_Block_OSMANYA,
	Ucd_Block_OSAGE,
	Ucd_Block_ELBASAN,
	Ucd_Block_CAUCASIAN_ALBANIAN,
	Ucd_Block_LINEAR_A,
	Ucd_Block_CYPRIOT_SYLLABARY,
	Ucd_Block_IMPERIAL_ARAMAIC,
	Ucd_Block_PALMYRENE,
	Ucd_Block_NABATAEAN,
	Ucd_Block_HATRAN,
	Ucd_Block_PHOENICIAN,
	Ucd_Block_LYDIAN,
	Ucd_Block_MEROITIC_HIEROGLYPHS,
	Ucd_Block_MEROITIC_CURSIVE,
	Ucd_Block_KHAROSHTHI,
	Ucd_Block_OLD_SOUTH_ARABIAN,
	Ucd_Block_OLD_NORTH_ARABIAN,
	Ucd_Block_MANICHAEAN,
	Ucd_Block_AVESTAN,
	Ucd_Block_INSCRIPTIONAL_PARTHIAN,
	Ucd_Block_INSCRIPTIONAL_PAHLAVI,
	Ucd_Block_PSALTER_PAHLAVI,
	Ucd_Block_OLD_TURKIC,
	Ucd_Block_OLD_HUNGARIAN,
	Ucd_Block_HANIFI_ROHINGYA,
	Ucd_Block_RUMI_NUMERAL_SYMBOLS,
	Ucd_Block_YEZIDI,
	Ucd_Block_OLD_SOGDIAN,
	Ucd_Block_SOGDIAN,
	Ucd_Block_CHORASMIAN,
	Ucd_Block_ELYMAIC,
	Ucd_Block_BRAHMI,
	Ucd_Block_KAITHI,
	Ucd_Block_SORA_SOMPENG,
	Ucd_Block_CHAKMA,
	Ucd_Block_MAHAJANI,
	Ucd_Block_SHARADA,
	Ucd_Block_SINHALA_ARCHAIC_NUMBERS,
	Ucd_Block_KHOJKI,
	Ucd_Block_MULTANI,
	Ucd_Block_KHUDAWADI,
	Ucd_Block_GRANTHA,
	Ucd_Block_NEWA,
	Ucd_Block_TIRHUTA,
	Ucd_Block_SIDDHAM,
	Ucd_Block_MODI,
	Ucd_Block_MONGOLIAN_SUPPLEMENT,
	Ucd_Block_TAKRI,
	Ucd_Block_AHOM,
	Ucd_Block_DOGRA,
	Ucd_Block_WARANG_CITI,
	Ucd_Block_DIVES_AKURU,
	Ucd_Block_NANDINAGARI,
	Ucd_Block_ZANABAZAR_SQUARE,
	Ucd_Block_SOYOMBO,
	Ucd_Block_PAU_CIN_HAU,
	Ucd_Block_BHAIKSUKI,
	Ucd_Block_MARCHEN,
	Ucd_Block_MASARAM_GONDI,
	Ucd_Block_GUNJALA_GONDI,
	Ucd_Block_MAKASAR,
	Ucd_Block_LISU_SUPPLEMENT,
	Ucd_Block_TAMIL_SUPPLEMENT,
	Ucd_Block_CUNEIFORM,
	Ucd_Block_CUNEIFORM_NUMBERS_AND_PUNCTUATION,
	Ucd_Block_EARLY_DYNASTIC_CUNEIFORM,
	Ucd_Block_EGYPTIAN_HIEROGLYPHS,
	Ucd_Block_EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS,
	Ucd_Block_ANATOLIAN_HIEROGLYPHS,
	Ucd_Block_BAMUM_SUPPLEMENT,
	Ucd_Block_MRO,
	Ucd_Block_BASSA_VAH,
	Ucd_Block_PAHAWH_HMONG,
	Ucd_Block_MEDEFAIDRIN,
	Ucd_Block_MIAO,
	Ucd_Block_IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION,
	Ucd_Block_TANGUT,
	Ucd_Block_TANGUT_COMPONENTS,
	Ucd_Block_KHITAN_SMALL_SCRIPT,
	Ucd_Block_TANGUT_SUPPLEMENT,
	Ucd_Block_KANA_SUPPLEMENT,
	Ucd_Block_KANA_EXTENDED_A,
	Ucd_Block_SMALL_KANA_EXTENSION,
	Ucd_Block_NUSHU,
	Ucd_Block_DUPLOYAN,
	Ucd_Block_SHORTHAND_FORMAT_CONTROLS,
	Ucd_Block_BYZANTINE_MUSICAL_SYMBOLS,
	Ucd_Block_MUSICAL_SYMBOLS,
	Ucd_Block_ANCIENT_GREEK_MUSICAL_NOTATION,
	Ucd_Block_MAYAN_NUMERALS,
	Ucd_Block_TAI_XUAN_JING_SYMBOLS,
	Ucd_Block_COUNTING_ROD_NUMERALS,
	Ucd_Block_MATHEMATICAL_ALPHANUMERIC_SYMBOLS,
	Ucd_Block_SUTTON_SIGNWRITING,
	Ucd_Block_GLAGOLITIC_SUPPLEMENT,
	Ucd_Block_NYIAKENG_PUACHUE_HMONG,
	Ucd_Block_WANCHO,
	Ucd_Block_MENDE_KIKAKUI,
	Ucd_Block_ADLAM,
	Ucd_Block_INDIC_SIYAQ_NUMBERS,
	Ucd_Block_OTTOMAN_SIYAQ_NUMBERS,
	Ucd_Block_ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS,
	Ucd_Block_MAHJONG_TILES,
	Ucd_Block_DOMINO_TILES,
	Ucd_Block_PLAYING_CARDS,
	Ucd_Block_ENCLOSED_ALPHANUMERIC_SUPPLEMENT,
	Ucd_Block_ENCLOSED_IDEOGRAPHIC_SUPPLEMENT,
	Ucd_Block_MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS,
	Ucd_Block_EMOTICONS,
	Ucd_Block_ORNAMENTAL_DINGBATS,
	Ucd_Block_TRANSPORT_AND_MAP_SYMBOLS,
	Ucd_Block_ALCHEMICAL_SYMBOLS,
	Ucd_Block_GEOMETRIC_SHAPES_EXTENDED,
	Ucd_Block_SUPPLEMENTAL_ARROWS_C,
	Ucd_Block_SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS,
	Ucd_Block_CHESS_SYMBOLS,
	Ucd_Block_SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A,
	Ucd_Block_SYMBOLS_FOR_LEGACY_COMPUTING,
	Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B,
	Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C,
	Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D,
	Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E,
	Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F,
	Ucd_Block_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT,
	Ucd_Block_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G,
	Ucd_Block_TAGS,
	Ucd_Block_VARIATION_SELECTORS_SUPPLEMENT,
	Ucd_Block_SUPPLEMENTARY_PRIVATE_USE_AREA_A,
	Ucd_Block_SUPPLEMENTARY_PRIVATE_USE_AREA_B,
	Ucd_Block_COUNT,
};


Ucd_Block ucd_block(uint32_t codepoint);
const char *enum_string(Ucd_Block value);


enum Ucd_Indic_Syllable : uint32_t {
	Ucd_Indic_Syllable_UNKNOWN,
	Ucd_Indic_Syllable_BINDU,
	Ucd_Indic_Syllable_VISARGA,
	Ucd_Indic_Syllable_AVAGRAHA,
	Ucd_Indic_Syllable_NUKTA,
	Ucd_Indic_Syllable_VIRAMA,
	Ucd_Indic_Syllable_PURE_KILLER,
	Ucd_Indic_Syllable_INVISIBLE_STACKER,
	Ucd_Indic_Syllable_VOWEL_INDEPENDENT,
	Ucd_Indic_Syllable_VOWEL_DEPENDENT,
	Ucd_Indic_Syllable_VOWEL,
	Ucd_Indic_Syllable_CONSONANT_PLACEHOLDER,
	Ucd_Indic_Syllable_CONSONANT,
	Ucd_Indic_Syllable_CONSONANT_DEAD,
	Ucd_Indic_Syllable_CONSONANT_WITH_STACKER,
	Ucd_Indic_Syllable_CONSONANT_PREFIXED,
	Ucd_Indic_Syllable_CONSONANT_PRECEDING_REPHA,
	Ucd_Indic_Syllable_CONSONANT_INITIAL_POSTFIXED,
	Ucd_Indic_Syllable_CONSONANT_SUCCEEDING_REPHA,
	Ucd_Indic_Syllable_CONSONANT_SUBJOINED,
	Ucd_Indic_Syllable_CONSONANT_MEDIAL,
	Ucd_Indic_Syllable_CONSONANT_FINAL,
	Ucd_Indic_Syllable_CONSONANT_HEAD_LETTER,
	Ucd_Indic_Syllable_MODIFYING_LETTER,
	Ucd_Indic_Syllable_TONE_LETTER,
	Ucd_Indic_Syllable_TONE_MARK,
	Ucd_Indic_Syllable_GEMINATION_MARK,
	Ucd_Indic_Syllable_CANTILLATION_MARK,
	Ucd_Indic_Syllable_REGISTER_SHIFTER,
	Ucd_Indic_Syllable_SYLLABLE_MODIFIER,
	Ucd_Indic_Syllable_CONSONANT_KILLER,
	Ucd_Indic_Syllable_NON_JOINER,
	Ucd_Indic_Syllable_JOINER,
	Ucd_Indic_Syllable_NUMBER_JOINER,
	Ucd_Indic_Syllable_NUMBER,
	Ucd_Indic_Syllable_BRAHMI_JOINING_NUMBER,
	Ucd_Indic_Syllable_COUNT,
};


Ucd_Indic_Syllable ucd_indic_syllable(uint32_t codepoint);
const char *enum_string(Ucd_Indic_Syllable value);


enum Ucd_Indic_Position : uint32_t {
	Ucd_Indic_Position_UNKNOWN,
	Ucd_Indic_Position_RIGHT,
	Ucd_Indic_Position_LEFT,
	Ucd_Indic_Position_VISUAL_ORDER_LEFT,
	Ucd_Indic_Position_LEFT_AND_RIGHT,
	Ucd_Indic_Position_TOP,
	Ucd_Indic_Position_BOTTOM,
	Ucd_Indic_Position_TOP_AND_BOTTOM,
	Ucd_Indic_Position_TOP_AND_RIGHT,
	Ucd_Indic_Position_TOP_AND_LEFT,
	Ucd_Indic_Position_TOP_AND_LEFT_AND_RIGHT,
	Ucd_Indic_Position_BOTTOM_AND_RIGHT,
	Ucd_Indic_Position_BOTTOM_AND_LEFT,
	Ucd_Indic_Position_TOP_AND_BOTTOM_AND_RIGHT,
	Ucd_Indic_Position_TOP_AND_BOTTOM_AND_LEFT,
	Ucd_Indic_Position_OVERSTRUCK,
	Ucd_Indic_Position_COUNT,
};


Ucd_Indic_Position ucd_indic_position(uint32_t codepoint);
const char *enum_string(Ucd_Indic_Position value);


enum Ucd_Grapheme_Property : uint32_t {
	Ucd_Grapheme_Property_UNKNOWN,
	Ucd_Grapheme_Property_PREPEND,
	Ucd_Grapheme_Property_CR,
	Ucd_Grapheme_Property_LF,
	Ucd_Grapheme_Property_CONTROL,
	Ucd_Grapheme_Property_EXTEND,
	Ucd_Grapheme_Property_REGIONAL_INDICATOR,
	Ucd_Grapheme_Property_SPACINGMARK,
	Ucd_Grapheme_Property_L,
	Ucd_Grapheme_Property_V,
	Ucd_Grapheme_Property_T,
	Ucd_Grapheme_Property_LV,
	Ucd_Grapheme_Property_LVT,
	Ucd_Grapheme_Property_ZWJ,
	Ucd_Grapheme_Property_COUNT,
};


Ucd_Grapheme_Property ucd_grapheme_property(uint32_t codepoint);
const char *enum_string(Ucd_Grapheme_Property value);


