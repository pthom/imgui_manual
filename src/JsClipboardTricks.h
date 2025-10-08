#pragma once

// The clipboard handling features take inspiration from sokol
// https://github.com/floooh/sokol

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
