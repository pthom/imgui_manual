// ImAnim Simple Demo - Showcases core animation features with simple, readable examples.
//
// Covers: tweens, easing, clips, oscillators, shake, wiggle, stagger, looping, chaining.

#include "imgui.h"
#include "im_anim.h"
#include <cmath>
#include <string>

#ifdef IMGUI_HAS_DEMO_MARKER_CALLBACK
#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback) GImGuiDemoMarkerCallback("im_anim_demo_basics", __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)
#else
#define IMGUI_DEMO_MARKER(section)
#endif

// ============================================================
// HELPER: Open/Close all collapsing headers and tree nodes
// ============================================================
static int s_open_all = 0;  // 0 = none, 1 = open all, -1 = close all

static void ApplyOpenAll()
{
    if (s_open_all != 0)
        ImGui::SetNextItemOpen(s_open_all > 0, ImGuiCond_Always);
}


// Show a tree node with the demo (no source code display in C++).
template<typename F>
void demo_header(const char* label, F demo_function)
{
    ApplyOpenAll();
    if (ImGui::TreeNodeEx(label)) {
        demo_function();
        ImGui::TreePop();
    }
}

// =============================================================================
// BASIC ANIMATIONS
// =============================================================================

void DemoTweenFloat()
{
    IMGUI_DEMO_MARKER("Basic Animations/Tween Float");
    ImGui::TextWrapped("Smoothly animate a float toward a target. Drag the slider while animating to see crossfade blending.");
    static float target = 50.0f;
    float dt = ImGui::GetIO().DeltaTime;

    ImGui::SliderFloat("Target", &target, 0.0f, 100.0f);

    float duration = 1.0f;
    float value = iam_tween_float(
        /*id*/ ImGui::GetID("float_demo"), /*channel_id*/ 0,
        target, duration,
        iam_ease_preset(iam_ease_out_cubic),
        iam_policy_crossfade, dt
    );
    ImGui::ProgressBar(value / 100.0f, ImVec2(-1, 0), "");
    ImGui::SameLine();
    ImGui::Text("%.1f", value);
}


void DemoColorTween()
{
    IMGUI_DEMO_MARKER("Basic Animations/Color Tween");
    ImGui::TextWrapped("Animate colors in perceptually uniform OKLAB space.");
    static int color_idx = 0;

    ImVec4 colors[] = {
        ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
        ImVec4(0.3f, 1.0f, 0.3f, 1.0f),
        ImVec4(0.3f, 0.3f, 1.0f, 1.0f),
        ImVec4(1.0f, 1.0f, 0.3f, 1.0f),
    };

    if (ImGui::Button("Next Color"))
        color_idx = (color_idx + 1) % 4;

    float dt = ImGui::GetIO().DeltaTime;
    float duration = 0.5f;
    ImVec4 color = iam_tween_color(
        /*id*/ ImGui::GetID("color"), /*channel_id*/ 0,
        colors[color_idx], duration,
        iam_ease_preset(iam_ease_out_cubic),
        iam_policy_crossfade, iam_col_oklab, dt
    );
    ImGui::SameLine();
    ImGui::ColorButton("##c", color, 0, ImVec2(100, 0));
}


void DemoOscillator()
{
    IMGUI_DEMO_MARKER("Basic Animations/Oscillator");
    ImGui::TextWrapped("Continuous sine wave for pulse/glow effects. Always active, no trigger needed.");
    float dt = ImGui::GetIO().DeltaTime;
    float amplitude = 0.3f;
    float frequency = 1.5f;
    float pulse = 1.0f + iam_oscillate(
        ImGui::GetID("pulse"), amplitude, frequency, iam_wave_sine, /*phase*/ 0.0f, dt
    );
    ImGui::Text("Pulsing:");
    ImGui::SameLine();
    ImGui::Button("Pulse!", ImVec2(80 * pulse, 30));
}


void DemoShake()
{
    IMGUI_DEMO_MARKER("Basic Animations/Shake");
    ImGui::TextWrapped("Triggered decaying shake for error/impact feedback. Click to trigger, then it decays to zero.");
    ImGuiID shake_id = ImGui::GetID("shake");

    if (ImGui::Button("Trigger Shake"))
        iam_trigger_shake(shake_id);

    float dt = ImGui::GetIO().DeltaTime;
    float intensity = 10.0f;
    float frequency = 30.0f;
    float decay_time = 0.5f;
    float offset = iam_shake(shake_id, intensity, frequency, decay_time, dt);
    ImGui::SameLine();
    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + offset, cursor.y));
    ImGui::Text("< Shaking!");
}


void DemoWiggle()
{
    IMGUI_DEMO_MARKER("Basic Animations/Wiggle");
    ImGui::TextWrapped("Continuous smooth random movement (noise-based). Unlike shake, it never stops. Useful for ambient/idle effects.");
    float dt = ImGui::GetIO().DeltaTime;
    float amplitude = 8.0f;
    float frequency = 3.0f;
    float offset = iam_wiggle(ImGui::GetID("wiggle"), amplitude, frequency, dt);
    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + offset, cursor.y));
    ImGui::Text("~ Wobbling!");
}


