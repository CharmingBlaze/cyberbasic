# Recommended Next Steps for Language Improvement

Based on the current state of CyberBasic, here are the most impactful improvements we should tackle next:

## 🎯 Top 3 Priorities

### 1. Object Methods (Highest Impact)
**Status:** Not implemented  
**Priority:** ⭐⭐⭐  
**Effort:** Medium (1-2 days)

**What it enables:**
```basic
LET vec = Vector3(3, 4, 0)
LET len = vec.length()           REM Returns 5.0
LET norm = vec.normalize()      REM Returns Vector3(0.6, 0.8, 0)
LET dot = vec.dot(otherVec)     REM Dot product
LET cross = vec.cross(otherVec) REM Cross product

LET color = Color(255, 128, 0, 255)
LET brightness = color.brightness() REM Calculate brightness
LET darker = color.darken(0.5)     REM Darken by 50%
```

**Why it matters:**
- Makes objects actually useful, not just data containers
- More intuitive than global functions
- Enables method chaining: `vec.normalize().scale(2.0)`

**Implementation approach:**
- Add method objects to built-in types
- Enhance interpreter to call methods on objects
- Register methods in object constructors

---

### 2. Better Error Messages (Quick Win)
**Status:** Basic errors exist, but dot notation errors are cryptic  
**Priority:** ⭐⭐⭐  
**Effort:** Low (1 day)

**Current problems:**
```basic
REM Current: Silent failure or "unknown expr"
Window.init()  REM Fails silently or cryptic error
vec.xyz        REM No helpful message
```

**What we need:**
```basic
REM Better: Clear, helpful errors
ERROR at line 5, column 10:
  Method 'init' not found in namespace 'Window'
  Did you mean: 'Window.init(width, height, title)'?
  Available methods: init, close, setTitle, getFPS, ...

ERROR at line 10, column 5:
  Property 'xyz' not found on Vector3
  Available properties: x, y, z
  Available methods: length(), normalize(), dot(), cross()
```

**Why it matters:**
- Dramatically improves developer experience
- Reduces debugging time
- Makes language more approachable

---

### 3. Example Programs (Showcase Features)
**Status:** No examples using new features  
**Priority:** ⭐⭐  
**Effort:** Low (1 day)

**What to create:**
1. **dot_notation_demo.bas** - Showcase all dot notation features
2. **game_template.bas** - Complete game using modern syntax
3. **object_methods.bas** - Demonstrate object capabilities

**Why it matters:**
- Shows developers how to use features
- Serves as documentation
- Demonstrates best practices

---

## 🔧 Medium Priority

### 4. REPL Mode (Interactive Development)
**Priority:** ⭐⭐  
**Effort:** High (3-5 days)

Interactive shell for testing code quickly:
```bash
$ cyberbasic --repl
> LET vec = Vector3(1, 2, 3)
> vec.length()
5.0
> Window.init(800, 600, "Test")
```

### 5. More Object Types
**Priority:** ⭐  
**Effort:** Medium

Add objects for:
- `Texture2D` - with methods like `draw()`, `getWidth()`, `getHeight()`
- `Model` - with methods like `draw()`, `getBoundingBox()`
- `Sound` - with methods like `play()`, `stop()`, `setVolume()`

---

## 📊 Impact vs Effort Matrix

```
High Impact
    │
    │  [1] Object Methods
    │  [2] Better Errors
    │
    │  [3] Examples
    │
    │              [4] REPL
    │
    │                    [5] More Objects
    │
Low Impact───────────────────────────────High Effort
                Low Effort
```

---

## 🚀 Recommended Action Plan

### Week 1: Quick Wins
1. **Day 1:** Better error messages for dot notation
2. **Day 2:** Create 2-3 example programs
3. **Day 3-4:** Implement object methods (Vector3, Vector2, Color)

### Week 2: Polish
4. **Day 1-2:** More object methods (cross, dot, etc.)
5. **Day 3:** Add more object types (Texture2D, etc.)
6. **Day 4-5:** REPL mode (if time permits)

---

## 💡 Why These Priorities?

1. **Object Methods** - Transforms the language from "has objects" to "objects are useful"
2. **Better Errors** - Makes the language actually usable for development
3. **Examples** - Shows what's possible and how to use features

These three improvements together will make CyberBasic significantly more powerful and user-friendly!

