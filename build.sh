# Compile this if you want ripemd160 MD digest.
#echo "gcc -DRMD160SUM $CFLAGS -o do_md do_md.c -lcrypto -lssl"
#gcc -DRMD160SUM $CFLAGS -o do_md do_md.c -lcrypto -lssl
# Compile this if you want ripemd160 and crc32 MD digests.
#echo "gcc -DCRC -DRMD160SUM $CFLAGS -o do_md do_md.c -lcrypto -lssl"
#gcc -DCRC -DRMD160SUM $CFLAGS -o do_md do_md.c -lcrypto -lssl
echo "clang-12 -Ofast -std=gnu99 -o mdp token.c mdp.c /usr/lib/x86_64-linux-gnu/libcrypto.a /usr/lib/x86_64-linux-gnu/libssl.a"
clang-12 -Ofast -std=gnu99 -o mdp mdp.c token.c sort.c /usr/lib/x86_64-linux-gnu/libcrypto.a /usr/lib/x86_64-linux-gnu/libssl.a
echo "clang-12 -Ofast -std=gnu99 -o crc64sum -DTEST_MAIN crc64sum.c"
clang-12 -Ofast -std=gnu99 -o crc64sum -DTEST_MAIN crc64sum.c
