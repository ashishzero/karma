#pragma once
#include "modules/core/karma.h"
#include "modules/core/unicode.h"
#include "modules/core/length_string.h"

#define stbtt_uint8        u8
#define stbtt_uint16       u16
#define stbtt_uint32       u32
#define stbtt_int8         s8
#define stbtt_int16        s16
#define stbtt_int32        s32
#define STBTT_malloc(x, u) ((void)(u), mallocate(x))
#define STBTT_free(x, u)   ((void)(u), mfree(x))
#define STBTT_assert(x)    assert(x)
#define STB_TRUETYPE_IMPLEMENTATION
#include "modules/core/stb_truetype.h"

enum Lookup_Flag_Bit : u32 {
	Lookup_Flag_RIGHT_TO_LEFT          = 0x0001,
	Lookup_Flag_IGNORE_BASE_GLYPH      = 0x0002,
	Lookup_Flag_IGNORE_LIGATURES       = 0x0004,
	Lookup_Flag_IGNORE_MARKS           = 0x0008,
	Lookup_Flag_USE_MARK_FILTERING_SET = 0x0010,
	Lookup_Flag_RESERVED               = 0x00e0,
	Lookup_Flag_MARK_ATTACHMENT_TYPE   = 0xff00
};
typedef u16 Lookup_Flag;

enum Script_Tag : u32 {
	Script_Tag_ADLAM                             = tag32be("adlm"),
	Script_Tag_AHOM                              = tag32be("ahom"),
	Script_Tag_ANATOLIAN_HIEROGLYPHS             = tag32be("hluw"),
	Script_Tag_ARABIC                            = tag32be("arab"),
	Script_Tag_ARMENIAN                          = tag32be("armn"),
	Script_Tag_AVESTAN                           = tag32be("avst"),
	Script_Tag_BALINESE                          = tag32be("bali"),
	Script_Tag_BAMUM                             = tag32be("bamu"),
	Script_Tag_BASSA_VAH                         = tag32be("bass"),
	Script_Tag_BATAK                             = tag32be("batk"),
	Script_Tag_BENGALI                           = tag32be("beng"),
	Script_Tag_BENGALI2                          = tag32be("bng2"),
	Script_Tag_BHAIKSUKI                         = tag32be("bhks"),
	Script_Tag_BOPOMOFO                          = tag32be("bopo"),
	Script_Tag_BRAHMI                            = tag32be("brah"),
	Script_Tag_BRAILLE                           = tag32be("brai"),
	Script_Tag_BUGINESE                          = tag32be("bugi"),
	Script_Tag_BUHID                             = tag32be("buhd"),
	Script_Tag_BYZANTINE_MUSIC                   = tag32be("byzm"),
	Script_Tag_CANADIAN_SYLLABICS                = tag32be("cans"),
	Script_Tag_CARIAN                            = tag32be("cari"),
	Script_Tag_CAUCASIAN_ALBANIAN                = tag32be("aghb"),
	Script_Tag_CHAKMA                            = tag32be("cakm"),
	Script_Tag_CHAM                              = tag32be("cham"),
	Script_Tag_CHEROKEE                          = tag32be("cher"),
	Script_Tag_CJK_IDEOGRAPHIC                   = tag32be("hani"),
	Script_Tag_COPTIC                            = tag32be("copt"),
	Script_Tag_CYPRIOT_SYLLABARY                 = tag32be("cprt"),
	Script_Tag_CYRILLIC                          = tag32be("cyrl"),
	Script_Tag_DEFAULT                           = tag32be("DFLT"),
	Script_Tag_DESERET                           = tag32be("dsrt"),
	Script_Tag_DEVANAGARI                        = tag32be("deva"),
	Script_Tag_DEVANAGARI2                       = tag32be("dev2"),
	Script_Tag_DOGRA                             = tag32be("dogr"),
	Script_Tag_DUPLOYAN                          = tag32be("dupl"),
	Script_Tag_EGYPTIAN_HIEROGLYPHS              = tag32be("egyp"),
	Script_Tag_ELBASAN                           = tag32be("elba"),
	Script_Tag_ETHIOPIC                          = tag32be("ethi"),
	Script_Tag_GEORGIAN                          = tag32be("geor"),
	Script_Tag_GLAGOLITIC                        = tag32be("glag"),
	Script_Tag_GOTHIC                            = tag32be("goth"),
	Script_Tag_GRANTHA                           = tag32be("gran"),
	Script_Tag_GREEK                             = tag32be("grek"),
	Script_Tag_GUJARATI                          = tag32be("gujr"),
	Script_Tag_GUJARATI2                         = tag32be("gjr2"),
	Script_Tag_GUNJALA_GONDI                     = tag32be("gong"),
	Script_Tag_GURMUKHI                          = tag32be("guru"),
	Script_Tag_GURMUKHI2                         = tag32be("gur2"),
	Script_Tag_HANGUL                            = tag32be("hang"),
	Script_Tag_HANGUL_JAMO                       = tag32be("jamo"),
	Script_Tag_HANIFI_ROHINGYA                   = tag32be("rohg"),
	Script_Tag_HANUNOO                           = tag32be("hano"),
	Script_Tag_HATRAN                            = tag32be("hatr"),
	Script_Tag_HEBREW                            = tag32be("hebr"),
	Script_Tag_KANA                              = tag32be("kana"),
	Script_Tag_IMPERIAL_ARAMAIC                  = tag32be("armi"),
	Script_Tag_INSCRIPTIONAL_PAHLAVI             = tag32be("phli"),
	Script_Tag_INSCRIPTIONAL_PARTHIAN            = tag32be("prti"),
	Script_Tag_JAVANESE                          = tag32be("java"),
	Script_Tag_KAITHI                            = tag32be("kthi"),
	Script_Tag_KANNADA                           = tag32be("knda"),
	Script_Tag_KANNADA2                          = tag32be("knd2"),
	Script_Tag_KAYAH_LI                          = tag32be("kali"),
	Script_Tag_KHAROSTHI                         = tag32be("khar"),
	Script_Tag_KHMER                             = tag32be("khmr"),
	Script_Tag_KHOJKI                            = tag32be("khoj"),
	Script_Tag_KHUDAWADI                         = tag32be("sind"),
	Script_Tag_LAO                               = tag32be("lao "),
	Script_Tag_LATIN                             = tag32be("latn"),
	Script_Tag_LEPCHA                            = tag32be("lepc"),
	Script_Tag_LIMBU                             = tag32be("limb"),
	Script_Tag_LINEAR_A                          = tag32be("lina"),
	Script_Tag_LINEAR_B                          = tag32be("linb"),
	Script_Tag_LISU                              = tag32be("lisu"),
	Script_Tag_LYCIAN                            = tag32be("lyci"),
	Script_Tag_LYDIAN                            = tag32be("lydi"),
	Script_Tag_MAHAJANI                          = tag32be("mahj"),
	Script_Tag_MAKASAR                           = tag32be("maka"),
	Script_Tag_MALAYALAM                         = tag32be("mlym"),
	Script_Tag_MALAYALAM2                        = tag32be("mlm2"),
	Script_Tag_MANDAIC_MANDAEAN                  = tag32be("mand"),
	Script_Tag_MANICHAEAN                        = tag32be("mani"),
	Script_Tag_MARCHEN                           = tag32be("marc"),
	Script_Tag_MASARAM_GONDI                     = tag32be("gonm"),
	Script_Tag_MATHEMATICAL_ALPHANUMERIC_SYMBOLS = tag32be("math"),
	Script_Tag_MEDEFAIDRIN                       = tag32be("medf"),
	Script_Tag_MEITEI_MAYEK                      = tag32be("mtei"),
	Script_Tag_MENDE_KIKAKUI                     = tag32be("mend"),
	Script_Tag_MEROITIC_CURSIVE                  = tag32be("merc"),
	Script_Tag_MEROITIC_HIEROGLYPHS              = tag32be("mero"),
	Script_Tag_MIAO                              = tag32be("plrd"),
	Script_Tag_MODI                              = tag32be("modi"),
	Script_Tag_MONGOLIAN                         = tag32be("mong"),
	Script_Tag_MRO                               = tag32be("mroo"),
	Script_Tag_MULTANI                           = tag32be("mult"),
	Script_Tag_MUSICAL_SYMBOLS                   = tag32be("musc"),
	Script_Tag_MYANMAR                           = tag32be("mymr"),
	Script_Tag_MYANMAR2                          = tag32be("mym2"),
	Script_Tag_NABATAEAN                         = tag32be("nbat"),
	Script_Tag_NEWA                              = tag32be("newa"),
	Script_Tag_NEW_TAI_LUE                       = tag32be("talu"),
	Script_Tag_N_KO                              = tag32be("nko "),
	Script_Tag_NUSHU                             = tag32be("nshu"),
	Script_Tag_ODIA                              = tag32be("orya"),
	Script_Tag_ODIA2                             = tag32be("ory2"),
	Script_Tag_OGHAM                             = tag32be("ogam"),
	Script_Tag_OL_CHIKI                          = tag32be("olck"),
	Script_Tag_OLD_ITALIC                        = tag32be("ital"),
	Script_Tag_OLD_HUNGARIAN                     = tag32be("hung"),
	Script_Tag_OLD_NORTH_ARABIAN                 = tag32be("narb"),
	Script_Tag_OLD_PERMIC                        = tag32be("perm"),
	Script_Tag_OLD_PERSIAN_CUNEIFORM             = tag32be("xpeo"),
	Script_Tag_OLD_SOGDIAN                       = tag32be("sogo"),
	Script_Tag_OLD_SOUTH_ARABIAN                 = tag32be("sarb"),
	Script_Tag_OLD_TURKIC_ORKHON_RUNIC           = tag32be("orkh"),
	Script_Tag_OSAGE                             = tag32be("osge"),
	Script_Tag_OSMANYA                           = tag32be("osma"),
	Script_Tag_PAHAWH_HMONG                      = tag32be("hmng"),
	Script_Tag_PALMYRENE                         = tag32be("palm"),
	Script_Tag_PAU_CIN_HAU                       = tag32be("pauc"),
	Script_Tag_PHAGS                             = tag32be("phag"),
	Script_Tag_PHOENICIAN                        = tag32be("phnx"),
	Script_Tag_PSALTER_PAHLAVI                   = tag32be("phlp"),
	Script_Tag_REJANG                            = tag32be("rjng"),
	Script_Tag_RUNIC                             = tag32be("runr"),
	Script_Tag_SAMARITAN                         = tag32be("samr"),
	Script_Tag_SAURASHTRA                        = tag32be("saur"),
	Script_Tag_SHARADA                           = tag32be("shrd"),
	Script_Tag_SHAVIAN                           = tag32be("shaw"),
	Script_Tag_SIDDHAM                           = tag32be("sidd"),
	Script_Tag_SIGN_WRITING                      = tag32be("sgnw"),
	Script_Tag_SINHALA                           = tag32be("sinh"),
	Script_Tag_SOGDIAN                           = tag32be("sogd"),
	Script_Tag_SORA_SOMPENG                      = tag32be("sora"),
	Script_Tag_SOYOMBO                           = tag32be("soyo"),
	Script_Tag_SUMERO_AKKADIAN_CUNEIFORM         = tag32be("xsux"),
	Script_Tag_SUNDANESE                         = tag32be("sund"),
	Script_Tag_SYLOTI_NAGRI                      = tag32be("sylo"),
	Script_Tag_SYRIAC                            = tag32be("syrc"),
	Script_Tag_TAGALOG                           = tag32be("tglg"),
	Script_Tag_TAGBANWA                          = tag32be("tagb"),
	Script_Tag_TAI_LE                            = tag32be("tale"),
	Script_Tag_TAI_THAM                          = tag32be("lana"),
	Script_Tag_TAI_VIET                          = tag32be("tavt"),
	Script_Tag_TAKRI                             = tag32be("takr"),
	Script_Tag_TAMIL                             = tag32be("taml"),
	Script_Tag_TAMIL2                            = tag32be("tml2"),
	Script_Tag_TANGUT                            = tag32be("tang"),
	Script_Tag_TELUGU                            = tag32be("telu"),
	Script_Tag_TELUGU2                           = tag32be("tel2"),
	Script_Tag_THAANA                            = tag32be("thaa"),
	Script_Tag_THAI                              = tag32be("thai"),
	Script_Tag_TIBETAN                           = tag32be("tibt"),
	Script_Tag_TIFINAGH                          = tag32be("tfng"),
	Script_Tag_TIRHUTA                           = tag32be("tirh"),
	Script_Tag_UGARITIC_CUNEIFORM                = tag32be("ugar"),
	Script_Tag_VAI                               = tag32be("vai "),
	Script_Tag_WARANG_CITI                       = tag32be("wara"),
	Script_Tag_YI                                = tag32be("yi  "),
	Script_Tag_ZANABAZAR_SQUARE                  = tag32be("zanb"),
};

