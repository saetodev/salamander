#ifndef SAL_TYPES_H
#define SAL_TYPES_H

#include <string.h>
#include <stdint.h>
#include <assert.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef uint8_t i8;
typedef uint16_t i16;
typedef uint32_t i32;
typedef uint64_t i64;

typedef float f32;
typedef double f64;

#define KIBIBYTE(n) ((n) * 1024ll)
#define MEBIBYTE(n) (KIBIBYTE(n) * 1024ll)
#define GIBIBYTE(n) (MEBIBYTE(n) * 1024ll)

struct Vec2 {
    f32 x;
    f32 y;

    Vec2() = default;
    Vec2(f32 x, f32 y): x(x), y(y) {}
};

class MemoryArena {
public:
    MemoryArena(u64 size): m_size(size) {
        m_bytes = new u8[size];
    }

    ~MemoryArena() {
        delete[] m_bytes;
        m_bytes = nullptr;
    }

    void clear() {
        m_offset = 0;
    }

    void *alloc(u64 size) {
        assert((m_offset + size) < m_size);

        void *mem = m_bytes + m_offset;
        m_offset += size;

        memset(mem, 0, size);

        return mem;
    }

private:
    u64 m_size = 0;
    u64 m_offset = 0;
    u8 *m_bytes = nullptr;
};

#endif