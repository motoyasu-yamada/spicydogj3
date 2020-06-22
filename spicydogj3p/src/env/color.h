/*-----------------------------------------------------------------------------
	カラーコード圧縮用即値定義
	
	$Id: color.h,v 1.1.1.1 2003/04/15 08:58:47 m_yamada Exp $
	$Revision: 1.1.1.1 $
	$Date: 2003/04/15 08:58:47 $
	$Author: m_yamada $

-------------------------------------------------------------------------------*/

#if defined(DOJA)
#define GET_COLOR_OF_NAME(value) (GRAPHICS.getColorOfName(value))
#define GET_COLOR_OF_RGB(value)  (GRAPHICS.getColorOfRGB(value))

#define	COLOR_BLACK		0
#define	COLOR_BLUE		1
#define	COLOR_LIME		2
#define	COLOR_AQUA		3
#define	COLOR_RED		4
#define	COLOR_FUCHSIA	5
#define	COLOR_YELLOW	6
#define	COLOR_WHITE		7
#define	COLOR_GRAY		8
#define	COLOR_NAVY		9
#define	COLOR_GREEN		10
#define	COLOR_TEAL		11
#define	COLOR_MAROON	12
#define	COLOR_PURPLE	13
#define	COLOR_OLIVE		14
#define	COLOR_SILVER	15

#elif defined(JPHONE)
#define GET_COLOR_OF_NAME(value) (value)
#define GET_COLOR_OF_RGB(r,g,b)  ((r << 16) + (g << 8) + b)

#define	COLOR_BLACK		0x000000
#define	COLOR_BLUE		0x0000FF
#define	COLOR_LIME		0x00FFFF
#define	COLOR_AQUA		0x0000FF
#define	COLOR_RED		0xFF0000
#define	COLOR_FUCHSIA	0x800000
#define	COLOR_YELLOW	0xFFFF00
#define	COLOR_WHITE		0xFFFFFF
#define	COLOR_GRAY		0x808080
#define	COLOR_NAVY		0x000080
#define	COLOR_GREEN		0x00FF00
#define	COLOR_TEAL		0x00FF00
#define	COLOR_MAROON	0x00FF00
#define	COLOR_PURPLE	0xFF00FF
#define	COLOR_OLIVE		0x000000
#define	COLOR_SILVER	0x808080

#else
#error unsupported configuration.

#endif
