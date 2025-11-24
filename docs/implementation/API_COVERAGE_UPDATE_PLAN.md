# API.MD Coverage Update Plan

## Summary

After analyzing `API.MD`, I've identified **200+ missing functions** that need to be added to complete full Raylib API coverage.

## Current Status

- **Existing Functions**: 527+ functions already registered
- **Missing Functions**: 200+ functions identified
- **Total Target**: 727+ functions for complete coverage

## Implementation Strategy

Given the large number of missing functions, I recommend a **phased approach**:

### Phase 1: High-Priority Functions (Immediate)
These are commonly used functions that should be added first:

1. **Window Functions**
   - `SetWindowSize()` - Set window dimensions
   - `GetWindowPosition()` - Get window position
   - `GetWindowScaleDPI()` - Get DPI scale

2. **Input Functions**
   - `IsKeyPressedRepeat()` - Check if key pressed again
   - `GetCharPressed()` - Get char pressed (unicode)
   - `SetGamepadVibration()` - Set gamepad vibration
   - `GetMouseWheelMoveV()` - Get mouse wheel movement (both axes)

3. **Shape Functions**
   - `DrawLineStrip()` - Draw lines sequence
   - `DrawCircleLinesV()` - Draw circle outline (Vector version)
   - `DrawRectanglePro()` - Draw rectangle with pro parameters
   - `DrawRectangleGradientV/H/Ex()` - Draw gradient rectangles
   - `GetCollisionRec()` - Get collision rectangle

4. **Image Functions**
   - `LoadImageFromScreen()` - Load image from screen (screenshot)
   - `IsImageValid()` - Check if image is valid
   - `GenImageGradientLinear/Radial/Square()` - Generate gradient images
   - `ImageCopy()` - Create image duplicate
   - `ImageResizeNN()` - Resize image (Nearest-Neighbor)

5. **Color Functions**
   - `ColorToHSV()` / `ColorFromHSV()` - HSV color conversion
   - `ColorLerp()` - Color interpolation
   - `GetColor()` - Get Color from hex value

6. **Text Functions**
   - `GetFontDefault()` - Get default font
   - `TextFormat()` - Text formatting (sprintf style)
   - `TextToUpper/Lower()` - Text case conversion
   - `TextToInteger/Float()` - Text to number conversion

7. **3D Model Functions**
   - `DrawCapsule()` / `DrawCapsuleWires()` - Draw capsule
   - `DrawBillboard()` / `DrawBillboardRec()` / `DrawBillboardPro()` - Draw billboards
   - `IsModelValid()` - Check if model is valid
   - `GetModelBoundingBox()` - Get model bounding box

8. **Audio Functions**
   - `GetMasterVolume()` - Get master volume
   - `IsSoundValid()` / `IsWaveValid()` / `IsMusicValid()` - Validation functions
   - `WaveCopy()` / `WaveCrop()` / `WaveFormat()` - Wave manipulation

9. **Raymath Functions**
   - `Vector2/3Zero()` / `Vector2/3One()` - Vector constants
   - `Vector2/3Add/Subtract/Multiply/Divide()` - Vector operations
   - `Vector2/3Length/Distance/DotProduct()` - Vector math
   - `Vector2/3Normalize/Lerp()` - Vector utilities
   - `MatrixIdentity/Invert/Transpose()` - Matrix operations
   - `QuaternionIdentity/Normalize/Lerp/Slerp()` - Quaternion operations

### Phase 2: Advanced Functions (Next)
These are more specialized functions:

1. **Spline Functions** - All spline drawing and evaluation functions
2. **Image Drawing Functions** - CPU-based image rendering functions
3. **Advanced Collision** - Point-in-polygon, line-line collision, etc.
4. **Shader Functions** - Advanced shader operations
5. **Animation Functions** - Model animation functions
6. **Audio Stream Functions** - Advanced audio stream operations

### Phase 3: Specialized Functions (Later)
These are niche or advanced functions:

1. **VR Functions** - VR stereo rendering
2. **Automation Functions** - Automation event recording/playback
3. **Callback Functions** - Custom callback setup
4. **Advanced Text Functions** - UTF-8 codepoint management
5. **Advanced Image Functions** - Image manipulation (blur, dither, etc.)

## Implementation Steps

1. **Create YAML Spec Files**
   - `specs/missing_raylib_functions.yaml` - Phase 1 functions (created)
   - `specs/missing_raylib_functions_phase2.yaml` - Phase 2 functions
   - `specs/missing_raylib_functions_phase3.yaml` - Phase 3 functions

2. **Update Generator**
   - Add new YAML files to generator script (done)
   - Test generation

3. **Regenerate Bindings**
   - Run generator to create new bindings
   - Verify compilation

4. **Update Namespace Registry**
   - Add new functions to appropriate namespaces
   - Organize by module

5. **Test Functions**
   - Create test programs for new functions
   - Verify functionality

## Files Created

1. `specs/missing_raylib_functions.yaml` - Initial missing functions (partial)
2. `API_MISSING_FUNCTIONS_ANALYSIS.md` - Complete analysis of all missing functions
3. `API_COVERAGE_UPDATE_PLAN.md` - This file (implementation plan)

## Next Steps

1. Complete `specs/missing_raylib_functions.yaml` with Phase 1 functions
2. Update generator to include the new YAML file (done)
3. Regenerate bindings
4. Test compilation
5. Update namespace registry
6. Create example programs

## Notes

- Some functions require complex parameter handling (arrays, callbacks, etc.)
- These may need custom implementations or simplified versions
- Priority should be given to commonly used functions
- Advanced functions can be added incrementally

