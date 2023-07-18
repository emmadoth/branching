#include <stdio.h>
#include <time.h>

// change this to alter how many times each function is ran for an average
#define n 1000000

#define tt 1, 1
#define tf 1, 0
#define ff 0, 0
#define ft 0, 1

#define bc b = clock()
#define ec e = clock()

#define pc(S) printf(S ": %.3fns\n", a)

#define perf(T, S)                                                \
for(size_t i=0;i<n;++i)                                           \
{                                                                 \
    bc;                                                           \
    T;                                                            \
    ec;                                                           \
    t[i]=((double)e-(double)b)/(double)CLOCKS_PER_SEC*1000000000; \
} avg; pc(S)

#define avg for(size_t i=0;i<n;++i) {a+=t[i];} a/=n

static inline _Bool eager(_Bool a, _Bool b)
{
    return a | b;
}

static inline _Bool lazy(_Bool a, _Bool b)
{
    if(a)
        return 1;
    return b;
}

#ifndef INLINE_ASM
extern _Bool eager_asm(_Bool a, _Bool b);
extern _Bool  lazy_asm(_Bool a, _Bool b);
#else
// TODO: inline assembly versions are not implemented correctly loll
static inline _Bool eager_asm(_Bool a, _Bool b)
{
    _Bool r = 0;
    asm inline volatile(
        "movzx %[a], %%eax\t\n"
        "movzx %[b], %%edx\t\n"
        "or %%eax, %%edx\t\n"
        "sete %%al\t\n"
        "mov %%al, %[r]\t\n"
        : [r] "=m" (r)
        : [a] "m" (a), [b] "m" (b)
    );
    return r;
}

static inline _Bool lazy_asm(_Bool a, _Bool b)
{
    _Bool r = 0;
    asm inline volatile(
        "mov %[a], %%eax\t\n"
        "test %%eax, %%eax\t\n"
        "sete %%al\t\n"
        "test %%dl, %%dl\t\n"
        "sete %%dl\t\n"
        "movzx %%dl, %%edx\t\n"
        "and %%eax, %%edx\t\n"
        "mov %%eax, %[r]\t\n"
        : [r] "=m" (r)
        : [a] "m" (a), [b] "m" (b)
    );
    return r;
}
#endif /* INLINE_ASM */

int main()
{

    clock_t b=0, e=0;
    double a=0, t[n]={0};

    perf(eager(tt),     "eager     true  true ");
    perf(eager(tf),     "eager     true  false");
    perf(eager(ff),     "eager     false false");
    perf(eager(ft),     "eager     false true ");

    perf( lazy(tt),     " lazy     true  true ");
    perf( lazy(tf),     " lazy     true  false");
    perf( lazy(ft),     " lazy     false true ");
    perf( lazy(ff),     " lazy     false false");

    perf(eager_asm(tt), "eager_asm true  true ");
    perf(eager_asm(tf), "eager_asm true  false");
    perf(eager_asm(ff), "eager_asm false false");
    perf(eager_asm(ft), "eager_asm false true ");

    perf( lazy_asm(tt), " lazy_asm true  true ");
    perf( lazy_asm(tf), " lazy_asm true  false");
    perf( lazy_asm(ft), " lazy_asm false true ");
    perf( lazy_asm(ff), " lazy_asm false false");

    return 0;
}
