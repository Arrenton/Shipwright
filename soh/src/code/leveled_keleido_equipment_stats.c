#include "global.h"
#include "src/overlays/misc/ovl_kaleido_scope/z_kaleido_scope.h"
#include "textures/parameter_static/parameter_static.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/nes_font_static/nes_font_static.h"
#include "textures/icon_item_24_static/icon_item_24_static.h"
#include "textures/do_action_static/do_action_static.h"

extern const char* _gAmmoDigit0Tex[];
extern const char* digitTextures[];



void Leveled_Keleido_Draw4b(PlayState* play, u16 vtxIndex, void* texture, u8 width, u8 height, Color_RGB8* color) {
    PauseContext* pauseCtx = &play->pauseCtx;
    Mtx matrix;

    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, color->r, color->g, color->b, pauseCtx->alpha);
    gDPSetEnvColor(POLY_KAL_DISP++, 0, 0, 0, 0);

    gSPVertex(POLY_KAL_DISP++, &play->pauseCtx.equipVtx[vtxIndex], 4, 0);

    gDPLoadTextureBlock_4b(POLY_KAL_DISP++, texture, G_IM_FMT_I, width, height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                           G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSP1Quadrangle(POLY_KAL_DISP++, 0, 2, 3, 1, 0);

    CLOSE_DISPS(play->state.gfxCtx);
}
void Leveled_Keleido_DrawI8(PlayState* play, u16 vtxIndex, void* texture, u8 width, u8 height, Color_RGB8* color) {
    PauseContext* pauseCtx = &play->pauseCtx;
    Mtx matrix;

    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, color->r, color->g, color->b, pauseCtx->alpha);
    gDPSetEnvColor(POLY_KAL_DISP++, 0, 0, 0, 0);

    gSPVertex(POLY_KAL_DISP++, &play->pauseCtx.equipVtx[vtxIndex], 4, 0);

    gDPLoadTextureBlock(POLY_KAL_DISP++, texture, G_IM_FMT_I, G_IM_SIZ_8b, width, height, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSP1Quadrangle(POLY_KAL_DISP++, 0, 2, 3, 1, 0);

    CLOSE_DISPS(play->state.gfxCtx);
}

void Leveled_Keleido_DrawIA8(PlayState* play, u16 vtxIndex, void* texture, u8 width, u8 height, Color_RGB8* color) {
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

void Leveled_Keleido_Vtx_DrawSmallNumber(PlayState* play, Vtx* vtx, u16 vtxIndex, u32 value, u8 maxDigits, Color_RGB8* color) {
    PauseContext* pauseCtx = &play->pauseCtx;
    s8 digit[] = { 0, 0, 0, 0, 0, 0 };
    u8 digits;
    u8 spacing = 6;

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
        Leveled_Keleido_DrawIA8(play, vtxIndex + ((digits - 1) - i) * 4, _gAmmoDigit0Tex[digit[i]], 8, 8, color);
    }
}

void Leveled_Keleido_Vtx_DrawLargeNumber(PlayState* play, Vtx* vtx, u16 vtxIndex, u32 value, u8 maxDigits,
                                         Color_RGB8* color) {
    PauseContext* pauseCtx = &play->pauseCtx;
    s8 digit[] = { 0, 0, 0, 0, 0, 0 };
    u8 digits;
    u8 spacing = 6;

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
        Leveled_Keleido_DrawI8(play, vtxIndex + ((digits - 1) - i) * 4, digitTextures[digit[i]], 8, 16, color);
    }
}

