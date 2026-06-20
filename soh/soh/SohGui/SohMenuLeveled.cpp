#include "SohMenu.h"
#include "SohGui.hpp"
#include <macros.h>
#include "leveled_actor_level_table.h"
#include "leveled_stat_math.h"
#include <algorithm>

namespace SohGui {
    enum LeveledUIMagicNumberType {
        LEVELED_MAGIC_NUMBER_UI_FULL,
        LEVELED_MAGIC_NUMBER_UI_CURRENT,
        LEVELED_MAGIC_NUMBER_UI_NONE
    };
    static const std::map<int32_t, const char*> magicNumberTypeList = {
        { LEVELED_MAGIC_NUMBER_UI_FULL, "Full" },
        { LEVELED_MAGIC_NUMBER_UI_CURRENT, "Current" },
        { LEVELED_MAGIC_NUMBER_UI_NONE, "None" }
    };

    enum LeveledBreakCapType {
        LEVELED_BREAK_CAP_NONE,
        LEVELED_BREAK_CAP_ENEMIES,
        LEVELED_BREAK_CAP_LINK,
        LEVELED_BREAK_CAP_BOTH
    };
    static const std::map<int32_t, const char*> breakCapTypeList = {
        { LEVELED_BREAK_CAP_NONE, "None" },
        { LEVELED_BREAK_CAP_ENEMIES, "Enemies Only" },
        { LEVELED_BREAK_CAP_LINK, "Link Only" },
        { LEVELED_BREAK_CAP_BOTH, "Enemies & Link" }
    };

    // Built once in AddMenuLeveled: enemies sorted alphabetically + an index->name map for the dropdown.
    static std::vector<const LeveledEnemyEntry*> sSortedEnemies;
    static std::map<int32_t, const char*> sEnemyComboMap;

    // Per-item leveling menu. "knob" selects which tuning sliders the per-item panel shows.
    enum LeveledItemKnob {
        LEVELED_KNOB_DAMAGE,  // damage-multiplier cap + EXP-gain %
        LEVELED_KNOB_CONTACT, // EXP-gain % only (utility/stun weapons that deal little/no damage)
        LEVELED_KNOB_STATIC,  // flat EXP per use (Farore's Wind, Lens of Truth)
        LEVELED_KNOB_PASSIVE, // EXP-gain % via enemy defeat / shield block (tunics, boots, shields, Nayru's)
    };
    struct LeveledItemMenuEntry {
        const char* display;
        int32_t item; // LeveledItemId (for live level/EXP readout)
        LeveledItemKnob knob;
        float damageCapDefault;  // ".DamageCap" default (DAMAGE knob)
        float expPercentDefault; // ".ExpPercent" default
        int32_t staticExpDefault;// ".StaticExp" default (STATIC knob)
    };
    static const LeveledItemMenuEntry sItemMenu[] = {
        { "Deku Stick",       LEVELED_ITEM_DEKU_STICK,     LEVELED_KNOB_DAMAGE,  5.0f, 50.0f, 0 },
        { "Deku Nut",         LEVELED_ITEM_DEKU_NUT,       LEVELED_KNOB_CONTACT, 1.0f, 50.0f, 0 },
        { "Fairy Slingshot",  LEVELED_ITEM_SLINGSHOT,      LEVELED_KNOB_DAMAGE,  4.0f, 50.0f, 0 },
        { "Bomb",             LEVELED_ITEM_BOMB,           LEVELED_KNOB_DAMAGE,  2.5f, 50.0f, 0 },
        { "Bombchu",          LEVELED_ITEM_BOMBCHU,        LEVELED_KNOB_DAMAGE,  2.5f, 50.0f, 0 },
        { "Boomerang",        LEVELED_ITEM_BOOMERANG,      LEVELED_KNOB_CONTACT, 1.0f, 50.0f, 0 },
        { "Fairy Bow",        LEVELED_ITEM_BOW,            LEVELED_KNOB_DAMAGE,  3.0f, 50.0f, 0 },
        { "Fire Arrow",       LEVELED_ITEM_FIRE_ARROW,     LEVELED_KNOB_DAMAGE,  3.0f, 50.0f, 0 },
        { "Ice Arrow",        LEVELED_ITEM_ICE_ARROW,      LEVELED_KNOB_DAMAGE,  3.0f, 50.0f, 0 },
        { "Light Arrow",      LEVELED_ITEM_LIGHT_ARROW,    LEVELED_KNOB_DAMAGE,  3.0f, 50.0f, 0 },
        { "Hookshot",         LEVELED_ITEM_HOOKSHOT,       LEVELED_KNOB_CONTACT, 1.0f, 50.0f, 0 },
        { "Megaton Hammer",   LEVELED_ITEM_HAMMER,         LEVELED_KNOB_DAMAGE,  3.0f, 50.0f, 0 },
        { "Lens of Truth",    LEVELED_ITEM_LENS,           LEVELED_KNOB_STATIC,  1.0f, 10.0f, 5 },
        { "Din's Fire",       LEVELED_ITEM_DINS_FIRE,      LEVELED_KNOB_DAMAGE,  3.0f, 50.0f, 0 },
        { "Farore's Wind",    LEVELED_ITEM_FARORES_WIND,   LEVELED_KNOB_STATIC,  1.0f, 10.0f, 30 },
        { "Nayru's Love",     LEVELED_ITEM_NAYRUS_LOVE,    LEVELED_KNOB_PASSIVE, 1.0f, 10.0f, 0 },
        { "Kokiri Sword",     LEVELED_ITEM_KOKIRI_SWORD,   LEVELED_KNOB_DAMAGE,  4.0f, 50.0f, 0 },
        { "Master Sword",     LEVELED_ITEM_MASTER_SWORD,   LEVELED_KNOB_DAMAGE,  4.0f, 50.0f, 0 },
        { "Biggoron's Sword", LEVELED_ITEM_BIGGORON_SWORD, LEVELED_KNOB_DAMAGE,  5.0f, 50.0f, 0 },
        { "Deku Shield",      LEVELED_ITEM_DEKU_SHIELD,    LEVELED_KNOB_PASSIVE, 1.0f,  5.0f, 0 },
        { "Hylian Shield",    LEVELED_ITEM_HYLIAN_SHIELD,  LEVELED_KNOB_PASSIVE, 1.0f,  5.0f, 0 },
        { "Mirror Shield",    LEVELED_ITEM_MIRROR_SHIELD,  LEVELED_KNOB_PASSIVE, 1.0f,  5.0f, 0 },
        { "Kokiri Tunic",     LEVELED_ITEM_KOKIRI_TUNIC,   LEVELED_KNOB_PASSIVE, 1.0f, 10.0f, 0 },
        { "Goron Tunic",      LEVELED_ITEM_GORON_TUNIC,    LEVELED_KNOB_PASSIVE, 1.0f, 10.0f, 0 },
        { "Zora Tunic",       LEVELED_ITEM_ZORA_TUNIC,     LEVELED_KNOB_PASSIVE, 1.0f, 10.0f, 0 },
        { "Kokiri Boots",     LEVELED_ITEM_KOKIRI_BOOTS,   LEVELED_KNOB_PASSIVE, 1.0f, 10.0f, 0 },
        { "Iron Boots",       LEVELED_ITEM_IRON_BOOTS,     LEVELED_KNOB_PASSIVE, 1.0f, 10.0f, 0 },
        { "Hover Boots",      LEVELED_ITEM_HOVER_BOOTS,    LEVELED_KNOB_PASSIVE, 1.0f, 10.0f, 0 },
    };
    static const int32_t sItemMenuCount = (int32_t)(sizeof(sItemMenu) / sizeof(sItemMenu[0]));
    static std::map<int32_t, const char*> sItemComboMap;

