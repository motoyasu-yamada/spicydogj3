#define AffineMatrix int[]
#define AffineMatrix_ScaleX(matrix) 0x100
#define AffineMatrix_ScaleY(matrix) 0x100
#define AffineMatrix_RotateX(matrix) 0
#define AffineMatrix_RotateY(matrix) 0
#define AffineMatrix_TranslateX(matrix) matrix[0]
#define AffineMatrix_TranslateY(matrix) matrix[1]

    private static AffineMatrix newMATRIX() {
        AffineMatrix matrix = readXY();
        return matrix;
    }

    protected static int operatorX(AffineMatrix matrix,int x,int y) {
        int v =(x * AffineMatrix_ScaleX(matrix) + 
                y * AffineMatrix_RotateX(matrix) ) / 0x100 + 
               AffineMatrix_TranslateX(matrix) + 
               SpicydogCanvas.DRAW_WIDTH /2;
        return v;
    }
    
    protected static int operatorY(AffineMatrix matrix,int x,int y) {
        int v =(y * AffineMatrix_ScaleY (matrix) + 
                x * AffineMatrix_RotateY(matrix) ) / 0x100 + 
               AffineMatrix_TranslateY(matrix) + 
               SpicydogCanvas.DRAW_HEIGHT/2;
        return v;
    }
