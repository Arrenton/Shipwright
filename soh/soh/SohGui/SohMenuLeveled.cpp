#include "SohMenu.h"
#include "SohGui.hpp"
#include <macros.h>

namespace SohGui {
enum LeveledUIMagicNumberType {
    LEVELED_MAGIC_NUMBER_UI_FULL,
    LEVELED_MAGIC_NUMBER_UI_CURRENT,
    LEVELED_MAGIC_NUMBER_UI_NONE
};
static const std::map<int32_t, const char*> magicNumberTypeList = { { LEVELED_MAGIC_NUMBER_UI_FULL, "Full" },
                                                                    { LEVELED_MAGIC_NUMBER_UI_CURRENT, "Current" },
                                                                    { LEVELED_MAGIC_NUMBER_UI_NONE, "None" } };

extern std::shared_ptr<SohMenu> mSohMenu;
using namespace UIWidgets;

void SohMenu::AddMenuLeveled() {
    // Add Leveled Menu
    AddMenuEntry("Leveled", CVAR_SETTING("Menu.LeveledSidebarSection"));

    //////////////////////////////////////////////////////////////////////////////////////////
    // UI Settings
    WidgetPath path = { "Leveled", "UI Settings", SECTION_COLUMN_1 };
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
        .Options(CheckboxOptions().Tooltip("Levels give health upgrades, up to 10 extra hearts.").DefaultValue(true));
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
        .Options(CheckboxOptions()
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
        ImGui::Text("Heart Container Value in Units: %d (%.2fx)",
                    CVarGetInteger("gLeveled.Difficulty.HeartUnits", 4) << 2,
                    (float)CVarGetInteger("gLeveled.Difficulty.HeartUnits", 4) / 4.0f);
        UIWidgets::CVarSliderInt("Heart Container Value in Units", "gLeveled.Difficulty.HeartUnits",
                                 IntSliderOptions()
                                     .LabelPosition(Within)
                                     .Min(1)
                                     .Max(32)
                                     .Format("")
                                     .DefaultValue(4)
                                     .Color(THEME_COLOR)
                                     .Tooltip("Sets how many health units each completed heart container is worth.\n"
                                              "One heart on the health meter is equal to 16 health units.\n"
                                              "A lower setting will result in lower total health.\n"
                                              "Change areas to update health capacity."));
    });
    // Damage Multiplier
    AddWidget(path, ("Damage Multiplier: "), WIDGET_CUSTOM).CustomFunction([](WidgetInfo& info) {
        ImGui::Text("Damage Multiplier: %.2fx",
                    (float)CVarGetInteger("gLeveled.Difficulty.Player.DamageMultiplier", 4) / 4.0f);
        UIWidgets::CVarSliderInt(
            "Leveled Damage Multiplier", "gLeveled.Difficulty.Player.DamageMultiplier",
            IntSliderOptions()
                .LabelPosition(Within)
                .Min(1)
                .Max(32)
                .Format("")
                .DefaultValue(4)
                .Color(THEME_COLOR)
                .Tooltip("Sets a multiplier for the damage the player takes. Includes ALL sources, even damage while "
                         "being frozen or burned.\nDamage cannot be reduced below 1."));
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
        .Options(FloatSliderOptions().Min(0.0f).Max(10.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
            "Sets the HP multiplier for enemies."));
    // Level Scale
    AddWidget(path, "Level Scale: %.2fx", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gLeveled.Difficulty.Enemy.LevelScale")
        .Options(FloatSliderOptions().Min(0.0f).Max(10.0f).Format("%.2fx").DefaultValue(1.0f).Tooltip(
            "Sets the multiplier for enemy levels.\nFor example, if set to 150%, a level 20 enemy will be level "
            "30.\nCaps at level 99."));
}

} // namespace SohGui
