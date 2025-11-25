#include "bas/gui_layout.hpp"
#include "bas/gui_manager.hpp"
#include <algorithm>
#include <iostream>

namespace bas {

// GuiLayout Implementation
GuiLayout::GuiLayout(LayoutType layout_type, const Rectangle& layout_bounds)
    : type(layout_type), bounds(layout_bounds), spacing(5), alignment(Alignment::START) {
}

void GuiLayout::add_item(int control_id, const LayoutConstraints& constraints) {
    items.emplace_back(control_id);
    items.back().constraints = constraints;
}

void GuiLayout::remove_item(int control_id) {
    items.erase(std::remove_if(items.begin(), items.end(),
        [control_id](const LayoutItem& item) { return item.control_id == control_id; }),
        items.end());
}

void GuiLayout::clear_items() {
    items.clear();
}

// GuiHorizontalLayout Implementation
GuiHorizontalLayout::GuiHorizontalLayout(const Rectangle& bounds)
    : GuiLayout(LayoutType::HORIZONTAL, bounds) {
}

void GuiHorizontalLayout::calculate_layout() {
    if (items.empty()) return;
    
    double total_width = bounds.width;
    double total_spacing = spacing * (items.size() - 1);
    double available_width = total_width - total_spacing;
    
    // Calculate total preferred width
    double total_preferred = 0;
    for (const auto& item : items) {
        if (item.constraints.preferred_width > 0) {
            total_preferred += item.constraints.preferred_width;
        } else {
            total_preferred += 100; // Default width
        }
    }
    
    // Distribute space
    double current_x = bounds.x;
    for (auto& item : items) {
        double item_width;
        if (item.constraints.preferred_width > 0) {
            item_width = item.constraints.preferred_width;
        } else {
            item_width = available_width / items.size();
        }
        
        // Apply constraints
        if (item.constraints.min_width > 0) {
            item_width = std::max(item_width, item.constraints.min_width);
        }
        if (item.constraints.max_width > 0) {
            item_width = std::min(item_width, item.constraints.max_width);
        }
        
        // Set position
        auto control = GuiManager::get_control(item.control_id);
        if (control) {
            Rectangle new_bounds = control->get_bounds();
            new_bounds.x = current_x + item.constraints.margin_left;
            new_bounds.width = item_width - item.constraints.margin_left - item.constraints.margin_right;
            new_bounds.y = bounds.y + item.constraints.margin_top;
            new_bounds.height = bounds.height - item.constraints.margin_top - item.constraints.margin_bottom;
            control->set_bounds(new_bounds);
        }
        
        current_x += item_width + spacing;
    }
}

void GuiHorizontalLayout::apply_layout() {
    calculate_layout();
}

// GuiVerticalLayout Implementation
GuiVerticalLayout::GuiVerticalLayout(const Rectangle& bounds)
    : GuiLayout(LayoutType::VERTICAL, bounds) {
}

void GuiVerticalLayout::calculate_layout() {
    if (items.empty()) return;
    
    double total_height = bounds.height;
    double total_spacing = spacing * (items.size() - 1);
    double available_height = total_height - total_spacing;
    
    // Calculate total preferred height
    double total_preferred = 0;
    for (const auto& item : items) {
        if (item.constraints.preferred_height > 0) {
            total_preferred += item.constraints.preferred_height;
        } else {
            total_preferred += 30; // Default height
        }
    }
    
    // Distribute space
    double current_y = bounds.y;
    for (auto& item : items) {
        double item_height;
        if (item.constraints.preferred_height > 0) {
            item_height = item.constraints.preferred_height;
        } else {
            item_height = available_height / items.size();
        }
        
        // Apply constraints
        if (item.constraints.min_height > 0) {
            item_height = std::max(item_height, item.constraints.min_height);
        }
        if (item.constraints.max_height > 0) {
            item_height = std::min(item_height, item.constraints.max_height);
        }
        
        // Set position
        auto control = GuiManager::get_control(item.control_id);
        if (control) {
            Rectangle new_bounds = control->get_bounds();
            new_bounds.x = bounds.x + item.constraints.margin_left;
            new_bounds.width = bounds.width - item.constraints.margin_left - item.constraints.margin_right;
            new_bounds.y = current_y + item.constraints.margin_top;
            new_bounds.height = item_height - item.constraints.margin_top - item.constraints.margin_bottom;
            control->set_bounds(new_bounds);
        }
        
        current_y += item_height + spacing;
    }
}

void GuiVerticalLayout::apply_layout() {
    calculate_layout();
}

// GuiGridLayout Implementation
GuiGridLayout::GuiGridLayout(const Rectangle& bounds, int cols, int rws)
    : GuiLayout(LayoutType::GRID, bounds), columns(cols), rows(rws) {
}

void GuiGridLayout::calculate_layout() {
    if (items.empty()) return;
    
    double cell_width = bounds.width / columns;
    double cell_height = bounds.height / rows;
    
    for (size_t i = 0; i < items.size(); ++i) {
        int row = static_cast<int>(i) / columns;
        int col = static_cast<int>(i) % columns;
        
        auto& item = items[i];
        auto control = GuiManager::get_control(item.control_id);
        if (control) {
            Rectangle new_bounds = control->get_bounds();
            new_bounds.x = bounds.x + col * cell_width + item.constraints.margin_left;
            new_bounds.y = bounds.y + row * cell_height + item.constraints.margin_top;
            new_bounds.width = cell_width - item.constraints.margin_left - item.constraints.margin_right;
            new_bounds.height = cell_height - item.constraints.margin_top - item.constraints.margin_bottom;
            control->set_bounds(new_bounds);
        }
    }
}

void GuiGridLayout::apply_layout() {
    calculate_layout();
}

// GuiLayoutManager Implementation
std::vector<std::unique_ptr<GuiLayout>> GuiLayoutManager::layouts;
int GuiLayoutManager::next_layout_id = 1;

int GuiLayoutManager::create_horizontal_layout(const Rectangle& bounds) {
    int id = next_layout_id++;
    layouts.push_back(std::make_unique<GuiHorizontalLayout>(bounds));
    return id;
}

int GuiLayoutManager::create_vertical_layout(const Rectangle& bounds) {
    int id = next_layout_id++;
    layouts.push_back(std::make_unique<GuiVerticalLayout>(bounds));
    return id;
}

int GuiLayoutManager::create_grid_layout(const Rectangle& bounds, int columns, int rows) {
    int id = next_layout_id++;
    layouts.push_back(std::make_unique<GuiGridLayout>(bounds, columns, rows));
    return id;
}

GuiLayout* GuiLayoutManager::get_layout(int layout_id) {
    if (layout_id > 0 && layout_id <= static_cast<int>(layouts.size())) {
        return layouts[layout_id - 1].get();
    }
    return nullptr;
}

void GuiLayoutManager::destroy_layout(int layout_id) {
    if (layout_id > 0 && layout_id <= static_cast<int>(layouts.size())) {
        layouts.erase(layouts.begin() + layout_id - 1);
    }
}

void GuiLayoutManager::destroy_all_layouts() {
    layouts.clear();
    next_layout_id = 1;
}

void GuiLayoutManager::add_control_to_layout(int layout_id, int control_id, const LayoutConstraints& constraints) {
    auto layout = get_layout(layout_id);
    if (layout) {
        layout->add_item(control_id, constraints);
    }
}

void GuiLayoutManager::remove_control_from_layout(int layout_id, int control_id) {
    auto layout = get_layout(layout_id);
    if (layout) {
        layout->remove_item(control_id);
    }
}

void GuiLayoutManager::update_layout(int layout_id) {
    auto layout = get_layout(layout_id);
    if (layout) {
        layout->apply_layout();
    }
}

void GuiLayoutManager::update_all_layouts() {
    for (auto& layout : layouts) {
        if (layout) {
            layout->apply_layout();
        }
    }
}

std::vector<int> GuiLayoutManager::get_all_layout_ids() {
    std::vector<int> ids;
    for (size_t i = 0; i < layouts.size(); ++i) {
        ids.push_back(static_cast<int>(i + 1));
    }
    return ids;
}

int GuiLayoutManager::get_layout_count() {
    return static_cast<int>(layouts.size());
}

} // namespace bas
