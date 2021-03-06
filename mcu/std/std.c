char *ftc(float x, char *p, int length) {
    char *s = p + length;
    int decimals;
    int units;

    if (x < 0) {
        decimals = (int)(x * -100) % 100;
        units = (int)(-1 * x);
    } else {
        decimals = (int)(x * 100) % 100;
        units = (int)x;
    }

    *--s = (decimals % 10) + '0';
    decimals /= 10;
    *--s = '.';

    while (units > 0) {
        *--s = (units % 10) + '0';
        units /= 10;
    }
    if (x < 0) *--s = '-';

    return s;
}