enum Feature_Tag : u32 {
	Feature_Tag_AALT = tag32be("aalt"), // Access All Alternates
	Feature_Tag_ABVF = tag32be("abvf"), // Above-base Forms
	Feature_Tag_ABVM = tag32be("abvm"), // Above-base Mark Positioning
	Feature_Tag_ABVS = tag32be("abvs"), // Above-base Substitutions
	Feature_Tag_AFRC = tag32be("afrc"), // Alternative Fractions
	Feature_Tag_AKHN = tag32be("akhn"), // Akhands
	Feature_Tag_BLWF = tag32be("blwf"), // Below-base Forms
	Feature_Tag_BLWM = tag32be("blwm"), // Below-base Mark Positioning
	Feature_Tag_BLWS = tag32be("blws"), // Below-base Substitutions
	Feature_Tag_CALT = tag32be("calt"), // Contextual Alternates
	Feature_Tag_CASE = tag32be("case"), // Case-Sensitive Forms
	Feature_Tag_CCMP = tag32be("ccmp"), // Glyph Composition / Decomposition
	Feature_Tag_CFAR = tag32be("cfar"), // Conjunct Form After Ro
	Feature_Tag_CJCT = tag32be("cjct"), // Conjunct Forms
	Feature_Tag_CLIG = tag32be("clig"), // Contextual Ligatures
	Feature_Tag_CPCT = tag32be("cpct"), // Centered CJK Punctuation
	Feature_Tag_CPSP = tag32be("cpsp"), // Capital Spacing
	Feature_Tag_CSWH = tag32be("cswh"), // Contextual Swash
	Feature_Tag_CURS = tag32be("curs"), // Cursive Positioning
	Feature_Tag_CV01 = tag32be("cv01"), // Character Variants
	Feature_Tag_C2PC = tag32be("c2pc"), // Petite Capitals From Capitals
	Feature_Tag_C2SC = tag32be("c2sc"), // Small Capitals From Capitals
	Feature_Tag_DIST = tag32be("dist"), // Distances
	Feature_Tag_DLIG = tag32be("dlig"), // Discretionary Ligatures
	Feature_Tag_DNOM = tag32be("dnom"), // Denominators
	Feature_Tag_DTLS = tag32be("dtls"), // Dotless Forms
	Feature_Tag_EXPT = tag32be("expt"), // Expert Forms
	Feature_Tag_FALT = tag32be("falt"), // Final Glyph on Line Alternates
	Feature_Tag_FIN2 = tag32be("fin2"), // Terminal Forms #2
	Feature_Tag_FIN3 = tag32be("fin3"), // Terminal Forms #3
	Feature_Tag_FINA = tag32be("fina"), // Terminal Forms
	Feature_Tag_FLAC = tag32be("flac"), // Flattened accent forms
	Feature_Tag_FRAC = tag32be("frac"), // Fractions
	Feature_Tag_FWID = tag32be("fwid"), // Full Widths
	Feature_Tag_HALF = tag32be("half"), // Half Forms
	Feature_Tag_HALN = tag32be("haln"), // Halant Forms
	Feature_Tag_HALT = tag32be("halt"), // Alternate Half Widths
	Feature_Tag_HIST = tag32be("hist"), // Historical Forms
	Feature_Tag_HKNA = tag32be("hkna"), // Horizontal Kana Alternates
	Feature_Tag_HLIG = tag32be("hlig"), // Historical Ligatures
	Feature_Tag_HNGL = tag32be("hngl"), // Hangul
	Feature_Tag_HOJO = tag32be("hojo"), // Hojo Kanji Forms (JIS X 0212-1990 Kanji Forms)
	Feature_Tag_HWID = tag32be("hwid"), // Half Widths
	Feature_Tag_INIT = tag32be("init"), // Initial Forms
	Feature_Tag_ISOL = tag32be("isol"), // Isolated Forms
	Feature_Tag_ITAL = tag32be("ital"), // Italics
	Feature_Tag_JALT = tag32be("jalt"), // Justification Alternates
	Feature_Tag_JP78 = tag32be("jp78"), // JIS78 Forms
	Feature_Tag_JP83 = tag32be("jp83"), // JIS83 Forms
	Feature_Tag_JP90 = tag32be("jp90"), // JIS90 Forms
	Feature_Tag_JP04 = tag32be("jp04"), // JIS2004 Forms
	Feature_Tag_KERN = tag32be("kern"), // Kerning
	Feature_Tag_LFBD = tag32be("lfbd"), // Left Bounds
	Feature_Tag_LIGA = tag32be("liga"), // Standard Ligatures
	Feature_Tag_LJMO = tag32be("ljmo"), // Leading Jamo Forms
	Feature_Tag_LNUM = tag32be("lnum"), // Lining Figures
	Feature_Tag_LOCL = tag32be("locl"), // Localized Forms
	Feature_Tag_LTRA = tag32be("ltra"), // Left-to-right alternates
	Feature_Tag_LTRM = tag32be("ltrm"), // Left-to-right mirrored forms
	Feature_Tag_MARK = tag32be("mark"), // Mark Positioning
	Feature_Tag_MED2 = tag32be("med2"), // Medial Forms #2
	Feature_Tag_MEDI = tag32be("medi"), // Medial Forms
	Feature_Tag_MGRK = tag32be("mgrk"), // Mathematical Greek
	Feature_Tag_MKMK = tag32be("mkmk"), // Mark to Mark Positioning
	Feature_Tag_MSET = tag32be("mset"), // Mark Positioning via Substitution
	Feature_Tag_NALT = tag32be("nalt"), // Alternate Annotation Forms
	Feature_Tag_NLCK = tag32be("nlck"), // NLC Kanji Forms
	Feature_Tag_NUKT = tag32be("nukt"), // Nukta Forms
	Feature_Tag_NUMR = tag32be("numr"), // Numerators
	Feature_Tag_ONUM = tag32be("onum"), // Oldstyle Figures
	Feature_Tag_OPBD = tag32be("opbd"), // Optical Bounds
	Feature_Tag_ORDN = tag32be("ordn"), // Ordinals
	Feature_Tag_ORNM = tag32be("ornm"), // Ornaments
	Feature_Tag_PALT = tag32be("palt"), // Proportional Alternate Widths
	Feature_Tag_PCAP = tag32be("pcap"), // Petite Capitals
	Feature_Tag_PKNA = tag32be("pkna"), // Proportional Kana
	Feature_Tag_PNUM = tag32be("pnum"), // Proportional Figures
	Feature_Tag_PREF = tag32be("pref"), // Pre-Base Forms
	Feature_Tag_PRES = tag32be("pres"), // Pre-base Substitutions
	Feature_Tag_PSTF = tag32be("pstf"), // Post-base Forms
	Feature_Tag_PSTS = tag32be("psts"), // Post-base Substitutions
	Feature_Tag_PWID = tag32be("pwid"), // Proportional Widths
	Feature_Tag_QWID = tag32be("qwid"), // Quarter Widths
	Feature_Tag_RAND = tag32be("rand"), // Randomize
	Feature_Tag_RCLT = tag32be("rclt"), // Required Contextual Alternates
	Feature_Tag_RKRF = tag32be("rkrf"), // Rakar Forms
	Feature_Tag_RLIG = tag32be("rlig"), // Required Ligatures
	Feature_Tag_RPHF = tag32be("rphf"), // Reph Forms
	Feature_Tag_RTBD = tag32be("rtbd"), // Right Bounds
	Feature_Tag_RTLA = tag32be("rtla"), // Right-to-left alternates
	Feature_Tag_RTLM = tag32be("rtlm"), // Right-to-left mirrored forms
	Feature_Tag_RUBY = tag32be("ruby"), // Ruby Notation Forms
	Feature_Tag_RVRN = tag32be("rvrn"), // Required Variation Alternates
	Feature_Tag_SALT = tag32be("salt"), // Stylistic Alternates
	Feature_Tag_SINF = tag32be("sinf"), // Scientific Inferiors
	Feature_Tag_SIZE = tag32be("size"), // Optical size
	Feature_Tag_SMCP = tag32be("smcp"), // Small Capitals
	Feature_Tag_SMPL = tag32be("smpl"), // Simplified Forms
	Feature_Tag_SS01 = tag32be("ss01"), // Stylistic Set 1
	Feature_Tag_SS02 = tag32be("ss02"), // Stylistic Set 2
	Feature_Tag_SS03 = tag32be("ss03"), // Stylistic Set 3
	Feature_Tag_SS04 = tag32be("ss04"), // Stylistic Set 4
	Feature_Tag_SS05 = tag32be("ss05"), // Stylistic Set 5
	Feature_Tag_SS06 = tag32be("ss06"), // Stylistic Set 6
	Feature_Tag_SS07 = tag32be("ss07"), // Stylistic Set 7
	Feature_Tag_SS08 = tag32be("ss08"), // Stylistic Set 8
	Feature_Tag_SS09 = tag32be("ss09"), // Stylistic Set 9
	Feature_Tag_SS10 = tag32be("ss10"), // Stylistic Set 10
	Feature_Tag_SS11 = tag32be("ss11"), // Stylistic Set 11
	Feature_Tag_SS12 = tag32be("ss12"), // Stylistic Set 12
	Feature_Tag_SS13 = tag32be("ss13"), // Stylistic Set 13
	Feature_Tag_SS14 = tag32be("ss14"), // Stylistic Set 14
	Feature_Tag_SS15 = tag32be("ss15"), // Stylistic Set 15
	Feature_Tag_SS16 = tag32be("ss16"), // Stylistic Set 16
	Feature_Tag_SS17 = tag32be("ss17"), // Stylistic Set 17
	Feature_Tag_SS18 = tag32be("ss18"), // Stylistic Set 18
	Feature_Tag_SS19 = tag32be("ss19"), // Stylistic Set 19
	Feature_Tag_SS20 = tag32be("ss20"), // Stylistic Set 20
	Feature_Tag_SSTY = tag32be("ssty"), // Math script style alternates
	Feature_Tag_STCH = tag32be("stch"), // Stretching Glyph Decomposition
	Feature_Tag_SUBS = tag32be("subs"), // Subscript
	Feature_Tag_SUPS = tag32be("sups"), // Superscript
	Feature_Tag_SWSH = tag32be("swsh"), // Swash
	Feature_Tag_TITL = tag32be("titl"), // Titling
	Feature_Tag_TJMO = tag32be("tjmo"), // Trailing Jamo Forms
	Feature_Tag_TNAM = tag32be("tnam"), // Traditional Name Forms
	Feature_Tag_TNUM = tag32be("tnum"), // Tabular Figures
	Feature_Tag_TRAD = tag32be("trad"), // Traditional Forms
	Feature_Tag_TWID = tag32be("twid"), // Third Widths
	Feature_Tag_UNIC = tag32be("unic"), // Unicase
	Feature_Tag_VALT = tag32be("valt"), // Alternate Vertical Metrics
	Feature_Tag_VATU = tag32be("vatu"), // Vattu Variants
	Feature_Tag_VERT = tag32be("vert"), // Vertical Writing
	Feature_Tag_VHAL = tag32be("vhal"), // Alternate Vertical Half Metrics
	Feature_Tag_VJMO = tag32be("vjmo"), // Vowel Jamo Forms
	Feature_Tag_VKNA = tag32be("vkna"), // Vertical Kana Alternates
	Feature_Tag_VKRN = tag32be("vkrn"), // Vertical Kerning
	Feature_Tag_VPAL = tag32be("vpal"), // Proportional Alternate Vertical Metrics
	Feature_Tag_VRT2 = tag32be("vrt2"), // Vertical Alternates and Rotation
	Feature_Tag_VRTR = tag32be("vrtr"), // Vertical Alternates for Rotation
	Feature_Tag_ZERO = tag32be("zero"), // Slashed Zero
};

