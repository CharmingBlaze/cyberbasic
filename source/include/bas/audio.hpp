#pragma once

#include "value.hpp"
#include "runtime.hpp"
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <cmath>

namespace bas {

// Forward declarations
struct AudioSource;
struct AudioListener;
struct MusicTrack;
class AudioSystem;

// Audio source types
enum class AudioSourceType {
    SOUND_EFFECT,
    AMBIENT,
    VOICE,
    MUSIC,
    CUSTOM
};

// Audio source structure
struct AudioSource {
    int id;
    std::string name;
    AudioSourceType type;
    std::string file_path;
    float volume;
    float pitch;
    float pan;
    bool loop;
    bool playing;
    bool paused;
    float position_x, position_y, position_z;
    float velocity_x, velocity_y, velocity_z;
    float min_distance;
    float max_distance;
    float rolloff_factor;
    float cone_inner_angle;
    float cone_outer_angle;
    float cone_outer_gain;
    
    AudioSource(int id, const std::string& name, AudioSourceType type)
        : id(id), name(name), type(type), volume(1.0f), pitch(1.0f), pan(0.0f),
          loop(false), playing(false), paused(false),
          position_x(0.0f), position_y(0.0f), position_z(0.0f),
          velocity_x(0.0f), velocity_y(0.0f), velocity_z(0.0f),
          min_distance(1.0f), max_distance(100.0f), rolloff_factor(1.0f),
          cone_inner_angle(360.0f), cone_outer_angle(360.0f), cone_outer_gain(0.0f) {}
};

// Audio listener structure
struct AudioListener {
    int id;
    std::string name;
    float position_x, position_y, position_z;
    float velocity_x, velocity_y, velocity_z;
    float forward_x, forward_y, forward_z;
    float up_x, up_y, up_z;
    float volume;
    bool active;
    
    AudioListener(int id, const std::string& name)
        : id(id), name(name), position_x(0.0f), position_y(0.0f), position_z(0.0f),
          velocity_x(0.0f), velocity_y(0.0f), velocity_z(0.0f),
          forward_x(0.0f), forward_y(0.0f), forward_z(-1.0f),
          up_x(0.0f), up_y(1.0f), up_z(0.0f), volume(1.0f), active(true) {}
};

// Music track structure
struct MusicTrack {
    int id;
    std::string name;
    std::string file_path;
    float volume;
    float pitch;
    bool loop;
    bool playing;
    bool paused;
    float fade_in_time;
    float fade_out_time;
    float crossfade_time;
    
    MusicTrack(int id, const std::string& name, const std::string& file_path)
        : id(id), name(name), file_path(file_path), volume(1.0f), pitch(1.0f),
          loop(true), playing(false), paused(false),
          fade_in_time(0.0f), fade_out_time(0.0f), crossfade_time(0.0f) {}
};

// Audio effect types
enum class AudioEffectType {
    REVERB,
    ECHO,
    CHORUS,
    DISTORTION,
    LOW_PASS,
    HIGH_PASS,
    BAND_PASS,
    COMPRESSOR,
    LIMITER,
    CUSTOM
};

// Audio effect structure
struct AudioEffect {
    int id;
    std::string name;
    AudioEffectType type;
    bool active;
    std::unordered_map<std::string, float> parameters;
    
    AudioEffect(int id, const std::string& name, AudioEffectType type)
        : id(id), name(name), type(type), active(true) {}
};

// Audio system
class AudioSystem {
private:
    std::vector<std::unique_ptr<AudioSource>> audio_sources;
    std::vector<std::unique_ptr<AudioListener>> audio_listeners;
    std::vector<std::unique_ptr<MusicTrack>> music_tracks;
    std::vector<std::unique_ptr<AudioEffect>> audio_effects;
    int next_audio_source_id;
    int next_audio_listener_id;
    int next_music_track_id;
    int next_audio_effect_id;
    float master_volume;
    float music_volume;
    float sfx_volume;
    bool muted;
    bool paused;
    
    // Audio processing
    void update_audio_sources();
    void update_audio_listeners();
    void update_music_tracks();
    void apply_audio_effects();
    void calculate_3d_audio(AudioSource& source, const AudioListener& listener);
    void apply_distance_attenuation(AudioSource& source, const AudioListener& listener);
    void apply_cone_attenuation(AudioSource& source, const AudioListener& listener);
    void apply_doppler_effect(AudioSource& source, const AudioListener& listener);
    
