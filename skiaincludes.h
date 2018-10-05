#ifndef SKIAINCLUDES_H
#define SKIAINCLUDES_H

#undef foreach

#include "skiadefines.h"

#include "GrBackendSurface.h"
#include "GrContext.h"
#include "SDL.h"
#include "SkCanvas.h"
#include "SkRandom.h"
#include "SkSurface.h"
#include "SkGradientShader.h"
#include "SkPixelRef.h"
#include "SkDashPathEffect.h"
#include "SkTypeface.h"
#include "SkImageFilter.h"
#include "SkDiscretePathEffect.h"
#include "SkPathMeasure.h"

#include "gl/GrGLUtil.h"

#include "gl/GrGLTypes.h"
#include "gl/GrGLFunctions.h"
#include "gl/GrGLInterface.h"

#include <GL/gl.h>

#include "AddInclude/SkStroke.h"

#define foreach Q_FOREACH

extern sk_sp<SkImage> makeSkImageCopy(const sk_sp<SkImage>& img);

#endif // SKIAINCLUDES_H
