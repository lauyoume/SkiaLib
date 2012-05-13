
/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include "SkCanvas.h"
#include "SkGraphics.h"
#include "SkImageDecoder.h"
#include "SkImageEncoder.h"
//#include "SkStream.h"
#include "SkString.h"
#include "SkTemplates.h"

#include "SkTypeface_win.h"
#include "SkUtils.h"

#include <windows.h>
#include <tchar.h>


static void show_help() {
    SkDebugf("usage: skhello [-o out-dir] [-t 'hello']\n  default output: skhello.png\n");
}

int main (int argc, char * const argv[]) {
    SkAutoGraphics ag;
    SkString path("skhello.png");
	char *str = NULL;

	
	size_t size = SkUTF16_ToUTF8((uint16_t*)L"Î¢ÈíÑÅºÚ",sizeof(L"Î¢ÈíÑÅºÚ"),str);

	str = SkNEW_ARRAY(char,size);
	ZeroMemory(str,size);

	SkUTF16_ToUTF8((uint16_t*)L"Î¢ÈíÑÅºÚ",sizeof(L"Î¢ÈíÑÅºÚ"),str);


    SkString text(str);

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help")) {
            show_help();
            return 0;
        }
        if (!strcmp(argv[i], "-o")) {
            if (i == argc-1) {
                SkDebugf("ERROR: -o needs a following filename\n");
                return -1;
            }
            path.set(argv[i+1]);
            i += 1; // skip the out dir name
        } else if (!strcmp(argv[i], "-t")) {
            if (i == argc-1) {
                SkDebugf("ERROR: -t needs a following string\n");
                return -1;
            }
            text.set(argv[i+1]);
            i += 1; // skip the text string
        }
    }

	LOGFONT logFont;
	memset(&logFont,0,sizeof(LOGFONT));
	logFont.lfHeight = -24;
	strcpy(logFont.lfFaceName,"Î¢ÈíÑÅºÚ");
	SkTypeface *font = SkCreateTypefaceFromLOGFONT(logFont);
	
	if(!font){
		SkDebugf("--- failed to create font %s\n", logFont.lfFaceName);
		return -1;
	}

    SkPaint paint;

	paint.setTypeface(font);
    paint.setAntiAlias(true);
    paint.setTextSize(SkIntToScalar(30));
	paint.setTextEncoding(SkPaint::kUTF8_TextEncoding);

    SkScalar width = paint.measureText(text.c_str(), text.size());
    SkScalar spacing = paint.getFontSpacing();

    int w = width + 330;
    int h = paint.getFontSpacing() + 130;
    SkBitmap bitmap;
    bitmap.setConfig(SkBitmap::kARGB_8888_Config, w, h);
    bitmap.allocPixels();

    SkCanvas canvas(bitmap);
    canvas.drawColor(SK_ColorWHITE);
    
    paint.setTextAlign(SkPaint::kCenter_Align);
	

    canvas.drawText(text.c_str(), text.size(),
                    SkIntToScalar(w)/2, SkIntToScalar(h)*2/3,
                    paint);

	SkBitmap gif;

	SkImageDecoder* decoder = CreateGIFImageDecoder();
	SkDELETE(decoder);

	SkImageDecoder::DecodeFile("d:\\tt.gif",&gif);

	canvas.drawBitmap(gif,100,60,&paint);

    bool success = SkImageEncoder::EncodeFile(path.c_str(), bitmap,
                               SkImageEncoder::kPNG_Type, 100);
    if (!success) {
        SkDebugf("--- failed to write %s\n", path.c_str());
    }
    return !success;
}

