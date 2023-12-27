#include "global.h"
#include "src/overlays/misc/ovl_kaleido_scope/z_kaleido_scope.h"
#include "textures/parameter_static/parameter_static.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/nes_font_static/nes_font_static.h"
#include "textures/icon_item_24_static/icon_item_24_static.h"
#include "textures/do_action_static/do_action_static.h"

extern const char* _gAmmoDigit0Tex[];
extern const char* digitTextures[];

typedef enum {
    LEVEL_L = 112,
    LEVEL_V = 116,
    HEALTH_ICON = 120,
    MAGIC_ICON = 124,
    ATTACK_ICON = 128,
    POWER_ICON = 132,
    COURAGE_ICON = 136,
    EXP_ICON = 140,
    NEXTLV_ICON = 144,
    HEALTH_DIVIDER = 148,
    MAGIC_DIVIDER = 152,
    LEVEL_DIGIT_1 = 156,
    LEVEL_DIGIT_2 = 160,
    HEALTH_DIGIT_1 = 164,
    HEALTH_DIGIT_2 = 168,
    HEALTH_DIGIT_3 = 172,
    HEALTH_DIGIT_4 = 176,
    MAXHEALTH_DIGIT_1 = 180,
    MAXHEALTH_DIGIT_2 = 184,
    MAXHEALTH_DIGIT_3 = 188,
    MAXHEALTH_DIGIT_4 = 192,
    MAGIC_DIGIT_1 = 196,
    MAGIC_DIGIT_2 = 200,
    MAGIC_DIGIT_3 = 204,
    MAXMAGIC_DIGIT_1 = 208,
    MAXMAGIC_DIGIT_2 = 212,
    MAXMAGIC_DIGIT_3 = 216,
    ATTACK_DIGIT_1 = 220,
    ATTACK_DIGIT_2 = 224,
    ATTACK_DIGIT_3 = 228,
    ATTACK_DIGIT_4 = 232,
    POWER_DIGIT_1 = 236,
    POWER_DIGIT_2 = 240,
    POWER_DIGIT_3 = 244,
    COURAGE_DIGIT_1 = 248,
    COURAGE_DIGIT_2 = 252,
    COURAGE_DIGIT_3 = 256,
    EXP_DIGIT_1 = 260,
    EXP_DIGIT_2 = 264,
    EXP_DIGIT_3 = 268,
    EXP_DIGIT_4 = 272,
    EXP_DIGIT_5 = 276,
    EXP_DIGIT_6 = 280,
    NEXTLV_DIGIT_1 = 284,
    NEXTLV_DIGIT_2 = 288,
    NEXTLV_DIGIT_3 = 292,
    NEXTLV_DIGIT_4 = 296,
    NEXTLV_DIGIT_5 = 300
} ElementVtxIndex;


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

    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, color->r, color->g, color->b, pauseCtx->alpha);

    gSPVertex(POLY_KAL_DISP++, &play->pauseCtx.equipVtx[vtxIndex], 4, 0);
    POLY_KAL_DISP = KaleidoScope_QuadTextureIA8(POLY_KAL_DISP, texture, width, height, 0);

    CLOSE_DISPS(play->state.gfxCtx);
}

