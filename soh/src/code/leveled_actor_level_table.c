#include "global.h"

typedef struct {
    s8 levelModifier;
    u16 experienceRate;
    u8 ignoreEntry;
} SceneLevelEntry;

/////////////////////////////////////////////////
//////////////////  BOSSES  /////////////////////
/////////////////////////////////////////////////
void Leveled_Gohma(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 7;
    actor->exp = 120;
    levelEntry->levelModifier = 3;
    levelEntry->experienceRate = 3000;
    
    if (actor->category == ACTORCAT_ENEMY) { // Larva
        actor->level = 5;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 0;
        if (actor->params < 10) {
            actor->exp = 5;
            levelEntry->experienceRate = 100;
        }
    }
}

void Leveled_Dodongo(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->category == ACTORCAT_BOSS) { // King Dodongo
        actor->level = 15;
        actor->exp = 500;
        levelEntry->levelModifier = 5;
        levelEntry->experienceRate = 4000;
    } else {
        actor->level = 14;
        actor->exp = 18;
        levelEntry->levelModifier = 4;
        levelEntry->experienceRate = 160;
    }
}

void Leveled_Barinade(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 21;
    levelEntry->levelModifier = 5;
    if (actor->params == -1) { // Body
        actor->exp = 850;
        levelEntry->experienceRate = 5000;
    }
}

void Leveled_PhantomGanon(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 28;
    levelEntry->levelModifier = 5;
    if (actor->params == 1)
        actor->exp = 1400;
        levelEntry->experienceRate = 6000;
}

void Leveled_Volvagia(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 35;
    levelEntry->levelModifier = 5;
    if (actor->id == ACTOR_BOSS_FD2)
        actor->exp = 2475;
        levelEntry->experienceRate = 6300;
}

void Leveled_Morpha(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 40;
    actor->exp = 3500;
    levelEntry->levelModifier = 5;
    levelEntry->experienceRate = 6600;
}

void Leveled_BongoBongo(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 45;
    actor->exp = 4666;
    levelEntry->levelModifier = 5;
    levelEntry->experienceRate = 7000;
}

void Leveled_Twinrova(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 52;
    levelEntry->levelModifier = 6;
    if (actor->params == 2)
        actor->exp = 6000;
        levelEntry->experienceRate = 7000;
}

void Leveled_Ganondorf(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 56;
    actor->exp = 9999;
    levelEntry->levelModifier = 5;
    levelEntry->experienceRate = 40000;
}

void Leveled_Ganon(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 60;
    levelEntry->levelModifier = 8;
}

/////////////////////////////////////////////////
///////////////  MINI-BOSSES  ///////////////////
/////////////////////////////////////////////////
void Leveled_BigOctorock(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 20;
    actor->exp = 320;
    levelEntry->levelModifier = 4;
    levelEntry->experienceRate = 2100;
}

void Leveled_FlareDancer(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 34;
    levelEntry->levelModifier = 4;
    if (actor->id == ACTOR_EN_FW) {
        actor->exp = 1000;
        levelEntry->experienceRate = 3000;
    }
}

void Leveled_DarkLink(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = GET_PLAYER(play)->actor.level;
    actor->exp = (u16)((f32)GetPlayerStat_NextLevelExpAtLevel(GET_PLAYER(play)->actor.level) * 0.50f);
    levelEntry->ignoreEntry = true;
}

void Leveled_DeadHand(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 44;
    levelEntry->levelModifier = 4;

    if (actor->id == ACTOR_EN_DH) {
        actor->exp = 2465;
        levelEntry->experienceRate = 3666;
    }
}

