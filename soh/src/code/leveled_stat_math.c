#include "global.h"

static u32 sExpTable[] = { 0,      30,     64,     105,    155,    217,    292,    385,    497,    632,    792,
                           981,    1202,   1458,   1753,   2089,   2471,   2903,   3388,   3930,   4534,   5203,
                           5942,   6755,   7648,   8625,   9691,   10851,  12110,  13474,  14949,  16540,  18253,
                           20094,  22070,  24188,  26453,  28874,  31456,  34209,  37139,  40254,  43562,  47073,
                           50794,  54734,  58904,  63312,  67968,  72883,  78067,  83531,  89286,  95345,  101719,
                           108421, 115463, 122859, 130623, 138770, 147314, 156270, 165655, 175485, 185778, 196552,
                           207824, 219614, 231943, 244831, 258300, 272372, 287071, 302422, 318450, 335182, 352645,
                           370870, 389885, 409724, 430419, 452005, 474520, 498001, 522489, 548027, 574659, 602434,
                           631400, 661611, 693122, 725993, 760287, 796070, 833414, 872395, 913094, 955597, 999999 };
/* Old table
{ 0,      30,     65,     110,    167,    240,    334,    452,    598,    777,    993,
  1251,   1555,   1910,   2320,   2791,   3329,   3937,   4621,   5388,   6242,   7189,
  8235,   9386,   10647,  12026,  13528,  15160,  16929,  18840,  20902,  23120,  25501,
  28054,  30784,  33700,  36809,  40119,  43637,  47371,  51329,  55520,  59951,  64630,
  69567,  74770,  80248,  86008,  92061,  98416,  105080, 112065, 119379, 127032, 135033,
  143392, 152119, 161225, 170719, 180611, 190912, 201632, 212783, 224374, 236417, 248922,
  261901, 275365, 289325, 303792, 318779, 334297, 350358, 366973, 384155, 401916, 420268,
  439224, 458796, 478997, 499839, 521336, 543501, 566346, 589885, 614131, 639098, 664799,
  691249, 718461, 746448, 775226, 804807, 835208, 866441, 898523, 931466, 965287, 999999 };*/

u8 Leveled_GetHealthAttackMultiplier() {
    return CLAMP(CVarGetInteger("gLeveled.Enhancements.AttackAndHPMultiplier", 9), 1, 32);
}

u16 GetActorStat_DisplayAttack(u16 attack, u8 power) {
    return GetActorStat_Attack(attack, power) / (1 + (float)power / 30.0f);
}

u16 GetActorStat_Attack(u16 attack, u8 power) {
    return (float)attack * (1 + (power - 2) * (0.14f + (power - 2) * 0.0006f));
}

f32 GetActorStat_EnemyAttack(u16 attack, u8 power) {
    return (float)attack * (1 + (power - 2) * (0.007f + (power - 2) * 0.0002f));
}

u8 GetActorStat_Power(u8 level) {
    return 3 + (u8)(84 * level / 99.0f);
}

u8 GetActorStat_Courage(u8 level) {
    return 2 + (u8)(75 * level / 99.0f);
}

u8 GetActorStat_PlayerPower(u8 level) {
    return 2 + (u8)(75 * level / 99.0f);
}

u8 GetActorStat_PlayerCourage(u8 level) {
    return 3 + (u8)(84 * level / 99.0f);
}

u16 GetActorStat_EnemyMaxHealth(u16 baseHealth, u8 level){ 
    return (u16)(CLAMP((f32)GetActorStat_Attack(baseHealth * Leveled_GetHealthAttackMultiplier(), GetActorStat_PlayerPower(level)) * CVarGetFloat("gLeveled.Difficulty.Enemy.HPPercent", 1.0f), 1, 0xffff));
}

u8 GetPlayerStat_BonusHearts(u8 level){
    if (CVarGetInteger("gLeveled.Player.Enhancements.HeartsWithLevelUp", 1) == 0){
        return 0;
    }

    u8 bonusHearts = (level + 1) / 8;
    if (bonusHearts > 10){
        bonusHearts = 10;
    }
    return bonusHearts;
}

u8 GetPlayerStat_MagicUnits(u8 level){
    if (CVarGetInteger("gLeveled.Player.Enhancements.MagicWithLevelUp", 1) == 0){
        return 48;
    }

    u8 maximumMagic = 12 + (u8)((f32)level / 2.8f) * 2;
    if (maximumMagic > 72){
        maximumMagic = 72;
    }
    return maximumMagic;
}

u16 GetPlayerStat_GetModifiedHealthCapacity(u16 baseHealth, u8 level){
    s32 heartUnits = CVarGetInteger("gLeveled.Difficulty.HeartUnits", 4) << 2;
    u16 baseHearts = baseHealth / 16;
    return (baseHearts + GetPlayerStat_BonusHearts(level)) * heartUnits;
}

u16 GetPlayerStat_NextLevelExpAtLevel(u8 level) {
    if (level == 99)
        return 0;

    s32 nextLv = sExpTable[level] - sExpTable[level - 1];
}

u16 GetActorStat_NextLevelExp(u8 level, u32 currentExp) {
    if (level == 99)
        return 0;

    s32 nextLv = sExpTable[level] - currentExp;

    if (nextLv < 0)
        nextLv = 0;
    return nextLv;
}

