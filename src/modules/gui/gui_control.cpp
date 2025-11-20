#include "bas/gui_control.hpp"
#include "raylib.h"
#include <stdexcept>
#include <iostream>

namespace bas {

// GuiControl Base Class Implementation
GuiControl::GuiControl(int control_id, const std::string& control_name, GuiControlType control_type, 
                       const Rectangle& control_bounds)
    : id(control_id), name(control_name), type(control_type), bounds(control_bounds),
      state(GuiControlState::NORMAL), visible(true), enabled(true), text(""),
      textColor(0, 0, 0, 255), backgroundColor(255, 255, 255, 255), borderColor(128, 128, 128, 255),
      fontSize(10), textAlignment(GuiTextAlignment::LEFT), 
      textAlignmentVertical(GuiTextAlignmentVertical::TOP), textWrapMode(GuiTextWrapMode::NONE) {
}

} // namespace bas