void Leveled_IronKnuckle(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 50;
    actor->exp = 2733;
    levelEntry->levelModifier = 5;
    levelEntry->experienceRate = 5000;

    if (actor->params == 0) {
        actor->level = 52;
        actor->exp = 3100;
        levelEntry->levelModifier = 6;
    } 
    if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
        actor->level = 55;
        actor->exp = 2850;
    }
}
/////////////////////////////////////////////////
//////////////  NORMAL ENEMIES  /////////////////
/////////////////////////////////////////////////
void Leveled_Dekubaba(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params == 1) { // Big baba
        actor->level = 24;
        actor->exp = 32;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 116;
        if (LINK_IS_CHILD) {
            actor->level = 5;
            actor->exp = 8;
        }
    } else {
        actor->level = 2;
        actor->exp = 3;
        levelEntry->levelModifier = -2;
        levelEntry->experienceRate = 60;
        if (play->sceneNum == SCENE_FOREST_TEMPLE) {
            actor->level = 23;
            actor->exp = 19;
        }
    }
}

void Leveled_StickDekuBaba(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 1;
    actor->exp = 1;
    levelEntry->levelModifier = -99;
    levelEntry->experienceRate = 1;
}

void Leveled_Skulltula(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 3;
    actor->exp = 3;
    levelEntry->levelModifier = 0;
    levelEntry->experienceRate = 80;
    if (play->sceneNum == SCENE_FOREST_TEMPLE) {
        actor->level = 21;
        actor->exp = 19;
    }
    if (play->sceneNum == SCENE_BOTTOM_OF_THE_WELL ||
        play->sceneNum == SCENE_SHADOW_TEMPLE) {
        actor->level = 38;
        actor->exp = 30;
    } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
        actor->level = 44;
        actor->exp = 34;
    } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
        actor->level = 50;
        actor->exp = 50;
    }
    if (actor->params == 1) { // Big
        actor->level += 3;
        actor->exp += 2;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 90;
    }
}

void Leveled_Wall_Skulltula(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (((play, actor->params & 0xE000) >> 0xD) != 0) { // Gold Skulltula
        actor->level = gSaveContext.inventory.gsTokens + 1;
        actor->exp = 0;
        levelEntry->ignoreEntry = true;
    } else {
        actor->level = 3;
        actor->exp = 2;
        levelEntry->levelModifier = -2;
        levelEntry->experienceRate = 40;
        if (play->sceneNum == SCENE_FOREST_TEMPLE) {
            actor->level = 24;
            actor->exp = 18;
        }
    }
}

void Leveled_DekuScrub(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 8;
    actor->exp = 7;
    levelEntry->levelModifier = 0;
    levelEntry->experienceRate = 75;
}

void Leveled_Tektite(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params == -2) { // Blue
        actor->level = 12;
        actor->exp = 12;
        levelEntry->levelModifier = 2;
        levelEntry->experienceRate = 100;
        if (LINK_IS_ADULT) {
            actor->level = 25;
            actor->exp = 21;
        }
        if (play->sceneNum == SCENE_WATER_TEMPLE) {
            actor->level = 36;
            actor->exp = 44;
        }
    } else {
        actor->level = 9;
        actor->exp = 9;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 90;
        if (LINK_IS_ADULT) {
            actor->level = 27;
            actor->exp = 24;
        }
    }
}

void Leveled_Guay(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 8;
    actor->exp = 4;
    levelEntry->levelModifier = -1;
    levelEntry->experienceRate = 50;
    if (LINK_IS_ADULT) {
        actor->level = 21;
        actor->exp = 13;
    }
}

void Leveled_Octorock(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 13;
    actor->exp = 12;
    levelEntry->levelModifier = 0;
    levelEntry->experienceRate = 70;
    if (actor->params != 0) {
        actor->level = 0;
        actor->exp = 0;
        levelEntry->levelModifier = 0;
        levelEntry->experienceRate = 0;
    }
}

void Leveled_Armos(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 12;
    actor->exp = 17;
    levelEntry->levelModifier = 2;
    levelEntry->experienceRate = 135;
    if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
        actor->level = 45;
        actor->exp = 60;
    } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) { // Ganon's Castle
        actor->level = 50;
        actor->exp = 70;
    }
}

void Leveled_BabyDodongo(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 11;
    actor->exp = 7;
    levelEntry->levelModifier = 1;
    levelEntry->experienceRate = 75;
}

