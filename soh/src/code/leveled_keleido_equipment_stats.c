#include "global.h"
#include "src/overlays/misc/ovl_kaleido_scope/z_kaleido_scope.h"
#include "textures/parameter_static/parameter_static.h"
#include "textures/icon_item_static/icon_item_static.h"

extern const char* _gAmmoDigit0Tex[];
void Leveled_Keleido_Vtx_DrawSmallNumber(PlayState* play, Vtx* vtx, u16 vtxIndex, u32 value, u8 maxDigits, u8 colorR, u8 colorG, u8 colorB) {
    PauseContext* pauseCtx = &play->pauseCtx;
    s8 digit[] = { 0, 0, 0, 0, 0, 0 };
    u8 digits;
    u8 spacing = 6;

    
    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_KAL_DISP++);

    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, colorR, colorG, colorB, pauseCtx->alpha);

    digits = 1;

    if (value >= 100000)
        digits += 1;
    if (value >= 10000)
        digits += 1;
    if (value >= 1000)
        digits += 1;
    if (value >= 100)
        digits += 1;
    if (value >= 10)
        digits += 1;

    if (digits > maxDigits)
        digits = maxDigits;

    s8 j;

    for (j = 0; value >= 100000; j++) {
        value -= 100000;
        digit[5] += 1;
    }
    for (j = 0; value >= 10000; j++) {
        value -= 10000;
        digit[4] += 1;
    }
    for (j = 0; value >= 1000; j++) {
        value -= 1000;
        digit[3] += 1;
    }
    for (j = 0; value >= 100; j++) {
        value -= 100;
        digit[2] += 1;
    }
    for (j = 0; value >= 10; j++) {
        value -= 10;
        digit[1] += 1;
    }
    digit[0] = value;

    for (u8 i = 0; i < digits; i++) {
        gDPPipeSync(POLY_KAL_DISP++);
        gSPVertex(POLY_KAL_DISP++, &vtx[vtxIndex + i * 4], 4, 0);

        /* gDPLoadTextureBlock(POLY_KAL_DISP++, _gAmmoDigit0Tex[digit[i]], G_IM_FMT_IA, G_IM_SIZ_8b, 8, 8, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);

        gSP1Quadrangle(POLY_KAL_DISP++, 0, 2, 3, 1, 0);*/

        POLY_KAL_DISP = KaleidoScope_QuadTextureIA8(POLY_KAL_DISP, _gAmmoDigit0Tex[digit[i]], 8, 8, 0);
    }
    
    CLOSE_DISPS(play->state.gfxCtx);
}

void Leveled_Keleido_DrawI8A(PlayState* play, u16 vtxIndex, void* texture, u8 width, u8 height, Color_RGB8* color) {
    PauseContext* pauseCtx = &play->pauseCtx;
    Mtx matrix;

    OPEN_DISPS(play->state.gfxCtx);

    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, color->r, color->g, color->b, pauseCtx->alpha);
    gDPPipeSync(POLY_KAL_DISP++);

    gSPVertex(POLY_KAL_DISP++, &play->pauseCtx.equipVtx[vtxIndex], 4, 0);
    POLY_KAL_DISP = KaleidoScope_QuadTextureIA8(POLY_KAL_DISP, texture, width, height, 0);

    CLOSE_DISPS(play->state.gfxCtx);
}

void Leveled_Keleido_Draw32(PlayState* play, u16 vtxIndex, void* texture, u8 width, u8 height, Color_RGB8* color) {
    PauseContext* pauseCtx = &play->pauseCtx;
    Mtx matrix;

    OPEN_DISPS(play->state.gfxCtx);

    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, color->r, color->g, color->b, pauseCtx->alpha);
    gDPPipeSync(POLY_KAL_DISP++);

    gSPVertex(POLY_KAL_DISP++, &play->pauseCtx.equipVtx[vtxIndex], 4, 0);
    KaleidoScope_DrawQuadTextureRGBA32(play->state.gfxCtx, texture, width, height, 0);

    CLOSE_DISPS(play->state.gfxCtx);
}

void Leveled_Keleido_EquipStats_Draw(PlayState* play) {
    u16 vtxIndex = 0;
    u16 vtxIndexStart = 112;
    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetCombineLERP(POLY_KAL_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                      ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetCombineMode(POLY_KAL_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

    Leveled_Keleido_DrawI8A(play, vtxIndexStart + vtxIndex * 4, gHeartFullTex, 16, 16, &(Color_RGB8){ 255, 0, 0 });
    vtxIndex++;
    Leveled_Keleido_Draw32(play, vtxIndexStart + vtxIndex * 4, gItemIconSwordKokiriTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    vtxIndex++;

    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, 465, 4, 255, 255,
                                        255);

    CLOSE_DISPS(play->state.gfxCtx);
}

void Leveled_Keleido_CreateQuadVertexGroup(Vtx* vtxList, s32 xStart, s32 yStart, s32 width, s32 height, u8 flippedH,
                                           f32 scale) {
    Interface_CreateQuadVertexGroup(vtxList, xStart, yStart, width * scale, height * scale, flippedH);
    vtxList[0].v.tc[0] /= scale;
    vtxList[1].v.tc[0] /= scale;
    vtxList[2].v.tc[0] /= scale;
    vtxList[2].v.tc[1] /= scale;
    vtxList[3].v.tc[0] /= scale;
    vtxList[3].v.tc[1] /= scale;
}

void Leveled_Keleido_EquipStats_InitVertices(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;
    u16 vtxIndex = 112;
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], -20, -50, 16, 16, 0, 0.5f);
    vtxIndex += 4;
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], -10, -50, 32, 32, 0, 0.25f);
    vtxIndex += 4;
    for (vtxIndex; vtxIndex < 252; vtxIndex += 4) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], 130 - vtxIndex * 1.5f, -50, 8, 8, 0, 1.0f);
        //pauseCtx->equipVtx[vtxIndex + 0].v.ob[1] = pauseCtx->equipVtx[vtxIndex + 1].v.ob[1] += pauseCtx->offsetY;
    }
}