void DemoEasingShowcase()
{
    IMGUI_DEMO_MARKER("Basic Animations/Easing Showcase");
    ImGui::TextWrapped("Compare easing curves side by side. Each bar uses a different easing function on the same time value.");
    static float time = 0.0f;

    float dt = ImGui::GetIO().DeltaTime;
    time += dt;
    float cycle_duration = 2.0f;
    float t = fmodf(time, cycle_duration) / cycle_duration;  // 0..1 looping

    if (ImGui::Button("Restart"))
        time = 0.0f;

    float bar_width = ImGui::GetContentRegionAvail().x - 100.0f;
    struct { const char* name; int type; } easings[] = {
        { "Out Cubic",    iam_ease_out_cubic },
        { "Out Bounce",   iam_ease_out_bounce },
        { "Out Elastic",  iam_ease_out_elastic },
        { "Out Back",     iam_ease_out_back },
        { "In Out Sine",  iam_ease_in_out_sine },
    };
    for (auto& e : easings) {
        float value = iam_eval_preset(e.type, t);
        ImGui::ProgressBar(value, ImVec2(bar_width, 0), "");
        ImGui::SameLine();
        ImGui::Text("%s", e.name);
    }
}


// =============================================================================
// CLIPS (Timeline-based animations)
// =============================================================================

void DemoClipDelay()
{
    IMGUI_DEMO_MARKER("Clips/Delay");
    ImGui::TextWrapped("Clip with a 1-second delay before animation starts.");
    static bool init = false;

    auto id_ch = ImGui::GetID("ch");
    if (!init) {
        iam_clip::begin(ImGui::GetID("delay_clip"))
            .key_float(id_ch, /*time*/ 0.0f, /*value*/ 0.0f, iam_ease_out_cubic)
            .key_float(id_ch, /*time*/ 1.0f, /*value*/ 1.0f)
            .set_delay(1.0f)
            .end();
        init = true;
    }

    if (ImGui::Button("Play (1s delay)"))
        iam_play(ImGui::GetID("delay_clip"), ImGui::GetID("delay_inst"));

    iam_instance inst = iam_get_instance(ImGui::GetID("delay_inst"));
    float value = 0.0f;
    if (inst.valid()) inst.get_float(id_ch, &value);
    ImGui::ProgressBar(value, ImVec2(-1, 20), "");
}


void DemoClipCallback()
{
    IMGUI_DEMO_MARKER("Clips/Callbacks");
    ImGui::TextWrapped("Clip with on_begin and on_complete callbacks. Counters increment each time.");
    static bool init = false;
    static int begin_count = 0, complete_count = 0;

    auto id_ch_scale = ImGui::GetID("ch_scale");
    auto id_clip = ImGui::GetID("cb_clip");
    auto id_inst = ImGui::GetID("cb_inst");

    if (!init) {
        iam_clip::begin(id_clip)
            .key_float(id_ch_scale, /*time*/ 0.0f, /*value*/ 0.5f, iam_ease_out_back)
            .key_float(id_ch_scale, /*time*/ 0.5f, /*value*/ 1.2f)
            .key_float(id_ch_scale, /*time*/ 1.0f, /*value*/ 1.0f, iam_ease_in_out_sine)
#ifdef IMGUI_BUNDLE_PYTHON_API
            .on_begin([](ImGuiID) { begin_count++; })
            .on_complete([](ImGuiID) { complete_count++; })
#else
            .on_begin([](ImGuiID, void*) { begin_count++; })
            .on_complete([](ImGuiID, void*) { complete_count++; })
#endif
            .end();
        init = true;
    }

    if (ImGui::Button("Play"))
        iam_play(id_clip, id_inst);

    iam_instance inst = iam_get_instance(id_inst);
    float scale = 1.0f;
    if (inst.valid()) inst.get_float(id_ch_scale, &scale);
    ImGui::SameLine();
    ImGui::Button("Animated", ImVec2(80 * scale, 30));
    ImGui::Text("on_begin: %d, on_complete: %d", begin_count, complete_count);
}


void DemoStagger()
{
    IMGUI_DEMO_MARKER("Clips/Stagger");
    ImGui::TextWrapped("Cascading animation: each element starts with an incremental delay.");
    static bool init = false;
    const int N = 5;

    auto id_clip = ImGui::GetID("stagger_clip");
    auto id_ch = ImGui::GetID("stagger_ch");
    if (!init) {
        iam_clip::begin(id_clip)
            .key_float(id_ch, /*time*/ 0.0f, /*value*/ 0.0f, iam_ease_out_back)
            .key_float(id_ch, /*time*/ 0.5f, /*value*/ 1.0f)
            .set_stagger(N, /*each_delay*/ 0.15f, /*from_center_bias*/ 0.0f)
            .end();
        init = true;
    }

    if (ImGui::Button("Play Stagger")) {
        for (int i = 0; i < N; i++)
            iam_play_stagger(id_clip, ImGui::GetID(std::to_string(i).c_str()), i);
    }

    for (int i = 0; i < N; i++) {
        iam_instance inst = iam_get_instance(ImGui::GetID(std::to_string(i).c_str()));
        float val = 0.0f;
        if (inst.valid()) inst.get_float(id_ch, &val);
        if (i > 0) ImGui::SameLine();
        ImGui::Button(std::to_string(i).c_str(), ImVec2(30, 30 + 20 * val));
    }
}


