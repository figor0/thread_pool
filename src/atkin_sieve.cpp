#include <atkin_sieve.h>

std::vector<bool> atkin_sieve::getPrimesUpTo(const unsigned limit) {
    // Предварительное просеивание
    std::vector<bool> sieve(limit);
    for (unsigned long long x2 = 1ull, dx2 = 3ull; x2 < limit; x2 += dx2, dx2 += 2ull)
        for (unsigned long long y2 = 1ull, dy2 = 3ull, n; y2 < limit; y2 += dy2, dy2 += 2ull) {
            // n = 4x² + y²
            n = (x2 << 2ull) + y2;
            if (n <= limit && (n % 12ull == 1ull || n % 12ull == 5ull))
                sieve[n].flip();
            // n = 3x² + y²
            n -= x2;
            if (n <= limit && n % 12ull == 7ull)
                sieve[n].flip();
            // n = 3x² - y² (при x > y)
            if (x2 > y2) {
                n -= y2 << 1ull;
                if (n <= limit && n % 12ull == 11ull)
                    sieve[n].flip();
            }
        }
    // Все числа, кратные квадратам, помечаются как составные
    unsigned r = 5u;
    for (unsigned long long r2 = r * r, dr2 = (r << 1ull) + 1ull; r2 < limit; ++r, r2 += dr2, dr2 += 2ull)
        if (sieve[r])
            for (unsigned long long mr2 = r2; mr2 < limit; mr2 += r2)
                sieve[mr2] = false;
    // Числа 2 и 3 — заведомо простые
    if (limit > 2u)
        sieve[2u] = true;
    if (limit > 3u)
        sieve[3u] = true;
    return sieve;
}