    // Utility functions
    float calculate_distance(float x1, float y1, float z1, float x2, float y2, float z2);
    float calculate_attenuation(float distance, float min_dist, float max_dist, float rolloff);
    float calculate_doppler_shift(float source_velocity, float listener_velocity, float speed_of_sound);
    void normalize_vector(float& x, float& y, float& z);
    float dot_product(float x1, float y1, float z1, float x2, float y2, float z2);
    
public:
    AudioSystem();
    ~AudioSystem();
    
    // Audio source management
    int create_audio_source(const std::string& name, AudioSourceType type);
    void remove_audio_source(int source_id);
    AudioSource* get_audio_source(int source_id);
    bool load_audio_file(int source_id, const std::string& file_path);
    bool play_audio_source(int source_id);
    bool pause_audio_source(int source_id);
    bool stop_audio_source(int source_id);
    void set_audio_source_volume(int source_id, float volume);
    void set_audio_source_pitch(int source_id, float pitch);
    void set_audio_source_pan(int source_id, float pan);
    void set_audio_source_loop(int source_id, bool loop);
    void set_audio_source_position(int source_id, float x, float y, float z);
    void set_audio_source_velocity(int source_id, float x, float y, float z);
    void set_audio_source_distance_range(int source_id, float min_distance, float max_distance);
    void set_audio_source_rolloff_factor(int source_id, float rolloff_factor);
    void set_audio_source_cone(int source_id, float inner_angle, float outer_angle, float outer_gain);
    
    // Audio listener management
    int create_audio_listener(const std::string& name);
    void remove_audio_listener(int listener_id);
    AudioListener* get_audio_listener(int listener_id);
    void set_audio_listener_position(int listener_id, float x, float y, float z);
    void set_audio_listener_velocity(int listener_id, float x, float y, float z);
    void set_audio_listener_orientation(int listener_id, float forward_x, float forward_y, float forward_z, float up_x, float up_y, float up_z);
    void set_audio_listener_volume(int listener_id, float volume);
    void set_audio_listener_active(int listener_id, bool active);
    
    // Music track management
    int create_music_track(const std::string& name, const std::string& file_path);
    void remove_music_track(int track_id);
    MusicTrack* get_music_track(int track_id);
    bool play_music_track(int track_id);
    bool pause_music_track(int track_id);
    bool stop_music_track(int track_id);
    void set_music_track_volume(int track_id, float volume);
    void set_music_track_pitch(int track_id, float pitch);
    void set_music_track_loop(int track_id, bool loop);
    void set_music_track_fade_times(int track_id, float fade_in, float fade_out);
    void crossfade_music_tracks(int from_track_id, int to_track_id, float crossfade_time);
    
    // Audio effect management
    int create_audio_effect(const std::string& name, AudioEffectType type);
    void remove_audio_effect(int effect_id);
    AudioEffect* get_audio_effect(int effect_id);
    void set_audio_effect_active(int effect_id, bool active);
    void set_audio_effect_parameter(int effect_id, const std::string& name, float value);
    void apply_audio_effect_to_source(int effect_id, int source_id);
    void apply_audio_effect_to_music(int effect_id, int track_id);
    
    // System management
    void update();
    void set_master_volume(float volume);
    void set_music_volume(float volume);
    void set_sfx_volume(float volume);
    void set_muted(bool muted);
    void set_paused(bool paused);
    void stop_all_audio();
    void pause_all_audio();
    void resume_all_audio();
    
    // Utility functions
    int get_audio_source_count() const;
    int get_audio_listener_count() const;
    int get_music_track_count() const;
    int get_audio_effect_count() const;
    float get_master_volume() const;
    float get_music_volume() const;
    float get_sfx_volume() const;
    bool is_muted() const;
    bool is_paused() const;
    
    // Pre-built audio sources
    int create_sound_effect(const std::string& name, const std::string& file_path);
    int create_ambient_sound(const std::string& name, const std::string& file_path);
    int create_voice_line(const std::string& name, const std::string& file_path);
    
    // Pre-built audio effects
    int create_reverb_effect(const std::string& name);
    int create_echo_effect(const std::string& name);
    int create_chorus_effect(const std::string& name);
    int create_distortion_effect(const std::string& name);
    int create_low_pass_filter(const std::string& name);
    int create_high_pass_filter(const std::string& name);
    int create_band_pass_filter(const std::string& name);
    int create_compressor_effect(const std::string& name);
    int create_limiter_effect(const std::string& name);
};

// Global audio system instance
extern std::unique_ptr<AudioSystem> g_audio_system;

// Native function declarations
void register_audio_functions(FunctionRegistry& registry);

} // namespace bas
