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
    auto panel = new wxPanel(this, wxID_ANY);

    const int WIDTH = FromDIP(30);
    const int HEIGHT = FromDIP(30);

    const int COLS = 20;
    const int ROWS = 15;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            auto square = new wxPanel(panel, wxID_ANY, wxPoint(j * WIDTH, i * HEIGHT), wxSize(WIDTH, HEIGHT));
            bool isDark = i % 2 == j % 2;
            square->SetBackgroundColour(isDark ? *wxBLACK : *wxWHITE);

            square->Bind(wxEVT_LEFT_DOWN, [square, isDark](wxMouseEvent &event)
                         {
                             square->SetBackgroundColour(isDark ? wxColour(200, 100, 100, 128) : wxColour(100, 200, 100));
                             square->Refresh();
                         });
        }
    }
}