void DemoClipLoop()
{
    IMGUI_DEMO_MARKER("Clips/Looping");
    ImGui::TextWrapped("Ping-pong looping: the animation bounces back and forth indefinitely.");
    static bool init = false;

    auto id_ch = ImGui::GetID("loop_ch");
    if (!init) {
        iam_clip::begin(ImGui::GetID("loop_clip"))
            .key_float(id_ch, /*time*/ 0.0f, /*value*/ 0.0f, iam_ease_in_out_sine)
            .key_float(id_ch, /*time*/ 1.0f, /*value*/ 1.0f)
            .set_loop(/*loop*/ true, /*direction*/ iam_dir_alternate)
            .end();
        init = true;
    }

    if (ImGui::Button("Play Loop"))
        iam_play(ImGui::GetID("loop_clip"), ImGui::GetID("loop_inst"));

    iam_instance inst = iam_get_instance(ImGui::GetID("loop_inst"));
    float value = 0.0f;
    if (inst.valid()) inst.get_float(id_ch, &value);
    ImGui::ProgressBar(value, ImVec2(-1, 20), "");
}


void DemoClipChain()
{
    IMGUI_DEMO_MARKER("Clips/Chaining");
    ImGui::TextWrapped("Two clips chained: A scales up, then B scales back down automatically.");
    static bool init = false;
    auto id_ch = ImGui::GetID("chain_ch");
    auto id_clip_a = ImGui::GetID("chain_a");
    auto id_clip_b = ImGui::GetID("chain_b");

    if (!init) {
        // Clip A: scale 1.0 -> 1.5 (overshoot with ease_out_back)
        iam_clip::begin(id_clip_a)
            .key_float(id_ch, /*time*/ 0.0f, /*value*/ 1.0f, iam_ease_out_back)
            .key_float(id_ch, /*time*/ 0.5f, /*value*/ 1.5f)
            .end();
        // Clip B: scale 1.5 -> 1.0 (smooth return)
        iam_clip::begin(id_clip_b)
            .key_float(id_ch, /*time*/ 0.0f, /*value*/ 1.5f, iam_ease_in_out_sine)
            .key_float(id_ch, /*time*/ 0.5f, /*value*/ 1.0f)
            .end();
        init = true;
    }

    if (ImGui::Button("Play A -> B"))
        iam_play(id_clip_a, ImGui::GetID("chain_inst")).then(id_clip_b);

    iam_instance inst = iam_get_instance(ImGui::GetID("chain_inst"));
    float scale = 1.0f;
    if (inst.valid()) inst.get_float(id_ch, &scale);
    ImGui::SameLine();
    ImGui::Button("Animated", ImVec2(80 * scale, 30));
}


// =============================================================================
// MAIN GUI
// =============================================================================
void ImAnimDemoBasicsWindow(bool create_window)
{
    if (create_window) {
        ImGui::SetNextWindowSize(ImVec2(650, 750), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("ImAnim Demo - Basics")) {
            ImGui::End();
            return;
        }
    }

    // Open/Close all sections (uses global s_open_all)
    s_open_all = 0;
    if (ImGui::Button("Open All"))
        s_open_all = 1;
    ImGui::SameLine();
    if (ImGui::Button("Close All"))
        s_open_all = -1;

    ImGui::Separator();

    ApplyOpenAll();
    if (ImGui::CollapsingHeader("Basic Animations")) {
        demo_header("Tween Float", DemoTweenFloat);
        demo_header("Color Tween (OKLAB)", DemoColorTween);
        demo_header("Oscillator", DemoOscillator);
        demo_header("Shake", DemoShake);
        demo_header("Wiggle", DemoWiggle);
        demo_header("Easing Showcase", DemoEasingShowcase);
    }

	ApplyOpenAll();
    if (ImGui::CollapsingHeader("Clips (Timeline)")) {
        demo_header("Delay", DemoClipDelay);
        demo_header("Callbacks", DemoClipCallback);
        demo_header("Stagger", DemoStagger);
        demo_header("Looping", DemoClipLoop);
        demo_header("Chaining", DemoClipChain);
    }

    if (create_window)
        ImGui::End();
}


// #ifndef IMGUI_BUNDLE_BUILD_DEMO_AS_LIBRARY
// #include "immapp/runner.h"
// #include "hello_imgui/hello_imgui.h"
// int main()
// {
//     HelloImGui::RunnerParams runner_params;
//     runner_params.callbacks.ShowGui = []() { ImAnimDemoBasicsWindow(false); };
//     runner_params.appWindowParams.windowTitle = "ImAnim Demo - Basics";
//
//     ImmApp::AddOnsParams addons_params;
//     addons_params.withImAnim = true;
//
//     ImmApp::Run(runner_params, addons_params);
// }
// #endif
