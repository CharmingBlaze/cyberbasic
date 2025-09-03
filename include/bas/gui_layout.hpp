#pragma once
#include "gui_types.hpp"
#include <vector>
#include <memory>

namespace bas {

// Layout Types
enum class LayoutType {
    NONE,
    HORIZONTAL,
    VERTICAL,
    GRID,
    ABSOLUTE
};

enum class Alignment {
    START,
    CENTER,
    END,
    STRETCH
};

// Layout Constraints
struct LayoutConstraints {
    double min_width;
    double max_width;
    double min_height;
    double max_height;
    double preferred_width;
    double preferred_height;
    Alignment horizontal_alignment;
    Alignment vertical_alignment;
    double margin_left;
    double margin_right;
    double margin_top;
    double margin_bottom;
    
    LayoutConstraints() 
        : min_width(0), max_width(-1), min_height(0), max_height(-1),
          preferred_width(-1), preferred_height(-1),
          horizontal_alignment(Alignment::START), vertical_alignment(Alignment::START),
          margin_left(0), margin_right(0), margin_top(0), margin_bottom(0) {}
};

// Layout Item
struct LayoutItem {
    int control_id;
    LayoutConstraints constraints;
    
    LayoutItem(int id) : control_id(id) {}
};

// Base Layout Class
class GuiLayout {
protected:
    LayoutType type;
    Rectangle bounds;
    std::vector<LayoutItem> items;
    double spacing;
    Alignment alignment;
    
public:
    GuiLayout(LayoutType layout_type, const Rectangle& layout_bounds);
    virtual ~GuiLayout() = default;
    
    // Item management
    void add_item(int control_id, const LayoutConstraints& constraints = LayoutConstraints());
    void remove_item(int control_id);
    void clear_items();
    
    // Layout properties
    void set_bounds(const Rectangle& new_bounds) { bounds = new_bounds; }
    void set_spacing(double new_spacing) { spacing = new_spacing; }
    void set_alignment(Alignment new_alignment) { alignment = new_alignment; }
    
    // Layout calculation
    virtual void calculate_layout() = 0;
    virtual void apply_layout() = 0;
    
    // Getters
    LayoutType get_type() const { return type; }
    const Rectangle& get_bounds() const { return bounds; }
    double get_spacing() const { return spacing; }
    Alignment get_alignment() const { return alignment; }
    const std::vector<LayoutItem>& get_items() const { return items; }
};

// Specific Layout Implementations
class GuiHorizontalLayout : public GuiLayout {
public:
    GuiHorizontalLayout(const Rectangle& bounds);
    void calculate_layout() override;
    void apply_layout() override;
};

class GuiVerticalLayout : public GuiLayout {
public:
    GuiVerticalLayout(const Rectangle& bounds);
    void calculate_layout() override;
    void apply_layout() override;
};

class GuiGridLayout : public GuiLayout {
private:
    int columns;
    int rows;
    
public:
    GuiGridLayout(const Rectangle& bounds, int cols, int rws);
    void calculate_layout() override;
    void apply_layout() override;
    void set_columns(int cols) { columns = cols; }
    void set_rows(int rws) { rows = rws; }
    int get_columns() const { return columns; }
    int get_rows() const { return rows; }
};

// Layout Manager
class GuiLayoutManager {
private:
    static std::vector<std::unique_ptr<GuiLayout>> layouts;
    static int next_layout_id;
    
public:
    // Layout creation
    static int create_horizontal_layout(const Rectangle& bounds);
    static int create_vertical_layout(const Rectangle& bounds);
    static int create_grid_layout(const Rectangle& bounds, int columns, int rows);
    
    // Layout management
    static GuiLayout* get_layout(int layout_id);
    static void destroy_layout(int layout_id);
    static void destroy_all_layouts();
    
    // Layout operations
    static void add_control_to_layout(int layout_id, int control_id, const LayoutConstraints& constraints = LayoutConstraints());
    static void remove_control_from_layout(int layout_id, int control_id);
    static void update_layout(int layout_id);
    static void update_all_layouts();
    
    // Utility functions
    static std::vector<int> get_all_layout_ids();
    static int get_layout_count();
};

} // namespace bas
