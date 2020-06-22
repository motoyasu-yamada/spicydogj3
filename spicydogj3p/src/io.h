
    private static byte[]  BUFFER;
    private static int     POS_READING;
    
    protected static  void skip(int n) {
        POS_READING  += n;
    }
    
    protected static int readFI24() {
        int digit = readSI16();
        int decim = readUI8();
        return (digit << 8) + decim;
    }
    
    protected static int readSI8() {
        return BUFFER[POS_READING++];
    }

    protected static  int readUI8() {
        int n = readSI8();
        if (n < 0)
            return 256 + n;
        else
            return n;
    }

    protected static  int readSI16() {
        int n = readUI16();
        if (0x8000 <= n)
            return -(0x10000) + n;
        else
            return n;
    }

    protected static  int readUI16() {
        return (readUI8() << 8) + readUI8();
    }

    protected static  int readUI32() {
        return (readUI16() << 16) + readUI16();
    }

    protected static int[] readXY() {
        int b1 = readUI8();
        int b2 = readUI8();
        int b3 = readUI8();
        int x = (b1 << 4) + ((b2 >> 4) & 0x0F);
        int y = ((b2 & 0xF) << 8) + b3;
        if (2048 <= x) {
            x -= 4096;
        }
        if (2048 <= y) {
            y -= 4096;
        }
        return new int[] {x,y};
    }
    
    protected static int readRGB() {
        return (readUI8() << 16) + (readUI8() << 8) + readUI8();
    }

    private static int readColorOfRGB() {
        return GET_COLOR_OF_RGB(readUI8(),readUI8(),readUI8());
    }

    protected static String readString() {
        int length = readUI16();
        int start  = POS_READING;
        POS_READING += length;
        return new String(BUFFER,start,length);
    }
