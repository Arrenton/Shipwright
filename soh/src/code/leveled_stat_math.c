#include "global.h"

static u32 sExpTable[] = { 0,      30,     65,     110,    167,    240,    334,    452,    598,    777,    993,
                           1251,   1555,   1910,   2320,   2791,   3329,   3937,   4621,   5388,   6242,   7189,
                           8235,   9386,   10647,  12026,  13528,  15160,  16929,  18840,  20902,  23120,  25501,
                           28054,  30784,  33700,  36809,  40119,  43637,  47371,  51329,  55520,  59951,  64630,
                           69567,  74770,  80248,  86008,  92061,  98416,  105080, 112065, 119379, 127032, 135033,
                           143392, 152119, 161225, 170719, 180611, 190912, 201632, 212783, 224374, 236417, 248922,
                           261901, 275365, 289325, 303792, 318779, 334297, 350358, 366973, 384155, 401916, 420268,
                           439224, 458796, 478997, 499839, 521336, 543501, 566346, 589885, 614131, 639098, 664799,
                           691249, 718461, 746448, 775226, 804807, 835208, 866441, 898523, 931466, 965287, 999999 };
/* Old table
{ 0,      40,     89,     154,    241,    355,    504,    694,    930,    1219,   1568,   1982,   2468,
  3033,   3681,   4420,   5257,   6196,   7245,   8409,   9695,   11110,  12659,  14349,  16185,  18175,
  20324,  22640,  25127,  27792,  30642,  33683,  36921,  40362,  44012,  47879,  51967,  56284,  60836,
  65628,  70668,  75961,  81513,  87332,  93422,  99792,  106445, 113390, 120632, 128177, 136032, 144203,
  152697, 161518, 170675, 180173, 190017, 200216, 210774, 221698, 232995, 244670, 256730, 269181, 282030,
  295282, 308944, 323022, 337522, 352451, 367815, 383621, 399873, 416580, 433746, 451379, 469484, 488067,
  507136, 526696, 546754, 567315, 588386, 609974, 632084, 654723, 677897, 701612, 725875, 750692, 776069,
  802012, 828528, 855623, 883303, 911574, 940443, 969916, 999999 };*/

u16 GetActorStat_DisplayAttack(u8 attack, u8 power) {
    return GetActorStat_Attack(attack, power) / (1 + (float)power / 30.0f);
}

u16 GetActorStat_Attack(u8 attack, u8 power) {
    //return attack * (1 + power / (10.0f / (1.0f + ((float)power / 15.0f)))); old

    return (float)attack * (1 + (power - 2) * (0.14f + (power - 2) * 0.0006f));
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
    //return (u16)Leveled_DamageFormula(baseHealth, GetActorStat_Power(level), GetActorStat_Courage(level)); old
    return GetActorStat_Attack(baseHealth, GetActorStat_Power(level));
}

u8 GetPlayerStat_BonusHearts(u8 level){
    return (level + 1) / 5;
}

u16 GetPlayerStat_GetModifiedHealthCapacity(u16 baseHealth, u8 level){
    s32 heartUnits = CVar_GetS32("gLeveledHeartUnits", 3) << 2;
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

f32 Leveled_DamageFormula(f32 attack, u8 power, u8 courage) {
    //return (float)GetActorStat_Attack(attack, power) / (1.0f + (float)courage / 16.0f);

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
    //return (float)GetActorStat_Attack(attack, power) / (1.0f + (float)courage / (10.0f / (1.0f + ((float)power / (16.0f - (float)courage / 35.0f))))); old
    f32 damage = attack;
    if (power >= courage) {
        for (u8 i = 0; i < power - courage; i++) {
            damage *= 1.05f + CLAMP_MIN(0.05f - (f32)i / 100.0f, 0);
        }
    } else {
        for (u8 i = 0; i < courage - power; i++) {
            damage *= 0.96f;
        }
    }
    return damage;
}

f32 Leveled_DamageModify(u8 actorCat, f32 attack, u8 power, u8 courage) {
    f32 damage;
    if (actorCat == ACTORCAT_PLAYER) {
        damage = Leveled_DamageFormulaOnPlayer(attack, power, courage);
    } else {
        damage = Leveled_DamageFormula(attack, power, courage);
    }

        
    if (damage >= 1.25f) 
        damage += Rand_ZeroOne() - 0.2f;

    if (damage >= 6)
        damage += Rand_ZeroFloat(damage * 0.12f) - damage * 0.06f;

    if (damage > 999)
        damage = 999;

    damage = (u16)(damage + 0.5f);

    return CLAMP_MIN(damage, attack > 0 ? 1 : 0);
}

u16 Leveled_GoldSkulltulaExperience(u8 tokens) {
    u16 experience = 5;
    u8 i;

    for (i = 0; i < tokens; i++) {
        experience += 5 + 5 * (u16)((f32)i / 10.0);
    }
    return experience;
}