enum Lang_Tag : u32 {
	Lang_Tag_ABAZA                       = tag32be("ABA "),
	Lang_Tag_ABKHAZIAN                   = tag32be("ABK "),
	Lang_Tag_ACHOLI                      = tag32be("ACH "),
	Lang_Tag_ACHI                        = tag32be("ACR "),
	Lang_Tag_ADYGHE                      = tag32be("ADY "),
	Lang_Tag_AFRIKAANS                   = tag32be("AFK "),
	Lang_Tag_AFAR                        = tag32be("AFR "),
	Lang_Tag_AGAW                        = tag32be("AGW "),
	Lang_Tag_AITON                       = tag32be("AIO "),
	Lang_Tag_AKAN                        = tag32be("AKA "),
	Lang_Tag_ALSATIAN                    = tag32be("ALS "),
	Lang_Tag_ALTAI                       = tag32be("ALT "),
	Lang_Tag_AMHARIC                     = tag32be("AMH "),
	Lang_Tag_ANGLO_SAXON                 = tag32be("ANG "),
	Lang_Tag_PHONETIC_TRANSCRIPTION      = tag32be("APPH"),
	Lang_Tag_ARABIC                      = tag32be("ARA "),
	Lang_Tag_ARAGONESE                   = tag32be("ARG "),
	Lang_Tag_AARI                        = tag32be("ARI "),
	Lang_Tag_RAKHINE                     = tag32be("ARK "),
	Lang_Tag_ASSAMESE                    = tag32be("ASM "),
	Lang_Tag_ASTURIAN                    = tag32be("AST "),
	Lang_Tag_ATHAPASKAN                  = tag32be("ATH "),
	Lang_Tag_AVAR                        = tag32be("AVR "),
	Lang_Tag_AWADHI                      = tag32be("AWA "),
	Lang_Tag_AYMARA                      = tag32be("AYM "),
	Lang_Tag_TORKI                       = tag32be("AZB "),
	Lang_Tag_AZERBAIJANI                 = tag32be("AZE "),
	Lang_Tag_BADAGA                      = tag32be("BAD "),
	Lang_Tag_BANDA                       = tag32be("BAD0"),
	Lang_Tag_BAGHELKHANDI                = tag32be("BAG "),
	Lang_Tag_BALKAR                      = tag32be("BAL "),
	Lang_Tag_BALINESE                    = tag32be("BAN "),
	Lang_Tag_BAVARIAN                    = tag32be("BAR "),
	Lang_Tag_BAULE                       = tag32be("BAU "),
	Lang_Tag_BATAK_TOBA                  = tag32be("BBC "),
	Lang_Tag_BERBER                      = tag32be("BBR "),
	Lang_Tag_BENCH                       = tag32be("BCH "),
	Lang_Tag_BIBLE_CREE                  = tag32be("BCR "),
	Lang_Tag_BANDJALANG                  = tag32be("BDY "),
	Lang_Tag_BELARUSSIAN                 = tag32be("BEL "),
	Lang_Tag_BEMBA                       = tag32be("BEM "),
	Lang_Tag_BENGALI                     = tag32be("BEN "),
	Lang_Tag_HARYANVI                    = tag32be("BGC "),
	Lang_Tag_BAGRI                       = tag32be("BGQ "),
	Lang_Tag_BULGARIAN                   = tag32be("BGR "),
	Lang_Tag_BHILI                       = tag32be("BHI "),
	Lang_Tag_BHOJPURI                    = tag32be("BHO "),
	Lang_Tag_BIKOL                       = tag32be("BIK "),
	Lang_Tag_BILEN                       = tag32be("BIL "),
	Lang_Tag_BISLAMA                     = tag32be("BIS "),
	Lang_Tag_KANAUJI                     = tag32be("BJJ "),
	Lang_Tag_BLACKFOOT                   = tag32be("BKF "),
	Lang_Tag_BALUCHI                     = tag32be("BLI "),
	Lang_Tag_PAO_KAREN                   = tag32be("BLK "),
	Lang_Tag_BALANTE                     = tag32be("BLN "),
	Lang_Tag_BALTI                       = tag32be("BLT "),
	Lang_Tag_BAMBARA_BAMANANKAN          = tag32be("BMB "),
	Lang_Tag_BAMILEKE                    = tag32be("BML "),
	Lang_Tag_BOSNIAN                     = tag32be("BOS "),
	Lang_Tag_BISHNUPRIYA_MANIPURI        = tag32be("BPY "),
	Lang_Tag_BRETON                      = tag32be("BRE "),
	Lang_Tag_BRAHUI                      = tag32be("BRH "),
	Lang_Tag_BRAJ_BHASHA                 = tag32be("BRI "),
	Lang_Tag_BURMESE                     = tag32be("BRM "),
	Lang_Tag_BODO                        = tag32be("BRX "),
	Lang_Tag_BASHKIR                     = tag32be("BSH "),
	Lang_Tag_BURUSHASKI                  = tag32be("BSK "),
	Lang_Tag_BETI                        = tag32be("BTI "),
	Lang_Tag_BATAK_SIMALUNGUN            = tag32be("BTS "),
	Lang_Tag_BUGIS                       = tag32be("BUG "),
	Lang_Tag_MEDUMBA                     = tag32be("BYV "),
	Lang_Tag_KAQCHIKEL                   = tag32be("CAK "),
	Lang_Tag_CATALAN                     = tag32be("CAT "),
	Lang_Tag_ZAMBOANGA_CHAVACANO         = tag32be("CBK "),
	Lang_Tag_CHINANTEC                   = tag32be("CCHN"),
	Lang_Tag_CEBUANO                     = tag32be("CEB "),
	Lang_Tag_CHECHEN                     = tag32be("CHE "),
	Lang_Tag_CHAHA_GURAGE                = tag32be("CHG "),
	Lang_Tag_CHATTISGARHI                = tag32be("CHH "),
	Lang_Tag_CHICHEWA_CHEWA_NYANJA       = tag32be("CHI "),
	Lang_Tag_CHUKCHI                     = tag32be("CHK "),
	Lang_Tag_CHUUKESE                    = tag32be("CHK0"),
	Lang_Tag_CHOCTAW                     = tag32be("CHO "),
	Lang_Tag_CHIPEWYAN                   = tag32be("CHP "),
	Lang_Tag_CHEROKEE                    = tag32be("CHR "),
	Lang_Tag_CHAMORRO                    = tag32be("CHA "),
	Lang_Tag_CHUVASH                     = tag32be("CHU "),
	Lang_Tag_CHEYENNE                    = tag32be("CHY "),
	Lang_Tag_CHIGA                       = tag32be("CGG "),
	Lang_Tag_WESTERN_CHAM                = tag32be("CJA "),
	Lang_Tag_EASTERN_CHAM                = tag32be("CJM "),
	Lang_Tag_COMORIAN                    = tag32be("CMR "),
	Lang_Tag_COPTIC                      = tag32be("COP "),
	Lang_Tag_CORNISH                     = tag32be("COR "),
	Lang_Tag_CORSICAN                    = tag32be("COS "),
	Lang_Tag_CREOLES                     = tag32be("CPP "),
	Lang_Tag_CREE                        = tag32be("CRE "),
	Lang_Tag_CARRIER                     = tag32be("CRR "),
	Lang_Tag_CRIMEAN_TATAR               = tag32be("CRT "),
	Lang_Tag_KASHUBIAN                   = tag32be("CSB "),
	Lang_Tag_CHURCH_SLAVONIC             = tag32be("CSL "),
	Lang_Tag_CZECH                       = tag32be("CSY "),
	Lang_Tag_CHITTAGONIAN                = tag32be("CTG "),
	Lang_Tag_SAN_BLAS_KUNA               = tag32be("CUK "),
	Lang_Tag_DANISH                      = tag32be("DAN "),
	Lang_Tag_DARGWA                      = tag32be("DAR "),
	Lang_Tag_DAYI                        = tag32be("DAX "),
	Lang_Tag_WOODS_CREE                  = tag32be("DCR "),
	Lang_Tag_GERMAN                      = tag32be("DEU "),
	Lang_Tag_DOGRI1                      = tag32be("DGO "),
	Lang_Tag_DOGRI2                      = tag32be("DGR "),
	Lang_Tag_DHANGU                      = tag32be("DHG "),
	Lang_Tag_DIMLI                       = tag32be("DIQ "),
	Lang_Tag_DIVEHI_DHIVEHI_MALDIVIAN    = tag32be("DIV "),
	Lang_Tag_ZARMA                       = tag32be("DJR "),
	Lang_Tag_DJAMBARRPUYNGU              = tag32be("DJR0"),
	Lang_Tag_DANGME                      = tag32be("DNG "),
	Lang_Tag_DAN                         = tag32be("DNJ "),
	Lang_Tag_DINKA                       = tag32be("DNK "),
	Lang_Tag_DARI                        = tag32be("DRI "),
	Lang_Tag_DHUWAL                      = tag32be("DUJ "),
	Lang_Tag_DUNGAN                      = tag32be("DUN "),
	Lang_Tag_DZONGKHA                    = tag32be("DZN "),
	Lang_Tag_EBIRA                       = tag32be("EBI "),
	Lang_Tag_EASTERN_CREE                = tag32be("ECR "),
	Lang_Tag_EDO                         = tag32be("EDO "),
	Lang_Tag_EFIK                        = tag32be("EFI "),
	Lang_Tag_GREEK                       = tag32be("ELL "),
	Lang_Tag_EASTERN_MANINKAKAN          = tag32be("EMK "),
	Lang_Tag_ENGLISH                     = tag32be("ENG "),
	Lang_Tag_ERZYA                       = tag32be("ERZ "),
	Lang_Tag_SPANISH                     = tag32be("ESP "),
	Lang_Tag_CENTRAL_YUPIK               = tag32be("ESU "),
	Lang_Tag_ESTONIAN                    = tag32be("ETI "),
	Lang_Tag_BASQUE                      = tag32be("EUQ "),
	Lang_Tag_EVENKI                      = tag32be("EVK "),
	Lang_Tag_EVEN                        = tag32be("EVN "),
	Lang_Tag_EWE                         = tag32be("EWE "),
	Lang_Tag_FRENCH_ANTILLEAN            = tag32be("FAN "),
	Lang_Tag_FANG                        = tag32be("FAN0"),
	Lang_Tag_PERSIAN                     = tag32be("FAR "),
	Lang_Tag_FANTI                       = tag32be("FAT "),
	Lang_Tag_FINNISH                     = tag32be("FIN "),
	Lang_Tag_FIJIAN                      = tag32be("FJI "),
	Lang_Tag_DUTCH_FLEMISH               = tag32be("FLE "),
	Lang_Tag_FE_FE                       = tag32be("FMP "),
	Lang_Tag_FOREST_NENETS               = tag32be("FNE "),
	Lang_Tag_FON                         = tag32be("FON "),
	Lang_Tag_FAROESE                     = tag32be("FOS "),
	Lang_Tag_FRENCH                      = tag32be("FRA "),
	Lang_Tag_CAJUN_FRENCH                = tag32be("FRC "),
	Lang_Tag_FRISIAN                     = tag32be("FRI "),
	Lang_Tag_FRIULIAN                    = tag32be("FRL "),
	Lang_Tag_ARPITAN                     = tag32be("FRP "),
	Lang_Tag_FUTA                        = tag32be("FTA "),
	Lang_Tag_FULAH                       = tag32be("FUL "),
	Lang_Tag_NIGERIAN_FULFULDE           = tag32be("FUV "),
	Lang_Tag_GA                          = tag32be("GAD "),
	Lang_Tag_SCOTTISH_GAELIC_GAELIC      = tag32be("GAE "),
	Lang_Tag_GAGAUZ                      = tag32be("GAG "),
	Lang_Tag_GALICIAN                    = tag32be("GAL "),
	Lang_Tag_GARSHUNI                    = tag32be("GAR "),
	Lang_Tag_GARHWALI                    = tag32be("GAW "),
	Lang_Tag_GEEZ                        = tag32be("GEZ "),
	Lang_Tag_GITHABUL                    = tag32be("GIH "),
	Lang_Tag_GILYAK                      = tag32be("GIL "),
	Lang_Tag_KIRIBATI_GILBERTESE         = tag32be("GIL0"),
	Lang_Tag_KPELLE_GUINEA               = tag32be("GKP "),
	Lang_Tag_GILAKI                      = tag32be("GLK "),
	Lang_Tag_GUMUZ                       = tag32be("GMZ "),
	Lang_Tag_GUMATJ                      = tag32be("GNN "),
	Lang_Tag_GOGO                        = tag32be("GOG "),
	Lang_Tag_GONDI                       = tag32be("GON "),
	Lang_Tag_GREENLANDIC                 = tag32be("GRN "),
	Lang_Tag_GARO                        = tag32be("GRO "),
	Lang_Tag_GUARANI                     = tag32be("GUA "),
	Lang_Tag_WAYUU                       = tag32be("GUC "),
	Lang_Tag_GUPAPUYNGU                  = tag32be("GUF "),
	Lang_Tag_GUJARATI                    = tag32be("GUJ "),
	Lang_Tag_GUSII                       = tag32be("GUZ "),
	Lang_Tag_HAITIAN_HAITIAN_CREOLE      = tag32be("HAI "),
	Lang_Tag_HALAM_FALAM_CHIN            = tag32be("HAL "),
	Lang_Tag_HARAUTI                     = tag32be("HAR "),
	Lang_Tag_HAUSA                       = tag32be("HAU "),
	Lang_Tag_HAWAIIAN                    = tag32be("HAW "),
	Lang_Tag_HAYA                        = tag32be("HAY "),
	Lang_Tag_HAZARAGI                    = tag32be("HAZ "),
	Lang_Tag_HAMMER_BANNA                = tag32be("HBN "),
	Lang_Tag_HERERO                      = tag32be("HER "),
	Lang_Tag_HILIGAYNON                  = tag32be("HIL "),
	Lang_Tag_HINDI                       = tag32be("HIN "),
	Lang_Tag_HIGH_MARI                   = tag32be("HMA "),
	Lang_Tag_HMONG                       = tag32be("HMN "),
	Lang_Tag_HIRI_MOTU                   = tag32be("HMO "),
	Lang_Tag_HINDKO                      = tag32be("HND "),
	Lang_Tag_HO                          = tag32be("HO	 "),
	Lang_Tag_HARARI                      = tag32be("HRI "),
	Lang_Tag_CROATIAN                    = tag32be("HRV "),
	Lang_Tag_HUNGARIAN                   = tag32be("HUN "),
	Lang_Tag_ARMENIAN                    = tag32be("HYE "),
	Lang_Tag_ARMENIAN_EAST               = tag32be("HYE0"),
	Lang_Tag_IBAN                        = tag32be("IBA "),
	Lang_Tag_IBIBIO                      = tag32be("IBB "),
	Lang_Tag_IGBO                        = tag32be("IBO "),
	Lang_Tag_IJO_LANGUAGES               = tag32be("IJO "),
	Lang_Tag_IDO                         = tag32be("IDO "),
	Lang_Tag_INTERLINGUE                 = tag32be("ILE "),
	Lang_Tag_ILOKANO                     = tag32be("ILO "),
	Lang_Tag_INTERLINGUA                 = tag32be("INA "),
	Lang_Tag_INDONESIAN                  = tag32be("IND "),
	Lang_Tag_INGUSH                      = tag32be("ING "),
	Lang_Tag_INUKTITUT                   = tag32be("INU "),
	Lang_Tag_INUPIAT                     = tag32be("IPK "),
	Lang_Tag_PHONETIC_TRANSCRIPTION_IPA  = tag32be("IPPH"),
	Lang_Tag_IRISH                       = tag32be("IRI "),
	Lang_Tag_IRISH_TRADITIONAL           = tag32be("IRT "),
	Lang_Tag_ICELANDIC                   = tag32be("ISL "),
	Lang_Tag_INARI_SAMI                  = tag32be("ISM "),
	Lang_Tag_ITALIAN                     = tag32be("ITA "),
	Lang_Tag_HEBREW                      = tag32be("IWR "),
	Lang_Tag_JAMAICAN_CREOLE             = tag32be("JAM "),
	Lang_Tag_JAPANESE                    = tag32be("JAN "),
	Lang_Tag_JAVANESE                    = tag32be("JAV "),
	Lang_Tag_LOJBAN                      = tag32be("JBO "),
	Lang_Tag_KRYMCHAK                    = tag32be("JCT "),
	Lang_Tag_YIDDISH                     = tag32be("JII "),
	Lang_Tag_LADINO                      = tag32be("JUD "),
	Lang_Tag_JULA                        = tag32be("JUL "),
	Lang_Tag_KABARDIAN                   = tag32be("KAB "),
	Lang_Tag_KABYLE                      = tag32be("KAB0"),
	Lang_Tag_KACHCHI                     = tag32be("KAC "),
	Lang_Tag_KALENJIN                    = tag32be("KAL "),
	Lang_Tag_KANNADA                     = tag32be("KAN "),
	Lang_Tag_KARACHAY                    = tag32be("KAR "),
	Lang_Tag_GEORGIAN                    = tag32be("KAT "),
	Lang_Tag_KAZAKH                      = tag32be("KAZ "),
	Lang_Tag_MAKONDE                     = tag32be("KDE "),
	Lang_Tag_KABUVERDIANU_CRIOULO        = tag32be("KEA "),
	Lang_Tag_KEBENA                      = tag32be("KEB "),
	Lang_Tag_KEKCHI                      = tag32be("KEK "),
	Lang_Tag_KHUTSURI_GEORGIAN           = tag32be("KGE "),
	Lang_Tag_KHAKASS                     = tag32be("KHA "),
	Lang_Tag_KHANTY_KAZIM                = tag32be("KHK "),
	Lang_Tag_KHMER                       = tag32be("KHM "),
	Lang_Tag_KHANTY_SHURISHKAR           = tag32be("KHS "),
	Lang_Tag_KHAMTI_SHAN                 = tag32be("KHT "),
	Lang_Tag_KHANTY_VAKHI                = tag32be("KHV "),
	Lang_Tag_KHOWAR                      = tag32be("KHW "),
	Lang_Tag_KIKUYU_GIKUYU               = tag32be("KIK "),
	Lang_Tag_KIRGHIZ_KYRGYZ              = tag32be("KIR "),
	Lang_Tag_KISII                       = tag32be("KIS "),
	Lang_Tag_KIRMANJKI                   = tag32be("KIU "),
	Lang_Tag_SOUTHERN_KIWAI              = tag32be("KJD "),
	Lang_Tag_EASTERN_PWO_KAREN           = tag32be("KJP "),
	Lang_Tag_BUMTHANGKHA                 = tag32be("KJZ "),
	Lang_Tag_KOKNI                       = tag32be("KKN "),
	Lang_Tag_KALMYK                      = tag32be("KLM "),
	Lang_Tag_KAMBA                       = tag32be("KMB "),
	Lang_Tag_KUMAONI                     = tag32be("KMN "),
	Lang_Tag_KOMO                        = tag32be("KMO "),
	Lang_Tag_KOMSO                       = tag32be("KMS "),
	Lang_Tag_KHORASANI_TURKIC            = tag32be("KMZ "),
	Lang_Tag_KANURI                      = tag32be("KNR "),
	Lang_Tag_KODAGU                      = tag32be("KOD "),
	Lang_Tag_KOREAN_OLD_HANGUL           = tag32be("KOH "),
	Lang_Tag_KONKANI                     = tag32be("KOK "),
	Lang_Tag_KIKONGO                     = tag32be("KON "),
	Lang_Tag_KOMI                        = tag32be("KOM "),
	Lang_Tag_KONGO                       = tag32be("KON0"),
	Lang_Tag_KOMI_PERMYAK                = tag32be("KOP "),
	Lang_Tag_KOREAN                      = tag32be("KOR "),
	Lang_Tag_KOSRAEAN                    = tag32be("KOS "),
	Lang_Tag_KOMI_ZYRIAN                 = tag32be("KOZ "),
	Lang_Tag_KPELLE                      = tag32be("KPL "),
	Lang_Tag_KRIO                        = tag32be("KRI "),
	Lang_Tag_KARAKALPAK                  = tag32be("KRK "),
	Lang_Tag_KARELIAN                    = tag32be("KRL "),
	Lang_Tag_KARAIM                      = tag32be("KRM "),
	Lang_Tag_KAREN                       = tag32be("KRN "),
	Lang_Tag_KOORETE                     = tag32be("KRT "),
	Lang_Tag_KASHMIRI                    = tag32be("KSH "),
	Lang_Tag_RIPUARIAN                   = tag32be("KSH0"),
	Lang_Tag_KHASI                       = tag32be("KSI "),
	Lang_Tag_KILDIN_SAMI                 = tag32be("KSM "),
	Lang_Tag_SGAW_KAREN                  = tag32be("KSW "),
	Lang_Tag_KUANYAMA                    = tag32be("KUA "),
	Lang_Tag_KUI                         = tag32be("KUI "),
	Lang_Tag_KULVI                       = tag32be("KUL "),
	Lang_Tag_KUMYK                       = tag32be("KUM "),
	Lang_Tag_KURDISH                     = tag32be("KUR "),
	Lang_Tag_KURUKH                      = tag32be("KUU "),
	Lang_Tag_KUY                         = tag32be("KUY "),
	Lang_Tag_KORYAK                      = tag32be("KYK "),
	Lang_Tag_WESTERN_KAYAH               = tag32be("KYU "),
	Lang_Tag_LADIN                       = tag32be("LAD "),
	Lang_Tag_LAHULI                      = tag32be("LAH "),
	Lang_Tag_LAK                         = tag32be("LAK "),
	Lang_Tag_LAMBANI                     = tag32be("LAM "),
	Lang_Tag_LAO                         = tag32be("LAO "),
	Lang_Tag_LATIN                       = tag32be("LAT "),
	Lang_Tag_LAZ                         = tag32be("LAZ "),
	Lang_Tag_L_CREE                      = tag32be("LCR "),
	Lang_Tag_LADAKHI                     = tag32be("LDK "),
	Lang_Tag_LEZGI                       = tag32be("LEZ "),
	Lang_Tag_LIGURIAN                    = tag32be("LIJ "),
	Lang_Tag_LIMBURGISH                  = tag32be("LIM "),
	Lang_Tag_LINGALA                     = tag32be("LIN "),
	Lang_Tag_LISU                        = tag32be("LIS "),
	Lang_Tag_LAMPUNG                     = tag32be("LJP "),
	Lang_Tag_LAKI                        = tag32be("LKI "),
	Lang_Tag_LOW_MARI                    = tag32be("LMA "),
	Lang_Tag_LIMBU                       = tag32be("LMB "),
	Lang_Tag_LOMBARD                     = tag32be("LMO "),
	Lang_Tag_LOMWE                       = tag32be("LMW "),
	Lang_Tag_LOMA                        = tag32be("LOM "),
	Lang_Tag_LURI                        = tag32be("LRC "),
	Lang_Tag_LOWER_SORBIAN               = tag32be("LSB "),
	Lang_Tag_LULE_SAMI                   = tag32be("LSM "),
	Lang_Tag_LITHUANIAN                  = tag32be("LTH "),
	Lang_Tag_LUXEMBOURGISH               = tag32be("LTZ "),
	Lang_Tag_LUBA_LULUA                  = tag32be("LUA "),
	Lang_Tag_LUBA_KATANGA                = tag32be("LUB "),
	Lang_Tag_GANDA                       = tag32be("LUG "),
	Lang_Tag_LUYIA                       = tag32be("LUH "),
	Lang_Tag_LUO                         = tag32be("LUO "),
	Lang_Tag_LATVIAN                     = tag32be("LVI "),
	Lang_Tag_MADURA                      = tag32be("MAD "),
	Lang_Tag_MAGAHI                      = tag32be("MAG "),
	Lang_Tag_MARSHALLESE                 = tag32be("MAH "),
	Lang_Tag_MAJANG                      = tag32be("MAJ "),
	Lang_Tag_MAKHUWA                     = tag32be("MAK "),
	Lang_Tag_MALAYALAM                   = tag32be("MAL "),
	Lang_Tag_MAM                         = tag32be("MAM "),
	Lang_Tag_MANSI                       = tag32be("MAN "),
	Lang_Tag_MAPUDUNGUN                  = tag32be("MAP "),
	Lang_Tag_MARATHI                     = tag32be("MAR "),
	Lang_Tag_MARWARI                     = tag32be("MAW "),
	Lang_Tag_MBUNDU                      = tag32be("MBN "),
	Lang_Tag_MBO                         = tag32be("MBO "),
	Lang_Tag_MANCHU                      = tag32be("MCH "),
	Lang_Tag_MOOSE_CREE                  = tag32be("MCR "),
	Lang_Tag_MENDE                       = tag32be("MDE "),
	Lang_Tag_MANDAR                      = tag32be("MDR "),
	Lang_Tag_MEEN                        = tag32be("MEN "),
	Lang_Tag_MERU                        = tag32be("MER "),
	Lang_Tag_PATTANI_MALAY               = tag32be("MFA "),
	Lang_Tag_MORISYEN                    = tag32be("MFE "),
	Lang_Tag_MINANGKABAU                 = tag32be("MIN "),
	Lang_Tag_MIZO                        = tag32be("MIZ "),
	Lang_Tag_MACEDONIAN                  = tag32be("MKD "),
	Lang_Tag_MAKASAR                     = tag32be("MKR "),
	Lang_Tag_KITUBA                      = tag32be("MKW "),
	Lang_Tag_MALE                        = tag32be("MLE "),
	Lang_Tag_MALAGASY                    = tag32be("MLG "),
	Lang_Tag_MALINKE                     = tag32be("MLN "),
	Lang_Tag_MALAYALAM_REFORMED          = tag32be("MLR "),
	Lang_Tag_MALAY                       = tag32be("MLY "),
	Lang_Tag_MANDINKA                    = tag32be("MND "),
	Lang_Tag_MONGOLIAN                   = tag32be("MNG "),
	Lang_Tag_MANIPURI                    = tag32be("MNI "),
	Lang_Tag_MANINKA                     = tag32be("MNK "),
	Lang_Tag_MANX                        = tag32be("MNX "),
	Lang_Tag_MOHAWK                      = tag32be("MOH "),
	Lang_Tag_MOKSHA                      = tag32be("MOK "),
	Lang_Tag_MOLDAVIAN                   = tag32be("MOL "),
	Lang_Tag_MON                         = tag32be("MON "),
	Lang_Tag_MOROCCAN                    = tag32be("MOR "),
	Lang_Tag_MOSSI                       = tag32be("MOS "),
	Lang_Tag_MAORI                       = tag32be("MRI "),
	Lang_Tag_MAITHILI                    = tag32be("MTH "),
	Lang_Tag_MALTESE                     = tag32be("MTS "),
	Lang_Tag_MUNDARI                     = tag32be("MUN "),
	Lang_Tag_MUSCOGEE                    = tag32be("MUS "),
	Lang_Tag_MIRANDESE                   = tag32be("MWL "),
	Lang_Tag_HMONG_DAW                   = tag32be("MWW "),
	Lang_Tag_MAYAN                       = tag32be("MYN "),
	Lang_Tag_MAZANDERANI                 = tag32be("MZN "),
	Lang_Tag_NAGA_ASSAMESE               = tag32be("NAG "),
	Lang_Tag_NAHUATL                     = tag32be("NAH "),
	Lang_Tag_NANAI                       = tag32be("NAN "),
	Lang_Tag_NEAPOLITAN                  = tag32be("NAP "),
	Lang_Tag_NASKAPI                     = tag32be("NAS "),
	Lang_Tag_NAURUAN                     = tag32be("NAU "),
	Lang_Tag_NAVAJO                      = tag32be("NAV "),
	Lang_Tag_N_CREE                      = tag32be("NCR "),
	Lang_Tag_NDEBELE                     = tag32be("NDB "),
	Lang_Tag_NDAU                        = tag32be("NDC "),
	Lang_Tag_NDONGA                      = tag32be("NDG "),
	Lang_Tag_LOW_SAXON                   = tag32be("NDS "),
	Lang_Tag_NEPALI                      = tag32be("NEP "),
	Lang_Tag_NEWARI                      = tag32be("NEW "),
	Lang_Tag_NGBAKA                      = tag32be("NGA "),
	Lang_Tag_NAGARI                      = tag32be("NGR "),
	Lang_Tag_NORWAY_HOUSE_CREE           = tag32be("NHC "),
	Lang_Tag_NISI                        = tag32be("NIS "),
	Lang_Tag_NIUEAN                      = tag32be("NIU "),
	Lang_Tag_NYANKOLE                    = tag32be("NKL "),
	Lang_Tag_NKO                         = tag32be("NKO "),
	Lang_Tag_DUTCH                       = tag32be("NLD "),
	Lang_Tag_NIMADI                      = tag32be("NOE "),
	Lang_Tag_NOGAI                       = tag32be("NOG "),
	Lang_Tag_NORWEGIAN                   = tag32be("NOR "),
	Lang_Tag_NOVIAL                      = tag32be("NOV "),
	Lang_Tag_NORTHERN_SAMI               = tag32be("NSM "),
	Lang_Tag_SOTHO_NORTHERN              = tag32be("NSO "),
	Lang_Tag_NORTHERN_TAI                = tag32be("NTA "),
	Lang_Tag_ESPERANTO                   = tag32be("NTO "),
	Lang_Tag_NYAMWEZI                    = tag32be("NYM "),
	Lang_Tag_NORWEGIAN_NYNORSK           = tag32be("NYN "),
	Lang_Tag_MBEMBE_TIGON                = tag32be("NZA "),
	Lang_Tag_OCCITAN                     = tag32be("OCI "),
	Lang_Tag_OJI_CREE                    = tag32be("OCR "),
	Lang_Tag_OJIBWAY                     = tag32be("OJB "),
	Lang_Tag_ODIA_FORMERLY_ORIYA         = tag32be("ORI "),
	Lang_Tag_OROMO                       = tag32be("ORO "),
	Lang_Tag_OSSETIAN                    = tag32be("OSS "),
	Lang_Tag_PALESTINIAN_ARAMAIC         = tag32be("PAA "),
	Lang_Tag_PANGASINAN                  = tag32be("PAG "),
	Lang_Tag_PALI                        = tag32be("PAL "),
	Lang_Tag_PAMPANGAN                   = tag32be("PAM "),
	Lang_Tag_PUNJABI                     = tag32be("PAN "),
	Lang_Tag_PALPA                       = tag32be("PAP "),
	Lang_Tag_PAPIAMENTU                  = tag32be("PAP0"),
	Lang_Tag_PASHTO                      = tag32be("PAS "),
	Lang_Tag_PALAUAN                     = tag32be("PAU "),
	Lang_Tag_BOUYEI                      = tag32be("PCC "),
	Lang_Tag_PICARD                      = tag32be("PCD "),
	Lang_Tag_PENNSYLVANIA_GERMAN         = tag32be("PDC "),
	Lang_Tag_POLYTONIC_GREEK             = tag32be("PGR "),
	Lang_Tag_PHAKE                       = tag32be("PHK "),
	Lang_Tag_NORFOLK                     = tag32be("PIH "),
	Lang_Tag_FILIPINO                    = tag32be("PIL "),
	Lang_Tag_PALAUNG                     = tag32be("PLG "),
	Lang_Tag_POLISH                      = tag32be("PLK "),
	Lang_Tag_PIEMONTESE                  = tag32be("PMS "),
	Lang_Tag_WESTERN_PANJABI             = tag32be("PNB "),
	Lang_Tag_POCOMCHI                    = tag32be("POH "),
	Lang_Tag_POHNPEIAN                   = tag32be("PON "),
	Lang_Tag_PROVENCAL_OLD_PROVENCAL     = tag32be("PRO "),
	Lang_Tag_PORTUGUESE                  = tag32be("PTG "),
	Lang_Tag_WESTERN_PWO_KAREN           = tag32be("PWO "),
	Lang_Tag_CHIN                        = tag32be("QIN "),
	Lang_Tag_KICHE                       = tag32be("QUC "),
	Lang_Tag_QUECHUA_BOLIVIA             = tag32be("QUH "),
	Lang_Tag_QUECHUA                     = tag32be("QUZ "),
	Lang_Tag_QUECHUA_ECUADOR             = tag32be("QVI "),
	Lang_Tag_QUECHUA_PERU                = tag32be("QWH "),
	Lang_Tag_RAJASTHANI                  = tag32be("RAJ "),
	Lang_Tag_RAROTONGAN                  = tag32be("RAR "),
	Lang_Tag_RUSSIAN_BURIAT              = tag32be("RBU "),
	Lang_Tag_R_CREE                      = tag32be("RCR "),
	Lang_Tag_REJANG                      = tag32be("REJ "),
	Lang_Tag_RIANG                       = tag32be("RIA "),
	Lang_Tag_TARIFIT                     = tag32be("RIF "),
	Lang_Tag_RITARUNGO                   = tag32be("RIT "),
	Lang_Tag_ARAKWAL                     = tag32be("RKW "),
	Lang_Tag_ROMANSH                     = tag32be("RMS "),
	Lang_Tag_VLAX_ROMANI                 = tag32be("RMY "),
	Lang_Tag_ROMANIAN                    = tag32be("ROM "),
	Lang_Tag_ROMANY                      = tag32be("ROY "),
	Lang_Tag_RUSYN                       = tag32be("RSY "),
	Lang_Tag_ROTUMAN                     = tag32be("RTM "),
	Lang_Tag_KINYARWANDA                 = tag32be("RUA "),
	Lang_Tag_RUNDI                       = tag32be("RUN "),
	Lang_Tag_AROMANIAN                   = tag32be("RUP "),
	Lang_Tag_RUSSIAN                     = tag32be("RUS "),
	Lang_Tag_SADRI                       = tag32be("SAD "),
	Lang_Tag_SANSKRIT                    = tag32be("SAN "),
	Lang_Tag_SASAK                       = tag32be("SAS "),
	Lang_Tag_SANTALI                     = tag32be("SAT "),
	Lang_Tag_SAYISI                      = tag32be("SAY "),
	Lang_Tag_SICILIAN                    = tag32be("SCN "),
	Lang_Tag_SCOTS                       = tag32be("SCO "),
	Lang_Tag_SEKOTA                      = tag32be("SEK "),
	Lang_Tag_SELKUP                      = tag32be("SEL "),
	Lang_Tag_OLD_IRISH                   = tag32be("SGA "),
	Lang_Tag_SANGO                       = tag32be("SGO "),
	Lang_Tag_SAMOGITIAN                  = tag32be("SGS "),
	Lang_Tag_TACHELHIT                   = tag32be("SHI "),
	Lang_Tag_SHAN                        = tag32be("SHN "),
	Lang_Tag_SIBE                        = tag32be("SIB "),
	Lang_Tag_SIDAMO                      = tag32be("SID "),
	Lang_Tag_SILTE_GURAGE                = tag32be("SIG "),
	Lang_Tag_SKOLT_SAMI                  = tag32be("SKS "),
	Lang_Tag_SLOVAK                      = tag32be("SKY "),
	Lang_Tag_NORTH_SLAVEY                = tag32be("SCS "),
	Lang_Tag_SLAVEY                      = tag32be("SLA "),
	Lang_Tag_SLOVENIAN                   = tag32be("SLV "),
	Lang_Tag_SOMALI                      = tag32be("SML "),
	Lang_Tag_SAMOAN                      = tag32be("SMO "),
	Lang_Tag_SENA                        = tag32be("SNA "),
	Lang_Tag_SHONA                       = tag32be("SNA0"),
	Lang_Tag_SINDHI                      = tag32be("SND "),
	Lang_Tag_SINHALA_SINHALESE           = tag32be("SNH "),
	Lang_Tag_SONINKE                     = tag32be("SNK "),
	Lang_Tag_SODO_GURAGE                 = tag32be("SOG "),
	Lang_Tag_SONGE                       = tag32be("SOP "),
	Lang_Tag_SOTHO_SOUTHERN              = tag32be("SOT "),
	Lang_Tag_ALBANIAN                    = tag32be("SQI "),
	Lang_Tag_SERBIAN                     = tag32be("SRB "),
	Lang_Tag_SARDINIAN                   = tag32be("SRD "),
	Lang_Tag_SARAIKI                     = tag32be("SRK "),
	Lang_Tag_SERER                       = tag32be("SRR "),
	Lang_Tag_SOUTH_SLAVEY                = tag32be("SSL "),
	Lang_Tag_SOUTHERN_SAMI               = tag32be("SSM "),
	Lang_Tag_SATERLAND_FRISIAN           = tag32be("STQ "),
	Lang_Tag_SUKUMA                      = tag32be("SUK "),
	Lang_Tag_SUNDANESE                   = tag32be("SUN "),
	Lang_Tag_SURI                        = tag32be("SUR "),
	Lang_Tag_SVAN                        = tag32be("SVA "),
	Lang_Tag_SWEDISH                     = tag32be("SVE "),
	Lang_Tag_SWADAYA_ARAMAIC             = tag32be("SWA "),
	Lang_Tag_SWAHILI                     = tag32be("SWK "),
	Lang_Tag_SWATI                       = tag32be("SWZ "),
	Lang_Tag_SUTU                        = tag32be("SXT "),
	Lang_Tag_UPPER_SAXON                 = tag32be("SXU "),
	Lang_Tag_SYLHETI                     = tag32be("SYL "),
	Lang_Tag_SYRIAC                      = tag32be("SYR "),
	Lang_Tag_SYRIAC_ESTRANGELA           = tag32be("SYRE"),
	Lang_Tag_SYRIAC_WESTERN              = tag32be("SYRJ"),
	Lang_Tag_SYRIAC_EASTERN              = tag32be("SYRN"),
	Lang_Tag_SILESIAN                    = tag32be("SZL "),
	Lang_Tag_TABASARAN                   = tag32be("TAB "),
	Lang_Tag_TAJIKI                      = tag32be("TAJ "),
	Lang_Tag_TAMIL                       = tag32be("TAM "),
	Lang_Tag_TATAR                       = tag32be("TAT "),
	Lang_Tag_TH_CREE                     = tag32be("TCR "),
	Lang_Tag_DEHONG_DAI                  = tag32be("TDD "),
	Lang_Tag_TELUGU                      = tag32be("TEL "),
	Lang_Tag_TETUM                       = tag32be("TET "),
	Lang_Tag_TAGALOG                     = tag32be("TGL "),
	Lang_Tag_TONGAN                      = tag32be("TGN "),
	Lang_Tag_TIGRE                       = tag32be("TGR "),
	Lang_Tag_TIGRINYA                    = tag32be("TGY "),
	Lang_Tag_THAI                        = tag32be("THA "),
	Lang_Tag_TAHITIAN                    = tag32be("THT "),
	Lang_Tag_TIBETAN                     = tag32be("TIB "),
	Lang_Tag_TIV                         = tag32be("TIV "),
	Lang_Tag_TURKMEN                     = tag32be("TKM "),
	Lang_Tag_TAMASHEK                    = tag32be("TMH "),
	Lang_Tag_TEMNE                       = tag32be("TMN "),
	Lang_Tag_TSWANA                      = tag32be("TNA "),
	Lang_Tag_TUNDRA_NENETS               = tag32be("TNE "),
	Lang_Tag_TONGA                       = tag32be("TNG "),
	Lang_Tag_TODO                        = tag32be("TOD "),
	Lang_Tag_TOMA                        = tag32be("TOD0"),
	Lang_Tag_TOK_PISIN                   = tag32be("TPI "),
	Lang_Tag_TURKISH                     = tag32be("TRK "),
	Lang_Tag_TSONGA                      = tag32be("TSG "),
	Lang_Tag_TSHANGLA                    = tag32be("TSJ "),
	Lang_Tag_TUROYO_ARAMAIC              = tag32be("TUA "),
	Lang_Tag_TULU                        = tag32be("TUM "),
	Lang_Tag_TUMBUKA                     = tag32be("TUL "),
	Lang_Tag_TUVIN                       = tag32be("TUV "),
	Lang_Tag_TUVALU                      = tag32be("TVL "),
	Lang_Tag_TWI                         = tag32be("TWI "),
	Lang_Tag_TAY                         = tag32be("TYZ "),
	Lang_Tag_TAMAZIGHT                   = tag32be("TZM "),
	Lang_Tag_TZOTZIL                     = tag32be("TZO "),
	Lang_Tag_UDMURT                      = tag32be("UDM "),
	Lang_Tag_UKRAINIAN                   = tag32be("UKR "),
	Lang_Tag_UMBUNDU                     = tag32be("UMB "),
	Lang_Tag_URDU                        = tag32be("URD "),
	Lang_Tag_UPPER_SORBIAN               = tag32be("USB "),
	Lang_Tag_UYGHUR                      = tag32be("UYG "),
	Lang_Tag_UZBEK                       = tag32be("UZB "),
	Lang_Tag_VENETIAN                    = tag32be("VEC "),
	Lang_Tag_VENDA                       = tag32be("VEN "),
	Lang_Tag_VIETNAMESE                  = tag32be("VIT "),
	Lang_Tag_VOLAPUK                     = tag32be("VOL "),
	Lang_Tag_VORO                        = tag32be("VRO "),
	Lang_Tag_WA                          = tag32be("WA	 "),
	Lang_Tag_WAGDI                       = tag32be("WAG "),
	Lang_Tag_WARAY_WARAY                 = tag32be("WAR "),
	Lang_Tag_WEST_CREE                   = tag32be("WCR "),
	Lang_Tag_WELSH                       = tag32be("WEL "),
	Lang_Tag_WALLOON                     = tag32be("WLN "),
	Lang_Tag_WOLOF                       = tag32be("WLF "),
	Lang_Tag_MEWATI                      = tag32be("WTM "),
	Lang_Tag_LU                          = tag32be("XBD "),
	Lang_Tag_KHENGKHA                    = tag32be("XKF "),
	Lang_Tag_XHOSA                       = tag32be("XHS "),
	Lang_Tag_MINJANGBAL                  = tag32be("XJB "),
	Lang_Tag_SOGA                        = tag32be("XOG "),
	Lang_Tag_KPELLE_LIBERIA              = tag32be("XPE "),
	Lang_Tag_SAKHA                       = tag32be("YAK "),
	Lang_Tag_YAO                         = tag32be("YAO "),
	Lang_Tag_YAPESE                      = tag32be("YAP "),
	Lang_Tag_YORUBA                      = tag32be("YBA "),
	Lang_Tag_Y_CREE                      = tag32be("YCR "),
	Lang_Tag_YI_CLASSIC                  = tag32be("YIC "),
	Lang_Tag_YI_MODERN                   = tag32be("YIM "),
	Lang_Tag_ZEALANDIC                   = tag32be("ZEA "),
	Lang_Tag_STANDARD_MOROCCAN_TAMAZIGHT = tag32be("ZGH "),
	Lang_Tag_ZHUANG                      = tag32be("ZHA "),
	Lang_Tag_CHINESE_HONG_KONG_SAR       = tag32be("ZHH "),
	Lang_Tag_CHINESE_PHONETIC            = tag32be("ZHP "),
	Lang_Tag_CHINESE_SIMPLIFIED          = tag32be("ZHS "),
	Lang_Tag_CHINESE_TRADITIONAL         = tag32be("ZHT "),
	Lang_Tag_ZANDE                       = tag32be("ZND "),
	Lang_Tag_ZULU                        = tag32be("ZUL "),
	Lang_Tag_ZAZAKI                      = tag32be("ZZA "),
};