void Leveled_Keleido_EquipStats_Draw(PlayState* play) {
    u16 vtxIndex = 0;
    u16 vtxIndexStart = 112;
    u8 attack = 1;
    Player* player = GET_PLAYER(play);
    Color_RGB8 textColor = { 255, 255, 255 };

    if (CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) == 1)
        attack = HEALTH_ATTACK_MULTIPLIER;
    if (CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) == 2)
        attack = HEALTH_ATTACK_MULTIPLIER << 1;
    if (CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) == 3) {
        attack = HEALTH_ATTACK_MULTIPLIER << 2;
        if (gBitFlags[3] & gSaveContext.inventory.equipment)
            attack = HEALTH_ATTACK_MULTIPLIER;
    }

    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetCombineLERP(POLY_KAL_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                      ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetTextureFilter(POLY_KAL_DISP++, G_TF_AVERAGE);

    Leveled_Keleido_Draw4b(play, vtxIndexStart + vtxIndex++ * 4, gMsgChar4CLatinCapitalLetterLTex, 16, 16, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw4b(play, vtxIndexStart + vtxIndex++ * 4, gMsgChar76LatinSmallLetterVTex, 16, 16, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_DrawIA8(play, vtxIndexStart + vtxIndex++ * 4, gHeartFullTex, 16, 16, &(Color_RGB8){ 255, 77, 55 });
    if (gSaveContext.magicCapacity > 0)
        Leveled_Keleido_Draw32(play, vtxIndexStart + vtxIndex++ * 4, gQuestIconMagicJarBigTex, 24, 24, &(Color_RGB8){ 255, 255, 255 });
    else
        vtxIndex++;
    Leveled_Keleido_Draw32(play, vtxIndexStart + vtxIndex++ * 4, gItemIconSwordKokiriTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw32(play, vtxIndexStart + vtxIndex++ * 4, gItemIconSilverGauntletsTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw32(play, vtxIndexStart + vtxIndex++ * 4, gItemIconShieldHylianTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw32(play, vtxIndexStart + vtxIndex++ * 4, gItemIconGoronsBraceletTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw4b(play, vtxIndexStart + vtxIndex++ * 4, gNextDoActionENGTex, 48, 16, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw4b(play, vtxIndexStart + vtxIndex++ * 4, gMsgChar2FSolidusTex, 16, 16, &(Color_RGB8){ 255, 255, 255 });
    if (gSaveContext.magicCapacity > 0)
        Leveled_Keleido_Draw4b(play, vtxIndexStart + vtxIndex++ * 4, gMsgChar2FSolidusTex, 16, 16, &(Color_RGB8){ 255, 255, 255 });
    else
        vtxIndex++;

    Leveled_Keleido_Vtx_DrawLargeNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, player->actor.level, 2, &(Color_RGB8){ 255, 255, 255 });
    vtxIndex += 2;
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, gSaveContext.health, 4, &(Color_RGB8){ 255, 255, 255 });
    vtxIndex += 4;
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, gSaveContext.healthCapacity2, 4, &(Color_RGB8){ 120, 255, 0 });
    vtxIndex += 4;
    if (gSaveContext.magicCapacity > 0) {
        Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, gSaveContext.magic, 3, &(Color_RGB8){ 255, 255, 255 });
        vtxIndex += 3;
        Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, gSaveContext.magicCapacity, 3, &(Color_RGB8){ 120, 255, 0 });
        vtxIndex += 3;
    } else {
        vtxIndex += 6;
    }
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, GetActorStat_Attack(attack, CLAMP(player->actor.power + player->actor.powerModifier, 0, 255)), 4, &(Color_RGB8){ 255, 255, 255 });
    vtxIndex += 4;
    if (player->actor.powerModifier > 0) {
        textColor.r = 120;
        textColor.g = 255;
        textColor.b = 0;
    } else if (player->actor.powerModifier < 0) {
        textColor.r = 255;
        textColor.g = 0;
        textColor.b = 0;
    }
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, CLAMP(player->actor.power + player->actor.powerModifier, 0, 255), 3, &textColor);
    vtxIndex += 3;
    if (player->actor.courageModifier > 0) {
        textColor.r = 120;
        textColor.g = 255;
        textColor.b = 0;
    } else if (player->actor.courageModifier < 0) {
        textColor.r = 255;
        textColor.g = 0;
        textColor.b = 0;
    }
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, CLAMP(player->actor.courage + player->actor.courageModifier, 0, 255), 3, &textColor);
    vtxIndex += 3;
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, gSaveContext.experience, 6, &(Color_RGB8){ 255, 255, 255 });
    vtxIndex += 6;
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, vtxIndexStart + vtxIndex * 4, GetActorStat_NextLevelExp(player->actor.level, gSaveContext.experience), 5, &(Color_RGB8){ 255, 255, 255 });
    vtxIndex += 5;

    

    CLOSE_DISPS(play->state.gfxCtx);
}