    extern std::shared_ptr<SohMenu> mSohMenu;
    using namespace UIWidgets;

    void SohMenu::AddMenuLeveled() {
        // Add Leveled Menu
        AddMenuEntry("Leveled", CVAR_SETTING("Menu.LeveledSidebarSection"));

        //////////////////////////////////////////////////////////////////////////////////////////
        // Master
        WidgetPath path = { "Leveled", "Master", SECTION_COLUMN_1 };
        AddSidebarEntry("Leveled", path.sidebarName, 1);
        AddWidget(path, "Master", WIDGET_SEPARATOR_TEXT);
        AddWidget(path, "Enable Leveled", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Master")
            .Options(CheckboxOptions()
                         .Tooltip("Master switch for the entire Leveled mod. When off, the game plays like normal "
                                  "SoH: enemies keep their vanilla stats, Link gains no EXP or levels, every Leveled "
                                  "HUD element is hidden, and all stat / damage / health / magic changes are disabled.\n"
                                  "Saved EXP is preserved, so turning it back on restores your level.\n"
                                  "Reload the scene (or save & reload) after toggling for it to fully apply.")
                         .DefaultValue(true));

        //////////////////////////////////////////////////////////////////////////////////////////
        // UI Settings
        path = { "Leveled", "UI Settings", SECTION_COLUMN_1 };
        AddSidebarEntry("Leveled", path.sidebarName, 1);
        // HUD
        AddWidget(path, "HUD", WIDGET_SEPARATOR_TEXT);
        // EXP Next Level
        AddWidget(path, "EXP to NEXT Level", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.HUD.ExperienceNextLevel")
            .Options(CheckboxOptions()
                         .Tooltip("Show experience required to level up popup in the HUD when gaining EXP.")
                         .DefaultValue(true));
        // Level Up
        AddWidget(path, "Level Up", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.HUD.LevelUp")
            .Options(CheckboxOptions().Tooltip("Show Level Up popup when leveling up.").DefaultValue(true));
        // Level Up Sound
        AddWidget(path, "Level Up Sound", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.HUD.LevelUpSound")
            .Options(CheckboxOptions().Tooltip("Plays sound when leveling up.").DefaultValue(true));
        // Item levels in the pause menu
        AddWidget(path, "Item Levels in Inventory", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.HUD.InventoryItemLevel")
            .Options(CheckboxOptions()
                         .Tooltip("In the pause menu, show the level, current EXP, and EXP to next level for the item "
                                  "your cursor is highlighting (next to its name).")
                         .DefaultValue(true));

        // Health Meter Numbers
        AddWidget(path, "Health Numbers", WIDGET_CVAR_COMBOBOX)
            .CVar("gLeveled.HUD.HealthNumbersType")
            .Options(ComboboxOptions()
                         .ComboMap(magicNumberTypeList)
                         .DefaultIndex(LEVELED_MAGIC_NUMBER_UI_FULL)
                         .Tooltip("Sets the numbers displayed on the health meter.")
                         .Color(THEME_COLOR));

        // Magic Meter Numbers
        AddWidget(path, "Magic Meter Numbers", WIDGET_CVAR_COMBOBOX)
            .CVar("gLeveled.HUD.MagicMeterNumbersType")
            .SameLine(true)
            .Options(ComboboxOptions()
                         .ComboMap(magicNumberTypeList)
                         .DefaultIndex(LEVELED_MAGIC_NUMBER_UI_FULL)
                         .Tooltip("Sets the numbers displayed on the magic meter.")
                         .Color(THEME_COLOR));
        //----------------------------------------------------------------------------------------
        // Floating Numbers
        AddWidget(path, "Floating Numbers", WIDGET_SEPARATOR_TEXT);
        // Enemy Damage
        AddWidget(path, "Enemy Damage", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.HUD.FloatingNumbers.EnemyDamage")
            .Options(CheckboxOptions().Tooltip("Show floating damage numbers on enemies.").DefaultValue(true));
        // Player Damage
        AddWidget(path, "Player Damage", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.HUD.FloatingNumbers.PlayerDamage")
            .Options(CheckboxOptions().Tooltip("Show floating damage numbers on link.").DefaultValue(true));
        // EXP Gain
        AddWidget(path, "EXP Gain", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.HUD.FloatingNumbers.ExpGain")
            .Options(CheckboxOptions().Tooltip("Show floating EXP gain numbers.").DefaultValue(true));
        //----------------------------------------------------------------------------------------
        // Navi Info
        AddWidget(path, "Navi Enemy Info", WIDGET_SEPARATOR_TEXT);
        // Navi Level
        AddWidget(path, "Navi tells enemy level", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Navi.TellEnemyLevel")
            .Options(CheckboxOptions().Tooltip("Navi will tell the enemy level.").DefaultValue(true));
        // Navi HP
        AddWidget(path, "Navi tells enemy max HP", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Navi.TellEnemyMaxHP")
            .Options(CheckboxOptions().Tooltip("Navi will tell the enemy's maximum HP.").DefaultValue(true));
        //////////////////////////////////////////////////////////////////////////////////////////
        // Entity Modifications
        path.sidebarName = "Entity Modifications";
        AddSidebarEntry("Leveled", path.sidebarName, 1);
        // Enemy / Link HP Mult
        AddWidget(path, "Enemy HP and Link's Attack Multiplier: %dx", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.Enhancements.AttackAndHPMultiplier")
            .Options(IntSliderOptions().Min(1).Max(32).Format("%d").DefaultValue(9).Tooltip(
                "Changes Link's Attack and enemy HP multiplier.\nThis doesn't change balance, but rather the size of "
                "numbers."));
        //----------------------------------------------------------------------------------------
        AddWidget(path, "Player Modifications", WIDGET_SEPARATOR_TEXT);
        // Level Increases Life
        AddWidget(path, "Level Gives Bonus Hearts", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Player.Enhancements.HeartsWithLevelUp")
            .Options(
                CheckboxOptions().Tooltip("Levels give health upgrades, up to 10 extra hearts.").DefaultValue(true));
        // Level Modifies Magic
        AddWidget(path, "Level Affects Magic Capacity", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Player.Enhancements.MagicWithLevelUp")
            .Options(CheckboxOptions()
                         .Tooltip("Levels alter magic capacity. Starts at 25% of the vanilla value, but will go up to "
                                  "150% of vanilla. Double magic works the same as the original, doubling capacity.")
                         .DefaultValue(true));
        // Equipment Stats
        AddWidget(path, "Equipment Affects Stats", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Player.Enhancements.EquipmentStats")
            .Options(CheckboxOptions()
                         .Tooltip("Equipment will alter stats. Such as Goron Tunic providing +3 STR, but gives -3 "
                                  "Defense, Bracelets give +1 STR, Shields, etc.")
                         .DefaultValue(true));
        //----------------------------------------------------------------------------------------
        AddWidget(path, "Enemy Modifications", WIDGET_SEPARATOR_TEXT);
        // Enemy Level Affects Attack
        AddWidget(path, "Enemy Level Affects Base Attack", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Enemy.Enhancements.AttackScalesWithLevel")
            .Options(
                CheckboxOptions()
                    .Tooltip("Enemies have a fixed attack value. This option scales this up the higher the enemy's "
                             "Strength stat. \nThis will increase difficulty a bit.")
                    .DefaultValue(true));
        //////////////////////////////////////////////////////////////////////////////////////////
        // Difficulty Options
        path.sidebarName = "Difficulty Options";
        AddSidebarEntry("Leveled", path.sidebarName, 1);

        // Player Options
        AddWidget(path, "Player Options", WIDGET_SEPARATOR_TEXT);
        // Heart Value
        AddWidget(path, ("Heart Container Value in Units: "), WIDGET_CUSTOM).CustomFunction([](WidgetInfo& info) {
                ImGui::BeginDisabled(!CVarGetInteger("gLeveled.Master", 1));
                ImGui::Text("Heart Container Value in Units: %d (%.2fx)", CVarGetInteger("gLeveled.Difficulty.HeartUnits", 4) << 2, (float)CVarGetInteger("gLeveled.Difficulty.HeartUnits", 4) / 4.0f);
                UIWidgets::CVarSliderInt("Heart Container Value in Units", "gLeveled.Difficulty.HeartUnits",
                IntSliderOptions().LabelPosition(Within).Min(1).Max(32).Format("").DefaultValue(4).Color(THEME_COLOR)
                    .Tooltip(
                    "Sets how many health units each completed heart container is worth.\n"
                    "One heart on the health meter is equal to 16 health units.\n"
                    "A lower setting will result in lower total health.\n"
                    "Change areas to update health capacity."));
                ImGui::EndDisabled();
        });
        // Damage Multiplier
        AddWidget(path, ("Damage Multiplier: "), WIDGET_CUSTOM).CustomFunction([](WidgetInfo& info) {
                ImGui::BeginDisabled(!CVarGetInteger("gLeveled.Master", 1));
                ImGui::Text("Damage Multiplier: %.2fx", (float)CVarGetInteger("gLeveled.Difficulty.Player.DamageMultiplier", 4) / 4.0f);
                UIWidgets::CVarSliderInt("Leveled Damage Multiplier", "gLeveled.Difficulty.Player.DamageMultiplier",
                IntSliderOptions().LabelPosition(Within).Min(1).Max(32).Format("").DefaultValue(4).Color(THEME_COLOR)
                    .Tooltip("Sets a multiplier for the damage the player takes. Includes ALL sources, even damage while being frozen or burned.\nDamage cannot be reduced below 1."));
                ImGui::EndDisabled();
        });
        //----------------------------------------------------------------------------------------
        // EXP Options
        AddWidget(path, "EXP Options", WIDGET_SEPARATOR_TEXT);
        // EXP Multiplier
        AddWidget(path, "EXP Rate: %.2fx", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.EXP.Rate")
            .Options(FloatSliderOptions().Min(0.0f).Max(10.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
                "Sets the EXP multiplier link gains, with the exception of gold skulltula tokens."));
        // Token EXP
        AddWidget(path, "Skulltula Token EXP Rate: %.2f x", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.EXP.TokenRate")
            .Options(FloatSliderOptions().Min(0.0f).Max(10.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
                "Sets the EXP multiplier link gains from gold skulltula tokens."));
        //----------------------------------------------------------------------------------------
        // Enemy Options
        AddWidget(path, "Enemy Options", WIDGET_SEPARATOR_TEXT);
        // Max GS Tokens make Ganon's Castle Enemies Lv99
        AddWidget(path, "Collecting all 100 tokens makes all enemies in Ganon's Castle level 99", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Difficulty.Enemy.MaxLevelInGanonCastle")
            .Options(CheckboxOptions()
                         .Tooltip("If all tokens are collected, all enemies in Ganon's Casle are level 99.")
                         .DefaultValue(false));
        // Enemy HP
        AddWidget(path, "Enemy HP: %.2fx", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.Enemy.HPPercent")
            .Options(
                FloatSliderOptions().Min(0.0f).Max(10.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
                "Sets the HP multiplier for enemies."));
        //----------------------------------------------------------------------------------------
        // Regular Enemy Scaling (non-bosses; mini-bosses count as regular enemies)
        AddWidget(path, "Regular Enemy Scaling", WIDGET_SEPARATOR_TEXT);
        // Scale regular enemy levels toward the player
        AddWidget(path, "Scale Regular Enemies to Player", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Difficulty.Enemy.ScaleToPlayer")
            .Options(CheckboxOptions()
                         .Tooltip("Regular (non-boss) enemies scale their level toward the player's, bounded between "
                                  "their base level (floor) and a cap (ceiling).\nKeeps enemies relevant as you level up.\n"
                                  "Big 10th-spawns can opt out via \"Big Spawns Ignore Scaling\" below.")
                         .DefaultValue(true));
        // Regular enemy base/floor level (separate from player scaling)
        AddWidget(path, "Regular Enemy Base Level Scale: %.2fx", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.Enemy.LevelScale")
            .Options(FloatSliderOptions().Min(0.0f).Max(10.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
                "Multiplier applied to each regular enemy's base level (its area-defined starting level).\n"
                "For example, at 1.50x a base level 20 enemy becomes base level 30.\n"
                "This adjusts the base/floor level only - it is separate from the player-scaling toggle above."));
        // Regular enemy scaling ceiling, as a multiple of base level
        AddWidget(path, "Regular Enemy Scaling Cap: %.2fx base level", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.Enemy.ScaleMaxMultiplier")
            .Options(FloatSliderOptions().Min(1.0f).Max(5.0f).Format("%.2fx").DefaultValue(2.0f).Tooltip(
                "Default ceiling for regular-enemy scaling, as a multiple of the enemy's base level.\n"
                "For example, 2.00x lets a base level 15 enemy scale up to level 30.\n"
                "Enemies with a custom per-enemy cap ignore this. Requires \"Scale Regular Enemies to Player\"."));
        // Regular enemy scaling multiplier (over-levels the player after scaling)
        AddWidget(path, "Regular Enemy Scaling Multiplier: %.2fx", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.Enemy.ScalingMultiplier")
            .Options(FloatSliderOptions().Min(1.0f).Max(5.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
                "After a regular enemy scales to the player, its level is multiplied by this so it out-levels "
                "Link for a while.\nE.g. with a scaling cap of 2.00x and this at 1.50x, a base level 5 enemy "
                "reaches level 15 (and stays ahead until Link catches up).\n1.00x = no extra."));
        // Let "big"/10th enemy spawns keep their designed level instead of scaling
        AddWidget(path, "Big Spawns Ignore Scaling", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Difficulty.Enemy.BigSpawnsIgnoreScaling")
            .Options(CheckboxOptions()
                         .Tooltip("The \"big\" 10th-spawn variants of Leevers, Stalchildren, and Guays keep their own "
                                  "designed level instead of being clamped to the player-scaling cap.\n"
                                  "Disable to make even these scale with the player like normal enemies.")
                         .DefaultValue(true));
        //----------------------------------------------------------------------------------------
        // Boss Scaling (true dungeon bosses + Ganon; mini-bosses scale as regular enemies)
        AddWidget(path, "Boss Scaling", WIDGET_SEPARATOR_TEXT);
        // Scale boss levels toward the player
        AddWidget(path, "Scale Boss Enemies to Player", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Difficulty.Boss.ScaleToPlayer")
            .Options(CheckboxOptions()
                         .Tooltip("Bosses scale their level toward the player's, bounded between their base level and "
                                  "the boss scaling cap below.\nOnly affects true dungeon bosses and Ganon "
                                  "(mini-bosses are handled by Regular Enemy Scaling).")
                         .DefaultValue(true));
        // Boss base/floor level multiplier
        AddWidget(path, "Boss Base Level Multiplier: %.2fx", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.Boss.BaseLevelMultiplier")
            .Options(FloatSliderOptions().Min(0.25f).Max(5.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
                "Multiplier applied to each boss's base level (its area-defined starting level).\n"
                "Independent of the regular-enemy Base Level Scale."));
        // Boss scaling ceiling, as a multiple of base level
        AddWidget(path, "Boss Scaling Cap: %.2fx base level", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.Boss.ScaleMaxMultiplier")
            .Options(FloatSliderOptions().Min(1.0f).Max(10.0f).Format("%.2fx").DefaultValue(3.0f).Tooltip(
                "How far above its base level a boss may scale to match the player, as a multiple of its base.\n"
                "For example, 3.00x lets a base level 30 boss scale up to level 90. Requires \"Scale Boss Enemies to Player\"."));
        // Boss scaling multiplier (over-levels the player after scaling)
        AddWidget(path, "Boss Scaling Multiplier: %.2fx", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Difficulty.Boss.ScalingMultiplier")
            .Options(FloatSliderOptions().Min(1.0f).Max(5.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
                "After a boss scales to the player, its level is multiplied by this so it out-levels Link.\n"
                "1.00x = no extra. Requires \"Scale Boss Enemies to Player\"."));

        //////////////////////////////////////////////////////////////////////////////////////////
        // Level Caps
        path = { "Leveled", "Level Caps", SECTION_COLUMN_1 };
        AddSidebarEntry("Leveled", path.sidebarName, 1);
        // Exceeding Level 99
        AddWidget(path, "Exceeding Level 99", WIDGET_SEPARATOR_TEXT);
        AddWidget(path, "Break Level Cap", WIDGET_CVAR_COMBOBOX)
            .CVar("gLeveled.LevelCap.BreakCap")
            .Options(ComboboxOptions()
                         .ComboMap(breakCapTypeList)
                         .DefaultIndex(LEVELED_BREAK_CAP_ENEMIES)
                         .Tooltip("Allows levels to exceed 99 (up to 255).\n"
                                  "None: nothing exceeds 99.\n"
                                  "Enemies Only: enemies/bosses may exceed 99 (e.g. when scaling to a high-level Link).\n"
                                  "Link Only: Link may exceed 99 (the EXP curve is extended past 99).\n"
                                  "Enemies & Link: both."));
        //----------------------------------------------------------------------------------------
        // Level Limits (cap Link's level by story progress)
        AddWidget(path, "Level Limits (by progress)", WIDGET_SEPARATOR_TEXT);
        AddWidget(path, "Limit Levels by Progress", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.LevelLimit.Enabled")
            .Options(CheckboxOptions()
                         .Tooltip("Caps Link's level based on dungeons cleared. Kid Limit and Adult Limit are the "
                                  "overall caps for each era; the dungeon sliders are sub-gates (the cap is the lowest "
                                  "of the era cap and any gate you haven't cleared yet).\n"
                                  "Change areas after clearing a dungeon for a raised cap to take effect.")
                         .DefaultValue(false));
        AddWidget(path, "Kid Level Limit: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.KidLimit")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Overall level cap while Link is a child."));
        AddWidget(path, "Adult Level Limit: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.AdultLimit")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Overall level cap while Link is an adult."));
        AddWidget(path, "Deku Tree: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.DekuTree")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Cap until the Deku Tree is cleared (Kokiri's Emerald obtained)."));
        AddWidget(path, "Death Mountain: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.DeathMountain")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Cap until Dodongo's Cavern is cleared (Goron's Ruby obtained)."));
        AddWidget(path, "Jabu Jabu: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.JabuJabu")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Cap until Jabu-Jabu's Belly is cleared (Zora's Sapphire obtained)."));
        AddWidget(path, "Dungeon Defeated 1: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.Dungeon1")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Cap until 1 adult dungeon (medallion) is cleared."));
        AddWidget(path, "Dungeon Defeated 2: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.Dungeon2")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Cap until 2 adult dungeons (medallions) are cleared."));
        AddWidget(path, "Dungeon Defeated 3: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.Dungeon3")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Cap until 3 adult dungeons (medallions) are cleared."));
        AddWidget(path, "Dungeon Defeated 4: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.Dungeon4")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Cap until 4 adult dungeons (medallions) are cleared."));
        AddWidget(path, "Dungeon Defeated 5: %d", WIDGET_CVAR_SLIDER_INT)
            .CVar("gLeveled.LevelLimit.Dungeon5")
            .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(99).Format("%d").Tooltip(
                "Cap until 5 adult dungeons (medallions) are cleared."));

        //////////////////////////////////////////////////////////////////////////////////////////
        // Areas (per-area level overrides) - generated from the shared area table
        path = { "Leveled", "Areas", SECTION_COLUMN_1 };
        AddSidebarEntry("Leveled", path.sidebarName, 3);
        AddWidget(path, "Per-Area Levels", WIDGET_SEPARATOR_TEXT);
        {
            s32 perColumn = (gLeveledAreaTableCount + 2) / 3;
            for (s32 i = 0; i < gLeveledAreaTableCount; i++) {
                const LeveledAreaEntry& a = gLeveledAreaTable[i];
                path.column = (i < perColumn) ? SECTION_COLUMN_1 : (i < perColumn * 2) ? SECTION_COLUMN_2 : SECTION_COLUMN_3;
                AddWidget(path, std::string(a.display) + ": %d", WIDGET_CVAR_SLIDER_INT)
                    .CVar(a.cvar)
                    .Options(IntSliderOptions().Min(1).Max(255).DefaultValue(a.defaultLevel).Format("%d").Tooltip(
                        "Base level for this area. The default shown is the mod's pre-determined value. Enemies in "
                        "this area scale to it (plus their per-enemy base/offset)."));
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        // Enemies (per-enemy level overrides) - pick one enemy from the dropdown, then edit its values
        path = { "Leveled", "Enemies", SECTION_COLUMN_1 };
        AddSidebarEntry("Leveled", path.sidebarName, 1);
        if (sSortedEnemies.empty()) {
            for (s32 i = 0; i < gLeveledEnemyTableCount; i++) {
                sSortedEnemies.push_back(&gLeveledEnemyTable[i]);
            }
            std::sort(sSortedEnemies.begin(), sSortedEnemies.end(),
                      [](const LeveledEnemyEntry* a, const LeveledEnemyEntry* b) {
                          return std::string(a->display) < std::string(b->display);
                      });
            for (s32 i = 0; i < (s32)sSortedEnemies.size(); i++) {
                sEnemyComboMap[i] = sSortedEnemies[i]->display;
            }
        }
        AddWidget(path, "Per-Enemy Overrides", WIDGET_SEPARATOR_TEXT);
        AddWidget(path, "Enemy", WIDGET_CVAR_COMBOBOX)
            .CVar("gLeveled.EnemyOverride.Selected")
            .Options(ComboboxOptions().ComboMap(sEnemyComboMap).DefaultIndex(0).Tooltip(
                "Pick an enemy (listed alphabetically) to edit its overrides below."));
        AddWidget(path, "EnemyOverrideSliders", WIDGET_CUSTOM).CustomFunction([](WidgetInfo& info) {
            ImGui::BeginDisabled(!CVarGetInteger("gLeveled.Master", 1));
            s32 sel = CVarGetInteger("gLeveled.EnemyOverride.Selected", 0);
            if (sel < 0 || sel >= (s32)sSortedEnemies.size()) {
                sel = 0;
            }
            const LeveledEnemyEntry* e = sSortedEnemies[sel];
            std::string scaleCvar = e->scaleCvar;
            std::string overCvar = scaleCvar.substr(0, scaleCvar.size() - 10) + ".OverMult";
            UIWidgets::CVarSliderInt((std::string(e->display) + " Base Level: %d").c_str(), e->baseCvar,
                IntSliderOptions().Min(0).Max(255).DefaultValue(0).Format("%d").Color(THEME_COLOR).Tooltip(
                    "0 = use the area's level. Higher sets a base-level floor (a higher area level still wins). "
                    "Applied before Base Level Scale."));
            UIWidgets::CVarSliderFloat((std::string(e->display) + " Scaling Cap: %.2fx").c_str(), e->scaleCvar,
                FloatSliderOptions().Min(0.0f).Max(10.0f).DefaultValue(0.0f).Format("%.2fx").Color(THEME_COLOR).Tooltip(
                    "Per-enemy scaling cap (multiple of base level). 0 = use the global cap; overrides it when higher."));
            UIWidgets::CVarSliderFloat((std::string(e->display) + " Scaling Multiplier: %.2fx").c_str(), overCvar.c_str(),
                FloatSliderOptions().Min(0.0f).Max(5.0f).DefaultValue(0.0f).Format("%.2fx").Color(THEME_COLOR).Tooltip(
                    "Multiplies this enemy's level after it scales to the player. 0 = use the global multiplier; wins when higher."));
            ImGui::EndDisabled();
        });

        //////////////////////////////////////////////////////////////////////////////////////////
        // Items (per-item weapon & equipment leveling)
        path = { "Leveled", "Items", SECTION_COLUMN_1 };
        AddSidebarEntry("Leveled", path.sidebarName, 1);
        if (sItemComboMap.empty()) {
            for (int32_t i = 0; i < sItemMenuCount; i++) {
                sItemComboMap[i] = sItemMenu[i].display;
            }
        }
        // Global category toggles
        AddWidget(path, "Global", WIDGET_SEPARATOR_TEXT);
        AddWidget(path, "Enable Equipment Scaling", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Item.EnableEquipment")
            .Options(CheckboxOptions()
                         .Tooltip("Let Swords, Shields, Tunics, and Boots gain EXP and level up.\n"
                                  "Off: they neither gain EXP nor apply any leveling bonus.")
                         .DefaultValue(true));
        AddWidget(path, "Enable C-Item Scaling", WIDGET_CVAR_CHECKBOX)
            .CVar("gLeveled.Item.EnableCItems")
            .Options(CheckboxOptions()
                         .Tooltip("Let every other item (sticks, nuts, bombs, bow & arrows, hookshot, hammer, magic "
                                  "spells, etc.) gain EXP and level up.\nOff: they neither gain EXP nor apply a bonus.")
                         .DefaultValue(true));
        AddWidget(path, "Equipment Stat Bonus per Level: %.2f%%", WIDGET_CVAR_SLIDER_FLOAT)
            .CVar("gLeveled.Item.EquipStatPercentPerLevel")
            .Options(FloatSliderOptions().Min(0.0f).Max(10.0f).Format("%.2f%%").DefaultValue(5.0f).Tooltip(
                "How much each level adds to a piece of equipment's stat contribution (the STR / defense from "
                "the \"Equipment Affects Stats\" system on the Entity Modifications page).\n"
                "At the default +5%/level: level 1 = +10%, level 99 = +500% (the item's positive stat is "
                "multiplied ~6x). Only positive stats are boosted - an item's tradeoffs are never made worse."));
        // Per-item tuning: pick an item, then edit its cap / gain rate below.
        AddWidget(path, "Per-Item Tuning", WIDGET_SEPARATOR_TEXT);
        AddWidget(path, "Item", WIDGET_CVAR_COMBOBOX)
            .CVar("gLeveled.Item.Selected")
            .Options(ComboboxOptions().ComboMap(sItemComboMap).DefaultIndex(0).Tooltip(
                "Pick an item to view its current level / EXP and edit its tuning below.\n"
                "Items earn EXP by making contact with enemies (10% of the enemy's EXP by default); equipment, "
                "shields, and the support spells use their own rules."));
        AddWidget(path, "ItemTuningSliders", WIDGET_CUSTOM).CustomFunction([](WidgetInfo& info) {
            ImGui::BeginDisabled(!CVarGetInteger("gLeveled.Master", 1));
            int32_t sel = CVarGetInteger("gLeveled.Item.Selected", 0);
            if (sel < 0 || sel >= sItemMenuCount) {
                sel = 0;
            }
            const LeveledItemMenuEntry& e = sItemMenu[sel];
            const char* prefix = Leveled_GetItemCvarPrefix(e.item);

            // Live readout of the selected item's progress.
            uint8_t lvl = Leveled_GetItemLevel(e.item);
            uint8_t cap = Leveled_GetItemLevelCap(e.item);
            uint32_t exp = Leveled_GetItemExp(e.item);
            uint32_t next = (lvl < cap) ? GetCumulativeExp(lvl) : exp;
            ImGui::Text("%s  -  Level %d / %d", e.display, lvl, cap);
            if (lvl < cap) {
                ImGui::Text("EXP toward next level: %u / %u", exp, next);
            } else {
                ImGui::Text("EXP: %u (max level for current cap)", exp);
            }
            // Equipment items (swords/shields/tunics/boots) also gain a % boost to their STR/defense stat.
            if (e.item >= LEVELED_ITEM_KOKIRI_SWORD) {
                ImGui::Text("Stat bonus at this level: +%.0f%% to its equipment stats",
                            (Leveled_GetEquipmentStatScale(e.item) - 1.0f) * 100.0f);
            }

            // Level cap (1-99) - shared by every item.
            UIWidgets::CVarSliderInt((std::string(e.display) + " Level Cap: %d").c_str(),
                (std::string(prefix) + ".LevelCap").c_str(),
                IntSliderOptions().Min(1).Max(99).DefaultValue(99).Format("%d").Color(THEME_COLOR).Tooltip(
                    "Highest level this item can reach. At the cap it provides its full bonus; at level 1 it "
                    "behaves like vanilla."));

            // Knob-specific tuning.
            if (e.knob == LEVELED_KNOB_DAMAGE) {
                UIWidgets::CVarSliderFloat((std::string(e.display) + " Damage at Max Level: %.2fx").c_str(),
                    (std::string(prefix) + ".DamageCap").c_str(),
                    FloatSliderOptions().Min(1.0f).Max(20.0f).DefaultValue(e.damageCapDefault).Format("%.2fx")
                        .Color(THEME_COLOR).Tooltip(
                        "Damage multiplier this weapon reaches at its level cap (1.00x at level 1, scaling up "
                        "linearly). Stacks on top of the global player damage scaling."));
            }
            if (e.knob == LEVELED_KNOB_STATIC) {
                UIWidgets::CVarSliderInt((std::string(e.display) + " EXP per Use: %d").c_str(),
                    (std::string(prefix) + ".StaticExp").c_str(),
                    IntSliderOptions().Min(0).Max(500).DefaultValue(e.staticExpDefault).Format("%d").Color(THEME_COLOR)
                        .Tooltip("Flat EXP this item earns each time it is used (only when it actually consumes magic)."));
            } else {
                const char* gainTip = (e.knob == LEVELED_KNOB_PASSIVE)
                    ? "Percent of a defeated enemy's EXP this item earns (shields earn from blocked attacks instead)."
                    : "Percent of an enemy's EXP this item earns each time it makes contact.";
                UIWidgets::CVarSliderFloat((std::string(e.display) + " EXP Gain: %.0f%%").c_str(),
                    (std::string(prefix) + ".ExpPercent").c_str(),
                    FloatSliderOptions().Min(0.0f).Max(100.0f).DefaultValue(e.expPercentDefault).Format("%.0f%%")
                        .Color(THEME_COLOR).Tooltip(gainTip));
            }

            // Magic-consuming items also get a cost-reduction knob (cost shrinks toward this as they level).
            bool isMagicItem = (e.item == LEVELED_ITEM_FIRE_ARROW || e.item == LEVELED_ITEM_ICE_ARROW ||
                                e.item == LEVELED_ITEM_LIGHT_ARROW || e.item == LEVELED_ITEM_DINS_FIRE ||
                                e.item == LEVELED_ITEM_FARORES_WIND || e.item == LEVELED_ITEM_NAYRUS_LOVE);
            if (isMagicItem) {
                UIWidgets::CVarSliderFloat((std::string(e.display) + " Magic Cost at Max Level: %.2fx").c_str(),
                    (std::string(prefix) + ".MagicCap").c_str(),
                    FloatSliderOptions().Min(0.1f).Max(1.0f).DefaultValue(0.5f).Format("%.2fx").Color(THEME_COLOR)
                        .Tooltip("Magic cost multiplier this item reaches at its level cap (1.00x at level 1). "
                                 "For example 0.50x halves the magic cost at max level. Cost never drops below 1."));
            }

            // Items with a timed effect get a duration knob (effect lasts longer as they level).
            if (e.item == LEVELED_ITEM_NAYRUS_LOVE || e.item == LEVELED_ITEM_HOVER_BOOTS) {
                float defDur = (e.item == LEVELED_ITEM_HOVER_BOOTS) ? 3.0f : 2.0f;
                UIWidgets::CVarSliderFloat((std::string(e.display) + " Duration at Max Level: %.2fx").c_str(),
                    (std::string(prefix) + ".DurationCap").c_str(),
                    FloatSliderOptions().Min(1.0f).Max(10.0f).DefaultValue(defDur).Format("%.2fx").Color(THEME_COLOR)
                        .Tooltip("How much longer this item's effect lasts at its level cap (1.00x at level 1).\n"
                                 "Nayru's Love: invulnerability time. Hover Boots: float time over gaps & water."));
            }
            // Stun items lengthen the enemy stun they cause as they level.
            if (e.item == LEVELED_ITEM_DEKU_NUT || e.item == LEVELED_ITEM_BOOMERANG ||
                e.item == LEVELED_ITEM_HOOKSHOT) {
                UIWidgets::CVarSliderFloat((std::string(e.display) + " Stun Duration at Max Level: %.2fx").c_str(),
                    (std::string(prefix) + ".DurationCap").c_str(),
                    FloatSliderOptions().Min(1.0f).Max(10.0f).DefaultValue(3.0f).Format("%.2fx").Color(THEME_COLOR)
                        .Tooltip("How much longer enemies stay stunned when struck by this item at its level cap "
                                 "(1.00x at level 1). Only enemies not already stunned are affected."));
            }
            // Deku Stick durability: a chance to survive a hit instead of breaking.
            if (e.item == LEVELED_ITEM_DEKU_STICK) {
                UIWidgets::CVarSliderFloat((std::string(e.display) + " Durability at Max Level: %.0f%% survive").c_str(),
                    (std::string(prefix) + ".DurabilityCap").c_str(),
                    FloatSliderOptions().Min(0.0f).Max(99.0f).DefaultValue(80.0f).Format("%.0f%%").Color(THEME_COLOR)
                        .Tooltip("Chance at the level cap to survive a strike instead of breaking (0% at level 1).\n"
                                 "At 80%, a max-level stick lasts about 5x as long on average."));
            }
            // Lens of Truth: slower magic drain as it levels.
            if (e.item == LEVELED_ITEM_LENS) {
                UIWidgets::CVarSliderFloat((std::string(e.display) + " Magic Duration at Max Level: %.2fx").c_str(),
                    (std::string(prefix) + ".DrainCap").c_str(),
                    FloatSliderOptions().Min(1.0f).Max(10.0f).DefaultValue(2.0f).Format("%.2fx").Color(THEME_COLOR)
                        .Tooltip("How much longer the Lens runs per point of magic at its level cap (1.00x at level 1).\n"
                                 "2.00x drains magic half as fast at max level."));
            }
            ImGui::EndDisabled();
        });

        // Grey out every other Leveled page while the master switch is off, so it's visually obvious
        // the rest of the mod is inert. ResetDisables() re-enables them each frame when it's back on.
        for (const char* sidebarName : { "UI Settings", "Entity Modifications", "Difficulty Options", "Level Caps", "Areas", "Enemies", "Items" }) {
            for (auto& column : menuEntries.at("Leveled").sidebars.at(sidebarName).columnWidgets) {
                for (WidgetInfo& widget : column) {
                    widget.preFunc = [](WidgetInfo& info) {
                        if (info.options && !CVarGetInteger("gLeveled.Master", 1)) {
                            info.options->disabled = true;
                            info.options->disabledTooltip = "Disabled because the Leveled \"Master\" switch is off.";
                        }
                    };
                }
            }
        }
    }

} // namespace SohGui