enum Font_Gpos : u16 {
	Font_Gpos_INVALID,
	Font_Gpos_SINGLE_ADJUSTMENT,
	Font_Gpos_PAIR_ADJUSTMENT,
	Font_Gpos_CURSIVE_ATTACHMENT,
	Font_Gpos_MARKTOBASE_ATTACHMENT,
	Font_Gpos_MARKTOLIGATURE_ATTACHMENT,
	Font_Gpos_MARKTOMARK_ATTACHMENT,
	Font_Gpos_CONTEXT_POSITIONING,
	Font_Gpos_CHAINED_CONTEXT_POSITIONING,
	Font_Gpos_EXTENSION_POSITIONING,

	Font_Gpos_COUNT
};

enum Font_Gsub : u16 {
	Font_Gsub_INVALID,
	Font_Gsub_SINGLE,
	Font_Gsub_MULTIPLE,
	Font_Gsub_ALTERNATE,
	Font_Gsub_LIGATURE,
	Font_Gsub_CONTEXTUAL,
	Font_Gsub_CHAINING_CONTEXTUAL,
	Font_Gsub_EXTENSION,
	Font_Gsub_REVERSE_CHAINING_CONTEXTUAL_SINGLE,

	Font_Gsub_COUNT,
};

struct Feature_Indices {
	u16  count;
	u16 *ptr;
};

struct Feature_List {
	u8 *            ptr;
	u16             count;
	u8 *            records;
	Feature_Indices indices;
};

struct Lookup_List {
	u8 *ptr;
	u16 count;
	u8 *lookups;
};

struct Dynamic_Font {
	stbtt_fontinfo info;

	void *glyph_class_defination;
	void *mark_attach_class_defination;
	void *mark_glyph_set;

	Feature_List gsub_feature;
	Lookup_List  gsub_lookup;
};

