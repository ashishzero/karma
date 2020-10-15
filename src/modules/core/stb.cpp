#include "karma.h"

#define STBI_ONLY_PNG
#define STBI_ASSERT(x)			  assert(x)
#define STBI_MALLOC(sz)           memory_allocate(sz)
#define STBI_REALLOC(p,newsz)     memory_reallocate(p,newsz)
#define STBI_FREE(p)              memory_free(p)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"


#define STBTT_assert(x)    assert(x)
#define STBTT_malloc(x,u)  ((void)(u),memory_allocate(x))
#define STBTT_free(x,u)    ((void)(u),memory_free(x))
#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_rect_pack.h"
#include "stb_truetype.h"