void Leveled_Keleido_Draw32(PlayState* play, u16 vtxIndex, void* texture, u8 width, u8 height, Color_RGB8* color) {
    PauseContext* pauseCtx = &play->pauseCtx;
    Mtx matrix;

    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, color->r, color->g, color->b, pauseCtx->alpha);

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

    Leveled_Keleido_Draw4b(play, LEVEL_L, gMsgChar4CLatinCapitalLetterLTex, 16, 16, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw4b(play, LEVEL_V, gMsgChar76LatinSmallLetterVTex, 16, 16, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_DrawIA8(play, HEALTH_ICON, gHeartFullTex, 16, 16, &(Color_RGB8){ 255, 77, 55 });
    Leveled_Keleido_Draw32(play, MAGIC_ICON, gQuestIconMagicJarBigTex, 24, 24, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw32(play, ATTACK_ICON, gItemIconSwordKokiriTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw32(play, POWER_ICON, gItemIconSilverGauntletsTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw32(play, COURAGE_ICON, gItemIconShieldHylianTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw32(play, EXP_ICON, gItemIconGoronsBraceletTex, 32, 32, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw4b(play, NEXTLV_ICON, gNextDoActionENGTex, 48, 16, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw4b(play, HEALTH_DIVIDER, gMsgChar2FSolidusTex, 16, 16, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Draw4b(play, MAGIC_DIVIDER, gMsgChar2FSolidusTex, 16, 16, &(Color_RGB8){ 255, 255, 255 });

    Leveled_Keleido_Vtx_DrawLargeNumber(play, play->pauseCtx.equipVtx, LEVEL_DIGIT_1, player->actor.level, 2, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, HEALTH_DIGIT_1, gSaveContext.health, 4, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, MAXHEALTH_DIGIT_1, gSaveContext.healthCapacity2, 4, &(Color_RGB8){ 120, 255, 0 });
    if (gSaveContext.magicCapacity > 0) {
        Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, MAGIC_DIGIT_1, gSaveContext.magic, 3, &(Color_RGB8){ 255, 255, 255 });
        Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, MAXMAGIC_DIGIT_1, gSaveContext.magicCapacity, 3, &(Color_RGB8){ 120, 255, 0 });
    }
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, ATTACK_DIGIT_1, GetActorStat_Attack(attack, CLAMP(player->actor.power + player->actor.powerModifier, 0, 255)), 4, &(Color_RGB8){ 255, 255, 255 });
    if (player->actor.powerModifier > 0) {
        textColor.r = 120;
        textColor.g = 255;
        textColor.b = 0;
    } else if (player->actor.powerModifier < 0) {
        textColor.r = 255;
        textColor.g = 0;
        textColor.b = 0;
    }
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, POWER_DIGIT_1, CLAMP(player->actor.power + player->actor.powerModifier, 0, 255), 3, &textColor);
    if (player->actor.courageModifier > 0) {
        textColor.r = 120;
        textColor.g = 255;
        textColor.b = 0;
    } else if (player->actor.courageModifier < 0) {
        textColor.r = 255;
        textColor.g = 0;
        textColor.b = 0;
    }
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, COURAGE_DIGIT_1, CLAMP(player->actor.courage + player->actor.courageModifier, 0, 255), 3, &textColor);
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, EXP_DIGIT_1, gSaveContext.experience, 6, &(Color_RGB8){ 255, 255, 255 });
    Leveled_Keleido_Vtx_DrawSmallNumber(play, play->pauseCtx.equipVtx, NEXTLV_DIGIT_1, GetActorStat_NextLevelExp(player->actor.level, gSaveContext.experience), 5, &(Color_RGB8){ 255, 255, 255 });

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
    u8 i = 0;
    s16 xPos = -66;
    s16 yPos = -54 - pauseCtx->offsetY;
    u8 smallNumberSpacing = 6;

    // L
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[LEVEL_L], xPos + 2, -yPos - 6, 8, 8, false, 0.5f);
    // V
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[LEVEL_V], xPos + 5, -yPos - 6, 8, 8, false, 0.5f);
    // Lv Digits
    for (i = 0, vtxIndex = LEVEL_DIGIT_1; vtxIndex <= LEVEL_DIGIT_2; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 9) + i * 8, -yPos, 8, 16, false, 1.0f);
    }

    xPos = -64;
    yPos += 15;
    // Heart
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[HEALTH_ICON], xPos, -yPos, 8, 8, false, 0.5f);
    // Divider
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[HEALTH_DIVIDER], xPos + 24, -yPos + 2, 8, 8, false, 1.0f);
    // Health Digits
    for (i = 0, vtxIndex = HEALTH_DIGIT_1; vtxIndex <= HEALTH_DIGIT_4; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }
    // Max Health Digits
    for (i = 0, vtxIndex = MAXHEALTH_DIGIT_1; vtxIndex <= MAXHEALTH_DIGIT_4; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 32) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    if (gSaveContext.magicCapacity > 0) {
        yPos += 8;
    }
    // Magic Pot
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[MAGIC_ICON], xPos, -yPos, 8, 8, false, 0.3f);
    // Divider
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[MAGIC_DIVIDER], xPos + 24, -yPos + 2, 8, 8, false, 1.0f);
    // Magic Digits
    for (i = 0, vtxIndex = MAGIC_DIGIT_1; vtxIndex <= MAGIC_DIGIT_3; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }
    // Max Magic Digits
    for (i = 0, vtxIndex = MAXMAGIC_DIGIT_1; vtxIndex <= MAXMAGIC_DIGIT_3; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 32) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos += 8;
    // Sword
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[ATTACK_ICON], xPos - 1, -yPos + 2, 8, 8, false, 0.33f);
    // Attack Val Digits
    for (i = 0, vtxIndex = ATTACK_DIGIT_1; vtxIndex <= ATTACK_DIGIT_4; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos += 8;
    // Power Glove
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[POWER_ICON], xPos, -yPos, 8, 8, false, 0.25f);
    // Power Val Digits
    for (i = 0, vtxIndex = POWER_DIGIT_1; vtxIndex <= POWER_DIGIT_3; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos += 8;
    // Shield
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[COURAGE_ICON], xPos, -yPos, 8, 8, false, 0.25f);
    // Courage Val Digits
    for (i = 0, vtxIndex = COURAGE_DIGIT_1; vtxIndex <= COURAGE_DIGIT_3; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos = 46 - pauseCtx->offsetY;
    // Bracelet
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[EXP_ICON], xPos, -yPos, 8, 8, false, 0.25f);
    // Experience Digits
    for (i = 0, vtxIndex = EXP_DIGIT_1; vtxIndex <= EXP_DIGIT_6; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 8) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }

    yPos += 8;
    // Next
    Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[NEXTLV_ICON], xPos - 6, -yPos + 1, 48, 16, false, 0.5f);
    // Next Lv Digits
    for (i = 0, vtxIndex = NEXTLV_DIGIT_1; vtxIndex <= NEXTLV_DIGIT_5; vtxIndex += 4, i++) {
        Leveled_Keleido_CreateQuadVertexGroup(&pauseCtx->equipVtx[vtxIndex], (xPos + 14) + i * smallNumberSpacing, -yPos, 8, 8, false, 1.0f);
    }
}