bool font_create(const String content, int index, Dynamic_Font *font) {
	*font = {};

	if (stbtt_InitFont(&font->info, content.data, stbtt_GetFontOffsetForIndex(content.data, index))) {
		// GDEF Information Caching
		{
			u16 *gdef  = (u16 *)(font->info.data + font->info.gdef);
			u16  major = bswap16p_be(gdef + 0);
			u16  minor = bswap16p_be(gdef + 1);
			assert(major == 1 && (minor == 0 || minor == 2 || minor == 3));

			u16 glyph_class_defination_offset       = bswap16p_be(gdef + 2);
			u16 mark_attach_class_defination_offset = bswap16p_be(gdef + 5);
			u16 mark_glyph_set_offset               = 0;
			if (minor > 0) mark_glyph_set_offset = bswap16p_be(gdef + 6);

			u8 *ptr = (u8 *)gdef;
			if (glyph_class_defination_offset)
				font->glyph_class_defination = ptr + glyph_class_defination_offset;
			if (mark_attach_class_defination_offset)
				font->mark_attach_class_defination = ptr + mark_attach_class_defination_offset;
			if (mark_glyph_set_offset)
				font->mark_glyph_set = ptr + mark_glyph_set_offset;
		}

		// GSUB Information Caching
		{
			u16 *gsub = (u16 *)(font->info.data + font->info.gsub);

			u16 major = bswap16p_be(gsub + 0);
			u16 minor = bswap16p_be(gsub + 1);

			// v1.0
			u16 script_list_offset  = bswap16p_be(gsub + 2);
			u16 feature_list_offset = bswap16p_be(gsub + 3);
			u16 lookup_list_offset  = bswap16p_be(gsub + 4);

			assert(major == 1);
			assert(minor == 0 || minor == 1);
			assert(script_list_offset && feature_list_offset && lookup_list_offset);

			u8 *ptr          = (u8 *)gsub;
			u8 *script_list  = ptr + script_list_offset;
			u8 *feature_list = ptr + feature_list_offset;
			u8 *lookup_list  = ptr + lookup_list_offset;

			u16 script_offset         = 0;
			u16 default_script_offset = 0;

			// TODO : binary search?
			u16  script_count = bswap16p_be(script_list);
			u16 *script       = (u16 *)(script_list + 2);
			for (u16 i = 0; i < script_count; ++i) {
				auto tag    = *((u32 *)script);
				auto offset = bswap16p_be(script + 2);
				script += 3;

				// TODO: Script switching

				// TODO: May be we always need 'DFLT' ?
				// Also the spec *seems* to suggest that there may not be 'DFLT' script record
				if (tag == Script_Tag_DEVANAGARI2) {
					script_offset = offset;
					break;
				}
				if (tag == Script_Tag_DEFAULT) default_script_offset = offset;
			}

			// TODO: Default vs Required Script
			assert(script_offset);

			// TODO: Proper Language Selection
			u16 *script_table = (u16 *)(script_list + script_offset);

			u16 default_lang_sys_offset = bswap16p_be(script_table + 0);
			u16 lang_sys_count          = bswap16p_be(script_table + 1);
			u16 lang_sys_offset         = 0;

			u16 *lang_sys_record = script_table + 2;
			for (u16 i = 0; i < lang_sys_count; ++i) {
				auto tag    = *((u32 *)lang_sys_record);
				auto offset = bswap16p_be(lang_sys_record + 2);
				lang_sys_record += 3;

				if (tag == Lang_Tag_SANSKRIT) {
					lang_sys_offset = offset;
					break;
				}
			}

			// TODO: Default Language vs Required Language
			assert(lang_sys_offset);

			u16 *lang_sys = (u16 *)((u8 *)script_table + lang_sys_offset);

			assert(bswap16p_be(lang_sys + 0) == 0);

			u16 req_feature_index   = bswap16p_be(lang_sys + 1);
			u16 feature_index_count = bswap16p_be(lang_sys + 2);

			u16 feature_count   = bswap16p_be(feature_list);
			u8 *feature_records = feature_list + 2;

			u16 lookup_count   = bswap16p_be(lookup_list);
			u8 *lookup_indices = lookup_list + 2;

			font->gsub_feature.indices.count = feature_index_count;
			font->gsub_feature.indices.ptr   = lang_sys + 3;

			font->gsub_feature.ptr     = feature_list;
			font->gsub_feature.count   = feature_count;
			font->gsub_feature.records = feature_records;

			font->gsub_lookup.ptr     = lookup_list;
			font->gsub_lookup.count   = lookup_count;
			font->gsub_lookup.lookups = lookup_indices;
		}

		return true;
	}
	return false;
}

//struct Glyph_Info {
//	u32 id;
//	int cluster;
//	u32 codepoints[10];
//	int codepoint_count = 0;
//};

constexpr u32 UNICODE_ZWNJ          = 0x200C;
constexpr u32 UNICODE_ZWJ           = 0x200D;
constexpr u32 UNICODE_ANUDATTA      = 0x0952;
constexpr u32 UNICODE_DEVANAGARI_RA = 0x0930;
constexpr u32 UNICODE_NBSP          = 0x00A0;

enum Shape_Class {
	Shape_Class_INVALID,
	Shape_Class_BASE,
	Shape_Class_CGJ,
	Shape_Class_CONS_MOD,
	Shape_Class_CONS_WITH_STACKER,
	Shape_Class_CONS_FINAL,
	Shape_Class_CONS_FINAL_MOD,
	Shape_Class_BASE_OTHER,
	Shape_Class_HALANT,
	Shape_Class_HALANT_NUM,
	Shape_Class_BASE_IND,
	Shape_Class_CONS_MED,
	Shape_Class_BASE_NUM,
	Shape_Class_OTHER,
	Shape_Class_REPHA,
	Shape_Class_SYM,
	Shape_Class_SYM_MOD,
	Shape_Class_CONS_SUB,
	Shape_Class_VOWEL,
	Shape_Class_VOWEL_MOD,
	Shape_Class_VARIATION_SELECTOR,
	Shape_Class_WORD_JOINER,
	Shape_Class_ZERO_WIDTH_JOINER,
	Shape_Class_ZERO_WIDTH_NON_JOINER,

	Shape_Class_COUNT
};

enum Shape_Subclass {
	Shape_Subclass_NONE,
	Shape_Subclass_CONS_MOD_ABOVE,
	Shape_Subclass_CONS_MOD_BELOW,
	Shape_Subclass_CONS_FINAL_ABOVE,
	Shape_Subclass_CONS_FINAL_BELOW,
	Shape_Subclass_CONS_FINAL_POST,
	Shape_Subclass_CONS_MED_ABOVE,
	Shape_Subclass_CONS_MED_BELOW,
	Shape_Subclass_CONS_MED_PRE,
	Shape_Subclass_CONS_MED_POST,
	Shape_Subclass_SYM_MOD_ABOVE,
	Shape_Subclass_SYM_MOD_BELOW,
	Shape_Subclass_VOWEL_ABOVE,
	Shape_Subclass_VOWEL_ABOVE_BELOW,
	Shape_Subclass_VOWEL_ABOVE_BELOW_POST,
	Shape_Subclass_VOWEL_ABOVE_POST,
	Shape_Subclass_VOWEL_BELOW,
	Shape_Subclass_VOWEL_BELOW_POST,
	Shape_Subclass_VOWEL_PRE,
	Shape_Subclass_VOWEL_POST,
	Shape_Subclass_VOWEL_PRE_ABOVE,
	Shape_Subclass_VOWEL_PRE_ABOVE_POST,
	Shape_Subclass_VOWEL_PRE_POST,
	Shape_Subclass_VOWEL_MOD_ABOVE,
	Shape_Subclass_VOWEL_MOD_BELOW,
	Shape_Subclass_VOWEL_MOD_PRE,
	Shape_Subclass_VOWEL_MOD_POST,

	Shape_Subclass_COUNT
};

struct Glyph_Cluster {
	utf32                  codepoint;
	u32                    glyph_id;
	Unicode_Indic_Syllable syllable;
	Unicode_Indic_Position position;
	Shape_Class            shape_class;
	Shape_Subclass         shape_subclass;
	bool                   base;
	int                    value;
};

struct Font_Shape {
	Array_View<Glyph_Cluster> codepoints;
	Array_View<Glyph_Cluster> originals;
};

struct Glyph_Range {
	s64                  index;
	Array<Glyph_Cluster> clusters;
	//Glyph_Info *infos   = 0;
	//s32         count   = 0;
	//s32         current = 0;
	//s32         max     = 0;
	//s32         skip    = 0;
};

#if 0
Glyph_Range glyph_range(u32 count) {
	Glyph_Range range;

	if (count) {
		range.max   = count;
		range.infos = new Glyph_Info[count];
	}

	return range;
}

void glyph_range_clear(Glyph_Range *range) {
	range->count = 0;
}

void glyph_range_add_no_bound_check(Glyph_Range *range, u32 id, u32 codepoint, int cluster) {
	assert(range->count < range->max);
	range->infos[range->count].id              = id;
	range->infos[range->count].codepoints[0]   = codepoint;
	range->infos[range->count].codepoint_count = 1;
	range->infos[range->count].cluster         = cluster;
	range->count += 1;
}

void glyph_range_reallocate(Glyph_Range *range, u32 count) {
	range->max   = count;
	range->infos = (Glyph_Info *)mreallocate(range->infos, range->max * sizeof(Glyph_Info));
	if (range->count > range->max) range->count = range->max;
}

void glyph_range_add(Glyph_Range *range, u32 id, u32 codepoint, int cluster) {
	if (range->count == range->max) {
		glyph_range_reallocate(range, (u32)array_get_grow_capacity(range->max, 1));
	}
	glyph_range_add_no_bound_check(range, id, codepoint, cluster);
}

u32 glyph_range_current(Glyph_Range *range) {
	return range->infos[range->current].id;
}

void glyph_range_next(Glyph_Range *range) {
	range->current += range->skip;
	range->skip = 1;
}

bool glyph_range_remains(Glyph_Range *range) {
	return range->current < range->count;
}

void glyph_range_remove_from_next(Glyph_Range *range, u32 count) {
	assert(range->current + (s32)count < range->count);
	u32 remove_count = range->count - range->current - 1 - count;
	memmove(range->infos + range->current + 1, range->infos + range->current + 1 + count, remove_count * sizeof(Glyph_Info));
	range->count -= count;
}

void glyph_range_shift_from_next(Glyph_Range *range, u32 count) {
	if (range->count + (s32)count >= range->max)
		glyph_range_reallocate(range, (u32)array_get_grow_capacity(range->max, count));
	u32 shift_count = range->count - range->current - 1;
	memmove(range->infos + range->current + 1 + count, range->infos + range->current + 1, shift_count * sizeof(Glyph_Info));
}

void glyph_range_free(Glyph_Range *range) {
	mfree(range->infos);
}
#endif

int font_find_coverage_index(void *coverage, u16 glyph_id) {
	u8 *ptr             = (u8 *)coverage;
	int index           = -1;
	u16 coverage_format = bswap16p_be(ptr);
	ptr += 2;

	// TODO: Perform binary search for finding coverage index

	if (coverage_format == 1) {
		u16  glyph_count = bswap16p_be(ptr);
		u16  id          = 0;
		u16 *glyph_ids   = (u16 *)(ptr + 2);
		for (u16 glyph_id_index = 0; glyph_id_index < glyph_count; ++glyph_id_index) {
			id = bswap16p_be(glyph_ids + glyph_id_index);
			if (glyph_id == id) {
				index = (int)glyph_id_index;
				break;
			}
		}
	} else if (coverage_format == 2) {
		u16  range_count    = bswap16p_be(ptr);
		u16  start_glyph_id = 0, end_glyph_id = 0, start_ptr_id = 0;
		u16 *range = (u16 *)(ptr + 2);
		for (u16 range_index = 0; range_index < range_count; ++range_index) {
			start_glyph_id = bswap16p_be(range + 0);
			end_glyph_id   = bswap16p_be(range + 1);
			start_ptr_id   = bswap16p_be(range + 2);
			range += 3;

			if (glyph_id >= start_glyph_id && glyph_id < end_glyph_id) {
				index = (int)(start_ptr_id + glyph_id - start_glyph_id);
				break;
			}
		}
	} else {
		invalid_code_path();
	}

	return index;
}

u16 font_find_glyph_class(void *glyph_class_defination, u16 glyph_id) {
	u16 class_format = bswap16p_be(glyph_class_defination);
	u8 *ptr          = (u8 *)glyph_class_defination + 2;

	if (class_format == 1) {
		u16  start_glyph_id    = bswap16p_be(ptr);
		u16  glyph_count       = bswap16p_be(ptr + 2);
		u16 *class_value_array = (u16 *)(ptr + 4);
		if (glyph_id >= start_glyph_id && glyph_id < start_glyph_id + glyph_count) {
			return bswap16p_be(class_value_array + glyph_id - glyph_count);
		}
	} else if (class_format == 2) {
		u16  class_range_count = bswap16p_be(ptr);
		u16  start_glyph_id = 0, end_glyph_id = 0, class_id = 0;
		u16 *class_range_record = (u16 *)(ptr + 2);
		for (u16 i = 0; i < class_range_count; ++i) {
			start_glyph_id = bswap16p_be(class_range_record + 0);
			end_glyph_id   = bswap16p_be(class_range_record + 1);
			class_id       = bswap16p_be(class_range_record + 2);
			if (glyph_id >= start_glyph_id && glyph_id < end_glyph_id) {
				return class_id;
			}
			class_range_record += 3;
		}
	}

	return 0;
}

enum Glyph_Type {
	Glyph_Type_UNKNOWN,
	Glyph_Type_BASE,
	Glyph_Type_LIGATURE,
	Glyph_Type_MARK,
	Glyph_Type_COMPONENT
};

Glyph_Type font_get_glyph_type(Dynamic_Font *font, u32 glyph_id) {
	auto glyph_class = font_find_glyph_class(font->glyph_class_defination, glyph_id);
	assert(glyph_class <= Glyph_Type_COMPONENT);
	return (Glyph_Type)glyph_class;
}

void font_apply_gsub_lookup(Dynamic_Font *font, void *lookup_ptr, Glyph_Range *range);

// Single Substitution
void font_apply_gsub_subtable_type1(void *subtable, Glyph_Range *range) {
	u16 *ptr             = (u16 *)subtable;
	u16  subformat       = bswap16p_be(ptr);
	u16  coverage_offset = bswap16p_be(ptr + 1);

	if (subformat == 1) {
		u16 unsigned_delta_glyph_id = bswap16p_be(ptr + 2);
		s16 delta_glyph_id          = *((s16 *)&unsigned_delta_glyph_id);

		auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
		if (coverage_index != -1) {
			range->clusters[range->index].glyph_id += delta_glyph_id;
		}
	} else if (subformat == 2) {
		auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
		if (coverage_index != -1) {
			u16  glyph_count          = bswap16p_be(ptr + 2);
			u16 *substitute_glyph_ids = ptr + 3;
			assert(coverage_index < glyph_count);
			range->clusters[range->index].glyph_id = bswap16p_be(substitute_glyph_ids + coverage_index);
		}
	} else {
		invalid_code_path();
	}
}

// Multiple Substitution
void font_apply_gsub_subtable_type2(void *subtable, Glyph_Range *range) {
	u16 *ptr             = (u16 *)subtable;
	u16  subformat       = bswap16p_be(ptr);
	u16  coverage_offset = bswap16p_be(ptr + 1);

	assert(subformat == 1);
	auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
	if (coverage_index != -1) {
		u16  sequence_count  = bswap16p_be(ptr + 2);
		u16 *sequence_offset = ptr + 3;
		u16 *sequence_table  = (u16 *)((u8 *)subtable + bswap16p_be(sequence_offset + coverage_index));
		u16  glyph_count     = bswap16p_be(sequence_table);

		sequence_table += 1;

		array_resize(&range->clusters, range->clusters.count + glyph_count - 1);
		u32 shift_count = (u32)range->clusters.count - (u32)range->index - 1;
		memmove(range->clusters.data + range->index + glyph_count, range->clusters.data + range->index + 1, shift_count * sizeof(Glyph_Cluster));

		for (u16 i = 0; i < glyph_count; ++i) {
			range->clusters[range->index + i]          = range->clusters[range->index];
			range->clusters[range->index + i].glyph_id = bswap16p_be(sequence_table + i);
		}
	}
}

// Alternate Substitution
void font_apply_gsub_subtable_type3(void *subtable, Glyph_Range *range, u16 alternate_index) {
	u16 *ptr                 = (u16 *)subtable;
	u16  subformat           = bswap16p_be(ptr);
	u16  coverage_offset     = bswap16p_be(ptr + 1);
	u16  alternate_set_count = bswap16p_be(ptr + 2);
	assert(subformat == 1);

	auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
	if (coverage_index != -1) {
		u16 *alternate_table = ptr + 3 + coverage_index;
		u16  glyph_count     = bswap16p_be(alternate_table);
		if (alternate_index >= glyph_count) alternate_index = 0;
		range->clusters[range->index].glyph_id = bswap16p_be(alternate_table + 1 + alternate_index);
	}
}

// Ligatures Substitution
void font_apply_gsub_subtable_type4(void *subtable, Glyph_Range *range) {
	u16 *ptr                  = (u16 *)subtable;
	u16  subformat            = bswap16p_be(ptr);
	u16  coverage_offset      = bswap16p_be(ptr + 1);
	u16  ligature_set_count   = bswap16p_be(ptr + 2);
	u16 *ligature_set_offsets = ptr + 3;

	assert(subformat == 1);

	int coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
	if (coverage_index != -1) {
		u16  ligature_set_offset = bswap16p_be(ligature_set_offsets + coverage_index);
		u8 * ligature_set        = (u8 *)subtable + ligature_set_offset;
		u16  ligature_count      = bswap16p_be(ligature_set);
		u16 *ligature_offsets    = (u16 *)(ligature_set + 2);

		for (u16 ligature_index = 0; ligature_index < ligature_count; ++ligature_index) {
			u16 *ligature            = (u16 *)(ligature_set + bswap16p_be(ligature_offsets + ligature_index));
			u16  ligature_glyph      = bswap16p_be(ligature);
			u16  component_count     = bswap16p_be(ligature + 1);
			u16 *component_glyph_ids = ligature + 2;

			if (component_count > range->clusters.count - range->index) continue;

			bool found = true;
			for (u16 component_index = 0; component_index < component_count - 1; ++component_index) {
				u16  glyph_id   = bswap16p_be(component_glyph_ids + component_index);
				auto look_index = range->index + component_index + 1;
				if (glyph_id != range->clusters[look_index].glyph_id || range->clusters[range->index].value != range->clusters[look_index].value) {
					found = false;
					break;
				}
			}

			if (found) {
#if 0
				switch (component_count) {
					case 2: {
						auto sa = range->clusters[range->index].syllable;
						auto sb = range->clusters[range->index + 1].syllable;
						if (sa == Unicode_Indic_Syllable_CONSONANT && sb == Unicode_Indic_Syllable_VIRAMA) {
							if (range->index + 2 < range->clusters.count 
								&& range->clusters[range->index + 2].value == range->clusters[range->index].value) {
								if (range->clusters[range->index].codepoint == UNICODE_DEVANAGARI_RA)
									range->clusters[range->index].syllable = Unicode_Indic_Syllable_CONSONANT_PRECEDING_REPHA;
								else
									range->clusters[range->index].syllable = Unicode_Indic_Syllable_CONSONANT_DEAD;
							} else {
								return;
							}
						}
					} break;
				}
#endif
				bool is_base = false;
				for (u32 comp_id = 0; comp_id < component_count; ++comp_id) {
					if (range->clusters[range->index + comp_id].base) {
						is_base = true;
						break;
					}
				}
				if (is_base) {
					range->clusters[range->index].base = true;
				}

				range->clusters[range->index].glyph_id = ligature_glyph;
				u32 move_count                         = (u32)range->clusters.count - (u32)range->index - component_count;
				memmove(range->clusters.data + range->index + 1, range->clusters.data + range->index + component_count, move_count * sizeof(Glyph_Cluster));
				range->clusters.count -= (component_count - 1);
				assert(range->clusters.count >= 0);

				break;
			}
		}
	}
}

