/**
 * Assert.
 */
void  do_exit(const char *msg) __attribute__((noreturn));

#ifdef NDEBUG
#define assert(X) do {} while (0)
#else
#define do_string2(x) do_string(x)
#define do_string(x) #x
#define assert(X) do { if (!(X)) do_exit("assertion '" #X  "' failed in "  __FILE__  ":" do_string2(__LINE__) ); } while (0)
#endif
