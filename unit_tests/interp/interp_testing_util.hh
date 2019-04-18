#ifndef INTERP_TESTING_UTIL_HH
#define INTERP_TESTING_UTIL_HH

#include <stddef.h>
#include <rs274ngc_interp.hh>
#include <interp_return.hh>

#define REQUIRE_RANGE(got, expected, tol) REQUIRE( (got) <= (expected + tol)); REQUIRE( (got) >= (expected - tol))
#define INTERP_FUZZ 1e-10
#define REQUIRE_FUZZ(got, expected) REQUIRE_RANGE(got, expected, INTERP_FUZZ)
#define REQUIRE_INTERP_OK(val) REQUIRE(val < INTERP_MIN_ERROR)

extern InterpBase *pinterp;

template <size_t N>
int execute_lines(Interp &interp, const char* (&lines)[N] )
{
  for (auto l : lines) {
    int res = interp.execute(l);
    if (res != INTERP_OK) {
      return res;
    }
  }
  return INTERP_OK;
}

// Do a bunch of setup / assignments to force reinitialize SAI internals enough that sections won't step on each other
// KLUDGE the reference-to-pointer is a workaround f
#define DECL_INIT_TEST_INTERP() \
    if (pinterp) { \
        delete pinterp; \
        pinterp = nullptr; \
    } \
    pinterp = makeInterp(); \
    Interp &test_interp = *dynamic_cast<Interp*>(pinterp); \
    setup * const settings = &test_interp._setup; \
    test_interp.init(); \
    settings->tool_table[0].toolno = 1; \

#endif // INTERP_TESTING_UTIL_HH
