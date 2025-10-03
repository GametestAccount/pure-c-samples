#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define BIG_ENDIAN 0x0
#define LITTLE_ENDIAN 0x1

#define GetByteEndian() ((*((uint8_t *)&(const uint16_t){1})) ? LITTLE_ENDIAN : BIG_ENDIAN)

#define bswap16(x) (((uint16_t)x << 8) | ((uint16_t)x >> 8))
#define bswap32(x) (((uint32_t)x << 24) | (((uint32_t)x << 8) & 0xFF0000) | (((uint32_t)x >> 8) & 0xFF00) | ((uint32_t)x >> 24))
#define bswap64(x) ( \
    ((uint64_t)x << 56) | \
    (((uint64_t)x << 40) & 0xFF000000000000) | \
    (((uint64_t)x << 24) & 0xFF0000000000) | \
    (((uint64_t)x << 8) & 0xFF00000000) | \
    (((uint64_t)x >> 8) & 0xFF000000) | \
    (((uint64_t)x >> 24) & 0xFF0000) | \
    (((uint64_t)x >> 40) & 0xFF00) | \
    (((uint64_t)x >> 56)) \
)

#define hton16(x) (GetByteEndian() ? bswap16(x) : x)
#define hton32(x) (GetByteEndian() ? bswap32(x) : x)
#define hton64(x) (GetByteEndian() ? bswap64(x) : x)

#define ntoh16(x) (hton16(x))
#define ntoh32(x) (hton32(x))
#define ntoh64(x) (hton64(x))

static void DisplayBits(uintmax_t x, size_t bytes)
{
    for (int i = (bytes * 8) - 1; i >= 0; i--)
        printf("%c%s", (x & ((uintmax_t)1 << i)) ? '1' : '0', (i % 8) ? "" : " ");

    putchar('\n');
}

static void DisplayBytes(const uint8_t *data, size_t size)
{
    for (int i = 0; i < size; i++)
        printf("0x%02x%s", data[i] & 0xFF, (i != (size - 1)) ? ", " : "\n");
}