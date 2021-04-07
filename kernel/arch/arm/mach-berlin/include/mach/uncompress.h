#ifdef CONFIG_DEBUG_LL
extern void putc(int c);
#else
#define putc(c)
#endif
#define flush()
#define arch_decomp_setup()
#define arch_decomp_wdog()
