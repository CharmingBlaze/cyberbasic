#include "bas/raygui.hpp"
#include "bas/value.hpp"
#include "bas/builtins.hpp"
#include "bas/raygui_helpers.hpp"
#include "bas/raymath_helpers.hpp"
#include "raylib.h"
#include "raymath.h"  // For TextToFloat and other utility functions
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
// Note: RAYGUI_IMPLEMENTATION is defined in raygui_impl.cpp
// This file only uses the header declarations
#include "raygui.h"
#pragma GCC diagnostic pop
#include <vector>
#include <string>
#include <cstring>

namespace bas {

// Helper to convert string array to char**
inline std::vector<const char*> string_array_to_charptr(const std::vector<Value>& args, size_t start_idx, size_t count) {
    std::vector<const char*> result;
    for (size_t i = 0; i < count && (start_idx + i) < args.size(); ++i) {
        result.push_back(args[start_idx + i].as_string().c_str());
    }
    return result;
}

void register_raygui_functions(FunctionRegistry& R) {
    // ========================================================================
    // System Functions
    // ========================================================================
    R.add_with_policy("GUIENABLE", NativeFn{"GUIENABLE", 0, [](const std::vector<Value>&) {
        GuiEnable();
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIDISABLE", NativeFn{"GUIDISABLE", 0, [](const std::vector<Value>&) {
        GuiDisable();
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUILOCK", NativeFn{"GUILOCK", 0, [](const std::vector<Value>&) {
        GuiLock();
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIUNLOCK", NativeFn{"GUIUNLOCK", 0, [](const std::vector<Value>&) {
        GuiUnlock();
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUISLOCKED", NativeFn{"GUISLOCKED", 0, [](const std::vector<Value>&) {
        return Value::from_bool(GuiIsLocked());
    }}, true);
    
    R.add_with_policy("GUISETALPHA", NativeFn{"GUISETALPHA", 1, [](const std::vector<Value>& a) {
        GuiSetAlpha(static_cast<float>(a[0].as_number()));
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUISETSTATE", NativeFn{"GUISETSTATE", 1, [](const std::vector<Value>& a) {
        GuiSetState(static_cast<int>(a[0].as_int()));
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIGETSTATE", NativeFn{"GUIGETSTATE", 0, [](const std::vector<Value>&) {
        return Value::from_int(GuiGetState());
    }}, true);
    
    R.add_with_policy("GUISETSTYLE", NativeFn{"GUISETSTYLE", 3, [](const std::vector<Value>& a) {
        GuiSetStyle(static_cast<int>(a[0].as_int()), static_cast<int>(a[1].as_int()), static_cast<int>(a[2].as_int()));
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIGETSTYLE", NativeFn{"GUIGETSTYLE", 2, [](const std::vector<Value>& a) {
        return Value::from_int(GuiGetStyle(static_cast<int>(a[0].as_int()), static_cast<int>(a[1].as_int())));
    }}, true);
    
    R.add_with_policy("GUISETFONT", NativeFn{"GUISETFONT", 1, [](const std::vector<Value>& a) {
        // Font is stored by ID in rlreg::fonts, but we need to access it
        // For now, use GetFontDefault() if font ID is 0, otherwise we'd need access to rlreg
        // This is a limitation - in production, we'd need a font registry accessor
        Font font = GetFontDefault();
        if (a[0].as_int() != 0) {
            // Try to get font from registry - this requires access to rlreg namespace
            // For production, we'll use default font and note this limitation
            font = GetFontDefault();
        }
        GuiSetFont(font);
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIGETFONT", NativeFn{"GUIGETFONT", 0, [](const std::vector<Value>&) {
        Font font = GuiGetFont();
        // Return font ID - in practice, we'd need to look up or create an ID
        // For now, return 0 (default font)
        return Value::from_int(0);
    }}, true);
    
    R.add_with_policy("GUILOADSTYLE", NativeFn{"GUILOADSTYLE", 1, [](const std::vector<Value>& a) {
        GuiLoadStyle(a[0].as_string().c_str());
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUILOADSTYLEDEFAULT", NativeFn{"GUILOADSTYLEDEFAULT", 0, [](const std::vector<Value>&) {
        GuiLoadStyleDefault();
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIGETTEXTWIDTH", NativeFn{"GUIGETTEXTWIDTH", 1, [](const std::vector<Value>& a) {
        return Value::from_int(GuiGetTextWidth(a[0].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUISETICONSCALE", NativeFn{"GUISETICONSCALE", 1, [](const std::vector<Value>& a) {
        GuiSetIconScale(static_cast<int>(a[0].as_int()));
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIDRAWICON", NativeFn{"GUIDRAWICON", 5, [](const std::vector<Value>& a) {
        GuiDrawIcon(static_cast<int>(a[0].as_int()), 
                    static_cast<int>(a[1].as_int()), 
                    static_cast<int>(a[2].as_int()), 
                    static_cast<int>(a[3].as_int()), 
                    Value_to_Color(a[4]));
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIENABLETOOLTIP", NativeFn{"GUIENABLETOOLTIP", 0, [](const std::vector<Value>&) {
        GuiEnableTooltip();
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUIDISABLETOOLTIP", NativeFn{"GUIDISABLETOOLTIP", 0, [](const std::vector<Value>&) {
        GuiDisableTooltip();
        return Value::from_bool(true);
    }}, true);
    
    R.add_with_policy("GUISETTOOLTIP", NativeFn{"GUISETTOOLTIP", 1, [](const std::vector<Value>& a) {
        GuiSetTooltip(a[0].as_string().c_str());
        return Value::from_bool(true);
    }}, true);

    // ========================================================================
    // Container/Separator Controls
    // ========================================================================
    R.add_with_policy("GUIWINDOWBOX", NativeFn{"GUIWINDOWBOX", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        return Value::from_int(GuiWindowBox(bounds, a[4].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUIGROUPBOX", NativeFn{"GUIGROUPBOX", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        return Value::from_int(GuiGroupBox(bounds, a[4].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUILINE", NativeFn{"GUILINE", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        return Value::from_int(GuiLine(bounds, a[4].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUIPANEL", NativeFn{"GUIPANEL", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        const char* text = a[4].is_nil() ? nullptr : a[4].as_string().c_str();
        return Value::from_int(GuiPanel(bounds, text));
    }}, true);
    
    R.add_with_policy("GUISCROLLPANEL", NativeFn{"GUISCROLLPANEL", 9, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        Rectangle content = make_rectangle(
            static_cast<float>(a[4].as_number()),
            static_cast<float>(a[5].as_number()),
            static_cast<float>(a[6].as_number()),
            static_cast<float>(a[7].as_number())
        );
        Vector2 scroll = {0, 0};
        Rectangle view = {0, 0, 0, 0};
        int result = GuiScrollPanel(bounds, a[8].as_string().c_str(), content, &scroll, &view);
        // Note: scroll and view are modified but we can't return them easily
        // User would need to use separate functions to get these values
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUITABBAR", NativeFn{"GUITABBAR", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        // TabBar: args[4] = array of strings, args[5] = active index (in/out)
        if (a.size() < 6) throw std::runtime_error("GUITABBAR: expected 6 args (x, y, width, height, textArray, activeIndex)");
        
        // Extract string array - store strings to keep them alive
        std::vector<std::string> textStrings;
        std::vector<const char*> textArray;
        if (a[4].is_map()) {
            auto& map = a[4].as_map();
            for (size_t i = 0; i < map.size(); ++i) {
                auto key = std::to_string(i);
                if (map.find(key) != map.end()) {
                    textStrings.push_back(map.at(key).as_string());
                    textArray.push_back(textStrings.back().c_str());
                }
            }
        } else if (a[4].is_array()) {
            auto& list = a[4].as_array();
            for (const auto& item : list) {
                textStrings.push_back(item.as_string());
                textArray.push_back(textStrings.back().c_str());
            }
        } else {
            throw std::runtime_error("GUITABBAR: textArray must be a map or list");
        }
        
        int active = static_cast<int>(a[5].as_int());
        int result = GuiTabBar(bounds, textArray.data(), static_cast<int>(textArray.size()), &active);
        // Note: active is modified but we return result (tab close index)
        return Value::from_int(result);
    }}, true);

    // ========================================================================
    // Basic Controls
    // ========================================================================
    R.add_with_policy("GUILABEL", NativeFn{"GUILABEL", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        return Value::from_int(GuiLabel(bounds, a[4].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUIBUTTON", NativeFn{"GUIBUTTON", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        return Value::from_int(GuiButton(bounds, a[4].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUILABELBUTTON", NativeFn{"GUILABELBUTTON", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        return Value::from_int(GuiLabelButton(bounds, a[4].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUITOGGLE", NativeFn{"GUITOGGLE", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        bool active = a[4].as_bool();
        int result = GuiToggle(bounds, "", &active);
        // Note: active is modified but we can't return it easily
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUITOGGLEGROUP", NativeFn{"GUITOGGLEGROUP", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        // ToggleGroup: args[4] = text string (semicolon-separated), args[5] = active index (in/out)
        if (a.size() < 6) throw std::runtime_error("GUITOGGLEGROUP: expected 6 args (x, y, width, height, textString, activeIndex)");
        
        std::string textStr = a[4].as_string();
        int active = static_cast<int>(a[5].as_int());
        int result = GuiToggleGroup(bounds, textStr.c_str(), &active);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUITOGGLESLIDER", NativeFn{"GUITOGGLESLIDER", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        int active = static_cast<int>(a[4].as_int());
        int result = GuiToggleSlider(bounds, "", &active);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUICHECKBOX", NativeFn{"GUICHECKBOX", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        bool checked = a[4].as_bool();
        int result = GuiCheckBox(bounds, "", &checked);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUICOMBOBOX", NativeFn{"GUICOMBOBOX", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        int active = static_cast<int>(a[4].as_int());
        int result = GuiComboBox(bounds, "", &active);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUIDROPDOWNBOX", NativeFn{"GUIDROPDOWNBOX", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        int active = static_cast<int>(a[4].as_int());
        bool editMode = a[5].as_bool();
        int result = GuiDropdownBox(bounds, "", &active, editMode);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUITEXTBOX", NativeFn{"GUITEXTBOX", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        // TextBox requires a mutable char buffer - this is complex
        // For now, we'll need a wrapper that manages the buffer
        std::string text = a[4].as_string();
        int textSize = static_cast<int>(a[5].as_int());
        if (textSize <= 0) textSize = 256;
        std::vector<char> buffer(textSize);
        strncpy(buffer.data(), text.c_str(), textSize - 1);
        buffer[textSize - 1] = '\0';
        bool editMode = true;
        int result = GuiTextBox(bounds, buffer.data(), textSize, editMode);
        // Note: buffer is modified but we can't return it easily
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUITEXTBOXMULTI", NativeFn{"GUITEXTBOXMULTI", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        std::string text = a[4].as_string();
        int textSize = static_cast<int>(a[5].as_int());
        if (textSize <= 0) textSize = 256;
        std::vector<char> buffer(textSize);
        strncpy(buffer.data(), text.c_str(), textSize - 1);
        buffer[textSize - 1] = '\0';
        bool editMode = true;
        bool result = GuiTextBox(bounds, buffer.data(), textSize, editMode);
        return Value::from_bool(result);
    }}, true);
    
    R.add_with_policy("GUIVALUEBOX", NativeFn{"GUIVALUEBOX", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        int value = static_cast<int>(a[4].as_int());
        int minValue = static_cast<int>(a[5].as_int());
        int maxValue = static_cast<int>(a[6].as_int());
        bool editMode = true;
        int result = GuiValueBox(bounds, "", &value, minValue, maxValue, editMode);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUIVALUEBOXFLOAT", NativeFn{"GUIVALUEBOXFLOAT", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        float value = static_cast<float>(a[4].as_number());
        bool editMode = true;
        std::string textValue = std::to_string(value);
        std::vector<char> buffer(32);
        strncpy(buffer.data(), textValue.c_str(), 31);
        buffer[31] = '\0';
        int result = GuiValueBoxFloat(bounds, "", buffer.data(), &value, editMode);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUISPINNER", NativeFn{"GUISPINNER", 7, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        int value = static_cast<int>(a[4].as_int());
        int minValue = static_cast<int>(a[5].as_int());
        int maxValue = static_cast<int>(a[6].as_int());
        bool editMode = true;
        int result = GuiSpinner(bounds, "", &value, minValue, maxValue, editMode);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUISLIDER", NativeFn{"GUISLIDER", 7, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        float value = static_cast<float>(a[4].as_number());
        float minValue = static_cast<float>(a[5].as_number());
        float maxValue = static_cast<float>(a[6].as_number());
        int result = GuiSlider(bounds, "", "", &value, minValue, maxValue);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUISLIDERBAR", NativeFn{"GUISLIDERBAR", 7, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        float value = static_cast<float>(a[4].as_number());
        float minValue = static_cast<float>(a[5].as_number());
        float maxValue = static_cast<float>(a[6].as_number());
        int result = GuiSliderBar(bounds, "", "", &value, minValue, maxValue);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUIPROGRESSBAR", NativeFn{"GUIPROGRESSBAR", 7, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        float value = static_cast<float>(a[4].as_number());
        float minValue = static_cast<float>(a[5].as_number());
        float maxValue = static_cast<float>(a[6].as_number());
        int result = GuiProgressBar(bounds, "", "", &value, minValue, maxValue);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUISTATUSBAR", NativeFn{"GUISTATUSBAR", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        return Value::from_int(GuiStatusBar(bounds, a[4].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUIDUMMYREC", NativeFn{"GUIDUMMYREC", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        return Value::from_int(GuiDummyRec(bounds, a[4].as_string().c_str()));
    }}, true);
    
    R.add_with_policy("GUIGRID", NativeFn{"GUIGRID", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        float spacing = static_cast<float>(a[4].as_number());
        int subdivs = static_cast<int>(a[5].as_int());
        Vector2 mouseCell = {0, 0};
        int result = GuiGrid(bounds, "", spacing, subdivs, &mouseCell);
        return Value::from_int(result);
    }}, true);

    // ========================================================================
    // Advanced Controls
    // ========================================================================
    R.add_with_policy("GUILISTVIEW", NativeFn{"GUILISTVIEW", 6, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        int scrollIndex = static_cast<int>(a[4].as_int());
        int active = static_cast<int>(a[5].as_int());
        int result = GuiListView(bounds, "", &scrollIndex, &active);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUILISTVIEWEX", NativeFn{"GUILISTVIEWEX", 8, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        // ListViewEx: args[4] = text array, args[5] = scrollIndex (in/out), args[6] = active (in/out), args[7] = focus (in/out)
        if (a.size() < 8) throw std::runtime_error("GUILISTVIEWEX: expected 8 args (x, y, width, height, textArray, scrollIndex, active, focus)");
        
        // Store strings to keep them alive
        std::vector<std::string> textStrings;
        std::vector<const char*> textArray;
        if (a[4].is_map()) {
            auto& map = a[4].as_map();
            for (size_t i = 0; i < map.size(); ++i) {
                auto key = std::to_string(i);
                if (map.find(key) != map.end()) {
                    textStrings.push_back(map.at(key).as_string());
                    textArray.push_back(textStrings.back().c_str());
                }
            }
        } else if (a[4].is_array()) {
            auto& list = a[4].as_array();
            for (const auto& item : list) {
                textStrings.push_back(item.as_string());
                textArray.push_back(textStrings.back().c_str());
            }
        } else {
            throw std::runtime_error("GUILISTVIEWEX: textArray must be a map or list");
        }
        
        int scrollIndex = static_cast<int>(a[5].as_int());
        int active = static_cast<int>(a[6].as_int());
        int focus = static_cast<int>(a[7].as_int());
        int result = GuiListViewEx(bounds, textArray.data(), static_cast<int>(textArray.size()), &scrollIndex, &active, &focus);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUICOLORPICKER", NativeFn{"GUICOLORPICKER", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        Color color = Value_to_Color(a[4]);
        int result = GuiColorPicker(bounds, "", &color);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUICOLORPICKERHSV", NativeFn{"GUICOLORPICKERHSV", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        Vector3 colorHsv = Value_to_Vector3(a[4]);
        int result = GuiColorPickerHSV(bounds, "", &colorHsv);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUICOLORPANEL", NativeFn{"GUICOLORPANEL", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        Color color = Value_to_Color(a[4]);
        int result = GuiColorPanel(bounds, "", &color);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUICOLORPANELHSV", NativeFn{"GUICOLORPANELHSV", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        Vector3 colorHsv = Value_to_Vector3(a[4]);
        int result = GuiColorPanelHSV(bounds, "", &colorHsv);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUICOLORBARALPHA", NativeFn{"GUICOLORBARALPHA", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        float alpha = static_cast<float>(a[4].as_number());
        int result = GuiColorBarAlpha(bounds, "", &alpha);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUICOLORBARHUE", NativeFn{"GUICOLORBARHUE", 5, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        float hue = static_cast<float>(a[4].as_number());
        int result = GuiColorBarHue(bounds, "", &hue);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUIMESSAGEBOX", NativeFn{"GUIMESSAGEBOX", 7, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        // MessageBox: args[4] = title, args[5] = message, args[6] = buttons (semicolon-separated)
        if (a.size() < 7) throw std::runtime_error("GUIMESSAGEBOX: expected 7 args (x, y, width, height, title, message, buttons)");
        const char* title = a[4].as_string().c_str();
        const char* message = a[5].as_string().c_str();
        const char* buttons = a[6].as_string().c_str();
        int result = GuiMessageBox(bounds, title, message, buttons);
        return Value::from_int(result);
    }}, true);
    
    R.add_with_policy("GUITEXTINPUTBOX", NativeFn{"GUITEXTINPUTBOX", 7, [](const std::vector<Value>& a) {
        Rectangle bounds = make_rectangle(
            static_cast<float>(a[0].as_number()),
            static_cast<float>(a[1].as_number()),
            static_cast<float>(a[2].as_number()),
            static_cast<float>(a[3].as_number())
        );
        // TextInputBox: args[4] = title, args[5] = message, args[6] = buttons, args[7] = text (in/out), args[8] = textMaxSize, args[9] = secretViewActive (in/out)
        if (a.size() < 10) throw std::runtime_error("GUITEXTINPUTBOX: expected 10 args (x, y, width, height, title, message, buttons, text, textMaxSize, secretViewActive)");
        const char* title = a[4].as_string().c_str();
        const char* message = a[5].as_string().c_str();
        const char* buttons = a[6].as_string().c_str();
        std::string text = a[7].as_string();
        int textMaxSize = static_cast<int>(a[8].as_int());
        if (textMaxSize <= 0) textMaxSize = 256;
        std::vector<char> buffer(textMaxSize);
        strncpy(buffer.data(), text.c_str(), textMaxSize - 1);
        buffer[textMaxSize - 1] = '\0';
        bool secretViewActive = a[9].as_bool();
        int result = GuiTextInputBox(bounds, title, message, buttons, buffer.data(), textMaxSize, &secretViewActive);
        // Note: buffer and secretViewActive are modified but we can't return them easily
        return Value::from_int(result);
    }}, true);
}

} // namespace bas

