#include "../GameStates/Credits.hpp"


namespace {
	Font titleFont;
	Font nameFont;
	f32 startY;
	f32 endY;
}

void Credits_Load() {
	titleFont.Load_Font(Fonts::Main, 25.f);
	nameFont.Load_Font(Fonts::Main, 50.f);

	//for every additional name \n\n one \n in name is \n\n in title 
	titleFont.Input_Text("By Orbital Candy\n\n\n\nDIRECTOR and PROGRAMMER\n\n\n\nPRODUCER and DESIGN LEAD\n\n\n\n TECHNICAL LEAD\n\n\n\n ART LEAD\n\n\n\n AUDIO LEAD\n\n\n\n GRAPHICS PROGRAMMER\n\n\n\n LEVEL DESIGNER\n\n\n\n ENVIRONMENT ARTIST AND UI\n\n\n\n COMPOSER\n\n\n\nSPECIAL THANKS\n\n\n\nFACULTY and ADVISORS\n\n\n\nPRESIDENT\n\n\n\nEXECUTIVES");

	nameFont.Input_Text("Cardinals");
	//"WWW.DIGIPEN.EDU"
	//"All content (C) 2025 Digipen Institute of Technology Singapore. All Rights Reserved", 800.f, 820.f

	//Gerald Wong\nSoroor Malekmohammadi Faradounbeh\nTommy Tan\n\n\n\nCreated at\n\n\n\n\n\n\nClaude Comair\n\nChu Jason Yeu Tat   Michael Gats\nTan Chek Ming   Prasanna Kumar Ghali\nMandy Wong   Johnny Deek"

	PRINT("Load: %s", "Credits\n");
}

void Credits_Initialize()
{
	startY = -430.f;
	PRINT("Initialize: %s", "Credits\n");
}

void Credits_Update() {

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.95f, 0.65f, 0.77f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
	startY += 5;
}

void Credits_Draw() {
	titleFont.Draw_Text(0.f, startY);
	nameFont.Draw_Text(0.f, startY + 37.5f);
}

void Credits_Free() {
	titleFont.Free_Font();
	nameFont.Free_Font();
	PRINT("Free: %s", "Credits\n");
}

void Credits_Unload() {

	PRINT("Unload: %s", "Credits\n");
}
