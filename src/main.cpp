#include <wx/wx.h>

#include <random>

#include "drawingcanvas.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    DrawingCanvas *canvas;

    int rectCount = 0;
    std::mt19937 randomGen;

    void OnAddButtonClick(wxCommandEvent &event);
    void OnRemoveButtonClick(wxCommandEvent &event);
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel *buttonPanel = new wxPanel(this);
    wxButton *addRectButton = new wxButton(buttonPanel, wxID_ANY, "Add Rect");
    wxButton *removeLastButton = new wxButton(buttonPanel, wxID_ANY, "Remove Top");
    wxButton *zoomInButton = new wxButton(buttonPanel, wxID_ANY, "Zoom In");
    wxButton *zoomOutButton = new wxButton(buttonPanel, wxID_ANY, "Zoom Out");

    wxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(addRectButton, 0, wxEXPAND | wxALL, FromDIP(3));
    buttonSizer->Add(removeLastButton, 0, wxEXPAND | wxALL, FromDIP(3));
    buttonSizer->Add(zoomInButton, 0, wxEXPAND | wxALL, FromDIP(3));
    buttonSizer->Add(zoomOutButton, 0, wxEXPAND | wxALL, FromDIP(3));

    buttonPanel->SetSizer(buttonSizer);

    canvas = new DrawingCanvas(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    sizer->Add(canvas, 1, wxEXPAND | wxALL, 0);
    sizer->Add(buttonPanel, 0, wxEXPAND | wxALL, 0);

    this->SetSizer(sizer);

    addRectButton->Bind(wxEVT_BUTTON, &MyFrame::OnAddButtonClick, this);
    removeLastButton->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveButtonClick, this);
    zoomInButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                       { canvas->ZoomIn(); });
    zoomOutButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                        { canvas->ZoomOut(); });
}

void MyFrame::OnAddButtonClick(wxCommandEvent &event)
{
    std::uniform_int_distribution sizeDistrib(this->FromDIP(50), this->FromDIP(100));
    std::uniform_int_distribution xDistrib(
        canvas->GetCanvasBounds().GetLeft(),
        canvas->GetCanvasBounds().GetRight());
    std::uniform_int_distribution yDistrib(
        canvas->GetCanvasBounds().GetTop(),
        canvas->GetCanvasBounds().GetBottom());
    std::uniform_real_distribution angleDistrib(0.0, M_PI * 2.0);

    std::uniform_int_distribution colorDistrib(0, 0xFFFFFF);

    rectCount++;
    canvas->AddRect(sizeDistrib(randomGen), sizeDistrib(randomGen), xDistrib(randomGen), yDistrib(randomGen),
                    angleDistrib(randomGen), wxColour(colorDistrib(randomGen)), "Rect #" + std::to_string(rectCount));
}

void MyFrame::OnRemoveButtonClick(wxCommandEvent &event)
{
    canvas->RemoveTopRect();
}