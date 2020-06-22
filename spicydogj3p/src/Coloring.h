
#define Coloring                int[]
#define Coloring_AddR(coloring) coloring[0]
#define Coloring_AddG(coloring) coloring[1]
#define Coloring_AddB(coloring) coloring[2]
#define Coloring_MulR(coloring) coloring[3]
#define Coloring_MulG(coloring) coloring[4]
#define Coloring_MulB(coloring) coloring[5]

    protected static Coloring newColoring() {
        Coloring coloring = new int[6];
        Coloring_AddR(coloring) = readSI16();
        Coloring_AddG(coloring) = readSI16();
        Coloring_AddB(coloring) = readSI16();
        Coloring_MulR(coloring) = readFI24();
        Coloring_MulG(coloring) = readFI24();
        Coloring_MulB(coloring) = readFI24();
        return coloring;
    }
    
    /** 色変形行列を元に色を変換しRGBからデバイス依存色に変換する。
        
        @param      coloring        色変形行列
        @param      srcColor        RGB形ｮ色 0x00rrggbb
        @return     変形後のデバイス依存形式の色情報 */
    protected static int realizeColor(Coloring coloring,int srcColor) {
        if (srcColor == -1) {
            return -1;
        }
        return GET_COLOR_OF_RGB(
                        saturate(
                            ((srcColor >> 16) & 0xFF),
                            Coloring_MulR(coloring),
                            Coloring_AddR(coloring)),
                        saturate(
                            ((srcColor >>  8) & 0xFF),
                            Coloring_MulG(coloring),
                            Coloring_AddG(coloring)),
                        saturate(
                            ( srcColor        & 0xFF),
                            Coloring_MulB(coloring),
                            Coloring_AddB(coloring))
                        );
    }
    
    /** 指定範囲内[0,255]で値を飽和させる。*/
    protected static int saturate(int src,int mul,int add) {
        int n;
        n = src * mul / 0x100 + add;

        if (n < 0) {
            return 0;
        } else if (255 < n) {
            return 255;
        }
        return n;
    }