void Leveled_Keese(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params == 4) { // Ice Keese
        actor->level = 34;
        actor->exp = 33;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 60;
    } else {
        actor->level = 9;
        actor->exp = 5;
        levelEntry->levelModifier = -1;
        levelEntry->experienceRate = 50;

        if (play->sceneNum == SCENE_FIRE_TEMPLE) {
            actor->level = 28;
            actor->exp = 22;
        } else if (play->sceneNum == SCENE_ICE_CAVERN) {
            actor->level = 31;
            actor->exp = 26;
        } else if (play->sceneNum == SCENE_WATER_TEMPLE) {
            actor->level = 34;
            actor->exp = 29;
        } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
            actor->level = 42;
            actor->exp = 36;
        } else if (play->sceneNum == SCENE_BOTTOM_OF_THE_WELL ||
            play->sceneNum == SCENE_SHADOW_TEMPLE) {
            actor->level = 38;
            actor->exp = 33;
        } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
            actor->level = 50;
            actor->exp = 38;
        }
        if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
            actor->level = 47;
            actor->exp = 80;
        }
    }
}

void Leveled_Peahat(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 18;
    actor->exp = 47;
    levelEntry->levelModifier = 10;
    levelEntry->experienceRate = 275;
    if (actor->params == 1) { // Larva
        actor->level = 13;
        levelEntry->levelModifier = 5;
        levelEntry->experienceRate = 0;
    }
}
void Leveled_Poh(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 10;
    actor->exp = 14;
    levelEntry->levelModifier = 0;
    levelEntry->experienceRate = 130;
    if (actor->params == 2 || actor->params == 3) { // Composer
        actor->level = 15;
        actor->exp = 25;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 130;
    }
}

void Leveled_Poh_Field(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 24;
    actor->exp = 25;
    levelEntry->levelModifier = 0;
    levelEntry->experienceRate = 130;
}

void Leveled_ReDead(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params >= -1) {
        actor->level = 25;
        actor->exp = 45;
        levelEntry->levelModifier = 2;
        levelEntry->experienceRate = 280;
        if (play->sceneNum == SCENE_BOTTOM_OF_THE_WELL || play->sceneNum == SCENE_SHADOW_TEMPLE) {
            actor->level = 42;
            actor->exp = 171;
        } else if (play->sceneNum == SCENE_GANONS_TOWER_COLLAPSE_INTERIOR || play->sceneNum == SCENE_INSIDE_GANONS_CASTLE_COLLAPSE) {
            actor->level = 50;
            actor->exp = 230;
        }
    } else { // Gibdo
        actor->level = 43;
        actor->exp = 183;
        levelEntry->levelModifier = 3;
        levelEntry->experienceRate = 305;
    }
}

void Leveled_Shabom(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 15;
    levelEntry->levelModifier = -1;
}

void Leveled_Bari(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 18;
    actor->exp = 25;
    levelEntry->levelModifier = 2;
    levelEntry->experienceRate = 85;
}

void Leveled_Biri(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 16;
    actor->exp = 17;
    levelEntry->levelModifier = 1;
    levelEntry->experienceRate = 100;
}

void Leveled_Stinger(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 17;
    actor->exp = 19;
    levelEntry->levelModifier = 1;
    levelEntry->experienceRate = 125;
    if (play->sceneNum == SCENE_WATER_TEMPLE) {
        actor->level = 37;
        actor->exp = 85;
    }
}

void Leveled_JabuTentacle(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 19;
    levelEntry->levelModifier = 3;
    if (actor->params < 3) {
        actor->exp = 61;
        levelEntry->experienceRate = 400;
    }
}

void Leveled_Tailpasaran(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 19;
    actor->exp = 22;
    levelEntry->levelModifier = 3;
    levelEntry->experienceRate = 125;
}

void Leveled_Stalchild(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 6 + (u16)(play, actor->params / 2);
    actor->exp = 3 + (u16)(play, actor->params / 2.5);
    levelEntry->levelModifier = 1 + (u16)(play, actor->params / 2);
    levelEntry->experienceRate = 45 + (u16)(play, actor->params * 5);
}

