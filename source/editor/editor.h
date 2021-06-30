#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "core/typedefs.h"
#include "core/input.h"
#include "core/window.h"
#include "utility/log.h"

namespace Mizery {

    class Editor
    {
    public:
        Editor(Window* window, bool32 showEditor);
    
        void processInput(Input& input);
        void render();
        void shutdown();
        
        void toggleEditor();
        
        inline bool32 isShowingEditor() const { return showEditor; }
    private:
        bool32 showEditor;
        Window* window;
    };

}