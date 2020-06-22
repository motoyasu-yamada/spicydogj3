/*
*   デバッグコード
*/
#ifdef DEBUG
    #define TRACE(s)     System.out.println(__FILE__ + "("+ __LINE__ + "):" + (s))
    #define TERMINATE()  throw new IllegalStateException()
    #define ASSERT(b)    do { if(!(b)) {TRACE("assert");TERMINATE();} } while(false)
    #define EXCEPTION(e) do { e.printStackTrace();TERMINATE(); } while(false)
    #define DEBUG_ONLY(s) s
#else
    #define TRACE(s)
    #define TERMINATE()
    #define ASSERT(b)
    #define EXCEPTION(e)
    #define DEBUG_ONLY(s)
#endif