void Leveled_Keleido_CreateQuadVertexGroup(Vtx* vtxList, s32 xStart, s32 yStart, s32 width, s32 height, u8 flippedH, f32 scale) {
    vtxList[0].v.ob[0] = xStart;
    vtxList[0].v.ob[1] = yStart;
    vtxList[0].v.tc[0] = (flippedH ? width : 0) << 5;
    vtxList[0].v.tc[1] = 0 << 5;

    vtxList[1].v.ob[0] = xStart + width;
    vtxList[1].v.ob[1] = yStart;
    vtxList[1].v.tc[0] = (flippedH ? width * 2 : width) << 5;
    vtxList[1].v.tc[1] = 0 << 5;

    vtxList[2].v.ob[0] = xStart;
    vtxList[2].v.ob[1] = yStart - height;
    vtxList[2].v.tc[0] = (flippedH ? width : 0) << 5;
    vtxList[2].v.tc[1] = height << 5;

    vtxList[3].v.ob[0] = xStart + width;
    vtxList[3].v.ob[1] = yStart - height;
    vtxList[3].v.tc[0] = (flippedH ? width * 2 : width) << 5;
    vtxList[3].v.tc[1] = height << 5;

    if (scale == 1.0f)
        return;

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
    u16 xPos = -66;
    u16 yPos = -54 - pauseCtx->offsetY;
    u8 smallNumberSpacing = 6;

    // L
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[112], xPos + 2, -yPos - 6, 8, 8, false, 0.5f);
    // V
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[116], xPos + 5, -yPos - 6, 8, 8, false, 0.5f);
    // Lv Digits
    for (int i = 0, vtxIndex = 156; vtxIndex < 164; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 9) + i * 8, -yPos, 8, 16, false, 1.0f);
    }

    xPos = -64;
    yPos += 15;
    // Heart
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[120], xPos, -yPos, 8, 8, false, 0.5f);
    // Divider
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[148], xPos + 24, -yPos + 2, 8, 8, false, 1.0f);
    // Health Digits
    for (int i = 0, vtxIndex = 164; vtxIndex < 180; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }
    // Max Health Digits
    for (int i = 0, vtxIndex = 180; vtxIndex < 196; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 32) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    if (gSaveContext.magicCapacity > 0) {
        yPos += 8;
    }
    // Magic Pot
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[124], xPos, -yPos, 8, 8, false, 0.3f);
    // Divider
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[152], xPos + 24, -yPos + 2, 8, 8, false, 1.0f);
    // Magic Digits
    for (int i = 0, vtxIndex = 196; vtxIndex < 208; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }
    // Max Magic Digits
    for (int i = 0, vtxIndex = 208; vtxIndex < 220; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 32) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos += 8;
    // Sword
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[128], xPos - 1, -yPos + 2, 8, 8, false, 0.33f);
    // Attack Val Digits
    for (int i = 0, vtxIndex = 220; vtxIndex < 236; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos += 8;
    // Power Glove
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[132], xPos, -yPos, 8, 8, false, 0.25f);
    // Power Val Digits
    for (int i = 0, vtxIndex = 236; vtxIndex < 248; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos += 8;
    // Shield
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[136], xPos, -yPos, 8, 8, false, 0.25f);
    // Courage Val Digits
    for (int i = 0, vtxIndex = 248; vtxIndex < 260; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos = 46;
    // Bracelet
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[140], xPos, -yPos, 8, 8, false, 0.25f);
    // Experience Digits
    for (int i = 0, vtxIndex = 260; vtxIndex < 284; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos += 8;
    // Next
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[144], xPos - 6, -yPos + 1, 48, 16, false, 0.5f);
    // Next Lv Digits
    for (int i = 0, vtxIndex = 284; vtxIndex < 304; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 14) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }
}