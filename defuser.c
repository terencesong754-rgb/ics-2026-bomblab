#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t low = (uint32_t)challenge;
    uint32_t high = (uint32_t)(challenge >> 32);
    uint32_t middle = (uint32_t)(challenge >> 16);

    uint32_t result1 = (high + middle * 8) ^ 0x9155;
    uint32_t result0 = (3 * low) ^ (high + 0xd771);

    values[0] = (result1 ^ 0x126e) + 8 * result0;
    count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t first = (uint32_t)(challenge >> 45);

    values[0] = ((first * 2) & 0x7e) + 0x13;

    for (uint32_t i = 1; i < 6; i++) {
        uint32_t shift = 12 + (i - 1) * 5;
        values[i] = (((uint32_t)(challenge >> shift) & 0x1f) + values[i - 1]) ^ (i * 8);
    }

    count = 6;
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t first = (uint32_t)(challenge & 0x7);

    uint32_t a = (uint32_t)(challenge >> 3);
    uint32_t b = (uint32_t)(challenge >> 32);
    uint32_t key = a ^ b;

    uint32_t answer = 0;

    switch (first) {
        case 0:
            answer = (key * 2) ^ 0xe350;
            break;
        case 1:
            answer = key * 3 + 0xce84;
            break;
        case 2:
            answer = (key ^ 0x70c2) + (key >> 1);
            break;
        case 3:
            answer = (key ^ 0x959b) + (key >> 1);
            break;
        case 4:
            answer = key * 5 + 0x102de;
            break;
        case 5:
            answer = key * 5 + 0x91c8;
            break;
        case 6:
            answer = key * 5 + 0x19357;
            break;
        case 7:
            answer = (key * 2) ^ 0x330a;
            break;
    }

    values[0] = first;
    values[1] = answer;
    count = 2;
    return write_answer(out, cap, values, count);
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;

    uint32_t target = (uint32_t)challenge & 0x7f;
    uint32_t key = (uint32_t)(challenge >> 7)
                ^ (uint32_t)(challenge >> 32);

    uint32_t low = 0;
    uint32_t high = 127;

    uint32_t mids[8];
    uint32_t ops[8];
    uint32_t depth = 0;

    while (1) {
        uint32_t mid = low + ((high - low) >> 1);

        if (target == mid) {
            uint32_t result = (mid + 0x75) ^ key;

            while (depth > 0) {
                depth--;
                result = ((result * 5) ^ (mids[depth] + key)) ^ ops[depth];
            }

            values[0] = result;
            break;
        }

        mids[depth] = mid;

        if (target < mid) {
            ops[depth] = 0x2;
            high = mid - 1;
        } else {
            ops[depth] = 0xc;
            low = mid + 1;
        }

        depth++;
    }

    count = 1;

    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t index = (uint32_t)(challenge & 0x7);
    uint32_t steps = (uint32_t)(challenge >> 3) & 0x3;

    uint32_t key = ((uint32_t)(challenge >> 6)
                ^ (uint32_t)(challenge >> 32)) ^ 0x38a6;

    uint32_t v1[11] = {
        0x836,  0xf638, 0x69cb, 0xfa53,
        0xb313, 0xa927, 0x84d0, 0xb060,
        0xa165, 0x6463, 0x913c
    };

    uint32_t v2[11] = {
        0xa31, 0x623, 0x153, 0x25a,
        0x37a, 0x82a, 0x981, 0x74f,
        0x42d, 0x52f, 0x25
    };

    uint32_t next[11] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0
    };

    uint32_t node = index;


    uint32_t result = key * 3 + v1[node];
    result ^= v2[node];

    uint32_t last_value = v2[node];

    for (uint32_t i = 0; i < steps; i++) {
        node = next[node];
        result = result * 3 + v1[node];
        result ^= v2[node];
        last_value = v2[node];
    }

    values[0] = result;
    values[1] = last_value;
    count = 2;
    return write_answer(out, cap, values, count);
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
        
    uint32_t index1 = (uint32_t)(challenge & 0x7);
    uint32_t index2 = (uint32_t)((challenge & 0x38) >> 3);

    uint32_t arg = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);

    uint32_t result = 0;


    switch (index1) {
        case 0:
            result = (arg >> 3) | (arg << 29);
            break;

        case 1:
            result = (arg ^ 0xf3fa) + arg * 8;
            break;

        case 2:
        case 3:
            result = ((arg << 3) | (arg >> 29)) ^ 0x9900;
            break;

        case 4:
            result = (arg ^ (arg >> 2)) + 0xb991;
            break;

        case 5:
        case 7:
            result = (arg + 0x5026) ^ (arg >> 2);
            break;

        case 6:
            result = (arg * 4 + 0xf8ce) ^ (arg * 5);
            break;
    }

    result ^= 0xb21d;

    switch (index2) {
        case 0:
            result = (result >> 3) | (result << 29);
            break;

        case 1:
            result = (result ^ 0xf3fa) + result * 8;
            break;

        case 2:
        case 3:
            result = ((result << 3) | (result >> 29)) ^ 0x9900;
            break;

        case 4:
            result = (result ^ (result >> 2)) + 0xb991;
            break;

        case 5:
        case 7:
            result = (result + 0x5026) ^ (result >> 2);
            break;

        case 6:
            result = (result * 4 + 0xf8ce) ^ (result * 5);
            break;
    }

    values[0] = result + 0xc814;
    count = 1;

    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
        
    uint32_t a = (uint32_t)context
            ^ (uint32_t)(context >> 32);

    uint32_t b = (uint32_t)challenge
            ^ (uint32_t)(challenge >> 32);

    uint32_t first = (uint32_t)context & 3;
    uint32_t x = a ^ b;

    uint32_t mix = ((x >> 3) ^ 0x8a2)
                + (x ^ 0xca01);

    uint32_t second = mix
                    ^ (0xca01 * first)
                    ^ (uint32_t)(context >> 16);

    uint32_t third = second
                ^ ((b >> 3) + a + first + 0x8a2);


    if (out == NULL || cap == 0) {
        return 1;
    }

    int len = snprintf(out, cap, "%032uOPEN %u %u %u",
                   0u,
                   (unsigned)first,
                   (unsigned)second,
                   (unsigned)third);

    if (len < 0 || (size_t)len >= cap) {
        return 1;
    }


    size_t used = (size_t)len;

    while (used < 36) {
        if (used + 1 >= cap) {
            return 1;
        }

        out[used++] = ' ';
    }

    out[used] = '\0';

    return 0;

}
