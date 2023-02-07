#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    auto panel = new wxScrolled<wxPanel>(this, wxID_ANY);
    panel->SetScrollRate(0, FromDIP(10));

    auto sizer = new wxBoxSizer(wxVERTICAL);

    for (int i = 0; i < 15; i++)
    {
        auto button = new wxButton(panel, wxID_ANY, "Button " + std::to_string(i));
        sizer->Add(button, 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    }

    panel->SetSizer(sizer);
}