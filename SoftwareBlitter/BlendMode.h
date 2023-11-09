#ifndef __BLENDMODE_H__
#define __BLENDMODE_H__

enum BlendMode
{
	ALWAYS_PASS_DEPTH,

	// maybe rename this to 'blit clear pixel only'
	ALWAYS_FAIL_DEPTH,

	COMPARE_DEPTH,
};

#endif