void Leveled_Beamos(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params == 0) { // Big
        actor->level = 48;
        actor->exp = 90;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 150;
    } else {
        actor->level = 11;
        actor->exp = 17;
        levelEntry->levelModifier = 0;
        levelEntry->experienceRate = 150;
        if (play->sceneNum == SCENE_BOTTOM_OF_THE_WELL ||
            play->sceneNum == SCENE_SHADOW_TEMPLE) {
            actor->level = 43;
            actor->exp = 80;
        } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
            actor->level = 47;
            actor->exp = 87;
        } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
            actor->level = 50;
            actor->exp = 90;
        } else if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
            actor->level = 47;
            actor->exp = 300;
        }
    }
}

void Leveled_Wolfos(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params == 0) {
        actor->level = 10;
        actor->exp = 24;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 180;
        if (LINK_IS_ADULT) {
            actor->level = 20;
            actor->exp = 29;
        }
        if (play->sceneNum == SCENE_FOREST_TEMPLE) {
            actor->level = 26;
            actor->exp = 43;
        } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
            actor->level = 47;
            actor->exp = 158;
        } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
            actor->level = 50;
            actor->exp = 165;
        }
    } else { // White
        actor->level = 37;
        actor->exp = 126;
        levelEntry->levelModifier = 3;
        levelEntry->experienceRate = 180;
    }
    if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
        actor->level = 47;
        actor->exp = 420;
    }
}

void Leveled_Lizalfos(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params == -2) { // Dinolfos
        actor->level = 52;
        actor->exp = 250;
        levelEntry->levelModifier = 4;
        levelEntry->experienceRate = 320;
    } else if (actor->params == -1) { // Lizalfos
        actor->level = 47;
        actor->exp = 210;
        levelEntry->levelModifier = 3;
        levelEntry->experienceRate = 290;
    } else { // Miniboss Lizalfos
        actor->level = 13;
        actor->exp = 140;
        levelEntry->levelModifier = 3;
        levelEntry->experienceRate = 1150;
        actor->ignoreExpReward = true;
    }
    if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
        actor->level = 47;
        actor->exp = 570;
    }
}

void Leveled_Moblins(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params == -1) { // Spear
        actor->level = 24;
        actor->exp = 32;
        levelEntry->levelModifier = 2;
        levelEntry->experienceRate = 140;
    } else if (actor->params == 0) { // Club
        actor->level = 25;
        actor->exp = 47;
        levelEntry->levelModifier = 3;
        levelEntry->experienceRate = 240;
    } else { // Spear patrol
        actor->level = 24;
        actor->exp = 24;
        levelEntry->levelModifier = 2;
        levelEntry->experienceRate = 140;
    }
}

void Leveled_Bubble(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    if (actor->params == -1) { // Blue
        actor->level = 25;
        actor->exp = 29;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 105;
    } else if (actor->params == -2) { // Red
        actor->level = 32;
        actor->exp = 47;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 110;
        if ( play->sceneNum == SCENE_SHADOW_TEMPLE) {
            actor->level = 40;
            actor->exp = 75;
        }
    } else if (actor->params == -3) { // White
        actor->level = 45;
        actor->exp = 76;
        levelEntry->levelModifier = 1;
        levelEntry->experienceRate = 107;
    } else if (actor->params == -4) { // Big Green
        actor->level = 41;
        actor->exp = 82;
        levelEntry->levelModifier = 2;
        levelEntry->experienceRate = 100;
    } else { // Green
        actor->level = 26;
        actor->exp = 31;
        levelEntry->levelModifier = 3;
        levelEntry->experienceRate = 93;
        if (play->sceneNum == SCENE_BOTTOM_OF_THE_WELL ||
            play->sceneNum == SCENE_SHADOW_TEMPLE) {
            actor->level = 41;
            actor->exp = 70;
        } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
            actor->level = 45;
            actor->exp = 77;
        } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
            actor->level = 50;
            actor->exp = 85;
        }
    }
    if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
        actor->level = 47;
        actor->exp = 220;
    }
}

