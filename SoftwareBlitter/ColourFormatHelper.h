#ifndef __COLOURFORMATHELPER_H__
#define __COLOURFORMATHELPER_H__

#define RGB16(red, green, blue) ( ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

#define RGB32(red, green, blue) ( (red << 16) | (green << 8) | (blue)  )

#endif