// Substitution Lookup Record
void font_appply_gsub_substitution_lookup_record(Dynamic_Font *font, void *subst_lookup_record_ptr, u16 subst_count, Glyph_Range *range) {
	u16 *subst_lookup_record = (u16 *)subst_lookup_record_ptr;

	for (u16 subst_lookup_record_index = 0; subst_lookup_record_index < subst_count; ++subst_lookup_record_index) {
		u16 glyph_sequence_index = bswap16p_be(subst_lookup_record);
		u16 lookup_list_index    = bswap16p_be(subst_lookup_record + 1);
		subst_lookup_record += 2;
		assert(range->index + glyph_sequence_index < range->clusters.count);

		auto save_current_index = range->index;
		range->index += glyph_sequence_index;
		u16 lookup_offset = bswap16p_be((u16 *)font->gsub_lookup.lookups + lookup_list_index);
		font_apply_gsub_lookup(font, font->gsub_lookup.ptr + lookup_offset, range);
		range->index = save_current_index;
	}
}

// Contextual Substitution
void font_apply_gsub_subtable_type5(Dynamic_Font *font, void *subtable, Glyph_Range *range) {
	u16 *ptr             = (u16 *)subtable;
	u16  subformat       = bswap16p_be(ptr + 0);
	u16  coverage_offset = bswap16p_be(ptr + 1);

	if (subformat == 1) {
		auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
		if (coverage_index != -1) {
			u16  subrule_set_offset_count = bswap16p_be(ptr + 2);
			u16 *subrule_set_offsets      = ptr + 3;

			assert(coverage_index < subrule_set_offset_count);

			u16  subrule_set_offset   = bswap16p_be(subrule_set_offsets + coverage_index);
			u16 *subrule_set          = (u16 *)((u8 *)subtable + subrule_set_offset);
			u16  subrule_offset_count = bswap16p_be(subrule_set);
			u16 *subrule_offsets      = subrule_set + 1;
			u8 * subrule_set_ptr      = (u8 *)subrule_set;

			for (u16 subrule_index = 0; subrule_index < subrule_offset_count; ++subrule_index) {
				u16  subrule_offset = bswap16p_be(subrule_offsets + subrule_index);
				u16 *subrule        = (u16 *)(subrule_set_ptr + subrule_offset);
				u16  glyph_count    = bswap16p_be(subrule);

				if (glyph_count > range->clusters.count - range->index) continue;

				bool found = true;
				for (u16 glyph_index = 0; glyph_index < glyph_count - 1; glyph_index += 1) {
					u16  input_glyph_id = bswap16p_be(subrule + 2 + glyph_index);
					auto look_index     = range->index + glyph_index + 1;
					if (input_glyph_id != range->clusters[look_index].glyph_id || range->clusters[range->index].value != range->clusters[look_index].value) {
						found = false;
						break;
					}
				}

				if (found) {
					u16  subst_count         = bswap16p_be(subrule + 1);
					u16 *subst_lookup_record = subrule + 2 + glyph_count - 1;
					font_appply_gsub_substitution_lookup_record(font, subst_lookup_record, subst_count, range);
					break;
				}
			}
		}
	} else if (subformat == 2) {
		auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
		if (coverage_index != -1) {
			u16  class_defination_offset   = bswap16p_be(ptr + 2);
			u16  subclass_set_offset_count = bswap16p_be(ptr + 3);
			u16 *subclass_set_offsets      = ptr + 4;
			u8 * class_defination          = (u8 *)subtable + class_defination_offset;

			auto class_id = font_find_glyph_class(class_defination, range->clusters[range->index].glyph_id);
			assert(class_id < subclass_set_offset_count);

			u16 subclass_set_offset = bswap16p_be(subclass_set_offsets + class_id);
			if (subclass_set_offset) {
				u8 * subclass_set          = ((u8 *)subtable + subclass_set_offset);
				u16  subclass_rule_count   = bswap16p_be(subclass_set);
				u16 *subclass_rule_offsets = (u16 *)(subclass_set + 2);

				for (u16 subclass_rule_index = 0; subclass_rule_index < subclass_rule_count; ++subclass_rule_index) {
					u16  subclass_rule_offset = bswap16p_be(subclass_rule_offsets + subclass_rule_index);
					u16 *subclass_rule        = (u16 *)(subclass_set + subclass_rule_offset);
					u16  glyph_count          = bswap16p_be(subclass_rule);

					if (glyph_count > range->clusters.count - range->index) continue;

					bool found = true;
					for (u16 glyph_index = 0; glyph_index < glyph_count - 1; glyph_index += 1) {
						u16  input_class_id = bswap16p_be(subclass_rule + 2 + glyph_index);
						auto look_index     = range->index + glyph_index + 1;
						if (input_class_id != font_find_glyph_class(class_defination, range->clusters[look_index].glyph_id) || range->clusters[range->index].value != range->clusters[look_index].value) {
							found = false;
							break;
						}
					}

					if (found) {
						u16  subst_count         = bswap16p_be(subclass_rule + 1);
						u16 *subst_lookup_record = subclass_rule + 2 + glyph_count - 1;
						font_appply_gsub_substitution_lookup_record(font, subst_lookup_record, subst_count, range);
						break;
					}
				}
			}
		}
	} else if (subformat == 3) {
		u16 glyph_count = coverage_offset; // second member is glyph_count for subformat 3
		if (range->index + glyph_count - 1 < range->clusters.count) {
			u16 *coverage_offsets = ptr + 3;

			bool found = true;
			for (u16 glyph_index = 0; glyph_index < glyph_count - 1; glyph_index += 1) {
				u16  coverage_offset = bswap16p_be(coverage_offsets + glyph_index);
				auto look_index      = range->index + glyph_index;
				auto coverage_index  = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[look_index].glyph_id || range->clusters[range->index].value != range->clusters[look_index].value);
				if (coverage_index != -1) {
					found = false;
					break;
				}
			}

			if (found) {
				u16  subst_count         = bswap16p_be(ptr + 2);
				u16 *subst_lookup_record = coverage_offsets + glyph_count;
				font_appply_gsub_substitution_lookup_record(font, subst_lookup_record, subst_count, range);
			}
		}

	} else {
		invalid_code_path();
	}
}

// Chaining Contextual Substitution
void font_apply_gsub_subtable_type6(Dynamic_Font *font, void *subtable, Glyph_Range *range) {
	u16 *ptr             = (u16 *)subtable;
	u16  subformat       = bswap16p_be(ptr + 0);
	u16  coverage_offset = bswap16p_be(ptr + 1);

	if (subformat == 1) {
		auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
		if (coverage_index != -1) {
			u16  chain_subrule_set_count   = bswap16p_be(ptr + 2);
			u16 *chain_subrule_set_offsets = ptr + 3;

			assert(coverage_index < chain_subrule_set_count);

			u16  chain_subrule_set_offset = bswap16p_be(chain_subrule_set_offsets + coverage_index);
			u16 *chain_subrule_set        = (u16 *)((u8 *)subtable + chain_subrule_set_offset);

			u16  chain_subrule_count   = bswap16p_be(chain_subrule_set);
			u16 *chain_subrule_offsets = chain_subrule_set + 1;
			u8 * chain_subrule_set_ptr = (u8 *)chain_subrule_set;

			for (u16 chain_subrule_index = 0; chain_subrule_index < chain_subrule_count; ++chain_subrule_index) {
				u16  chain_subrule_offset = bswap16p_be(chain_subrule_offsets + chain_subrule_index);
				u16 *chain_subrule        = (u16 *)(chain_subrule_set_ptr + chain_subrule_offset);

				u16 backtrack_glyph_count = bswap16p_be(chain_subrule);
				if (range->index - backtrack_glyph_count < 0) continue;
				u16 *backtrack_glyphs = chain_subrule + 1;

				u16  input_glyph_count     = bswap16p_be(backtrack_glyphs + backtrack_glyph_count);
				u16 *input_glyphs          = backtrack_glyphs + backtrack_glyph_count + 1;
				u16  lookahead_glyph_count = bswap16p_be(input_glyphs + input_glyph_count - 1);
				u16 *lookahead_glyphs      = input_glyphs + input_glyph_count - 1 + 1;

				u16 required_glyph_count = input_glyph_count + lookahead_glyph_count - 1;
				if (range->index + required_glyph_count >= range->clusters.count) continue;

				bool found = true;

				for (u16 glyph_index = 0; glyph_index < backtrack_glyph_count; ++glyph_index) {
					auto glyph_id   = bswap16p_be(backtrack_glyphs + glyph_index);
					auto look_index = range->index - 1 - glyph_index;
					if (range->clusters[look_index].glyph_id != glyph_id || range->clusters[range->index].value != range->clusters[look_index].value) {
						found = false;
						break;
					}
				}

				if (found) {
					for (u16 glyph_index = 0; glyph_index < input_glyph_count; ++glyph_index) {
						auto glyph_id   = bswap16p_be(input_glyphs + glyph_index);
						auto look_index = range->index + glyph_index + 1;
						if (glyph_id != range->clusters[look_index].glyph_id || range->clusters[range->index].value != range->clusters[look_index].value) {
							found = false;
							break;
						}
					}
				}

				if (found) {
					for (u16 glyph_index = 0; glyph_index < lookahead_glyph_count; ++glyph_index) {
						auto glyph_id   = bswap16p_be(lookahead_glyphs + glyph_index);
						auto look_index = range->index + glyph_index + input_glyph_count;
						if (glyph_id != range->clusters[look_index].glyph_id || range->clusters[range->index].value != range->clusters[look_index].value) {
							found = false;
							break;
						}
					}
				}

				if (found) {
					u16  subst_count         = bswap16p_be(lookahead_glyphs + lookahead_glyph_count);
					u16 *subst_lookup_record = lookahead_glyphs + lookahead_glyph_count + 1;
					font_appply_gsub_substitution_lookup_record(font, subst_lookup_record, subst_count, range);
					break;
				}
			}
		}
	} else if (subformat == 2) {
		auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
		if (coverage_index != -1) {
			u16  backtrack_class_defination_offset = bswap16p_be(ptr + 2);
			u16  input_class_defination_offset     = bswap16p_be(ptr + 3);
			u16  lookahead_class_defination_offset = bswap16p_be(ptr + 4);
			u16  chain_subclass_set_count          = bswap16p_be(ptr + 5);
			u16 *chain_subclass_set_offsets        = ptr + 6;

			u8 * backtrack_class_defination = (u8 *)subtable + backtrack_class_defination_offset;
			u8 * input_class_defination     = (u8 *)subtable + input_class_defination_offset;
			u8 * lookahead_class_defination = (u8 *)subtable + lookahead_class_defination_offset;
			auto class_id                   = font_find_glyph_class(input_class_defination, range->clusters[range->index].glyph_id);
			assert(class_id < chain_subclass_set_count);

			u16 chain_subclass_set_offset = bswap16p_be(chain_subclass_set_offsets + class_id);
			if (chain_subclass_set_offset) {
				u8 * chain_subclass_set          = ((u8 *)subtable + chain_subclass_set_offset);
				u16  chain_subclass_rule_count   = bswap16p_be(chain_subclass_set);
				u16 *chain_subclass_rule_offsets = (u16 *)(chain_subclass_set + 2);

				for (u16 chain_subclass_rule_index = 0;
					 chain_subclass_rule_index < chain_subclass_rule_count;
					 ++chain_subclass_rule_index) {
					u16  chain_subclass_rule_offset = bswap16p_be(chain_subclass_rule_offsets + chain_subclass_rule_index);
					u16 *chain_subclass_rule        = (u16 *)(chain_subclass_set + chain_subclass_rule_offset);

					u16 backtrack_class_count = bswap16p_be(chain_subclass_rule);
					if (range->index - backtrack_class_count < 0) continue;
					u16 *backtrack_classes = chain_subclass_rule + 1;

					u16  input_class_count     = bswap16p_be(backtrack_classes + backtrack_class_count);
					u16 *input_classes         = backtrack_classes + backtrack_class_count + 1;
					u16  lookahead_class_count = bswap16p_be(input_classes + input_class_count - 1);
					u16 *lookahead_classes     = input_classes + input_class_count - 1 + 1;

					u16 required_class_count = input_class_count + lookahead_class_count - 1;
					if (range->index + required_class_count >= range->clusters.count) continue;

					bool found = true;

					for (u16 class_index = 0; class_index < backtrack_class_count; ++class_index) {
						auto class_id     = bswap16p_be(backtrack_classes + class_index);
						auto look_index   = range->index - 1 - class_index;
						auto chk_class_id = font_find_glyph_class(backtrack_class_defination, range->clusters[look_index].glyph_id);
						if (chk_class_id != class_id || range->clusters[range->index].value != range->clusters[look_index].value) {
							found = false;
							break;
						}
					}

					if (found) {
						for (u16 class_index = 0; class_index < input_class_count; ++class_index) {
							auto class_id     = bswap16p_be(input_classes + class_index);
							auto look_index   = range->index + class_index + 1;
							auto chk_class_id = font_find_glyph_class(input_class_defination, range->clusters[look_index].glyph_id);
							if (class_id != chk_class_id || range->clusters[range->index].value != range->clusters[look_index].value) {
								found = false;
								break;
							}
						}
					}

					if (found) {
						for (u16 class_index = 0; class_index < lookahead_class_count; ++class_index) {
							auto class_id     = bswap16p_be(lookahead_classes + class_index);
							auto look_index   = range->index + class_index + input_class_count;
							auto chk_class_id = font_find_glyph_class(input_class_defination, range->clusters[look_index].glyph_id);
							if (class_id != chk_class_id || range->clusters[range->index].value != range->clusters[look_index].value) {
								found = false;
								break;
							}
						}
					}

					if (found) {
						u16  subst_count         = bswap16p_be(lookahead_classes + lookahead_class_count);
						u16 *subst_lookup_record = lookahead_classes + lookahead_class_count + 1;
						font_appply_gsub_substitution_lookup_record(font, subst_lookup_record, subst_count, range);
						break;
					}
				}
			}
		}
	} else if (subformat == 3) {
		u16 backtrack_glyph_count = coverage_offset; // second member is backtrack_glyph_count for subformat 3
		if (range->index - backtrack_glyph_count < 0) return;
		u16 *backtrack_coverage_offsets = ptr + 2;

		u16  input_glyph_count          = bswap16p_be(backtrack_coverage_offsets + backtrack_glyph_count);
		u16 *input_coverage_offsets     = backtrack_coverage_offsets + backtrack_glyph_count + 1;
		u16  lookahead_glyph_count      = bswap16p_be(input_coverage_offsets + input_glyph_count);
		u16 *lookahead_coverage_offsets = input_coverage_offsets + input_glyph_count + 1;

		u16 required_glyph_count = input_glyph_count + lookahead_glyph_count - 1;
		if (range->index + required_glyph_count >= range->clusters.count) return;

		bool found = true;

		for (u16 glyph_coverage_index = 0; glyph_coverage_index < backtrack_glyph_count; ++glyph_coverage_index) {
			u16  coverage_offset = bswap16p_be(backtrack_coverage_offsets + glyph_coverage_index);
			auto look_index      = range->index - 1 - glyph_coverage_index;
			auto coverage_index  = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[look_index].glyph_id);
			if (coverage_index != -1 || range->clusters[range->index].value != range->clusters[look_index].value) {
				found = false;
				break;
			}
		}

		if (found) {
			for (u16 glyph_coverage_index = 0; glyph_coverage_index < input_glyph_count; ++glyph_coverage_index) {
				u16  coverage_offset = bswap16p_be(input_coverage_offsets + glyph_coverage_index);
				auto look_index      = range->index + glyph_coverage_index;
				auto coverage_index  = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[look_index].glyph_id);
				if (coverage_index != -1 || range->clusters[range->index].value != range->clusters[look_index].value) {
					found = false;
					break;
				}
			}
		}

		if (found) {
			for (u16 glyph_coverage_index = 0; glyph_coverage_index < lookahead_glyph_count; ++glyph_coverage_index) {
				u16  coverage_offset = bswap16p_be(lookahead_coverage_offsets + glyph_coverage_index);
				auto look_index      = range->index + glyph_coverage_index + input_glyph_count;
				auto coverage_index  = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[look_index].glyph_id);
				if (coverage_index != -1 || range->clusters[range->index].value != range->clusters[look_index].value) {
					found = false;
					break;
				}
			}
		}

		if (found) {
			u16  subst_count         = bswap16p_be(lookahead_coverage_offsets + lookahead_glyph_count);
			u16 *subst_lookup_record = lookahead_coverage_offsets + lookahead_glyph_count + 1;
			font_appply_gsub_substitution_lookup_record(font, subst_lookup_record, subst_count, range);
		}

	} else {
		invalid_code_path();
	}
}