void Leveled_Stalfos(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 27;
    actor->exp = 64;
    levelEntry->levelModifier = 4;
    levelEntry->experienceRate = 345;

    if (play->sceneNum == SCENE_FOREST_TEMPLE) {
        actor->ignoreExpReward = true;
        actor->exp = 250;
        levelEntry->experienceRate = 800;
    }else if (play->sceneNum == SCENE_SHADOW_TEMPLE) {
        actor->level = 43;
        actor->exp = 214;
    } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
        actor->level = 48;
        actor->exp = 236;
    } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER ||
               play->sceneNum == SCENE_GANONS_TOWER_COLLAPSE_INTERIOR ||
               play->sceneNum == SCENE_INSIDE_GANONS_CASTLE_COLLAPSE) {
        actor->level = 50;
        actor->exp = 260;
    } else if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
        actor->level = 47;
        actor->exp = 700;
    }
}

void Leveled_Floormaster(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 26;
    actor->exp = 41;
    levelEntry->levelModifier = 2;
    levelEntry->experienceRate = 170;
    actor->ignoreExpReward = true;
    if (play->sceneNum == SCENE_BOTTOM_OF_THE_WELL ||
        play->sceneNum == SCENE_SHADOW_TEMPLE) {
        actor->level = 42;
        actor->exp = 140;
    } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
        actor->level = 47;
        actor->exp = 160;
    }
}

void Leveled_Wallmaster(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 27;
    actor->exp = 44;
    levelEntry->levelModifier = 2;
    levelEntry->experienceRate = 175;
    if (play->sceneNum == SCENE_BOTTOM_OF_THE_WELL ||
        play->sceneNum == SCENE_SHADOW_TEMPLE) {
        actor->level = 41;
        actor->exp = 129;
    } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
        actor->level = 47;
        actor->exp = 138;
    } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
        actor->level = 50;
        actor->exp = 150;
    }
}

void Leveled_Poh_Sisters(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 28;
    actor->exp = 127;
    levelEntry->levelModifier = 4;
    levelEntry->experienceRate = 450;
}

void Leveled_LikeLike(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 32;
    actor->exp = 99;
    levelEntry->levelModifier = 2;
    levelEntry->experienceRate = 245;
    if (play->sceneNum == SCENE_WATER_TEMPLE) {
        actor->level = 38;
        actor->exp = 142;
    } else if (play->sceneNum == SCENE_BOTTOM_OF_THE_WELL ||
               play->sceneNum == SCENE_SHADOW_TEMPLE) {
        actor->level = 43;
        actor->exp = 161;
    } else if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
        actor->level = 48;
        actor->exp = 182;
    } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
        actor->level = 50;
        actor->exp = 190;
    } else    if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
        actor->level = 47;
        actor->exp = 375;
    }
}

void Leveled_TorchSlug(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 33;
    actor->exp = 67;
    levelEntry->levelModifier = 1;
    levelEntry->experienceRate = 135;
    if (play->sceneNum == SCENE_SPIRIT_TEMPLE) {
        actor->level = 48;
        actor->exp = 163;
    } else if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
        actor->level = 50;
        actor->exp = 170;
    } else if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
        actor->level = 47;
        actor->exp = 350;
    }
}
void Leveled_Freezer(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 35;
    actor->exp = 101;
    levelEntry->levelModifier = 3;
    levelEntry->experienceRate = 205;
    if (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE || play->sceneNum == SCENE_GANONS_TOWER) {
        actor->level = 50;
        actor->exp = 160;
    }
}

void Leveled_ShellBlade(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 38;
    actor->exp = 129;
    levelEntry->levelModifier = 2;
    levelEntry->experienceRate = 200;
    if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
        actor->level = 47;
        actor->exp = 390;
    }
}

void Leveled_RollingSpike(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 37;
    actor->exp = 100;
    levelEntry->levelModifier = 1;
    levelEntry->experienceRate = 180;
    if (play->sceneNum == SCENE_GERUDO_TRAINING_GROUND) {
        actor->level = 47;
        actor->exp = 325;
    }
}