u16 GetEnemyExperienceReward(u8 level, u16 expRate) {
    if (expRate == 0)
        return 0;

    return CLAMP(round((3 + CLAMP_MAX(floor(level / 6) * 4, 4) + (level - 1) * ((0.1 + (level / 95.0)) + pow(CLAMP_MIN(level - 8, 0), 1.25) / 80.0)) * expRate / 100.0), 1, 9999);
}

f32 Leveled_DamageFormula(f32 attack, u8 power, u8 courage) {
    f32 damage = GetActorStat_Attack(attack, power);
    if (power >= courage) {
        for (u8 i = 0; i < power - courage; i++) {
            damage *= 1.01f;
        }
    } else {
        for (u8 i = 0; i < courage - power; i++) {
            damage *= 0.97f;
        }
    }
    return damage;
}

f32 Leveled_DamageFormulaOnPlayer(f32 attack, u8 power, u8 courage) {
    f32 damage = attack;
    
    if (CVarGetInteger("gLeveled.Enemy.Enhancements.AttackScalesWithLevel", 1) == 1){
        damage = GetActorStat_EnemyAttack(attack, power);

        if (power >= courage) {
            for (u8 i = 0; i < power - courage; i++) {
                f32 multAddition = CLAMP_MIN((0.07f - (power * 0.0005f)) - (f32)i / (100.0f - (power * 0.33f)), 0);
                damage *= 1.04f + multAddition;
            }
        } else {
            for (u8 i = 0; i < courage - power; i++) {
                damage *= 0.96f;
            }
        }
    } else {
        if (power >= courage) {
            for (u8 i = 0; i < power - courage; i++) {
                damage *= 1.05f + CLAMP_MIN(0.05f - (f32)i / 100.0f, 0);
            }
        } else {
            for (u8 i = 0; i < courage - power; i++) {
                damage *= 0.96f;
            }
        }
    }
    return damage;
}

f32 Leveled_DamageModify(Actor* actor, Actor* attackingActor, f32 attack) {
    f32 damage;
    if (actor->category == ACTORCAT_PLAYER) {
        damage = Leveled_DamageFormulaOnPlayer(attack, CLAMP(attackingActor->power + attackingActor->powerModifier, 0, 255), CLAMP(actor->courage + actor->courageModifier, 0, 255));
    } else {
        damage = Leveled_DamageFormula(attack, CLAMP(attackingActor->power + attackingActor->powerModifier, 0, 255), CLAMP(actor->courage + actor->courageModifier, 0, 255));
    }

        
    if (damage >= 1.25f) 
        damage += Rand_ZeroOne() - 0.2f;

    if (damage >= 6)
        damage += Rand_ZeroFloat(damage * 0.12f) - damage * 0.06f;

    if (damage > 9999)
        damage = 9999;

    damage = (u16)(damage + 0.5f);

    return CLAMP_MIN(damage, attack > 0 ? 1 : 0);
}

u16 Leveled_GoldSkulltulaExperience(u8 tokens) {
    u16 experience = 5;
    u8 i;

    for (i = 0; i < tokens; i++) {
        experience += 5 + 5 * i / (f32)10.0;
    }
    return (u16)((f32)experience * CVarGetFloat("gLeveled.Difficulty.EXP.TokenRate", 1.0f));
}

void Leveled_SetPlayerModifiedStats(Player* player) {
    s8 powerModifier = 0;
    s8 courageModifier = 0;

    if (CVarGetInteger("gLeveled.Player.Enhancements.EquipmentStats", 1) == 1){
        switch (CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD)){
            case PLAYER_SWORD_MASTER:
                courageModifier += 1;
                break;

            case PLAYER_SWORD_BIGGORON:
                if (gBitFlags[3] & gSaveContext.inventory.equipment){
                    powerModifier -= 7;
                    courageModifier -= 12;
                } else {
                    powerModifier += 2;
                    courageModifier -= 8;
                }
                break;

            default:
                break;
        }
        
        switch (CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC) - 1){
            case PLAYER_TUNIC_GORON:
                powerModifier += 3;
                courageModifier -= 3;
                break;
            
            case PLAYER_TUNIC_ZORA:
                powerModifier -= 3;
                courageModifier += 3;
                break;

            default:
                break;
        }
        
        switch (CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD)){
            case PLAYER_SHIELD_DEKU:
                courageModifier += 1;
                break;

            case PLAYER_SHIELD_HYLIAN:
                courageModifier += 2;
                break;
            
            case PLAYER_SHIELD_MIRROR:
                powerModifier -= 2;
                courageModifier += 3;
                break;

            default:
                break;
        }
        
        switch (CUR_EQUIP_VALUE(EQUIP_TYPE_BOOTS) - 1){
            case PLAYER_BOOTS_IRON:
                powerModifier += 2;
                courageModifier += 1;
                break;
            
            case PLAYER_BOOTS_HOVER:
                courageModifier -= 1;
                break;

            default:
                break;
        }

        powerModifier += Player_GetStrength();
    }

    player->actor.powerModifier = powerModifier;
    player->actor.courageModifier = courageModifier;
}