//  Reverse Chaining Contextual Single Substitution
void font_apply_gsub_subtable_type8(void *subtable, Glyph_Range *range) {
	u16 *ptr             = (u16 *)subtable;
	u16  subformat       = bswap16p_be(ptr + 0);
	u16  coverage_offset = bswap16p_be(ptr + 1);

	auto coverage_index = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[range->index].glyph_id);
	if (coverage_index != -1) {
		u16  backtrack_glyph_count      = bswap16p_be(ptr + 2);
		u16 *backtrack_coverage_offsets = ptr + 3;
		u16  lookahead_glyph_count      = bswap16p_be(backtrack_coverage_offsets + backtrack_glyph_count);
		u16 *lookahead_coverage_offsets = backtrack_coverage_offsets + backtrack_glyph_count + 1;

		if (range->index - backtrack_glyph_count < 0) return;
		if (range->index + lookahead_glyph_count >= range->clusters.count) return;

		bool found = true;

		for (u16 glyph_coverage_index = 0; glyph_coverage_index < backtrack_glyph_count; ++glyph_coverage_index) {
			u16  coverage_offset = bswap16p_be(backtrack_coverage_offsets + glyph_coverage_index);
			auto look_index      = range->index - 1 - glyph_coverage_index;
			auto coverage_index  = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[look_index].glyph_id);
			if (coverage_index != -1 || range->clusters[range->index].value != range->clusters[look_index].value) {
				found = false;
				break;
			}
		}

		if (found) {
			for (u16 glyph_coverage_index = 0; glyph_coverage_index < lookahead_glyph_count; ++glyph_coverage_index) {
				u16  coverage_offset = bswap16p_be(lookahead_coverage_offsets + glyph_coverage_index);
				auto look_index      = range->index + glyph_coverage_index + 1;
				auto coverage_index  = font_find_coverage_index((u8 *)subtable + coverage_offset, range->clusters[look_index].glyph_id);
				if (coverage_index != -1 || range->clusters[range->index].value != range->clusters[look_index].value) {
					found = false;
					break;
				}
			}
		}

		if (found) {
			u16  glyph_count     = bswap16p_be(lookahead_coverage_offsets + lookahead_glyph_count);
			u16 *subst_glyph_ids = lookahead_coverage_offsets + lookahead_glyph_count + 1;
			assert(coverage_index < glyph_count);
			u16 subst_glyph_id                     = bswap16p_be(subst_glyph_ids + coverage_index);
			range->clusters[range->index].glyph_id = subst_glyph_id;
		}
	}
}

void font_apply_gsub_subtable_type7(Dynamic_Font *font, void *subtable, Glyph_Range *range) {
	u16 *ptr        = (u16 *)subtable;
	u16  subformat  = bswap16p_be(ptr + 0);
	u16  ext_type   = bswap16p_be(ptr + 1);
	u32  ext_offset = bswap16p_be(ptr + 2);

	u8 *ext_subtable = (u8 *)subtable + ext_offset;

	assert(subformat == 1);

	switch (ext_type) {
		case Font_Gsub_SINGLE:
			font_apply_gsub_subtable_type1(ext_subtable, range);
			break;

		case Font_Gsub_MULTIPLE:
			font_apply_gsub_subtable_type2(ext_subtable, range);
			break;

		case Font_Gsub_ALTERNATE:
#if 0
		font_apply_gsub_subtable_type3(ext_subtable, range, 0);
#endif
			break;

		case Font_Gsub_LIGATURE:
			font_apply_gsub_subtable_type4(ext_subtable, range);
			break;

		case Font_Gsub_CONTEXTUAL:
			font_apply_gsub_subtable_type5(font, ext_subtable, range);
			break;

		case Font_Gsub_CHAINING_CONTEXTUAL:
			font_apply_gsub_subtable_type6(font, ext_subtable, range);
			break;

		case Font_Gsub_EXTENSION:
			invalid_code_path();
			break;

		case Font_Gsub_REVERSE_CHAINING_CONTEXTUAL_SINGLE:
			font_apply_gsub_subtable_type8(ext_subtable, range);
			break;

			invalid_default_case();
	}
}

void font_apply_gsub_lookup(Dynamic_Font *font, void *lookup_ptr, Glyph_Range *range) {
	u16 *lookup = (u16 *)lookup_ptr;
	u16  type   = bswap16p_be(lookup + 0);
	u16  flag   = bswap16p_be(lookup + 1);

	u32 glyph_id = range->clusters[range->index].glyph_id;

	Lookup_Flag class_req_flags = 0;
	class_req_flags |= Lookup_Flag_IGNORE_BASE_GLYPH | Lookup_Flag_IGNORE_LIGATURES;
	class_req_flags |= Lookup_Flag_IGNORE_MARKS | Lookup_Flag_USE_MARK_FILTERING_SET;
	class_req_flags |= Lookup_Flag_MARK_ATTACHMENT_TYPE;

	u16 glyph_class;

	if (class_req_flags & flag) {
		assert(font->glyph_class_defination);
		glyph_class = font_find_glyph_class(font->glyph_class_defination, glyph_id);
	} else {
		glyph_class = 0;
	}

	if ((flag & Lookup_Flag_IGNORE_BASE_GLYPH) && glyph_class == 1) return;
	if ((flag & Lookup_Flag_IGNORE_LIGATURES) && glyph_class == 2) return;
	if ((flag & Lookup_Flag_IGNORE_MARKS) && glyph_class == 3) return;

	u16  subtable_count   = bswap16p_be(lookup + 2);
	u16 *subtable_offsets = lookup + 3;

	if (glyph_class == 4) {
		if (flag & Lookup_Flag_USE_MARK_FILTERING_SET) {
			assert(font->mark_glyph_set);
			u16 *ptr = (u16 *)font->mark_glyph_set;

			u16 table_format = bswap16p_be(ptr + 0);
			assert(table_format == 1);
			u16 mark_glyph_set_count = bswap16p_be(ptr + 1);
			u16 mark_filtering_set   = bswap16p_be(lookup + 3 + subtable_count);
			assert(mark_filtering_set < mark_glyph_set_count);
			u32 coverage_offset = bswap32p_be((u32 *)ptr + 1 + mark_filtering_set);
			// TODO: I could not find from where the coverage offset is specified, confirm that this is correct!!!
			if (font_find_coverage_index((u8 *)font->mark_glyph_set + coverage_offset, glyph_id) == -1) return; // skip all unspecified MARK
		} else if (flag & Lookup_Flag_MARK_ATTACHMENT_TYPE) {
			assert(font->mark_attach_class_defination);
			// only process those MARK that are present in the Mark Attachment Class
			if (font_find_glyph_class(font->mark_attach_class_defination, glyph_id) != 3) return;
		}
	}

	u8 *subtable_ptr_start = (u8 *)lookup;

	switch (type) {
		case Font_Gsub_SINGLE:
			for (u16 subtable_index = 0; subtable_index < subtable_count; ++subtable_index)
				font_apply_gsub_subtable_type1(subtable_ptr_start + bswap16p_be(subtable_offsets + subtable_index), range);
			break;

		case Font_Gsub_MULTIPLE:
			for (u16 subtable_index = 0; subtable_index < subtable_count; ++subtable_index)
				font_apply_gsub_subtable_type2(subtable_ptr_start + bswap16p_be(subtable_offsets + subtable_index), range);
			break;

		case Font_Gsub_ALTERNATE:
#if 0
		for (u16 subtable_index = 0; subtable_index < subtable_count; ++subtable_index)
			font_apply_gsub_subtable_type3(subtable_ptr_start + bswap16p_be(subtable_offsets + subtable_index), range, 0);
#endif
			break;

		case Font_Gsub_LIGATURE:
			for (u16 subtable_index = 0; subtable_index < subtable_count; ++subtable_index)
				font_apply_gsub_subtable_type4(subtable_ptr_start + bswap16p_be(subtable_offsets + subtable_index), range);
			break;

		case Font_Gsub_CONTEXTUAL:
			for (u16 subtable_index = 0; subtable_index < subtable_count; ++subtable_index)
				font_apply_gsub_subtable_type5(font, subtable_ptr_start + bswap16p_be(subtable_offsets + subtable_index), range);
			break;

		case Font_Gsub_CHAINING_CONTEXTUAL:
			for (u16 subtable_index = 0; subtable_index < subtable_count; ++subtable_index)
				font_apply_gsub_subtable_type6(font, subtable_ptr_start + bswap16p_be(subtable_offsets + subtable_index), range);
			break;

		case Font_Gsub_EXTENSION:
			for (u16 subtable_index = 0; subtable_index < subtable_count; ++subtable_index)
				font_apply_gsub_subtable_type7(font, subtable_ptr_start + bswap16p_be(subtable_offsets + subtable_index), range);
			break;

		case Font_Gsub_REVERSE_CHAINING_CONTEXTUAL_SINGLE:
			for (u16 subtable_index = 0; subtable_index < subtable_count; ++subtable_index)
				font_apply_gsub_subtable_type8(subtable_ptr_start + bswap16p_be(subtable_offsets + subtable_index), range);
			break;

			invalid_default_case();
	}
}

void font_apply_gsub_feature(Dynamic_Font *font, void *feature, Glyph_Range *range) {
	u16  lookup_list_indices_count = bswap16p_be((u16 *)feature + 1);
	u16 *lookup_list_indices       = (u16 *)feature + 2;

	for (u16 list_counter = 0; list_counter < lookup_list_indices_count; ++list_counter) {
		u16 lookup_list_index = bswap16p_be(lookup_list_indices + list_counter);
		assert(lookup_list_index < font->gsub_lookup.count);
		u16   lookup_offset = bswap16p_be((u16 *)font->gsub_lookup.lookups + lookup_list_index);
		auto *lookup        = font->gsub_lookup.ptr + lookup_offset;
		font_apply_gsub_lookup(font, lookup, range);
	}
}

bool match_next(const Array_View<Glyph_Cluster> &arr, Unicode_Indic_Syllable syllable, int *match) {
	int index = *match;
	if (index < arr.count) {
		if (arr[index].syllable == syllable) {
			*match = index + 1;
			return true;
		}
		return false;
	}
	return false;
}

bool match_next(const Array_View<Glyph_Cluster> &arr, u32 codepoint, int *match) {
	int index = *match;
	if (index < arr.count) {
		if (arr[index].codepoint == codepoint) {
			*match = index + 1;
			return true;
		}
		return false;
	}
	return false;
}

bool match_range(const Array_View<Glyph_Cluster> &arr, u32 codepoint_start, u32 codepoint_end, int *match) {
	int index = *match;
	if (index < arr.count) {
		if (arr[index].codepoint >= codepoint_start && arr[index].codepoint <= codepoint_end) {
			*match = index + 1;
			return true;
		}
		return false;
	}
	return false;
}