void Leveled_GerudoFighter(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 45;
    actor->exp = 455;
    levelEntry->levelModifier = 2;
    levelEntry->experienceRate = 1250;
}

void Leveled_Leever(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 45;
    actor->exp = 15;
    levelEntry->levelModifier = 0;
    levelEntry->experienceRate = 20;
}

void Leveled_Anubis(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 48;
    levelEntry->levelModifier = 3;
    if (actor->id == ACTOR_EN_ANUBICE) {
        actor->exp = 120;
        levelEntry->experienceRate = 155;
    }
}

void Leveled_Arwing(PlayState* play, Actor* actor, SceneLevelEntry* levelEntry) {
    actor->level = 20;
    actor->exp = 50;
    levelEntry->levelModifier = 3;
    levelEntry->experienceRate = 215;
}


void Actor_GetLevelAndExperience(PlayState* play, Actor* actor, u16 actorIdOverride) {
    SceneLevelEntry levelEntry;
    levelEntry.levelModifier = 0;
    levelEntry.experienceRate = 0;
    levelEntry.ignoreEntry = 0;

    u16 actorId = actor->id;
    if (actorIdOverride != 0) {
        actorId = actorIdOverride;
    }

    switch (actorId) {
        case ACTOR_BOSS_DODONGO:
        case ACTOR_EN_DODONGO:
            Leveled_Dodongo(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_GOMA:
        case ACTOR_EN_GOMA:
            Leveled_Gohma(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_VA:
            Leveled_Barinade(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_GANONDROF:
            Leveled_PhantomGanon(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_FD:
        case ACTOR_BOSS_FD2:
            Leveled_Volvagia(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_MO:
            Leveled_Morpha(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_SST:
            Leveled_BongoBongo(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_TW:
            Leveled_Twinrova(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_GANON:
            Leveled_Ganondorf(play, actor, &levelEntry);
            break;
        case ACTOR_BOSS_GANON2:
            Leveled_Ganon(play, actor, &levelEntry);
            break;
        case ACTOR_EN_DEKUBABA:
            Leveled_Dekubaba(play, actor, &levelEntry);
            break;
        case ACTOR_EN_KAREBABA:
            Leveled_StickDekuBaba(play, actor, &levelEntry);
            break;
        case ACTOR_EN_ST:
            Leveled_Skulltula(play, actor, &levelEntry);
            break;
        case ACTOR_EN_DEKUNUTS:
            Leveled_DekuScrub(play, actor, &levelEntry);
            break;
        case ACTOR_EN_TITE:
            Leveled_Tektite(play, actor, &levelEntry);
            break;
        case ACTOR_EN_CROW:
            Leveled_Guay(play, actor, &levelEntry);
            break;
        case ACTOR_EN_OKUTA:
            Leveled_Octorock(play, actor, &levelEntry);
            break;
        case ACTOR_EN_AM:
            Leveled_Armos(play, actor, &levelEntry);
            break;
        case ACTOR_EN_DODOJR:
            Leveled_BabyDodongo(play, actor, &levelEntry);
            break;
        case ACTOR_EN_FIREFLY:
            Leveled_Keese(play, actor, &levelEntry);
            break;
        case ACTOR_EN_PEEHAT:
            Leveled_Peahat(play, actor, &levelEntry);
            break;
        case ACTOR_EN_POH:
            Leveled_Poh(play, actor, &levelEntry);
        case ACTOR_EN_PO_FIELD:
            Leveled_Poh_Field(play, actor, &levelEntry);
            break;
        case ACTOR_EN_RD:
            Leveled_ReDead(play, actor, &levelEntry);
            break;
        case ACTOR_EN_SKB:
            Leveled_Stalchild(play, actor, &levelEntry);
            break;
        case ACTOR_EN_SW:
            Leveled_Wall_Skulltula(play, actor, &levelEntry);
            break;
        case ACTOR_EN_VM:
            Leveled_Beamos(play, actor, &levelEntry);
            break;
        case ACTOR_EN_WF:
            Leveled_Wolfos(play, actor, &levelEntry);
            break;
        case ACTOR_EN_ZF:
            Leveled_Lizalfos(play, actor, &levelEntry);
            break;
        case ACTOR_EN_BUBBLE:
            Leveled_Shabom(play, actor, &levelEntry);
            break;
        case ACTOR_EN_BILI:
            Leveled_Biri(play, actor, &levelEntry);
            break;
        case ACTOR_EN_VALI:
            Leveled_Bari(play, actor, &levelEntry);
            break;
        case ACTOR_EN_TP:
            Leveled_Tailpasaran(play, actor, &levelEntry);
            break;
        case ACTOR_EN_BA:
        case ACTOR_EN_BX:
            Leveled_JabuTentacle(play, actor, &levelEntry);
            break;
        case ACTOR_EN_EIYER:
        case ACTOR_EN_WEIYER:
            Leveled_Stinger(play, actor, &levelEntry);
            break;
        case ACTOR_EN_BIGOKUTA:
            Leveled_BigOctorock(play, actor, &levelEntry);
            break;
        case ACTOR_EN_MB:
            Leveled_Moblins(play, actor, &levelEntry);
            break;
        case ACTOR_EN_BB:
            Leveled_Bubble(play, actor, &levelEntry);
            break;
        case ACTOR_EN_TEST:
            Leveled_Stalfos(play, actor, &levelEntry);
            break;
        case ACTOR_EN_FLOORMAS:
            Leveled_Floormaster(play, actor, &levelEntry);
            break;
        case ACTOR_EN_WALLMAS:
            Leveled_Wallmaster(play, actor, &levelEntry);
            break;
        case ACTOR_EN_PO_SISTERS:
            Leveled_Poh_Sisters(play, actor, &levelEntry);
            break;
        case ACTOR_EN_RR:
            Leveled_LikeLike(play, actor, &levelEntry);
            break;
        case ACTOR_EN_BW:
            Leveled_TorchSlug(play, actor, &levelEntry);
            break;
        case ACTOR_EN_FZ:
            Leveled_Freezer(play, actor, &levelEntry);
            break;
        case ACTOR_EN_SB:
            Leveled_ShellBlade(play, actor, &levelEntry);
            break;
        case ACTOR_EN_NY:
            Leveled_RollingSpike(play, actor, &levelEntry);
            break;
        case ACTOR_EN_TORCH2:
            Leveled_DarkLink(play, actor, &levelEntry);
            break;
        case ACTOR_EN_DH:
        case ACTOR_EN_DHA:
            Leveled_DeadHand(play, actor, &levelEntry);
            break;
        case ACTOR_EN_FD:
        case ACTOR_EN_FW:
        case ACTOR_EN_FD_FIRE:
            Leveled_FlareDancer(play, actor, &levelEntry);
            break;
        case ACTOR_EN_IK:
            Leveled_IronKnuckle(play, actor, &levelEntry);
            break;
        case ACTOR_EN_GELDB:
            Leveled_GerudoFighter(play, actor, &levelEntry);
            break;
        case ACTOR_EN_REEBA:
            Leveled_Leever(play, actor, &levelEntry);
            break;
        case ACTOR_EN_ANUBICE:
        case ACTOR_EN_ANUBICE_FIRE:
            Leveled_Anubis(play, actor, &levelEntry);
            break;
        case ACTOR_EN_CLEAR_TAG:
            Leveled_Arwing(play, actor, &levelEntry);
            break;
        default:
            actor->level = GET_PLAYER(play)->actor.level;
            break;
    }

    s8 sceneLevel = Leveled_GetSceneLevel(play->sceneNum);
    if (!levelEntry.ignoreEntry && sceneLevel >= 0) {
        actor->level = (u8)CLAMP((f32)(sceneLevel + levelEntry.levelModifier) * (f32)CVarGetFloat("gLeveled.Difficulty.Enemy.LevelScale", 1.0f), 1, 99);
        actor->exp = GetEnemyExperienceReward(actor->level, levelEntry.experienceRate);
    }
}