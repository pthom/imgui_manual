#include "Common_ExampleApp.h"

// The code below was copy-pasted verbatim from imgui_demo.cpp
// Do not edit!!!
//-----------------------------------------------------------------------------
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
//-----------------------------------------------------------------------------

static void ShowDummyObject(const char* prefix, int uid)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(uid);
    ImGui::AlignTextToFramePadding();   // Text and Tree nodes are less high than framed widgets, here we add vertical spacing to make the tree lines equal high.
    bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
    ImGui::NextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("my sailor is rich");
    ImGui::NextColumn();
    if (node_open)
    {
        static float dummy_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
        for (int i = 0; i < 8; i++)
        {
            ImGui::PushID(i); // Use field index as identifier.
            if (i < 2)
            {
                ShowDummyObject("Child", 424242);
            }
            else
            {
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                ImGui::TreeNodeEx("Field", flags, "Field_%d", i);
                ImGui::NextColumn();
                ImGui::SetNextItemWidth(-1);
                if (i >= 5)
                    ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
                else
                    ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
                ImGui::NextColumn();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

// Demonstrate create a simple property editor.
static void ShowExampleAppPropertyEditor(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Example: Property editor", p_open))
    {
        ImGui::End();
        return;
    }
    DemoCode("Examples/Property Editor");


    HelpMarker(
        "This example shows how you may implement a property editor using two columns.\n"
        "All objects/fields data are dummies here.\n"
        "Remember that in many simple cases, you can use ImGui::SameLine(xxx) to position\n"
        "your cursor horizontally instead of using the Columns() API.");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    ImGui::Columns(2);
    ImGui::Separator();

    // Iterate dummy objects with dummy members (all the same data)
    for (int obj_i = 0; obj_i < 3; obj_i++)
        ShowDummyObject("Object", obj_i);

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
}


void Playground()
{
    ShowExampleAppPropertyEditor(nullptr);
}

