#pragma once

// The clipboard handling features take inspiration from sokol
// https://github.com/floooh/sokol
//
// Unused since we switched to glfw3 via pongasoft/emscripten-glfw
// which has clipboard support built-in (see https://github.com/ocornut/imgui/issues/7851)

#ifdef __EMSCRIPTEN__
#define IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER
// #define IMGUIMANUAL_CLIPBOARD_IMPORT_FROM_BROWSER # This still needs work...
#endif


#ifdef IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER
void JsClipboard_InstallPlaformSetClipboardText();
#endif

#ifdef IMGUIMANUAL_CLIPBOARD_IMPORT_FROM_BROWSER
void JsClipboard_AddJsHook();
#endif
