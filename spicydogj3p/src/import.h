/** インポート定義 */
import java.io.*;
import java.util.*;

#if defined(JPHONE)
    import com.j_phone.midlet.*;
    import com.j_phone.system.*;
    import com.jblend.media.smaf.*;
    import javax.microedition.io.*;
    import javax.microedition.lcdui.*;
    import javax.microedition.midlet.*;

#if defined(JPHONE_PLATFORM_G)
    import com.jblend.media.smaf.phrase.*;
#endif
#if defined(JPHONE_PLATFORM_F)
    import com.j_phone.amuse.*;
#endif

#else
    #error unsupported configuration.
#endif