Font_Shape font_shape_string(Dynamic_Font *font, String string) {
	//defer { glyph_range_free(&range); };
	Array<Glyph_Cluster> codepoints;

	String_Iter iter;
	while (string_iter_next(string, &iter)) {
		auto codepoint = iter.codepoint.code;

		Glyph_Cluster d;

		u32 *normals       = 0;
		auto normals_count = unicode_normalize(codepoint, &normals);
		if (normals_count) {
			for (int i = 0; i < normals_count; ++i) {
				d.codepoint = normals[i];
				d.value     = 0;
				array_add(&codepoints, d);
			}
		} else {
			d.codepoint = codepoint;
			d.value     = 0;
			array_add(&codepoints, d);
		}
	}

	// Clustering for cursor position
#if 0
	int cluster_value = 0;
	for (s64 i = 0; i < codepoints.count; ++i) {
		bool should_inc;

		if (i == 0 || i == codepoints.count - 1) {
			should_inc = false;
		} else {
			auto p = codepoints[i - 1].prop;
			auto n = codepoints[i].prop;

			if (p == unicode_Grapheme_Property_CR && n == unicode_Grapheme_Property_LF)
				should_inc = false;
			else if (p == unicode_Grapheme_Property_CONTROL || p == unicode_Grapheme_Property_CR || p == unicode_Grapheme_Property_LF)
				should_inc = true;
			else if (n == unicode_Grapheme_Property_CONTROL || n == unicode_Grapheme_Property_CR || n == unicode_Grapheme_Property_LF)
				should_inc = true;
			else if (p == unicode_Grapheme_Property_L &&
					 (n == unicode_Grapheme_Property_L || n == unicode_Grapheme_Property_V || n == unicode_Grapheme_Property_LV || n == unicode_Grapheme_Property_LVT))
				should_inc = false;
			else if ((p == unicode_Grapheme_Property_LV || p == unicode_Grapheme_Property_V) && (n == unicode_Grapheme_Property_V || n == unicode_Grapheme_Property_T))
				should_inc = false;
			else if ((p == unicode_Grapheme_Property_LVT || p == unicode_Grapheme_Property_T) && (n == unicode_Grapheme_Property_T))
				should_inc = false;
			else if (n == unicode_Grapheme_Property_EXTEND || n == unicode_Grapheme_Property_ZWJ)
				should_inc = false;
			else if (n == unicode_Grapheme_Property_SPACINGMARK || p == unicode_Grapheme_Property_PREPEND)
				should_inc = false;
			// TODO: We don't check for Emoji, do we need to support Emojis?
			else if (n == unicode_Grapheme_Property_REGIONAL_INDICATOR && p == unicode_Grapheme_Property_REGIONAL_INDICATOR)
				should_inc = false;
			else
				should_inc = true;
		}

		if (should_inc) cluster_value += 1;

		codepoints[i].value    = cluster_value;
		codepoints[i].syllable = unicode_indic_syllable(codepoints[i].codepoint);
	}
#endif

	for (s64 index = 0; index < codepoints.count; ++index) {
		codepoints[index].syllable = unicode_indic_syllable(codepoints[index].codepoint);
		codepoints[index].position = unicode_indic_position(codepoints[index].codepoint);

		auto syllable  = codepoints[index].syllable;
		auto position  = codepoints[index].position;
		auto category  = unicode_category(codepoints[index].codepoint);
		auto codepoint = codepoints[index].codepoint;

		Shape_Class shape_class = Shape_Class_INVALID;

		if (syllable == Unicode_Indic_Syllable_NUMBER ||
			(syllable == Unicode_Indic_Syllable_AVAGRAHA && category == Unicode_Category_LO) ||
			(syllable == Unicode_Indic_Syllable_BINDU && category == Unicode_Category_LO) ||
			syllable == Unicode_Indic_Syllable_CONSONANT ||
			(syllable == Unicode_Indic_Syllable_CONSONANT_FINAL && category == Unicode_Category_LO) ||
			syllable == Unicode_Indic_Syllable_CONSONANT_HEAD_LETTER ||
			(syllable == Unicode_Indic_Syllable_CONSONANT_MEDIAL && category == Unicode_Category_LO) ||
			(syllable == Unicode_Indic_Syllable_CONSONANT_SUBJOINED && category == Unicode_Category_LO) ||
			syllable == Unicode_Indic_Syllable_TONE_LETTER ||
			(syllable == Unicode_Indic_Syllable_VOWEL && category == Unicode_Category_LO) ||
			syllable == Unicode_Indic_Syllable_VOWEL_INDEPENDENT ||
			(syllable == Unicode_Indic_Syllable_VOWEL_DEPENDENT && category == Unicode_Category_LO)) {
			shape_class = Shape_Class_BASE;
		} else if (codepoint == 0x034F) {
			shape_class = Shape_Class_CGJ;
		} else if (syllable == Unicode_Indic_Syllable_NUKTA ||
				   syllable == Unicode_Indic_Syllable_GEMINATION_MARK ||
				   syllable == Unicode_Indic_Syllable_CONSONANT_KILLER) {
			shape_class = Shape_Class_CONS_MOD;
		} else if (syllable == Unicode_Indic_Syllable_CONSONANT_WITH_STACKER) {
			shape_class = Shape_Class_CONS_WITH_STACKER;
		} else if ((syllable == Unicode_Indic_Syllable_CONSONANT_FINAL && category != Unicode_Category_LO) ||
				   syllable == Unicode_Indic_Syllable_CONSONANT_SUCCEEDING_REPHA) {
			shape_class = Shape_Class_CONS_FINAL;
		} else if (syllable == Unicode_Indic_Syllable_SYLLABLE_MODIFIER) {
			shape_class = Shape_Class_CONS_FINAL_MOD;
		} else if (syllable == Unicode_Indic_Syllable_CONSONANT_PLACEHOLDER ||
				   codepoint == 0x2015 || codepoint == 0x2022 || (codepoint >= 0x25FB && codepoint <= 0x25FE)) {
			shape_class = Shape_Class_BASE_OTHER;
		} else if (syllable == Unicode_Indic_Syllable_VIRAMA ||
				   syllable == Unicode_Indic_Syllable_INVISIBLE_STACKER) {
			shape_class = Shape_Class_HALANT;
		} else if (syllable == Unicode_Indic_Syllable_NUMBER_JOINER) {
			shape_class = Shape_Class_HALANT_NUM;
		} else if (syllable == Unicode_Indic_Syllable_CONSONANT_DEAD ||
				   syllable == Unicode_Indic_Syllable_MODIFYING_LETTER ||
				   (category == Unicode_Category_PO && codepoint != 0x104E && codepoint != 0x2022) ||
				   codepoint == 0x002D) {
			shape_class = Shape_Class_BASE_IND;
		} else if (syllable == Unicode_Indic_Syllable_CONSONANT_MEDIAL && category != Unicode_Category_LO) {
			shape_class = Shape_Class_CONS_MED;
		} else if (syllable == Unicode_Indic_Syllable_BRAHMI_JOINING_NUMBER) {
			shape_class = Shape_Class_BASE_NUM;
		} else if (unicode_script(codepoint) == Unicode_Script_COMMON ||
				   category == Unicode_Category_ZS) {
			shape_class = Shape_Class_OTHER;
		} else if (syllable == Unicode_Indic_Syllable_CONSONANT_PRECEDING_REPHA ||
				   syllable == Unicode_Indic_Syllable_CONSONANT_PREFIXED) {
			shape_class = Shape_Class_REPHA;
		} else if ((category == Unicode_Category_SO && codepoint != 0x25CC) ||
				   category == Unicode_Category_SC) {
			shape_class = Shape_Class_SYM;
		} else if (
			codepoint == 0x1B6B || codepoint == 0x1B6C || 
			codepoint == 0x1B6D || codepoint == 0x1B6E ||
			codepoint == 0x1B6F || codepoint == 0x1B70 ||
			codepoint == 0x1B71 || codepoint == 0x1B72 ||
			codepoint == 0x1B73) {
			shape_class = Shape_Class_SYM_MOD;
		} else if (syllable == Unicode_Indic_Syllable_CONSONANT_SUBJOINED && category != Unicode_Category_LO) {
			shape_class = Shape_Class_CONS_SUB;
		} else if ((syllable == Unicode_Indic_Syllable_VOWEL && category != Unicode_Category_LO) ||
			(syllable == Unicode_Indic_Syllable_VOWEL_DEPENDENT && category != Unicode_Category_LO) ||
			syllable == Unicode_Indic_Syllable_PURE_KILLER) {
			shape_class = Shape_Class_VOWEL;
		} else if ((syllable == Unicode_Indic_Syllable_BINDU && category != Unicode_Category_LO) ||
				   syllable == Unicode_Indic_Syllable_TONE_MARK ||
				   syllable == Unicode_Indic_Syllable_CANTILLATION_MARK ||
				   syllable == Unicode_Indic_Syllable_REGISTER_SHIFTER ||
				   syllable == Unicode_Indic_Syllable_VISARGA) {
			shape_class = Shape_Class_VOWEL_MOD;
		} else if (codepoint >= 0xFE00 && codepoint <= 0xFE0F) {
			shape_class = Shape_Class_VARIATION_SELECTOR;
		} else if (codepoint == 0x2060) {
			shape_class = Shape_Class_WORD_JOINER;
		} else if (syllable == Unicode_Indic_Syllable_JOINER) {
			shape_class = Shape_Class_ZERO_WIDTH_JOINER;
		} else if (syllable == Unicode_Indic_Syllable_NON_JOINER) {
			shape_class = Shape_Class_ZERO_WIDTH_NON_JOINER;
		}

		codepoints[index].shape_class = shape_class;

		Shape_Subclass shape_subclass = Shape_Subclass_NONE;

		if (shape_class == Shape_Class_CONS_MOD) {
			if (position == Unicode_Indic_Position_TOP) {
				shape_subclass = Shape_Subclass_CONS_MOD_ABOVE;
			} else if (position == Unicode_Indic_Position_BOTTOM) {
				shape_subclass = Shape_Subclass_CONS_MOD_BELOW;
			}
		} else if (shape_class == Shape_Class_CONS_FINAL) {
			if (position == Unicode_Indic_Position_TOP) {
				shape_subclass = Shape_Subclass_CONS_FINAL_ABOVE;
			} else if (position == Unicode_Indic_Position_BOTTOM) {
				shape_subclass = Shape_Subclass_CONS_FINAL_BELOW;
			} else if (position == Unicode_Indic_Position_RIGHT) {
				shape_subclass = Shape_Subclass_CONS_FINAL_POST;
			}
		} else if (shape_class == Shape_Class_CONS_MED) {
			if (position == Unicode_Indic_Position_TOP) {
				shape_subclass = Shape_Subclass_CONS_MED_ABOVE;
			} else if (position == Unicode_Indic_Position_BOTTOM) {
				shape_subclass = Shape_Subclass_CONS_MED_BELOW;
			} else if (position == Unicode_Indic_Position_LEFT) {
				shape_subclass = Shape_Subclass_CONS_MED_PRE;
			} else if (position == Unicode_Indic_Position_RIGHT) {
				shape_subclass = Shape_Subclass_CONS_MED_POST;
			}
		} else if (shape_class == Shape_Class_SYM_MOD) {
			if (codepoint == 0x1B6B || codepoint == 0x1B6D ||
				codepoint == 0x1B6E || codepoint == 0x1B6F ||
				codepoint == 0x1B70 || codepoint == 0x1B71 ||
				codepoint == 0x1B72 || codepoint == 0x1B73) {
				shape_subclass = Shape_Subclass_SYM_MOD_ABOVE;
			} else if (codepoint == 0x1B6C) {
				shape_subclass = Shape_Subclass_SYM_MOD_BELOW;
			}
		} else if (shape_class == Shape_Class_VOWEL) {
			if (position == Unicode_Indic_Position_TOP) {
				shape_subclass = Shape_Subclass_VOWEL_ABOVE;
			} else if (position == Unicode_Indic_Position_TOP_AND_BOTTOM) {
				shape_subclass = Shape_Subclass_VOWEL_ABOVE_BELOW;
			} else if (position == Unicode_Indic_Position_TOP_AND_BOTTOM_AND_RIGHT) {
				shape_subclass = Shape_Subclass_VOWEL_ABOVE_BELOW_POST;
			} else if (position == Unicode_Indic_Position_TOP_AND_RIGHT) {
				shape_subclass = Shape_Subclass_VOWEL_ABOVE_POST;
			} else if (position == Unicode_Indic_Position_BOTTOM || position == Unicode_Indic_Position_OVERSTRUCK) {
				shape_subclass = Shape_Subclass_VOWEL_BELOW;
			} else if (position == Unicode_Indic_Position_BOTTOM_AND_RIGHT) {
				shape_subclass = Shape_Subclass_VOWEL_BELOW_POST;
			} else if (position == Unicode_Indic_Position_LEFT) {
				shape_subclass = Shape_Subclass_VOWEL_PRE;
			} else if (position == Unicode_Indic_Position_RIGHT) {
				shape_subclass = Shape_Subclass_VOWEL_POST;
			} else if (position == Unicode_Indic_Position_TOP_AND_LEFT) {
				shape_subclass = Shape_Subclass_VOWEL_PRE_ABOVE;
			} else if (position == Unicode_Indic_Position_TOP_AND_LEFT_AND_RIGHT) {
				shape_subclass = Shape_Subclass_VOWEL_PRE_ABOVE_POST;
			} else if (position == Unicode_Indic_Position_LEFT_AND_RIGHT) {
				shape_subclass = Shape_Subclass_VOWEL_PRE_POST;
			}
		} else if (shape_class == Shape_Class_VOWEL_MOD) {
			if (position == Unicode_Indic_Position_TOP) {
				shape_subclass = Shape_Subclass_VOWEL_MOD_ABOVE;
			} else if (position == Unicode_Indic_Position_BOTTOM || position == Unicode_Indic_Position_OVERSTRUCK) {
				shape_subclass = Shape_Subclass_VOWEL_MOD_BELOW;
			} else if (position == Unicode_Indic_Position_LEFT) {
				shape_subclass = Shape_Subclass_VOWEL_MOD_PRE;
			} else if (position == Unicode_Indic_Position_RIGHT) {
				shape_subclass = Shape_Subclass_VOWEL_MOD_POST;
			}
		}

		codepoints[index].shape_subclass = shape_subclass;
	}

	int cluster_value = 0;
	for (s64 index = 0; index < codepoints.count;) {
		auto syllable = codepoints[index].syllable;

		int iter = (int)index;

		bool consonant_not_found = true;

		// Consonant matching
		while (match_next(codepoints, Unicode_Indic_Syllable_CONSONANT, &iter)) {
			consonant_not_found = false;

			match_next(codepoints, Unicode_Indic_Syllable_NUKTA, &iter);

			if (match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter)) {
				if (match_next(codepoints, UNICODE_ZWNJ, &iter) || match_next(codepoints, UNICODE_ZWJ, &iter)) {
				}
			} else if (match_next(codepoints, UNICODE_ZWNJ, &iter) || match_next(codepoints, UNICODE_ZWJ, &iter)) {
				if (!match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter)) break;
			} else {
				match_next(codepoints, UNICODE_ANUDATTA, &iter);
				if (match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter)) {
					if (match_next(codepoints, UNICODE_ZWNJ, &iter) || match_next(codepoints, UNICODE_ZWJ, &iter)) {
					}
				} else {
					while (match_next(codepoints, Unicode_Indic_Syllable_VOWEL_DEPENDENT, &iter)) {
					}
					match_next(codepoints, Unicode_Indic_Syllable_NUKTA, &iter);
					match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter);
				}
				break;
			}
		}

		if (consonant_not_found) {
			if (match_next(codepoints, UNICODE_DEVANAGARI_RA, &iter)) {
				if (!match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter)) {
					goto done;
				}
			}

			// Vowel matching
			if (match_next(codepoints, Unicode_Indic_Syllable_VOWEL_INDEPENDENT, &iter)) {
				match_next(codepoints, Unicode_Indic_Syllable_NUKTA, &iter);

				if (match_next(codepoints, UNICODE_ZWNJ, &iter)) {
					if (!match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter)) goto done;
					if (!match_next(codepoints, Unicode_Indic_Syllable_CONSONANT, &iter)) goto done;
				} else if (match_next(codepoints, UNICODE_ZWJ, &iter)) {
					if (match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter)) {
						if (!match_next(codepoints, Unicode_Indic_Syllable_CONSONANT, &iter)) goto done;
					} else if (!match_next(codepoints, Unicode_Indic_Syllable_CONSONANT, &iter)) {
						goto done;
					}
				} else if (match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter)) {
					if (!match_next(codepoints, Unicode_Indic_Syllable_CONSONANT, &iter)) goto done;
				}
			} else { // Stand-Alone cluster
				if (iter == 0 || unicode_grapheme_property(codepoints[iter].codepoint) == Unicode_Grapheme_Property_UNKNOWN) {
					if (!match_next(codepoints, UNICODE_NBSP, &iter)) goto done;
					match_next(codepoints, Unicode_Indic_Syllable_NUKTA, &iter);

					if (match_next(codepoints, UNICODE_ZWNJ, &iter) || match_next(codepoints, UNICODE_ZWJ, &iter)) {
						if (!match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter)) goto done;
						if (!match_next(codepoints, Unicode_Indic_Syllable_CONSONANT, &iter)) goto done;
					}
				}
			}

			while (match_next(codepoints, Unicode_Indic_Syllable_VOWEL_DEPENDENT, &iter)) {
			}

			match_next(codepoints, Unicode_Indic_Syllable_NUKTA, &iter);
			match_next(codepoints, Unicode_Indic_Syllable_VIRAMA, &iter);
		}

	done:

		// Common ending
		match_next(codepoints, Unicode_Indic_Syllable_MODIFYING_LETTER, &iter);
		// vedic
		match_range(codepoints, 0xA8E0, 0xA8FF, &iter);
		match_range(codepoints, 0xA8E0, 0xA8FF, &iter);

		int clustered_counts = iter - (int)index;
		if (clustered_counts == 0) clustered_counts = 1;
		for (int ii = 0; ii < clustered_counts; ++ii) {
			codepoints[ii + index].value = cluster_value;
		}

		cluster_value += 1;
		index += clustered_counts;
	}

	// Converting codepoints to glyph indices
	for (s32 i = 0; i < codepoints.count; ++i) {
		auto id                = (u32)stbtt_FindGlyphIndex(&font->info, (int)codepoints[i].codepoint);
		codepoints[i].glyph_id = id;
		codepoints[i].position = unicode_indic_position(codepoints[i].codepoint);
		codepoints[i].base     = false;
		//glyph_range_add_no_bound_check(&range, id, codepoints[i].codepoint, codepoints[i].value);
	}

#if 0
	for (s64 index = 0; index < codepoints.count; ++index) {
		auto first = index;
		auto last  = index;
		for (s64 iter = first + 1; iter < codepoints.count && codepoints[first].value == codepoints[iter].value; ++iter)
			last += 1;

		auto base_index = first;
		for (auto iter = last; iter >= first; --iter) {
			if (codepoints[iter].syllable == Unicode_Indic_Syllable_CONSONANT) {
				if (iter + 1 <= last) {
					if (codepoints[iter + 1].position != Unicode_Indic_Position_BOTTOM &&
						codepoints[iter + 1].position != Unicode_Indic_Position_RIGHT) {
						base_index = iter;
						break;
					} else if (codepoints[iter].codepoint == UNICODE_DEVANAGARI_RA &&
							   codepoints[iter + 1].position != Unicode_Indic_Position_LEFT) {
						base_index = iter;
						break;
					} else {
						base_index = iter;
					}
				} else {
					base_index = iter;
					break;
				}
			}
		}

		for (auto iter = first; iter + 2 < last; iter += 2) {
			if (codepoints[iter + 0].codepoint == UNICODE_DEVANAGARI_RA &&
				codepoints[iter + 1].syllable == Unicode_Indic_Syllable_VIRAMA &&
				codepoints[iter + 2].syllable == Unicode_Indic_Syllable_CONSONANT) {

				if (codepoints[iter + 2].codepoint == UNICODE_DEVANAGARI_RA &&
					iter + 3 < last && codepoints[iter + 3].syllable == Unicode_Indic_Syllable_VIRAMA) {

				} else {
					auto temp            = codepoints[iter + 0];
					codepoints[iter + 0] = codepoints[iter + 2];
					codepoints[iter + 2] = codepoints[iter + 1];
					codepoints[iter + 1] = temp;
				}
			}
		}
		

		codepoints[base_index].base = true;

		index += last - first;
	}
#endif

	//for (s64 index = 0; index < codepoints.count; ++index) {
	//	if (index + 2 < codepoints.count) {
	//
	//	}
	//}

	//Glyph_Cluster sentinel;
	//sentinel.codepoint = 0;
	//sentinel.glyph_id  = 0;
	//sentinel.syllable  = Unicode_Indic_Syllable_UNKNOWN;
	//sentinel.value     = MAX_INT32;
	//array_add(&codepoints, sentinel);

	static Feature_Tag features[] = {
		Feature_Tag_LOCL,
		Feature_Tag_NUKT,
		Feature_Tag_AKHN,
		Feature_Tag_RPHF,
		Feature_Tag_RKRF,
		Feature_Tag_BLWF,
		Feature_Tag_HALF,
		Feature_Tag_VATU,
		Feature_Tag_CJCT,
		Feature_Tag_PRES,
		Feature_Tag_ABVS,
		Feature_Tag_BLWS,
		Feature_Tag_PSTS,
		Feature_Tag_HALN,
		Feature_Tag_CALT,
	};

	for (s64 index = 0; index < codepoints.count; ++index) {
		auto first = index;
		auto last  = index;
		for (s64 iter = first + 1; iter < codepoints.count && codepoints[first].value == codepoints[iter].value; ++iter)
			last += 1;

		auto base_index = first;
		for (auto iter = last; iter >= first; --iter) {
			if (codepoints[iter].syllable == Unicode_Indic_Syllable_CONSONANT) {
				if (iter + 1 <= last) {
					if (codepoints[iter + 1].position != Unicode_Indic_Position_BOTTOM &&
						codepoints[iter + 1].position != Unicode_Indic_Position_RIGHT) {
						base_index = iter;
						break;
					} else if (codepoints[iter].codepoint == UNICODE_DEVANAGARI_RA &&
							   codepoints[iter + 1].position != Unicode_Indic_Position_LEFT) {
						base_index = iter;
						break;
					} else {
						base_index = iter;
					}
				} else {
					base_index = iter;
					break;
				}
			}
		}

#if 0
		for (auto iter = first; iter + 2 <= last; ++iter) {
			if (codepoints[iter + 0].codepoint == UNICODE_DEVANAGARI_RA &&
				codepoints[iter + 1].syllable == Unicode_Indic_Syllable_VIRAMA &&
				codepoints[iter + 2].syllable == Unicode_Indic_Syllable_CONSONANT) {
				if (codepoints[iter + 2].codepoint == UNICODE_DEVANAGARI_RA &&
					iter + 3 < last && codepoints[iter + 3].syllable == Unicode_Indic_Syllable_VIRAMA) {
					break;
				} else {
					auto temp0            = codepoints[iter + 0];
					auto temp1            = codepoints[iter + 1];
					memmove(&codepoints[iter + 0], &codepoints[iter + 2], (base_index - iter - 1) * sizeof(Glyph_Cluster));
					base_index -= 2;
					codepoints[base_index + 1] = temp0;
					codepoints[base_index + 2] = temp1;
					break;
				}
			}
		}
#endif

		codepoints[base_index].base = true;

		index += last - first;
	}

	Array<Glyph_Cluster> originals;
	array_resize(&originals, codepoints.count);
	for (s64 ii = 0; ii < codepoints.count; ++ii)
		array_add(&originals, codepoints[ii]);

	Glyph_Range range;
	range.clusters = codepoints;
	range.index    = 0;

#if 1
	auto &gsub = font->gsub_feature;
	s64   skip = 1;
	for (range.index = 0; range.index < range.clusters.count; range.index += skip) {
		// TODO: Speed!!!
		for (int i = 0; i < static_count(features); ++i) {
			for (u16 feature_index = 0; feature_index < gsub.indices.count; ++feature_index) {
				u16 feature_record_index = bswap16p_be((u16 *)gsub.indices.ptr + feature_index);
				assert(feature_record_index < gsub.count);
				u8 *        feature_record = gsub.records + feature_record_index * 6;
				Feature_Tag feature_tag    = *((Feature_Tag *)feature_record);
				if (feature_tag == features[i]) {
					font_apply_gsub_feature(font, gsub.ptr + bswap16p_be(feature_record + 4), &range);
				}
			}
		}
	}

#endif

	codepoints = range.clusters;

	Font_Shape shape;
	//shape.glyph_infos = range.infos;
	//shape.glyph_count = range.count;
	shape.codepoints = codepoints;
	shape.originals  = originals;

	return shape;
}
