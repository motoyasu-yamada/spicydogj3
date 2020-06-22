
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
    
    /** �F�ό`�s������ɐF��ϊ���RGB����f�o�C�X�ˑ��F�ɕϊ�����B
        
        @param      coloring        �F�ό`�s��
        @param      srcColor        RGB�`��F 0x00rrggbb
        @return     �ό`��̃f�o�C�X�ˑ��`���̐F��� */
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
    
    /** �w��͈͓�[0,255]�Œl��O�a������B*/
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
