#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "value.hpp"

namespace bas {

struct AnimationFrame {
    int frame_index;
    double x, y, width, height;
    double duration;
    
    AnimationFrame(int idx, double px, double py, double w, double h, double dur)
        : frame_index(idx), x(px), y(py), width(w), height(h), duration(dur) {}
};

class Animation {
private:
    int id;
    std::string name;
    int sprite_id;
    std::vector<AnimationFrame> frames;
    bool loop;
    double current_time;
    int current_frame;
    bool playing;
    
public:
    Animation(int anim_id, const std::string& anim_name, int spr_id)
        : id(anim_id), name(anim_name), sprite_id(spr_id), loop(false), 
          current_time(0.0), current_frame(0), playing(false) {}
    
    int get_id() const { return id; }
    const std::string& get_name() const { return name; }
    int get_sprite_id() const { return sprite_id; }
    
    void add_frame(int frame_index, double x, double y, double width, double height, double duration);
    void set_loop(bool should_loop) { loop = should_loop; }
    bool is_looping() const { return loop; }
    
    void play() { playing = true; current_time = 0.0; current_frame = 0; }
    void stop() { playing = false; }
    void pause() { playing = false; }
    void resume() { playing = true; }
    
    bool is_playing() const { return playing; }
    int get_current_frame() const { return current_frame; }
    
    void update(double delta_time);
    AnimationFrame get_current_frame_data() const;
    
    size_t get_frame_count() const { return frames.size(); }
};

struct Sprite {
    int id;
    std::string name;
    int texture_id;
    double width, height;
    double origin_x, origin_y;
    double x, y, z;
    double rotation;
    double scale_x, scale_y;
    bool visible;
    int current_animation_id;
    
    Sprite(int sprite_id, const std::string& sprite_name, int tex_id, double w, double h)
        : id(sprite_id), name(sprite_name), texture_id(tex_id), width(w), height(h),
          origin_x(0.0), origin_y(0.0), x(0.0), y(0.0), z(0.0), rotation(0.0),
          scale_x(1.0), scale_y(1.0), visible(true), current_animation_id(-1) {}
    
    int get_id() const { return id; }
    const std::string& get_name() const { return name; }
    int get_texture_id() const { return texture_id; }
    
    void set_origin(double ox, double oy) { origin_x = ox; origin_y = oy; }
    void set_position(double px, double py, double pz = 0.0) { x = px; y = py; z = pz; }
    void set_rotation(double rot) { rotation = rot; }
    void set_scale(double sx, double sy) { scale_x = sx; scale_y = sy; }
    void set_visible(bool vis) { visible = vis; }
    
    double get_x() const { return x; }
    double get_y() const { return y; }
    double get_z() const { return z; }
    double get_rotation() const { return rotation; }
    double get_scale_x() const { return scale_x; }
    double get_scale_y() const { return scale_y; }
    bool is_visible() const { return visible; }
};

class SpriteAnimationSystem {
private:
    static int next_sprite_id;
    static int next_animation_id;
    static std::unordered_map<int, std::shared_ptr<Sprite>> sprites;
    static std::unordered_map<int, std::shared_ptr<Animation>> animations;
    
public:
    // Sprite management
    static int create_sprite(const std::string& name, int texture_id, double width, double height);
    static std::shared_ptr<Sprite> get_sprite(int sprite_id);
    static void destroy_sprite(int sprite_id);
    
    // Animation management
    static int create_animation(const std::string& name, int sprite_id);
    static std::shared_ptr<Animation> get_animation(int animation_id);
    static void destroy_animation(int animation_id);
    
    // Animation control
    static void play_animation(int sprite_id, int animation_id);
    static void stop_animation(int sprite_id);
    static void pause_animation(int sprite_id);
    static void resume_animation(int sprite_id);
    
    // System update
    static void update_all_animations(double delta_time);
    
    // Cleanup
    static void shutdown();
};

} // namespace bas
