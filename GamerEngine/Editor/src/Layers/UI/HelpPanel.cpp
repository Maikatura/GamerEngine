#include "Editor.pch.h"
#include "HelpPanel.h"
#include "imgui_markdown/imgui_markdown.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Shellapi.h"
#include <string>

static ImGui::MarkdownConfig mdConfig;

static ImFont* H1 = NULL;
static ImFont* H2 = NULL;
static ImFont* H3 = NULL;

void LinkCallback(ImGui::MarkdownLinkCallbackData data_);
inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_);

void LinkCallback(ImGui::MarkdownLinkCallbackData data_)
{
    std::string url(data_.link, data_.linkLength);
    if(!data_.isImage)
    {
        ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
}


inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_)
{
    // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
    ImTextureID image = ImGui::GetIO().Fonts->TexID;
    // > C++14 can use ImGui::MarkdownImageData imageData{ true, false, image, ImVec2( 40.0f, 20.0f ) };
    ImGui::MarkdownImageData imageData;
    imageData.isValid = true;
    imageData.useLinkCallback = false;
    imageData.user_texture_id = image;
    imageData.size = ImVec2(40.0f, 20.0f);

    // For image resize when available size.x > image width, add
    ImVec2 const contentSize = ImGui::GetContentRegionAvail();
    if(imageData.size.x > contentSize.x)
    {
        float const ratio = imageData.size.y / imageData.size.x;
        imageData.size.x = contentSize.x;
        imageData.size.y = contentSize.x * ratio;
    }

    return imageData;
}


void MarkdownFormatCallback(const ImGui::MarkdownFormatInfo& markdownFormatInfo_, bool start_)
{
    // Call the default first so any settings can be overwritten by our implementation.
    // Alternatively could be called or not called in a switch statement on a case by case basis.
    // See defaultMarkdownFormatCallback definition for furhter examples of how to use it.
    ImGui::defaultMarkdownFormatCallback(markdownFormatInfo_, start_);

    switch(markdownFormatInfo_.type)
    {
        // example: change the colour of heading level 2
        case ImGui::MarkdownFormatType::HEADING:
        {
            if(markdownFormatInfo_.level == 2)
            {
                if(start_)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(51, 153, 255, 255));
                }
                else
                {
                    ImGui::PopStyleColor();
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void Markdown(const std::string& markdown_)
{
    // You can make your own Markdown function with your prefered string container and markdown config.
    // > C++14 can use ImGui::MarkdownConfig mdConfig{ LinkCallback, NULL, ImageCallback, ICON_FA_LINK, { { H1, true }, { H2, true }, { H3, false } }, NULL };
    mdConfig.linkCallback = LinkCallback;
    mdConfig.tooltipCallback = NULL;
    mdConfig.imageCallback = ImageCallback;
    mdConfig.linkIcon = ICON_FK_LINK;
    mdConfig.headingFormats[0] = { H1, true };
    mdConfig.headingFormats[1] = { H2, true };
    mdConfig.headingFormats[2] = { H3, false };
    mdConfig.userData = NULL;
    mdConfig.formatCallback = MarkdownFormatCallback;
    ImGui::Markdown(markdown_.c_str(), markdown_.length(), mdConfig);
}

void HelpPanel::OnAttach()
{
    Layer::OnAttach();

    //float fontSize = 12.0f;
    //
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->Clear();
    //// Base font
    //io.Fonts->AddFontFromFileTTF("Editor/fonts/Vera.ttf", fontSize);
    //// Bold headings H2 and H3
    //H2 = io.Fonts->AddFontFromFileTTF("Editor/fonts/Vera.ttf", fontSize);
    //H3 = mdConfig.headingFormats[1].font;
    //// bold heading H1
    //float fontSizeH1 = fontSize * 1.1f;
    //H1 = io.Fonts->AddFontFromFileTTF("Editor/fonts/Vera.ttf", fontSizeH1);
}

bool HelpPanel::OnImGuiRender()
{
    


   



	bool isOpen = true;
	ImGui::Begin("Help Panel", &isOpen);

    const std::string markdownText = R"([Made by](https://github.com/maikatura)
	# Here will be a documentation :)
	)";
    Markdown(markdownText);

	ImGui::End();
	return isOpen;
}
